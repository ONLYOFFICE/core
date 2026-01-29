#ifndef HTMLREADER_H
#define HTMLREADER_H

#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../DesktopEditor/xml/include/xmlutils.h"

#include "Writers/IWriter.h"
#include "Tags/HTMLTags.h"
#include "Table.h"

namespace HTML
{
class CHTMLReader
{
	XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
	NSCSS::CCssCalculator    m_oCSSCalculator; // Css калькулятор

	std::wstring m_sTmp;  // Temp папка
	std::wstring m_sSrc;  // Директория источника
	std::wstring m_sDst;  // Директория назначения
	std::wstring m_sBase; // Полный базовый адрес
	std::wstring m_sCore; // Путь до корневого файла (используется для работы с Epub)

	IWriter *m_pWriter;

	std::unordered_map<UINT, std::shared_ptr<ITag>> m_mTags;
public:
	CHTMLReader();
	~CHTMLReader();

	HRESULT ConvertFromTo(const std::wstring& wsFrom, const std::wstring& wsTo);

	NSCSS::CCssCalculator* GetCSSCalculator();
private:
	void InitOOXMLTags();
	void InitMDTags();

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

	bool ReadAnchor(std::vector<NSCSS::CNode>& arSelectors);
	bool ReadSVG(const std::vector<NSCSS::CNode>& arSelectors);
	bool ReadEmptyTag(UINT unTag, const std::vector<NSCSS::CNode>& arSelectors);
	bool ReadDefaultTag(UINT unTag, std::vector<NSCSS::CNode>& arSelectors);

	bool ReadTable(std::vector<NSCSS::CNode>& arSelectors);
	void ReadTableCaption(CStorageTable& oTable, std::vector<NSCSS::CNode>& arSelectors);
	void ReadTableRows(CStorageTable& oTable, std::vector<NSCSS::CNode>& arSelectors, ERowParseMode eMode);
	void ReadTableColspan(CStorageTable& oTable);

	void GetSubClass(std::vector<NSCSS::CNode>& arSelectors);
};
}

#endif // HTMLREADER_H
