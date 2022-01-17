#include "inspector_interface.h"
#include "inspector_pool.h"//to get inspectors

void NSJSBase::v8_debug::before(const v8::Local<v8::Context> &context
                                      , v8::Platform *platform
                                      , const std::string &contextName) {
    internal::CInspectorPool::get().getInspector(context, platform, contextName).beforeLaunch();
}

void NSJSBase::v8_debug::disposeInspector(const v8::Local<v8::Context> &context) {
    internal::CInspectorPool::get().disposeInspector(context);
}
