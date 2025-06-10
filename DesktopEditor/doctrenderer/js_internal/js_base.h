#ifndef _CORE_EXT_JS_BASE_H_
#define _CORE_EXT_JS_BASE_H_

#include "../../common/File.h"
#include "../../common/Array.h"
#include "../../../OOXML/Base/SmartPtr.h"
#include "../../graphics/BaseThread.h"

#include <functional>

// disable export (ios/android problem (external embed objects))
//#define JSBASE_NO_USE_DYNAMIC_LIBRARY

#ifdef JSBASE_NO_USE_DYNAMIC_LIBRARY
#define JS_DECL
#else
#include "../../common/base_export.h"
#ifdef JSBASE_USE_DYNAMIC_LIBRARY_BUILDING
#define JS_DECL Q_DECL_EXPORT
#else
#define JS_DECL Q_DECL_IMPORT
#endif
#endif

#define JS_VALUE_EXIST(value) (value.is_init() && !value->isNull() && !value->isUndefined())
#define JS_IS_VALUE_ARRAY(value) (value.is_init() && !value->isNull() && !value->isUndefined() && value->isArray())

template <class T>
using JSSmart = NSCommon::smart_ptr<T>;

namespace NSJSBase
{
	class CJSObject;
	class CJSArray;
	class CJSTypedArray;
	class CJSFunction;

	/**
	 * The class represents a wrapper for primitive values and objects in JS.
	 * Objects of this class are used to interact with variables in JS contexts.
	 */
	class JS_DECL CJSValue
	{
	public:
		CJSValue();
		virtual ~CJSValue();
		/**
		 * Returns true if the value is undefined.
		 */
		virtual bool isUndefined()          = 0;
		/**
		 * Returns true if the value is null.
		 */
		virtual bool isNull()               = 0;
		/**
		 * Returns true if the value is a boolean value.
		 */
		virtual bool isBool()               = 0;
		/**
		 * Returns true if the value is a number.
		 */
		virtual bool isNumber()             = 0;
		/**
		 * Returns true if the value is a string.
		 */
		virtual bool isString()             = 0;
		/**
		 * Returns true if the value is an array.
		 */
		virtual bool isArray()              = 0;
		/**
		 * Returns true if the value is a typed array.
		 */
		virtual bool isTypedArray()         = 0;
		/**
		 * Returns true if the value is an object.
		 */
		virtual bool isObject()             = 0;
		/**
		 * Returns true if the value is a function.
		 */
		virtual bool isFunction()           = 0;
		/**
		 * Returns true if the value is empty.
		 */
		virtual bool isEmpty()              = 0;

		/**
		 * Makes this value undefined.
		 */
		virtual void doUndefined()          = 0;
		/**
		 * Makes this value null.
		 */
		virtual void doNull()               = 0;

		/**
		 * Converts the value to a boolean value.
		 */
		virtual bool toBool()               = 0;
		/**
		 * Converts the value to a 32-bit integer.
		 */
		virtual int toInt32()               = 0;
		/**
		 * Converts the value to a 32-bit unsigned integer.
		 */
		virtual unsigned int toUInt32()     = 0;
		/**
		 * Converts the value to a double value.
		 */
		virtual double toDouble()           = 0;
		/**
		 * Converts the value to an ASCII string.
		 */
		virtual std::string toStringA()     = 0;
		/**
		 * Converts the value to a wchar string.
		 */
		virtual std::wstring toStringW()    = 0;

		/**
		 * Converts the value to an object.
		 */
		virtual JSSmart<CJSObject> toObject()       = 0;
		/**
		 * Converts the value to an array.
		 */
		virtual JSSmart<CJSArray> toArray()         = 0;
		/**
		 * Converts the value to a typed array.
		 */
		virtual JSSmart<CJSTypedArray> toTypedArray() = 0;
		/**
		 * Converts the value to a function.
		 */
		virtual JSSmart<CJSFunction> toFunction()   = 0;
	};

	/**
	 * The base class for member of CJSEmbedObject class.
	 */
	class JS_DECL CJSEmbedObjectPrivateBase
	{
	public:
		CJSEmbedObjectPrivateBase();
		virtual ~CJSEmbedObjectPrivateBase();
	};

	/**
	 * The base class for member of CJSEmbedObject class.
	 * Inheritors of this class implement engine-specific behaviour for embedding external C++ class in a JS context.
	 */
	class JS_DECL CJSEmbedObjectAdapterBase
	{
	public:
		CJSEmbedObjectAdapterBase();
		virtual ~CJSEmbedObjectAdapterBase();
	};

	/**
	 * The class that used for getting arguments of the function called in a JS context.
	 */
	class JS_DECL CJSFunctionArguments
	{
	public:
		/**
		 * Returns number of the arguments.
		 */
		virtual int GetCount() = 0;
		/**
		 * Returns a value of the argument by its index.
		 * @param index The index of an argument.
		 */
		virtual JSSmart<CJSValue> Get(const int& index) = 0;
	};

	/**
	 * The base class that should be inherited by classes intended to be embedded in a JS context.
	 */
	class JS_DECL CJSEmbedObject
	{
	public:
		CJSEmbedObject();
		virtual ~CJSEmbedObject();

	public:
		/**
		 * Returns a pointer to the member of the embedded class.
		 */
		virtual void* getObject();
		/**
		 * Creates and returns an adapter for the embedded class (its implementation is generated).
		 */
		virtual CJSEmbedObjectAdapterBase* getAdapter();

	protected:
		CJSEmbedObjectPrivateBase* embed_native_internal;
		CJSEmbedObjectAdapterBase* m_pAdapter;

		friend class CJSEmbedObjectPrivateBase;
		friend class CJSEmbedObjectPrivate;
	};

	/**
	 * The class represents a wrapper for objects in JS.
	 */
	class JS_DECL CJSObject : public CJSValue
	{
	public:
		CJSObject();
		virtual ~CJSObject();

		/**
		 * Returns specified property of the object.
		 * @param name The name of a property.
		 */
		virtual JSSmart<CJSValue> get(const char* name)         = 0;
		/**
		 * Sets a property of the object.
		 * @param name The name of a property.
		 * @param value The value of a property.
		 */
		virtual void set(const char* name, JSSmart<CJSValue> value) = 0;
		virtual void set(const char* name, const int& value)    = 0;
		virtual void set(const char* name, const double& value) = 0;
		// Common function
		void set(const char* name, JSSmart<CJSObject> value);

		/**
		 * Returns a vector containing the names of the properties of this object as strings, including properties from prototype objects.
		 */
		virtual std::vector<std::string> getPropertyNames()     = 0;
		/**
		 * Returns a pointer to the native embedded object.
		 */
		virtual CJSEmbedObject* getNative()                     = 0;
		/**
		 * Calls a function of the object.
		 * @param name The name of a function to call.
		 * @param argc Number of arguments.
		 * @param argv The array of arguments.
		 * @return The value returned by function.
		 */
		virtual JSSmart<CJSValue> call_func(const char* name, const int argc = 0, JSSmart<CJSValue> argv[] = NULL) = 0;
		/**
		 * Converts the object to a value.
		 */
		virtual JSSmart<CJSValue> toValue()                     = 0;
	};

	/**
	 * The class represents a wrapper for arrays in JS.
	 */
	class JS_DECL CJSArray : public CJSValue
	{
	public:
		CJSArray();
		virtual ~CJSArray();

		/**
		 * Returns the number of elements.
		 */
		virtual int getCount()                                  = 0;
		/**
		 * Returns an array value by its index.
		 * @param index The index of the array value.
		 */
		virtual JSSmart<CJSValue> get(const int& index)         = 0;
		/**
		 * Sets an array value by its index.
		 * @param index The index of the array value.
		 * @param value The array value to be set.
		 */
		virtual void set(const int& index, JSSmart<CJSValue> value) = 0;
		virtual void set(const int& index, const int& value)    = 0;
		virtual void set(const int& index, const double& value) = 0;

		/**
		 * Add the specified value to the array.
		 * @param value The value to be added.
		 */
		virtual void add(JSSmart<CJSValue> value)               = 0;
		/**
		 * Add null to the array.
		 */
		virtual void add_null()                                 = 0;
		/**
		 * Add undefined to the array.
		 */
		virtual void add_undefined()                            = 0;
		/**
		 * Add a boolean value to the array.
		 * @param value The boolean value to be added.
		 */
		virtual void add_bool(const bool& value)                = 0;
		/**
		 * Add a byte of data to the array.
		 * @param value The byte to be added.
		 */
		virtual void add_byte(const BYTE& value)                = 0;
		/**
		 * Add an integer value to the array.
		 * @param value The integer value to be added.
		 */
		virtual void add_int(const int& value)                  = 0;
		/**
		 * Add a double value to the array.
		 * @param value The double value to be added.
		 */
		virtual void add_double(const double& value)            = 0;
		/**
		 * Add an ASCII string to the array.
		 * @param value The string to be added.
		 */
		virtual void add_stringa(const std::string& value)      = 0;
		/**
		 * Add a wchar string to the array.
		 * @param value The wstring to be added.
		 */
		virtual void add_string(const std::wstring& value)      = 0;
		/**
		 * Converts the array to a value.
		 */
		virtual JSSmart<CJSValue> toValue()                     = 0;
	};

	namespace NSAllocator
	{
		/**
		 * Engine-specific memory allocator.
		 * @param size The size of allocated memory in bytes.
		 * @return Pointer to the allocated memory. Returns nullptr if allocation was not successful.
		 */
		JS_DECL unsigned char* Alloc(const size_t& size);
		/**
		 * Engine-specific memory deallocator.
		 * @param data Pointer to the previously allocated with Alloc() memory.
		 * @param size The size of allocated memory.
		 */
		JS_DECL void Free(unsigned char* data, const size_t& size);
	}

	/**
	 * The class that stores raw chunks of binary data.
	 */
	class JS_DECL CJSDataBuffer
	{
	public:
		BYTE* Data;
		size_t Len;

		bool IsExternalize;

	public:
		CJSDataBuffer();

		/**
		 * Returns a pointer to the new allocated memory that contains a copy of the binary data.
		 */
		BYTE* Copy();
		/**
		 * Frees the memory for the binary data.
		 */
		void Free();
	};

	/**
	 * The class represents a wrapper for typed arrays in JS.
	 */
	class JS_DECL CJSTypedArray : public CJSValue
	{
	public:
		CJSTypedArray(BYTE* data = NULL, int count = 0);
		virtual ~CJSTypedArray();

		/**
		 * Returns the length of the typed array in bytes.
		 */
		virtual int getCount()          = 0;
		/**
		 * Returns the binary data stored in the typed array.
		 */
		virtual CJSDataBuffer getData() = 0;
		/**
		 * Converts the typed array to a value.
		 */
		virtual JSSmart<CJSValue> toValue() = 0;

		/**
		 * Detaches this ArrayBuffer and all its views (typed arrays).
		 * Detaching sets the byte length of the buffer and all typed arrays to zero,
		 * preventing JavaScript from ever accessing underlying backing store.
		 * ArrayBuffer should have been externalized.
		 */
		virtual void Detach() = 0;
	};

	/**
	 * The class represents a wrapper for functions in JS.
	 */
	class JS_DECL CJSFunction : public CJSValue
	{
	public:
		CJSFunction();
		virtual ~CJSFunction();

		/**
		 * Calls the function.
		 * @param recv The reciever of the function call.
		 * @param argc Number of arguments.
		 * @param argv The array of arguments.
		 * @return The value returned by the function.
		 */
		virtual JSSmart<CJSValue> Call(CJSValue* recv, int argc, JSSmart<CJSValue> argv[]) = 0;
	};

	/**
	 * The class for tracking exceptions during code execution in JS contexts.
	 */
	class JS_DECL CJSTryCatch
	{
	public:
		CJSTryCatch();
		virtual ~CJSTryCatch();

		/**
		 * Returns true if exception was caught and print some info about it to stdout.
		 */
		virtual bool Check() = 0;
	};

	using EmbedObjectCreator = CJSEmbedObject* (*)();

	/**
	 * The class for getting JS context instance for working with it.
	 */
	class CJSContextPrivate;
	class JS_DECL CJSContext
	{
	public:
		CJSContextPrivate* m_internal;

	public:
		CJSContext(const bool& bIsInitialize = true);
		~CJSContext();

		/**
		 * Initializes the JS context.
		 * By default it happens automatically when creating a CJSConext instance.
		 * @param snapshotPath Path to snapshot file.
		 */
		void Initialize(const std::wstring& snapshotPath = L"");
		/**
		 * Releases any resources taken by the JS context.
		 * Generally there is no need to call it manually, cause this method called when CJSConext is being destructed.
		 */
		void Dispose();

		/**
		 * Get information about snapshot
		 * @return Returns true if snapshot was used on Initialize method
		 */
		bool isSnapshotUsed();

		/**
		 * Creates and returns the pointer to an object for tracking exceptions during code execution in current JS context.
		 */
		JSSmart<CJSTryCatch> GetExceptions();
		/**
		 * Creates and returns the pointer to a global object for this JS context.
		 */
		JSSmart<CJSObject> GetGlobal();

		/**
		 * Enters the JS context and makes it the active context for further work.
		 * This method is called in the constructor of CJSContextScope class.
		 */
		void Enter();
		/**
		 * Exits the JS context, which restores the context that was in place when entering the current context.
		 * This method is called in the destructor of CJSContextScope class.
		 */
		void Exit();

		/**
		 * Сheck if context is current.
		 * This method is not safe.
		 */
		bool IsEntered();

		/**
		 * Embeds specified class in JS contexts.
		 * @tparam T Embedded class name.
		 * @param isAllowedInJS If true, user can create the embedded object instance from JS code using CreateEmbedObject().
		 * If this parameter is false, then user can do so only using JSContext::createEmbedObject().
		 */
		template<typename T>
		static void Embed(const bool& isAllowedInJS = true)
		{
			AddEmbedCreator(T::getName(), T::getCreator, isAllowedInJS);
		}

		/**
		 * Generate snapshot by script.
		 * @param script The script to be executed.
		 * @param snapshotPath The path to the snapshot
		 * @return Returns true whether a snapshot has been created.
		 */
		static bool generateSnapshot(const std::string& script, const std::wstring& snapshotPath);

		/**
		 * Run the script in the current JS context.
		 * @param script The script to be executed.
		 * @param exception The object for handling exceptions.
		 * @param scriptPath The path to the script, where cache files will be created for further usage.
		 * If scriptPath is not specified, cache is not used.
		 * @return The value returned from JS context after script execution.
		 */
		JSSmart<CJSValue> runScript(const std::string& script, JSSmart<CJSTryCatch> exception = NULL, const std::wstring& scriptPath = std::wstring(L""));
		/**
		 * Parses the JSON string and convert it to corresponding JS value.
		 * @param json_content The JSON string to be parsed.
		 * @return The pointer to resulted JS value after parsing.
		 */
		JSSmart<CJSValue> JSON_Parse(const char* json_content);
		/**
		 * Creates a string that contains the JSON-serialized representation of a JS value.
		 * @param value The JS value to serialize.
		 * @return The string that contains the result of serialization, or empty string in case of errors.
		 */
		std::string JSON_Stringify(JSSmart<CJSValue> value);
		/**
		 * Do not use this function. It is for internal needs.
		 * Associates current context with the specifed thread id.
		 * @param id The id of a thread.
		 */
		void MoveToThread(ASC_THREAD_ID* id = NULL);

		/**
		 * Do not use this function manually. It is called from CJSContext::Embed method.
		 * Adds a creator object for corresponding embedded class name.
		 * @param name The name of an embedded class.
		 * @param creator The creator function for an embedded class.
		 * @param isAllowedInJS Specifies whether user can create the embedded object from JS code or not.
		 */
		static void AddEmbedCreator(const std::string& name, EmbedObjectCreator creator, const bool& isAllowedInJS = true);

		// NOTE: All raw pointers obtained from the following functions, should be deleted by caller.
	public:
		/**
		 * Creates and returns undefined JS value in current context.
		 */
		static CJSValue* createUndefined();
		/**
		 * Creates and returns null JS value in current context.
		 */
		static CJSValue* createNull();
		/**
		 * Creates and returns a boolean JS value in current context.
		 * @param value The boolean value.
		 */
		static CJSValue* createBool(const bool& value);
		/**
		 * Creates and returns an integer JS value in current context.
		 * @param value The integer value.
		 */
		static CJSValue* createInt(const int& value);
		/**
		 * Creates and returns an unsigned integer JS value in current context.
		 * @param value The unsigned integer value.
		 */
		static CJSValue* createUInt(const unsigned int& value);
		/**
		 * Creates and returns a double JS value in current context.
		 * @param value The double value.
		 */
		static CJSValue* createDouble(const double& value);
		/**
		 * Creates and returns a JS string in current context.
		 * @param value The pointer to a null terminated char array.
		 * @param length The length of a created string. By default the whole string will be created.
		 */
		static CJSValue* createString(const char* value, const int& length = -1);
		/**
		 * Creates and returns a JS string in current context.
		 * @param value The pointer to a null terminated wchar_t array.
		 * @param length The length of a created string. By default the whole string will be created.
		 */
		static CJSValue* createString(const wchar_t* value, const int& length = -1);
		/**
		 * Creates and returns a JS string in current context.
		 * @param value The ASCII string.
		 */
		static CJSValue* createString(const std::string& value);
		/**
		 * Creates and returns a JS string in current context.
		 * @param value The wchar string.
		 */
		static CJSValue* createString(const std::wstring& value);

		/**
		 * Creates and returns an empty JS object in current context.
		 */
		static CJSObject* createObject();

		/**
		 * Creates and returns a JS array of specified length in current context.
		 * @param count The length of an array.
		 */
		static CJSArray* createArray(const int& count);

		/**
		 * Creates and returns a JS typed array in current context.
		 * @param data The pointer to binary data.
		 * @param count The length of an array in bytes.
		 * @param isExternalize If true the memory block will not be reclaimed when the created JS array is destroyed.
		 * If this parameter is false then the memory block will be released using NSAllocator::Free function when the JS typed array is destroyed.
		 */
		static CJSTypedArray* createUint8Array(BYTE* data = NULL, int count = 0, const bool& isExternalize = true);

		/**
		 * Creates and returns a JS typed array in current context from the specified binary file.
		 * @param sFilePath The path to a binary file.
		 * @return The JS typed array as JS value. Or null value if the binary file was not found.
		 */
		static CJSValue* createUint8Array(const std::wstring& sFilePath);

		/**
		 * Creates and returns a JS object, which is a wrapper around specified embedded class.
		 * @param name The name of an embedded class.
		 */
		static JSSmart<CJSObject> createEmbedObject(const std::string& name);

	public:
		/**
		 * Returns a copy of the last entered JS context.
		 * Do not use Enter, Exit or any scope-changing operations on this object.
		 * If you need so, use original CJSContext instance for this purposes.
		 */
		static JSSmart<CJSContext> GetCurrent();

	public:
		/**
		 * Sets directory for external initialization.
		 * @param sDirectory The directory path.
		 */
		static void ExternalInitialize(const std::wstring& sDirectory);
		/**
		 * Externally reclaim any resources for all JS contexts.
		 */
		static void ExternalDispose();
		/**
		 * Returns true if typed arrays are supported natively on current platform.
		 */
		static bool IsSupportNativeTypedArrays();
	};


	/**
	 * The class that sets a new scope for any local variables created after its construction.
	 * In the destructor of the class this scope and all local variables created in it will be removed.
	 */
	class CJSLocalScopePrivate;
	class JS_DECL CJSLocalScope
	{
	public:
		CJSLocalScopePrivate* m_internal;

	public:
		CJSLocalScope();
		~CJSLocalScope();
	};

	/**
	 * The class that sets execution context for further work.
	 * This class can be used instead of Enter() and Exit() methods, cause it calls them on its creation and destruction, respectively.
	 */
	class JS_DECL CJSContextScope
	{
	public:
		JSSmart<CJSContext> m_context;

	public:
		CJSContextScope(JSSmart<CJSContext> context);
		~CJSContextScope();
	};
}

// Macro for embedding
#define DECLARE_EMBED_METHODS										\
	static std::string getName();									\
	static CJSEmbedObject* getCreator();							\
	virtual CJSEmbedObjectAdapterBase* getAdapter() override;

/**
 * If you want to embed your external C++ class in a JS context then do the following:
 *
 * 1. Make sure that your class is inherit CJSEmbedObject class.
 * 2. Override `getObject()` method and write its implementation if needed.
 * 3. Use macro `DECLARE_EMBED_METHODS` in the `public` section of your class declaration.
 * 4. Include "doctrenderer/js_internal/js_base_embed.pri" in your .pro file.
 * 5. Add a line `ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(YourClassHeader.h)` to your .pro file.
 * 6. Run the script "doctrenderer/embed/embed.py" with the name of your class header as an argument for generating some necessary files for embedding.
 *	  If you're going to embed a class for doctrenderer library, then also specify `--internal` (or just `-i`) option.
 * 7. In C++ code call `CJSContext::Embed<YourClassName>()`.
 *
 * Then you can call `CreateEmbedOjbect('YourClassName')` in JS code or `CJSContext::createEmbedObject('YourClassName')` in C++ code
 * to get a JS object-wrapper for the embedded class.
 *
 * NOTE: If you don't want to export certain functions from your embedded class for some reason,
 *       then add the inline comment "[noexport]" at the start of a function declaration.
 *       Also you can use `#ifdef ... #endif` blocks (see doctrenderer/test/embed/external/Embed.h for an example).
 */

#endif // _CORE_EXT_JS_BASE_H_
