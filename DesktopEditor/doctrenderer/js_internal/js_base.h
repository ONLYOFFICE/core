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

		virtual bool isUndefined()          = 0;
		virtual bool isNull()               = 0;
		virtual bool isBool()               = 0;
		virtual bool isNumber()             = 0;
		virtual bool isString()             = 0;
		virtual bool isArray()              = 0;
		virtual bool isTypedArray()         = 0;
		virtual bool isObject()             = 0;
		virtual bool isFunction()           = 0;
		virtual bool isEmpty()              = 0;

		virtual void doUndefined()          = 0;
		virtual void doNull()               = 0;

		virtual bool toBool()               = 0;
		virtual int toInt32()               = 0;
		virtual unsigned int toUInt32()     = 0;
		virtual double toDouble()           = 0;
		virtual std::string toStringA()     = 0;
		virtual std::wstring toStringW()    = 0;

		virtual CJSObject* toObject()       = 0;
		virtual CJSArray* toArray()         = 0;
		virtual CJSTypedArray* toTypedArray() = 0;
		virtual CJSFunction* toFunction()   = 0;

		virtual JSSmart<CJSObject> toObjectSmart();
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
		virtual int GetCount() = 0;
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

		virtual CJSValue* get(const char* name)                 = 0;
		virtual void set(const char* name, CJSValue* value)     = 0;

		virtual void set(const char* name, const int& value)    = 0;
		virtual void set(const char* name, const double& value) = 0;

		virtual CJSEmbedObject* getNative()                     = 0;

		virtual JSSmart<CJSValue> call_func(const char* name, const int argc = 0, JSSmart<CJSValue> argv[] = NULL) = 0;
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

		virtual int getCount()                                  = 0;
		virtual JSSmart<CJSValue> get(const int& index)         = 0;
		virtual void set(const int& index, CJSValue* value)     = 0;
		virtual void set(const int& index, const bool& value)   = 0;
		virtual void set(const int& index, const int& value)    = 0;
		virtual void set(const int& index, const double& value) = 0;

		virtual void add(CJSValue* value)                       = 0;
		virtual void add_null()                                 = 0;
		virtual void add_undefined()                            = 0;
		virtual void add_bool(const bool& value)                = 0;
		virtual void add_byte(const BYTE& value)                = 0;
		virtual void add_int(const int& value)                  = 0;
		virtual void add_double(const double& value)            = 0;
		virtual void add_stringa(const std::string& value)      = 0;
		virtual void add_string(const std::wstring& value)      = 0;
		virtual JSSmart<CJSValue> toValue()                     = 0;
	};

	namespace NSAllocator
	{
		unsigned char* Alloc(const size_t& size);
		void Free(unsigned char* data, const size_t& size);
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
		BYTE* Copy();
		CJSDataBuffer();
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

		virtual int getCount()          = 0;
		virtual CJSDataBuffer getData() = 0;
		virtual JSSmart<CJSValue> toValue() = 0;
	};

	/**
	 * The class represents a wrapper for functions in JS.
	 */
	class JS_DECL CJSFunction : public CJSValue
	{
	public:
		CJSFunction();
		virtual ~CJSFunction();

		virtual CJSValue* Call(CJSValue* recv, int argc, JSSmart<CJSValue> argv[]) = 0;
	};

	/**
	 * The class for tracking exceptions during code execution in JS contexts.
	 */
	class JS_DECL CJSTryCatch
	{
	public:
		CJSTryCatch();
		virtual ~CJSTryCatch();
		virtual bool Check() = 0;
	};

	using EmbedObjectCreator = CJSEmbedObject* (*)();

	enum IsolateAdditionalDataType {
		iadtSingletonNative = 0,
		iadtUndefined = 255
	};


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

		void Initialize();
		void Dispose();

		CJSTryCatch* GetExceptions();

		CJSObject* GetGlobal();

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
		 * Embeds specified class in all JS contexts.
		 * You can then call `CreateEmbedOjbect('EmbeddedClassName')` for getting an instance of embedded class.
		 * @tparam T Embedded class name.
		 * @param type Specifies how an object of the embedded class will be created.
		 */
		template<typename T>
		static void Embed(const IsolateAdditionalDataType& type = iadtUndefined)
		{
			AddEmbedCreator(T::getName(), T::getCreator, type);
		}

		JSSmart<CJSValue> runScript(const std::string& script, JSSmart<CJSTryCatch> exception = NULL, const std::wstring& scriptPath = std::wstring(L""));
		CJSValue* JSON_Parse(const char* json_content);
		void MoveToThread(ASC_THREAD_ID* id = NULL);

		static void AddEmbedCreator(const std::string& name, EmbedObjectCreator creator, const IsolateAdditionalDataType& type = iadtUndefined);

	public:
		static CJSValue* createUndefined();
		static CJSValue* createNull();
		static CJSValue* createBool(const bool& value);
		static CJSValue* createInt(const int& value);
		static CJSValue* createUInt(const unsigned int& value);
		static CJSValue* createDouble(const double& value);
		static CJSValue* createString(const char* value, const int& length = -1);
		static CJSValue* createString(const wchar_t* value, const int& length = -1);
		static CJSValue* createString(const std::string& value);
		static CJSValue* createString(const std::wstring& value);

		static CJSObject* createObject();

		static CJSArray* createArray(const int& count);

		static CJSTypedArray* createUint8Array(BYTE* data = NULL, int count = 0, const bool& isExternalize = true);

		static CJSValue* createUint8Array(const std::wstring& sFilePath);

	public:
		static JSSmart<CJSContext> GetCurrent();

	public:
		static void ExternalInitialize(const std::wstring& sDirectory);
		static void ExternalDispose();
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
 * 2. Override `getObject()` method and write its implementation.
 * 3. Use macro `DECLARE_EMBED_METHODS` in the `public` section of your class declaration.
 * 4. Include "doctrenderer/js_internal/js_base_embed.pri" in your .pro file.
 * 5. Add a line `ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(YourClassHeader.h)` to your .pro file.
 * 6. Run the script "doctrenderer/embed/embed.py" with the name of your class header as an argument.
 *	  If you embedding a class for doctrenderer library, then also specify `--internal` (or just `-i`) option.
 */

#endif // _CORE_EXT_JS_BASE_H_
