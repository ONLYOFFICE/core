#include "stream_utils.h"
#include "fstream_wrapper.h"


using namespace CFCPP;


_INT64 CFCPP::Length(const CFCPP::Stream& st)
{
    if (st.get() == nullptr)
        return 0;

    auto curPos = st->tell();
    st->seek(0, std::ios_base::end);
    auto ssize = st->tell();
    st->seek(curPos);

    return ssize;
}

bool CFCPP::IsOpen(const Stream &st)
{
    if (std::dynamic_pointer_cast<FStreamWrapper>(st))
        return std::static_pointer_cast<FStreamWrapper>(st)->is_open();

    return false;
}
