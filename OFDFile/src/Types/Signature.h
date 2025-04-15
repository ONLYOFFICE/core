#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../Utils/XmlReader.h"
#include "../Utils/Types.h"

namespace OFD
{
struct TProvider
{
	std::wstring m_wsProviderName;
	std::wstring m_wsVersion;
	std::wstring m_wsCompany;

	void Read(CXmlReader& oLiteReader);
};

struct TReference
{
	std::wstring m_wsFileRef;
	std::wstring m_wsCheckValue;

	static TReference* Read(CXmlReader& oLiteReader);
};

struct TReferences
{
	std::wstring m_wsCheckMethod;
	std::vector<TReference*> m_arValues;

	~TReferences();

	void Read(CXmlReader& oLiteReader);
};

struct TStampAnnot
{
	unsigned int m_unID;
	unsigned int m_unPageRef;
	TBox         m_oBoundary;

	TStampAnnot();

	void Read(CXmlReader& oLiteReader);
};

struct TSignedInfo
{
	TProvider    m_oProvider;
	std::wstring m_wsSignatureMethod;
	std::wstring m_wsSignatureDateTime;
	TReferences  m_oReferences;
	TStampAnnot  m_oStampAnnot;

	void Read(CXmlReader& oLiteReader);
};

class CSignature
{
	TSignedInfo m_oSignedInfo;
	std::wstring m_wsSignedValue;

	std::wstring m_wsRootPath;
public:
	CSignature();

	static CSignature* Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath);

	bool Draw(IRenderer* pRenderer, unsigned int unPageIndex, NSFonts::IApplicationFonts* pFonts) const;
};
}

#endif // SIGNATURE_H
