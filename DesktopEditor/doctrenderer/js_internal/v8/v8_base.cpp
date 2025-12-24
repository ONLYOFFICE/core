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
						oFileTest.WriteFile(pCacheData->data, (DWORD)pCacheData->length);
						oFileTest.CloseFile();
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

	class CJSLocalScopePrivate
	{
	public:
		v8::HandleScope m_scope;

	public:
		CJSLocalScopePrivate() : m_scope(CV8Worker::GetCurrent())
		{
		}
		~CJSLocalScopePrivate()
		{
		}
	};

	CJSLocalScope::CJSLocalScope() : m_internal(new CJSLocalScopePrivate())
	{
	}

	CJSLocalScope::~CJSLocalScope()
	{
		delete m_internal;
	}

	CJSContext::CJSContext(const bool& bIsInitialize)
	{
		m_internal = new CJSContextPrivate();
		if (bIsInitialize)
			Initialize();
	}

	CJSContext::~CJSContext()
	{
		if (m_internal->m_isolate && !m_internal->m_contextPersistent.IsEmpty())
			Dispose();
		RELEASEOBJECT(m_internal);
	}

	JSSmart<CJSTryCatch> CJSContext::GetExceptions()
	{
		return new CV8TryCatch();
	}

	void CJSContext::Initialize(const std::wstring& snapshotPath)
	{
		if (m_internal->m_isolate == NULL)
		{
#ifdef V8_SUPPORT_SNAPSHOTS
			if (!snapshotPath.empty())
			{
				BYTE* data = NULL;
				DWORD dataLength = 0;
				if (NSFile::CFileBinary::ReadAllBytes(snapshotPath, &data, dataLength))
				{
					m_internal->m_startup_data.data = reinterpret_cast<const char*>(data);
					m_internal->m_startup_data.raw_size = (int)dataLength;
				}
			}
#endif

			// get new isolate
			v8::Isolate* isolate = CV8Worker::getInitializer().CreateNew(m_internal->m_startup_data.data ? &m_internal->m_startup_data : nullptr);
			m_internal->m_isolate = isolate;
			// get new context
			v8::Isolate::Scope iscope(isolate);
			v8::HandleScope scope(isolate);
			m_internal->m_contextPersistent.Reset(isolate, v8::Context::New(isolate));
			// create temporary local handle to context
			m_internal->m_context = v8::Local<v8::Context>::New(isolate, m_internal->m_contextPersistent);
			// insert embed functions to global object of this context
			m_internal->InsertToGlobal("CreateEmbedObject", CreateEmbedNativeObject);
			m_internal->InsertToGlobal("FreeEmbedObject", FreeNativeObject);
			// clear temporary local handle
			m_internal->m_context.Clear();
		}
	}

	class WeakHandleVisitor : public v8::PersistentHandleVisitor
	{
	private:
		WeakHandleVisitor() = default;

	public:
		void VisitPersistentHandle(v8::Persistent<v8::Value>* value, uint16_t class_id) override
		{
			if (class_id == CJSEmbedObjectPrivate::kWeakHandleId)
			{
				v8::Isolate* isolate = v8::Isolate::GetCurrent();
				v8::HandleScope scope(isolate);
				v8::Local<v8::Object> handle = value->Get(isolate).As<v8::Object>();
				v8::Local<v8::External> field = v8::Local<v8::External>::Cast(handle->GetInternalField(0));
				CJSEmbedObject* native = static_cast<CJSEmbedObject*>(field->Value());
				delete native;
			}
		}

	public:
		static WeakHandleVisitor* getInstance()
		{
			static WeakHandleVisitor visitor;
			return &visitor;
		}
	};

	void CJSContext::Dispose()
	{
#ifdef V8_INSPECTOR
		if (CV8Worker::getInitializer().isInspectorUsed())
			CInspectorPool::get().disposeInspector(m_internal->m_isolate);
#endif

		m_internal->m_contextPersistent.Reset();
		// destroy native object in the weak handles before isolate disposal
		v8::Isolate* isolate = m_internal->m_isolate;
		{
			v8::Isolate::Scope scope(isolate);
			isolate->VisitHandlesWithClassIds(WeakHandleVisitor::getInstance());
		}
		isolate->Dispose();
		m_internal->m_isolate = NULL;
	}

	bool CJSContext::isSnapshotUsed()
	{
		return m_internal->m_startup_data.data != NULL;
	}

	JSSmart<CJSObject> CJSContext::GetGlobal()
	{
		CJSObjectV8* ret = new CJSObjectV8();
		ret->value = m_internal->m_context->Global();
		return ret;
	}

	void CJSContext::Enter()
	{
		v8::Isolate* isolate = m_internal->m_isolate;
#ifdef LOG_TO_COUT
		std::cout << "Entering isolate \t" << m_internal->m_isolate << std::endl;
#endif
		isolate->Enter();
		m_internal->m_scope.push(new CJSLocalScope());
		m_internal->m_context = v8::Local<v8::Context>::New(isolate, m_internal->m_contextPersistent);
		if (!m_internal->m_context.IsEmpty())
			m_internal->m_context->Enter();

		m_internal->m_entered = true;
	}

	void CJSContext::Exit()
	{
#ifdef LOG_TO_COUT
		std::cout << "Exiting isolate \t" << m_internal->m_isolate << std::endl;
#endif
		if (!m_internal->m_context.IsEmpty())
			m_internal->m_context->Exit();
		delete m_internal->m_scope.top();
		m_internal->m_scope.pop();
		if (m_internal->m_scope.empty())
			m_internal->m_context.Clear();
		else
			m_internal->m_context = m_internal->m_isolate->GetCurrentContext();
		m_internal->m_isolate->Exit();

		m_internal->m_entered = false;
	}

	bool CJSContext::IsEntered()
	{
		return m_internal->m_entered;
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
		if (CV8Worker::getInitializer().isInspectorUsed())
			CInspectorPool::get().getInspector(m_internal->m_isolate).startAgent();
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

	bool CJSContext::generateSnapshot(const std::string& script, const std::wstring& snapshotPath)
	{
#ifdef V8_SUPPORT_SNAPSHOTS
		bool result = false;
		// Snapshot creator should be in its own scope, because it handles entering, exiting and disposing the isolate
		v8::SnapshotCreator snapshotCreator;
		v8::Isolate* isolate = snapshotCreator.GetIsolate();
		{
			CV8TryCatch try_catch(isolate);

			v8::HandleScope handle_scope(isolate);
			// Create a new context
			v8::Local<v8::Context> context = v8::Context::New(isolate);
			v8::Context::Scope context_scope(context);

			// Handle compile & run errors
			v8::Local<v8::Object> global = context->Global();
			global->Set(context, v8::String::NewFromUtf8Literal(isolate, "window"), global).Check();
			global->Set(context, v8::String::NewFromUtf8Literal(isolate, "self"), global).Check();
			global->Set(context, v8::String::NewFromUtf8Literal(isolate, "native"), v8::Undefined(isolate)).Check();

			// Compile
			v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, script.c_str()).ToLocalChecked();
			v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
			// Run
			script->Run(context).IsEmpty();

			snapshotCreator.SetDefaultContext(context);
		}
		v8::StartupData data = snapshotCreator.CreateBlob(v8::SnapshotCreator::FunctionCodeHandling::kKeep);
		// Save snapshot to file
		NSFile::CFileBinary snapshotFile;
		if (data.data && snapshotFile.CreateFile(snapshotPath))
		{
			snapshotFile.WriteFile(data.data, (DWORD)data.raw_size);
			snapshotFile.CloseFile();
			result = true;
		}

		delete[] data.data;

		return result;
#else
		return false;
#endif
	}

	JSSmart<CJSContext> CJSContext::GetCurrent()
	{
		CJSContext* ret = new CJSContext(false);
		ret->m_internal->m_isolate = CV8Worker::GetCurrent();
		ret->m_internal->m_context = ret->m_internal->m_isolate->GetCurrentContext();
		return ret;
	}

	JSSmart<CJSValue> CJSContext::JSON_Parse(const char *sTmp)
	{
		CJSValueV8* _value = new CJSValueV8();
#ifndef V8_OS_XP
		v8::MaybeLocal<v8::Value> retValue = v8::JSON::Parse(m_internal->m_context, CreateV8String(CV8Worker::GetCurrent(), sTmp));
		if (!retValue.IsEmpty())
			_value->value = retValue.ToLocalChecked();
		else
			_value->doUndefined();
#else
		// TODO: Use MaybeLocal version
		_value->value = v8::JSON::Parse(CreateV8String(CV8Worker::GetCurrent(), sTmp));
#endif
		return _value;
	}

	std::string CJSContext::JSON_Stringify(JSSmart<CJSValue> value)
	{
		// if don't return an empty string explicitly, V8 will return "undefined", which is incorrect
		if (value->isUndefined())
			return "";

		CJSValueV8* _value = static_cast<CJSValueV8*>(value.GetPointer());
		v8::MaybeLocal<v8::String> result;
#ifndef V8_OS_XP
#ifdef V8_VERSION_89_PLUS
		result = v8::JSON::Stringify(m_internal->m_context, _value->value);
#else
		v8::MaybeLocal<v8::Object> json_object = _value->value->ToObject(m_internal->m_context);
		if (json_object.IsEmpty())
			// in case of null and other non-object values
			result = _value->value->ToString(m_internal->m_context);
		else
			result = v8::JSON::Stringify(m_internal->m_context, json_object.ToLocalChecked());
#endif
#else
		// there is no built-in stringifier in V8_XP, so use JSON.stringify() from JS
		v8::Local<v8::Object> json = m_internal->m_context->Global()->Get(CreateV8String(m_internal->m_isolate, "JSON"))->ToObject();
		v8::Local<v8::Function> stringify = json->Get(CreateV8String(m_internal->m_isolate, "stringify")).As<v8::Function>();
		result = stringify->Call(json, 1, &_value->value)->ToString(m_internal->m_context);
#endif
		if (result.IsEmpty())
			return "";

		v8::String::Utf8Value data(V8IsolateFirstArg result.ToLocalChecked());
		return std::string((char*)(*data), data.length());
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

// embed
namespace NSJSBase
{
	class CJSFunctionArgumentsV8 : public CJSFunctionArguments
	{
		const v8::FunctionCallbackInfo<v8::Value>* m_args;
		int m_count;
		// TODO: do we actually need it?
		int m_skip_count;
	public:
		CJSFunctionArgumentsV8(const v8::FunctionCallbackInfo<v8::Value>* args, const bool skip_count = 1)
		{
			m_args = args;
			m_skip_count = skip_count;
			m_count = m_args->Length() - m_skip_count;
		}
	public:
		virtual int GetCount() override
		{
			return m_count;
		}
		virtual JSSmart<CJSValue> Get(const int& index) override
		{
			if (index < m_count)
				return js_value(m_args->operator[](index + m_skip_count));
			return js_value(v8::Undefined(m_args->GetIsolate()));
		}
	};

	// this function is called when method from embedded object is called
	void _Call(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CJSEmbedObject* _this = (CJSEmbedObject*)unwrap_native(args.Holder());
		CJSFunctionArgumentsV8 _args(&args, 0);
		JSSmart<CJSValue> funcIndex = js_value(args.Data());
		CJSEmbedObjectAdapterV8* _adapter = static_cast<CJSEmbedObjectAdapterV8*>(_this->getAdapter());
		JSSmart<CJSValue> ret = _adapter->Call(funcIndex->toInt32(), &_args);
		js_return(args, ret);
	}

	v8::Handle<v8::ObjectTemplate> CreateEmbedObjectTemplate(v8::Isolate* isolate, CJSEmbedObject* pNativeObj)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		CJSEmbedObjectAdapterV8* pAdapter = static_cast<CJSEmbedObjectAdapterV8*>(pNativeObj->getAdapter());

		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		pAdapter->initFunctions(pNativeObj);
		std::vector<std::string> arNames = pAdapter->getMethodNames();
		for (int i = 0, len = arNames.size(); i < len; ++i)
		{
			// associate all methods with corresponding Call() index
			result->Set(CreateV8String(isolate, arNames[i].c_str()), v8::FunctionTemplate::New(isolate, _Call, v8::Integer::New(isolate, i)));
		}

		return handle_scope.Escape(result);
	}

	void CreateEmbedNativeObject(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		v8::HandleScope scope(isolate);

		if (args.Length() != 1)
		{
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}

		std::string sName;
		v8::String::Utf8Value data(V8IsolateFirstArg args[0]);
		if (NULL != *data)
			sName = std::string((char*)*data, data.length());

		CEmbedObjectRegistrator& oRegistrator = CEmbedObjectRegistrator::getInstance();
		CEmbedObjectRegistrator::store_t::iterator itFound = oRegistrator.m_infos.find(sName);
		if (itFound == oRegistrator.m_infos.end())
		{
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}

		CEmbedObjectRegistrator::CEmdedClassInfo& oInfo = itFound->second;

		if (oInfo.m_bIsCreationAllowed == false)
		{
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}

		CJSEmbedObject* pNativeObj = oInfo.m_creator();
		v8::Local<v8::ObjectTemplate> oCurTemplate;
		CJSEmbedObjectAdapterV8Template* pTemplateAdapter = dynamic_cast<CJSEmbedObjectAdapterV8Template*>(pNativeObj->getAdapter());
		if (pTemplateAdapter)
		{
			oCurTemplate = pTemplateAdapter->getTemplate(isolate);
		}
		else
		{
			oCurTemplate = CreateEmbedObjectTemplate(isolate, pNativeObj);
		}
		v8::MaybeLocal<v8::Object> oTemplateMayBe = oCurTemplate->NewInstance(isolate->GetCurrentContext());
		v8::Local<v8::Object> obj = oTemplateMayBe.ToLocalChecked();
		obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), pNativeObj));

		NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);
		args.GetReturnValue().Set(obj);
	}

	void FreeNativeObject(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		v8::HandleScope scope(isolate);

		if (args.Length() != 1)
		{
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}

		v8::Local<v8::Object> obj = args[0].As<v8::Object>();
		v8::Local<v8::External> field = v8::Local<v8::External>::Cast(obj->GetInternalField(0));
		CJSEmbedObject* native = static_cast<CJSEmbedObject*>(field->Value());
		delete native;
		// weak persistent handle will be cleared and removed in CJSEmbedObjectPrivate destructor
	}
}
