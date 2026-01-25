#ifndef OOXMLTAGS_H
#define OOXMLTAGS_H

#include "HTMLTags.h"
#include "../Interpretators/OOXMLInterpretator.h"
#include <stack>

namespace HTML
{
template<>
struct TAnchor<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TAnchor(COOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override
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

	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override
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
};

template<>
struct TAbbr<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TAbbr(COOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override
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

	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override
	{
		if (!ValidInterpretator())
			return;

		m_pInterpretator->GetCurrentDocument().WriteString(L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>");
	}
};

template<>
struct TBreak<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TBreak(COOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override
	{
		if (!ValidInterpretator())
			return false;

		m_pInterpretator->Break(arSelectors);

		return true;
	}

	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override
	{
		if (!ValidInterpretator())
			return;
	}
};

template<>
struct TDivision<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	std::stack<UINT> m_arFootnoteIDs;

	TDivision(COOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override
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

	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override
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
};

bool NotValidExtension(const std::wstring& sExtention);
bool IsSVG(const std::wstring& wsExtention);
bool ReadSVG(const std::wstring& wsSvg, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath);
bool ReadBase64(const std::wstring& wsSrc, const std::wstring& wsImagePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsExtention);
bool GetStatusUsingExternalLocalFiles();
bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsSrcPath, const std::wstring& wsCorePath, bool bIsAllowExternalLocalFiles);

std::wstring NormalizePath(const std::wstring& wsPath);
std::wstring CombinePaths(const std::wstring& wsFirstPath, const std::wstring& wsSecondPath);

std::wstring GetFileExtention(const std::wstring& wsFilePath);
std::wstring GetFileName(const std::wstring& wsFilePath);

bool DownloadImage(const std::wstring& wsHref, const std::wstring& wsDst);
bool CopyImage(std::wstring wsImageSrc, const std::wstring& wsSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles);
bool UpdateImageData(const std::wstring& wsImagePath, TImageData& oImageData);

bool ReadAllTextUtf8(const std::wstring& wsFilePath, std::wstring& wsText);
bool RemoveFile(const std::wstring& wsFilePath);

template<>
struct TImage<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	std::vector<std::wstring> m_arrImages;

	TImage(COOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override
	{
		if (!ValidInterpretator())
			return false;

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
			wsSrc = NormalizePath(wsSrc);

			if (!CanUseThisPath(wsSrc, m_pInterpretator->GetSrcPath(), m_pInterpretator->GetCorePath(), bIsAllowExternalLocalFiles))
			{
				m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
				return true;
			}
		}

		// Проверка расширения
		if (!bRes)
		{
			wsExtention = GetFileExtention(wsSrc);
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
			bRes = DownloadImage(m_pInterpretator->GetBasePath() + wsSrc, wsDst);

			if (!bRes)
			{
				m_pInterpretator->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
				return true;
			}

			if (IsSVG(wsExtention))
			{
				std::wstring wsFileData;

				if (!ReadAllTextUtf8(wsDst, wsFileData) ||
				    !ReadSVG(wsFileData, m_pInterpretator->GetFonts(), m_pInterpretator->GetTempDir(), wsImagePath))
					bRes = false;

				RemoveFile(wsDst);
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
					bRes = CopyImage(CombinePaths(m_pInterpretator->GetBasePath(), wsSrc), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
				if (!bRes)
					bRes = CopyImage(CombinePaths(m_pInterpretator->GetSrcPath(), CombinePaths(m_pInterpretator->GetBasePath(), wsSrc)), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
			}
			if (!bRes)
				bRes = CopyImage(CombinePaths(m_pInterpretator->GetSrcPath(), wsSrc), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
			if (!bRes)
				bRes = CopyImage(m_pInterpretator->GetSrcPath() + L"/" + GetFileName(wsSrc), m_pInterpretator->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
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

	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override
	{
		if (!ValidInterpretator())
			return;
	}
};
}

#endif // OOXMLTAGS_H
