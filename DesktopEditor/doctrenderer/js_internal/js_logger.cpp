#include "./js_logger.h"
#include <stdio.h>
#include "../../common/File.h"
#include "../../graphics/Timer.h"

namespace NSJSBase
{
    std::string g_logger_file = NSSystemUtils::GetEnvVariableA(L"js_speed");
    bool g_logger_enabled = g_logger_file.empty() ? false : true;
    unsigned long g_logger_time = 0;

    void LOGGER_START_FUNC()
    {
        g_logger_time = NSTimers::GetTickCount();
    }
    void LOGGER_LAP_FUNC(const char* details)
    {
        if (!g_logger_enabled)
            return;
        unsigned long dwCur = (unsigned long)NSTimers::GetTickCount();

        if (g_logger_file.length() == 1)
        {
            printf(details);
            printf(": %d\n", (int)(dwCur - g_logger_time));
        }
        else
        {
            FILE* f = fopen(g_logger_file.c_str(), "a+");
            fprintf(f, "%s: %d\n", details, (int)(dwCur - g_logger_time));
            fclose(f);
        }
        g_logger_time = dwCur;
    }
}
