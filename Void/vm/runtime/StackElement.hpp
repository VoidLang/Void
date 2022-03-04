#pragma once

#include "../../Common.hpp"

namespace Void
{
	template <typename T>

	/**
	 * Represents a sub-stack of the actual stack.
	 */
	class StackElement
	{
	private:
		/**
		 * Sub-stack data.
		 */
		LIST_T<T> data;

	public:
		/**
		 * Push a value to the top of the stack.
		 */
		void push(T value)
		{
			data.push_back(value);
		}

		/**
		 * Append a value to the beginning of the stack.
		 */
		void append(T value)
		{
			data.insert(data.begin(), value);
		}

		/**
		 * Pull the first element of the stack.
		 */
		T pull()
		{
			T value = data[0];
			data.erase(data.begin());
			return value;
		}

		/**
		 * Get the first element of the stack.
		 */
		T get()
		{
			return data[0];
		}

		/**
		 * Get the size of the stack.
		 */
		SIZE_T size()
		{
			return data.size();
		}
	};
}
