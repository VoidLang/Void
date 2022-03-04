#include "Lexer.hpp"

namespace Void
{
	/**
	 * Initalize lexer.
	 */
	Lexer::Lexer(STRING data)
	{
		this->data = data;
	}

	/**
	 * Lex the next token of the data.
	 */
	Token Lexer::next()
	{
		// ignore whitespaces

		while (isSpace(peek()))
		{
			if (peek() == '\n')
			{
				lineIndex = 0;
				get();
				return Token(TokenType::LineNumber, TO_STRING(++lineNumber));
			}
			get();
		}

		// handle content end

		if (peek() == '\0')
			return Token(TokenType::Finish);

		// handle identifiers

		else if (isIdentifier(peek()))
			return makeIdentifier();

		// handle operators

		else if (isOperator(peek()))
			return makeOperator();

		// handle separators

		else if (isSeparator(peek()))
			return makeSeparator();

		// handle numbers

		else if (isNumber(peek()))
			return makeNumber();
	
		return Token(TokenType::Unexpected);
	}

	/**
	 * Make an identifier token using the current data.
     */
	Token Lexer::makeIdentifier()
	{
		// get the full identifier

		SIZE_T begin = current;
		while (isIdentifier(peek()) || isNumber(peek())) get();

		// determine token type

		TokenType type = TokenType::Identifier;
		STRING token = range(begin, current);

		if (isExpression(token))
			type = TokenType::Expression;

		else if (isType(token))
			type = TokenType::Type;

		else if (isModifier(token))
			type = TokenType::Modifier;

		else if (isBoolean(token))
			type = TokenType::Boolean;

		else if (isInfo(token))
			type = TokenType::Info;

		else if (isNull(token))
			type = TokenType::Null;

		// make token

		return Token(type, token);
	}

	/**
	 * Make an operator token using the current data.
	 */
	Token Lexer::makeOperator()
	{
		return Token(TokenType::Operator, STRING(1, get()));
	}

	/**
	 * Make a separator token using the current data.
	 */
	Token Lexer::makeSeparator()
	{
		TokenType type = TokenType::Unexpected;

		switch (get())
		{
			case ';':
				type = TokenType::Semicolon;
				break;
			case ':':
				type = TokenType::Colon;
				break;
			case ',':
				type = TokenType::Comma;
				break;
			case '{':
				type = TokenType::Begin;
				break;
			case '}':
				type = TokenType::End;
				break;
			case '(':
				type = TokenType::Open;
				break;
			case ')':
				type = TokenType::Close;
				break;
			case '[':
				type = TokenType::Start;
				break;
			case ']':
				type = TokenType::Stop;
				break;
		}

		return Token(type);
	}

	/**
	 * Make a number token using the current data.
	 */
	Token Lexer::makeNumber()
	{
		// get the beginning of the number's content

		SIZE_T begin = current;

		// determine if the number is integer

		bool integer = true;

		// handle hexadecimal numbers

		if (peek() == '0' && at(current + 1) == 'x')
		{
			// skip the '0x' chars

			move(2);
			
			// handle number content

			while (isHexNumber(peek()))
			{
				get();
			}

			// make hexadecimal number token

			STRING value = range(begin, current);

			return Token(TokenType::Hexadecimal, value);
		}

		// handle normal number

		while (isNumberContent(upper(peek())))
		{
			// handle non-integer number

			if (peek() == '.')
			{
				// number can not contain multiple '.' chars

				if (!integer)
				{
					return Token(TokenType::Unexpected);
				}

				integer = false;
			}

			// check if the number has ending

			if (isNumberEnding(upper(peek())))
			{
				// non-integers can not have '.' chars in it

				if (upper(peek()) == 'I' && !integer)
				{
					return Token(TokenType::Unexpected);
				}

				// get the number type

				TokenType type = TokenType::Integer;

				switch (upper(peek()))
				{
					case 'L':
						type = TokenType::Long;
						break;
					case 'F':
						type = TokenType::Float;
						break;
					case 'D':
						type = TokenType::Double;
						break;
					case 'S':
						type = TokenType::Short;
						break;
					case 'B':
						type = TokenType::Byte;
						break;
				}

				move(1);

				// get the value of the number

				STRING value = range(begin, current - 1);

				// make number token

				return Token(type, value);
			}

			get();
		}

		// get the value of the number

		STRING value = range(begin, current);

		return Token(TokenType::Integer, value);
	}

	/**
	 * Get the character at the current index.
	 */
	char Lexer::peek()
	{
		if (has(current))
		{
			return data[current];
		}
		return '\0';
	}

	/**
	 * Get the character at the current index and move to the next index.
	 */
	char Lexer::get()
	{
		if (has(current))
		{
			lineIndex++;
			return data[current++];
		}
		return '\0';
	}

	/**
	 * Get the previous character from the data.
	 */
	char Lexer::prev()
	{
		if (has(current - 1))
		{
			return data[current - 1];
		}
		return '\0';
	}

	/**
	 * Get the previous -Nth character from the data.
	 */
	char Lexer::prev(SIZE_T skip)
	{
		if (has(current - skip))
		{
			return data[current - skip];
		}
		return '\0';
	}

	/**
	 * Move the cursor with the given amount.
	 */
	void Lexer::move(SIZE_T skip)
	{
		current += skip;
	}

	/**
	 * Determine whether the given index is in bounds of the data length.
	 */
	bool Lexer::has(SIZE_T index)
	{
		return index >= 0 && index < data.length();
	}

	/**
	 * Get char at the given index from the data.
	 */
	char Lexer::at(SIZE_T index)
	{
		return data[index];
	}

	/**
	 * Get the data in the given range.
	 */
	STRING Lexer::range(SIZE_T begin, SIZE_T end)
	{
		STRING result;

		for (; begin < end; begin++)
		{
			result += data[begin];
		}

		return result;
	}

	/**
	 * Determine whether the given character is a space.
	 */
	bool Lexer::isSpace(char c)
	{
		switch (c) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				return true;
			default:
				return false;
		}
	}

	/**
	 * Determine whether the given character is identifier-like.
	 */
	bool Lexer::isIdentifier(char c)
	{
		return isalpha(c) || c == '_';
	}

	/**
	 * Determine whether the given character is numeric.
	 */
	bool Lexer::isNumber(char c)
	{
		switch (c)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return true;
			default:
				return false;
		}
	}

	/**
	 * Determine whether the given character is a number ending.
	 */
	bool Lexer::isNumberEnding(char c)
	{
		switch (c)
		{
			case 'L':
			case 'F':
			case 'D':
			case 'I':
			case 'S':
			case 'B':
				return true;
			default:
				return false;
		}
	}

	/**
	 * Determine whether the given character is a hexadecimal number.
	 */
	bool Lexer::isHexNumber(char c)
	{
		switch (c)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return true;
			default:
				return false;
		}
	}

	/**
	 * Determine whether the given character is a number's content.
	 */
	bool Lexer::isNumberContent(char c)
	{
		switch (c)
		{
			case '.':
			case '_':
				return true;
			default:
				return isHexNumber(c) || isNumberEnding(c);
		}
	}

	/**
	 * Determine whether the given character is an operator.
	 */
	bool Lexer::isOperator(char c)
	{
		switch (c)
		{
			case '.':
			case '=':
			case '+':
			case '-':
			case '*':
			case '/':
			case '<':
			case '>':
			case '?':
			case '!':
			case '^':
			case '&':
			case '~':
				return true;
			default:
				return false;
		}
	}

	/**
	 * Determine whether the given character is a separator.
	 */
	bool Lexer::isSeparator(char c)
	{
		switch (c)
		{
			case ';':
			case ':':
			case ',':
			case '{':
			case '}':
			case '(':
			case ')':
			case '[':
			case ']':
				return true;
			default:
				return false;
		}
	}

	/**
	 * Convert the given character to lowercase.
	 */
	char Lexer::lower(char c)
	{
		return LOWERCASE(c);
	}

	/**
	 * Convert the given character to uppercase.
	 */
	char Lexer::upper(char c)
	{
		return UPPERCASE(c);
	}

	/**
	 * Determine whether the given token is an expression.
	 */
	bool Lexer::isExpression(STRING token)
	{
		return token == "new"
			|| token == "namespace"
			|| token == "class"
			|| token == "enum"
			|| token == "interface"
			|| token == "for"
			|| token == "while"
			|| token == "repeat"
			|| token == "do"
			|| token == "if"
			|| token == "else"
			|| token == "switch"
			|| token == "case"
			|| token == "when"
			|| token == "continue"
			|| token == "break"
			|| token == "return"
			|| token == "await"
			|| token == "goto";
	}
	
	/**
	 * Determine whether the given token is a type.
	 */
	bool Lexer::isType(STRING token)
	{
		return token == "byte"
			|| token == "short"
			|| token == "int"
			|| token == "double"
			|| token == "float"
			|| token == "long"
			|| token == "void"
			|| token == "boolean"
			|| token == "char"
			|| token == "var";
	}

	/**
	 * Determine whether the given token is a modifier.
	 */
	bool Lexer::isModifier(STRING token)
	{
		return token == "public"
			|| token == "protected"
			|| token == "private"
			|| token == "static"
			|| token == "final"
			|| token == "native"
			|| token == "extern"
			|| token == "volatile"
			|| token == "transient"
			|| token == "synchronized"
			|| token == "async"
			|| token == "const"
			|| token == "unsafe"
			|| token == "weak"
			|| token == "strong";
	}

	/**
	 * Determine whether the given token is a boolean.
	 */
	bool Lexer::isBoolean(STRING token)
	{
		return token == "true" || token == "false";
	}

	/**
	 * Determine whether the given token is a file info.
	 */
	bool Lexer::isInfo(STRING token)
	{
		return token == "package" || token == "import" || token == "using";
	}

	/**
	 * Determine whether the given token is a null value.
	 */
	bool Lexer::isNull(STRING token)
	{
		return token == "null" || token == "nullptr";
	}
}