#include "v8_base.h"


v8::Local<v8::String> CreateV8String(v8::Isolate* i, const char* str, const int& len)
{
    return v8::String::NewFromUtf8(i, str, kV8NormalString, len).ToLocalChecked();
}
v8::Local<v8::String> CreateV8String(v8::Isolate* i, const std::string& str)
{
    return v8::String::NewFromUtf8(i, str.c_str(), kV8NormalString, (int)str.length()).ToLocalChecked();
}

CV8Initializer* CV8Worker::m_pInitializer = NULL;
bool CV8Worker::m_bUseExternalInitialize = false;

namespace NSJSBase
{

    class CCacheDataScript
    {
    private:
        BYTE* Data;
        int Length;

        v8::ScriptCompiler::Source* Source;
        v8::ScriptCompiler::CachedData* CachedData;

        std::wstring Path;

    public:
        CCacheDataScript(const std::wstring& sPath)
        {
            Data = NULL;
            Length = 0;

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

            Source      = NULL;
            CachedData  = NULL;
            Path        = sPath;
        }
        ~CCacheDataScript()
        {
            //RELEASEOBJECT(Source);
            //RELEASEOBJECT(CachedData);
            RELEASEARRAYOBJECTS(Data);
        }

        v8::Local<v8::Script> Compile(const v8::Local<v8::Context>& _context, const v8::Local<v8::String>& source)
        {
            v8::Local<v8::Script> script;
            if (NULL == Data)
            {
                Source = new v8::ScriptCompiler::Source(source);
                script = v8::ScriptCompiler::Compile(_context, Source, kV8ProduceCodeCache).ToLocalChecked();

                const v8::ScriptCompiler::CachedData* _cachedData = Source->GetCachedData();
                NSFile::CFileBinary oFileTest;
                if (_cachedData && oFileTest.CreateFileW(Path))
                {
                    oFileTest.WriteFile(_cachedData->data, (DWORD)_cachedData->length);
                    oFileTest.CloseFile();
                }
            }
            else
            {
                CachedData = new v8::ScriptCompiler::CachedData(Data, Length);
                Source = new v8::ScriptCompiler::Source(source, CachedData);
                script = v8::ScriptCompiler::Compile(_context, Source, v8::ScriptCompiler::kConsumeCodeCache).ToLocalChecked();
            }
            return script;
        }

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
        std::cout << "CJSCONTEXT DTOR CALLED\n";
        RELEASEOBJECT(m_internal);
    }

    CJSTryCatch* CJSContext::GetExceptions()
    {
        return new CV8TryCatch();
    }

    void CJSContext::Initialize()
    {
        CV8Worker::Initialize();
        m_internal->m_isolate = CV8Worker::getInitializer()->CreateNew();
    }
    void CJSContext::Dispose()
    {
        std::cout << "CONTEXT DISPOSE\n";
#ifdef V8_INSPECTOR
        m_internal->disposeInspector();
#endif
        m_internal->m_isolate->Dispose();
        m_internal->m_isolate = NULL;
        if (!CV8Worker::IsUseExternalInitialize())
            CV8Worker::Dispose();        
    }

    void CJSContext::CreateContext()
    {
        m_internal->m_context = v8::Context::New(CV8Worker::GetCurrent(), NULL, m_internal->m_global);
        //
    }

    void CJSContext::CreateGlobalForContext()
    {
        m_internal->m_global = v8::ObjectTemplate::New(CV8Worker::GetCurrent());
    }

    CJSObject* CJSContext::GetGlobal()
    {
        CJSObjectV8* ret = new CJSObjectV8(
#ifdef V8_INSPECTOR
                 this
#endif
                    );
        ret->value = m_internal->m_context->Global();
        return ret;
    }

    CJSIsolateScope* CJSContext::CreateIsolateScope()
    {
        return new CJSIsolateScopeV8(m_internal->m_isolate);
    }

    CJSContextScope* CJSContext::CreateContextScope()
    {
        return new CJSContextScopeV8(m_internal->m_context);
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

    CJSTypedArray* CJSContext::createUint8Array(BYTE* data, int count)
    {
        CJSTypedArrayV8* _value = new CJSTypedArrayV8(data, count);
        return _value;
    }

    JSSmart<CJSValue> CJSContext::runScript(const std::string& script
                                            , JSSmart<CJSTryCatch> exception
                                            , const std::wstring& scriptPath)
    {
#ifdef V8_INSPECTOR
        return m_internal->getInspector().runScript(script, exception, scriptPath);
#else
        return runScriptImpl(
                    m_internal->m_context
                    , script
                    , exception
                    , scriptPath
                    );
#endif
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
        v8::MaybeLocal<v8::Value> retValue = v8::JSON::Parse(
                    m_internal->m_context, CreateV8String(CV8Worker::GetCurrent(), sTmp));
        if (!retValue.IsEmpty())
            _value->value = retValue.ToLocalChecked();
    #else
        _value->value = v8::JSON::Parse(CreateV8String(CV8Worker::GetCurrent(), sTmp));
    #endif
        return _value;
    }

    void CJSContext::ExternalInitialize()
    {
        CV8Worker::SetUseExetralInitialize();
    }
    void CJSContext::ExternalDispose()
    {
        CV8Worker::Dispose();
    }
    bool CJSContext::IsSupportNativeTypedArrays()
    {
        return true;
    }

    JSSmart<CJSValue> callFuncImpl(
            v8::Local<v8::Object> value
            , v8::Local<v8::Context> context
            , const char *name
            , const int argc
            , JSSmart<CJSValue> argv[])
    {
        //get function value
        v8::Local<v8::String> v8name = CreateV8String(context->GetIsolate(), name);
        v8::Local<v8::Value> funcval = value->Get(context, v8name).ToLocalChecked();

        //check for function
        if (!funcval->IsFunction()) {
            return new CJSValueV8();
        }

        //make function
        v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(funcval);

        //check for zero arguments
        if (0 == argc) {
            v8::MaybeLocal<v8::Value> result = function->Call(context, value, 0, nullptr);
            if (result.IsEmpty()) {
                return new CJSValueV8();
            }
            return new CJSValueV8(result.ToLocalChecked());
        }

        //argv isn't empty

        //fullfill v8-style argument array
        std::unique_ptr<v8::Local<v8::Value>[]> v8argv(new v8::Local<v8::Value>[argc]);
        for (int i = 0; i < argc; ++i)
        {
            CJSValueV8 *v8argument =
                    static_cast<CJSValueV8*>(argv[i].GetPointer());
            v8argv.get()[i] = v8argument->value;
        }

        //call function
        v8::MaybeLocal<v8::Value> result =
                function->Call(context, value, argc, v8argv.get());
        if (result.IsEmpty()) {
            return new CJSValueV8();
        }
        return new CJSValueV8(result.ToLocalChecked());
    }

#ifdef V8_INSPECTOR

    v8_debug::CPerContextInspector &CJSContextPrivate::getInspector()
    {
        return m_Inspector.maybeInit(m_context, CV8Worker::getInitializer()->getPlatform());
    }

    void CJSContextPrivate::disposeInspector()
    {
        m_Inspector.dispose();
    }

#endif

}//namespace NSJSBase

namespace {
    //compile script
    v8::Local<v8::Script> compileOnlyScript(v8::Local<v8::Context> context
                                            , v8::Local<v8::String> source) {
        v8::MaybeLocal<v8::Script> maybeScript = v8::Script::Compile(context, source);
        if (maybeScript.IsEmpty()) {
            return v8::Local<v8::Script>();
        } else {
            return maybeScript.ToLocalChecked();
        }
    }
    v8::Local<v8::Script> compileScriptWithPath(v8::Local<v8::Context> context
                                                , v8::Local<v8::String> source
                                                , const std::wstring &scriptPath) {
        //создаём путь к файлу в той же директории с тем же названием, но с расширением .cache
        std::size_t dotPos = scriptPath.rfind(L".");
        std::wstring sCachePath = scriptPath.substr(0, dotPos) + L".cache";
        NSJSBase::CCacheDataScript oCachedScript(sCachePath);
        return oCachedScript.Compile(context, source);
    }

    //run
    v8::MaybeLocal<v8::Value> runScriptWithException(v8::Local<v8::Context> context
            , v8::Local<v8::Script> script
            , JSSmart<NSJSBase::CJSTryCatch> pException) {
        if (pException.is_init()//если TryCatch есть
                &&
                pException->Check()//и он поймал исключение
                ) {
            //не запускаем скрипт, возвращаем дефолтное значение
            return v8::MaybeLocal<v8::Value>();
        }
        //если всё ок, запускаем скрипт
        return script->Run(context);
    }
}

namespace NSJSBase {

    v8::Local<v8::Script> compileScript(v8::Local<v8::Context> context
                                        , const std::string &script
                                        , const std::wstring &scriptPath)
    {
        //make v8 source string
        v8::Local<v8::String> source = CreateV8String(
                    //so was in the original code
                    //I think it is correct to get context's isolate
                    CV8Worker::GetCurrent()//isolate
                                                      , script);

        //compile script
        return
                scriptPath.empty()
                ?
                    compileOnlyScript(context, source)
                  :
                    compileScriptWithPath(context, source, scriptPath);
    }

    JSSmart<CJSValue> runScriptImpl(v8::Local<v8::Context> context
                                    , const std::string &script
                                    , JSSmart<CJSTryCatch> pException
                                    , const std::wstring &scriptPath)
    {
        //compile script
        v8::Local<v8::Script> v8script = compileScript(context, script, scriptPath);

        //run
        v8::MaybeLocal<v8::Value> maybeResult = runScriptWithException(context
                                                                       , v8script
                                                                       , pException);

        //ptr to subclass of CJSValue
        CJSValueV8 *result = new CJSValueV8();

        //if there's a result, set it
        if (!maybeResult.IsEmpty()) {
            result->value = maybeResult.ToLocalChecked();
        }

        return result;
    }

}//namespace NSJSBase
