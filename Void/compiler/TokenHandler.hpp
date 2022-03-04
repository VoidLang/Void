#pragma once

#include "../Common.hpp"
#include "Token.hpp"

namespace Void
{
	/**
	 * Represents a basic token handler.
	 */
	class TokenHandler
	{
	private:
		/**
		 * List of tokens to be handled.
		 */
		LIST_T<Token>& tokens;

		/**
		 * Parsing begin index.
		 */
		SIZE_T& begin;

		/**
	     * Current parsing index.
		 */
		SIZE_T& cursor;

	public:
		/**
		 * Initialize token handler.
		 */
		TokenHandler(LIST_T<Token>& tokens, SIZE_T& begin, SIZE_T& cursor);

		/**
		 * Get the token at the given index.
		 */
		Token at(SIZE_T& index);

		/**
		 * Get the next token.
		 */
		Token next();

		/**
		 * Get the current token.
		 */
		Token get();

		/**
		 * Determine if the given index is in bounds of the tokens.
		 */
		bool has(SIZE_T index);
	};
}