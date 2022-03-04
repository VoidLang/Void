#pragma once

#include "../Common.hpp"
#include "Description.hpp"

namespace Void
{
    /**
     * Determines a Void program which builds bytecode from raw inputs.
     */
    class Program
    {
    private:
        /**
         * Executable path.
         */
        STRING path;

        /**
         * Program start arguments. 
         */
        LIST arguments;

        /**
         * Program definitions.
         */
        MAP<STRING, STRING> definitions;

    public:
        /**
         * Program description.
         */
        Description description;

        /**
         * Initialize program.
         */
        Program(STRING path, LIST arguments);

        /**
         * Build the bytecode.
         */
        LIST build();

        /**
         * Validate executable path.
         */
        bool validate();

        /**
         * Determine if program has definition.
         */
        bool hasDefinition(STRING definition);

        /**
         * Set definition value.
         */
        void setDefinition(STRING definition, STRING value);

        /**
         * Get definition value.
         */
        STRING getDefinition(STRING definition);

        // void 

    private:
        /**
         * Read file, build bytecode.
         */
        void buildFile(STRING path, LIST& bytecode);
    };
}