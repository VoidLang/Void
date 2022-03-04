#include "Class.hpp"
#include "../Modifier.hpp"
#include "../../util/Strings.hpp"
#include "../../util/Lists.hpp"
#include "../Instruction.hpp"

namespace Void
{
    class Factory;

    /**
     * Initialize class.
     */
    Class::Class(STRING name, STRING superclass, LIST modifiers, LIST interfaces, Factory* factory)
    {
        this->name = name;
        this->superclass = superclass;

        this->modifiers = getModifiers(modifiers);
        this->interfaces = interfaces;

        this->factory = factory;
    }

    /**
     * Build class content.
     */
    void Class::build(LIST bytecode)
    {
        // the name of the current class

        STRING className = "";

        // the superclass of the current class

        STRING classSuperclass = "void.lang.Object";

        // the modifiers of the current class

        LIST classModifiers;

        // the list of interfaces of the current class

        LIST classInterfaces;

        // the name of the current method

        STRING methodName = "";

        // the return type of the current method

        STRING methodReturnType = "V";

        // the modifiers of the current method

        LIST methodModifiers;

        // the parameters of the current method

        LIST methodParameters;

        // the name of the current field

        STRING fieldName = "";

        // the modifiers of the current field

        LIST fieldModifiers;

        // the type of the current field

        STRING fieldType = "";

        // the content of the current object

        LIST content;
    
        // determines if the declaration of the current object's content has begun

        bool begun = false;

        // the index of classes inside the content
        // to be incremented by every 'cbegin'
        // to be decremented by every 'cend'

        int classIndex = 0;

        // the index of methods inside the content
        // to be incremented by every 'mbegin'
        // to be decremented by every 'mend'

        int methodIndex = 0;

        // the index of fields inside the content
        // to be incremented by every 'fbegin'
        // to be decremented by every 'fend'

        int fieldIndex = 0;

        // the type of the current declaration
        // class 0
        // method 1
        // field 2

        int type = -1;

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

                if (instruction == Instruction::CLASS_END && type == 0 && --classIndex == 0)
                {
                    // end declaration

                    begun = false;
                    type = -1;

                    // check if class is already defined

                    if (factory->hasClass(className))
                    {
                        error("ClassRedefineException: Class '" << className << "' already exists.");
                    }

                    // define class

                    Class* clazz = new Class(
                        className, classSuperclass,
                        classModifiers, classInterfaces,
                        factory
                    );

                    factory->defineClass(clazz);
                    clazz->build(content);
                    // clazz->initialize();

                    // clear variables after declaration is done

                    className = "";
                    classSuperclass = "void.lang.Object";

                    classModifiers.clear();
                    classInterfaces.clear();
                    content.clear();
                }

                else if (instruction == Instruction::METHOD_END && type == 1 && --methodIndex == 0)
                {
                    // end declaration

                    begun = false;
                    type = -1;

                    // check if method is already dafined

                    if (hasMethod(methodName, methodParameters))
                    {
                        error("MethodRedefineException: Method " << methodName << "(" << Strings::join(methodParameters, " ") << ") already exists.");
                    }

                    // define method

                    Method* method = new Method(
                        methodName, methodReturnType,
                        methodModifiers, methodParameters,
                        factory, this
                    );

                    defineMethod(method);
                    method->build(content);

                    // clear variables after declaration is done

                    methodName = "";
                    methodReturnType = "V";
                    methodModifiers.clear();
                    methodParameters.clear();
                    content.clear();
                }

                else if (instruction == Instruction::FIELD_END && type == 2 && --fieldIndex == 0)
                {
                    // end declaration

                    begun = false;
                    type = -1;

                    if (hasField(fieldName))
                    {
                        error("FieldRedefineException: Field '" << fieldName << "' already exists.");
                    }

                    // define field

                    Field* field = new Field(
                        fieldName, fieldType,
                        fieldModifiers, factory, this
                    );

                    defineField(field);
                    field->build(content);

                    // clear variables after declaration is done

                    fieldName = "";
                    fieldType = "";
                    fieldModifiers.clear();
                }

                else
                {
                    if (instruction == Instruction::CLASS_BEGIN)
                    {
                        classIndex++;
                    }

                    else if (instruction == Instruction::METHOD_BEGIN)
                    {
                        methodIndex++;
                    }

                    else if (instruction == Instruction::FIELD_BEGIN)
                    {
                        fieldIndex++;
                    }

                    // append line to class content

                    content.push_back(line);
                }
            }

            // class definition start (determine class name)

            else if (instruction == Instruction::CLASS_DEFINE)
            {
                className = args[1];
                type = 0;
            }

            else if (instruction == Instruction::METHOD_DEFINE)
            {
                methodName = args[1];
                type = 1;
            }

            else if (instruction == Instruction::FIELD_DEFINE)
            {
                fieldName = args[1];
                type = 2;
            }

            else if (type == 0)
            {
                // determine class modifiers

                if (instruction == Instruction::CLASS_MODIFIER)
                {
                    classModifiers = Lists::subList(args, 1);
                }

                // determine class superclass

                else if (instruction == Instruction::CLASS_EXTENDS)
                {
                    classSuperclass = args[1];
                }

                // determine class 

                else if (instruction == Instruction::CLASS_IMPLEMENTS)
                {
                    classInterfaces = Lists::subList(args, 1);
                }

                // handle class content begin

                else if (instruction == Instruction::CLASS_BEGIN && classIndex++ == 0)
                {
                    begun = true;
                }
            }

            else if (type == 1)
            {
                // determine method modifiers

                if (instruction == Instruction::METHOD_MODIFIER)
                {
                    methodModifiers = Lists::subList(args, 1);
                }

                // determine method return type

                else if (instruction == Instruction::METHOD_RETURN_TYPE)
                {
                    methodReturnType = args[1];
                }

                // determine method parameters

                else if (instruction == Instruction::METHOD_PARAMETER)
                {
                    methodParameters = Lists::subList(args, 1);
                }

                // handle method content begin

                else if (instruction == Instruction::METHOD_BEGIN && methodIndex++ == 0)
                {
                    begun = true;
                }
            }

            else if (type == 2)
            {
                // handle field modifiers

                if (instruction == Instruction::FIELD_MODIFIER)
                {
                    fieldModifiers = Lists::subList(args, 1);
                }

                // handle field type

                else if (instruction == Instruction::FIELD_TYPE)
                {
                    fieldType = args[1];
                }

                // handle field content begin

                else if (instruction == Instruction::FIELD_BEGIN && fieldIndex++ == 0)
                {
                    begun = true;
                }
            }
        }
    }

    /**
     * Initialize static class fields.
     * Call static constructor if exists.
     */
    void Class::initialize(Stack* heap)
    {
        // get the static constructor

        Method* constructor = getMethod("<cinit>", LIST());

        // invoke static constructor if exists

        if (constructor != nullptr)
        {
            constructor->invoke(factory, heap, nullptr, nullptr);
        }

        // invoke static class fields

        for (Field* field : fields)
        {
            if (!field->hasModifier(Modifier::STATIC))
            {
                continue;
            }

            field->invoke(factory, heap, nullptr);
        }
    }

    /**
     * Get a method with the given name and arguments.
     */
    Method* Class::getMethod(STRING name, LIST parameters)
    {
        // for method in methods declared in the class

        for (Method* method : methods)
        {
            SIZE_T size = method->parameters.size();

            // check if the name and the parameters' size matches

            if (method->name == name && size == parameters.size())
            {
                // check if parameters equals

                for (SIZE_T i = 0; i < size; i++)
                {
                    // parameter did not match, continue searching

                    if (method->parameters[i] != parameters[i])
                    {
                        // continue searching

                        goto search;
                    }
                }

                // method found

                return method;
            }
        search:;
        }

        // method not found

        return nullptr;
    }

    /**
     * Get a method with the given name and arguments recursively.
     */
    Method* Class::findMethod(STRING name, LIST parameters)
    {
        // get the method from the current class

        Method* method = getMethod(name, parameters);
        
        // seems like we have found the method

        if (method != nullptr)
        {
            return method;
        }

        // recursively search for methods of the super classes

        Class* super = factory->getClass(superclass);

        while (super != nullptr)
        {
            // search for method in the superclass

            method = super->findMethod(name, parameters);

            // method found

            if (method != nullptr)
            {
                return method;
            }

            // get the superclass of the current superclass

            super = factory->getClass(super->superclass);
        }

        // TODO wanna search for interfaces?????

        return nullptr;
    }

    /**
     * Determine if a method exists with the given name and arguments.
     */
    bool Class::hasMethod(STRING name, LIST parameters)
    {
        return getMethod(name, parameters) != nullptr;
    }

    /**
     * Define a method in the class.
     */
    void Class::defineMethod(Method* method)
    {
        methods.push_back(method);
    }

    /**
     * Get a field with the given name.
     */
    Field* Class::getField(STRING name)
    {
        // for field in fields declared inside the class

        for (Field* field : fields)
        {
            // check if names equals

            if (field->name == name)
            {
                return field;
            }
        }

        return nullptr;
    }

    /**
     * Defermine if a field exists with the given name.
     */
    bool Class::hasField(STRING name)
    {
        return getField(name) != nullptr;
    }

    /**
     * Define a field in the class.
     */
    void Class::defineField(Field* field)
    {
        fields.push_back(field);
    }

    /**
     * Debug class and it's content.
     */
    void Class::debug()
    {
        // debug class modifiers

        LIST modifiers = getModifiers(this->modifiers);
        
        if (modifiers.size() > 0)
        {
            print(Strings::join(modifiers, " ") << " ");
        }

        // debug class name

        print("class " << name << " ");

        // debug class superclass

        if (superclass != "" && superclass != "void.lang.Object")
        {
            print("extends " << superclass << " ");
        }

        // debug class interfaces

        if (interfaces.size() > 0)
        {
            print("implements " << Strings::join(interfaces, ", ") << " ");
        }

        // debug class body

        println("{");

        // debug fields

        for (Field* field : fields)
        {
            field->debug();
        }

        // debug methods

        for (Method* method : methods)
        {
            method->debug();
        }

        println("}");
    }

    /**
     * Parse the native path of the class.
     */
    STRING Class::nativePath()
    {
        // determine the native method path

        LIST pathList;

        pathList.push_back("_Included_");

        // append declaring class' name

        LIST name;

        Strings::split(this->name, name, '.');
        Lists::append(pathList, name);

        // get the final path

        return Strings::join(pathList, "_");
    }

    /**
     * Get the list of the native methods declared.
     */
    LIST_T<Method*> Class::nativeMethods()
    {
        LIST_T<Method*> result;

        for (Method* method : methods)
        {
            if (method->hasModifier(Modifier::NATIVE))
            {
                result.push_back(method);
            }
        }

        return result;
    }
}