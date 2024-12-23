#ifndef HWPFILE_H
#define HWPFILE_H

#include <string>

#ifndef HWPFILE_USE_DYNAMIC_LIBRARY
#define HWP_FILE_DECL_EXPORT
#else
#include "../../DesktopEditor/common/base_export.h"
#define HWP_FILE_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace HWP { class CHWPFile_Private; }
class HWP_FILE_DECL_EXPORT CHWPFile
{
	HWP::CHWPFile_Private *m_pInternal;
	std::wstring m_wsTempDirectory;
public:
	CHWPFile(const std::wstring& wsFileName);
	~CHWPFile();

	void SetTempDirectory(const std::wstring& wsTempDirectory);

	bool Open();
	void Close();
	bool ConvertToOOXML(const std::wstring& wsFilePath);
};

#endif // HWPFILE_H
