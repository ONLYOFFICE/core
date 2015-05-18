#pragma once

#include <iosfwd>

namespace cpdoccore { 

#if 1
template <class Ostream>
class logging
{
public:
    logging(Ostream & Ostream) : ostream_(Ostream)
    {}

    template <class T>
    Ostream & operator << (const T & t)
    {
        return (ostream_ << t );
    }

private:
    Ostream & ostream_;
};

#else

template <class Ostream>
class logging
{
public:
    logging(Ostream & Ostream)
    {}

    template <class T>
    logging & operator << (const T & t)
    {
        return *this;
    }

private:

};

#endif

extern logging< std::wostream > logging_err;
extern logging< std::wostream > logging_cout;

}

#define _CP_LOG_error	::cpdoccore::logging_err
#define _CP_LOG_info	::cpdoccore::logging_err
#define _CP_LOG_warning ::cpdoccore::logging_err

#define _CP_LOG(A) _CP_LOG_##A
