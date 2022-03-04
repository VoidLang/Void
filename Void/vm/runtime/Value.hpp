#pragma once

#include "../../Common.hpp"
#include "../Factory.hpp"
#include "../element/Class.hpp"
#include "../runtime/Stack.hpp"
#include "../element/Field.hpp"
#include "../Modifier.hpp"

// Quality code productions LLC.

namespace Void
{
	// forward declare
	class Factory;
	class Instance;
	class Class;
	class Stack;
	enum class Modifier;
	class Field;

	/**
	 * Represents a runtime object that is either an instance or a primitive.
	 */
	class Value
	{
	private:
		/**
		 * A mutex for each value.
		 */
		MUTEX* mutex;
		
		/**
		 * A thread lock system.
		 */
		LOCK<MUTEX>* lock;

	public:
		/**
		 * Initialize value.
		 */
		Value();

		/**
		 * Get the type of the value.
		 */
		virtual STRING getType();

		/**
		 * Determine if the value is a primitive.
		 */
		virtual bool isPrimitive();

		/**
		 * Get the primitive of the value.
		 */
		virtual ANY getPrimitive();
	
		/**
		 * Get the instance of the value.
		 */
		virtual Instance* getInstance();
	};

	/**
	 * Represents a runtime class instance.
	 */
	class Instance : public Value
	{
	private:
		/**
		 * The registry of values for the non static class fields.
		 */
		MAP<Field*, Value*> values;

	public:
		/**
		 * The instantiated class.
		 */
		Class* clazz;

		/**
		 * Create instance.
		 */
		Instance(Factory* factory, Class* clazz, Stack* stack);

		/**
		 * Get the type of the value.
		 */
		STRING getType();

		/**
		 * Determine if the value is a primitive.
		 */
		bool isPrimitive();

		/**
		 * Get the primitive of the value.
		 */
		ANY getPrimitive();

		/**
		 * Get the instance of the value.
		 */
		Instance* getInstance();

		/**
		 * Get the value of a field from the instance.
		 */
		Value* getValue(Field* field);

		/**
		 * Set the value of the field of the instance.
		 */
		void setValue(Field* field, Value* value);

		/**
		 * Get a field from the value registry.
		 */
		Field* getField(STRING name);

		/**
		 * Determine if a value exists for the given field name. 
		 */
		bool hasValue(STRING field);

		/**
		 * Debug the instance.
		 */
		virtual STRING debug();
	};

	/**
	 * Represents a runtime primitive value.
	 */
	class Primitive : public Value
	{
	private:
		/**
		 * The primitive type.
		 */
		STRING type;

		/**
		 * The primitive value.
		 */
		ANY value;

	public:
		/**
		 * Initialize primitive.
		 */
		Primitive(STRING type, ANY value);

		/**
		 * Get the type of the value.
		 */
		STRING getType();

		/**
		 * Determine if the value is a primitive.
		 */
		bool isPrimitive();

		/**
		 * Get the primitive of the value.
		 */
		ANY getPrimitive();

		/**
		 * Get the instance of the value.
		 */
		Instance* getInstance();
	};

	/**
	 * Represents a runtime array base.
	 */
	class BaseArray : public Instance
	{
	public:
		/**
		 * The type of elements stored in the array.
		 */
		STRING type;

		/**
		 * The size of the array.		 
		 */
		int size;

		/**
		 * Initialize array base.
		 */
		BaseArray(Factory* factory, Class* clazz, Stack* stack, STRING type, int size);
	};

	template <typename T> 

	struct Element
	{
	public:
		bool exists = false;
		T data {};

		Element &operator=(T value)
		{
			exists = true;
			data = value;
			return *this;
		}
	};

	template <typename T>

	/**
	 * Represents a runtime array holding an N amount of T type objects.
	 */
	class Array : public BaseArray
	{
	private:
		/**
		 * Dynamic template array holding the elements.
		 */
		Element<T>* data;

	public:
		/**
		 * Initialize array.
		 */
		Array(Factory* factory, Class* clazz, Stack* stack, STRING type, int size) : BaseArray(factory, clazz, stack, type, size)
		{
			data = new Element<T>[size];
		}

		/**
		 * Get the value at the given index.
		 */
		T get(int index, T defaultValue)
		{
			auto element = data[index];
			if (element.exists)
			{
				return element.data;
			}
			return defaultValue;
		}
	
		/**
		 * Set the value at the given index.
		 */
		void set(int index, T value)
		{
			data[index] = value;
		}

		/**
		 * Debug the array.
		 */
		STRING debug()
		{
			const void* address = static_cast<const void*>(this);

			STRING_STREAM stream;
			stream << "<array " << type << "[" << size << "] at 0x" << address << ">";

			return stream.str();
		}
	};

	/**
	 * Instantiate a built-in class.
	 */
	Instance* createInstance(Factory* factory, Class* clazz, Stack* stack);

	/**
	 * Include buildin classes into the virtual machine.
	 */
	void include(Factory* factory);

	/**
	 * Include the void.lang.Object class.
	 */
	void includeObject(Factory* factory);

	/**
	 * Invclude the void.lang.System class.
	 */
	void includeSystem(Factory* factory);

	/**
	 * Represents a built-in implementation for void.lang.String.
	 */
	class String : public Instance
	{
	public:
		/**
		 * The actual value of the string.
		 */
		STRING value;

		/**
		 * Initialize string.
		 */
		String(Factory* factory, Stack* stack, STRING value);
	};

	/**
	 * Include the void.lang.String class.
	 */
	void includeString(Factory* factory);
}