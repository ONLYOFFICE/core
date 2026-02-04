#ifndef HTMLREADER_H
#define HTMLREADER_H

#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../DesktopEditor/xml/include/xmlutils.h"

#include "HTMLParameters.h"
#include "MarkdownParameters.h"

#include "Writers/IWriter.h"
#include "Tags/HTMLTags.h"
#include "Table.h"

namespace HTML
{
class CHTMLReader
{
	XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
	NSCSS::CCssCalculator    m_oCSSCalculator; // Css калькулятор

	std::wstring m_wsTempDirectory; // Temp папка
	std::wstring m_wsSrcDirectory;  // Директория источника
	std::wstring m_wsDstDirectory;  // Директория назначения
	std::wstring m_wsBaseDirectory; // Полный базовый адрес
	std::wstring m_wsCoreDirectory; // Путь до корневого файла (используется для работы с Epub)

	IWriter *m_pWriter;

	std::unordered_map<UINT, std::shared_ptr<ITag>> m_mTags;
public:
	CHTMLReader();
	~CHTMLReader();

	void SetTempDirectory(const std::wstring& wsPath);
	void SetCoreDirectory(const std::wstring& wsPath);

	HRESULT ConvertHTML2OOXML   (const std::wstring& wsPath, const std::wstring& wsDirectory, THTMLParameters*     pParameters = nullptr);
	HRESULT ConvertHTML2Markdown(const std::wstring& wsPath, const std::wstring& wsFinalFile, TMarkdownParameters* pParameters = nullptr);

	HRESULT ConvertMHT2OOXML    (const std::wstring& sPath,  const std::wstring& sDirectory,  THTMLParameters*     pParameters = nullptr);
	HRESULT ConvertMHT2Markdown (const std::wstring& sPath,  const std::wstring& sDirectory,  TMarkdownParameters* pParameters = nullptr);

	NSCSS::CCssCalculator* GetCSSCalculator();
private:
	void Clear();
	void InitOOXMLTags(THTMLParameters* pParametrs = nullptr);
	void InitMDTags();

	bool IsHTML();

	bool HTML2XHTML(const std::wstring& wsFileName);

	HRESULT ConvertHTML(const std::wstring& wsPath, const std::wstring& wsDirectory);

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
