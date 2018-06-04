#pragma once

#include <ostream>
#include <istream>
#include <sstream>
#include <string>
#include <cmath>

#include "json.hpp"

#define UnexpectedEndOfInputString() ("Unexpected End of Input")
#define UnexpectedTokenString(token, pos) ( \
	static_cast<std::ostringstream&>( \
		std::ostringstream{} << "Unexpected Token '" << (char)token << "' at offset " << std::to_string(pos) \
	).str() \
)

#define UnexpectedTokenError(token, pos) JSON::Error(UnexpectedTokenString(token, pos))
#define UnexpectedEndOfInputError() JSON::Error(UnexpectedEndOfInputString())

namespace nbsdx {
	namespace util {
    class JSONParser {
		private:
      std::istream &read_stream;
			std::string error;

			void ConsumeWS() {
				read_stream >> std::ws;
			}

		public:
			JSONParser(std::istream &stream) : read_stream(stream) {}

			JSON parse() {
				ConsumeWS();

				if (!read_stream) {
					return UnexpectedEndOfInputError();
				}

				switch (read_stream.peek()) {
				case '{': {
					return __ParseObject();
				} break;

				case '[': {
					return __ParseArray();
				} break;

				default: 
					return __ParseLiteral();
				}
			}

		private:
			JSON __ParseObject() {
				JSON object = JSON::Object();
				read_stream.get();	// consume {
				ConsumeWS();		// consume whitespace before first key or }
				
				// Empty Object case.
				if (read_stream.peek() == '}') {
					read_stream.get(); // consume }
					return object;
				}

				while (true) {
					if (!read_stream) {
						return UnexpectedEndOfInputError();
					}

          ConsumeWS();

					if (read_stream.peek() != '"') {
						return UnexpectedTokenError(read_stream.peek(), read_stream.tellg());
					}

					std::string key;
					if (!__TryParseString(key)) {
						return JSON::Error(error);
					}

					ConsumeWS();
					if (read_stream.peek() != ':') {
						return UnexpectedTokenError(read_stream.peek(), read_stream.tellg());
					}
					read_stream.get(); // consume :

					// Don't need to ConsumeWS, parse will do it for us.
					JSON value = parse();
					if (value.IsError()) {
						return value;
					}
					object.Emplace(key, std::move(value));

					// We need to read either a , or a }. otherwise parse error.
					ConsumeWS();
					if (read_stream.peek() == ',') {
						read_stream.get(); // consume ,
					}
					else if (read_stream.peek() == '}') {
						read_stream.get(); // consume }
						return object;
					}
					else {
						return UnexpectedTokenError(read_stream.peek(), read_stream.tellg());
					}
				}
			}

			JSON __ParseArray() {
				JSON array = JSON::Array();
				read_stream.get();	// consume [
				ConsumeWS();		// consume whitespace first entry or ]

				// Empty Array case.
				if (read_stream.peek() == ']') {
					read_stream.get(); // consume ]
					return array;
				}

				while (true) {
					if (!read_stream) {
						return UnexpectedEndOfInputError();
					}

					// Parse the next value.
					JSON value = parse();
					if (value.IsError()) {
						return value;
					}
					array.EmplaceBack(std::move(value));

					// We need to read either a , or a ]. otherwise parse error.
					ConsumeWS();
					if (read_stream.peek() == ',') {
						read_stream.get(); // consume ,
					}
					else if (read_stream.peek() == ']') {
						read_stream.get(); // consume ]
						return array;
					}
					else {
						return UnexpectedTokenError(read_stream.peek(), read_stream.tellg());
					}
				}
			}

			JSON __ParseLiteral() {
				char start = read_stream.peek();

				// Parse a String
				if (start == '"') {
					std::string str;
					if (!__TryParseString(str)) {
						return JSON::Error(error);
					}
					return JSON::String(str);
				}

				// Parse a Number (or Integer)
				else if (start == '-' || (start >= '0' && start <= '9')) {
					int64_t i;
					double d;
					bool is_integer;
					if (!__TryParseNumber(i, d, is_integer)) {
						return JSON::Error(error);
					}
					if (is_integer) {
						return JSON::Integer(i);
					}
					else {
						return JSON::Number(d);
					}
				}

				// Parse a Boolean
				else if (start == 't' || start == 'f') {
					bool boolean;
					if (!__TryParseBoolean(boolean)) {
						return JSON::Error(error);
					}
					return JSON::Boolean(boolean);
				}

				// Parse a Null
				else if (start == 'n') {
					if (!__TryParseNull()) {
						return JSON::Error(error);
					}
					return JSON::Null();
				}
				
				// Invalid Character
				else {
					return UnexpectedTokenError(read_stream.peek(), read_stream.tellg());
				}
			}

			/**
			 *
			 */
			bool __TryParseString(std::string &dest) {
				auto start_position = read_stream.tellg();
				char c; // XXX: Might need to be ReadStream::int_type

				read_stream.get(); // consume leading "
				while (true) {
					// If we can't read from the stream, die.
					if (!read_stream) {
						error = UnexpectedEndOfInputString();
						return false;
					}

					// We can use "get" here instead of "peek" because we will
					// ALWAYS consume this character.
					switch ((c = read_stream.get())) {

					// Escaped Character
					case '\\': {
						// Again, this character will ALWAYS be consumed.
						switch ((c = read_stream.get())) {
						case '"': dest += '"'; break;
						case '\\': dest += '\\'; break;
						case '/': dest += '/'; break;
						case 'b': dest += '\b'; break;
						case 'f': dest += '\f'; break;
						case 'n': dest += '\n'; break;
						case 'r': dest += '\r'; break;
						case 't': dest += '\t'; break;
						case 'u': {
							// TODO: read 4 characters into a buffer, then std::stoi(buffer, 0, 16);
							// Then cast to a character and insert into string. But we don't support
							// unicode yet, so just return false for now.
							error = "Unicode escaping is not supported.";
							return false; 
						} break;
						default: {
              // NOTE: We need to cast tellg() result to an int
              error = UnexpectedTokenString(c, (int)read_stream.tellg()-1);
							return false;
						}
						}
						
					} break;

					// End of String
					case '\"': {
						return true;
					} break; 

					// Regular Character
					default: {
						dest += c;
					}
					}
				}
			}

			bool __TryParseNumber(int64_t &i, double &d, bool &is_integer) {
				std::string base_buffer, exp_buffer;
				bool has_decimal;
				char c;

				// Need to break this into stages.
				// 1. Parse Integer portion
				// 2. Parse (optional) decimal portion
				// 3. Parse (optional) exponent portion
				// We stop parsing when we reach the end of the stage 
				// and the next character does not match the next expected
				// character(s). Or we finish stage 3.

				if (read_stream.peek() == '-') {
					base_buffer += read_stream.get();
				}
				if (read_stream.peek() == '0') {
					base_buffer += read_stream.get();
					goto parse_decimal;
				}

			parse_integer:
				while (true) {
					// If we can't read from the stream, die.
					if (!read_stream) {
						error = UnexpectedEndOfInputString();
						return false;
					}
					c = read_stream.peek();

					if (c >= '0' && c <= '9') {
						base_buffer += read_stream.get();
					}
					else {
						break;
					}
				}

			parse_decimal:
				if (read_stream.peek() == '.') {
					base_buffer += read_stream.get(); // consume .

					if (!(read_stream.peek() >= '0' && read_stream.peek() <= '9')) {
						// We have to have at least one digit.
						error = UnexpectedTokenString(read_stream.peek(), read_stream.tellg());
						return false;
					}

					has_decimal = true;
					while (true) {
						// If we can't read from the stream, die.
						if (!read_stream) {
							error = UnexpectedEndOfInputString();
							return false;
						}
						c = read_stream.peek();

						if (c >= '0' && c <= '9') {
							base_buffer += read_stream.get();
						}
						else {
							break;
						}
					}
				}

			parse_exponent:
				if (read_stream.peek() == 'e' || read_stream.peek() == 'E') {
					read_stream.get(); // consume e/E

					if (read_stream.peek() == '-' || read_stream.peek() == '+') {
						exp_buffer += read_stream.get();
					}

					if (!(read_stream.peek() >= '0' && read_stream.peek() <= '9')) {
						// We have to have at least one digit.
						error = UnexpectedTokenString(read_stream.peek(), read_stream.tellg());
						return false;
					}

					while (true) {
						// If we can't read from the stream, die.
						if (!read_stream) {
							error = UnexpectedEndOfInputString();
							return false;
						}
						c = read_stream.get();

						if (c >= '0' && c <= '9') {
							exp_buffer += c;
						}
						else {
							break;
						}
					}
				}

				// For simplicity, if we have an exponent or a decimal part, this is a double.
				if (has_decimal || !exp_buffer.empty()) {
					is_integer = false;
					d = std::stod(base_buffer);
					if (!exp_buffer.empty()) {
						d = d * std::pow(10, std::stoi(exp_buffer));
					}
				}
				else {
					is_integer = true;
					i = std::stol(base_buffer);
				}
				
				return true;
			}

			bool __TryParseNull() {
				int i = 0;
				const char *target = "null";

				while (i < 4) {
					if (read_stream.peek() != target[i++]) {
						error = UnexpectedTokenString(read_stream.peek(), read_stream.tellg());
						return false;
					}
					read_stream.get(); // consume
				}
				return true;
			}

			bool __TryParseBoolean(bool &b) {
				int i = 0, last_i;
				const char *target = nullptr;
				if (read_stream.peek() == 't') {
					last_i = 4;
					target = "true";
					b = true;
				}
				else {
					last_i = 5;
					target = "false";
					b = false;
				}

				while (i < last_i) {
					if (read_stream.peek() != target[i++]) {
						error = UnexpectedTokenString(read_stream.peek(), read_stream.tellg());
						return false;
					}
					read_stream.get(); // consume
				}

				return true;
			}
		};
	}
}
