#include "Field.hpp"

namespace Void
{
    /**
     * Initialize field.
     */
    Field::Field(STRING name, STRING type, LIST modifiers, Factory* factory, Class* clazz) 
        : Field(name, type, getModifiers(modifiers), factory, clazz) {}

    /**
     * Initialize field with already parsed modifiers.
     */
    Field::Field(STRING name, STRING type, int modifiers, Factory* factory, Class* clazz)
    {
        this->name = name;
        this->type = type;
        this->modifiers = modifiers;
        this->factory = factory;
        this->primitive = type[0] != 'L';
        this->clazz = clazz;
    }

    /**
     * Build field content.
     */
    void Field::build(LIST bytecode)
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
     * Determine if the field has modifier.
     */
    bool Field::hasModifier(Modifier modifier)
    {
        int mod = static_cast<UNDERLYING_TYPE<Modifier>::type>(modifier);
        return (modifiers & mod) > 0;
    }

    /**
     * Determine if the option is set.
     */
    bool Field::hasOption(STRING option)
    {
        return factory->options.has(option);
    }

    /**
     * Invoke (initialize) the field.
     */
    void Field::invoke(Factory* factory, Stack* stackSrc, Instance* instance)
    {
        // create data holders

        Stack* stack = new Stack(stackSrc, this->clazz->name + "." + this->name + ":" + this->type);
        Storage* storage = new Storage();

        // prepare for a non-static call

        if (instance != nullptr)
        {
            storage->instanceStorage.set(0, instance);
        }
        
        // execute bytecode, get the result
        
        ANY result;

        execute(factory, stack, storage, bytecode, result, this);

        // convert call result to value

        Value* value = nullptr;

        if (primitive)
        {
            // handle integer result

            if (type == "I")
            {
                value = new Primitive(type, ANY_CAST<int>(result));
            }

            // handle float result

            else if (type == "F")
            {
                value = new Primitive(type, ANY_CAST<float>(result));
            }

            // TODO support more primitive types
        }

        // handle instance result

        else
        {
            value = ANY_CAST<Instance*>(result);
        }

        // store value in the right place

        if (instance == nullptr)
        {
            this->value = value;
        }

        else
        {
            instance->setValue(this, value);
        }
        

        // TODO prob wanna delete stack and storage here because they are no longer needed
    }

    /**
     * Copy field value to the stack.
     */
    void Field::get(Stack* stack, Instance* instance)
    {
        // get the correct value
        // static     -> field value
        // non-static -> instance value

        Value* value = (instance != nullptr) ? instance->getValue(this) : this->value;

        // field value is a primitive

        if (primitive)
        {
            std::string type = value->getType();

            // field value is an integer

            if (type == "I")
            {
                stack->intStack.push(std::any_cast<int>(value->getPrimitive()));
            }

            // field value is a float

            else if (type == "F")
            {
                stack->floatStack.push(std::any_cast<float>(value->getPrimitive()));
            }

            // TODO support more primitive types
        }

        // field value is an instance

        else
        {
            // field value is null

            if (value == nullptr)
            {
                stack->instanceStack.push(nullptr);
            }

            // field value is not null

            else
            {
                stack->instanceStack.push(value->getInstance());
            }
        }
    }

    /**
     * Copy value from the stack to the field.
     */
    void Field::put(Stack* stack, Instance* instance)
    {
        // convert stack element to value

        Value* value = nullptr;

        if (primitive)
        {
            if (type == "I")
            {
                value = new Primitive(type, stack->intStack.pull());
            }

            else if (type == "F")
            {
                value = new Primitive(type, stack->floatStack.pull());
            }
        }
        else
        {
            value = stack->instanceStack.pull();
        }

        // store value in the right place

        if (instance != nullptr)
        {
            instance->setValue(this, value);
        }

        else 
        {
            this->value = value;
        }
    }

    /**
     * Debug field and it's content.
     */
    void Field::debug()
    {
        // debug field modifiers

        LIST modifiers = getModifiers(this->modifiers);

        print("    ");

        if (modifiers.size() > 0)
        {
            print(Strings::join(modifiers, " ") << " ");
        }

        // debug field return type and name

        print(type << " " << name);

        // debug field body

        println(";");

        for (int i = 0; i < bytecode.size(); i++)
        {
            println("        " << i << ": " << bytecode[i]);
        }
    }
}