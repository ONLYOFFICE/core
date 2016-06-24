/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

namespace BinXlsxRW{
	int g_nCurFormatVersion = 0;

	CXlsxSerializer::CXlsxSerializer()
	{
		m_pExternalDrawingConverter = NULL;
	}
	CXlsxSerializer::~CXlsxSerializer()
	{
	}

	void CXlsxSerializer::CreateXlsxFolders(CString& sXmlOptions, CString sDstPath,  CString& sMediaPath, CString& sEmbedPath)
	{
        OOX::CPath pathMediaDir = sDstPath + FILE_SEPARATOR_STR + _T("xl") + FILE_SEPARATOR_STR + _T("media");
		OOX::CPath pathEmbedDir = sDstPath + FILE_SEPARATOR_STR + _T("xl") + FILE_SEPARATOR_STR + _T("embeddings");
		
        //создавать папку надо даже при сохранении в csv, потому что когда читаем из бинарника тему, она записывается в файл.
        OOX::CPath pathXlDir = sDstPath + FILE_SEPARATOR_STR + _T("xl");

        OOX::CPath pathThemeDir = pathXlDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultDirectory().GetPath();

        OOX::CPath pathThemeFile = pathThemeDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultFileName().GetPath();

        OOX::CPath pathThemeThemeRelsDir = pathThemeDir + FILE_SEPARATOR_STR + _T("_rels");

        NSDirectory::CreateDirectory(string2std_string(pathXlDir.GetPath()));
        NSDirectory::CreateDirectory(string2std_string(pathThemeDir.GetPath()));
        NSDirectory::CreateDirectory(string2std_string(pathThemeThemeRelsDir.GetPath()));
        NSDirectory::CreateDirectory(string2std_string(pathMediaDir.GetPath()));
        NSDirectory::CreateDirectory(string2std_string(pathEmbedDir.GetPath()));

        //Create Default Theme
        {
            Writers::DefaultThemeWriter oDefaultThemeWriter;
            oDefaultThemeWriter.Write(pathThemeFile.GetPath());
        }

		sMediaPath = pathMediaDir.GetPath();
		sEmbedPath = pathEmbedDir.GetPath();
	}
    bool CXlsxSerializer::loadFromFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions, const CString& sMediaDir, const CString& sEmbedDir)
	{
		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetMediaDstPath(sMediaDir);
		oOfficeDrawingConverter.SetEmbedDstPath(sEmbedDir);

		//папка с бинарников
		std::wstring strFileInDir = NSSystemPath::GetDirectoryName(string2std_string(sSrcFileName));
		CString sFileInDir = strFileInDir.c_str();

		VARIANT var;
		var.vt = VT_BSTR;
#if defined(_WIN32) || defined (_WIN64)
		var.bstrVal = sFileInDir.AllocSysString();
		oOfficeDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir2"), var);
		RELEASESYSSTRING(var.bstrVal);
#else
		var.bstrVal = sFileInDir.GetString();
		oOfficeDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir2"), var);
#endif

		BinXlsxRW::BinaryFileReader oBinaryFileReader;
		oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, &oOfficeDrawingConverter, sXMLOptions);
		return true;
	}
    bool CXlsxSerializer::saveToFile(const CString& sDstFileName, const CString& sSrcPath, const CString& sXMLOptions)
	{
		COfficeFontPicker* pFontPicker = new COfficeFontPicker();
		pFontPicker->Init(m_sFontDir);
		CFontManager* pFontManager = pFontPicker->get_FontManager();
		DocWrapper::FontProcessor fp;
		fp.setFontManager(pFontManager);
		
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
		if(false == m_sEmbeddedFontsDir.IsEmpty())
		{
			NSDirectory::CreateDirectory(string2std_string(m_sEmbeddedFontsDir));

			pFontPicker->SetEmbeddedFontsDirectory(m_sEmbeddedFontsDir);
			pEmbeddedFontsManager = pFontPicker->GetNativeCutter();

			//добавим мега шрифт
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), pFontManager);
			pEmbeddedFontsManager->CheckFont(_T("Arial"), pFontManager);
			//pEmbeddedFontsManager добавляются все цифры
			//для заголовков
			pEmbeddedFontsManager->CheckFont(_T("Calibri"), pFontManager);
			pEmbeddedFontsManager->CheckString(CString(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			//дополнение для ошибок "#NULL!", "#DIV/0!"...
			pEmbeddedFontsManager->CheckString(CString(_T("#!/?")));
			//дополнение для num форматов по умолчанию с id от 0 до 49
			pEmbeddedFontsManager->CheckString(CString(_T(".%E+-():")));
		}

		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetFontManager(pFontManager);
		oOfficeDrawingConverter.SetFontDir(m_sFontDir);
		oOfficeDrawingConverter.SetFontPicker(pFontPicker);

		BinXlsxRW::BinaryFileWriter oBinaryFileWriter(fp);
		oBinaryFileWriter.Open(sSrcPath, sDstFileName, pEmbeddedFontsManager, &oOfficeDrawingConverter, sXMLOptions);

		RELEASEOBJECT(pFontPicker);
		return true;
	}
	bool CXlsxSerializer::loadChart(CString& sChartPath, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize)
	{
		bool bRes = false;
		//todo передать нормальный oRootPath
		OOX::CPath oRootPath;
		OOX::Spreadsheet::CChartSpace oChart(oRootPath, sChartPath);
		if(NULL != m_pExternalDrawingConverter)
		{
			long nStartPos = oBufferedStream.GetPosition();
			BinXlsxRW::BinaryCommonWriter oBcw(oBufferedStream);

			CString sOldRelsPath = m_pExternalDrawingConverter->GetRelsPath();
			m_pExternalDrawingConverter->SetRelsPath(sChartPath);

			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(oBufferedStream, m_pExternalDrawingConverter);	
			oBinaryChartWriter.WriteCT_ChartSpace(oChart);

			m_pExternalDrawingConverter->SetRelsPath(sOldRelsPath);

			long nEndPos = oBufferedStream.GetPosition();
			lDataSize = nEndPos - nStartPos;
			bRes = true;
		}
		return bRes;
	}
    bool CXlsxSerializer::saveChart(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, CString& sFilepath, CString& sContentTypePath, CString** sContentTypeElement, const long& lChartNumber)
	{
		bool bRes = false;
		*sContentTypeElement = NULL;
		if(NULL != m_pExternalDrawingConverter)
		{
			m_pExternalDrawingConverter->SetDstContentRels();

			//получаем sThemePath из bsFilename предполагая что папка theme находится на уровень выше bsFilename
			CString sThemePath;
			CString sEmbedingPath;
			CString sFilenameReverse = sFilepath;sFilenameReverse.MakeReverse();
			
			int nIndex	= sFilenameReverse.Find(FILE_SEPARATOR_CHAR);
			nIndex		= sFilenameReverse.Find(FILE_SEPARATOR_CHAR, nIndex + 1);
			if(-1 != nIndex)
			{
				CString sFilepathLeft = sFilepath.Left(sFilepath.GetLength() - nIndex);
				sThemePath = sFilepathLeft + _T("theme");
				sEmbedingPath = sFilepathLeft + _T("embeddings");
			}

			//todo theme path
			BinXlsxRW::SaveParams oSaveParams(sThemePath);
			OOX::Spreadsheet::CChartSpace oChartSpace;
			BinXlsxRW::BinaryChartReader oBinaryChartReader(oBufferedStream, oSaveParams, m_pExternalDrawingConverter);
			oBinaryChartReader.ReadCT_ChartSpace(lLength, &oChartSpace.m_oChartSpace);

			if(oChartSpace.isValid())
			{
				//todo не делать embeddings, если пишем xlsx
				//save xlsx
				if(!sEmbedingPath.IsEmpty())
				{
					std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(lChartNumber) + L".xlsx";
					std::wstring sXlsxPath = string2std_string(sEmbedingPath + FILE_SEPARATOR_STR) + sXlsxFilename;
					writeChartXlsx(sXlsxPath, oChartSpace);

					std::wstring sChartsWorksheetRelsName = L"../embeddings/" + sXlsxFilename;
					long rId;
					CString bstrChartsWorksheetRelType = OOX::Spreadsheet::FileTypes::ChartsWorksheet.RelationType();
					m_pExternalDrawingConverter->WriteRels(bstrChartsWorksheetRelType, std_string2string(sChartsWorksheetRelsName), CString(), &rId);

					oChartSpace.m_oChartSpace.m_externalData = new OOX::Spreadsheet::CT_ExternalData();
					oChartSpace.m_oChartSpace.m_externalData->m_id = new CString();
					oChartSpace.m_oChartSpace.m_externalData->m_id->AppendFormat(L"rId%d", rId);
					oChartSpace.m_oChartSpace.m_externalData->m_autoUpdate = new OOX::Spreadsheet::CT_Boolean();
					oChartSpace.m_oChartSpace.m_externalData->m_autoUpdate->m_val = new bool(false);
				}

				std::wstring strFilepath	= string2std_string(sFilepath);
                CString strDir              = std_string2string(NSSystemPath::GetDirectoryName(strFilepath));
                CString strFilename         = std_string2string(NSSystemPath::GetFileName(strFilepath));

                OOX::CPath pathRelsDir = strDir + FILE_SEPARATOR_STR + _T("_rels");

                OOX::CSystemUtility::CreateDirectories(pathRelsDir.GetPath());

				oChartSpace.write2(sFilepath);

                OOX::CPath pathRelsFile = pathRelsDir + FILE_SEPARATOR_STR + strFilename + _T(".rels");
                m_pExternalDrawingConverter->SaveDstContentRels(pathRelsFile.GetPath());

				CString sContentType(sContentTypePath);
				sContentType.Append(strFilename);

				(*sContentTypeElement) = new CString();
				(*sContentTypeElement)->Format(_T("<Override PartName=\"%ls\" ContentType=\"application/vnd.openxmlformats-officedocument.drawingml.chart+xml\"/>"), sContentType);
				(*sContentTypeElement)->Append(oSaveParams.sAdditionalContentTypes);
				
				bRes = true;
			}
		}
		return bRes;
	}
	void CXlsxSerializer::setFontDir(CString& sFontDir)
	{
		m_sFontDir = sFontDir;
	}
	void CXlsxSerializer::setEmbeddedFontsDir(CString& sEmbeddedFontsDir)
	{
		m_sEmbeddedFontsDir = sEmbeddedFontsDir;
	}
	void CXlsxSerializer::setDrawingConverter(NSBinPptxRW::CDrawingConverter* pDrawingConverter)
	{
		m_pExternalDrawingConverter = pDrawingConverter;
	}
	void CXlsxSerializer::writeChartXlsx(const std::wstring& sDstFile, const OOX::Spreadsheet::CChartSpace& oChart)
	{
		//анализируем chart
		BinXlsxRW::ChartWriter helper;
		helper.parseChart(oChart.m_oChartSpace.m_chart);
		//создаем temp
		std::wstring sTempDir = NSSystemPath::GetDirectoryName(sDstFile) + FILE_SEPARATOR_STR + NSSystemPath::GetFileName(sDstFile) + L"_TEMP";
		NSDirectory::CreateDirectory(sTempDir);
		OOX::CPath oPath(sTempDir.c_str());
		//шиблонные папки
		CString sXmlOptions = _T("");
		CString sMediaPath;// will be filled by 'CreateXlsxFolders' method
		CString sEmbedPath; // will be filled by 'CreateXlsxFolders' method
		CreateXlsxFolders (sXmlOptions, std_string2string(sTempDir), sMediaPath, sEmbedPath);
		//заполняем Xlsx
		OOX::Spreadsheet::CXlsx oXlsx;
		helper.toXlsx(oXlsx);
		//write
		CString sAdditionalContentTypes;
		oXlsx.Write(oPath, sAdditionalContentTypes);
		//zip
		COfficeUtils oOfficeUtils(NULL);
		oOfficeUtils.CompressFileOrDirectory(sTempDir, sDstFile, -1);
		//clean
		NSDirectory::DeleteDirectory(sTempDir);
	}
};
