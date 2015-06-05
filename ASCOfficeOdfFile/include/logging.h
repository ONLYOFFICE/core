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
    #if _DEBUG
            return (ostream_ << t );
    #endif
        }

    private:
        Ostream & ostream_;
    };

    extern logging< std::wostream > logging_cout;

}

#define _CP_LOG     cpdoccore::logging_cout
