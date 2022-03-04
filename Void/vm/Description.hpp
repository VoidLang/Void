#pragma once

#include "../Common.hpp"

namespace Void
{
    /**
     * Represents the program's descrption.
     */
    class Description
    {
    private:
        /**
         * Determines the program entry point.
         */
        OPTIONAL_T<STRING> programMain;
    
    public:
        /**
         * Determines if the program main is set.
         */
        bool hasProgramMain();

        /**
         * Get the program's main path.
         */
        STRING getProgramMain();
        /**
         * Sets the value of the program main.
         */
        void setProgramMain(STRING programMain);
    };
}