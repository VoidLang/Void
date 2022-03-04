#include "Exceptions.hpp"

namespace Void
{
    namespace Exceptions
    {
        /**
         * Get the name of the exception using exception code.
         */
        STRING Exceptions::getName(unsigned int exception)
        {
            switch (exception)
            {
                case EXCEPTION_ACCESS_VIOLATION:
                    return "ACCESS_VIOLATION";
                case EXCEPTION_DATATYPE_MISALIGNMENT:
                    return "DATATYPE_MISALIGNMENT";
                case EXCEPTION_BREAKPOINT:
                    return "BREAKPOINT";
                case EXCEPTION_SINGLE_STEP:
                    return "SINGLE_STEP";
                case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
                    return "ARRAY_BOUNDS_EXCEEDED";
                case EXCEPTION_FLT_DENORMAL_OPERAND:
                    return "FLT_DENORMAL_OPERAND";
                case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                    return "FLT_DIVIDE_BY_ZERO";
                case EXCEPTION_FLT_INEXACT_RESULT:
                    return "FLT_INEXACT_RESULT";
                case EXCEPTION_FLT_INVALID_OPERATION:
                    return "FLT_INVALID_OPERATION";
                case EXCEPTION_FLT_OVERFLOW:
                    return "FLT_OVERFLOW";
                case EXCEPTION_FLT_STACK_CHECK:
                    return "FLT_STACK_CHECK";
                case EXCEPTION_FLT_UNDERFLOW:
                    return "FLT_UNDERFLOW";
                case EXCEPTION_INT_DIVIDE_BY_ZERO:
                    return "INT_DIVIDE_BY_ZERO";
                case EXCEPTION_INT_OVERFLOW:
                    return "INT_OVERFLOW";
                case EXCEPTION_PRIV_INSTRUCTION:
                    return "PRIV_INSTRUCTION";
                case EXCEPTION_IN_PAGE_ERROR:
                    return "IN_PAGE_ERROR";
                case EXCEPTION_ILLEGAL_INSTRUCTION:
                    return "ILLEGAL_INSTRUCTION";
                case EXCEPTION_NONCONTINUABLE_EXCEPTION:
                    return "NONCONTINUABLE_EXCEPTION";
                case EXCEPTION_STACK_OVERFLOW:
                    return "STACK_OVERFLOW";
                case EXCEPTION_INVALID_DISPOSITION:
                    return "INVALID_DISPOSITION";
                case EXCEPTION_GUARD_PAGE:
                    return "GUARD_PAGE";
                case EXCEPTION_INVALID_HANDLE:
                    return "INVALID_HANDLE";
                default:
                    return "UKNOWN";
            }
        }

        /**
         * Handle a windows-level exception.
         */
        int handle(unsigned int exception, _EXCEPTION_POINTERS* pointers)
        {
            if (exception == CONTROL_C_EXIT)
            {
                return EXCEPTION_CONTINUE_SEARCH;
            }
            println('\n' << "[Void] A windows-level exception occurred: "
                << getName(exception) << " (" << exception << ")"
                << '\n' << pointers);
            return EXCEPTION_EXECUTE_HANDLER;
        }
    }
}
