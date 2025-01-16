#ifndef HWPFILE_H
#define HWPFILE_H

#include <string>

#ifndef HWPFILE_USE_DYNAMIC_LIBRARY
#define HWP_FILE_DECL_EXPORT
#else
#include "../../DesktopEditor/common/base_export.h"
#define HWP_FILE_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace HWP { class CWriterContext; }
class HWP_FILE_DECL_EXPORT CHWPFile
{
	HWP::CWriterContext *m_pInternal;
	std::wstring m_wsFileName;
	std::wstring m_wsTempDirectory;
public:
	CHWPFile();
	~CHWPFile();

	void SetTempDirectory(const std::wstring& wsTempDirectory);

	bool OpenHWP(const std::wstring& wsFilePath);
	bool OpenHWPX(const std::wstring& wsFilePath);
	void Close();
	bool ConvertToOOXML(const std::wstring& wsFilePath);
	bool ConvertToOOXML_Dir(const std::wstring& wsDirectoryPath);

	static bool IsHWPFormat(const std::wstring& wsFilePath);
	static bool IsHWPXFormat(const std::wstring& wsFilePath);
};

#endif // HWPFILE_H
