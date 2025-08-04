#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <string>
#include <vector>

#include "Content/PageBlock.h"

namespace OFD
{
class CParameter
{
public:
	CParameter(CXmlReader& oLiteReader);
private:
	std::wstring m_wsName;
	std::wstring m_wsValue;
};

typedef CPageBlock CAppearance;

enum class EAnnotType
{
	Link,
	Path,
	Highlight,
	Stamp,
	Watermark
};

class CAnnot
{
public:
	CAnnot(CXmlReader& oLiteReader);
	~CAnnot();

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
private:
	EAnnotType m_eType;
	bool m_bVisible;
	bool m_bPrint;
	bool m_bNoZoom;
	bool m_bNoRotate;
	bool m_bReadOnly;

	std::vector<CParameter*> m_arParameters;
	std::vector<CAppearance*> m_arAppearances;
};

class CPageAnnot
{
public:
	CPageAnnot();

	static CPageAnnot* Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath);

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
private:

	std::vector<CAnnot*> m_arAnnots;
};

class CAnnotation
{
public:
	CAnnotation();
	~CAnnotation();

	bool Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath);

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
private:
	CPageAnnot *m_pPageAnnot;
};
}

#endif // ANNOTATION_H
