#include "Modifier.hpp"

namespace Void
{
    /**
     * Get the value of a modifier.
     */
    int getModifier(STRING modifier)
    {
        for (SIZE_T i = 0; i < 12; i++)
        {
            if (MODIFIER_KEYS[i] == modifier)
            {
                return MODIFIER_VALUES[i];
            }
        }
        return 0;
    }
    
    /**
     * Get the values of the modifiers.
     */
    int getModifiers(LIST modifiers)
    {
        int flag = 0;
        for (STRING modifier : modifiers)
        {
            flag |= getModifier(modifier);
        }
        return flag;
    }

    /**
     * Get the names of the modifiers.
     */
    LIST getModifiers(int modifier)
    {
        LIST modifiers;

        // for sizeof(modifiers)

        for (SIZE_T i = 0; i < 12; i++)
        {
            // check if modifier is present

            if ((modifier & MODIFIER_VALUES[i]) > 0)
            {
                // append modifier name 

                modifiers.push_back(MODIFIER_KEYS[i]);
            }
        }
        return modifiers;
    }

    /**
     * Determines if the modifier name matches it's value.
     */
    bool operator==(STRING string, Modifier modifier)
    {
        return static_cast<int>(modifier) == getModifier(string);
    }
}