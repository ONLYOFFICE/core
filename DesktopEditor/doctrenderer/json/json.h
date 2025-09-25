#ifndef JSON_H_
#define JSON_H_

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#ifndef JSON_DECL
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
#endif

// uncomment to enable exceptions throwing
// #define JSON_DEBUG

#ifdef JSON_DEBUG
#include <stdexcept>
#endif

namespace NSJSON
{
	typedef unsigned char BYTE;

	enum class ImageFormat
	{
		ifRGBA,
		ifBGRA,
		ifARGB,
		ifInvalid
	};
	
	class CValue;
	class CValueRef;
	class CTypedValue;
	// Main value interface.
	// This class provide interface to work with each type of values.
	class JSON_DECL IValue
	{
	protected:
		IValue();
		IValue(const std::shared_ptr<CTypedValue>& internal);
		~IValue();

		// Disable copy for this class (implemented in heirs)
		IValue(const IValue& other) = delete;
		IValue& operator=(const IValue& other) = delete;

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
		 * Returns true if the value is not undefined or null.
		 */
		bool IsInit() const;
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
		bool IsString() const;
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
		/**
		 * Returns true if the value is an image.
		 */
		bool IsImage() const;

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

	public:
		// JSON CONVERSION
		/**
		 * Converts this value to a JSON-string.
		 * @return The JSON-string or empty string in case of errors.
		 */
		std::string ToJSON();

	protected:
		// Creates a value from primitive types
		IValue(bool value);
		IValue(int value);
		IValue(double value);
		IValue(const char* value);
		IValue(const std::string& value);
		IValue(const wchar_t* value);
		IValue(const std::wstring& value);

	public:
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
		const CValueRef Get(int index) const;
		CValueRef Get(int index);

		// operators [] works the same way as Get(index)
		const CValueRef operator[](int index) const;
		CValueRef operator[](int index);

	protected:
		// create array from initializer list
		IValue(std::initializer_list<CValue> elements);

	public:
		// FUNCTIONS FOR WORKING WITH TYPED ARRAYS
		/**
		 * Gets data of typed array.
		 * @return the pointer to memory, allocated for this typed array. If current value is not a typed array, returns nullptr.
		 */
		const BYTE* GetData() const;
		BYTE* GetData();

		// FUNCTIONS FOR WORKING WITH OBJECTS
		/**
		 * Gets a property of this object.
		 * @param name The name of the property.
		 * @returns the value of the object's property. If current value is not an object, returns undefined value.
		 */
		const CValueRef Get(const char* name) const;
		CValueRef Get(const char* name);

		// operators [] works the same way as Get(name)
		const CValueRef operator[](const char* name) const;
		CValueRef operator[](const char* name);

		/**
		 * Retrieves all property names from current object.
		 * @returns a vector containing the names of the properties of this object as strings. If current value is not an object, returns an empty vector.
		 */
		std::vector<std::string> GetPropertyNames() const;

		// FUNCTIONS FOR WORKING WITH IMAGES
		/**
		 * Gets bits of image.
		 * @return the pointer to memory, allocated for the image. If current value is not an image, returns nullptr.
		 */
		const BYTE* GetImageBits() const;
		BYTE* GetImageBits();
		/**
		 * Gets width of the image.
		 * @returns Returns the width of the image. If current value is not an image, returns 0.
		 */
		int GetImageWidth() const;
		/**
		 * Gets height of the image.
		 * @returns Returns the height of the image. If current value is not an image, returns 0.
		 */
		int GetImageHeight() const;
		/**
		 * Gets format of the image.
		 * @returns Returns the image format. If current value is not an image, returns ImageFormat::ifInvalid.
		 */
		ImageFormat GetImageFormat() const;

		/**
		 * Make image bits external.
		 */
		void ImageExternalize();

		/**
		 * Alloc image bits as internal.
		 */
		void ImageAlloc(const int& width, const int& height, const ImageFormat& format);

	protected:
		std::shared_ptr<CTypedValue> m_internal;
	};

	// Main value implementation
	class JSON_DECL CValue : public IValue
	{
	public:
		CValue();
		CValue(const CValue& other);
		CValue(const CValueRef& ref);
		~CValue();

		CValue& operator=(const CValue& other);
		CValue& operator=(const CValueRef& ref);

		// PRIMITIVES CONSTRUCTORS
		CValue(bool value);
		CValue(int value);
		CValue(double value);
		CValue(const char* value);
		CValue(const std::string& value);
		CValue(const wchar_t* value);
		CValue(const std::wstring& value);

		// ARRAY CONSTRUCTORS
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

		// TYPED ARRAY
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

		// IMAGE
		/**
		 * Creates and returns new image object.
		 * @param bits The pointer to image data. The pointer should be acquired with AllocImageBits().
		 * @param width The width of the image.
		 * @param height The height of the image.
		 * @param format The format of the image.
		 * @param isExternalize If true the memory will not be reclaimed when the created image is destroyed.
		 * If this parameter is false then the memory will be released using FreeImageBits() during the image object destruction.
		 */
		static CValue CreateImage(BYTE* bits, int width, int height, ImageFormat format = ImageFormat::ifBGRA, bool isExternalize = true);
		static CValue CreateEmptyImage(ImageFormat format = ImageFormat::ifBGRA);
		/**
		 * Allocates the memory for an image.
		 * @param width The width of the image.
		 * @param height The height of the image.
		 */
		static BYTE* AllocImageBits(int width, int height);
		/**
		 * Frees the memory for a image bits.
		 * @param data The allocated memory to be released.
		 */
		static void FreeImageBits(BYTE* bits);

		// OBJECT CONSTRUCTOR
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

		// JSON CONVERSION
		/**
		 * Creates and returns new value from JSON-string.
		 * @param jsonString The JSON-string from which new value will be created.
		 * @returns created value. If JSON-string is invalid, value will be undefined.
		 */
		static CValue FromJSON(const std::string& jsonString);

		friend class CValueRef;
	};

	// Main value reference implementation
	class JSON_DECL CValueRef : public IValue
	{
	public:
		CValueRef(const CValueRef& other);
		CValueRef(const CValue& value);
		~CValueRef();

		CValueRef& operator=(const CValueRef& other);
		CValueRef& operator=(const CValue& value);

		friend class CValue;
	};
}

#endif	// JSON_H_
