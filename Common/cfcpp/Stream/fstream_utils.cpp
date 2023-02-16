/*
* (c) Copyright Ascensio System SIA 2010-2019
*
* This program is a free software product. You can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License (AGPL)
* version 3 as published by the Free Software Foundation. In accordance with
* Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
* that Ascensio System SIA expressly excludes the warranty of non-infringement
* of any third-party rights.
*
* This program is distributed WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
* details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
*
* You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
* street, Riga, Latvia, EU, LV-1050.
*
* The  interactive user interfaces in modified source and object code versions
* of the Program must display Appropriate Legal Notices, as required under
* Section 5 of the GNU AGPL version 3.
*
* Pursuant to Section 7(b) of the License you must retain the original Product
* logo when distributing the program. Pursuant to Section 7(e) we decline to
* grant you any rights under trademark law for use of our trademarks.
*
* All the Product's GUI elements, including illustrations and icon sets, as
* well as technical writing content are licensed under the terms of the
* Creative Commons Attribution-ShareAlike 4.0 International. See the License
* terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
*
*/
#include "fstream_utils.h"
#include "fstream_wrapper.h"
#include "stream_utils.h"
#include "../../../DesktopEditor/common/File.h"
#include <algorithm>


using namespace CFCPP;

CFCPP::Stream CFCPP::OpenFileStream(const std::wstring & filename, bool bRewrite, bool trunc)
{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
	CFCPP::Stream st;

	// it's not good, but otherwise file doesn't create or if use ios::app, then the seek for writing will be blocked
	if (bRewrite)
		std::wfstream create(filename, std::ios::app | std::ios::out);

	if (trunc && bRewrite)
		st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc));
	else if (bRewrite)
		st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in | std::ios::out));
	else
		st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in));

	return st;
#else
	BYTE* pUtf8 = nullptr;
    LONG lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename.c_str(), filename.length(), pUtf8, lLen, false);
    std::string utf8filename((char*)pUtf8, lLen);
    delete [] pUtf8;

    return OpenFileStream(utf8filename, bRewrite, trunc);
#endif
}

CFCPP::Stream CFCPP::OpenFileStream(const std::string & filename, bool bRewrite, bool trunc)
{
    CFCPP::Stream st;

    // it's not good, but otherwise file doesn't create or if use ios::app, then the seek for writing will be blocked
    if (bRewrite)
        std::fstream create(filename, std::ios::app | std::ios::out);

    if (trunc && bRewrite)
        st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc));
    else if (bRewrite)
        st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in | std::ios::out));
    else
        st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in));

    return st;
}

_INT64 CFCPP::FileLenght(const std::wstring & filename)
{
    auto stream = OpenFileStream(filename);
    auto lenght = Length(stream);
    stream->close();

    return lenght;
}

ULONG64 CFCPP::FileFNVHash(const std::wstring & filename, _INT64 len, _INT64 offset)
{

    auto stream = OpenFileStream(filename);
    if (!IsOpen(stream))
        return 0;

    if (len < 0)
        len = Length(stream);

    stream->seek(offset);

    ULONG64 h = 2166136261;
    constexpr _INT64 bufLen = 0x2000;
    char buffer[bufLen];
    while (len > 0)
    {
        memset(buffer, 0, bufLen);
        _INT64 readLen = std::min(bufLen, len);
        stream->read(buffer, readLen);
        _INT64 i;

        for (i = 0; i < readLen; i++)
            h = (h * 16777619) ^ buffer[i];

        len -= readLen;
    }

    return h;
}
