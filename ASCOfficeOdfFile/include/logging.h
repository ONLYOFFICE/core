#pragma once

#include <iosfwd>


namespace cpdoccore
{
    template <class Ostream>
    class logging
    {
    public:
        logging(Ostream &stream) : ostream_(stream)
        {}

        template <class T>
        Ostream & operator << (const T & t)
        {
            ostream_ << t;
            return ostream_;
        }

    private:
        Ostream & ostream_;
    };

    extern logging< std::wostream >     logging_cout;
    extern logging< std::wstringstream >logging_log;
}

#ifdef _DEBUG
    #define _CP_LOG     cpdoccore::logging_cout
#else
    #define _CP_LOG     cpdoccore::logging_log
#endif
