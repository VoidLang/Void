#pragma once

#include "../../Common.hpp"

namespace Void
{
	template <typename T>

	/**
	 * Represents a sub-storage of the actual storage.
	 */
	class StorageElement
	{
	private:
		/**
		 * Sub-storage data.
		 */
		MAP<int, T> data;

	public:
		/**
		 * Set the value of the given sub-storage unit.
		 */
		void set(int index, T value)
		{
			data[index] = value;
		}

		/**
		 * Get the value of the given sub-storage unit.
		 */
		T get(int index)
		{
			return data[index];
		}

		/**
		 * Remove the value of the given sub-storage unit.
		 */
		void pop(int index)
		{
			data.erase(index);
		}
	};
}