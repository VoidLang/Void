#pragma once

#include "../../Common.hpp"
#include "Executable.hpp"
#include "../Factory.hpp"
#include "../Modifier.hpp"

namespace Void
{
    class Factory;
    class Stack;
    class Value;
    class Instance;
    class Class;

    class Field : public Executable
    {
    private:
        /**
         * The virtual machine's factory.
         */
        Factory* factory;

    public:
        /**
         * The name of the field.
         */
        STRING name;

        /**
         * The type of the field.
         */
        STRING type;

        /**
         * Determines is the type of the field primitive.
         */
        bool primitive;

        /**
         * The value of the field.
         */
        Value* value;

        /**
         * The modifiers of the field.
         */
        int modifiers;

        /**
         * Field bytecode.
         */
        LIST bytecode;

        /**
         * Initialize field.
         */
        Field(STRING name, STRING type, LIST modifiers, Factory* factory, Class* clazz);

        /**
         * Initialize field.
         */
        Field(STRING name, STRING type, int modifiers, Factory* factory, Class* clazz);

        /**
         * Build field content.
         */
        void build(LIST bytecode);

        /**
         * Determine if the field has modifier.
         */
        bool hasModifier(Modifier modifier);

        /**
         * Determine if the option is set.
         */
        bool hasOption(STRING option);

        /**
         * Invoke (initialize) the field.
         */
        void invoke(Factory* factory, Stack* stackSrc, Instance* instance);

        /**
         * Copy field value to the stack.
         */
        void get(Stack* stack, Instance* instance);

        /**
         * Copy value from the stack to the field.
         */
        void put(Stack* stack, Instance* instance);

        /**
        * Debug field and it's content.
        */
        void debug();
    };
}