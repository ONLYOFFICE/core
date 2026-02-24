#ifndef HTMLREADER_H
#define HTMLREADER_H

#include <unordered_map>

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

	bool m_bIsTempDirOwner;
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

	HRESULT ConvertHTML2OOXML   (const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, THTMLParameters*     pParameters = nullptr);
	HRESULT ConvertHTML2Markdown(const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, TMarkdownParameters* pParameters = nullptr);

	HRESULT ConvertMHT2OOXML    (const std::wstring& wsPath,  const std::wstring& wsDirectory,  THTMLParameters*     pParameters = nullptr);
	HRESULT ConvertMHT2Markdown (const std::wstring& wsPath,  const std::wstring& wsFinalFile,  TMarkdownParameters* pParameters = nullptr);

	HRESULT ConvertMHT2OOXML    (const std::vector<std::wstring>& arPaths,  const std::wstring& wsDirectory,  THTMLParameters*     pParameters = nullptr);
	HRESULT ConvertMHT2Markdown (const std::vector<std::wstring>& arPaths,  const std::wstring& wsFinalFile,  TMarkdownParameters* pParameters = nullptr);

	NSCSS::CCssCalculator* GetCSSCalculator();
private:
	void Clear();
	void InitOOXMLTags(THTMLParameters* pParametrs = nullptr);
	void InitMDTags(TMarkdownParameters* pParametrs = nullptr);

	bool IsHTML();

	typedef std::function<bool(const std::wstring&, XmlUtils::CXmlLiteReader&)> Convert_Func;

	HRESULT InitAndConvert2OOXML(const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, Convert_Func Convertation, THTMLParameters* pParameters = nullptr);
	HRESULT InitAndConvert2Markdown(const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, Convert_Func Convertation, TMarkdownParameters* pParameters = nullptr);

	bool Convert(const std::wstring& wsPath, Convert_Func Convertation);

	void ReadStyle();
	void ReadStyle2();
	void ReadStyleFromNetwork();

	void ReadDocument();
	void ReadHead();
	void ReadBody();

	bool ReadStream(std::vector<NSCSS::CNode>& arSelectors, bool bInsertEmptyP = false);
	bool ReadInside(std::vector<NSCSS::CNode>& arSelectors);

	bool ReadText(std::vector<NSCSS::CNode>& arSelectors);

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
