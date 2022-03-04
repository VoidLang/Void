#pragma once

#include "../Common.hpp"
#include "Token.hpp"

namespace Void
{
	/**
	 * The lexer is the part of the compiler system that 
	 * converts raw input data to parseable tokens.
	 */
	class Lexer
	{
	private:
		/**
		 * The input data to be converted to tokens.
		 */
		STRING data;

		/**
		 * The current index of the data that is handled.
		 */
		SIZE_T current = 0;
	
	public:
		/**
		 * The number of the current line being processed.
		 */
		SIZE_T lineNumber = 1;

		/**
		 * The index of the current character in the line being processed.
		 */
		SIZE_T lineIndex = 0;

		/**
		 * Initalize lexer.
		 */
		Lexer(STRING data);

		/**
		 * Lex the next token of the data.
		 */
		Token next();

		/**
		 * Make an identifier token using the current data.
		 */
		Token makeIdentifier();

		/**
		 * Make an operator token using the current data.
		 */
		Token makeOperator();

		/**
		 * Make a separator token using the current data.
		 */
		Token makeSeparator();

		/**
         * Make a number token using the current data.
		 */
		Token makeNumber();

		/**
		 * Get the character at the current index.
		 */
		char peek();

		/**
		 * Get the character at the current index and move to the next index.
		 */
		char get();

		/**
		 * Get the previous character from the data.
		 */
		char prev();

		/**
		 * Get the previous -Nth character from the data.
		 */
		char prev(SIZE_T skip);

		/**
		 * Move the cursor with the given amount.
		 */
		void move(SIZE_T skip);

		/**
		 * Determine whether the given index is in bounds of the data length.
		 */
		bool has(SIZE_T index);

		/**
		 * Get char at the given index from the data.
		 */
		char at(SIZE_T index);

		/**
		 * Get the data in the given range.
		 */
		STRING range(SIZE_T begin, SIZE_T end);

		/**
		 * Determine whether the given character is a space.
		 */
		bool isSpace(char c);

		/**
		 * Determine whether the given character is identifier-like.
		 */
		bool isIdentifier(char c);

		/**
		 * Determine whether the given character is numeric.
		 */
		bool isNumber(char c);

		/**
		 * Determine whether the given character is a number ending.
		 */
		bool isNumberEnding(char c);

		/**
		 * Determine whether the given character is a hexadecimal number.
		 */
		bool isHexNumber(char c);

		/**
		 * Determine whether the given character is a number's content.
		 */
		bool isNumberContent(char c);

		/**
		 * Determine whether the given character is an operator.
		 */
		bool isOperator(char c);

		/**
		 * Determine whether the given character is a separator.
		 */
		bool isSeparator(char c);

		/**
		 * Convert the given character to lowercase.
		 */
		char lower(char c);

		/**
		 * Convert the given character to uppercase.
		 */
		char upper(char c);

		/**
		 * Determine whether the given token is an expression.
		 */
		bool isExpression(STRING token);

		/**
		 * Determine whether the given token is a type.
		 */
		bool isType(STRING token);

		/**
		 * Determine whether the given token is a modifier.
		 */
		bool isModifier(STRING token);

		/**
		 * Determine whether the given token is a boolean.
		 */
		bool isBoolean(STRING token);

		/**
		 * Determine whether the given token is a file info.
		 */
		bool isInfo(STRING token);

		/**
		 * Determine whether the given token is a null value.
		 */
		bool isNull(STRING token);
	};
}