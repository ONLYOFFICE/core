#ifndef HTMLREADER_H
#define HTMLREADER_H

#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../DesktopEditor/xml/include/xmlutils.h"

#include "HTMLInterpretator.h"
#include "Tags/HTMLTags.h"

#include "../Common/3dParty/html/gumbo-parser/src/gumbo.h"

namespace HTML
{
#define HtmlTag GumboTag

class CHTMLReader
{
	XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
	NSCSS::CCssCalculator    m_oStylesCalculator; // Css калькулятор

	std::wstring m_sTmp;  // Temp папка
	std::wstring m_sSrc;  // Директория источника
	std::wstring m_sDst;  // Директория назначения
	std::wstring m_sBase; // Полный базовый адрес
	std::wstring m_sCore; // Путь до корневого файла (используется для работы с Epub)

	IHTMLInterpretator *m_pInterpretator;

	std::map<unsigned int, ITag*> m_mTags;
public:
	CHTMLReader();
	~CHTMLReader();

	HRESULT ConvertFromTo(const std::wstring& wsFrom, const std::wstring& wsTo);
private:
	bool IsHTML();

	bool HTML2XHTML(const std::wstring& wsFileName);

	void ReadStyle();
	void ReadStyle2();
	void ReadStyleFromNetwork();

	void ReadDocument();
	void ReadHead();
	void ReadBody();

	bool ReadStream(std::vector<NSCSS::CNode>& arSelectors, bool bInsertEmptyP = false);
	bool ReadInside(std::vector<NSCSS::CNode>& arSelectors);

	bool ReadText(std::vector<NSCSS::CNode>& arSelectors);
	bool ReadA(std::vector<NSCSS::CNode>& arSelectors);
	bool ReadBr(std::vector<NSCSS::CNode>& arSelectors);

	bool ReadDefaultTag(HtmlTag eTag, std::vector<NSCSS::CNode>& arSelectors);

	void GetSubClass(std::vector<NSCSS::CNode>& arSelectors);
};
}

#endif // HTMLREADER_H
