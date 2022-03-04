#include "VirtualMachine.hpp"

namespace Void
{
    /**
     * Initialize virtual machine.
     */
    VirtualMachine::VirtualMachine(Factory& factory, Program& program, Options& options) : 
        options(options), factory(factory), program(program) 
    { }

    /**
     * Load bytecode into the virtual machine.
     */
    void VirtualMachine::load(LIST bytecode)
    {
        factory.build(bytecode);
    }

    /**
     * Debug all the classes, methods, fields
     * created within the virual machine.
     */
    void VirtualMachine::debug()
    {
        for (Class* clazz : factory.classes) 
        {
            clazz->debug();
        }
    }
}
