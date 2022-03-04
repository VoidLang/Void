#pragma once

#include "../Common.hpp"
#include "Factory.hpp"
#include "../util/Options.hpp"
#include "Program.hpp"

namespace Void
{
    /**
     * A virtual machine instantiated for each ran executables.
     * Dynamically loaded executables will be attached to an existing one.
     */
    class VirtualMachine
    {
    private:
        /**
         * Program start options.
         */
        Options& options;

        /**
         * Program's class factory.
         */
        Factory& factory;

        /**
         * The program.
         */
        Program& program;

    public:
        /**
         * Initialize virtual machine.
         */
        VirtualMachine(Factory& factory, Program& program, Options& options);

        /**
         * Load executable bytecode into the virtual machine.
         */
        void load(LIST bytecode);

        /**
         * Debug all the classes, methods, fields 
         * created within the virual machine.
         */
        void debug();
    };
}