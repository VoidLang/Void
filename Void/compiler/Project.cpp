#include "Project.hpp"

namespace Void
{
    /**
     * Initialize project xml.
     */
    Project::Project(STRING path)
    {
        this->path = path;
    }

    /**
     * Load xml file.
     */
    bool Project::load()
    {
        return true;
    }
}