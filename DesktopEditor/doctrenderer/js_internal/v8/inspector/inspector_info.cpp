#include "inspector_info.h"


int NSJSBase::v8_debug::internal::CInspectorInfo::getContextId()
{
    static int currentId = 1;
    return currentId++;
}
