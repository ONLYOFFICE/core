#include "v8_base.h"

v8::Local<v8::String> CreateV8String(v8::Isolate* i, const char* str, const int& len)
{
	return v8::String::NewFromUtf8(i, str, kV8NormalString, len).ToLocalChecked();
}
v8::Local<v8::String> CreateV8String(v8::Isolate* i, const std::string& str)
{
	return v8::String::NewFromUtf8(i, str.c_str(), kV8NormalString, (int)str.length()).ToLocalChecked();
}

std::wstring CV8Worker::m_sExternalDirectory = L"";

namespace NSJSBase
{
	class CCacheDataScript
	{
	private:
		BYTE* Data;
		int Length;

		std::wstring Path;

	public:
		CCacheDataScript(const std::wstring& sPath)
		{
			Data = NULL;
			Length = 0;

			Path = sPath;

#ifndef V8_VERSION_89_PLUS
			if (!sPath.empty())
			{
				BYTE* _data = NULL;
				DWORD _data_length = 0;
				if (NSFile::CFileBinary::ReadAllBytes(sPath, &_data, _data_length))
				{
					Data = _data;
					Length = (int)_data_length;
				}
			}
#endif
		}
		~CCacheDataScript()
		{
			RELEASEARRAYOBJECTS(Data);
		}

#ifdef V8_VERSION_89_PLUS
		v8::Local<v8::Script> Compile(const v8::Local<v8::Context>& _context, const v8::Local<v8::String>& source)
		{
			v8::Local<v8::Script> script;

			if (Path.empty())
			{
				// no cache
				v8::ScriptCompiler::Source oSource(source);
				v8::MaybeLocal<v8::Script> sctiptMB = v8::ScriptCompiler::Compile(_context, &oSource, v8::ScriptCompiler::kNoCompileOptions);
				if (!sctiptMB.IsEmpty())
					script = sctiptMB.ToLocalChecked();

				return script;
			}

			if (NSFile::CFileBinary::Exists(Path))
			{
				// load cache from file
				BYTE* _data = NULL;
				DWORD _data_length = 0;
				if (NSFile::CFileBinary::ReadAllBytes(Path, &_data, _data_length))
				{
					Data = _data;
					Length = (int)_data_length;
				}

				// compile with cache
				v8::ScriptCompiler::CachedData* pCacheData = new v8::ScriptCompiler::CachedData(Data, Length);
				v8::ScriptCompiler::Source oSource(source, pCacheData);

				v8::MaybeLocal<v8::Script> sctiptMB = v8::ScriptCompiler::Compile(_context, &oSource, v8::ScriptCompiler::kConsumeCodeCache);
				if (!sctiptMB.IsEmpty())
					script = sctiptMB.ToLocalChecked();
			}
			else
			{
				v8::ScriptCompiler::CachedData* pCacheData = nullptr;

				// save cache to file
				NSFile::CFileBinary oFileTest;
				if (oFileTest.CreateFileW(Path))
				{
					// create cache data
					v8::ScriptCompiler::Source oSource(source);
					v8::Local<v8::Script> pScriptCache = v8::ScriptCompiler::Compile(_context, &oSource, v8::ScriptCompiler::kNoCompileOptions).ToLocalChecked();
					pCacheData = v8::ScriptCompiler::CreateCodeCache(pScriptCache->GetUnboundScript());

					if (pCacheData)
					{
						// save cache to file
						NSFile::CFileBinary oFileTest;
						if (oFileTest.CreateFileW(Path))
						{
							oFileTest.WriteFile(pCacheData->data, (DWORD)pCacheData->length);
							oFileTest.CloseFile();
						}
					}
				}

				// compile with/without(if pCacheData === NULL) cache data
				v8::ScriptCompiler::Source oSource2(source, pCacheData);
				v8::ScriptCompiler::CompileOptions compileOptions = (nullptr == pCacheData) ? v8::ScriptCompiler::kNoCompileOptions : v8::ScriptCompiler::kConsumeCodeCache;

				v8::MaybeLocal<v8::Script> sctiptMB = v8::ScriptCompiler::Compile(_context, &oSource2, compileOptions);
				if (!sctiptMB.IsEmpty())
					script = sctiptMB.ToLocalChecked();
			}
			return script;
		}
#else
		v8::Local<v8::Script> Compile(const v8::Local<v8::Context>& _context, const v8::Local<v8::String>& source)
		{
			v8::Local<v8::Script> script;
			if (NULL == Data)
			{
				v8::ScriptCompiler::Source oSource(source);
				script = v8::ScriptCompiler::Compile(_context, &oSource, kV8ProduceCodeCache).ToLocalChecked();

				const v8::ScriptCompiler::CachedData* _cachedData = oSource.GetCachedData();
				NSFile::CFileBinary oFileTest;
				if (_cachedData && oFileTest.CreateFileW(Path))
				{
					oFileTest.WriteFile(_cachedData->data, (DWORD)_cachedData->length);
					oFileTest.CloseFile();
				}
			}
			else
			{
				v8::ScriptCompiler::CachedData* pCachedData = new v8::ScriptCompiler::CachedData(Data, Length);
				v8::ScriptCompiler::Source oSource(source, pCachedData);
				script = v8::ScriptCompiler::Compile(_context, &oSource, v8::ScriptCompiler::kConsumeCodeCache).ToLocalChecked();
			}
			return script;
		}
#endif

		bool IsInit()
		{
			return Data != NULL && Length > 0;
		}
	};

	class CJSIsolateScopeV8 : public CJSIsolateScope
	{
	public:
		v8::Isolate::Scope  isolate_scope;
		v8::Locker          isolate_locker;

	public:
		CJSIsolateScopeV8(v8::Isolate* isolate) : CJSIsolateScope(),
			isolate_scope(isolate),
			isolate_locker(isolate)
		{
		}
		virtual ~CJSIsolateScopeV8()
		{
		}
	};

	class CJSContextScopeV8 : public CJSContextScope
	{
	public:
		v8::Context::Scope m_scope;

	public:
		CJSContextScopeV8(v8::Local<v8::Context> context) : m_scope(context)
		{
		}
		virtual ~CJSContextScopeV8()
		{
		}
	};

	class CJSLocalScopeV8 : public CJSLocalScope
	{
	public:
		v8::HandleScope m_scope;

	public:
		CJSLocalScopeV8() : m_scope(CV8Worker::GetCurrent())
		{
		}
		virtual ~CJSLocalScopeV8()
		{
		}
	};


	CJSContext::CJSContext()
	{
		m_internal = new CJSContextPrivate();
	}
	CJSContext::~CJSContext()
	{
		RELEASEOBJECT(m_internal);
	}

	CJSTryCatch* CJSContext::GetExceptions()
	{
		return new CV8TryCatch();
	}

	void CJSContext::Initialize()
	{
		m_internal->m_isolate = CV8Worker::getInitializer().CreateNew();
	}
	void CJSContext::Dispose()
	{
#ifdef V8_INSPECTOR
		v8_debug::disposeInspector(m_internal->m_context);
#endif
		m_internal->m_isolate->Dispose();
		m_internal->m_isolate = NULL;
	}

	void CJSContext::CreateContext()
	{
		m_internal->m_context = v8::Context::New(CV8Worker::GetCurrent(), NULL, m_internal->m_global);
	}

	void CJSContext::CreateGlobalForContext()
	{
		m_internal->m_global = v8::ObjectTemplate::New(CV8Worker::GetCurrent());
	}

	CJSObject* CJSContext::GetGlobal()
	{
		CJSObjectV8* ret = new CJSObjectV8();
		ret->value = m_internal->m_context->Global();
		return ret;
	}

	CJSIsolateScope* CJSContext::CreateIsolateScope()
	{
		return new CJSIsolateScopeV8(m_internal->m_isolate);
	}

	CJSContextScope* CJSContext::CreateContextScope()
	{
		CJSContextScope* pScope = new CJSContextScopeV8(m_internal->m_context);

		JSSmart<CJSObject> global = GetCurrent()->GetGlobal();
		global->set("window", global.GetPointer());

		return pScope;
	}

	CJSLocalScope* CJSContext::CreateLocalScope()
	{
		return new CJSLocalScopeV8();
	}

	CJSValue* CJSContext::createUndefined()
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->doUndefined();
		return _value;
	}

	CJSValue* CJSContext::createNull()
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->doNull();
		return _value;
	}

	CJSValue* CJSContext::createBool(const bool& value)
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->value = v8::Boolean::New(CV8Worker::GetCurrent(), value);
		return _value;
	}

	CJSValue* CJSContext::createInt(const int& value)
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->value = v8::Integer::New(CV8Worker::GetCurrent(), value);
		return _value;
	}

	CJSValue* CJSContext::createUInt(const unsigned int& value)
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->value = v8::Integer::NewFromUnsigned(CV8Worker::GetCurrent(), value);
		return _value;
	}

	CJSValue* CJSContext::createDouble(const double& value)
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->value = v8::Number::New(CV8Worker::GetCurrent(), value);
		return _value;
	}

	CJSValue* CJSContext::createString(const char* value, const int& length)
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->value = CreateV8String(CV8Worker::GetCurrent(), value, length);
		return _value;
	}

	CJSValue* CJSContext::createString(const wchar_t* value, const int& length)
	{
		std::string sUtf8 = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(value, (length != -1) ? (LONG)length : (LONG)wcslen(value));
		return createString((const char*)sUtf8.c_str(), (int)sUtf8.length());
	}

	CJSValue* CJSContext::createString(const std::string& value)
	{
		CJSValueV8* _value = new CJSValueV8();
		_value->value = CreateV8String(CV8Worker::GetCurrent(), value.c_str(), (int)value.length());
		return _value;
	}

	CJSValue* CJSContext::createString(const std::wstring& value)
	{
		std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(value);
		return createString(sReturn);
	}

	CJSObject* CJSContext::createObject()
	{
		CJSObjectV8* _value = new CJSObjectV8();
		_value->value = v8::Object::New(CV8Worker::GetCurrent());
		return _value;
	}

	CJSArray* CJSContext::createArray(const int& count)
	{
		CJSArrayV8* _value = new CJSArrayV8();
		_value->value = v8::Array::New(CV8Worker::GetCurrent(), count);
		_value->m_count = count;
		return _value;
	}

	CJSTypedArray* CJSContext::createUint8Array(BYTE* data, int count, const bool& isExternalize)
	{
		CJSTypedArrayV8* _value = new CJSTypedArrayV8(data, count, isExternalize);
		return _value;
	}

	JSSmart<CJSValue> CJSContext::runScript(const std::string& script, JSSmart<CJSTryCatch> exception, const std::wstring& scriptPath)
	{
#ifdef V8_INSPECTOR
		v8_debug::before(m_internal->m_context, CV8Worker::getInitializer()->getPlatform(), "");
#endif
		LOGGER_START

		v8::Local<v8::String> _source = CreateV8String(CV8Worker::GetCurrent(), script.c_str());
		v8::Local<v8::Script> _script;
		if(!scriptPath.empty())
		{
			std::wstring sCachePath = scriptPath.substr(0, scriptPath.rfind(L".")) + L".cache";
			CCacheDataScript oCachedScript(sCachePath);
			_script = oCachedScript.Compile(m_internal->m_context, _source);
		}
		else
		{
			v8::MaybeLocal<v8::Script> _scriptRetValue = v8::Script::Compile(V8ContextFirstArg _source);
			if (!_scriptRetValue.IsEmpty())
				_script = _scriptRetValue.ToLocalChecked();
		}

		LOGGER_LAP("compile")

		CJSValueV8* _return = new CJSValueV8();

		v8::MaybeLocal<v8::Value> retValue;
		if (exception.is_init())
		{
			if (!exception->Check())
				retValue = _script->Run(V8ContextOneArg);
		}
		else
		{
			retValue = _script->Run(V8ContextOneArg);
		}

		if (!retValue.IsEmpty())
			_return->value = retValue.ToLocalChecked();

		LOGGER_LAP("run")

				return _return;
	}

	CJSContext* CJSContext::GetCurrent()
	{
		CJSContext* ret = new CJSContext();
		ret->m_internal->m_isolate = CV8Worker::GetCurrent();
		ret->m_internal->m_context = ret->m_internal->m_isolate->GetCurrentContext();
		// global???
		return ret;
	}

	CJSValue* CJSContext::JSON_Parse(const char *sTmp)
	{
		CJSValueV8* _value = new CJSValueV8();
#ifndef V8_OS_XP
		v8::MaybeLocal<v8::Value> retValue = v8::JSON::Parse(m_internal->m_context, CreateV8String(CV8Worker::GetCurrent(), sTmp));
		if (!retValue.IsEmpty())
			_value->value = retValue.ToLocalChecked();
		else
			_value->doUndefined();
#else
		_value->value = v8::JSON::Parse(CreateV8String(CV8Worker::GetCurrent(), sTmp));
#endif
		return _value;
	}

	void CJSContext::MoveToThread(ASC_THREAD_ID* id)
	{
		// none
	}

	void CJSContext::ExternalInitialize(const std::wstring& sDirectory)
	{
		CV8Worker::m_sExternalDirectory = sDirectory;
	}
	void CJSContext::ExternalDispose()
	{
		CV8Worker::Dispose();
	}
	bool CJSContext::IsSupportNativeTypedArrays()
	{
		return true;
	}

	unsigned char* NSAllocator::Alloc(const size_t& size)
	{
		return (unsigned char*)CV8Worker::getInitializer().getAllocator()->AllocateUninitialized(size);
	}
	void NSAllocator::Free(unsigned char* data, const size_t& size)
	{
		CV8Worker::getInitializer().getAllocator()->Free(data, size);
	}
}
