#pragma once

#include "../../Common.hpp"
#include "../Token.hpp"
#include "Instructions.hpp"

namespace Void
{
	/**
	 * Represents a body token parser that converts tokens to instructions.
	 */
	class Body
	{
	private:
		/**
		 * The current index of the token that is handled.
		 */
		SIZE_T cursor = 0;

		/**
		 * The current line number.
		 */
		SIZE_T lineNumber;

		/**
		 * List of tokens to be parsed.
		 */
		LIST_T<Token> tokens;

		/**
		 * List of the parsed instructions.
		 */
		LIST_T<Instruction> instructions;

	public:
		/**
		 * Initialize body.
		 */
		Body(LIST_T<Token> tokens);

		/**
		 * Parse the body.
		 */
		bool parse();

	private:
		/**
		 * Parse the next instruction.
		 */
		Instruction next();

		/**
		 * Declare a primitive-type local variable.
		 */
		Instruction declareLocalPrimitive();

		/**
		 * Get the current token.
		 */
		Token peek();

		/**
		 * Get the next token.
		 */
		Token get();

		/**
		 * Get the token at the given index.
		 */
		Token at(SIZE_T index);

		/**
		 * Determine whether the given index is in bounds of the data length.
		 */
		bool has(SIZE_T index);
	};
}