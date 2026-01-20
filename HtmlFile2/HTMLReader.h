#ifndef HTMLREADER_H
#define HTMLREADER_H

#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../DesktopEditor/xml/include/xmlutils.h"

#include "HTMLInterpretator.h"
#include "Tags/HTMLTags.h"

namespace HTML
{
class CHTMLReader
{
	XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
	NSCSS::CCssCalculator    m_oStylesCalculator; // Css калькулятор

	NSCSS::NSProperties::CPage m_oPageData; // Стили страницы

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

	HRESULT ReadFromFile(const std::wstring& wsFileName);
private:
	bool IsHTML();

	bool HTML2XHTML(const std::wstring& wsFileName);

	void ReadStyle();
	void ReadStyle2();
	void ReadStyleFromNetwork();

	void ReadDocument();
	void ReadHead();
	void ReadBody();

	bool ReadStream(std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS, bool bInsertEmptyP = false);
	bool ReadInside(std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS);

	bool ReadA(std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS);

	void GetSubClass(std::vector<NSCSS::CNode>& sSelectors);
};
}

#endif // HTMLREADER_H
