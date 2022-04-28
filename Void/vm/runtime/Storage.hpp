#pragma once

#include "StorageElement.hpp"
#include "Value.hpp"

namespace Void
{
    class Instance;

    /**
     * Represents a dynamic storage that holds certain types of objects.
     */
    class Storage
    {
    public:
        /**
         * An int holding sub storage.
         */
        StorageElement<int> intStorage;

        /**
         * A float holding sub storage.
         */
        StorageElement<float> floatStorage;

        /**
         * A long holding sub storage. 
         */
        StorageElement<LONG> longStorage;

        /**
         * A boolean holding sub storage.
         */
        StorageElement<bool> booleanStorage;

        /**
         * An instance holding sub storage.
         */
        StorageElement<Instance*> instanceStorage;
    };
}