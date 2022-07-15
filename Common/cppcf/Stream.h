#pragma once


#include <sstream>
#include <fstream>
#include <memory>
#include "../../DesktopEditor/common/Types.h"

namespace CFCPP
{
using Stream = std::shared_ptr<std::iostream>;
std::streamsize Length(const Stream& st)
{
    if (st.get() == nullptr)
        return 0;

    auto curPos = st->tellg();
    st->seekg(std::ios_base::end);
    auto ssize = st->tellg();
    st->seekg(curPos);

    return ssize;
}
}
