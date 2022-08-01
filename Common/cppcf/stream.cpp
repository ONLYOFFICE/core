#include "stream.h"


std::streamsize CFCPP::Length(const CFCPP::Stream& st)
{
    if (st.get() == nullptr)
        return 0;

    auto curPos = st->tellg();
    st->seekg(0, std::ios_base::end);
    auto ssize = st->tellg();
    st->seekg(curPos);

    return ssize;
}
