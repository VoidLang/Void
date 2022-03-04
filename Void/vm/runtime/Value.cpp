#include "Value.hpp"

namespace Void
{
    /**
     * Initialize value.
     */
    Value::Value()
    {
        // create mutex
        mutex = new MUTEX();
        // create thread lock
        lock = new LOCK<MUTEX>(*mutex, DEFER_LOCK);
    }

    /**
     * Get the type of the value.
     */
    STRING Value::getType()
    {
        throw "Not implemented";
    }

    /**
     * Determine if the value is a primitive.
     */
    bool Value::isPrimitive()
    {
        throw "Not implemented";
    }

    /**
     * Get the primitive of the value.
     */
    ANY Value::getPrimitive()
    {
        throw "Not implemented";
    }

    /**
     * Get the instance of the value.
     */
    Instance* Value::getInstance()
    {
        throw "Not implemented";
    }
    
    /**
     * Create instance.
     */
    Instance::Instance(Factory* factory, Class* clazz, Stack* stack)
    {
        this->clazz = clazz;

        // copy non-static fields

        for (Field* field : clazz->fields)
        {
            // skip static fields

            if (field->hasModifier(Modifier::STATIC))
            {
                continue;
            }

            // invoke the field

            field->invoke(factory, stack, this);
        }

        // check if class has a custom superclass

        if (clazz->superclass == "void.lang.Object")
        {
            return;
        }

        // for superclass in class hierarchy

        Class* superClass = factory->getClass(clazz->superclass);

        while (superClass != nullptr)
        {
            // copy superclass fields

            for (Field* field : superClass->fields)
            {
                // skip static fields

                if (field->hasModifier(Modifier::STATIC))
                {
                    continue;
                }

                // check if a value already exists for the given field name

                if (hasValue(field->name))
                {
                    continue;
                }

                // invoke the field

                field->invoke(factory, stack, this);
            }

            // get the next class

            STRING nextClass = superClass->superclass;

            // check if superclass is not a custom class

            if (nextClass == "void.lang.Object")
            {
                break;
            }

            // get superclass superclass

            superClass = factory->getClass(nextClass);
        }
    }

    /**
     * Get the type of the value.
     */
    STRING Instance::getType()
    {
        return clazz->name;
    }

    /**
     * Determine if the value is a primitive.
     */
    bool Instance::isPrimitive()
    {
        return false;
    }

    /**
     * Get the primitive of the value.
     */
    ANY Instance::getPrimitive()
    {
        throw "Can not get an instance as primitive";
    }

    /**
     * Get the instance of the value.
     */
    Instance* Instance::getInstance()
    {
        return this;
    }

    /**
     * Get a field from the value registry.
     */
    Field* Instance::getField(STRING name)
    {
        // for entry in values

        for (const auto& [key, value] : values)
        {
            // check if field key equals 

            if (key->name == name)
            {
                return key;
            }
        }

        return nullptr;
    }

    /**
     * Get the value of a field from the instance.
     */
    Value* Instance::getValue(Field* field)
    {
        // for entry in values

        for (const auto& [key, value] : values)
        {
            // check if field key equals 

            if (key == field)
            {
                return value;
            }
        }

        return nullptr;
    }

    /**
     * Set the value of the field of the instance.
     */
    void Instance::setValue(Field* field, Value* value)
    {
        values[field] = value;
    }

    /**
     * Determine if a value exists for the given field name.
     */
    bool Instance::hasValue(STRING field)
    {
        // for entry in values

        for (const auto& [key, value] : values)
        {
            // check if field name equals 

            if (key->name == field)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * Debug the instance.
     */
    STRING Instance::debug()
    {
        const void* address = static_cast<const void*>(this);
        
        STRING_STREAM stream;
        stream << "<class " << clazz->name << " at 0x" << address << ">";

        return stream.str();
    }

    /**
     * Initialize primitive.
     */
    Primitive::Primitive(STRING type, ANY value)
    {
        this->type = type;
        this->value = value;
    }

    /**
     * Get the type of the value.
     */
    STRING Primitive::getType()
    {
        return type;
    }

    /**
     * Determine if the value is a primitive.
     */
    bool Primitive::isPrimitive()
    {
        return true;
    }

    /**
     * Get the primitive of the value.
     */
    ANY Primitive::getPrimitive()
    {
        return value;
    }

    /**
     * Get the instance of the value.
     */
    Instance* Primitive::getInstance()
    {
        throw "Can not get a primitive as an instance";
    }

    /**
     * Initialize array base.
     */
    BaseArray::BaseArray(Factory* factory, Class* clazz, Stack* stack, STRING type, int size) : Instance(factory, clazz, stack)
    {
        this->type = type;
        this->size = size;
    }

    /**
     * Instantiate a built-in class.
     */
    Instance* createInstance(Factory* factory, Class* clazz, Stack* stack)
    {
        // split up class name

        LIST parts;
        Strings::split(clazz->name, parts, '.');

        // handle void.lang package

        if (parts[1] == "lang")
        {
            // handle void.lang.String

            if (parts[2] == "String")
            {
                return new String(factory, stack, "");
            }
        }

        // handle void.io package

        else if (parts[1] == "io")
        {

        }

        return nullptr;
    }

    /**
     * Include buildin classes into the virtual machine.
     */
    void include(Factory* factory)
    {
        // include the void.lang package

        includeObject(factory);
        includeString(factory);
        includeSystem(factory);
    }

    /**
     * Include the void.lang.Object class.
     */
    void includeObject(Factory* factory)
    {
        // create Object class

        Class* clazz = new Class("void.lang.Object", "", Lists::toList(1, "public"), LIST(), factory);
    
        // create toString method
        Method* toString = new Method("toString", "Lvoid.lang.String", Lists::toList(2, "public", "native"), LIST(), factory, clazz);

        // override method body

        toString->override = [](Factory* factory, Stack* stack, Storage* storage, Instance* instance, Executable* caller, Executable* self, ANY& result)
        {
            Instance* ins = storage->instanceStorage.get(0);
            result = (Instance*) new String(factory, stack, ins->debug());
        };

        clazz->defineMethod(toString);

        factory->defineClass(clazz);
    }

    /**
     * Invclude the void.lang.System class.
     */
    void includeSystem(Factory* factory)
    {
        // create System class

        Class* clazz = new Class("void.lang.System", "void.lang.Object", Lists::toList(2, "public", "final"), LIST(), factory);

        // create loadLibrary method

        Method* loadLibrary = new Method("loadLibrary", "V", Lists::toList(3, "public", "static", "native"), Lists::toList(1, "Lvoid.lang.String"), factory, clazz);

        // override method body

        loadLibrary->override = [](Factory* factory, Stack* stack, Storage* storage, Instance* instance, Executable* caller, Executable* self, ANY& result)
        {
            String* libname = dynamic_cast<String*>(storage->instanceStorage.get(0));

            factory->loadLibrary(libname->value, caller->clazz);
        };

        clazz->defineMethod(loadLibrary);

        factory->defineClass(clazz);
    }

    /**
     * Initialize string.
     */
    String::String(Factory* factory, Stack* stack, STRING value) : Instance(factory, factory->getClass("void.lang.String"), stack)
    {
        this->value = value;
    }

    /**
     * Include the void.lang.String class.
     */
    void includeString(Factory* factory)
    {
        // create String class

        Class* clazz = new Class("void.lang.String", "", Lists::toList(1, "public"), LIST(), factory);

        // define the class

        factory->defineClass(clazz);
    }
}
