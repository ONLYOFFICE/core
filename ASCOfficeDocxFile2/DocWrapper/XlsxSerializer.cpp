/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "XlsxSerializer.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../XlsxSerializerCom/Reader/BinaryWriter.h"
#include "../../XlsxSerializerCom/Writer/BinaryReader.h"
#include "../../ASCOfficePPTXFile/Editor/FontPicker.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "ChartWriter.h"

#include "../BinReader/DefaultThemeWriter.h"
//#include "../../Common/DocxFormat/Source/XlsxFormat/Chart/Chart.h"

namespace BinXlsxRW{
	int g_nCurFormatVersion = 0;

	CXlsxSerializer::CXlsxSerializer()
	{
		m_pExternalDrawingConverter = NULL;
		m_bIsNoBase64 = false;
	}
	CXlsxSerializer::~CXlsxSerializer()
	{
	}

    void CXlsxSerializer::CreateXlsxFolders(const std::wstring& sXmlOptions, const std::wstring& sDstPath,  std::wstring& sMediaPath, std::wstring& sEmbedPath)
	{
        OOX::CPath pathMediaDir = sDstPath + FILE_SEPARATOR_STR + _T("xl") + FILE_SEPARATOR_STR + _T("media");
		OOX::CPath pathEmbedDir = sDstPath + FILE_SEPARATOR_STR + _T("xl") + FILE_SEPARATOR_STR + _T("embeddings");
		
        //создавать папку надо даже при сохранении в csv, потому что когда читаем из бинарника тему, она записывается в файл.
        OOX::CPath pathXlDir = sDstPath + FILE_SEPARATOR_STR + _T("xl");

        OOX::CPath pathThemeDir = pathXlDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultDirectory().GetPath();

        OOX::CPath pathThemeFile = pathThemeDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultFileName().GetPath();

        OOX::CPath pathThemeThemeRelsDir = pathThemeDir + FILE_SEPARATOR_STR + _T("_rels");

        NSDirectory::CreateDirectory(pathXlDir.GetPath());
        NSDirectory::CreateDirectory(pathThemeDir.GetPath());
        NSDirectory::CreateDirectory(pathThemeThemeRelsDir.GetPath());
        NSDirectory::CreateDirectory(pathMediaDir.GetPath());
        NSDirectory::CreateDirectory(pathEmbedDir.GetPath());

        //Create Default Theme
        {
            Writers::DefaultThemeWriter oDefaultThemeWriter;
            oDefaultThemeWriter.Write(pathThemeFile.GetPath());
        }

		sMediaPath = pathMediaDir.GetPath();
		sEmbedPath = pathEmbedDir.GetPath();
	}
    _UINT32 CXlsxSerializer::loadFromFile(const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sXMLOptions, const std::wstring& sMediaDir, const std::wstring& sEmbedDir)
	{
        std::wstring strFileInDir = NSSystemPath::GetDirectoryName(sSrcFileName);

		NSBinPptxRW::CDrawingConverter oDrawingConverter;
		
        oDrawingConverter.SetDstPath(sDstPath + FILE_SEPARATOR_STR + L"xl");
        oDrawingConverter.SetSrcPath(strFileInDir, 2);

		oDrawingConverter.SetMediaDstPath(sMediaDir);
		oDrawingConverter.SetEmbedDstPath(sEmbedDir);

		BinXlsxRW::BinaryFileReader oBinaryFileReader;		
		return oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, &oDrawingConverter, sXMLOptions);
	}
    _UINT32 CXlsxSerializer::saveToFile(const std::wstring& sDstFileName, const std::wstring& sSrcPath, const std::wstring& sXMLOptions)
	{
		COfficeFontPicker* pFontPicker = new COfficeFontPicker();
		pFontPicker->Init(m_sFontDir);
        NSFonts::IFontManager* pFontManager = pFontPicker->get_FontManager();
		DocWrapper::FontProcessor fp;
		fp.setFontManager(pFontManager);
		
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
        if(false == m_sEmbeddedFontsDir.empty())
		{
			NSDirectory::CreateDirectory(m_sEmbeddedFontsDir);

			pFontPicker->SetEmbeddedFontsDirectory(m_sEmbeddedFontsDir);
			pEmbeddedFontsManager = pFontPicker->GetNativeCutter();

			//добавим мега шрифт
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), pFontManager);
			pEmbeddedFontsManager->CheckFont(_T("Arial"), pFontManager);
			//pEmbeddedFontsManager добавляются все цифры
			//для заголовков
			pEmbeddedFontsManager->CheckFont(_T("Calibri"), pFontManager);
            pEmbeddedFontsManager->CheckString(std::wstring(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			//дополнение для ошибок "#NULL!", "#DIV/0!"...
            pEmbeddedFontsManager->CheckString(std::wstring(_T("#!/?")));
			//дополнение для num форматов по умолчанию с id от 0 до 49
            pEmbeddedFontsManager->CheckString(std::wstring(_T(".%E+-():")));
		}

		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetFontManager(pFontManager);
		oOfficeDrawingConverter.SetFontDir(m_sFontDir);
		oOfficeDrawingConverter.SetFontPicker(pFontPicker);

		BinXlsxRW::BinaryFileWriter oBinaryFileWriter(fp);
		_UINT32 result = oBinaryFileWriter.Open(sSrcPath, sDstFileName, pEmbeddedFontsManager, &oOfficeDrawingConverter, sXMLOptions, m_bIsNoBase64);

		RELEASEOBJECT(pFontPicker);
		return result;
	}
 	bool CXlsxSerializer::saveChart(NSBinPptxRW::CBinaryFileReader* pReader, long lLength, const std::wstring& sFilepath, const long& lChartNumber)
	{
		if (NULL == pReader) return false;
		if (NULL == m_pExternalDrawingConverter) return false;
		bool bRes = false;

		m_pExternalDrawingConverter->SetDstContentRels();

	//получаем sThemePath из bsFilename предполагая что папка theme находится на уровень выше bsFilename
		std::wstring sDrawingsPath;
		std::wstring sThemePath;
		std::wstring sEmbedingPath;
		std::wstring sContentTypePath;
		
        size_t nIndex	= (int)sFilepath.rfind(FILE_SEPARATOR_CHAR); 
        nIndex		= (int)sFilepath.rfind(FILE_SEPARATOR_CHAR, nIndex - 1);
		
		if(std::wstring::npos != nIndex)
		{
            std::wstring sFilepathLeft = sFilepath.substr(0, nIndex + 1);

			sThemePath		= sFilepathLeft + L"theme";
			sEmbedingPath	= sFilepathLeft + L"embeddings";
			sDrawingsPath	= sFilepathLeft + L"drawings";
		}
        if		(pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX)	sContentTypePath = L"/word/charts/";
		else if (pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_XLSX)	sContentTypePath = L"/xl/charts/";
		else															sContentTypePath = L"/ppt/charts/";

	//todo theme path
		BinXlsxRW::SaveParams			oSaveParams(sDrawingsPath, sThemePath, m_pExternalDrawingConverter->GetContentTypes());
		OOX::Spreadsheet::CChartSpace	oChartSpace(NULL);
		BinXlsxRW::BinaryChartReader	oBinaryChartReader(*pReader, oSaveParams, m_pExternalDrawingConverter);
		
		oBinaryChartReader.ReadCT_ChartSpace(lLength, &oChartSpace);

		if(oChartSpace.isValid())
		{
			//save xlsx embedded for chart
			if(pReader->m_nDocumentType != XMLWRITER_DOC_TYPE_XLSX && !sEmbedingPath.empty())
			{
				std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(lChartNumber) + L".xlsx";
				std::wstring sXlsxPath = sEmbedingPath + FILE_SEPARATOR_STR + sXlsxFilename;
				
				if (writeChartXlsx(sXlsxPath, oChartSpace))
				{
					pReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(L"xlsx");

					std::wstring sChartsWorksheetRelsName = L"../embeddings/" + sXlsxFilename;
					unsigned int rId;
					std::wstring bstrChartsWorksheetRelType = OOX::FileTypes::MicrosoftOfficeExcelWorksheet.RelationType();
					m_pExternalDrawingConverter->WriteRels(bstrChartsWorksheetRelType, sChartsWorksheetRelsName, std::wstring(), &rId);

					oChartSpace.m_oChartSpace.m_externalData = new OOX::Spreadsheet::CT_ExternalData();
					oChartSpace.m_oChartSpace.m_externalData->m_id = new std::wstring();
					oChartSpace.m_oChartSpace.m_externalData->m_id->append(L"rId");
					oChartSpace.m_oChartSpace.m_externalData->m_id->append(std::to_wstring(rId));
					oChartSpace.m_oChartSpace.m_externalData->m_autoUpdate = new OOX::Spreadsheet::CT_Boolean();
					oChartSpace.m_oChartSpace.m_externalData->m_autoUpdate->m_val = new bool(false);
				}
			}

			std::wstring strFilepath	= sFilepath;
            std::wstring strDir			= NSSystemPath::GetDirectoryName(strFilepath);
            std::wstring strFilename	= NSSystemPath::GetFileName(strFilepath);

            OOX::CPath pathRelsDir = strDir + FILE_SEPARATOR_STR + _T("_rels");

            OOX::CSystemUtility::CreateDirectories(pathRelsDir.GetPath());

			oChartSpace.write(sFilepath, sContentTypePath, *pReader->m_pRels->m_pManager->m_pContentTypes);

            OOX::CPath pathRelsFile = pathRelsDir + FILE_SEPARATOR_STR + strFilename + _T(".rels");
            m_pExternalDrawingConverter->SaveDstContentRels(pathRelsFile.GetPath());

			bRes = true;
		}
		return bRes;
	}
    void CXlsxSerializer::setFontDir(const std::wstring& sFontDir)
	{
		m_sFontDir = sFontDir;
	}
    void CXlsxSerializer::setEmbeddedFontsDir(const std::wstring& sEmbeddedFontsDir)
	{
		m_sEmbeddedFontsDir = sEmbeddedFontsDir;
	}
	void CXlsxSerializer::setDrawingConverter(NSBinPptxRW::CDrawingConverter* pDrawingConverter)
	{
		m_pExternalDrawingConverter = pDrawingConverter;
	}
	void CXlsxSerializer::setIsNoBase64(bool bIsNoBase64)
	{
		m_bIsNoBase64 = bIsNoBase64;
	}

	bool CXlsxSerializer::writeChartXlsx(const std::wstring& sDstFile, const OOX::Spreadsheet::CChartSpace& oChart)
	{
	//анализируем chart
		BinXlsxRW::ChartWriter helper;
		helper.parseChart(oChart.m_oChartSpace.m_chart);
	//создаем temp
		std::wstring sTempDir = NSSystemPath::GetDirectoryName(sDstFile) + FILE_SEPARATOR_STR + NSSystemPath::GetFileName(sDstFile) + L"_TEMP";
		NSDirectory::CreateDirectory(sTempDir);
		OOX::CPath oPath(sTempDir.c_str());
	//шиблонные папки
        std::wstring sXmlOptions = _T("");
        std::wstring sMediaPath;// will be filled by 'CreateXlsxFolders' method
        std::wstring sEmbedPath; // will be filled by 'CreateXlsxFolders' method
		CreateXlsxFolders (sXmlOptions, sTempDir, sMediaPath, sEmbedPath);
	//заполняем Xlsx
		OOX::Spreadsheet::CXlsx oXlsx;
		helper.toXlsx(oXlsx);
	//write
		OOX::CContentTypes oContentTypes;
		bool res = oXlsx.Write(oPath, oContentTypes);
		if (res)
		{
			//zip
			COfficeUtils oOfficeUtils(NULL);
			oOfficeUtils.CompressFileOrDirectory(sTempDir, sDstFile, true);
		}
	//clean
		NSDirectory::DeleteDirectory(sTempDir);
		return res;
	}
	bool CXlsxSerializer::hasPivot(const std::wstring& sSrcPath)
	{
		//todo CXlsx
		std::wstring sData;
		NSFile::CFileBinary::ReadAllTextUtf8(sSrcPath + FILE_SEPARATOR_STR + L"[Content_Types].xml", sData);
		return std::wstring::npos != sData.find(OOX::Spreadsheet::FileTypes::PivotTable.OverrideType());
	}
};
