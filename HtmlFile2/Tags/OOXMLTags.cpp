#include "OOXMLTags.h"
#include "../src/StringFinder.h"

#include "../../Common/Network/FileTransporter/include/FileTransporter.h"

#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/common/Base64.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/ProcessEnv.h"
#include "../../DesktopEditor/common/Path.h"

namespace HTML
{
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

TAnchor<COOXMLInterpretator>::TAnchor(COOXMLInterpretator* pInterpretator)
    : TTag(pInterpretator)
{}

bool TAnchor<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	std::wstring wsRef, wsAlt, wsName;
	bool bCross = false;

	if (arSelectors.back().GetAttributeValue(L"href", wsRef) && wsRef.find('#') != std::wstring::npos)
		bCross = true;

	if (arSelectors.back().GetAttributeValue(L"name", wsName))
		m_pInterpretator->WriteBookmark(wsName);

	arSelectors.back().GetAttributeValue(L"alt", wsAlt);

	if (!m_pInterpretator->OpenP())
		m_pInterpretator->CloseR();
	else
		m_pInterpretator->WritePPr(arSelectors);

	if (bCross)
		m_pInterpretator->OpenCrossHyperlink(wsRef, arSelectors);
	else
	{
		std::wstring wsTooltip(wsRef);
		arSelectors.back().GetAttributeValue(L"title", wsTooltip);

		m_pInterpretator->OpenExternalHyperlink(wsRef, wsTooltip, arSelectors);
	}

	return true;
}

void TAnchor<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	bool bCross = false;
	std::wstring wsFootnote;

	if (arSelectors.back().m_wsStyle.find(L"mso-footnote-id") != std::wstring::npos)
		wsFootnote = arSelectors.back().m_wsStyle.substr(arSelectors.back().m_wsStyle.rfind(L':') + 1);
	else
	{
		if (arSelectors.back().GetAttributeValue(L"epub:type", wsFootnote) && wsFootnote.find(L"noteref"))
			wsFootnote = L"href";
	}

	std::wstring wsRef;

	if (arSelectors.back().GetAttributeValue(L"href", wsRef))
	{
		if(wsRef.find('#') != std::wstring::npos)
			bCross = true;
	}

	if (bCross)
	{
		if (wsFootnote == L"href")
			wsFootnote = wsRef.substr(wsRef.find('#') + 1);

		m_pInterpretator->CloseCrossHyperlink(arSelectors, wsFootnote, wsRef);
	}
	else
		m_pInterpretator->CloseExternalHyperlink();
}

TAbbr<COOXMLInterpretator>::TAbbr(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TAbbr<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	std::wstring wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"title", wsTitle))
		return false;

	m_pInterpretator->WritePPr(arSelectors);

	XmlString* pCurrentDocument{&m_pInterpretator->GetCurrentDocument()};

	pCurrentDocument->WriteString(L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r><w:r><w:instrText>HYPERLINK \\l \"" + m_pInterpretator->AddLiteBookmark() + L"\" \\o \"");
	pCurrentDocument->WriteEncodeXmlString(wsTitle);
	pCurrentDocument->WriteString(L"\"</w:instrText></w:r>");
	pCurrentDocument->WriteString(L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>");

	return true;
}

void TAbbr<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pInterpretator->GetCurrentDocument().WriteString(L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>");
}

TBreak<COOXMLInterpretator>::TBreak(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TBreak<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	m_pInterpretator->Break(arSelectors);

	return true;
}

void TBreak<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

TDivision<COOXMLInterpretator>::TDivision(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TDivision<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	m_pInterpretator->UpdatePageStyle(arSelectors);

	UINT unMsoFootnote = 0;

	if (L"footnote" == arSelectors.back().GetAttributeValue(L"epub:type"))
		++unMsoFootnote;

	if (std::wstring::npos != arSelectors.back().m_wsStyle.find(L"mso-element:footnote"))
		++unMsoFootnote;

	std::wstring wsFootnoteID;

	if (!arSelectors.back().m_wsId.empty())
	{
		wsFootnoteID = m_pInterpretator->FindFootnote(arSelectors.back().m_wsId);

		if (!wsFootnoteID.empty())
			++unMsoFootnote;

		if (unMsoFootnote >= 2 && !wsFootnoteID.empty())
		{
			m_pInterpretator->OpenFootnote(wsFootnoteID);
			m_pInterpretator->SetCurrentDocument(&m_pInterpretator->GetNotesXml());
		}
	}

	m_arFootnoteIDs.push(unMsoFootnote);

	return true;
}

void TDivision<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator() || m_arFootnoteIDs.empty())
		return;

	if (m_arFootnoteIDs.top() >= 2)
	{
		m_pInterpretator->CloseFootnote();
		m_pInterpretator->RollBackState();
	}

	m_arFootnoteIDs.pop();
}

TImage<COOXMLInterpretator>::TImage(COOXMLInterpretator* pInterpretator, XmlUtils::CXmlLiteReader* pXmlReader)
	: TTag(pInterpretator), m_pXmlReader(pXmlReader)
{}

bool TImage<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	if (L"svg" == arSelectors.back().m_wsName)
	{
		if (nullptr == m_pXmlReader)
			return false;

		const std::wstring wsImagePath{m_pInterpretator->GetMediaDir() + L'i' + std::to_wstring(m_arrImages.size()) + L".png"};

		if (!ReadSVG(m_pXmlReader->GetOuterXml(), m_pInterpretator->GetFonts(), m_pInterpretator->GetTempDir(), wsImagePath))
			return false;

		TImageData oNewImageData;
		if (!UpdateImageData(wsImagePath, oNewImageData))
			return false;

		m_pInterpretator->WritePPr(arSelectors);
		m_pInterpretator->WriteImage(oNewImageData, std::to_wstring(m_arrImages.size()));
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
		m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
		return true;
	}

	bool bRes = false;
	std::wstring wsExtention;
	const std::wstring wsImagePath{m_pInterpretator->GetMediaDir() + L'i' + std::to_wstring(m_arrImages.size())};

	// Предполагаем картинку в Base64
	if (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"data" && wsSrc.find(L"/", 4) != std::wstring::npos)
		bRes = ReadBase64(wsSrc, wsImagePath, m_pInterpretator->GetFonts(), m_pInterpretator->GetTempDir(), wsExtention);

	const bool bIsAllowExternalLocalFiles = GetStatusUsingExternalLocalFiles();

	if (!bRes && (wsSrc.length() <= 7 || L"http" != wsSrc.substr(0, 4)))
	{
		wsSrc = NSSystemPath::ShortenPath(wsSrc);

		if (!CanUseThisPath(wsSrc, m_pInterpretator->GetSrcPath(), m_pInterpretator->GetCorePath(), bIsAllowExternalLocalFiles))
		{
			m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
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

	const std::wstring wsBasePath{m_pInterpretator->GetBasePath()};

	// Предполагаем картинку в сети
	if (!bRes &&
	    ((!wsBasePath.empty() && wsBasePath.length() > 4 && wsBasePath.substr(0, 4) == L"http") ||
	      (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"http")))
	{
		const std::wstring wsDst = wsImagePath + L'.' + ((!wsExtention.empty()) ? wsExtention : L"png");

		// Проверка gc_allowNetworkRequest предполагается в kernel_network
		NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(m_pInterpretator->GetBasePath() + wsSrc, false);
		oDownloadImg.SetFilePath(wsDst);
		bRes = oDownloadImg.DownloadSync();

		if (!bRes)
		{
			m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		if (IsSVG(wsExtention))
		{
			std::wstring wsFileData;

			if (!NSFile::CFileBinary::ReadAllTextUtf8(wsDst, wsFileData) ||
			    !ReadSVG(wsFileData, m_pInterpretator->GetFonts(), m_pInterpretator->GetTempDir(), wsImagePath))
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
			m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		// Проверка на повтор
		std::vector<std::wstring>::iterator nFind = std::find(m_arrImages.begin(), m_arrImages.end(), wsSrc);
		if (nFind != m_arrImages.end())
		{
			bRes = true;
			nImageId = nFind - m_arrImages.begin();
		}
	}

	// Предполагаем картинку по локальному пути
	if (!bRes)
	{
		const std::wstring wsDst = wsImagePath + L'.' + wsExtention;

		if (!m_pInterpretator->GetBasePath().empty())
		{
			if (!bRes)
				bRes = CopyImage(NSSystemPath::Combine(m_pInterpretator->GetBasePath(), wsSrc), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
			if (!bRes)
				bRes = CopyImage(NSSystemPath::Combine(m_pInterpretator->GetSrcPath(), NSSystemPath::Combine(m_pInterpretator->GetBasePath(), wsSrc)), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		}
		if (!bRes)
			bRes = CopyImage(NSSystemPath::Combine(m_pInterpretator->GetSrcPath(), wsSrc), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		if (!bRes)
			bRes = CopyImage(m_pInterpretator->GetSrcPath() + L"/" + NSFile::GetFileName(wsSrc), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		if (!bRes)
			bRes = CopyImage(wsSrc, m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
	}

	if (!bRes)
		m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
	else
	{
		m_arrImages.push_back(wsSrc);

		m_pInterpretator->WritePPr(arSelectors);

		const std::wstring wsImageID{std::to_wstring(m_arrImages.size())};

		if (nImageId < 0)
		{
			m_pInterpretator->WriteImageRels(wsImageID, wsImageID + L'.' + wsExtention);
			m_arrImages.push_back(wsSrc);
		}

		if (!oImageData.ZeroSize())
		{
			m_pInterpretator->WriteImage(oImageData, wsImageID);
			return true;
		}

		TImageData oNewImageData{oImageData};
		if (!UpdateImageData(wsImagePath + L'.' + wsExtention, oNewImageData))
			return false;

		m_pInterpretator->WriteImage(oNewImageData, wsImageID);
	}

	return true;
}

void TImage<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

TFont<COOXMLInterpretator>::TFont(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TFont<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
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
			// if(wsValue.front() == L'+')
			// 	nSize += NSStringFinder::ToInt(wsValue.substr(1));
			// else if(wsValue.front() == L'-')
			// 	nSize -= NSStringFinder::ToInt(wsValue.substr(1));
			// else
			// 	nSize = NSStringFinder::ToInt(wsValue);
		}

		if (nSize < 1 || nSize > 7)
			nSize = 3;

		arSelectors.back().m_pCompiledStyle->m_oFont.SetSize(HTML_FONTS[nSize - 1], NSCSS::UnitMeasure::Point,  arSelectors.size());
	}

	return true;
}

void TFont<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

TInput<COOXMLInterpretator>::TInput(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TInput<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
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
		m_pInterpretator->WritePPr(arSelectors);
		m_pInterpretator->OpenR();
		m_pInterpretator->WriteRPr(m_pInterpretator->GetCurrentDocument(), arSelectors);
		m_pInterpretator->OpenT();
		m_pInterpretator->GetCurrentDocument().WriteEncodeXmlString(wsValue + L' ');
		m_pInterpretator->CloseT();
		m_pInterpretator->CloseR();
	}

	return true;
}

void TInput<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

TBaseFont<COOXMLInterpretator>::TBaseFont(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TBaseFont<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
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

	m_pInterpretator->SetBaseFont(L"*{" + wsFontStyles + L'}');

	return true;
}

void TBaseFont<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

TBlockquote<COOXMLInterpretator>::TBlockquote(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TBlockquote<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	const std::wstring wsKeyWord{arSelectors.back().m_wsName};

	std::map<std::wstring, UINT>::const_iterator itFound = m_mDivs.find(wsKeyWord);

	if (m_mDivs.end() != itFound)
	{
		m_pInterpretator->SetDivId(std::to_wstring(itFound->second));
		return true;
	}

	const std::wstring wsId{std::to_wstring(m_mDivs.size() + 1)};

	XmlString &oWebSettings{m_pInterpretator->GetWebSettingsXml()};

	if (m_mDivs.empty())
		oWebSettings.WriteString(L"<w:divs>");

	NSCSS::CCompiledStyle *pStyle = arSelectors.back().m_pCompiledStyle;

	const bool bInTable = ElementInTable(arSelectors);

	INT nMarLeft   = (!bInTable) ? 720 : 0;
	INT nMarRight  = (!bInTable) ? 720 : 0;
	INT nMarTop    = (!bInTable) ? 100 : 0;
	INT nMarBottom = (!bInTable) ? 100 : 0;

	const NSCSS::NSProperties::CPage *pPageData{m_pInterpretator->GetPageData()};

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

	m_pInterpretator->SetDivId(wsId);

	return true;
}

void TBlockquote<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pInterpretator->RollBackDivId();
}

THorizontalRule<COOXMLInterpretator>::THorizontalRule(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator), m_unShapeId(1)
{}

bool THorizontalRule<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
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

	XmlString& oCurrentDocument{m_pInterpretator->GetCurrentDocument()};

	m_pInterpretator->OpenP();
	oCurrentDocument.WriteString(L"<w:pPr><w:jc w:val=\"" + wsAlign + L"\"/></w:pPr>");
	m_pInterpretator->OpenR();

	const NSCSS::NSProperties::CPage *pPageData{m_pInterpretator->GetPageData()};

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

	m_pInterpretator->CloseP();

	++m_unShapeId;

	return true;
}

void THorizontalRule<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

TList<COOXMLInterpretator>::TList(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator), m_unNumberingId(1)
{}

bool TList<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return false;

	m_pInterpretator->CloseP();

	//Нумерованный список
	if (L"ol" == arSelectors.back().m_wsName)
	{
		const int nStart{NSStringFinder::ToInt(arSelectors.back().GetAttributeValue(L"start"), 1)};

		XmlString& oNumberXml{m_pInterpretator->GetNumberingXml()};

		const std::wstring wsStart(std::to_wstring(nStart));
		oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
		oNumberXml.WriteString(std::to_wstring(m_unNumberingId++));
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
	}

	return true;
}

void TList<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pInterpretator->CloseP();
}

TListElement<COOXMLInterpretator>::TListElement(COOXMLInterpretator* pInterpretator)
	: TTag(pInterpretator)
{}

bool TListElement<COOXMLInterpretator>::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	return ValidInterpretator();
}

void TListElement<COOXMLInterpretator>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pInterpretator->CloseP();
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
}
