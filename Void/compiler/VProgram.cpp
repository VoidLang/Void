#include "VProgram.hpp"

namespace Void
{
    /**
     * Define a class.
     */
    bool VProgram::defineClass(VClass* clazz)
    {
        // check if a class with the given name already exists

        if (hasClass(clazz->name))
            return false;

        // define the class

        classes.push_back(clazz);
        return true;
    }

    /**
     * Get a class with the given name.
     */
    VClass* VProgram::getClass(STRING name)
    {
        // for class in classes

        for (VClass* clazz : classes)
        {
            // check if the class name matches

            if (clazz->name == name)
            {
                // class found

                return clazz;
            }
        }

        // no class found

        return nullptr;
    }

    /**
     * Determine whether a class with the given name exists.
     */
    bool VProgram::hasClass(STRING name)
    {
        return getClass(name) != nullptr;
    }

    /**
     * Build the program to bytecode.
     */
    LIST VProgram::build()
    {
        // create result bytecode

        LIST bytecode;

        // build classes

        for (VClass* clazz : classes)
        {
            clazz->build(bytecode);
        }

        // build finished

        return bytecode;
    }
}