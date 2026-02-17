#include "OOXMLTags.h"

#include "../src/StringFinder.h"
#include "../Table.h"

#include "../../Common/Network/FileTransporter/include/FileTransporter.h"

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/common/Base64.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/ProcessEnv.h"
#include "../../DesktopEditor/common/Path.h"

#include <boost/tuple/tuple.hpp>

namespace HTML
{
#define DEFAULT_PAGE_WIDTH  12240 // Значение в Twips
#define DEFAULT_PAGE_HEIGHT 15840 // Значение в Twips

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors);

inline bool NotValidExtension(const std::wstring& sExtention);
inline bool IsSVG(const std::wstring& wsExtention);
bool ReadSVG(const std::wstring& wsSvg, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath);
bool ReadBase64(const std::wstring& wsSrc, const std::wstring& wsImagePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsExtention);
bool GetStatusUsingExternalLocalFiles();
bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsSrcPath, const std::wstring& wsCorePath, bool bIsAllowExternalLocalFiles);

bool CopyImage(std::wstring wsImageSrc, const std::wstring& wsSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles);
bool UpdateImageData(const std::wstring& wsImagePath, TImageData& oImageData);

const static double HTML_FONTS[7] = {7.5, 10, 12, 13.5, 18, 24, 36};

CAnchor<COOXMLWriter>::CAnchor(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CAnchor<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	std::wstring wsRef, wsAlt, wsName;
	bool bCross = false;

	if (arSelectors.back().GetAttributeValue(L"href", wsRef) && wsRef.find('#') != std::wstring::npos)
		bCross = true;

	if (arSelectors.back().GetAttributeValue(L"name", wsName))
		m_pWriter->WriteEmptyBookmark(wsName);

	arSelectors.back().GetAttributeValue(L"alt", wsAlt);

	std::wstring wsFootnote;

	if (arSelectors.back().m_wsStyle.find(L"mso-footnote-id") != std::wstring::npos)
		wsFootnote = arSelectors.back().m_wsStyle.substr(arSelectors.back().m_wsStyle.rfind(L':') + 1);
	else
	{
		if (arSelectors.back().GetAttributeValue(L"epub:type", wsFootnote) && wsFootnote.find(L"noteref"))
			wsFootnote = L"href";
	}

	bool bFootnote = false;
	if (arSelectors.size() > 1)
	{
		const NSCSS::CNode& oNode = arSelectors[arSelectors.size() - 2];
		bFootnote = oNode.m_wsName == L"p" && oNode.m_wsClass == L"MsoFootnoteText";
	}

	if (bCross)
		m_pWriter->SetHyperlinkData(wsRef, L"", true, wsFootnote, bFootnote);
	else
	{
		std::wstring wsTooltip(wsRef);
		arSelectors.back().GetAttributeValue(L"title", wsTooltip);

		m_pWriter->SetHyperlinkData(wsRef, wsTooltip, false, wsFootnote, bFootnote);
	}

	return true;
}

void CAnchor<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->ClearHyperlinkData();
}

CAbbr<COOXMLWriter>::CAbbr(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CAbbr<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	std::wstring wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"title", wsTitle))
		return false;

	m_pWriter->WritePPr(arSelectors);

	XmlString* pCurrentDocument{m_pWriter->GetCurrentDocument()};

	pCurrentDocument->WriteString(L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r><w:r><w:instrText>HYPERLINK \\l \"" + m_pWriter->AddLiteBookmark() + L"\" \\o \"");
	pCurrentDocument->WriteEncodeXmlString(wsTitle);
	pCurrentDocument->WriteString(L"\"</w:instrText></w:r>");
	pCurrentDocument->WriteString(L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>");

	return true;
}

void CAbbr<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->GetCurrentDocument()->WriteString(L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>");
}

CBreak<COOXMLWriter>::CBreak(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CBreak<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->Break(arSelectors);

	return true;
}

void CBreak<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CDivision<COOXMLWriter>::CDivision(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CDivision<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->UpdatePageStyle(arSelectors);

	UINT unMsoFootnote = 0;

	if (L"footnote" == arSelectors.back().GetAttributeValue(L"epub:type"))
		++unMsoFootnote;

	if (std::wstring::npos != arSelectors.back().m_wsStyle.find(L"mso-element:footnote"))
		++unMsoFootnote;

	std::wstring wsFootnoteID;

	if (!arSelectors.back().m_wsId.empty())
	{
		wsFootnoteID = m_pWriter->FindFootnote(arSelectors.back().m_wsId);

		if (!wsFootnoteID.empty())
			++unMsoFootnote;

		if (unMsoFootnote >= 2 && !wsFootnoteID.empty())
		{
			m_pWriter->OpenFootnote(wsFootnoteID);
			m_pWriter->SetCurrentDocument(&m_pWriter->GetNotesXml());
		}
	}

	m_arFootnoteIDs.push(unMsoFootnote);

	return true;
}

void CDivision<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter() || m_arFootnoteIDs.empty())
		return;

	if (m_arFootnoteIDs.top() >= 2)
	{
		m_pWriter->CloseFootnote();
		m_pWriter->RollBackState();
	}

	m_pWriter->CloseP();
	m_arFootnoteIDs.pop();
}

CImage<COOXMLWriter>::CImage(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CImage<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	if (L"svg" == arSelectors.back().m_wsName)
	{
		if (oExtraData.empty() || typeid(const std::wstring&) != oExtraData.type())
			return false;

		const std::wstring wsImagePath{m_pWriter->GetMediaDir() + L'i' + std::to_wstring(m_arrImages.size()) + L".png"};

		if (!ReadSVG(boost::any_cast<const std::wstring&>(oExtraData), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsImagePath))
			return false;

		TImageData oNewImageData;
		if (!UpdateImageData(wsImagePath, oNewImageData))
			return false;

		m_pWriter->WritePPr(arSelectors);
		m_pWriter->WriteImage(oNewImageData, std::to_wstring(m_arrImages.size()));
		return true;
	}

	const std::wstring wsAlt{arSelectors.back().GetAttributeValue(L"alt")};
	std::wstring wsSrc{arSelectors.back().GetAttributeValue(L"src")};

	TImageData oImageData;

	std::wstring wsTempValue;
	NSCSS::NSProperties::CDigit oTempDigit;

	#define GET_UNIT_DATA(variable_data, data_variable)\
	if (NSCSS::UnitMeasure::None == data_variable.GetUnitMeasure())\
		variable_data = static_cast<UINT>(NSCSS::CUnitMeasureConverter::ConvertPx(data_variable.ToDouble(), NSCSS::Inch, 96) * 914400);\
	else\
		variable_data = static_cast<UINT>(data_variable.ToDouble(NSCSS::Inch) * 914400)

	#define READ_IMAGE_DATA(name_data, variable_data)\
	if (arSelectors.back().GetAttributeValue(name_data, wsTempValue))\
	{\
		if (oTempDigit.SetValue(wsTempValue))\
		{\
			GET_UNIT_DATA(variable_data, oTempDigit);\
		}\
	}

	READ_IMAGE_DATA(L"width", oImageData.m_unWidth);
	READ_IMAGE_DATA(L"height", oImageData.m_unHeight);
	READ_IMAGE_DATA(L"hspace", oImageData.m_nHSpace);
	READ_IMAGE_DATA(L"vspace", oImageData.m_nVSpace);

	if (nullptr != arSelectors.back().m_pCompiledStyle)
	{
		oTempDigit = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetWidth();

		if (0 == oImageData.m_unWidth && !oTempDigit.Empty())
		{
			GET_UNIT_DATA(oImageData.m_unWidth, oTempDigit);
		}

		oTempDigit = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHeight();

		if (0 == oImageData.m_unHeight && !oTempDigit.Empty())
		{
			GET_UNIT_DATA(oImageData.m_unHeight, oTempDigit);
		}
	}

	if (wsSrc.empty())
	{
		m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
		return true;
	}

	bool bRes = false;
	std::wstring wsExtention;
	const std::wstring wsImagePath{m_pWriter->GetMediaDir() + L'i' + std::to_wstring(m_arrImages.size())};

	// Предполагаем картинку в Base64
	if (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"data" && wsSrc.find(L"/", 4) != std::wstring::npos)
		bRes = ReadBase64(wsSrc, wsImagePath, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsExtention);

	const bool bIsAllowExternalLocalFiles = GetStatusUsingExternalLocalFiles();

	if (!bRes && (wsSrc.length() <= 7 || L"http" != wsSrc.substr(0, 4)))
	{
		wsSrc = NSSystemPath::ShortenPath(wsSrc);

		if (!CanUseThisPath(wsSrc, m_pWriter->GetSrcPath(), m_pWriter->GetCorePath(), bIsAllowExternalLocalFiles))
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}
	}

	// Проверка расширения
	if (!bRes)
	{
		wsExtention = NSFile::GetFileExtention(wsSrc);
		std::transform(wsExtention.begin(), wsExtention.end(), wsExtention.begin(), tolower);

		std::wstring::const_iterator itFound = std::find_if(wsExtention.cbegin(), wsExtention.cend(), [](wchar_t wChar){ return !iswalpha(wChar) && L'+' != wChar; });

		if (wsExtention.cend() != itFound)
			wsExtention.erase(itFound, wsExtention.cend());
	}

	const std::wstring wsBasePath{m_pWriter->GetBasePath()};

	// Предполагаем картинку в сети
	if (!bRes &&
	    ((!wsBasePath.empty() && wsBasePath.length() > 4 && wsBasePath.substr(0, 4) == L"http") ||
	      (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"http")))
	{
		if (!wsExtention.empty() && NotValidExtension(wsExtention))
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		const std::wstring wsDst = wsImagePath + L'.' + ((!wsExtention.empty()) ? wsExtention : L"png");

		// Проверка gc_allowNetworkRequest предполагается в kernel_network
		NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(m_pWriter->GetBasePath() + wsSrc, false);
		oDownloadImg.SetFilePath(wsDst);
		bRes = oDownloadImg.DownloadSync();

		if (!bRes)
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		if (IsSVG(wsExtention))
		{
			std::wstring wsFileData;

			if (!NSFile::CFileBinary::ReadAllTextUtf8(wsDst, wsFileData) ||
			    !ReadSVG(wsFileData, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsImagePath))
				bRes = false;

			NSFile::CFileBinary::Remove(wsDst);
			wsExtention = L"png";
		}
		else if (wsExtention.empty())
		{
			//TODO:: лучше узнавать формат изображения из содержимого
			wsExtention = L"png";
		}
	}

	int nImageId = -1;

	if (!bRes)
	{
		if (NotValidExtension(wsExtention))
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		// Проверка на повтор
		const std::vector<std::wstring>::const_iterator nFind = std::find(m_arrImages.cbegin(), m_arrImages.cend(), wsSrc);
		if (nFind != m_arrImages.end())
		{
			bRes = true;
			nImageId = nFind - m_arrImages.cbegin();
		}
	}

	// Предполагаем картинку по локальному пути
	if (!bRes)
	{
		const std::wstring wsDst = wsImagePath + L'.' + wsExtention;

		if (!m_pWriter->GetBasePath().empty())
		{
			if (!bRes)
				bRes = CopyImage(NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
			if (!bRes)
				bRes = CopyImage(NSSystemPath::Combine(m_pWriter->GetSrcPath(), NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc)), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		}
		if (!bRes)
			bRes = CopyImage(NSSystemPath::Combine(m_pWriter->GetSrcPath(), wsSrc), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		if (!bRes)
			bRes = CopyImage(m_pWriter->GetSrcPath() + L"/" + NSFile::GetFileName(wsSrc), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		if (!bRes)
			bRes = CopyImage(wsSrc, m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
	}

	if (!bRes)
		m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
	else
	{
		m_pWriter->WritePPr(arSelectors);

		const std::wstring wsImageID{std::to_wstring(m_arrImages.size())};
		m_arrImages.push_back(wsSrc);

		if (nImageId < 0)
		{
			m_pWriter->WriteImageRels(wsImageID, wsImageID + L'.' + wsExtention);
			m_arrImages.push_back(wsSrc);
		}

		if (!oImageData.ZeroSize())
		{
			m_pWriter->WriteImage(oImageData, wsImageID);
			return true;
		}

		TImageData oNewImageData{oImageData};
		if (!UpdateImageData(wsImagePath + L'.' + wsExtention, oNewImageData))
			return false;

		m_pWriter->WriteImage(oNewImageData, wsImageID);
	}

	return true;
}

void CImage<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CFont<COOXMLWriter>::CFont(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CFont<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	std::wstring wsValue;

	if (arSelectors.back().GetAttributeValue(L"color", wsValue))
		arSelectors.back().m_pCompiledStyle->m_oText.SetColor(wsValue, arSelectors.size());

	if (arSelectors.back().GetAttributeValue(L"face", wsValue))
		arSelectors.back().m_pCompiledStyle->m_oFont.SetFamily(wsValue, arSelectors.size());

	if (arSelectors.back().GetAttributeValue(L"size", wsValue))
	{
		int nSize = 3;
		if(!wsValue.empty())
		{
			if(wsValue.front() == L'+')
				nSize += NSStringFinder::ToInt(wsValue.substr(1));
			else if(wsValue.front() == L'-')
				nSize -= NSStringFinder::ToInt(wsValue.substr(1));
			else
				nSize = NSStringFinder::ToInt(wsValue);
		}

		if (nSize < 1 || nSize > 7)
			nSize = 3;

		arSelectors.back().m_pCompiledStyle->m_oFont.SetSize(HTML_FONTS[nSize - 1], NSCSS::UnitMeasure::Point,  arSelectors.size());
	}

	return true;
}

void CFont<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CInput<COOXMLWriter>::CInput(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CInput<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	std::wstring wsValue{arSelectors.back().GetAttributeValue(L"value")};
	const std::wstring wsAlt{arSelectors.back().GetAttributeValue(L"alt")},
	                   wsType{arSelectors.back().GetAttributeValue(L"type")};

	if(wsType == L"hidden")
		return false;

	if(wsValue.empty())
		wsValue = wsAlt;

	if(!wsValue.empty())
	{
		m_pWriter->WritePPr(arSelectors);
		m_pWriter->OpenR();
		m_pWriter->WriteRPr(*(m_pWriter->GetCurrentDocument()), arSelectors);
		m_pWriter->OpenT();
		m_pWriter->GetCurrentDocument()->WriteEncodeXmlString(wsValue + L' ');
		m_pWriter->CloseT();
		m_pWriter->CloseR();
	}

	return true;
}

void CInput<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CBaseFont<COOXMLWriter>::CBaseFont(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CBaseFont<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	std::wstring wsFontStyles, wsValue;

	if (arSelectors.back().GetAttributeValue(L"face", wsValue))
		wsFontStyles += L"font-family:" + wsValue + L';';

	if (arSelectors.back().GetAttributeValue(L"size", wsValue))
	{
		wsFontStyles += L"font-size:";

		switch(NSStringFinder::ToInt(wsValue, 3))
		{
			case 1: wsFontStyles += L"7.5pt;";  break;
			case 2: wsFontStyles += L"10pt;";   break;
			default:
			case 3: wsFontStyles += L"12pt;";   break;
			case 4: wsFontStyles += L"13.5pt;"; break;
			case 5: wsFontStyles += L"18pt;";   break;
			case 6: wsFontStyles += L"24pt;";   break;
			case 7: wsFontStyles += L"36pt;";   break;
		}
	}

	if (arSelectors.back().GetAttributeValue(L"color", wsValue))
		wsFontStyles += L"text-color:" + wsValue + L';';

	if (wsFontStyles.empty())
		return false;

	m_pWriter->SetBaseFont(L"*{" + wsFontStyles + L'}');

	return true;
}

void CBaseFont<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CBlockquote<COOXMLWriter>::CBlockquote(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CBlockquote<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	//TODO:: когда Blockquote в Blockquote, то к первому нужно добавлять <w:divsChild>

	const std::wstring wsKeyWord{arSelectors.back().m_wsName};

	std::map<std::wstring, UINT>::const_iterator itFound = m_mDivs.find(wsKeyWord);

	if (m_mDivs.end() != itFound)
	{
		m_pWriter->SetDivId(std::to_wstring(itFound->second));
		return true;
	}

	const std::wstring wsId{std::to_wstring(m_mDivs.size() + 1)};

	XmlString &oWebSettings{m_pWriter->GetWebSettingsXml()};

	if (m_mDivs.empty())
		oWebSettings.WriteString(L"<w:divs>");

	NSCSS::CCompiledStyle *pStyle = arSelectors.back().m_pCompiledStyle;

	const bool bInTable = ElementInTable(arSelectors);

	INT nMarLeft   = (!bInTable) ? 720 : 0;
	INT nMarRight  = (!bInTable) ? 720 : 0;
	INT nMarTop    = (!bInTable) ? 100 : 0;
	INT nMarBottom = (!bInTable) ? 100 : 0;

	const NSCSS::NSProperties::CPage *pPageData{m_pWriter->GetPageData()};

	if (!pStyle->m_oMargin.GetLeft().Empty() && !pStyle->m_oMargin.GetLeft().Zero())
		nMarLeft  = pStyle->m_oMargin.GetLeft().ToInt(NSCSS::Twips, pPageData->GetWidth().ToInt(NSCSS::Twips));

	if (!pStyle->m_oMargin.GetRight().Empty() && !pStyle->m_oMargin.GetRight().Zero())
		nMarRight = pStyle->m_oMargin.GetRight().ToInt(NSCSS::Twips, pPageData->GetWidth().ToInt(NSCSS::Twips));

	if (!pStyle->m_oMargin.GetTop().Empty() && !pStyle->m_oMargin.GetTop().Zero())
		nMarTop = pStyle->m_oMargin.GetTop().ToInt(NSCSS::Twips, pPageData->GetHeight().ToInt(NSCSS::Twips));

	if (!pStyle->m_oMargin.GetBottom().Empty() && !pStyle->m_oMargin.GetBottom().Zero())
		nMarBottom = pStyle->m_oMargin.GetBottom().ToInt(NSCSS::Twips, pPageData->GetHeight().ToInt(NSCSS::Twips));

	if (L"blockquote" == wsKeyWord)
	{
		oWebSettings.WriteString(L"<w:div w:id=\"" + wsId + L"\">");
		oWebSettings.WriteString(L"<w:blockQuote w:val=\"1\"/>");
		oWebSettings.WriteString(L"<w:marLeft w:val=\"" + std::to_wstring(nMarLeft) + L"\"/>");
		oWebSettings.WriteString(L"<w:marRight w:val=\"" + std::to_wstring(nMarRight) + L"\"/>");
		oWebSettings.WriteString(L"<w:marTop w:val=\"" + std::to_wstring(nMarTop) + L"\"/>");
		oWebSettings.WriteString(L"<w:marBottom w:val=\"" + std::to_wstring(nMarBottom) + L"\"/>");
		oWebSettings.WriteString(L"<w:divBdr>");
		oWebSettings.WriteString(L"<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"</w:divBdr>");
		oWebSettings.WriteString(L"</w:div>");
	}

	m_mDivs.insert(std::make_pair(wsKeyWord, m_mDivs.size() + 1));

	m_pWriter->SetDivId(wsId);

	return true;
}

void CBlockquote<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->RollBackDivId();
}

CHorizontalRule<COOXMLWriter>::CHorizontalRule(COOXMLWriter* pWriter)
	: CTag(pWriter), m_unShapeId(1)
{}

bool CHorizontalRule<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	for (const NSCSS::CNode& item : arSelectors)
	{
		if (item.m_wsName == L"div" && item.m_wsStyle == L"mso-element:footnote-list")
			return false;
	}

	NSCSS::NSProperties::CDigit oSize, oWidth;
	NSCSS::NSProperties::CColor oColor;
	bool bShade = true;
	std::wstring wsAlign{L"center"};
	std::wstring wsValue;

	if (arSelectors.back().GetAttributeValue(L"align", wsValue))
	{
		if (NSStringFinder::Equals(L"left", wsValue))
			wsAlign = L"left";
		else if (NSStringFinder::Equals(L"right", wsValue))
			wsAlign = L"right";
		else if (NSStringFinder::Equals(L"center", wsValue))
			wsAlign = L"center";
	}

	if (arSelectors.back().GetAttributeValue(L"color", wsValue))
		oColor.SetValue(wsValue);

	if (arSelectors.back().GetAttributeValue(L"noshade", wsValue))
		bShade = false;

	if (arSelectors.back().GetAttributeValue(L"size", wsValue))
		oSize.SetValue(wsValue);

	if (arSelectors.back().GetAttributeValue(L"width", wsValue))
		oWidth.SetValue(wsValue);

	XmlString& oCurrentDocument{*m_pWriter->GetCurrentDocument()};

	m_pWriter->OpenP();
	oCurrentDocument.WriteString(L"<w:pPr><w:jc w:val=\"" + wsAlign + L"\"/></w:pPr>");
	m_pWriter->OpenR();

	const NSCSS::NSProperties::CPage *pPageData{m_pWriter->GetPageData()};

	const unsigned int unPageWidth{static_cast<unsigned int>((pPageData->GetWidth().ToDouble(NSCSS::Inch) - pPageData->GetMargin().GetLeft().ToDouble(NSCSS::Inch) - pPageData->GetMargin().GetRight().ToDouble(NSCSS::Inch)) * 914400.)};

	std::wstring wsWidth;

	// width измеряется в px или %
	if (!oWidth.Empty())
		wsWidth = std::to_wstring(static_cast<int>((NSCSS::UnitMeasure::Percent != oWidth.GetUnitMeasure()) ? (NSCSS::CUnitMeasureConverter::ConvertPx(oWidth.ToDouble(), NSCSS::Inch, 96) * 914400.) : oWidth.ToDouble(NSCSS::Inch, unPageWidth)));
	else
		wsWidth = std::to_wstring(unPageWidth);

	std::wstring wsHeight{L"14288"};

	// size измеряется только в px
	if (!oSize.Empty())
		wsHeight = std::to_wstring(static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(oSize.ToDouble(), NSCSS::Inch, 96) * 914400.));

	oCurrentDocument.WriteString(L"<w:rPr><w:noProof/></w:rPr>");
	oCurrentDocument.WriteString(L"<mc:AlternateContent><mc:Choice Requires=\"wps\"><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">");
	oCurrentDocument.WriteString(L"<wp:extent cx=\"" + wsWidth + L"\" cy=\"0\"/>");
	oCurrentDocument.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	oCurrentDocument.WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_unShapeId) + L"\" name=\"Line " + std::to_wstring(m_unShapeId) + L"\"/>"
	                                  "<wp:cNvGraphicFramePr/>"
	                                      "<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">"
	                                            "<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"
	                                                "<wps:wsp>"
	                                                    "<wps:cNvSpPr/>"
	                                                    "<wps:spPr>");
	oCurrentDocument.WriteString(L"<a:xfrm>"
	                                  "<a:off x=\"0\" y=\"0\"/>"
	                                  "<a:ext cx=\"" + wsWidth + L"\" cy=\"0\"/>"
	                              "</a:xfrm>"
	                              "<a:custGeom><a:pathLst><a:path>"
	                                  "<a:moveTo><a:pt x=\"0\" y=\"0\"/></a:moveTo>"
	                                  "<a:lnTo><a:pt x=\"" + wsWidth + L"\" y=\"0\"/></a:lnTo>"
	                              "</a:path></a:pathLst></a:custGeom>"
	                              "<a:ln w=\"" + wsHeight + L"\"><a:solidFill><a:srgbClr val=\"" + ((!oColor.Empty()) ? oColor.ToHEX() : L"808080") + L"\"/></a:solidFill></a:ln>");

	if (bShade)
		oCurrentDocument.WriteString(L"<a:scene3d><a:camera prst=\"orthographicFront\"/><a:lightRig rig=\"threePt\" dir=\"t\"/></a:scene3d><a:sp3d><a:bevelT prst=\"angle\"/></a:sp3d>");

	oCurrentDocument.WriteString(L"</wps:spPr><wps:bodyPr/></wps:wsp></a:graphicData></a:graphic></wp:inline></w:drawing></mc:Choice></mc:AlternateContent>");

	m_pWriter->CloseP();

	++m_unShapeId;

	return true;
}

void CHorizontalRule<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CList<COOXMLWriter>::CList(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CList<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->CloseP();

	//Нумерованный список
	if (L"ol" == arSelectors.back().m_wsName)
	{
		const int nStart{NSStringFinder::ToInt(arSelectors.back().GetAttributeValue(L"start"), 1)};

		XmlString& oNumberXml{m_pWriter->GetNumberingXml()};

		const std::wstring wsStart(std::to_wstring(nStart));
		oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
		oNumberXml.WriteString(std::to_wstring(m_pWriter->GetListId()));
		oNumberXml.WriteString(L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%1.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"709\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%2.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1429\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%3.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"2149\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%4.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2869\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%5.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3589\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%6.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"4309\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%7.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5029\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%8.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5749\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%9.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"6469\" w:hanging=\"180\"/></w:pPr></w:lvl></w:abstractNum>");

		m_pWriter->IncreaseListId();
	}

	return true;
}

void CList<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->CloseP();
}

CListElement<COOXMLWriter>::CListElement(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CListElement<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	return ValidWriter();
}

void CListElement<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->CloseP();
}

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors)
{
	return arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const NSCSS::CNode& oNode) { return L"table" == oNode.m_wsName; });
}

inline bool NotValidExtension(const std::wstring& sExtention)
{
	return  sExtention != L"bmp" && sExtention != L"emf"  && sExtention != L"emz"  && sExtention != L"eps"  && sExtention != L"fpx" && sExtention != L"gif"  &&
			sExtention != L"jpe" && sExtention != L"jpeg" && sExtention != L"jpg"  && sExtention != L"jfif" && sExtention != L"pct" && sExtention != L"pict" &&
			sExtention != L"png" && sExtention != L"pntg" && sExtention != L"psd"  && sExtention != L"qtif" && sExtention != L"sgi" && sExtention != L"wmz"  &&
			sExtention != L"tga" && sExtention != L"tpic" && sExtention != L"tiff" && sExtention != L"tif"  && sExtention != L"wmf" && !IsSVG(sExtention);
}

inline bool IsSVG(const std::wstring& wsExtention)
{
	return L"svg" == wsExtention || L"svg+xml" == wsExtention;
}

bool ReadSVG(const std::wstring& wsSvg, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath)
{
	MetaFile::IMetaFile* pSvgReader = MetaFile::Create(pFonts);
	if (!pSvgReader->LoadFromString(wsSvg))
	{
		RELEASEINTERFACE(pSvgReader);
		return false;
	}

	NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
	pGrRenderer->SetFontManager(pSvgReader->get_FontManager());

	double dX, dY, dW, dH;
	pSvgReader->GetBounds(&dX, &dY, &dW, &dH);

	if (dW < 0) dW = -dW;
	if (dH < 0) dH = -dH;

	double dOneMaxSize = (double)1000.;

	if (dW > dH && dW > dOneMaxSize)
	{
		dH *= (dOneMaxSize / dW);
		dW = dOneMaxSize;
	}
	else if (dH > dW && dH > dOneMaxSize)
	{
		dW *= (dOneMaxSize / dH);
		dH = dOneMaxSize;
	}

	int nWidth  = static_cast<int>(dW + 0.5);
	int nHeight = static_cast<int>(dH + 0.5);

	double dWidth  = 25.4 * nWidth / 96;
	double dHeight = 25.4 * nHeight / 96;

	BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
	if (!pBgraData)
	{
		double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

		nWidth = (int)(dKoef * nWidth);
		nHeight = (int)(dKoef * nHeight);

		dWidth  = 25.4 * nWidth / 96;
		dHeight = 25.4 * nHeight / 96;

		pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
	}

	if (!pBgraData)
		return false;

	unsigned int alfa = 0xffffff;
	//дефолтный тон должен быть прозрачным, а не белым
	//memset(pBgraData, 0xff, nWidth * nHeight * 4);
	for (int i = 0; i < nWidth * nHeight; i++)
	{
		((unsigned int*)pBgraData)[i] = alfa;
	}
	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(-4 * nWidth);

	pGrRenderer->CreateFromBgraFrame(&oFrame);
	pGrRenderer->SetSwapRGB(false);
	pGrRenderer->put_Width(dWidth);
	pGrRenderer->put_Height(dHeight);

	pSvgReader->SetTempDirectory(wsTempDir);
	pSvgReader->DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

	oFrame.SaveFile(wsImagePath + L".png", 4);
	oFrame.put_Data(NULL);

	RELEASEINTERFACE(pGrRenderer);

	if (pBgraData)
		free(pBgraData);

	RELEASEINTERFACE(pSvgReader);

	return true;
}

bool ReadBase64(const std::wstring& wsSrc, const std::wstring& wsImagePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsExtention)
{
	bool bRes = false;
	size_t nBase = wsSrc.find(L"/", 4);
	nBase++;

	size_t nEndBase = wsSrc.find(L";", nBase);
	if (nEndBase == std::wstring::npos)
		return bRes;

	wsExtention = wsSrc.substr(nBase, nEndBase - nBase);

	if (wsExtention == L"octet-stream")
		wsExtention = L"jpg";

	if (NotValidExtension(wsExtention))
		return bRes;

	nBase = wsSrc.find(L"base64", nEndBase);
	if (nBase == std::wstring::npos)
		return bRes;

	int nOffset = nBase + 7;
	int nSrcLen = (int)(wsSrc.length() - nBase + 1);
	int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
	if (nDecodeLen != 0)
	{
		BYTE* pImageData = new BYTE[nDecodeLen];

		if (!pImageData || FALSE == NSBase64::Base64Decode(wsSrc.c_str() + nOffset, nSrcLen, pImageData, &nDecodeLen))
			return bRes;

		if (IsSVG(wsExtention))
		{
			std::wstring wsSvg(pImageData, pImageData + nDecodeLen);
			bRes = ReadSVG(wsSvg, pFonts, wsTempDir, wsImagePath);
			wsExtention = L"png";
		}
		else
		{
			NSFile::CFileBinary oImageWriter;

			if (oImageWriter.CreateFileW(wsImagePath + L'.' + wsExtention))
				bRes = oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);

			oImageWriter.CloseFile();
		}

		RELEASEARRAYOBJECTS(pImageData);
	}

	return bRes;
}

bool GetStatusUsingExternalLocalFiles()
{
	if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
		return NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

	return true;
}

bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsSrcPath, const std::wstring& wsCorePath, bool bIsAllowExternalLocalFiles)
{
	if (bIsAllowExternalLocalFiles)
		return true;

	if (!wsCorePath.empty())
	{
		const std::wstring wsFullPath = NSSystemPath::ShortenPath(NSSystemPath::Combine(wsSrcPath, wsPath));
		return boost::starts_with(wsFullPath, wsCorePath);
	}

	if (wsPath.length() >= 3 && L"../" == wsPath.substr(0, 3))
		return false;

	return true;
}

bool CopyImage(std::wstring wsImageSrc, const std::wstring& wsSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles)
{
	bool bRes = false;
	bool bAllow = true;

	if (!bIsAllowExternalLocalFiles)
	{
		wsImageSrc = NSSystemPath::NormalizePath(wsImageSrc);
		const std::wstring wsStartSrc = NSSystemPath::NormalizePath(wsSrc);
		bAllow = wsImageSrc.substr(0, wsStartSrc.length()) == wsStartSrc;
	}
	if (bAllow)
		bRes = NSFile::CFileBinary::Copy(wsSrc, wsDst);

	return bRes;
}

bool UpdateImageData(const std::wstring& wsImagePath, TImageData& oImageData)
{
	CBgraFrame oBgraFrame;
	if (!oBgraFrame.OpenFile(wsImagePath))
	{
		NSFile::CFileBinary::Remove(wsImagePath);
		return false;
	}

	if (0 != oImageData.m_unWidth || 0 != oImageData.m_unHeight)
	{
		const double dMaxScale = std::max(oImageData.m_unWidth  / oBgraFrame.get_Width(),
		                                  oImageData.m_unHeight / oBgraFrame.get_Height());

		oImageData.m_unWidth  = oBgraFrame.get_Width()  * dMaxScale;
		oImageData.m_unHeight = oBgraFrame.get_Height() * dMaxScale;
	}
	else
	{
		oImageData.m_unWidth  = oBgraFrame.get_Width();
		oImageData.m_unHeight = oBgraFrame.get_Height();

		if (oImageData.m_unWidth > oImageData.m_unHeight)
		{
			int nW = oImageData.m_unWidth * 9525;
			nW = (nW > 7000000 ? 7000000 : nW);
			oImageData.m_unHeight = (int)((double)oImageData.m_unHeight * (double)nW / (double)oImageData.m_unWidth);
			oImageData.m_unWidth = nW;
		}
		else
		{
			int nH = oImageData.m_unHeight * 9525;
			nH = (nH > 8000000 ? 8000000 : nH);
			int nW = (int)((double)oImageData.m_unWidth * (double)nH / (double)oImageData.m_unHeight);
			if (nW > 7000000)
			{
				nW = 7000000;
				oImageData.m_unHeight = (int)((double)oImageData.m_unHeight * (double)nW / (double)oImageData.m_unWidth);
			}
			else
				oImageData.m_unHeight = nH;
			oImageData.m_unWidth = nW;
		}
	}

	return true;
}

CCaption<COOXMLWriter>::CCaption(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CCaption<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WritePPr(arSelectors);

	return true;
}

void CCaption<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->CloseP();
}

std::wstring CreateBorders(const NSCSS::NSProperties::CBorder& oBorder, const NSCSS::NSProperties::CIndent* pPadding = NULL, bool bAddIntermediateLines = false, TTableStyles::ETableRules enTableRule = TTableStyles::ETableRules::None)
{
	std::wstring wsTable;

	if (oBorder.EqualSides() && (NULL == pPadding || pPadding->Equals()))
	{
		const std::wstring wsBorderStyle = NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))));

		wsTable = L"<w:top "    + wsBorderStyle + L"/>" + L"<w:left "   + wsBorderStyle + L"/>" +
		          L"<w:bottom " + wsBorderStyle + L"/>" + L"<w:right "  + wsBorderStyle + L"/>";
	}
	else
	{
		if (oBorder.GetTopBorder().Valid())
			wsTable += L"<w:top "    + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetTopBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetTop()))))       + L"/>";

		if (oBorder.GetLeftBorder().Valid())
			wsTable += L"<w:left "   + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))))     + L"/>";

		if (oBorder.GetBottomBorder().Valid())
			wsTable += L"<w:bottom " + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetBottomBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetBottom())))) + L"/>";

		if (oBorder.GetRightBorder().Valid())
			wsTable += L"<w:right "  + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetRightBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetRight()))))   + L"/>";
	}

	if (!bAddIntermediateLines)
		return wsTable;

	if (TTableStyles::ETableRules::Rows == enTableRule || TTableStyles::ETableRules::All == enTableRule)
	{
		NSCSS::NSProperties::CBorderSide oNewSide(oBorder.GetBottomBorder());
		oNewSide.SetWidth(L"1pt", 0, true);

		wsTable += L"<w:insideH " + NSCSS::CDocumentStyle::CalculateBorderStyle(oNewSide) + L"/>";
	}

	if (TTableStyles::ETableRules::Cols == enTableRule || TTableStyles::ETableRules::All == enTableRule)
	{
		NSCSS::NSProperties::CBorderSide oNewSide(oBorder.GetRightBorder());
		oNewSide.SetWidth(L"1pt", 0, true);

		wsTable += L"<w:insideV " + NSCSS::CDocumentStyle::CalculateBorderStyle(oNewSide) + L"/>";
	}

	return wsTable;
}

std::wstring CreateBorders(const std::wstring& wsStyle, UINT unSize, UINT unSpace, const std::wstring& wsAuto)
{
	const std::wstring wsBodyBorder{L"w:val=\"" + wsStyle + L"\" w:sz=\"" + std::to_wstring(unSize) + L"\" w:space=\"" + std::to_wstring(unSpace) + L"\" w:color=\"" + wsAuto + L"\""};

	return L"<w:top "    + wsBodyBorder + L"/>" +
	       L"<w:left "   + wsBodyBorder + L"/>" +
	       L"<w:bottom " + wsBodyBorder + L"/>" +
	       L"<w:right "  + wsBodyBorder + L"/>";
}

#define CreateOutsetBorders(enType) CreateBorders(L"outset", 6, 0, L"auto", enType)

std::wstring CreateDefaultBorder(std::wstring wsSideName)
{
	std::transform(wsSideName.begin(), wsSideName.end(), wsSideName.begin(), std::towlower);
	return L"<w:" + wsSideName + L" w:val=\"single\" w:sz=\"1\" w:space=\"0\" w:color=\"auto\"/>";
}

CTable<COOXMLWriter>::CTable(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CTable<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter() || oExtraData.empty() || typeid(CStorageTable*) != oExtraData.type())
		return false;

	CStorageTable* pStorageTable{boost::any_cast<CStorageTable*>(oExtraData)};

	if (pStorageTable->Empty())
		return false;

	XmlString& oCurrentDocument{*m_pWriter->GetCurrentDocument()};

	oCurrentDocument.WriteNodeBegin(L"w:tbl");
	oCurrentDocument.WriteNodeBegin(L"w:tblPr");

	const TTableStyles& oTableStyles{pStorageTable->GetTableStyles()};

	if (!oTableStyles.m_oWidth.Empty() && !oTableStyles.m_oWidth.Zero())
	{
		if (NSCSS::UnitMeasure::Percent == oTableStyles.m_oWidth.GetUnitMeasure())
			oCurrentDocument += L"<w:tblW w:w=\"" + std::to_wstring(oTableStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
		else
			oCurrentDocument += L"<w:tblW w:w=\"" + std::to_wstring(oTableStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
	}
	else
		oCurrentDocument += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

	if (!oTableStyles.m_oMargin.GetLeft().Empty() && !oTableStyles.m_oMargin.GetLeft().Zero())
	{
		if (NSCSS::UnitMeasure::Percent == oTableStyles.m_oMargin.GetLeft().GetUnitMeasure())
			oCurrentDocument += L"<w:tblInd w:w=\"" + std::to_wstring(oTableStyles.m_oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
		else
			oCurrentDocument += L"<w:tblInd w:w=\"" + std::to_wstring(oTableStyles.m_oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
	}

	if (!oTableStyles.m_wsAlign.empty())
		oCurrentDocument += L"<w:jc w:val=\"" + oTableStyles.m_wsAlign + L"\"/>";

	if (0 < oTableStyles.m_nCellSpacing && oTableStyles.m_oBorder.GetCollapse() != NSCSS::NSProperties::BorderCollapse::Collapse)
		oCurrentDocument += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(oTableStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";

	if (!oTableStyles.m_oBorder.Empty() && !oTableStyles.m_oBorder.Zero())
		oCurrentDocument += L"<w:tblBorders>" + CreateBorders(oTableStyles.m_oBorder, NULL, true, (TTableStyles::ETableRules::Groups == oTableStyles.m_enRules && !pStorageTable->GetColgroups().empty()) ? TTableStyles::ETableRules::Cols : oTableStyles.m_enRules) + L"</w:tblBorders>";

	if (!oTableStyles.m_oPadding.Empty() && !oTableStyles.m_oPadding.Zero())
	{
		const int nTopPadding    = std::max(0, oTableStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nLeftPadding   = std::max(0, oTableStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
		const int nBottomPadding = std::max(0, oTableStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nRightPadding  = std::max(0, oTableStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));

		oCurrentDocument.WriteNodeBegin(L"w:tblCellMar");

		if (0 != nTopPadding)
			oCurrentDocument += L"<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>";

		if (0 != nLeftPadding)
			oCurrentDocument += L"<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>";

		if (0 != nBottomPadding)
			oCurrentDocument += L"<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>";

		if (0 != nRightPadding)
			oCurrentDocument += L"<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>";

		oCurrentDocument.WriteNodeEnd(L"w:tblCellMar");
	}
	else
		oCurrentDocument += L"<w:tblCellMar><w:top w:w=\"15\" w:type=\"dxa\"/><w:left w:w=\"15\" w:type=\"dxa\"/><w:bottom w:w=\"15\" w:type=\"dxa\"/><w:right w:w=\"15\" w:type=\"dxa\"/></w:tblCellMar>";

	oCurrentDocument += L"<w:tblLook w:val=\"04A0\" w:noVBand=\"1\" w:noHBand=\"0\" w:lastColumn=\"0\" w:firstColumn=\"1\" w:lastRow=\"0\" w:firstRow=\"1\"/>";
	oCurrentDocument.WriteNodeEnd(L"w:tblPr");

	if (pStorageTable->HaveCaption())
	{
		oCurrentDocument.WriteNodeBegin(L"w:tr");
		oCurrentDocument.WriteNodeBegin(L"w:tc");
		oCurrentDocument.WriteNodeBegin(L"w:tcPr");
		oCurrentDocument += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";
		oCurrentDocument += L"<w:gridSpan w:val=\"" + std::to_wstring(pStorageTable->GetMaxColumns()) + L"\"/>";
		oCurrentDocument += L"<w:tcBorders><w:top w:val=\"nil\"/><w:left w:val=\"nil\"/><w:bottom w:val=\"nil\"/><w:right w:val=\"nil\"/></w:tcBorders>";
		oCurrentDocument += L"<w:vAlign w:val=\"center\"/>";
		oCurrentDocument += L"<w:hideMark/>";
		oCurrentDocument.WriteNodeEnd(L"w:tcPr");
		WriteToStringBuilder(*(pStorageTable->GetCaptionData()), oCurrentDocument);
		oCurrentDocument.WriteNodeEnd(L"w:tc");
		oCurrentDocument.WriteNodeEnd(L"w:tr");
	}

	return true;
}

void CTable<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->GetCurrentDocument()->WriteNodeEnd(L"w:tbl");
	m_pWriter->WriteEmptyParagraph(true);
}

CTableRow<COOXMLWriter>::CTableRow(COOXMLWriter* pWriter)
	: CTag(pWriter)
{}

bool CTableRow<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	using DataForRow = boost::tuple<const TTableRowStyle*, const CStorageTable&, ERowParseMode, ERowPosition>;

	if (!ValidWriter() || oExtraData.empty() || typeid(DataForRow) != oExtraData.type())
		return false;

	const DataForRow& oDataForRow(boost::any_cast<DataForRow>(oExtraData));
	const TTableRowStyle* pTableRowStyles{boost::get<0>(oDataForRow)};

	if (nullptr == pTableRowStyles)
		return false;

	XmlString& oCurrentDocument{*m_pWriter->GetCurrentDocument()};

	oCurrentDocument.WriteNodeBegin(L"w:tr");

	const TTableStyles& oTableStyles{boost::get<1>(oDataForRow).GetTableStyles()};

	if (!pTableRowStyles->Empty() || 0 < oTableStyles.m_nCellSpacing)
	{
		oCurrentDocument.WriteNodeBegin(L"w:trPr");

		if (pTableRowStyles->m_bIsHeader)
			oCurrentDocument += L"<w:tblHeader/>";

		if (0 < pTableRowStyles->m_unMaxHeight)
			oCurrentDocument += L"<w:trHeight w:val=\"" + std::to_wstring(pTableRowStyles->m_unMaxHeight) + L"\"/>";

		if (0 < oTableStyles.m_nCellSpacing)
			oCurrentDocument += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(oTableStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";

		oCurrentDocument.WriteNodeEnd(L"w:trPr");
	}

	return true;
}

void HTML::CTableRow<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->GetCurrentDocument()->WriteNodeEnd(L"w:tr");
}

CTableCell<COOXMLWriter>::CTableCell(COOXMLWriter* pInterpretator)
	: CTag(pInterpretator)
{}

std::wstring CalculateSidesToClean(UINT unColumnNumber, const std::vector<CTableColgroup*>& arColgroups, UINT unMaxColumns)
{
	if (arColgroups.empty())
		return std::wstring();

	UINT unCurrentNumber = 0;

	for (const CTableColgroup* pColgroup : arColgroups)
	{
		for (const CTableCol* pCol : pColgroup->GetCols())
		{
			if (unCurrentNumber + 1 == unCurrentNumber)
				return (1 != pCol->GetSpan()) ? L"<w:right w:val=\"nil\"/>" : std::wstring();

			unCurrentNumber += pCol->GetSpan();

			if (unColumnNumber == unCurrentNumber)
				return (1 != pCol->GetSpan()) ? L"<w:left w:val=\"nil\"/>" : std::wstring();
			else if (unColumnNumber < unCurrentNumber)
				return std::wstring((1 != unColumnNumber) ? L"<w:left w:val=\"nil\"/>" : L"") + std::wstring((unMaxColumns != unColumnNumber) ? L"<w:right w:val=\"nil\"/>" : L"");
		}
	}

	return std::wstring();
}

bool CTableCell<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	using DataForCell = boost::tuple<const CStorageTableCell&, const CStorageTable&, UINT, ERowParseMode, ERowPosition>;

	if (!ValidWriter() || oExtraData.empty() || typeid(DataForCell) != oExtraData.type())
		return false;

	const DataForCell& oDataForCell{boost::any_cast<const DataForCell>(oExtraData)};
	const CStorageTableCell& oStorageTableCell{boost::get<0>(oDataForCell)};

	XmlString& oCurrentDocument{*m_pWriter->GetCurrentDocument()};

	oCurrentDocument.WriteNodeBegin(L"w:tc");
	oCurrentDocument.WriteNodeBegin(L"w:tcPr");

	const ERowParseMode eParseMode{boost::get<3>(oDataForCell)};

	if (ERowParseMode::Header == eParseMode)
		oCurrentDocument += L"<w:tblHeader/>";

	TTableCellStyle oCellStyle(*oStorageTableCell.GetStyles());

	const CStorageTable& oStorageTable{boost::get<1>(oDataForCell)};
	const UINT unColumnNumber{boost::get<2>(oDataForCell)};
	const TTableCellStyle* pColStyle{oStorageTable.GetColStyle(unColumnNumber)};

	if (NULL != pColStyle)
		oCellStyle += pColStyle;

	if (!oCellStyle.m_oWidth.Empty())
	{
		if (NSCSS::UnitMeasure::Percent == oCellStyle.m_oWidth.GetUnitMeasure())
			oCurrentDocument += L"<w:tcW w:w=\"" + std::to_wstring(oCellStyle.m_oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
		else
		{
			if (!oCellStyle.m_oWidth.Zero())
			{
				int nWidth;
				if (NSCSS::UnitMeasure::None != oCellStyle.m_oWidth.GetUnitMeasure())
					nWidth = oCellStyle.m_oWidth.ToInt(NSCSS::UnitMeasure::Twips);
				else
					nWidth = static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(oCellStyle.m_oWidth.ToDouble(), NSCSS::UnitMeasure::Twips, 96) + 0.5);

				oCurrentDocument += L"<w:tcW w:w=\"" + std::to_wstring(nWidth) + L"\" w:type=\"dxa\"/>";
			}
			else
				oCurrentDocument += L"<w:tcW w:w=\"6\" w:type=\"dxa\"/>";
		}
	}
	else
		oCurrentDocument += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";

	if (1 != oStorageTableCell.GetColspan())
		oCurrentDocument += L"<w:gridSpan w:val=\"" + std::to_wstring(oStorageTableCell.GetColspan()) + L"\"/>";

	if (oStorageTableCell.Merged())
		oCurrentDocument += L"<w:vMerge w:val=\"continue\"/>";
	else if (1 < oStorageTableCell.GetRowspan())
		oCurrentDocument += L"<w:vMerge w:val=\"restart\"/>";

	const TTableStyles oTableStyles{oStorageTable.GetTableStyles()};

	if (!oCellStyle.m_oBorder.Empty() && !oCellStyle.m_oBorder.Zero() /*&& oCellStyle.m_oBorder != oTableStyles.m_oBorder*/)
		oCurrentDocument += L"<w:tcBorders>" + CreateBorders(oCellStyle.m_oBorder, &oCellStyle.m_oPadding) + L"</w:tcBorders>";
	else if (TTableStyles::ETableRules::Groups == oTableStyles.m_enRules)
	{
		std::wstring wsBorders;

		if (oStorageTable.HaveColgroups())
			wsBorders += CalculateSidesToClean(unColumnNumber, oStorageTable.GetColgroups(), oStorageTable.GetMaxColumns());

		const ERowPosition eRowPosition{boost::get<4>(oDataForCell)};

		if (ERowParseMode::Header == eParseMode && ERowPosition::Last == eRowPosition)
			wsBorders += CreateDefaultBorder(L"bottom");
		else if (ERowParseMode::Foother == eParseMode && ERowPosition::First == eRowPosition)
			wsBorders += CreateDefaultBorder(L"top");

		if (!wsBorders.empty())
			oCurrentDocument += L"<w:tcBorders>" + wsBorders + L"</w:tcBorders>";
	}

	if (!oCellStyle.m_oBackground.Empty())
	{
		const std::wstring wsShdFill{(NSCSS::NSProperties::ColorNone == oCellStyle.m_oBackground.GetType()) ? L"auto" : oCellStyle.m_oBackground.ToWString()};
		oCurrentDocument += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + wsShdFill + L"\"/>";
	}

	if (!oCellStyle.m_wsVAlign.empty())
		oCurrentDocument += L"<w:vAlign w:val=\"" + oCellStyle.m_wsVAlign + L"\"/>";
	else
		oCurrentDocument += L"<w:vAlign w:val=\"center\"/>";

	if (!oCellStyle.m_oPadding.Empty() && oTableStyles.m_oPadding != oCellStyle.m_oPadding)
	{
		const int nTopPadding    = std::max(oTableStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
		                                    oCellStyle  .m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nLeftPadding   = std::max(oTableStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
		                                    oCellStyle  .m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));
		const int nBottomPadding = std::max(oTableStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
		                                    oCellStyle  .m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nRightPadding  = std::max(oTableStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
		                                    oCellStyle  .m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));

		oCurrentDocument += L"<w:tcMar>"
		                         "<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>"
		                         "<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>"
		                         "<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>"
		                         "<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>"
		                     "</w:tcMar>";
	}

	oCurrentDocument += L"<w:hideMark/>";
	oCurrentDocument.WriteNodeEnd(L"w:tcPr");

	return true;
}

void HTML::CTableCell<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->GetCurrentDocument()->WriteNodeEnd(L"w:tc");
}

CHTML<COOXMLWriter>::CHTML(COOXMLWriter* pInterpretator)
	: CTag(pInterpretator)
{}

bool CHTML<COOXMLWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	if (!arSelectors.back().m_mAttributes.empty())
	{
		std::wstring wsBackground;
		if (arSelectors.back().GetAttributeValue(L"bgcolor", wsBackground))
		{
			NSCSS::NSProperties::CColor oColor;
			oColor.SetValue(wsBackground);

			if (!oColor.Empty() && !oColor.None())
			{
				const std::wstring wsHEXColor{oColor.ToHEX()};

				if (!wsHEXColor.empty())
					m_pWriter->GetCurrentDocument()->WriteString(L"<w:background w:color=\"" + wsHEXColor + L"\"/>");
			}
		}
	}

	return true;
}

void CHTML<COOXMLWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

}
