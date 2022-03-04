#pragma once

#include "../Common.hpp"

namespace Void
{
    namespace Strings
    {
        /**
         * Split up string into parts between each "separator". 
         */
        size_t split(STRING& string, LIST& result, char separator);

        /**
         * Join all list elements in a list using a separator.
         */
        STRING join(LIST& list, STRING separator);

        /**
         * Update all the certain parts of the string to a new value. 
         */
        bool replace(STRING& string, STRING from, STRING to);

        /**
         * Determine if the given string starts with the given prefix.
         */
        bool startsWith(STRING string, STRING prefix);
    }
}