#ifndef _CORE_EXT_JS_LOGGER_H_
#define _CORE_EXT_JS_LOGGER_H_

#include "../../common/SystemUtils.h"

namespace NSJSBase
{
    void LOGGER_START_FUNC();
    void LOGGER_LAP_FUNC(const char* details);
}

#if 1
#define LOGGER_START NSJSBase::LOGGER_START_FUNC();
#define LOGGER_LAP(__logger_lap_name__) NSJSBase::LOGGER_LAP_FUNC(#__logger_lap_name__);
#define LOGGER_LAP_NAME(__logger_lap_name__) NSJSBase::LOGGER_LAP_FUNC(__logger_lap_name__);
#else
#define LOGGER_START
#define LOGGER_LAP(__logger_lap_name__)
#define LOGGER_LAP_NAME(__logger_lap_name__)
#endif

#endif // _CORE_EXT_JS_LOGGER_H_
