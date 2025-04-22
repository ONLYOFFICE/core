#ifndef BASE_H
#define BASE_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../OfficeUtils/src/ZipFolder.h"

#include "Document.h"
#include "Types/Signature.h"

namespace OFD
{
enum class EDocUsege
{
	Normal,
	EBook,
	ENewsPaper,
	EMagzine
};

class CDocInfo
{
	std::wstring m_wsDocId;
	std::wstring m_wsTitle;
	std::wstring m_wsAuthor;
	std::wstring m_wsSubject;
	std::wstring m_wsAbstact;

	std::wstring m_wsCreationDate;
	std::wstring m_wsModDate;

	EDocUsege    m_eDocUsage;
	std::wstring m_wsCover;
	std::vector<std::wstring> m_arKeywords;

	std::wstring m_wsCreator;
	std::wstring m_wsCreatorVersion;

	std::vector<std::wstring> m_arCustomData;
public:
	CDocInfo();
	bool Read(CXmlReader& oLiteReader);
};

class CDocBody
{
	CDocInfo  m_oDocInfo;
	CDocument m_oDocument;
	// std::wstring m_wsVersions;
	std::vector<CSignature*> m_arSignatures;

	void ReadSignatures(const std::wstring& wsFilePath, IFolder* pFolder);
public:
	CDocBody();
	~CDocBody();

	static CDocBody* Read(CXmlReader& oLiteReader, IFolder* pFolder);

	bool DrawPage(IRenderer* pRenderer, int nPageIndex) const;

	unsigned int GetPageCount() const;
	bool GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const;
};

class CBase
{
	std::vector<CDocBody*> m_arDocBodies;
public:
	CBase();
	~CBase();

	bool Read(IFolder* pFolder);
	void DrawPage(IRenderer* pRenderer, int nPageIndex) const;

	unsigned int GetPageCount() const;
	void GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const;
};
}

#endif // BASE_H
