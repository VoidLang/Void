#pragma once

#include "../Common.hpp"

namespace Void
{
    enum class Instruction
    {
        CLASS_DEFINE,
        CLASS_MODIFIER,
        CLASS_EXTENDS,
        CLASS_IMPLEMENTS,
        CLASS_BEGIN,
        CLASS_END,
        
        METHOD_DEFINE,
        METHOD_MODIFIER,
        METHOD_RETURN_TYPE,
        METHOD_PARAMETER,
        METHOD_BEGIN,
        METHOD_END,

        FIELD_DEFINE,
        FIELD_MODIFIER,
        FIELD_TYPE,
        FIELD_BEGIN,
        FIELD_END,

        LINKER
    };

    bool operator==(STRING string, Instruction instruction);
}