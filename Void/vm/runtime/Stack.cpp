#include "Stack.hpp"

namespace Void
{
    /**
     * Initialize stack.
     */
    Stack::Stack(Stack* parent, STRING name)
    {
        this->parent = parent;
        this->name = name;
        this->index = (parent != nullptr) ? parent->index + 1 : 0;
    }

    /**
     * Get the current stack trace recursively.
     */
    void Stack::getStackTrace(LIST_T<Stack*>& stacks)
    {
        stacks.push_back(this);
        if (parent != nullptr)
        {
            parent->getStackTrace(stacks);
        }
    }
}