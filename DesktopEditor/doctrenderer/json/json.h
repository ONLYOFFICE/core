#ifndef JSON_H_
#define JSON_H_

#include <string>
#include <initializer_list>
#include <memory>

#ifdef JSBASE_NO_USE_DYNAMIC_LIBRARY
#define JSON_DECL
#else
#include "../../common/base_export.h"
#ifdef JSBASE_USE_DYNAMIC_LIBRARY_BUILDING
#define JSON_DECL Q_DECL_EXPORT
#else
#define JSON_DECL Q_DECL_IMPORT
#endif
#endif

namespace NSJSON
{
	typedef unsigned char BYTE;

	class CValueContainer;
	// Main class for storing values.
	// This class provide interface to work with each type of values.
	class JSON_DECL CValue
	{
	public:
		CValue();

	public:
		// TYPE CHECKS
		/**
		 * Returns true if the value is undefined.
		 */
		bool IsUndefined() const;
		/**
		 * Returns true if the value is null.
		 */
		bool IsNull() const;
		/**
		 * Returns true if the value is a boolean value.
		 */
		bool IsBool() const;
		/**
		 * Returns true if the value is an integer.
		 */
		bool IsInt() const;
		/**
		 * Returns true if the value is a double value.
		 */
		bool IsDouble() const;
		/**
		 * Returns true if the value is a string.
		 */
		bool IsStringA() const;
		/**
		 * Returns true if the value is a wstring.
		 */
		bool IsStringW() const;
		/**
		 * Returns true if the value is an array.
		 */
		bool IsArray() const;
		/**
		 * Returns true if the value is a typed array.
		 */
		bool IsTypedArray() const;
		/**
		 * Returns true if the value is an object.
		 */
		bool IsObject() const;

		// FUNCTIONS FOR WORKING WITH PRIMITIVE VALUES
		/**
		 * Converts this value to a boolean value.
		 * @returns Corresponding boolean value. If the value is not a boolean, returns false instead.
		 */
		bool ToBool() const;
		/**
		 * Converts this value to an integer.
		 * @returns Corresponding integer value. If the value is not a number, returns 0 instead.
		 */
		int ToInt() const;
		/**
		 * Converts this value to a double value.
		 * @returns Corresponding double value. If the value is not a number, returns 0.0 instead.
		 */
		double ToDouble() const;
		/**
		 * Converts this value to a std::string.
		 * @returns Corresponding std::string. If the value is not a string, returns empty string instead.
		 */
		std::string ToStringA() const;
		/**
		 * Converts this value to a wstring.
		 * @returns Corresponding std::wstring. If the value is not a string, returns empty string instead.
		 */
		std::wstring ToStringW() const;

		// Type cast operators work the same way as conversion functions
		operator bool() const;
		operator int() const;
		operator double() const;
		operator std::string() const;
		operator std::wstring() const;

		// Creates a value from primitive types
		CValue(bool value);
		CValue(int value);
		CValue(double value);
		CValue(const char* value);
		CValue(const std::string& value);
		CValue(const wchar_t* value);
		CValue(const std::wstring& value);

		// Assigns primitive
		CValue& operator=(bool value);
		CValue& operator=(int value);
		CValue& operator=(double value);
		CValue& operator=(const char* value);
		CValue& operator=(const std::string& value);
		CValue& operator=(const wchar_t* value);
		CValue& operator=(const std::wstring& value);

		// FUNCTIONS FOR WORKING WITH ARRAYS
		/**
		 * Gets lengths of the array/typed array.
		 * @returns Returns the number of elements in the array or number of bytes in typed array. If current value is not an array/typed array, returns 0.
		 */
		int GetCount() const;

		/**
		 * Gets an array value by its index.
		 * @param index The index of the array value.
		 * @returns the value in the array. If current value is not an array, returns undefined value.
		 */
		const CValue Get(int index) const;
		CValue Get(int index);

		// operators [] works the same way as Get(index)
		const CValue operator[](int index) const;
		CValue operator[](int index);

		/**
		 * Creates an array with initializer list syntax (CValue arr = {1, 2, 3}).
		 * @param elements The elements of an array as an std::initializer_list.
		 */
		CValue(std::initializer_list<CValue> elements);
		/**
		 * Creates and returns new array.
		 * @param count The number of elements reserved for the array.
		 */
		static CValue CreateArray(int count);

		// FUNCTIONS FOR WORKING WITH TYPED ARRAYS
		/**
		 * Gets data of typed array.
		 * @return the pointer to memory, allocated for this typed array. If current value is not a typed array, returns nullptr.
		 */
		const BYTE* GetData() const;
		BYTE* GetData();

		/**
		 * Creates and returns new typed array.
		 * @param data The pointer to binary data. The pointer should be acquired with AllocTypedArray()!
		 * @param count The length of an array in bytes.
		 * @param isExternalize If true the memory block will not be reclaimed when the created typed array is destroyed.
		 * If this parameter is false then the memory block will be released using FreeTypedArray() during the typed array destruction.
		 */
		static CValue CreateTypedArray(BYTE* data, int count, bool isExternalize = true);
		/**
		 * Allocates the memory for a typed array by creating a buffer array of the specified size.
		 * @param size The buffer array size.
		 */
		static BYTE* AllocTypedArray(size_t size);
		/**
		 * Frees the memory for a typed array.
		 * @param data The allocated memory to be released.
		 * @param size The buffer array size.
		 */
		static void FreeTypedArray(BYTE* data, size_t size);

		// FUNCTIONS FOR WORKING WITH OBJECTS
		/**
		 * Gets a property of this object.
		 * @param name The name of the property.
		 * @returns the value of the object's property. If current value is not an object returns undefined value.
		 */
		const CValue Get(const char* name) const;
		CValue Get(const char* name);

		// operators [] works the same way as Get(name)
		const CValue operator[](const char* name) const;
		CValue operator[](const char* name);

		/**
		 * Creates and returns empty object.
		 */
		static CValue CreateObject();

		// OTHER FUNCTIONS
		/**
		 * Creates and returns undefined value (the same as CValue()).
		 */
		static CValue CreateUndefined();
		/**
		 * Creates and returns null value.
		 */
		static CValue CreateNull();

	private:
		std::shared_ptr<CValueContainer> m_internal;
	};
}

#endif	// JSON_H_
