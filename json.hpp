#pragma once

#include <stdint.h>
#include <map>
#include <vector>
#include <memory>
#include <cstring>
#include <type_traits>
#include <iostream>

namespace nbsdx {
namespace util {

class JSON;

enum class JSONValueType {
  Invalid,
	Error,
	Null,
	Object,
	Array,
	String,
	Number,
	Integer,
	Boolean
};

union JSONValue {
	int64_t integer;
	double number;
	bool boolean;
	std::string *string;
	std::map<std::string, JSON> *object;
	std::vector<JSON> *array;
};

class JSONReference {
	JSON *ref;
public:
	JSONReference(JSON *ref=nullptr) : ref(ref) {}

	operator bool() const {
		return ref != nullptr;
	}

	JSON* operator->() const {
		return ref;
	}

	JSON& operator*() const {
		return *ref;
	}
};

class JSON {
	JSONValueType type;
	JSONValue value;

private:

	JSON(JSONValueType _type)
		: type(_type)
	{
		switch (type) {
		case JSONValueType::Object: {
			value.object = new std::map<std::string, JSON>();
		} break;
		case JSONValueType::Array: {
			value.array = new std::vector<JSON>();
		} break;
		case JSONValueType::String:
		case JSONValueType::Error: {
			value.string = new std::string();
		} break;
		default: {
			std::memset(&value, 0, sizeof(JSONValue));
		}
		}
	}

	JSON DeepClone() const {
		JSON clone(type);

		switch (type) {
		case JSONValueType::Object: {
			for (const auto &entry : *value.object) {
				(*clone.value.object)[entry.first] = entry.second.DeepClone();
			}
		} break;
		case JSONValueType::Array: {
			for (const auto &val : *value.array) {
				clone.value.array->push_back(val.DeepClone());
			}
		} break;
		case JSONValueType::String:
		case JSONValueType::Error: {
			clone.value.string->assign(*value.string);
		} break;
		default: {
			clone.value = value;
		}
		}
		return clone;
	}

public: // Static Functions

	static JSON Object() {
		return JSON(JSONValueType::Object);
	}

	static JSON Array() {
		return JSON(JSONValueType::Array);
	}

	static JSON Null() {
		return JSON(JSONValueType::Null);
	}

	static JSON String(const std::string &str = "") {
		JSON j(JSONValueType::String);
		j.value.string->assign(str);
		return j;
	}

	static JSON Number(double value = 0.0) {
		JSON j(JSONValueType::Number);
		j.value.number = value;
		return j;
	}

	static JSON Integer(int64_t value = 0) {
		JSON j(JSONValueType::Integer);
		j.value.integer = value;
		return j;
	}

	static JSON Boolean(bool value = false) {
		JSON j(JSONValueType::Boolean);
		j.value.boolean = value;
		return j;
	}

	static JSON Error(const std::string &error) {
		JSON j(JSONValueType::Error);
		j.value.string->assign(error);
		return j;
	}

public: // Non-Static Functions

	JSON() : type(JSONValueType::Null) {}

	JSON(const JSON &other) : JSON()
	{
		// so dirty
		*this = other;
	}

	JSON(JSON &&other)
		: type(other.type)
		, value(other.value)
	{
		other.type = JSONValueType::Invalid;
	}

	~JSON() {
		switch (this->type) {
		case JSONValueType::Invalid: {
			return;
		} break;

		case JSONValueType::Object: {
			delete this->value.object;
		} break;

		case JSONValueType::Array: {
			delete this->value.array;
		} break;

		case JSONValueType::String:
		case JSONValueType::Error: {
			delete this->value.string;
		} break;
		}

		this->type = JSONValueType::Invalid;
	}

	const JSON& operator=(JSON &&other) {
		type = other.type;
		value = other.value;
		other.type = JSONValueType::Invalid;
		other.value.integer = 0;
		return *this;
	}

	const JSON& operator=(const JSON &other) {
		// mmmm this is dirty
		this->~JSON();
		*this = other.DeepClone();
return *this;
	}

	/// Function for reading/writing JSON values.

	inline JSONValueType Type() const { return type; }

	inline bool IsError() const { return type == JSONValueType::Error; }
	inline bool IsNull() const { return type == JSONValueType::Null; }
	inline bool IsObject() const { return type == JSONValueType::Object; }
	inline bool IsArray() const { return type == JSONValueType::Array; }
	inline bool IsNumber() const { return type == JSONValueType::Number; }
	inline bool IsInteger() const { return type == JSONValueType::Integer; }
	inline bool IsString() const { return type == JSONValueType::String; }
	inline bool IsBoolean() const { return type == JSONValueType::Boolean; }

	int64_t AsInteger() const {
		return IsInteger() ? value.integer : 0;
	}

	double AsNumber() const {
		return IsNumber() ? value.number : 0;
	}

	bool AsBoolean() const {
		return IsBoolean() ? value.boolean : false;
	}

	const std::string& AsString() const {
    return *value.string;
		//return IsString() ? *value.string : std::move(std::string(""));
	}

	const std::string& ErrorMessage() const {
    return *value.string;
		//return IsError() ? (*value.string) : throw this;// std::move(std::string(""));
	}

	size_t Length() const {
		if (type != JSONValueType::Array) {
			return -1;
		}
		return value.array->size();
	}


  template <typename T>
    typename std::enable_if<std::is_convertible<T,std::string>::value, JSONReference>::type
	operator[](const std::string &key) const {
		if (type != JSONValueType::Object) {
			return nullptr;
		}

		auto search = value.object->find(key);
		if (search == value.object->end()) {
			return nullptr;
		}
		else {
			return &search->second;
		}
	}

  template <typename T>
    typename std::enable_if<std::is_integral<T>::value, JSONReference>::type
	operator[](T index) const {
		if (type != JSONValueType::Array) {
			return nullptr;
		}

		if (index >= value.array->size() || index < 0) {
			return nullptr;
		}

		return &(value.array->operator[](index));
	}

/// Functions that are allowed to modify the JSON obejct.

  template <typename T>
    typename std::enable_if<std::is_convertible<T,std::string>::value, JSON&>::type
	operator[](T key) {
		if (!IsObject()) {
			// haha we're an object now.
			this->~JSON();
			*this = JSON::Object();
		}

		return value.object->operator[](std::string(key));
	}

	void Emplace(const std::string &key, JSON &&val) {
		if (type != JSONValueType::Object) {
			return;
		}
		value.object->emplace(key, val);
	}

	void EmplaceBack(JSON &&val) {
		if (type != JSONValueType::Array) {
			return;
		}
		value.array->emplace_back(val);
	}

/// Debug/Utility functions

	std::string ToString(bool pretty=false, unsigned indent=1, unsigned depth=0) const {
		std::ostringstream stream;

		//std::string padding(" "); // TODO: Fix

		switch (type) {
    case JSONValueType::Error:
      stream << "JSON Error: " << *value.string;
      break;

		case JSONValueType::Object: {
			bool first = true;
			stream << '{';
			//if (pretty) stream << '\n';
			for (const auto &entry : *value.object) {
				//if (pretty) stream;
				if (first) { first = false; }
				else { stream << ','; }
				stream << __EncodeJSONString(entry.first) << ':' << entry.second.ToString();
			}
			stream << '}';
		} break;

		case JSONValueType::Array: {
			bool first = true;
			stream << '[';
			//if (pretty) stream << '\n';
			for (const auto &val : *value.array) {
				//if (pretty) stream << padding;
				if (first) { first = false; }
				else { stream << ','; }
				stream << val.ToString();
			}
			stream << ']';
		} break;

		case JSONValueType::String:
			stream << __EncodeJSONString(*value.string);
			break;
		case JSONValueType::Number:
			stream << value.number;
			break;
		case JSONValueType::Integer:
			stream << value.integer;
			break;
		case JSONValueType::Boolean:
			stream << (value.boolean ? "true" : "false");
			break;
		case JSONValueType::Null:
			stream << "null";
			break;
		}

		return stream.str();
	}

	void print() const {
		std::cout << "JSON <Type: " << (int)type << ", Opaque: " << value.object;
		switch (type) {

		case JSONValueType::Object:
			std::cout << ", Object: [Keys: ";
			for (const auto &entry : *value.object) {
				std::cout << entry.first << ",";
			}
			std::cout << "]";
			break;
		case JSONValueType::Array:
			std::cout << ", Array: [Length: " << value.array->size() << "]";
			break;
		case JSONValueType::String:
			std::cout << ", String: " << *value.string;
			break;
		case JSONValueType::Error:
			std::cout << ", Error: " << *value.string;
			break;
		case JSONValueType::Number:
			std::cout << ", Number: " << value.number;
			break;
		case JSONValueType::Integer:
			std::cout << ", Integer: " << value.integer;
			break;
		case JSONValueType::Boolean:
			std::cout << ", Boolean: " << value.boolean;
			break;
		case JSONValueType::Null:
			std::cout << ", Null";
			break;
		}
		std::cout << ">\n";
	}

private:
	std::string __EncodeJSONString(const std::string &raw) const {
		std::ostringstream stream;
		stream << '"';
		for (auto c : raw) {
			switch (c) {
			case '"': stream << "\\\""; break;
			case '\\': stream << "\\\\"; break;
			case '/': stream << "\\/"; break;
			case '\b': stream << "\\b"; break;
			case '\f': stream << "\\f"; break;
			case '\n': stream << "\\n"; break;
			case '\r': stream << "\\r"; break;
			case '\t': stream << "\\t"; break;
				// TODO: unicode???
			default:
				stream << c;
			}
		}
		stream << '"';
		return stream.str();
	}
};

}
}



