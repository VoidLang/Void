#pragma once

#include "../Common.hpp"

#include <stdio.h>
#include <windows.h>
#include <excpt.h>

namespace Void
{
    namespace Exceptions
    {
        /**
         * Get the name of the exception using exception code.
         */
        STRING getName(unsigned int exception);

        /**
         * Handle a windows-level exception.
         */
        int handle(unsigned int exception, _EXCEPTION_POINTERS* pointers);
    }
}
