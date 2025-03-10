#ifndef OFDFILE_H
#define OFDFILE_H

#include "../OfficeUtils/src/ZipFolder.h"

#include "src/Base.h"

#ifndef OFD_USE_DYNAMIC_LIBRARY
#define OFD_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define OFD_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace OFD
{
class OFD_DECL_EXPORT COfdFile
{
	std::wstring m_wsTempDir;

	CBase     m_oBase;

	bool Read(IFolder* pFolder);
	IFolder* CreateTempDir() const;
public:
	COfdFile();
	~COfdFile();

	void Close();

	void SetTempDir(const std::wstring& wsPath);
	std::wstring GetTempDir() const;

	bool LoadFromFile(const std::wstring& wsFilePath);

	bool SaveToFile(const std::wstring& wsFilePath);
	bool SaveToDir(const std::wstring& wsDir);
};
}

#endif // OFDFILE_H
