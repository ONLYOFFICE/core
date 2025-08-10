#include "Fb2File.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/xml/include/xmlencoding.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/SystemUtils.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../OfficeUtils/src/OfficeUtils.h"
#include "../Common/3dParty/html/htmltoxhtml.h"
#include "./template/template.h"

#include <vector>
#include <map>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

// Ячейка таблицы
struct CTc
{
	int i;
	int j;
	std::wstring sGridSpan = L"1";

	CTc(int _i, int _j, const std::wstring& sColspan) : i(_i), j(_j), sGridSpan(sColspan) {}

	bool operator==(const CTc& c2)
	{
		return (i == c2.i && j == c2.j && sGridSpan == c2.sGridSpan);
	}
};

// Описание информации о произведении. Тэг title-info, src-title-info
struct STitleInfo
{
	std::wstring m_sGenres;    // Жанры
	std::wstring m_sAuthors;   // Авторы
	std::wstring m_sBookTitle; // Название
	std::wstring m_sKeywords;  // Ключевые слова
	std::wstring m_sAnnotation;// Аннотация
	/*
	std::vector<SAuthor> m_arTranslator;  // Переводчики
	std::wstring m_sLang;      // Язык после перевода

	std::pair<std::wstring, std::wstring>* m_pDate; // Дата
	std::wstring* m_pSrcLang;                       // Язык до перевода
	std::map<std::wstring, std::wstring> m_mSequence; // Серии книг
	*/
};

// Описание информации о fb2-документе. Тэг document-info
/*
struct SDocumentInfo
{
	std::vector<SAuthor> m_arAuthors;     // Авторы
	std::vector<std::wstring> m_arSrcUrl; // URL страницы

	std::wstring m_pDate; // Дата
	std::wstring m_sId;                            // Идентификатор
	std::wstring m_sVersion;                       // Версия документа

	std::wstring* m_pProgramUsed; // Использованные программы
	std::wstring* m_pSrcOcr;      // Автор текста
	std::wstring* m_pHistory;     // История

	SDocumentInfo()
	{
		m_pProgramUsed = NULL;
		m_pSrcOcr = NULL;
		m_pHistory = NULL;
	}

	~SDocumentInfo()
	{
		m_arAuthors.clear();
		m_arSrcUrl.clear();

		RELEASEARRAYOBJECTS(m_pProgramUsed);
		RELEASEARRAYOBJECTS(m_pSrcOcr);
		RELEASEARRAYOBJECTS(m_pHistory);
	}
};
*/

// Информация о бумажном издании. Тэг publish-info
/*
struct SPublishInfo
{
	std::wstring* m_pBookName;  // Название
	std::wstring* m_pPublisher; // Издательство
	std::wstring* m_pCity;      // Место
	std::wstring* m_pYear;      // Год
	std::wstring* m_pIsbn;      // ISBN

	std::map<std::wstring, std::wstring> m_mSequence; // Серии книг

	SPublishInfo()
	{
		m_pBookName = NULL;
		m_pPublisher = NULL;
		m_pCity = NULL;
		m_pYear = NULL;
		m_pIsbn = NULL;
	}

	~SPublishInfo()
	{
		m_mSequence.clear();

		RELEASEARRAYOBJECTS(m_pBookName);
		RELEASEARRAYOBJECTS(m_pPublisher);
		RELEASEARRAYOBJECTS(m_pCity);
		RELEASEARRAYOBJECTS(m_pYear);
		RELEASEARRAYOBJECTS(m_pIsbn);
	}
};
*/

void replace_all(std::wstring& s, const std::wstring& s1, const std::wstring& s2)
{
	size_t pos = s.find(s1);
	size_t l = s2.length();
	while (pos != std::string::npos)
	{
		if (!(s1 == L"&" && s2 == L"&amp;" && s.length() > pos + 4 && s[pos] == L'&' && s[pos + 1] == L'a' && s[pos + 2] == L'm' && s[pos + 3] == L'p' && s[pos + 4] == L';'))
			s.replace(pos, s1.length(), s2);
		pos = s.find(s1, pos + l);
	}
}

std::wstring EncodeXmlString(const std::wstring& s)
{
	std::wstring sRes = s;

	replace_all(sRes, L"&", L"&amp;");
	replace_all(sRes, L"<", L"&lt;");
	replace_all(sRes, L">", L"&gt;");
	replace_all(sRes, L"\"", L"&quot;");
	replace_all(sRes, L"\'", L"&#39;");
	replace_all(sRes, L"\n", L" ");
	replace_all(sRes, L"\r", L" ");
	replace_all(sRes, L"\t", L" ");

	return sRes;
}

enum class EParagraphPropertie
{
	Sup,
	Sub,
	Strikethrough,
	Emphasis,
	Strong
};

typedef std::vector<EParagraphPropertie> ParagraphProperties;

class CFb2File_Private
{
public:
	XmlUtils::CXmlLiteReader m_oLightReader;  // SAX Reader
	STitleInfo m_oTitleInfo;                  // Данные о книге
	// SDocumentInfo m_oDocumentInfo;         // Информация об fb2-документе
	// std::wstring m_sTmpFolder;             // Рабочая папка
	std::map<std::wstring, std::vector<std::wstring>> m_mImages; // Картинки
	std::map<std::wstring, std::wstring> m_mFootnotes;           // Сноски

	NSStringUtils::CStringBuilder m_oDocXmlRels;  // document.xml.rels
	NSStringUtils::CStringBuilder m_oNoteXmlRels; // footnotes.xml.rels

private:
	int m_nContentsId;       // ID содержания
	int m_nCrossReferenceId; // ID перекрестной ссылки
	int m_nHyperlinkId;      // ID внешней ссылки
	bool m_bFootnote;        // Чтение Footnote из html
	bool m_bInP;
	bool m_bInNote;
	bool m_bInTable;

	ParagraphProperties m_arParagraphProperties;

	// STitleInfo* m_pSrcTitleInfo;  // Данные об исходнике книги
	// SPublishInfo* m_pPublishInfo; // Сведения об издании книги
	// std::map<std::wstring, std::wstring> m_mCustomInfo;       // Произвольная информация

public:
	CFb2File_Private()
	{
		// m_pSrcTitleInfo = NULL;
		// m_pPublishInfo  = NULL;
		m_nContentsId       = 1;
		m_nCrossReferenceId = 1;
		m_nHyperlinkId      = 1;
		m_bFootnote = false;
		m_bInP      = false;
		m_bInNote   = false;
		m_bInTable  = false;
	}

	~CFb2File_Private()
	{
		m_oLightReader.Clear();
		m_mFootnotes.clear();
		m_mImages.clear();
		/*
		m_mCustomInfo.clear();
		if(m_pSrcTitleInfo)
			delete m_pSrcTitleInfo;
		if(m_pPublishInfo)
			delete m_pPublishInfo;
		*/
	}

	// wrapper для чтения из файла
	bool OpenFromFile(const std::wstring& sFile)
	{
		// считаем, что это хмл, так как проверка на формат будет раньше (x2t)
		std::string sFileContentUtf8 = XmlUtils::GetXmlContentAsUTF8(sFile);
		return m_oLightReader.FromStringA(sFileContentUtf8);
	}

	// Проверяет наличие тэга FictionBook
	bool isFictionBook()
	{
		return m_oLightReader.ReadNextNode() && m_oLightReader.GetName() == L"FictionBook";
	}

	void OpenP(NSStringUtils::CStringBuilder& oBuilder)
	{
		if (m_bInP || m_bInTable)
			return;

		oBuilder.WriteString(L"<p>");
		m_bInP = true;
	}

	void CloseP(NSStringUtils::CStringBuilder& oBuilder)
	{
		if (!m_bInP || m_bInTable)
			return;

		oBuilder.WriteString(L"</p>");
		m_bInP = false;
	}

	void WriteText(NSStringUtils::CStringBuilder& oBuilder, const std::wstring& wsText, const std::vector<EParagraphPropertie>& arPProperties)
	{
		if (wsText.end() == std::find_if_not(wsText.begin(), wsText.end(), [](wchar_t wchChar){ return iswspace(wchChar) && 0xa0 != wchChar;}))
			return;

		OpenP(oBuilder);

		for (const EParagraphPropertie ePropertie : arPProperties)
		{
			switch(ePropertie)
			{
				case EParagraphPropertie::Sup: oBuilder.WriteString(L"<sup>"); break;
				case EParagraphPropertie::Sub: oBuilder.WriteString(L"<sub>"); break;
				case EParagraphPropertie::Strikethrough: oBuilder.WriteString(L"<strikethrough>"); break;
				case EParagraphPropertie::Emphasis: oBuilder.WriteString(L"<emphasis>"); break;
				case EParagraphPropertie::Strong: oBuilder.WriteString(L"<strong>"); break;
			}
		}

		oBuilder.WriteEncodeXmlString(wsText);

		for (ParagraphProperties::const_reverse_iterator itPropertie = arPProperties.crbegin(); itPropertie != arPProperties.crend(); ++itPropertie)
		{
			switch(*itPropertie)
			{
				case EParagraphPropertie::Sup: oBuilder.WriteString(L"</sup>"); break;
				case EParagraphPropertie::Sub: oBuilder.WriteString(L"</sub>"); break;
				case EParagraphPropertie::Strikethrough: oBuilder.WriteString(L"</strikethrough>"); break;
				case EParagraphPropertie::Emphasis: oBuilder.WriteString(L"</emphasis>"); break;
				case EParagraphPropertie::Strong: oBuilder.WriteString(L"</strong>"); break;
			}
		}
	}

	// Читает image
	// НЕ имеет право писать p
	void readImage(NSStringUtils::CStringBuilder& oBuilder)
	{
		// Читаем href
		std::wstring sImageName;
		while (m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sName = m_oLightReader.GetName();
			size_t nLen = (sName.length() > 4 ? sName.length() - 4 : 0);
			if (sName.substr(nLen) == L"href")
			{
				std::wstring sText = m_oLightReader.GetText();
				if (sText.length() > 1)
				{
					sImageName = sText.substr(1);
					break;
				}
			}
		}
		m_oLightReader.MoveToElement();

		readCrossReference(oBuilder);

		std::map<std::wstring, std::vector<std::wstring>>::iterator it = m_mImages.find(sImageName);
		if (it != m_mImages.end())
		{
			// Пишем картинку в файл
			// extent
			oBuilder += L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"";
			oBuilder += it->second[1];
			oBuilder += L"\" cy=\"";
			oBuilder += it->second[2];
			// docPr
			oBuilder += L"\"/><wp:docPr id=\"";
			oBuilder += it->second[0];
			// graphic
			// pic:nvPicPr
			oBuilder += L"\" name=\"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"";
			oBuilder += it->second[0];
			// pic:blipFill
			// рельсы rPic + id
			oBuilder += L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic";
			oBuilder += it->second[0];
			// pic:spPr
			oBuilder += L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"";
			oBuilder += it->second[1];
			oBuilder += L"\" cy=\"";
			oBuilder += it->second[2];
			// Конец записи
			oBuilder += L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>";
		}
	}

	// Читает title
	void readTitle(const std::wstring& sLevel, NSStringUtils::CStringBuilder& oBuilder)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		bool bFirstP = true;
		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			// Пишем заголовок
			if (m_oLightReader.GetName() == L"p")
			{
				// Пишем title + sLevel
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"";
				oBuilder += sLevel;
				oBuilder += L"\"/>";
				// Заголовок книги с новой страницы
				if (sLevel == L"title" && bFirstP)
				{
					oBuilder += L"<w:pageBreakBefore/>";
					bFirstP = false;
				}
				oBuilder += L"</w:pPr>";
				// Пишем ссылку от оглавления
				std::wstring sContentsId;
				if (sLevel == L"title1")
				{
					sContentsId = std::to_wstring(m_nContentsId++);
					oBuilder += L"<w:bookmarkStart w:id=\"";
					oBuilder += sContentsId;
					oBuilder += L"\" w:name=\"_Toc";
					oBuilder += sContentsId;
					oBuilder += L"\"/>";
				}
				readP(L"", oBuilder);
				// Пишем ссылку от оглавления
				if (sLevel == L"title1")
				{
					oBuilder += L"<w:bookmarkEnd w:id=\"";
					oBuilder += sContentsId;
					oBuilder += L"\"/>";
				}
				oBuilder += L"</w:p>";
			}
		}
	}

	// Читает epigraph
	void readEpigraph(NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"p")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr>";
				readP(L"", oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"poem")
				readPoem(oBuilder);
			else if (sName == L"cite")
				readCite(oBuilder);
			else if (sName == L"empty-line")
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr></w:p>";
			// Автор эпиграфа выделяется полужирным
			else if (sName == L"text-author")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr>";
				readP(L"<w:b/>", oBuilder);
				oBuilder += L"</w:p>";
			}
		}
	}

	// Читает p
	// НЕ имеет право писать p
	// sRStyle - накопленный стиль
	void readP(const std::wstring& sRStyle, NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		int nDepth = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode2(nDepth))
		{
			std::wstring sName = m_oLightReader.GetName();
			// Читаем обычный текст
			if (sName == L"#text")
			{
				// Стиль текста
				oBuilder += L"<w:r><w:rPr>";
				oBuilder += sRStyle;
				// Сам текст
				oBuilder += L"</w:rPr><w:t xml:space=\"preserve\">";
				oBuilder.WriteEncodeXmlString(m_oLightReader.GetText());
				oBuilder += L"</w:t></w:r>";

			}
			// Читаем полужирный текст
			else if (sName == L"strong")
				readP(sRStyle + L"<w:b/>", oBuilder);
			// Читаем курсивный текст
			else if (sName == L"emphasis")
				readP(sRStyle + L"<w:i/>", oBuilder);
			// Читаем стилизованный текст
			else if (sName == L"style")
				readP(sRStyle, oBuilder);
			// Читаем ссылку
			else if (sName == L"a")
			{
				bool bCross = true, bMsoFootnote = false;
				// Читаем href
				std::wstring sRef;
				while (m_oLightReader.MoveToNextAttribute())
				{
					std::wstring sTName = m_oLightReader.GetName();
					std::wstring sTText = m_oLightReader.GetText();
					size_t nLen = (sTName.length() > 4 ? sTName.length() - 4 : 0);
					if (sTName.substr(nLen) == L"href")
					{

						size_t nRef = sTText.find('#');
						if (nRef != 0)
						{
							bCross = false;
							sRef = sTText;
						}
						else if (sTText.length() > 1)
							sRef = sTText.substr(1);
						break;
					}
					if (sTName == L"style" && sTText.find(L"mso-footnote-id") != std::wstring::npos)
						bMsoFootnote = true;
				}
				m_oLightReader.MoveToElement();
				if (m_bFootnote && bMsoFootnote)
					continue;

				if (bCross)
				{
					std::map<std::wstring, std::wstring>::iterator it = m_mFootnotes.find(sRef);
					if (it != m_mFootnotes.end())
					{
						// Читаем текст внутри сноски
						if (!bMsoFootnote)
							readP(sRStyle, oBuilder);
						// Стиль сноски
						oBuilder += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"";
						oBuilder += it->second;
						oBuilder += L"\"/></w:r>";
					}
					// Перекрестная ссылка
					else
					{
						oBuilder += L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"";
						oBuilder += sRef;
						oBuilder += L"\">";
						// Читаем текст внутри ссылки
						readP(sRStyle + L"<w:rStyle w:val=\"cross\"/>", oBuilder);
						oBuilder += L"</w:hyperlink>";
					}
				}
				else
				{
					// Пишем рельсы
					NSStringUtils::CStringBuilder* oRelationshipXml = &m_oDocXmlRels;
					if (m_bInNote)
						oRelationshipXml = &m_oNoteXmlRels;
					oRelationshipXml->WriteString(L"<Relationship Id=\"rHyp");
					oRelationshipXml->WriteString(std::to_wstring(m_nHyperlinkId));
					oRelationshipXml->WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
					oRelationshipXml->WriteEncodeXmlString(sRef);
					oRelationshipXml->WriteString(L"\" TargetMode=\"External\"/>");

					// Пишем в document.xml
					oBuilder.WriteString(L"<w:hyperlink w:tooltip=\"");
					oBuilder.WriteEncodeXmlString(sRef);
					oBuilder.WriteString(L"\" r:id=\"rHyp");
					oBuilder.WriteString(std::to_wstring(m_nHyperlinkId++));
					oBuilder.WriteString(L"\">");

					// Читаем текст внутри ссылки
					readP(sRStyle, oBuilder);
					oBuilder += L"</w:hyperlink>";
				}
			}
			// Читаем вычеркнутый текст
			else if (sName == L"strikethrough")
				readP(sRStyle + L"<w:strike/>", oBuilder);
			// Читает нижний текст
			else if (sName == L"sub")
				readP(sRStyle + L"<w:vertAlign w:val=\"subscript\"/>", oBuilder);
			// Читает верхний текст
			else if (sName == L"sup")
				readP(sRStyle + L"<w:vertAlign w:val=\"superscript\"/>", oBuilder);
			// Читает код
			else if (sName == L"code")
				readP(sRStyle + L"<w:rStyle w:val=\"code\"/>", oBuilder);
			// Читает картинку в тексте
			else if (sName == L"image")
				readImage(oBuilder);
		}
	}

	// Читает poem
	void readPoem(NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"title")
				readTitle(L"title4", oBuilder);
			else if (sName == L"epigraph")
				readEpigraph(oBuilder);
			else if (sName == L"stanza")
			{
				if (m_oLightReader.IsEmptyNode())
					continue;

				int nSDeath = m_oLightReader.GetDepth();
				while (m_oLightReader.ReadNextSiblingNode(nSDeath))
				{
					std::wstring sSName = m_oLightReader.GetName();
					if (sSName == L"title")
						readTitle(L"title5", oBuilder);
					else if (sSName == L"v" || sSName == L"subtitle")
					{
						oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr>";
						readP(L"", oBuilder);
						oBuilder += L"</w:p>";
					}
				}
			}
			// Автор поэмы выделяется полужирным
			else if (sName == L"text-author")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr>";
				readP(L"<w:b/>", oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"date")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr><w:r><w:t>";
				oBuilder.WriteEncodeXmlString(content());
				oBuilder += L"</w:t></w:r></w:p>";
			}
		}
	}

	// Читает cite
	void readCite(NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"p" || sName == L"subtitle")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr>";
				readP(L"", oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"empty-line")
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr></w:p>";
			else if (sName == L"poem")
				readPoem(oBuilder);
			else if (sName == L"table")
				readTable(oBuilder);
			// Автор цитаты выделяется полужирным
			else if (sName == L"text-author")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr>";
				readP(L"<w:b/>", oBuilder);
				oBuilder += L"</w:p>";
			}
		}
	}

	// Читает table
	void readTable(NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		// Стиль таблицы
		oBuilder += L"<w:tbl><w:tblPr><w:tblStyle w:val=\"table-t\"/><w:tblW w:w=\"0\" w:type=\"auto\"/><w:tblLayout w:type=\"fixed\"/></w:tblPr>";

		NSStringUtils::CStringBuilder oTmpBuilder;
		std::vector<CTc> mTable;
		int nGridCol = 0;
		int i = 1; // Строка
		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			// tr - строки в таблице
			if (m_oLightReader.GetName() != L"tr" || m_oLightReader.IsEmptyNode())
				continue;
			int nTCol = 0;
			int j = 1; // Столбец
			oTmpBuilder += L"<w:tr>";
			int nTrDeath = m_oLightReader.GetDepth();
			while (m_oLightReader.ReadNextSiblingNode(nTrDeath))
			{
				int nColspan = 1;
				int nRowspan = 1;
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() == L"colspan")
						nColspan = stoi(m_oLightReader.GetText());
					else if(m_oLightReader.GetName() == L"rowspan")
						nRowspan = stoi(m_oLightReader.GetText());
				}
				m_oLightReader.MoveToElement();

				// Вставляем ячейки до
				std::vector<CTc>::iterator it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
				std::vector<CTc>::iterator it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				while(it1 != mTable.end() || it2 != mTable.end())
				{
					oTmpBuilder.WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
					std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
					oTmpBuilder.WriteString(sCol);
					oTmpBuilder.WriteString(L"\"/></w:tcPr><w:p></w:p></w:tc>");
					j += stoi(sCol);
					it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
					it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				}

				oTmpBuilder += L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/>";
				if (nRowspan != 1)
				{
					oTmpBuilder.WriteString(L"<w:vMerge w:val=\"restart\"/>");
					std::wstring sColspan = std::to_wstring(nColspan);
					if (nRowspan == 0)
						mTable.push_back({0, j, sColspan});
					else
						for (int k = i + 1; k < i + nRowspan; k++)
							mTable.push_back({k, j, sColspan});
				}
				if (nColspan != 1)
				{
					oTmpBuilder.WriteString(L"<w:gridSpan w:val=\"");
					oTmpBuilder.WriteString(std::to_wstring(nColspan));
					oTmpBuilder.WriteString(L"\"/>");
					j += nColspan - 1;
				}
				oTmpBuilder.WriteString(L"</w:tcPr><w:p>");
				// Читаем th. Ячейка заголовка таблицы. Выравнивание посередине. Выделяется полужирным
				if (m_oLightReader.GetName() == L"th")
				{
					if (++nTCol > nGridCol)
						nGridCol = nTCol;
					oTmpBuilder += L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>";
					readP(L"<w:b/>", oTmpBuilder);
				}
				// Читаем td. Ячейка таблицы. Выравнивание вправо
				else if (m_oLightReader.GetName() == L"td")
				{
					if(++nTCol > nGridCol)
						nGridCol = nTCol;
					oTmpBuilder += L"<w:pPr><w:jc w:val=\"right\"/></w:pPr>";
					readP(L"", oTmpBuilder);
				}
				oTmpBuilder += L"</w:p></w:tc>";
				j++;

				// Вставляем ячейки после
				it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
				it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				while(it1 != mTable.end() || it2 != mTable.end())
				{
					oTmpBuilder.WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
					std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
					oTmpBuilder.WriteString(sCol);
					oTmpBuilder.WriteString(L"\"/></w:tcPr><w:p></w:p></w:tc>");
					j += stoi(sCol);
					it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
					it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				}
			}
			oTmpBuilder += L"</w:tr>";
			i++;
		}
		// Размеры таблицы
		std::wstring sGridCol;
		if (nGridCol != 0)
			sGridCol = std::to_wstring((int)(9570.0 / (double)nGridCol));
		oBuilder += L"<w:tblGrid>";
		for (int i = 0; i < nGridCol; i++)
		{
			oBuilder += L"<w:gridCol w:w=\"";
			oBuilder += sGridCol;
			oBuilder += L"\"/>";
		}
		oBuilder += L"</w:tblGrid>";
		// Конец таблицы
		oBuilder += oTmpBuilder.GetData();
		oBuilder += L"</w:tbl>";
		// Пустая строка после таблицы, чтобы следующий текст не приклеивался
		oBuilder += L"<w:p></w:p>";
	}

	void readAnnotationFromDescription(NSStringUtils::CStringBuilder& oBuilder, bool bInP)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		int nADeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode2(nADeath))
		{
			std::wstring sAnName = m_oLightReader.GetName();
			if (sAnName == L"#text")
			{
				if (!bInP)
					continue;
				m_oTitleInfo.m_sAnnotation += ((m_oTitleInfo.m_sAnnotation.empty() ? L"" : L" ") + m_oLightReader.GetText());
			}
			else
				readAnnotationFromDescription(oBuilder, bInP || sAnName == L"p" || sAnName == L"poem" || sAnName == L"cite" || sAnName == L"subtitle");
		}
	}

	// Читает annotation
	void readAnnotation(NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		int nADeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nADeath))
		{
			std::wstring sAnName = m_oLightReader.GetName();
			if (sAnName == L"p" || sAnName == L"subtitle")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"annotation\"/></w:pPr>";
				readP(L"", oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sAnName == L"poem")
				readPoem(oBuilder);
			else if (sAnName == L"cite")
				readCite(oBuilder);
			else if (sAnName == L"empty-line")
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"annotation\"/></w:pPr></w:p>";
			else if (sAnName == L"table")
				readTable(oBuilder);
		}
	}

	// Читает перекрестные ссылки
	void readCrossReference(NSStringUtils::CStringBuilder& oBuilder)
	{
		bool bPBB = false, bCBB = false;
		// id для перекрестных ссылок
		while (m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sAtrName = m_oLightReader.GetName();
			if (sAtrName == L"id")
			{
				std::wstring sCrossId = std::to_wstring(m_nCrossReferenceId++);
				oBuilder += L"<w:bookmarkStart w:id=\"";
				oBuilder += sCrossId;
				oBuilder += L"\" w:name=\"";
				oBuilder += m_oLightReader.GetText();
				oBuilder += L"\"/><w:bookmarkEnd w:id=\"";
				oBuilder += sCrossId;
				oBuilder += L"\"/>";
			}
			else if (sAtrName == L"style")
			{
				std::wstring sAtrText = m_oLightReader.GetText();
				bCBB = sAtrText.find(L"column-break-before") != std::wstring::npos;
				bPBB = sAtrText.find(L"page-break-before") != std::wstring::npos;
			}
		}
		m_oLightReader.MoveToElement();

		if (bPBB || bCBB)
		{
			oBuilder += L"<w:r><w:br w:type=\"";
			oBuilder += (bPBB ? L"page\" w:clear=\"all" : L"column");
			oBuilder += L"\"/></w:r>";
		}
	}

	// Читает section
	void readSection(int nLevel, NSStringUtils::CStringBuilder& oBuilder)
	{
		readCrossReference(oBuilder);

		if (m_oLightReader.IsEmptyNode())
			return;

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"title")
			{
				std::wstring sTitle = L"section-p";
				if (nLevel < 10)
					sTitle = L"title" + std::to_wstring(nLevel);
				readTitle(sTitle, oBuilder);
			}
			else if (sName == L"epigraph")
				readEpigraph(oBuilder);
			else if (sName == L"image")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"image\"/></w:pPr>";
				readImage(oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"annotation")
				readAnnotation(oBuilder);
			else if (sName == L"section")
				readSection(nLevel + 1, oBuilder);
			else if (sName == L"p")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"section-p\"/></w:pPr>";
				readP(L"", oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"subtitle")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"section-subtitle\"/></w:pPr>";
				readP(L"", oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"poem")
				readPoem(oBuilder);
			else if (sName == L"cite")
				readCite(oBuilder);
			else if (sName == L"empty-line")
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"section-p\"/></w:pPr></w:p>";
			else if (sName == L"table")
			   readTable(oBuilder);
		}
	}

	// Читает body
	void readBody(NSStringUtils::CStringBuilder& oBuilder)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"image")
			{
				oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"image\"/></w:pPr>";
				readImage(oBuilder);
				oBuilder += L"</w:p>";
			}
			else if (sName == L"title")
				readTitle(L"title", oBuilder);
			else if (sName == L"epigraph")
				readEpigraph(oBuilder);
			else if (sName == L"section")
				readSection(1, oBuilder);
		}
	}

	// Читает содержание, binary, body, сноски, description
	bool readText(const std::wstring& sPath, const std::wstring& sMediaDirectory, NSStringUtils::CStringBuilder& oContents, NSStringUtils::CStringBuilder& oFootnotes)
	{
		if (!m_oLightReader.IsValid())
		{
			// Открывает файл на проверку
			if (!OpenFromFile(sPath))
				return false;
			// Читаем FictionBook
			if (!isFictionBook())
				return false;
		}

		if (m_oLightReader.IsEmptyNode())
			return true;

		int nContentsId = 1;
		int nImageId    = 1;
		int nFootnoteId = 1;
		oContents += L"<w:sdt><w:sdtPr><w:docPartObj><w:docPartGallery w:val=\"Table of Contents\"/><w:docPartUnique/></w:docPartObj></w:sdtPr><w:sdtContent>";

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			// Читаем body
			if (sName == L"body")
			{
				// Сноски
				bool bNotes = false;
				while(m_oLightReader.MoveToNextAttribute())
				{
					if (m_oLightReader.GetName() == L"name" &&
						m_oLightReader.GetText() == L"notes")
						bNotes = true;
				}
				m_oLightReader.MoveToElement();
				// Читаем сноски
				if (bNotes)
					readNotes(nFootnoteId, oFootnotes);
				// Читаем содержание
				else
					readContents(nContentsId, oContents);
			}
			// Читаем картинки
			else if (sName == L"binary")
				getImage(std::to_wstring(nImageId++), sMediaDirectory);
		}
		oContents += L"</w:sdtContent></w:sdt>";
		return true;
	}

	// Читает содержание
	void readContents(int& nContentsId, NSStringUtils::CStringBuilder& oContents)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		bool bFirstTitle = true;
		int nBDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nBDeath))
		{
			// Читаем section
			if (m_oLightReader.GetName() != L"section" || m_oLightReader.IsEmptyNode())
				continue;
			int nSDeath = m_oLightReader.GetDepth();
			while (m_oLightReader.ReadNextSiblingNode(nSDeath))
			{
				// Читаем title
				if (m_oLightReader.GetName() != L"title" || m_oLightReader.IsEmptyNode())
					continue;
				oContents += L"<w:p><w:pPr><w:pStyle w:val=\"contents\"/><w:tabs><w:tab w:val=\"right\" w:pos=\"9355\" w:leader=\"none\"/></w:tabs>";
				// Абзац с новой страницы
				if (bFirstTitle)
					oContents += L"<w:pageBreakBefore/>";
				oContents += L"</w:pPr>";
				if (bFirstTitle)
				{
					oContents += L"<w:r><w:fldChar w:fldCharType=\"begin\"/><w:instrText xml:space=\"preserve\">TOC \\n \\h </w:instrText><w:fldChar w:fldCharType=\"separate\"/></w:r>";
					bFirstTitle = false;
				}
				oContents += L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"_Toc";
				oContents += std::to_wstring(nContentsId++);
				oContents += L"\" w:history=\"1\">";

				int nTDeath = m_oLightReader.GetDepth();
				while (m_oLightReader.ReadNextSiblingNode(nTDeath))
				{
					if (m_oLightReader.GetName() == L"p")
					{
						readP(L"<w:rStyle w:val=\"contents-p\"/>", oContents);
						// Вставляем пробел между словами
						oContents += L"<w:r><w:t xml:space=\"preserve\"> </w:t></w:r>";
					}
				}
				oContents += L"</w:hyperlink></w:p>";
			}
		}
		if (!bFirstTitle)
			oContents += L"<w:p><w:r><w:fldChar w:fldCharType=\"end\"/></w:r></w:p>";
	}

	// Читает сноски
	void readNotes(int& nFootnoteId, NSStringUtils::CStringBuilder& oFootnotes)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		m_bInNote = true;
		int nBDepth = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nBDepth))
		{
			if (m_oLightReader.GetName() != L"section")
				continue;
			// Читаем id
			std::wstring sFootnoteName;
			while (m_oLightReader.MoveToNextAttribute())
			{
				if (m_oLightReader.GetName() == L"id")
				{
					sFootnoteName = m_oLightReader.GetText();
					break;
				}
			}
			m_oLightReader.MoveToElement();
			if (sFootnoteName.empty() || m_oLightReader.IsEmptyNode())
				continue;

			std::wstring sFootnoteId = std::to_wstring(nFootnoteId++);
			m_mFootnotes.insert(std::make_pair(sFootnoteName, sFootnoteId));
			// Пишем сноску
			oFootnotes += L"<w:footnote w:id=\"";
			oFootnotes += sFootnoteId;
			oFootnotes += L"\">";

			// Читаем внутренность section
			int nSDepth = m_oLightReader.GetDepth();
			while (m_oLightReader.ReadNextSiblingNode(nSDepth))
			{
				std::wstring sName = m_oLightReader.GetName();
				if (sName == L"title")
				{
					if (m_oLightReader.IsEmptyNode())
						continue;

					int nTDepth = m_oLightReader.GetDepth();
					while (m_oLightReader.ReadNextSiblingNode(nTDepth))
					{
						if (m_oLightReader.GetName() == L"p")
							ReadFootnote(oFootnotes);
					}
				}
				else if (sName == L"p" || sName == L"subtitle")
					ReadFootnote(oFootnotes);
				else if (sName == L"poem")
				{
					if (m_oLightReader.IsEmptyNode())
						continue;

					int nPDepth = m_oLightReader.GetDepth();
					while (m_oLightReader.ReadNextSiblingNode(nPDepth))
					{
						std::wstring sPName = m_oLightReader.GetName();
						// Читаем stanza (один или более)
						if (sPName == L"stanza")
						{
							if (m_oLightReader.IsEmptyNode())
								continue;

							int nSDeath = m_oLightReader.GetDepth();
							while (m_oLightReader.ReadNextSiblingNode(nSDeath))
							{
								// Читаем v (один или более)
								if (m_oLightReader.GetName() == L"v")
									ReadFootnote(oFootnotes);
							}
						}
						// Читаем text-author (любое)
						else if (sPName == L"text-author")
							ReadFootnote(oFootnotes);
					}
				}
			}
			oFootnotes += L"</w:footnote>";
		}
		m_bInNote = false;
	}

	// Читает binary
	void getImage(const std::wstring& sImageId, const std::wstring& sMediaDirectory)
	{
		std::wstring sId, sType = L".png";
		while (m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sName = m_oLightReader.GetName();
			// Читает id
			if (sName == L"id")
				sId = m_oLightReader.GetText();
			else if (sName == L"content-type")
				sType = m_oLightReader.GetText().find(L"jpeg") == std::wstring::npos ? L".png" : L".jpeg";
		}
		m_oLightReader.MoveToElement();
		if (sId.empty() || m_oLightReader.IsEmptyNode())
			return;

		// Пишет картинку в файл
		NSFile::CFileBinary oImageWriter;
		std::wstring sImagePath = sMediaDirectory + L"/image" + sImageId + sType;
		if (oImageWriter.CreateFileW(sImagePath))
		{
			std::string sBase64 = contentA();
			int nSrcLen = (int)sBase64.length();
			int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
			BYTE* pImageData = new BYTE[nDecodeLen];
			if (TRUE == NSBase64::Base64Decode(sBase64.c_str(), nSrcLen, pImageData, &nDecodeLen))
				oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
			RELEASEARRAYOBJECTS(pImageData);
			oImageWriter.CloseFile();

			// Получаем размеры картинки
			CBgraFrame oBgraFrame;
			oBgraFrame.OpenFile(sImagePath);
			int nHy = oBgraFrame.get_Height();
			int nWx = oBgraFrame.get_Width();
			if (nWx > nHy)
			{
				int nW = nWx * 9525;
				nW = (nW > 7000000 ? 7000000 : nW);
				nHy = (int)((double)nHy * (double)nW / (double)nWx);
				nWx = nW;
			}
			else
			{
				int nH = nHy * 9525;
				nH = (nH > 9000000 ? 9000000 : nH);
				int nW = (int)((double)nWx * (double)nH / (double)nHy);
				if (nW > 7000000)
				{
					nW = 7000000;
					nHy = (int)((double)nHy * (double)nW / (double)nWx);
				}
				else
					nHy = nH;
				nWx = nW;
			}
			std::vector<std::wstring> vImage;
			vImage.push_back(sImageId);
			vImage.push_back(std::to_wstring(nWx));
			vImage.push_back(std::to_wstring(nHy));

			m_mImages.insert(std::make_pair(sId, vImage));
			// Запись картинок в рельсы
			m_oDocXmlRels += L"<Relationship Id=\"rPic";
			m_oDocXmlRels += sImageId;
			m_oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
			m_oDocXmlRels += (L"image" + sImageId + sType);
			m_oDocXmlRels += L"\"/>";
		}
	}

	// Читает description
	void readDescription(NSStringUtils::CStringBuilder& oBuilder)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		int nDepth = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDepth))
		{
			// Читаем title-info
			if (m_oLightReader.GetName() == L"title-info")
				getTitleInfo(oBuilder);
			/*
			// Читаем src-title-info (ноль или один)
			else if(sName == L"src-title-info")
			{
				m_pSrcTitleInfo = new STitleInfo();
				getTitleInfo(*m_pSrcTitleInfo);
			}
			// Читаем document-info
			else if(sName == L"document-info")
				getDocumentInfo();
			// Читаем publish-info (ноль или один)
			else if(sName == L"publish-info")
			{
				m_pPublishInfo = new SPublishInfo();
				getPublishInfo();
			}
			// Читаем custom-info (любое)
			else if(sName == L"custom-info")
				getCustomInfo();
			*/
		}
	}

	// Читает custom-info
	/*
	void getCustomInfo()
	{
		std::wstring sIntoType = L"";
		if(m_oLightReader.MoveToNextAttribute())
			sIntoType = m_oLightReader.GetText();
		m_oLightReader.MoveToElement();
		std::wstring sCustomInfo = content();
		m_mCustomInfo.insert(std::make_pair(sIntoType, sCustomInfo));
	}
	*/

	// Читает publish-info
	/*
	void getPublishInfo()
	{
		if(m_oLightReader.IsEmptyNode())
			return;

		int nDepth = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDepth))
		{
			std::wstring sName = m_oLightReader.GetName();
			// Читаем book-name (ноль или один)
			if(sName == L"book-name")
			{
				if(m_pPublishInfo->m_pBookName)
					delete[] m_pPublishInfo->m_pBookName;
				m_pPublishInfo->m_pBookName = new std::wstring[1];
				*m_pPublishInfo->m_pBookName = content();
			}
			// Читаем publisher (ноль или один)
			else if(sName == L"publisher")
			{
				if(m_pPublishInfo->m_pPublisher)
					delete[] m_pPublishInfo->m_pPublisher;
				m_pPublishInfo->m_pPublisher = new std::wstring[1];
				*m_pPublishInfo->m_pPublisher = content();
			}
			// Читаем city (ноль или один)
			else if(sName == L"city")
			{
				if(m_pPublishInfo->m_pCity)
					delete[] m_pPublishInfo->m_pCity;
				m_pPublishInfo->m_pCity = new std::wstring[1];
				*m_pPublishInfo->m_pCity = content();
			}
			// Читаем year (ноль или один)
			else if(sName == L"year")
			{
				if(m_pPublishInfo->m_pYear)
					delete[] m_pPublishInfo->m_pYear;
				m_pPublishInfo->m_pYear = new std::wstring[1];
				*m_pPublishInfo->m_pYear = content();
			}
			// Читаем isbn (ноль или один)
			else if(sName == L"isbn")
			{
				if(m_pPublishInfo->m_pIsbn)
					delete[] m_pPublishInfo->m_pIsbn;
				m_pPublishInfo->m_pIsbn = new std::wstring[1];
				*m_pPublishInfo->m_pIsbn = content();
			}
			// Читаем sequence (любое)
			else if(sName == L"sequence")
			{
				std::wstring sSName = L"";
				std::wstring sSNumber = L"";
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() == L"name")
						sSName = m_oLightReader.GetText();
					else if(m_oLightReader.GetName() == L"number")
						sSNumber = m_oLightReader.GetText();
				}
				m_oLightReader.MoveToElement();
				m_pPublishInfo->m_mSequence.insert(std::make_pair(sSName, sSNumber));
			}
		}
	}
	*/

	// Читает document-info
	/*
	void getDocumentInfo()
	{
		if(m_oLightReader.IsEmptyNode())
			return;

		int nDepth = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDepth))
		{
			std::wstring sName = m_oLightReader.GetName();
			// Читаем author (один или более)
			if(sName == L"author")
				getAuthor(m_oDocumentInfo.m_arAuthors);
			// Читаем program-used (ноль или один)
			else if(sName == L"program-used")
			{
				if(m_oDocumentInfo.m_pProgramUsed)
					delete[] m_oDocumentInfo.m_pProgramUsed;
				m_oDocumentInfo.m_pProgramUsed = new std::wstring[1];
				*m_oDocumentInfo.m_pProgramUsed = content();
			}
			// Читаем date
			else if(sName == L"date")
				m_oDocumentInfo.m_pDate = content();
			// Читаем src-url (любое)
			else if(sName == L"src-url")
				m_oDocumentInfo.m_arSrcUrl.push_back(content());
			// Читаем src-ocr (ноль или один)
			else if(sName == L"src-ocr")
			{
				if(m_oDocumentInfo.m_pSrcOcr)
					delete[] m_oDocumentInfo.m_pSrcOcr;
				m_oDocumentInfo.m_pSrcOcr = new std::wstring[1];
				*m_oDocumentInfo.m_pSrcOcr = content();
			}
			// Читаем id
			else if(sName == L"id")
				m_oDocumentInfo.m_sId = content();
			// Читаем version
			else if(sName == L"version")
				m_oDocumentInfo.m_sVersion = content();
			// Читаем history (ноль или один)
			else if(sName == L"history")
			{
				if(m_oDocumentInfo.m_pHistory)
					delete[] m_oDocumentInfo.m_pHistory;
				m_oDocumentInfo.m_pHistory = new std::wstring[1];
				*m_oDocumentInfo.m_pHistory = L"";
			}
		}
	}
	*/

	// Читает title-info и src-title-info
	void getTitleInfo(NSStringUtils::CStringBuilder& oBuilder)
	{
		if (m_oLightReader.IsEmptyNode())
			return;

		int nDepth = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDepth))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"annotation")
				readAnnotationFromDescription(oBuilder, false);
			else if (sName == L"coverpage")
			{
				if (m_oLightReader.IsEmptyNode())
					continue;

				int nCDepth = m_oLightReader.GetDepth();
				while (m_oLightReader.ReadNextSiblingNode(nCDepth))
				{
					if (m_oLightReader.GetName() == L"image")
					{
						oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"image\"/></w:pPr>";
						readImage(oBuilder);
						oBuilder += L"</w:p>";
					}
				}
			}
			else if (sName == L"genre")
			{
				std::wstring sGenre = content();
				m_oTitleInfo.m_sGenres += ((m_oTitleInfo.m_sGenres.empty() ? L"" : L", ") + sGenre);
			}
			// Читает поля автора
			else if (sName == L"author")
			{
				std::wstring sFirstName, sMiddleName, sLastName, sNickname;
				int nDepth = m_oLightReader.GetDepth();
				while (m_oLightReader.ReadNextSiblingNode(nDepth))
				{
					std::wstring sName = m_oLightReader.GetName();
					if (sName == L"first-name")
						sFirstName  = content();
					else if (sName == L"middle-name")
						sMiddleName = content();
					else if (sName == L"last-name")
						sLastName   = content();
					else if (sName == L"nickname")
						sNickname    = content();
					/*
					else if(sName == L"home-page")
						oAuthor.home_page.push_back(content());
					else if(sName == L"email")
						oAuthor.email.push_back(content());
					else if(sName == L"id")
						oAuthor.id = content();
					*/
				}

				m_oTitleInfo.m_sAuthors +=
						((m_oTitleInfo.m_sAuthors.empty() ? L"" : L";") +
						(sLastName.empty() ? L"" : (sLastName + L' ')) +
						(sFirstName.empty()  ? L"" : (sFirstName  + L' ')) +
						(sMiddleName.empty()   ? L"" : (sMiddleName   + L' ')) +
						 sNickname);
			}
			else if (sName == L"book-title")
				m_oTitleInfo.m_sBookTitle = content();
			// Читаем keywords (ноль или один)
			else if(sName == L"keywords")
				m_oTitleInfo.m_sKeywords = content();
			/*
			// Читаем date (ноль или один)
			else if(sName == L"date")
			{
				if(oTitleInfo.m_pDate)
					delete[] oTitleInfo.m_pDate;
				oTitleInfo.m_pDate = new std::pair<std::wstring, std::wstring>[1];
				std::wstring sDate = L"";
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() == L"name")
						sDate = m_oLightReader.GetText();
				}
				m_oLightReader.MoveToElement();
				*oTitleInfo.m_pDate = make_pair(sDate, content());
			}
			// Читаем lang
			else if(sName == L"lang")
				oTitleInfo.m_sLang = content();
			// Читаем src-lang (ноль или один)
			else if(sName == L"src-lang")
			{
				if(oTitleInfo.m_pSrcLang)
					delete[] oTitleInfo.m_pSrcLang;
				oTitleInfo.m_pSrcLang = new std::wstring[1];
				*oTitleInfo.m_pSrcLang = content();
			}
			// Читаем translator (любое)
			else if(sName == L"translator")
				getAuthor(oTitleInfo.m_arTranslator);
			// Читаем sequence (любое)
			else if(sName == L"sequence")
			{
				std::wstring sSName = L"";
				std::wstring sSNumber = L"";
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() == L"name")
						sSName = m_oLightReader.GetText();
					else if(m_oLightReader.GetName() == L"number")
						sSNumber = m_oLightReader.GetText();
				}
				m_oLightReader.MoveToElement();
				oTitleInfo.m_mSequence.insert(std::make_pair(sSName, sSNumber));
			}
			*/
		}
	}

	// Возвращает содержание узла
	std::wstring content()
	{
		if (!m_oLightReader.IsEmptyNode() && m_oLightReader.ReadNextSiblingNode2(m_oLightReader.GetDepth()))
			return m_oLightReader.GetText();
		return L"";
	}

	std::string contentA()
	{
		if(!m_oLightReader.IsEmptyNode() && m_oLightReader.ReadNextSiblingNode2(m_oLightReader.GetDepth()))
			return m_oLightReader.GetTextA();
		return "";
	}

	std::wstring GenerateUUID()
	{
		std::mt19937 oRand(time(0));
		std::wstringstream sstream;
		sstream << std::setfill(L'0') << std::hex << std::setw(8) << (oRand() & 0xffffffff);
		sstream << L'-';
		sstream << std::setfill(L'0') << std::hex << std::setw(4) << (oRand() & 0xffff);
		sstream << L'-';
		sstream << std::setfill(L'0') << std::hex << std::setw(4) << (oRand() & 0xffff);
		sstream << L'-';
		sstream << std::setfill(L'0') << std::hex << std::setw(4) << (oRand() & 0xffff);
		sstream << L'-';
		sstream << std::setfill(L'0') << std::hex << std::setw(8) << (oRand() & 0xffffffff);
		return sstream.str();
	}

	// html -> fb2

	void readStream(NSStringUtils::CStringBuilder& oXml, ParagraphProperties& arPProperties)
	{
		int nDepth = m_oLightReader.GetDepth();
		if (m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDepth))
			return;
		do
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"#text")
				WriteText(oXml, m_oLightReader.GetText(), arPProperties);
			else if (sName == L"br" && !m_bInTable)
			{
				bool bPBB = false, bCBB = false;
				while (m_oLightReader.MoveToNextAttribute())
				{
					if (m_oLightReader.GetName() != L"style")
						continue;
					std::wstring sAtrText = m_oLightReader.GetText();
					bCBB = sAtrText.find(L"column-break-before") != std::wstring::npos;
					bPBB = sAtrText.find(L"page-break-before") != std::wstring::npos;
				}
				m_oLightReader.MoveToElement();

				std::wstring sStyle;
				if (bPBB || bCBB)
				{
					sStyle = L" style=\"";
					sStyle += (bPBB ? L"page" : L"column");
					sStyle += L"-break-before\"";
				}

				if (m_bInP)
					oXml.WriteString(L"</p><p" + sStyle + L">");
				else
					oXml.WriteString(L"<p" + sStyle + L"></p>");
			}
			else if (sName == L"div")
			{
				std::wstring sFootnoteName;
				NSStringUtils::CStringBuilder oFootnote;
				while (m_oLightReader.MoveToNextAttribute())
				{
					std::wstring sAtrName = m_oLightReader.GetName();
					std::wstring sAtrText = m_oLightReader.GetText();
					if (sAtrName == L"style" && sAtrText == L"mso-element:footnote")
						m_bFootnote = true;
					else if (sAtrName == L"id")
						sFootnoteName = sAtrText;
				}
				m_oLightReader.MoveToElement();
				if (m_bFootnote && !sFootnoteName.empty())
				{
					readStream(oFootnote, arPProperties);
					m_mFootnotes.insert(std::make_pair(sFootnoteName, oFootnote.GetData()));
					m_bFootnote = false;
				}
				else
					readStream(oXml, arPProperties);
			}
			else if (sName == L"p")
			{
				OpenP(oXml);
				readStream(oXml, arPProperties);
				CloseP(oXml);
			}
			else if (sName == L"title")
			{
				m_oTitleInfo.m_sBookTitle = EncodeXmlString(m_oLightReader.GetText2());
			}
			else if (sName == L"meta")
			{
				std::wstring sAtrName, sAtrContent;
				while (m_oLightReader.MoveToNextAttribute())
				{
					std::wstring sAtr = m_oLightReader.GetName();
					if (sAtr == L"name")
						sAtrName = m_oLightReader.GetText();
					else if (sAtr == L"content")
						sAtrContent = m_oLightReader.GetText();
				}
				m_oLightReader.MoveToElement();

				if (!sAtrName.empty())
				{
					if (sAtrName == L"creator")
						m_oTitleInfo.m_sAuthors = EncodeXmlString(sAtrContent);
					else if (sAtrName == L"description")
						m_oTitleInfo.m_sAnnotation = EncodeXmlString(sAtrContent);
					else if (sAtrName == L"subject")
						m_oTitleInfo.m_sGenres = EncodeXmlString(sAtrContent);
					else if (sAtrName == L"keywords")
						m_oTitleInfo.m_sKeywords = EncodeXmlString(sAtrContent);
				}
			}
			else if (sName == L"h1" || sName == L"h2" || sName == L"h3" || sName == L"h4" || sName == L"h5" || sName == L"h6")
			{
				bool bInH = false;
				int nH = 0;
				if (!m_bInTable && !m_bInP)
				{
					nH = stoi(sName.substr(1));
					for (int i = 0; i < nH; ++i)
						oXml.WriteString(L"<section>");
					oXml.WriteString(L"<title><p>");
					bInH = true;
					m_bInP = true;
				}
				readStream(oXml, arPProperties);
				if (bInH)
				{
					oXml.WriteString(L"</p></title>");
					for (int i = 0; i < nH; ++i)
						oXml.WriteString(L"</section>");
					m_bInP = false;
				}
			}
			else if (sName == L"span")
			{
				std::wstring sStyle;
				while (m_oLightReader.MoveToNextAttribute())
					if (m_oLightReader.GetName() == L"style")
						sStyle = m_oLightReader.GetText();
				m_oLightReader.MoveToElement();

				std::wstring sAlign;
				size_t nAlign = sStyle.find(L"vertical-align");
				if (nAlign != std::wstring::npos)
				{
					nAlign = sStyle.find(L':', nAlign);
					size_t nAlignEnd = sStyle.find(L';', nAlign);
					sAlign = sStyle.substr(nAlign + 1, (nAlignEnd < sStyle.length() ? nAlignEnd : sStyle.length()) - nAlign);
					if (sAlign == L"super")
					{
						arPProperties.push_back(EParagraphPropertie::Sup);
						readStream(oXml, arPProperties);
						arPProperties.pop_back();
					}
					else if (sAlign == L"sub")
					{
						arPProperties.push_back(EParagraphPropertie::Sub);
						readStream(oXml, arPProperties);
						arPProperties.pop_back();
					}
					else
						readStream(oXml, arPProperties);
				}
				else
					readStream(oXml, arPProperties);
			}
			else if (sName == L"s")
			{
				arPProperties.push_back(EParagraphPropertie::Strikethrough);
				readStream(oXml, arPProperties);
				arPProperties.pop_back();
			}
			else if (sName == L"i")
			{
				arPProperties.push_back(EParagraphPropertie::Emphasis);
				readStream(oXml, arPProperties);
				arPProperties.pop_back();
			}
			else if (sName == L"b")
			{
				arPProperties.push_back(EParagraphPropertie::Strong);
				readStream(oXml, arPProperties);
				arPProperties.pop_back();
			}
			else if (sName == L"table")
			{
				oXml.WriteString(L"<table>");
				m_bInTable = true;

				ParagraphProperties arTableProperties;
				readStream(oXml, arTableProperties);

				oXml.WriteString(L"</table>");
				m_bInTable = false;
			}
			else if (sName == L"tr")
			{
				oXml.WriteString(L"<tr>");
				readStream(oXml, arPProperties);
				oXml.WriteString(L"</tr>");
			}
			else if (sName == L"td" || sName == L"th")
			{
				oXml.WriteString(L"<td");
				while (m_oLightReader.MoveToNextAttribute())
				{
					if (m_oLightReader.GetName() == L"colspan")
						oXml.WriteString(L" colspan=\"" + m_oLightReader.GetText() + L"\"");
					else if (m_oLightReader.GetName() == L"rowspan")
						oXml.WriteString(L" rowspan=\"" + m_oLightReader.GetText() + L"\"");
				}
				m_oLightReader.MoveToElement();
				oXml.WriteString(L">");
				readStream(oXml, arPProperties);
				oXml.WriteString(L"</td>");
			}
			else if (sName == L"a")
			{
				bool bFootnote = false;
				oXml.WriteString(L"<a ");
				while (m_oLightReader.MoveToNextAttribute())
				{
					std::wstring sAtrName = m_oLightReader.GetName();
					std::wstring sAtrText = m_oLightReader.GetText();
					if (!sAtrName.empty() && !sAtrText.empty())
					{
						if (!m_bFootnote && sAtrName == L"style" && sAtrText.find(L"mso-footnote-id") != std::wstring::npos)
						{
							oXml.WriteString(sAtrName + L"=\"");
							oXml.WriteEncodeXmlString(sAtrText);
							oXml.WriteString(L"\" ");

							sAtrText = sAtrText.substr(sAtrText.rfind(L':') + 1);
							oXml.WriteString(L"href=\"#");
							oXml.WriteString(sAtrText);
							oXml.WriteString(L"\" type=\"note\" ");
							bFootnote = true;
						}
						else
						{
							if (bFootnote && (sAtrName == L"href" || sAtrName == L"type"))
								continue;
							if (sAtrName == L"name")
								sAtrName = L"id";
							oXml.WriteString(sAtrName + L"=\"");
							oXml.WriteEncodeXmlString(sAtrText);
							oXml.WriteString(L"\" ");
						}
					}
				}
				m_oLightReader.MoveToElement();
				oXml.WriteString(L">");

				readStream(oXml, arPProperties);
				oXml.WriteString(L"</a>");
			}
			else if (sName == L"ul")
				readLi(oXml, true, arPProperties);
			else if (sName == L"ol")
				readLi(oXml, false, arPProperties);
			else if (sName == L"img")
			{
				std::wstring sId, sBinary;
				while (m_oLightReader.MoveToNextAttribute())
				{
					if (m_oLightReader.GetName() == L"src")
					{
						sBinary = m_oLightReader.GetText();
						sBinary.erase(0, sBinary.find(L',') + 1);
						std::vector<std::wstring> vImage;
						vImage.push_back(sBinary);
						sId = std::to_wstring(m_mImages.size() + 1);
						m_mImages.insert(std::make_pair(sId, vImage));
					}
				}
				m_oLightReader.MoveToElement();
				oXml.WriteString(L"<image l:href=\"#img" + sId + L".png\"/>");
			}
			else
				readStream(oXml, arPProperties);
		} while (m_oLightReader.ReadNextSiblingNode2(nDepth));
	}

	void readLi(NSStringUtils::CStringBuilder& oXml, bool bUl, ParagraphProperties& arProperties)
	{
		int nNum = 1;
		while (m_oLightReader.MoveToNextAttribute())
			if (m_oLightReader.GetName() == L"start")
				nNum = std::stoi(m_oLightReader.GetText());
		m_oLightReader.MoveToElement();
		int nDeath = m_oLightReader.GetDepth();
		if (m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
			return;
		do
		{
			if (m_oLightReader.GetName() == L"li")
			{
				OpenP(oXml);

				if (bUl)
					oXml.AddCharSafe(183);
				else
				{
					std::wstring sPoint = std::to_wstring(nNum) + L'.';
					while (m_oLightReader.MoveToNextAttribute())
					{
						if (m_oLightReader.GetName() == L"style")
						{
							std::wstring sText = m_oLightReader.GetText();
							size_t nListType = sText.find(L"list-style-type: ");
							if (nListType != std::wstring::npos)
							{
								nListType += 17;
								size_t nListTypeEnd = sText.find(L';', nListType);
								std::wstring sListType = sText.substr(nListType, nListTypeEnd - nListType);
								if (sListType == L"decimal")
									break;
								else if (sListType == L"upper-alpha")
									sPoint = std::wstring((nNum - 1) / 26 + 1, L'A' + (nNum - 1) % 26);
								else if (sListType == L"lower-alpha")
									sPoint = std::wstring((nNum - 1) / 26 + 1, L'a' + (nNum - 1) % 26);
								else if (sListType == L"lower-roman")
									sPoint = ToLowerRoman(nNum);
								else if (sListType == L"upper-roman")
									sPoint = ToUpperRoman(nNum);
								sPoint += L'.';
							}
						}
					}
					m_oLightReader.MoveToElement();

					nNum++;
					oXml.WriteString(sPoint);
				}
				oXml.WriteString(L" ");
				readStream(oXml, arProperties);

				CloseP(oXml);
			}
		} while (m_oLightReader.ReadNextSiblingNode2(nDeath));
	}

	void readTitleInfo(NSStringUtils::CStringBuilder& oTitleInfo)
	{
		if (!m_oTitleInfo.m_sBookTitle.empty())
		{
			oTitleInfo.WriteString(L"<book-title>");
			oTitleInfo.WriteString(m_oTitleInfo.m_sBookTitle);
			oTitleInfo.WriteString(L"</book-title>");
		}
		if (!m_oTitleInfo.m_sAuthors.empty())
		{
			oTitleInfo.WriteString(L"<author><nickname>");
			oTitleInfo.WriteString(m_oTitleInfo.m_sAuthors);
			oTitleInfo.WriteString(L"</nickname></author>");
		}
		if (!m_oTitleInfo.m_sGenres.empty())
		{
			oTitleInfo.WriteString(L"<genre>");
			oTitleInfo.WriteString(m_oTitleInfo.m_sGenres);
			oTitleInfo.WriteString(L"</genre>");
		}
		if (!m_oTitleInfo.m_sKeywords.empty())
		{
			oTitleInfo.WriteString(L"<keywords>");
			oTitleInfo.WriteString(m_oTitleInfo.m_sKeywords);
			oTitleInfo.WriteString(L"</keywords>");
		}
		if (!m_oTitleInfo.m_sAnnotation.empty())
		{
			oTitleInfo.WriteString(L"<annotation><p>");
			oTitleInfo.WriteString(m_oTitleInfo.m_sAnnotation);
			oTitleInfo.WriteString(L"</p></annotation>");
		}
	}

	std::wstring ToUpperRoman(int number)
	{
		if (number < 0 || number > 3999) return L"";
		if (number < 1) return L"";
		if (number >= 1000) return L"M" + ToUpperRoman(number - 1000);
		if (number >= 900) return L"CM" + ToUpperRoman(number - 900);
		if (number >= 500) return L"D"  + ToUpperRoman(number - 500);
		if (number >= 400) return L"CD" + ToUpperRoman(number - 400);
		if (number >= 100) return L"C"  + ToUpperRoman(number - 100);
		if (number >= 90) return L"XC"  + ToUpperRoman(number - 90);
		if (number >= 50) return L"L"   + ToUpperRoman(number - 50);
		if (number >= 40) return L"XL"  + ToUpperRoman(number - 40);
		if (number >= 10) return L"X"   + ToUpperRoman(number - 10);
		if (number >= 9) return L"IX"   + ToUpperRoman(number - 9);
		if (number >= 5) return L"V"    + ToUpperRoman(number - 5);
		if (number >= 4) return L"IV"   + ToUpperRoman(number - 4);
		if (number >= 1) return L"I"    + ToUpperRoman(number - 1);
		return L"";
	}

	std::wstring ToLowerRoman(int number)
	{
		if (number < 0 || number > 3999) return L"";
		if (number < 1) return L"";
		if (number >= 1000) return L"m" + ToLowerRoman(number - 1000);
		if (number >= 900) return L"cm" + ToLowerRoman(number - 900);
		if (number >= 500) return L"d"  + ToLowerRoman(number - 500);
		if (number >= 400) return L"cd" + ToLowerRoman(number - 400);
		if (number >= 100) return L"c"  + ToLowerRoman(number - 100);
		if (number >= 90) return L"xc"  + ToLowerRoman(number - 90);
		if (number >= 50) return L"l"   + ToLowerRoman(number - 50);
		if (number >= 40) return L"xl"  + ToLowerRoman(number - 40);
		if (number >= 10) return L"x"   + ToLowerRoman(number - 10);
		if (number >= 9) return L"ix"   + ToLowerRoman(number - 9);
		if (number >= 5) return L"v"    + ToLowerRoman(number - 5);
		if (number >= 4) return L"iv"   + ToLowerRoman(number - 4);
		if (number >= 1) return L"i"    + ToLowerRoman(number - 1);
		return L"";
	}

private:
	void ReadFootnote(NSStringUtils::CStringBuilder& oFootnotes)
	{
		oFootnotes += L"<w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/></w:r>";
		m_bFootnote = true;
		readP(L"", oFootnotes);
		m_bFootnote = false;
		oFootnotes += L"</w:p>";
	}
};

CFb2File::CFb2File()
{
	m_internal = new CFb2File_Private();
}

CFb2File::~CFb2File()
{
	RELEASEOBJECT(m_internal);
}

// Проверяет, соответствует ли fb2 файл формату
bool CFb2File::IsFb2File(const std::wstring& sFile)
{
	// Открывает файл на проверку
	if (!m_internal->OpenFromFile(sFile))
		return false;
	// Читаем FictionBook
	return m_internal->isFictionBook();
}

// Выставление рабочей папки
void CFb2File::SetTmpDirectory(const std::wstring& sFolder)
{
	// m_internal->m_sTmpFolder = sFolder;
}

// sPath - путь к файлу fb2, sDirectory - директория, где формируется и создается docx
HRESULT CFb2File::Open(const std::wstring& sPath, const std::wstring& sDirectory, CFb2Params* oParams)
{
	// Копирование шаблона
	if (!ExtractTemplate(sDirectory))
		return S_FALSE;

	// Начало файла
	NSStringUtils::CStringBuilder oDocument;
	oDocument += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";

	// Создаем сноски
	NSStringUtils::CStringBuilder oFootnotes;
	oFootnotes += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
	oFootnotes += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";

	// Создаем рельсы
	m_internal->m_oDocXmlRels  += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
	m_internal->m_oDocXmlRels  += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
	m_internal->m_oDocXmlRels  += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
	m_internal->m_oDocXmlRels  += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
	m_internal->m_oDocXmlRels  += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
	m_internal->m_oDocXmlRels  += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
	m_internal->m_oDocXmlRels  += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";
	m_internal->m_oNoteXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";

	// Директория картинок
	std::wstring sMediaDirectory = sDirectory + L"/word/media";
	NSDirectory::CreateDirectory(sMediaDirectory);

	NSStringUtils::CStringBuilder oBuilder;
	NSStringUtils::CStringBuilder oDescription;
	NSStringUtils::CStringBuilder oContents;

	// Читаем содержание, картинки, сноски
	bool bNeedContents = false;
	if (oParams)
		bNeedContents = oParams->bNeedContents;
	if (!m_internal->readText(sPath, sMediaDirectory, oContents, oFootnotes))
		return S_FALSE;

	// Переходим в начало
	if (!m_internal->m_oLightReader.MoveToStart())
		return S_FALSE;

	int nDeath = m_internal->m_oLightReader.GetDepth();
	while (m_internal->m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		std::wstring sName = m_internal->m_oLightReader.GetName();
		if (sName == L"description")
			m_internal->readDescription(oDescription);
		else if (sName == L"body")
		{
			bool bNotes = false;
			while (m_internal->m_oLightReader.MoveToNextAttribute())
			{
				if (m_internal->m_oLightReader.GetName() == L"name" &&
					m_internal->m_oLightReader.GetText() == L"notes")
					bNotes = true;
			}
			m_internal->m_oLightReader.MoveToElement();
			if (bNotes)
				continue;
			m_internal->readBody(oBuilder);
		}
	}

	// Конец сносок
	oFootnotes += L"</w:footnotes>";
	// Пишем сноски в файл
	NSFile::CFileBinary oFootnotesWriter;
	if (oFootnotesWriter.CreateFileW(sDirectory + L"/word/footnotes.xml"))
	{
		oFootnotesWriter.WriteStringUTF8(oFootnotes.GetData());
		oFootnotesWriter.CloseFile();
	}

	// Вставляем description
	oDocument += oDescription.GetData();
	// Вставляем содержание
	if (bNeedContents)
		oDocument += oContents.GetData();
	// Вставляем основное тело
	oDocument += oBuilder.GetData();
	// Конец документа
	oDocument += L"<w:sectPr><w:footnotePr/><w:type w:val=\"nextPage\"/><w:pgSz w:w=\"11906\" w:h=\"16838\" w:orient=\"portrait\"/><w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"709\" w:footer=\"709\" w:gutter=\"0\"/><w:cols w:num=\"1\" w:sep=\"0\" w:space=\"708\" w:equalWidth=\"1\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body></w:document>";
	// Пишем документ в файл
	NSFile::CFileBinary oDocumentXmlWriter;
	if (oDocumentXmlWriter.CreateFileW(sDirectory + L"/word/document.xml"))
	{
		oDocumentXmlWriter.WriteStringUTF8(oDocument.GetData());
		oDocumentXmlWriter.CloseFile();
	}

	// Директория app и core
	std::wstring sDocPropsDirectory = sDirectory + L"/docProps";
	NSDirectory::CreateDirectory(sDocPropsDirectory);

	// Создаем core.xml
	NSStringUtils::CStringBuilder oCore;
	// Заголовок
	oCore += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"><dc:title>";
	oCore.WriteString(EncodeXmlString(m_internal->m_oTitleInfo.m_sBookTitle));
	// Жанры
	oCore.WriteString(L"</dc:title>");
	if (!m_internal->m_oTitleInfo.m_sGenres.empty())
	{
		oCore.WriteString(L"<dc:subject>");
		oCore.WriteString(EncodeXmlString(m_internal->m_oTitleInfo.m_sGenres));
		oCore.WriteString(L"</dc:subject>");
	}
	// Авторы
	if (!m_internal->m_oTitleInfo.m_sAuthors.empty())
	{
		oCore.WriteString(L"<dc:creator>");
		oCore.WriteString(EncodeXmlString(m_internal->m_oTitleInfo.m_sAuthors));
		oCore.WriteString(L"</dc:creator>");
	}
	// Ключевые слова
	if (!m_internal->m_oTitleInfo.m_sKeywords.empty())
	{
		oCore.WriteString(L"<cp:keywords>");
		oCore.WriteString(EncodeXmlString(m_internal->m_oTitleInfo.m_sKeywords));
		oCore.WriteString(L"</cp:keywords>");
	}
	if (!m_internal->m_oTitleInfo.m_sAnnotation.empty())
	{
		oCore.WriteString(L"<dc:description>");
		oCore.WriteString(EncodeXmlString(m_internal->m_oTitleInfo.m_sAnnotation));
		oCore.WriteString(L"</dc:description>");
	}
	// Конец core
	oCore += L"</cp:coreProperties>";
	// Пишем core в файл
	NSFile::CFileBinary oCoreWriter;
	if (oCoreWriter.CreateFileW(sDocPropsDirectory + L"/core.xml"))
	{
		oCoreWriter.WriteStringUTF8(oCore.GetData());
		oCoreWriter.CloseFile();
	}

	// Получаем версию ONLYOFFICE
	std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
	if (sApplication.empty())
		sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
	#if defined(INTVER)
	std::string sVersion = VALUE2STR(INTVER);
	#endif
	sApplication += L"/";
	sApplication += UTF8_TO_U(sVersion);
	// Создаем app.xml
	NSStringUtils::CStringBuilder oApp;
	oApp += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"><Application>";
	oApp += sApplication;
	oApp += L"</Application><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><LinksUpToDate>false</LinksUpToDate><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc></Properties>";
	// Пишем app в файл
	NSFile::CFileBinary oAppWriter;
	if (oAppWriter.CreateFileW(sDocPropsDirectory + L"/app.xml"))
	{
		oAppWriter.WriteStringUTF8(oApp.GetData());
		oAppWriter.CloseFile();
	}

	// Конец рельсов
	m_internal->m_oDocXmlRels += L"</Relationships>";
	// Пишем рельсы в файл
	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(sDirectory + L"/word/_rels/document.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(m_internal->m_oDocXmlRels.GetData());
		oRelsWriter.CloseFile();
	}
	m_internal->m_oNoteXmlRels += L"</Relationships>";
	if (oRelsWriter.CreateFileW(sDirectory + L"/word/_rels/footnotes.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(m_internal->m_oNoteXmlRels.GetData());
		oRelsWriter.CloseFile();
	}

	// Архивим в docx
	bool bNeedDocx = false;
	if (oParams)
		bNeedDocx = oParams->bNeedDocx;
	if (bNeedDocx)
	{
		COfficeUtils oZip;
		return oZip.CompressFileOrDirectory(sDirectory, sDirectory + L"/" + NSFile::GetFileName(sPath) + L".docx");
	}
	return S_OK;
}

// sHtmlFile - путь к файлу html, sDst - путь к результирующему файлу fb2, sInpTitle - входящий заголовок файла
HRESULT CFb2File::FromHtml(const std::wstring& sHtmlFile, const std::wstring& sDst, const std::wstring& sInpTitle)
{
	std::wstring sTitle = sInpTitle;
	if (sTitle.empty())
		sTitle = NSFile::GetFileName(sDst);

	BYTE* pData;
	DWORD nLength;
	if (!NSFile::CFileBinary::ReadAllBytes(sHtmlFile, &pData, nLength))
		return S_FALSE;

	std::string sContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
	bool bNeedConvert = true;
	if (nLength > 4)
	{
		if (pData[0] == 0xFF && pData[1] == 0xFE && !(pData[2] == 0x00 && pData[3] == 0x00))
			bNeedConvert = false;
		if (pData[0] == 0xFE && pData[1] == 0xFF)
			bNeedConvert = false;

		if (pData[0] == 0xFF && pData[1] == 0xFE && pData[2] == 0x00 && pData[3] == 0x00)
			bNeedConvert = false;
		if (pData[0] == 0 && pData[1] == 0 && pData[2] == 0xFE && pData[3] == 0xFF)
			bNeedConvert = false;
	}
	RELEASEARRAYOBJECTS(pData);

	//XmlUtils::CXmlLiteReader oIndexHtml;
	std::wstring xhtml = htmlToXhtml(sContent, bNeedConvert);

	if (!m_internal->m_oLightReader.FromString(xhtml))
		return S_FALSE;

	m_internal->m_oLightReader.ReadNextNode(); // html
	int nDepth = m_internal->m_oLightReader.GetDepth();
	m_internal->m_oLightReader.ReadNextSiblingNode(nDepth); // head
	m_internal->m_oLightReader.ReadNextSiblingNode(nDepth); // body

	//std::vector<std::wstring> arrBinary;
	NSStringUtils::CStringBuilder oDocument;
	ParagraphProperties arProperties;
	m_internal->readStream(oDocument, arProperties);

	NSStringUtils::CStringBuilder oRes;
	oRes.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><FictionBook xmlns=\"http://www.gribuser.ru/xml/fictionbook/2.0\" xmlns:l=\"http://www.w3.org/1999/xlink\">");
	// description title-info
	oRes.WriteString(L"<description><title-info>");
	if (m_internal->m_oTitleInfo.m_sBookTitle.empty())
		m_internal->m_oTitleInfo.m_sBookTitle = EncodeXmlString(sTitle);
	NSStringUtils::CStringBuilder oTitleInfo;
	m_internal->readTitleInfo(oTitleInfo);
	oRes.WriteString(oTitleInfo.GetData());
	oRes.WriteString(L"</title-info></description>");
	// body
	oRes.WriteString(L"<body><section>");
	oRes.WriteString(oDocument.GetData());
	oRes.WriteString(L"</section></body>");
	// notes
	if (!m_internal->m_mFootnotes.empty())
	{
		oRes.WriteString(L"<body name=\"notes\">");
		for (std::map<std::wstring, std::wstring>::iterator i = m_internal->m_mFootnotes.begin(); i != m_internal->m_mFootnotes.end(); i++)
		{
			oRes.WriteString(L"<section id=\"" + i->first + L"\">");
			oRes.WriteString(i->second);
			oRes.WriteString(L"</section>");
		}
		oRes.WriteString(L"</body>");
	}
	// binary
	for (std::map<std::wstring, std::vector<std::wstring>>::iterator i = m_internal->m_mImages.begin(); i != m_internal->m_mImages.end(); i++)
	{
		oRes.WriteString(L"<binary id=\"img" + i->first + L".png\" content-type=\"image/png\">");
		oRes.WriteString(i->second[0]);
		oRes.WriteString(L"</binary>");
	}
	oRes.WriteString(L"</FictionBook>");
	// Запись в файл
	NSFile::CFileBinary oWriter;
	if (oWriter.CreateFileW(sDst))
	{
		oWriter.WriteStringUTF8(oRes.GetData());
		oWriter.CloseFile();
		return S_OK;
	}
	return S_FALSE;

}
