#pragma once

#include "../../Common.hpp"
#include "../Factory.hpp"
#include "Stack.hpp"
#include "Storage.hpp"
#include "../../util/Strings.hpp"
#include "../../util/Lists.hpp"

namespace Void
{
    class Factory;
    class Stack;

    /**
     * Execute bytecode for the given executable.
     */
    void execute(Factory* factory, Stack* stack, Storage* storage, LIST bytecode, ANY& result, Executable* executable);

    /**
     * Handle integer management instructions.
     */
    void handleIntegerManagement(STRING command, LIST args, STRING line, ANY& result, SIZE_T& cursor, SIZE_T length, Stack* stack, Storage* storage, Factory* factory, Executable* executable);

    /**
     * Handle float management instructions.
     */
    void handleFloatManagement(STRING command, LIST args, Stack* stack, Storage* storage, Executable* executable);

    /**
     * Handle boolean management instructions.
     */
    void handleBooleanManagement(STRING command, LIST args, Stack* stack, Storage* storage, Executable* executable);

    /**
     * Handle instance management instructions.
     */
    void handleInstanceManagement(STRING command, LIST args, ANY& result, SIZE_T& cursor, SIZE_T length, Stack* stack, Storage* storage, Executable* executable);

    /**
     * Handle method call instructions.
     */
    void handleMethodCalls(STRING command, LIST args, Stack* stack, Storage* Storage, Factory* factory, Executable* executable);

    /**
     * Handle instance creation instructions.
     */
    void handleInstanceCreation(STRING command, LIST args, Stack* stack, Factory* factory);

    /**
     * Handle goto/field (get) management instructions.
     */
    void handleFieldGetAndGoto(STRING command, LIST args, SIZE_T& cursor, Stack* stack, Factory* factory, Executable* executable);

    /**
     * Handle goto/field (put) management instructions.
     */
    void handleFieldPutAndGoto(STRING command, LIST args, STRING line, SIZE_T& cursor, Stack* stack, Factory* factory, Executable* executable);

    /**
     * Handle string management instructions.
     */
    void handleStringManagement(STRING command, LIST args, STRING line, Stack* stack, Factory* factory);

    /**
     * Handle if statmenet instructions.
     */
    void handleIfStatements(STRING command, LIST args, SIZE_T& cursor, Stack* stack, Executable* executable);
}