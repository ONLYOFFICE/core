#include "inspector.h"
#include "inspectormanager.h"//for getting inspector
#include "../v8_base.h"//v8 wrappers and smart_ptr
#include "inspector_impl.h"//inspector implementation
#include <fstream>//std::ifstream

namespace {
    bool checkForInspection()
    {
        std::ifstream checkFile{};
        checkFile.open(
                    //you can insert here your absolute path
                    //i considered adding std::filesystem excessive for such a small job
                     "D:/111/work/core/DesktopEditor/doctrenderer/js_internal/v8/inspector"
                     "/other_files/print_here_anything_to_refuse_debugging.txt"
                );
        //debug always, if file does not exist or could not be opened
        if (!checkFile) {
            return true;
        }
        //check for eof as first symbol
        char check = checkFile.get();
        char eof = std::char_traits<char>::eof();
        return check == eof;//continue inspection only if file is empty
    }
}//namespace anonymous

void NSJSBase::v8_debug::CInspector::noteScriptExec(const std::string &script)
{
    std::cout << "to inspect script";
    if (script.size() > bigScript) {
        std::cout << " of length " << script.size() << std::endl;
    } else {
        std::cout << ":\n" << script << std::endl;
    }
}

void NSJSBase::v8_debug::CInspector::noteFuncCall(const char *func)
{
    std::cout << "to call function " << func << std::endl;
}

NSJSBase::v8_debug::CInspector::CInspector(
        v8::Local<v8::Context> context
        , v8::Platform *platform)
    : pImpl(
          internal::CInspectorManager::getInspector(
              context
              , platform
              , checkForInspection()
              )
          )
{
    //
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CInspector::runScript(
        const std::string &scriptStr
        , NSCommon::smart_ptr<CJSTryCatch> &pException
        , const std::wstring &scriptPath
        )
{
    noteScriptExec(scriptStr);
    return pImpl->runScript(internal::CScriptExecData{
                              scriptStr
                              , pException
                              , scriptPath
                          });
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CInspector::callFunc(
        v8::Local<v8::Object> value
        , const char *name
        , int argc
        , NSCommon::smart_ptr<CJSValue> argv[]
        )
{
    noteFuncCall(name);
    return pImpl->callFunc(internal::CFCallData{
                             value
                             , name
                             , argc
                             , argv
                           });
}

//must move dtor here, because here CInspectorImpl is complete class
NSJSBase::v8_debug::CInspector::~CInspector() = default;
