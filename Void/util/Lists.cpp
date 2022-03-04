#include "Lists.hpp"

namespace Void
{
    /**
     * Create a sub-list from begin to the end.
     */
    LIST Lists::subList(LIST list, int begin)
    {
        return LIST(list.begin() + begin, list.begin() + list.size());
    }

    /**
     * Create a sub-list from begin to the end.
     */
    LIST Lists::subListBack(LIST list, int begin, int end)
    {
        return LIST(list.begin() + begin, list.end() + end);
    }

    /**
     * Create list with the arguments passed.
     */
    LIST Lists::toList(int size, ...)
    {
        va_list list;

        LIST result;

        va_start(list, size);

        for (int i = 0; i < size; i++)
        {
            result.push_back(STRING(va_arg(list, char*)));
        }

        va_end(list);

        return result;
    }

    /**
     * Append a list to another.
     */
    void Lists::append(LIST& dest, LIST& append)
    {
        for (auto element : append)
        {
            dest.push_back(element);
        }
    }
}