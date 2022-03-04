#include "Executable.hpp"

namespace Void
{
    /**
     * Determine if the given section exists.
     */
    bool Executable::hasSection(STRING section)
    {
        // for entry in sections

        for (const auto& [key, value] : sections)
        {
            // check if section key equals 

            if (section == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Determine if the given linker exists.
     */
    bool Executable::hasLinker(STRING linker)
    {
        // for entry in linkers

        for (const auto& [key, value] : linkers)
        {
            // check if linker key equals 

            if (linker == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Determine if the given linker value exists.
     */
    bool Executable::hasLinkerValue(int val)
    {
        // for entry in linkers

        for (const auto& [key, value] : linkers)
        {
            // check if linker value equals 

            if (value == val)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Get the value of a linker.
     */
    int Executable::getLinker(STRING linker)
    {
        // for entry in linkers

        for (const auto& [key, value] : linkers)
        {
            // check if linker value equals 

            if (key == linker)
            {
                return value;
            }
        }

        return -1;
    }

    /**
     * Get the value of a section.
     */
    SIZE_T Executable::getSection(STRING section)
    {
        // for entry in sections

        for (const auto& [key, value] : sections)
        {
            // check if linker value equals 

            if (key == section)
            {
                return value;
            }
        }

        return -1;
    }
}
