#ifndef AGG_SVG_EXCEPTION_INCLUDED
#define AGG_SVG_EXCEPTION_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

namespace agg 
{ 
namespace svg
{
    class exception
    {
    public:
        ~exception()
        {
            delete [] m_msg;
        }

        exception() : m_msg(0) {}
          
        exception(const char* fmt, ...) :
            m_msg(0)
        {
            if(fmt) 
            {
                m_msg = new char [4096];
                va_list arg;
                va_start(arg, fmt);
                vsprintf(m_msg, fmt, arg);
                va_end(arg);
            }
        }

        exception(const exception& exc) :
            m_msg(exc.m_msg ? new char[strlen(exc.m_msg) + 1] : 0)
        {
            if(m_msg) strcpy(m_msg, exc.m_msg);
        }
        
        const char* msg() const { return m_msg; }

    private:
        char* m_msg;
    };

}
}

#endif
