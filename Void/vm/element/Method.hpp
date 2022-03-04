#pragma once

#include "../../Common.hpp"
#include "../Factory.hpp"
#include "../runtime/Stack.hpp"
#include "../runtime/Storage.hpp"
#include "Executable.hpp"
#include "../runtime/Executor.hpp"

namespace Void
{
	class Factory;
	class Stack;
	class Class;
	enum class Modifier;

	class Method : public Executable
	{
	private:
		/**
		 * The virtual machine's factory.
		 */
		Factory* factory;

	public:
		/**
		 * The name of the method.
		 */
		STRING name;

		/**
		 * The return type of the method.
		 */
		STRING returnType;

		/**
		 * The modifiers of the method.
		 */
		int modifiers;

		/**
		 * The parameter types of the method.
		 */
		LIST parameters;

		/**
		 * The alternative method body for native methods.
		 */
		FUNCTION<void(Factory*, Stack*, Storage*, Instance*, Executable*, Executable*, ANY&)> override;

		/**
		 * The address of the DLL method.
		 */
		FARPROC methodAddress;

		/**
		 * Initialize method.
		 */
		Method(STRING name, STRING returnType, LIST modifiers, LIST parameters, Factory* factory, Class* clazz);

		/**
		 * Build method content.
		 */
		void build(LIST bytecode);

		/**
		 * Determine if the option is set.
		 */
		bool hasOption(STRING option);

		/**
		 * Perform a method call.
		 */
		void invoke(Factory* factory, Stack* stackSrc, Instance* instance, Executable* caller);

		/**
		 * Determine if the field has modifier.
		 */
		bool hasModifier(Modifier modifier);

		/**
		 * Debug method and it's content.
		 */
		void debug();

		/**
		 * Parse the native path of the method.
		 */
		STRING nativePath();

		/**
		 * Get the native return type of the method.
		 */
		STRING nativeReturnType();
	};
}