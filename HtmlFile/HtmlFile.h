#ifndef _HTMLFILE_HTMLFILE_H_
#define _HTMLFILE_HTMLFILE_H_

#include <string>

#ifndef HTMLFILE_USE_DYNAMIC_LIBRARY
#define HTMLFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define HTMLFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

class HTMLFILE_DECL_EXPORT CHtmlFile
{
private:

public:

    CHtmlFile();
    ~CHtmlFile();

    ///
    /// \brief Convert
    /// \param sXml -
    /// <html>
    ///     <sdk>file:///D:/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Word/sdk-all.js</sdk>
    ///     <file>file:///C:/Users/oleg.korshul/Desktop/original_message%20(5).html</file>
    ///     <file>file://192.168.3.208/allusers/Files/HTML/AllHTML/cars.html</file>
    ///     <destination>D:/test/Document/</destination> (end /!!!)
    /// </html>\

    /// \param sPathInternal - path (subprocesspath = path + HtmlFileInternal.exe) ("" -> GetProcessDirectory()/HtmlFileInternal/HtmlFileInternal.exe)
    /// \return 1 error, 0 - success
    ///
    int Convert(const std::wstring& sXml, const std::wstring& sPathInternal = L"");
};

#endif // _HTMLFILE_HTMLFILE_H_
