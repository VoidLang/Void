#pragma once

#include "../Common.hpp"

namespace Void
{
    namespace Lists
    {
        /**
         * Create a sub-list from begin to the end.
         */
        LIST subList(LIST list, int begin);

        /**
         * Create a sub-list from begin to the end.
         */
        LIST subListBack(LIST list, int begin, int end);

        /**
         * Create list with the arguments passed.
         */
        LIST toList(int size, ...);

        /**
         * Append a list to another.
         */
        void append(LIST& dest, LIST& append);
    }
}