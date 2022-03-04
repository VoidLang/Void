#include "Method.hpp"
#include "../../util/Strings.hpp"
#include "../Modifier.hpp"
#include "../runtime/StackElement.hpp"
#include "../runtime/StorageElement.hpp"

namespace Void
{
	/**
	 * Initialize method.
	 */
	Method::Method(STRING name, STRING returnType, LIST modifiers, LIST parameters, Factory* factory, Class* clazz) // : Executable(factory)
	{
		this->name = name;
		this->returnType = returnType;
		this->modifiers = getModifiers(modifiers);
		this->parameters = parameters;
		this->factory = factory;
		this->clazz = clazz;
	}

	/**
	 * Build method content.
	 */
	void Method::build(LIST bytecode)
	{
		this->bytecode = bytecode;

		for (SIZE_T i = 0; i < bytecode.size(); i++)
		{
			STRING line = bytecode[i];

			// split up string 

			LIST args;
			Strings::split(line, args, ' ');

			STRING instruction = args[0];

			// handle sections

			if (instruction[0] == ':')
			{
				// get the name of the section

				STRING section = instruction.substr(1, instruction.length());

				// check if section already exists

				if (hasSection(section) && !hasOption("XNoWarnings") && !hasOption("XNoSectionWarns"))
				{
					println("[Warning] Duplicate section '" << section << "'");
				}
				
				// set section

				sections[section] = i;
			}

			// handle linkers

			else if (instruction == "#link")
			{
				// get the name of the linker

				STRING linker = args[1];

				// check if linker already exists

				bool noWarning = hasOption("XNoWarnings");
				bool noLinkerWarns = hasOption("XNoLinkerWarns");

				if (hasLinker(linker) && !noWarning && !noLinkerWarns)
				{
					println("[Warning] Duplicate linker '" << linker << "'");
				}

				try
				{
					int value = stoi(args[2]);

					// check if linker value already exists

					if (hasLinkerValue(value) && !noWarning && !noLinkerWarns && !hasOption("XNoDuplicateLinkerWarns"))
					{
						println("[Warning] Duplicate value for linker '" << linker << "' = " << value);
					}

					// set linker

					linkers[linker] = value;
				}
				catch (...)
				{
					if (!noWarning && !noLinkerWarns && !hasOption("XNoInvalidLinkerWarns"))
					{
						println("[Warning] Invalid value for linker '" << linker << "' = " << args[1]);
					}
				}
			}
		}
	}

	/**
	 * Determine if the option is set.
	 */
	bool Method::hasOption(STRING option)
	{
		return factory->options.has(option);
	}

	/**
     * Perform a method call.
	 */
	void Method::invoke(Factory* factory, Stack* stackSrc, Instance* instance, Executable* caller)
	{
		// create data holders

		Stack* stack = new Stack(stackSrc, this->clazz->name + "." + this->name + "(" + Strings::join(parameters, ", ") + ")");
		Storage* storage = new Storage();

		// create storage indexes
		
		int acount = 0; // instance
		int icount = 0; // int
		int fcount = 0; // float
	
		// prepare for a non-static call

		if (instance != nullptr)
		{
			storage->instanceStorage.set(0, instance);
			acount++; // skip 0 because that's where the instance is stored
		}

		// copy elements from caller's stack to the current storage

		for (STRING parameter : parameters)
		{
			// parameter is an array
			if (parameter[0] == '[')
			{
				Instance* instance = stackSrc->instanceStack.pull();
				
				if (!instanceof(instance, BaseArray))
				{
					error("Expected an array, but got: " << instance->getType());
				}

				BaseArray* array = dynamic_cast<BaseArray*>(instance);

				if (array->type != parameter.substr(1, parameter.length()))
				{
					println("IllegalArgumentException: " << parameter << " expected, but got [" << array->type);
				}

				storage->instanceStorage.set(acount++, instance);
			}

			// parameter is an instance

			else if (parameter[0] == 'L')
			{
				Instance* instance = stackSrc->instanceStack.pull();
				storage->instanceStorage.set(acount++, instance);
			}

			// paremeter is an integer

			else if (parameter == "I")
			{
				int value = stackSrc->intStack.pull();
				storage->intStorage.set(icount++, value);
			}

			// parameter is a float

			else if (parameter == "F")
			{
				float value = stackSrc->floatStack.pull();
				storage->floatStorage.set(fcount++, value);
			}
		}

		// execute bytecode, get result

		ANY result;

		// handle native methods
		// but keep in mind they dont have a body
		// which means you either call a vm-level 
		// override or a dynamic library implementation

		if (hasModifier(Modifier::NATIVE))
		{
			override(factory, stack, storage, instance, caller, this, result);
		}

		else
		{
			execute(factory, stack, storage, bytecode, result, this);
		}

		// put the result onto the caller's stack

		// return type is an instance

		if (returnType[0] == 'L')
		{
			if (strcmp(result.type().name(), "std::nullptr_t") == 0)
			{
				stackSrc->instanceStack.push(nullptr);
			}
			else
			{
				Instance* instance = ANY_CAST<Instance*>(result);
				stackSrc->instanceStack.push(instance);
			}
		}
		
		// return type if an integer

		else if (returnType == "I")
		{
			int value = ANY_CAST<int>(result);
			stackSrc->intStack.push(value);
		}

		// return type is a float

		else if (returnType == "F")
		{
			float value = std::any_cast<float>(result);
			stackSrc->floatStack.push(value);
		}

		// TODO support other primitives (double, short, byte, boolean, etc)

		// TODO garbage collect
	}

	/**
	 * Determine if the field has modifier.
	 */
	bool Method::hasModifier(Modifier modifier)
	{
		int mod = static_cast<UNDERLYING_TYPE<Modifier>::type>(modifier);
		return (modifiers & mod) > 0;
	}


	/**
	 * Debug method and it's content.
	 */
	void Method::debug()
	{
		// debug method modifiers

		LIST modifiers = getModifiers(this->modifiers);

		print("    ");

		if (modifiers.size() > 0)
		{
			print(Strings::join(modifiers, " ") << " ");
		}

		// debug method return type and name

		print(returnType << " " << name);

		// debug method parameters

		print("(" << Strings::join(parameters, ", ") << ")");
	
		// check if method is native

		if (hasModifier(Modifier::NATIVE))
		{
			println(";");
			return;
		}

		// debug method body

		println(" {");

		for (int i = 0; i < bytecode.size(); i++)
		{
			println("        " << i << ": " << bytecode[i]);
		}

		println("    }");
	}

	/**
	 * Parse the native path of the method.
	 */
	STRING Method::nativePath()
	{
		// determine the native method path

		LIST pathList;

		pathList.push_back("Void");

		// append declaring class' name

		LIST name;

		Strings::split(clazz->name, name, '.');
		Lists::append(pathList, name);

		pathList.push_back(this->name);

		// append method parameters

		Lists::append(pathList, this->parameters);

		// get the final path

		return Strings::join(pathList, "_");
	}

	/**
	 * Get the native return type of the method.
	 */
	STRING Method::nativeReturnType()
	{
		if (returnType[0] == 'L')
		{
			return "VObject";
		}

		else if (returnType == "V")
		{
			return "void";
		}

		else if (returnType == "I")
		{
			return "int";
		}

		else if (returnType == "F")
		{
			return "float";
		}

		return "void";
	}
}