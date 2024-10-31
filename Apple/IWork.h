#ifndef _FB2FILE_FB2FILE_H
#define _FB2FILE_FB2FILE_H

#include <string>

#ifndef IWORK_USE_DYNAMIC_LIBRARY
#define IWORK_FILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define IWORK_FILE_DECL_EXPORT Q_DECL_EXPORT
#endif

enum class IWorkFileType
{
	Pages   = 0,
	Numbers = 1,
	Keynote = 2,

	None    = 255
};

class CIWorkFile_Private;
class IWORK_FILE_DECL_EXPORT CIWorkFile
{
private:
	CIWorkFile_Private* m_internal;
public:
	CIWorkFile();
	~CIWorkFile();

	IWorkFileType GetType(const std::wstring& sFile);
	int Convert2Odf(const std::wstring& sFile, const std::wstring& sOutputFile);
	void SetTmpDirectory(const std::wstring& sFolder);
};

#endif // _FB2FILE_FB2FILE_H
