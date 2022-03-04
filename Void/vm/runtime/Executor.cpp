#include "Executor.hpp"

namespace Void 
{
    /**
     * Execute bytecode for the given executable.
     */
    void execute(Factory* factory, Stack* stack, Storage* storage, LIST bytecode, ANY& result, Executable* executable)
    {
        for (SIZE_T cursor = 0; cursor < bytecode.size(); )
        {
            // get the current executing line

            std::string line = bytecode[cursor];

            // split up the instruction args

            LIST args;
            Strings::split(line, args, ' ');

            // get the command and the first char of it

            STRING command = args[0];
            char c = command[0];

            // prefix check to reduce tons of if-else checking

            if (c == 'i')
            {
                // handle if instructions

                if (command[1] == 'f')
                {
                    handleIfStatements(command, args, cursor, stack, executable);
                }

                // handle method invokes

                else if (command[1] == 'n' && command[2] == 'v')
                {
                    handleMethodCalls(command, args, stack, storage, factory, executable);
                }

                // handle stack element conversions

                else if (command[1] == '2')
                {
                }

                // handle integer management

                else
                {
                    handleIntegerManagement(command, args, line, result, cursor, bytecode.size(), stack, storage, factory, executable);
                }
            }

            // handle float management

            else if (c == 'f')
            {
                handleFloatManagement(command, args, stack, storage, executable);
            }

            // handle instance management

            else if (c == 'a')
            {
                handleInstanceManagement(command, args, result, cursor, bytecode.size(), stack, storage, executable);
            }

            // handle goto/field management

            else if (c == 'g')
            {
                handleFieldGetAndGoto(command, args, cursor, stack, factory, executable);
            }

            // handle field manipulation/print streams

            else if (c == 'p')
            {
                handleFieldPutAndGoto(command, args, line, cursor, stack, factory, executable);
            }

            // handle instance creation

            else if (c == 'n')
            {
                handleInstanceCreation(command, args, stack, factory);
            }

            // handle string management

            else if (c == 's')
            {
                handleStringManagement(command, args, line, stack, factory);
            }

            // handle object synchronization

            else if (c == 'm')
            {
            }

            // handle boolean management

            else if (c == 'z')
            {
                handleBooleanManagement(command, args, stack, storage, executable);
            }

            // handle exception throwing

            else if (command == "throw")
            {
            }

            // handle object deletion

            else if (command == "delete")
            {
            }

            // stop executing, exit loop

            else if (command == "return")
            {
            }

            // exit program

            else if (command == "exit")
            {
            }

            cursor++; // move cursor to the next line to continue executing
        }
    }

    /**
     * Handle integer management instructions.
     */
    void handleIntegerManagement(STRING command, LIST args, STRING line, ANY& result, SIZE_T& cursor, SIZE_T length, Stack* stack, Storage* storage, Factory* factory, Executable* executable)
    {
        // push an integer to the stack

        if (command == "ipush")
        {
            stack->intStack.push(stoi(args[1]));
        }

        // load an integer from the given storage unit to the stack

        else if (command == "iload")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            int value = storage->intStorage.get(index);
            stack->intStack.push(value);
        }

        // store an integer in the given storage unit

        else if (command == "istore")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            int value = stack->intStack.pull();
            storage->intStorage.set(index, value);
        }

        // store an integer in the given storage unit whilst keeping the integer in the stack

        else if (command == "istorew")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            int value = stack->intStack.get();
            storage->intStorage.set(index, value);
        }

        // load an integer from the given array

        else if (command == "iaload")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get the length of a null array");
            }

            Array<int>* array = dynamic_cast<Array<int>*>(instance);

            int index = stack->intStack.pull();
            
            // check if the index is out of the bounds

            if (index < 0 || index >= array->size)
            {
                error("IndexOutOfBoundsException: Index " << index << " is out of bounds [0; " << array->size << "[");
            }

            stack->intStack.push(array->get(index, 0));
        }

        // store an integer in the given array

        else if (command == "iastore")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get the length of a null array");
            }

            // cast the instance to an int array

            Array<int>* array = dynamic_cast<Array<int>*>(instance);

            // get the index to store at

            int index = stack->intStack.pull();

            // check if the index is out of the bounds

            if (index < 0 || index >= array->size)
            {
                error("IndexOutOfBoundsException: Index " << index << " is out of bounds [0; " << array->size <<  "[");
            }

            // get the value to store

            int value = stack->intStack.pull();

            // store the value

            array->set(index, value);
        }

        // take two integers from the stack, add (a + b) back to it

        else if (command == "iadd")
        {
            stack->intStack.push(stack->intStack.pull() + stack->intStack.pull());
        }

        // take two integers from the stack, add (a - b) back to it

        else if (command == "irem")
        {
            int a = stack->intStack.pull();
            int b = stack->intStack.pull();
            stack->intStack.push(a - b);
        }

        // take two integers from the stack, add (a * b) back to it

        else if (command == "imul")
        {
            stack->intStack.push(stack->intStack.pull() * stack->intStack.pull());
        }

        // take two integers from the stack, add (a * b) back to it

        else if (command == "idiv")
        {
            int a = stack->intStack.pull();
            int b = stack->intStack.pull();
            stack->intStack.push(a / b);
        }

        // take two integers from the stack, add (a % b) back to it

        else if (command == "imod")
        {
            int a = stack->intStack.pull();
            int b = stack->intStack.pull();
            stack->intStack.push(a % b);
        }

        // take an integer from the stack, add (a + 1) back to it

        else if (command == "iinc")
        {
            int value = stack->intStack.pull();
            stack->intStack.push(value + 1);
        }

        // take an integer from the stack, add (a - 1) back to it

        else if (command == "idecr")
        {
            int value = stack->intStack.pull();
            stack->intStack.push(value - 1);
        }
        
        // take an integer from the stack, add (-a) back to it

        else if (command == "ineg")
        {
            stack->intStack.push(-stack->intStack.pull());
        }

        // move an integer from the stack to the call result

        else if (command == "ireturn")
        {
            int value = stack->intStack.pull();
            result = value;
            cursor = length; // move cursor to the end so the executor loop breaks
        }

        // pull an integer from the stack

        else if (command == "ipop")
        {
            stack->intStack.pull();
        }

        // duplicate the integer on the stack top

        else if (command == "idup")
        {
            stack->intStack.append(stack->intStack.get());
        }

        // duplicate the integer on the stack top X times

        else if (command == "idup_x")
        {
            int value = stack->intStack.get();
            int amount = stoi(args[1]);

            for (int i = 0; i < amount; i++)
            {
                stack->intStack.append(value);
            }
        }

        // debug the first stack unit

        else if (command == "iprint")
        {
            print(stack->intStack.pull());
        }

        // debug the first stack unit and print a new line

        else if (command == "iprintln")
        {
            println(stack->intStack.pull());
        }

        // debug the first stack unit whils keeping the value in the stack

        else if (command == "idebug")
        {
            print(stack->intStack.get());
        }

        // debug the first stack unit whils keeping the value in the stack and print a new line

        else if (command == "idebugln")
        {
            println(stack->intStack.get());
        }

        // debug the current int stack size

        else if (command == "istacksize")
        {
            stack->intStack.push(stack->intStack.size());
        }
        
        // get console input

        else if (command == "input")
        {
            int index;
            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            STRING string = line.substr(7);
            string.pop_back();

            print(string);

            STRING input;
            GET_LINE(CONSOLE_IN, input);
            
            stack->instanceStack.push(new String(factory, stack, input));
        }
    }

    /**
     * Handle float management instructions.
     */
    void handleFloatManagement(STRING command, LIST args, Stack* stack, Storage* storage, Executable* executable)
    {
        // push a float to the stack

        if (command == "fpush")
        {
            float value = stof(args[1]);
            stack->floatStack.push(value);
        }
        
        // load a float from the given storage unit to the stack

        else if (command == "fload")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            float value = storage->floatStorage.get(index);
            stack->floatStack.push(value);
        }

        // store a float in the given storage unit

        else if (command == "fstore")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            float value = stack->floatStack.pull();
            storage->floatStorage.set(index, value);
        }

        // store a float in the given storage unit whilst keeping the float in the stack

        else if (command == "fstorew")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            float value = stack->floatStack.get();
            storage->floatStorage.set(index, value);
        }

        // debug the first stack unit

        else if (command == "fprint")
        {
            print(stack->floatStack.pull());
        }

        // debug the first stack unit and print a new line

        else if (command == "fprintln")
        {
            println(stack->floatStack.pull());
        }

        // debug the first stack unit whils keeping the value in the stack

        else if (command == "fdebug")
        {
            print(stack->floatStack.get());
        }

        // debug the first stack unit whils keeping the value in the stack and print a new line

        else if (command == "fdebugln")
        {
            println(stack->floatStack.get());
        }

        // debug the current float stack size

        else if (command == "fstacksize")
        {
            stack->intStack.push(stack->floatStack.size());
        }
    }

    /**
     * Handle boolean management instructions.
     */
    void handleBooleanManagement(STRING command, LIST args, Stack* stack, Storage* storage, Executable* executable)
    {
        // push a float to the stack

        if (command == "zpush")
        {
            stack->booleanStack.push(args[1] == "true" || args[1] == "1");
        }

        // load a boolean from the given storage unit to the stack

        else if (command == "zload")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            bool value = storage->booleanStorage.get(index);
            stack->booleanStack.push(value);
        }

        // store a boolean in the given storage unit

        else if (command == "zstore")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            bool value = stack->booleanStack.pull();
            storage->booleanStorage.set(index, value);
        }

        // store a boolean in the given storage unit whilst keeping the boolean in the stack

        else if (command == "zstorew")
        {
            int index;

            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }

            bool value = stack->booleanStack.get();
            storage->booleanStorage.set(index, value);
        }

        // debug the first stack unit

        else if (command == "zprint")
        {
            print((stack->booleanStack.pull() ? "true" : "false"));
        }

        // debug the first stack unit and print a new line

        else if (command == "zprintln")
        {
            println((stack->booleanStack.pull() ? "true" : "false"));
        }

        // debug the first stack unit whils keeping the value in the stack

        else if (command == "zdebug")
        {
            print((stack->booleanStack.get() ? "true" : "false"));
        }

        // debug the first stack unit whils keeping the value in the stack and print a new line

        else if (command == "zdebugln")
        {
            println((stack->booleanStack.get() ? "true" : "false"));
        }

        // debug the current boolean stack size

        else if (command == "zstacksize")
        {
            stack->intStack.push(stack->booleanStack.size());
        }
    }

    /**
     * Handle instance management instructions.
     */
    void handleInstanceManagement(STRING command, LIST args, ANY& result, SIZE_T& cursor, SIZE_T length, Stack* stack, Storage* storage, Executable* executable)
    {
        // load an instance from the given storage unit to the stack

        if (command == "aload")
        {
            int index;
            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                if (args[1] == "this")
                    index = 0;
                else
                    index = executable->getLinker(args[1]);
            }
            stack->instanceStack.push(storage->instanceStorage.get(index));
        }

        // store an instance in the given storage unit

        else if (command == "astore")
        {
            int index;
            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }
            storage->instanceStorage.set(index, stack->instanceStack.pull());
        }

        // store an instance in the given storage unit whilst keeping the integer in the stack

        else if (command == "astorew")
        {
            int index;
            try
            {
                index = stoi(args[1]);
            }
            catch (...)
            {
                index = executable->getLinker(args[1]);
            }
            Instance* instance = stack->instanceStack.get();
            storage->instanceStorage.set(index, instance);
        }

        // load an instance from the given array

        else if (command == "aaload")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get the length of a null array");
            }

            Array<Instance*>* array = dynamic_cast<Array<Instance*>*>(instance);

            int index = stack->intStack.pull();

            // check if the index is out of the bounds

            if (index < 0 || index >= array->size)
            {
                error("IndexOutOfBoundsException: Index " << index << " is out of bounds [0; " << array->size << "[");
            }

            stack->instanceStack.push(array->get(index, nullptr));
        }

        // store an instance in the given array

        else if (command == "aastore")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get the length of a null array");
            }

            // cast the instance to an int array

            Array<Instance*>* array = dynamic_cast<Array<Instance*>*>(instance);

            // get the index to store at

            int index = stack->intStack.pull();

            // check if the index is out of the bounds

            if (index < 0 || index >= array->size)
            {
                error("IndexOutOfBoundsException: Index " << index << " is out of bounds [0; " << array->size << "[");
            }

            // get the value to store

            Instance* value = stack->instanceStack.pull();

            // store the value

            array->set(index, value);
        }


        // pull an instance from the stack

        else if (command == "apop")
        {
            stack->instanceStack.pull();
        }

        // duplicate instance on the stack top

        else if (command == "adup")
        {
            stack->instanceStack.append(stack->instanceStack.get());
        }

        // duplicate instance on the stack top X times

        else if (command == "adup_x")
        {
            Instance* instance = stack->instanceStack.get();
            int amount = stoi(args[1]);

            for (int i = 0; i < amount; i++)
            {
                stack->instanceStack.append(instance);
            }
        }

        // move an instance from the stack to the call result

        else if (command == "areturn")
        {
            result = stack->instanceStack.pull();
            cursor = length; // move cursor to the end so the executor loop breaks
            return;
        }

        // get the length of the given array

        else if (command == "arraylength")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get the length of a null array");
            }

            // cast the instance to an array

            BaseArray* array = dynamic_cast<BaseArray*>(instance);

            // put the array's length onto the stack

            stack->intStack.push(array->size);
        }

        // debug the first stack unit

        else if (command == "aprint")
        {
            Instance* instance = stack->instanceStack.pull();
            if (instance == nullptr)
            {
                print("null");
            }
            else
            {
                print(instance->debug());
            }
        }

        // debug the first stack unit and print a new line

        else if (command == "aprintln")
        {
            Instance* instance = stack->instanceStack.pull();
            if (instance == nullptr)
            {
                println("null");
            }
            else
            {
                println(instance->debug());
            }
        }

        // debug the first stack unit whils keeping the value in the stack

        else if (command == "adebug")
        {
            Instance* instance = stack->instanceStack.get();
            if (instance == nullptr)
            {
                print("null");
            }
            else
            {
                print(instance->debug());
            }
        }

        // debug the first stack unit whils keeping the value in the stack and print a new line

        else if (command == "adebugln")
        {
            Instance* instance = stack->instanceStack.get();
            if (instance == nullptr)
            {
                println("null");
            }
            else
            {
                println(instance->debug());
            }
        }

        // debug the current instance stack size

        else if (command == "astacksize")
        {
            stack->intStack.push(stack->instanceStack.size());
        }
    }

    /**
     * Handle method call instructions.
     */
    void handleMethodCalls(STRING command, LIST args, Stack* stack, Storage* Storage, Factory* factory, Executable* executable)
    {
        // handle virtual method call

        if (command == "invokevirtual")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get '" << args[1] << "' field of a null object");
            }

            // get the class of the instance

            Class* clazz = instance->clazz;

            // get the method parameters 

            LIST parameters;

            if (args.size() > 3)
            {
                parameters = Lists::subList(args, 2);
            }

            Method* method = clazz->findMethod(args[1], parameters);

            // check if the method is missing

            if (method == nullptr)
            {
                error("NoSuchMethodException: No such method " << args[1] << "(" << Strings::join(parameters, " ") << ") of class " << clazz->name);
            }

            // check if method is not static

            if (method->hasModifier(Modifier::STATIC))
            {
                error("IllegalAccessException: Can not non-statically invoke a static method");
            }

            // invoke the target method

            method->invoke(factory, stack, instance, executable);
        }

        // handle static method call

        else if (command == "invokestatic")
        {
            // get the target method's class

            Class* clazz = factory->getClass(args[1]);
            
            // check if the class is missing

            if (clazz == nullptr)
            {
                error("NoSuchClassException: " << args[1]);
            }

            // get the method parameters 

            LIST parameters;

            if (args.size() > 3)
            {
                parameters = Lists::subList(args, 3);
            }

            // get the target method

            Method* method = clazz->getMethod(args[2], parameters);

            // check if the method is missing

            if (method == nullptr)
            {
                error("NoSuchMethodException: No such method " << args[2] << "(" << Strings::join(parameters, " ") << ") of class " << clazz->name);
            }

            // check if method is not static

            if (!method->hasModifier(Modifier::STATIC))
            {
                error("IllegalAccessException: Can not statically invoke a non-static method");
            }

            // invoke the target method

            method->invoke(factory, stack, nullptr, executable);
        }
    }

    /**
     * Handle instance creation instructions.
     */
    void handleInstanceCreation(STRING command, LIST args, Stack* stack, Factory* factory)
    {
        // handle instance creation

        if (command == "new")
        {
            // get the target method's class

            Class* clazz = factory->getClass(args[1]);

            // check if the class is missing

            if (clazz == nullptr)
            {
                error("NoSuchClassException: " << args[1]);
            }

            // create instance

            Instance* instance = Strings::startsWith(args[1], "void.") 
                ? createInstance(factory, clazz, stack) // create builtin class instance
                : new Instance(factory, clazz, stack);  // create user-declared class instance

            // push instance onto the instance stack

            stack->instanceStack.push(instance);
        }

        // handle array creation

        else if (command == "newarray")
        {
            // get the stack size

            int size = stack->intStack.pull();

            // get the array type

            STRING type = args[1];

            // type is a class type

            if (type[0] == 'L')
            {
                // remove type prefix

                STRING actualType = type.substr(1, type.length());
            
                // get array type class

                Class* clazz = factory->getClass(actualType);

                // check if class is missing

                if (clazz == nullptr)
                {
                    error("NoSuchClassException: " << actualType);
                }

                stack->instanceStack.push(new Array<Instance*>(factory, clazz, stack, type, size));
            }

            // type is a primitive

            else
            {
                BaseArray* array = nullptr;

                // type is an integer

                if (type == "I")
                {
                    array = new Array<int>(factory, factory->getClass("void.lang.Object"), stack, type, size);
                }

                // type is a float

                else if (type == "F")
                {
                    array = new Array<float>(factory, factory->getClass("void.lang.Object"), stack, type, size);
                }

                // TODO support more types

                stack->instanceStack.push(array);
            }
        }

        // push a null instance onto the stack

        else if (command == "nullptr")
        {
            stack->instanceStack.push(nullptr);
        }
    }

    /**
     * Handle goto/field (get) management instructions.
     */
    void handleFieldGetAndGoto(STRING command, LIST args, SIZE_T& cursor, Stack* stack, Factory* factory, Executable* executable)
    {
        // jump to the given line index

        if (command == "goto")
        {
            try
            {
                cursor = stoi(args[1]);
            }
            catch (...)
            {
                cursor = executable->getSection(args[1]);
            }
        }

        // get a non-static field's value

        else if (command == "getfield")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to get '" << args[1] << "' field of a null object");
            }

            // get the field from the instance

            Field* field = instance->getField(args[1]);
            
            // check if field is missing

            if (field == nullptr)
            {
                error("NoSuchFieldException: No such field '" << args[1] << "' of instance " << instance->getType());
            }

            // chekc if field is static

            if (field->hasModifier(Modifier::STATIC))
            {
                error("IllegalAccessException: Can not non-statically get the value of a static field");
            }
            
            // copy the field's value to the stack

            field->get(stack, instance);
        }

        // get a static field's value

        else if (command == "getstatic")
        {
            // get the target class

            Class* clazz = factory->getClass(args[1]);
        
            // check if the class is missing

            if (clazz == nullptr)
            {
                error("NoSuchClassException: " << args[1]);
            }

            // get the target field

            Field* field = clazz->getField(args[2]);

            // check if the field is missing

            if (field == nullptr)
            {
                error("NoSuchFieldException: No such field '" << args[1] << "' of class " << clazz->name);
            }

            // chekc if field is not static

            if (!field->hasModifier(Modifier::STATIC))
            {
                error("IllegalAccessException: Can not statically get the value of a non-static field");
            }

            // copy the field's value to the stack

            field->get(stack, nullptr);
        }
    }

    /**
     * Handle goto/field (put) management instructions.
     */
    void handleFieldPutAndGoto(STRING command, LIST args, STRING line, SIZE_T& cursor, Stack* stack, Factory* factory, Executable* executable)
    {
        // set the value of a non-static field

        if (command == "putfield")
        {
            // get the instance from the stack top

            Instance* instance = stack->instanceStack.pull();

            // check if the instance is null

            if (instance == nullptr)
            {
                error("NullPointerException: Trying to set '" << args[1] << "' field of a null object");
            }

            // get the field from the instance

            Field* field = instance->getField(args[1]);

            // check if field is missing

            if (field == nullptr)
            {
                error("NoSuchFieldException: No such field '" << args[1] << "' of instance " << instance->getType());
            }

            // chekc if field is static

            if (field->hasModifier(Modifier::STATIC))
            {
                error("IllegalAccessException: Can not non-statically set the value of a static field");
            }

            // copy the field's value to the stack

            field->put(stack, instance);
        }

        // set the value of a static field

        else if (command == "putstatic")
        {
            // get the target class

            Class* clazz = factory->getClass(args[1]);

            // check if the class is missing

            if (clazz == nullptr)
            {
                error("NoSuchClassException: " << args[1]);
            }

            // get the target field

            Field* field = clazz->getField(args[2]);

            // check if the field is missing

            if (field == nullptr)
            {
                error("NoSuchFieldException: No such field '" << args[1] << "' of class " << clazz->name);
            }

            // chekc if field is not static

            if (!field->hasModifier(Modifier::STATIC))
            {
                error("IllegalAccessException: Can not statically set the value of a non-static field");
            }

            // copy the field's value to the stack

            field->put(stack, nullptr);
        }

        // print the given message to the console

        else if (command == "print")
        {
            STRING string = line.substr(7);
            string.pop_back();
            print(string);
        }

        // print the given message to the console

        else if (command == "println")
        {
            if (args.size() > 1)
            {
                STRING string = line.substr(9);
                string.pop_back();
                print(string);
            }
            println("");
        }
    }

    /**
     * Handle string management instructions.
     */
    void handleStringManagement(STRING command, LIST args, STRING line, Stack* stack, Factory* factory)
    {
        if (command == "stacktrace")
        {
            int max = -1;

            if (args.size() > 1)
            {
                max = stoi(args[1]);
            }

            LIST_T<Stack*> stackTrace;
            stack->getStackTrace(stackTrace);

            for (int i = 0; (max < 0 || i < max) && i < stackTrace.size(); i++)
            {
                Stack* st = stackTrace.at(i);
                println("stack [" << st->index << "]: " << st->name);
            }
        }

        else if (command == "spush")
        {
            STRING string = line.substr(7);
            string.pop_back();
            stack->instanceStack.push(new String(factory, stack, string));
        }

        else if (command == "sprint")
        {
            String* string = dynamic_cast<String*>(stack->instanceStack.pull());
            println(string->value);
        }

        else if (command == "sprintln")
        {

            String* string = dynamic_cast<String*>(stack->instanceStack.pull());
            println(string->value);
        }
    }

    /**
     * Handle if statmenet instructions.
     */
    void handleIfStatements(STRING command, LIST args, SIZE_T& cursor, Stack* stack, Executable* executable)
    {
        // check if two integers equals (a == b)

        if (command == "ifieq")
        {
            int a = stack->intStack.pull();
            int b = stack->intStack.pull();

            if (a == b)
            {
                try
                {
                    cursor = stoi(args[1]);
                }
                catch (...)
                {
                    cursor = executable->getSection(args[1]);
                }
            }
        }

        // check if two integers does not equal (a != b)

        else if (command == "ifineq")
        {
            int a = stack->intStack.pull();
            int b = stack->intStack.pull();

            if (a != b)
            {
                try
                {
                    cursor = stoi(args[1]);
                }
                catch (...)
                {
                    cursor = executable->getSection(args[1]);
                }
            }
        }

        // check if who strings equals

        else if (command == "ifseq")
        {
            String* a = dynamic_cast<String*>(stack->instanceStack.pull());
            String* b = dynamic_cast<String*>(stack->instanceStack.pull());

            if (a->value == b->value)
            {
                try
                {
                    cursor = stoi(args[1]);
                }
                catch (...)
                {
                    cursor = executable->getSection(args[1]);
                }
            }
        }

        // check if two references equals

        else if (command == "ifrefeq")
        {
            Instance* a = stack->instanceStack.pull();
            Instance* b = stack->instanceStack.pull();

            if (a == b)
            {
                try
                {
                    cursor = stoi(args[1]);
                }
                catch (...)
                {
                    cursor = executable->getSection(args[1]);
                }
            }
        }
    }
}
