#include "Factory.hpp"
#include "../util/Strings.hpp"
#include "../util/Lists.hpp"
#include "Instruction.hpp"
#include "../util/Exceptions.hpp"

namespace Void
{
	class Class;

	/**
	 * Initializ factory.
	 */
	Factory::Factory(Options& options) : options(options)
	{
	}

	/**
	 * Build the factory.
	 * Make classes using bytecode.
	 */
	void Factory::build(LIST bytecode)
	{
		// the name of the current class

		STRING name = "";
		
		// the superclass of the current class

		STRING superclass = "void.lang.Object";

		// the modifiers of the current class

		LIST modifiers;

		// the list of interfaces of the current class

		LIST interfaces;

		// the content of the current class
		// to be handled by the class itself

		LIST content;

		// determines if the declaration of the current class' content has begun

		bool begun = false;

		// the index of classes inside the content
		// to be incremented by every 'cbegin'
		// to be decremented by every 'cend'

		int classIndex = 0;

		// handle bytecode

		for (STRING line : bytecode)
		{
			// split up the line 
			LIST args;
			Strings::split(line, args, ' ');

			STRING instruction = args[0];
		
			if (begun)
			{
				// handle class content end

				if (instruction == Instruction::CLASS_END && --classIndex == 0)
				{
					begun = false;

					// check if class already exists

					if (hasClass(name))
					{
						error("ClassRedefineException: Class '" + name + "' already exists.");
					}

					// create, define and initialize class
					
					Class* clazz = new Class(
						name, superclass,
						modifiers, interfaces,
						this
					);

					defineClass(clazz);
					// TODO build class content
					clazz->build(content);
					// clazz->initialize();

					// clear variables after declaration is done

					name = "";
					superclass = "void.lang.Object";
					
					modifiers.clear();
					interfaces.clear();
					content.clear();
				}

				else
				{
					if (instruction == Instruction::CLASS_BEGIN)
					{
						classIndex++;
					}
					// append line to class content

					content.push_back(line);
				}
			}

			// class definition start (determine class name)

			else if (instruction == Instruction::CLASS_DEFINE)
			{
				name = args[1];
			}

			// determine class modifiers

			else if (instruction == Instruction::CLASS_MODIFIER)
			{
				modifiers = Lists::subList(args, 1);
			}

			// determine class superclass

			else if (instruction == Instruction::CLASS_EXTENDS)
			{
				superclass = args[1];
			}

			// determine class 

			else if (instruction == Instruction::CLASS_IMPLEMENTS)
			{
				interfaces = Lists::subList(args, 1);
			}

			// handle class content begin

			else if (instruction == Instruction::CLASS_BEGIN && classIndex++ == 0)
			{
				begun = true;
			}
		}
	}

	/**
	 * Initialize static class fields.
	 * Call static constructors.
	 */
	void Factory::initialize(Stack* heap)
	{
		for (Class* clazz : classes)
		{
			clazz->initialize(heap);
		}
	}

	/**
	 * Determine if a class with the given name exists.
	 */
	bool Factory::hasClass(STRING name)
	{
		return getClass(name) != nullptr;
	}

	/**
	 * Get a class with the given name.
	 */
	Class* Factory::getClass(STRING name)
	{
		for (Class* clazz : classes)
		{
			if (clazz->name == name)
			{
				return clazz;
			}
		}
		return nullptr;
	}

	/**
	 * Define a class in the factory.
	 */
	void Factory::defineClass(Class* clazz)
	{
		classes.push_back(clazz);
	}

	/**
	 * Load a dynamic-link library and implement native methods for the given class.
	 */
	void Factory::loadLibrary(STRING name, Class* clazz)
	{
		// instantiate library

		// HINSTANCE library = LoadLibrary(L"C:\\Users\\Admin\\source\\repos\\Void-DLL\\x64\\Debug\\Void-DLL.dll");
		WSTRING temp = WSTRING(name.begin(), name.end());
		HINSTANCE library = LoadLibrary(temp.c_str());

		// check if library could not load

		if (!library)
		{
			print("InvalidLibraryException: ");

			DWORD message = ::GetLastError();
			if (message == 0)
			{ 
				error("Unable to load library"); 
			}
			else 
			{ 
				error("Unable to load library: " << SYSTEM_CATEGORY().message(message));
			}
		}

		// get the methods declared by the caller class

		for (Method* method : clazz->methods)
		{
			// make sure method is native

			if (!method->hasModifier(Modifier::NATIVE))
			{
				continue;
			}

			// get the method native path

			STRING path = method->nativePath();

			// resolve function address

			method->methodAddress = GetProcAddress(library, path.c_str());

			// VOID_FUNCTION function = (VOID_FUNCTION) GetProcAddress(library, path.c_str());
			if (!method->methodAddress) 
			{
				println("Unable to locate method");
			}

			// override native method body

			method->override = [](Factory* factory, Stack* stack, Storage* storage, Instance* instance, Executable* caller, Executable* self, ANY& result)
			{
				VNIEnv* env = new VNIEnv();

				Method* method = (Method*)self;

				SIZE_T size = method->parameters.size();

				int acount = 0; // instance
				int icount = 0; // int
				int fcount = 0; // float

				if (instance != nullptr)
				{
					acount++;
				}

				// TODO copy elements from stack src to VNIParams

				ANY* elements = new ANY[size];
				
				for (SIZE_T i = 0; i < size; i++)
				{
					STRING parameter = method->parameters[i];

					if (parameter[0] == '[' || parameter[0] == 'L')
					{
						elements[i] = storage->instanceStorage.get(acount++);
					}

					if (parameter == "I")
					{
						elements[i] = storage->intStorage.get(icount++);
					}

					else if (parameter == "F")
					{
						elements[i] = storage->floatStorage.get(fcount++);
					}
				}

				VNIParams* params = new VNIParams(elements, (int) size);

				FARPROC call = method->methodAddress;

				if (!method->hasModifier(Modifier::STATIC))
				{
					STRING type = method->returnType;

					if (type == "V")
					{
						println("handling void call");

						VOID_CALL voidCall = (VOID_CALL) call;

						voidCall(env, nullptr, params);
					}

					else if (type == "I")
					{
						println("handling int call");

						INT_CALL intCall = (INT_CALL) call;

						println("casted shit");

						int val = intCall(env, nullptr, params);

						println("loll " << val);

						result = val;
					
						println("int call handled");
					}
				}

				else
				{

				}
			};
		}
	}
}