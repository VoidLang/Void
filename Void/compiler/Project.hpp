#pragma once

#include "../Common.hpp"

namespace Void
{
    /**
     * Represents a wrapper for the project settings xml file.
     */
    class Project
    {
    private:
        /**
         * Xml file path.
         */
        STRING path;

    public:
        /**
         * Initialize project xml.
         */
        Project(STRING path);

        /**
         * Load xml file.
         */
        bool load();
    };
}