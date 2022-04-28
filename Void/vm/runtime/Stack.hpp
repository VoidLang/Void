#pragma once

#include "StackElement.hpp"
#include "Value.hpp"

namespace Void
{
    class Instance;

    /**
     * Represents a dynamic stack that holds certain types of objects.
     */
    class Stack
    {
    private:
        /**
         * Parent stack.
         */
        Stack* parent;

    public:
        /**
         * An int holding sub stack.
         */
        StackElement<int> intStack;

        /**
         * A float holding sub stack.
         */
        StackElement<float> floatStack;

        /**
         * A long holding sub stack.
         */
        StackElement<LONG> longStack;

        /**
         * A boolean holding sub stack.
         */
        StackElement<bool> booleanStack;

        /**
         * An instance holding sub stack.
         */
        StackElement<Instance*> instanceStack;

        /**
         * Determines the current stack index.
         * (how far the stack is from the heap)
         */
        int index;

        /**
         * The name of the stack's caller.
         */
        STRING name;

        /**
         * Initialize stack.
         */
        Stack(Stack* parent, STRING name);

        /**
         * Get the current stack trace recursively.
         */
        void getStackTrace(LIST_T<Stack*>& stacks);
    };
}
