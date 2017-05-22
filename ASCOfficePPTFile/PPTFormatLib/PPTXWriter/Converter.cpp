/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#pragma once

#include "../../../ASCOfficePPTXFile/Editor/Drawing/Document.h"

#include "../../../ASCOfficePPTXFile/Editor/DefaultNotesMaster.h"
#include "../../../ASCOfficePPTXFile/Editor/DefaultNotesTheme.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../../DesktopEditor/common/Directory.h"

#include "../Reader/PPTDocumentInfo.h"

#include "ShapeWriter.h"
#include "StylesWriter.h"

#include "Converter.h"	

namespace NSPresentationEditor
{
	namespace NSPPTXWriterConst
	{
        static std::wstring g_string_rels_presentation = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/>\
</Relationships>");

        static std::wstring g_string_core = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\
<dc:title>Slide 1</dc:title>\
<dc:creator>OnlyOffice</dc:creator>\
<cp:lastModifiedBy>OnlyOffice</cp:lastModifiedBy>\
<cp:revision>1</cp:revision>\
</cp:coreProperties>");
	}
}


NSPresentationEditor::CPPTXWriter::CPPTXWriter()
{
    m_strTempDirectory	= NSDirectory::GetTempPath() + FILE_SEPARATOR_STR + _T("TempPPTX");
    m_strDstFileName	= NSDirectory::GetTempPath() + FILE_SEPARATOR_STR + _T("Test.pptx");

	m_pDocument			= NULL;
	m_pUserInfo			= NULL;

    m_pShapeWriter = new CShapeWriter();
}
	
NSPresentationEditor::CPPTXWriter::~CPPTXWriter()
{
	RELEASEOBJECT(m_pShapeWriter);
}

void NSPresentationEditor::CPPTXWriter::CreateFile(CPPTUserInfo* pUserInfo	)
{
	m_pUserInfo = pUserInfo;

	m_pDocument = dynamic_cast<CDocument*>(pUserInfo);

	m_pDocument->m_oInfo.m_lUnitsHor			= 36000 * m_pDocument->m_oInfo.m_lMillimetresHor;
	m_pDocument->m_oInfo.m_lUnitsVer			= 36000 * m_pDocument->m_oInfo.m_lMillimetresVer;

	m_oManager.Clear();
	m_oManager.SetDstMedia(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR);

	m_pShapeWriter->InitNextId();

    NSDirectory::CreateDirectory(m_strTempDirectory);
	CFile oFile;
    std::wstring strMemory = _T("");

	// _rels 
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels"));
	
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels")  + FILE_SEPARATOR_STR + _T(".rels"));
	strMemory = NSPPTXWriterConst::g_string_rels_presentation;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// docProps 
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps"));
	
	// core
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml"));
	strMemory = NSPPTXWriterConst::g_string_core;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// app
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml"));
	WriteApp(oFile);
	oFile.CloseFile();

	// content types
	WriteContentTypes();

	// ppt 
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt"));
	WritePresInfo();

	WriteAll();
}

void NSPresentationEditor::CPPTXWriter::CreateFile(CDocument* pDocument)
{
	m_pDocument = pDocument;
	m_oManager.Clear();
	m_oManager.SetDstMedia(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR);

	m_pShapeWriter->InitNextId();

    NSDirectory::CreateDirectory(m_strTempDirectory);
	CFile oFile;
    std::wstring strMemory = _T("");

	// _rels 
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels"));
	
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels")  + FILE_SEPARATOR_STR + _T(".rels"));
	strMemory = NSPPTXWriterConst::g_string_rels_presentation;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// docProps 
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps"));
	
	// core
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml"));
	strMemory = NSPPTXWriterConst::g_string_core;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// app
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml"));
	WriteApp(oFile);
	oFile.CloseFile();

	// content types
	WriteContentTypes();

	// ppt 
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt"));
	WritePresInfo();

	WriteAll();
}

void NSPresentationEditor::CPPTXWriter::CloseFile()
{
	m_oManager.Clear();
}


void NSPresentationEditor::CPPTXWriter::WriteContentTypes()
{
    std::wstring strContentTypes = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\
<Default Extension=\"bmp\" ContentType=\"image/bmp\" />\
<Default Extension=\"png\" ContentType=\"image/png\" />\
<Default Extension=\"jpeg\" ContentType=\"image/jpeg\" />\
<Default Extension=\"wmf\" ContentType=\"image/x-wmf\" />\
<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\" />\
<Default Extension=\"xml\" ContentType=\"application/xml\" />\
<Default Extension=\"gif\" ContentType=\"image/gif\"/>\
<Default Extension=\"emf\" ContentType=\"image/x-emf\"/>\
<Default Extension=\"wav\" ContentType=\"audio/wav\"/>\
<Default Extension=\"wma\" ContentType=\"audio/x-ms-wma\"/>\
<Default Extension=\"mp3\" ContentType=\"audio/unknown\"/>\
<Default Extension=\"m4a\" ContentType=\"audio/unknown\"/>\
<Default Extension=\"wmv\" ContentType=\"video/x-ms-wmv\"/>\
<Default Extension=\"avi\" ContentType=\"video/avi\"/>\
<Default Extension=\"m4v\" ContentType=\"video/unknown\"/>\
<Default Extension=\"mp4\" ContentType=\"video/unknown\"/>\
<Default Extension=\"mov\" ContentType=\"video/unknown\"/>\
<Default Extension=\"xls\" ContentType=\"application/vnd.ms-excel\"/>\
<Default Extension=\"xlsx\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\"/>\
<Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\" />\
<Default Extension=\"jpg\" ContentType=\"application/octet-stream\"/>");

	strContentTypes += _T("<Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" />\
<Override PartName=\"/ppt/presProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" />\
<Override PartName=\"/ppt/viewProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml\" />\
<Override PartName=\"/ppt/tableStyles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml\"/>\
<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\" />\
<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\" />");

	int nIndexLayout = 1, nIndexTheme = 1;
	
	for (size_t nT = 0; nT < m_pDocument->m_arThemes.size(); nT++, nIndexTheme++)
	{
        strContentTypes += L"<Override PartName=\"/ppt/theme/theme" + std::to_wstring(nIndexTheme) + \
                L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\" />";
		
		strContentTypes += L"<Override PartName=\"/ppt/slideMasters/slideMaster" + std::to_wstring(nIndexTheme) + \
                L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\"/>";


		for (size_t nL = 0; nL < m_pDocument->m_arThemes[nT].m_arLayouts.size(); nL++, nIndexLayout++)
		{
            strContentTypes += L"<Override PartName=\"/ppt/slideLayouts/slideLayout" + std::to_wstring(nIndexLayout) +
                L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\"/>";
		}
	}

	if (m_pDocument->m_pNotesMaster)
	{
		strContentTypes += L"<Override PartName=\"/ppt/theme/theme" + std::to_wstring(nIndexTheme++) + L".xml\" \
	ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>";
		strContentTypes += L"<Override PartName=\"/ppt/notesMasters/notesMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml\"/>";
	}
	if (m_pDocument->m_pHandoutMaster)
	{
		strContentTypes += L"<Override PartName=\"/ppt/theme/theme" + std::to_wstring(nIndexTheme++) + L".xml\" \
	ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>";
		strContentTypes += L"<Override PartName=\"/ppt/handoutMasters/handoutMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.handoutMaster+xml\"/>";
	}

	for (size_t nS = 0; nS < m_pDocument->m_arSlides.size(); ++nS)
	{
        strContentTypes += L"<Override PartName=\"/ppt/slides/slide" + std::to_wstring( nS + 1)
                + L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slide+xml\"/>";
	}

	for (size_t nS = 0; nS < m_pDocument->m_arNotes.size(); ++nS)
	{
        strContentTypes += L"<Override PartName=\"/ppt/notesSlides/notesSlide" + std::to_wstring( nS + 1)
                    + L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml\"/>";
	}
	strContentTypes += _T("</Types>");

	CFile oFile;
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("[Content_Types].xml"));
	oFile.WriteStringUTF8(strContentTypes);
	oFile.CloseFile();
}


void NSPresentationEditor::CPPTXWriter::WriteApp(CFile& oFile)
{
    std::wstring str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\">\
<TotalTime>0</TotalTime>\
<Words>0</Words>\
<Application>OnlyOffice</Application>\
<PresentationFormat>On-screen Show (4:3)</PresentationFormat>\
<Paragraphs>0</Paragraphs>");

	oFile.WriteStringUTF8(str1);

    oFile.WriteStringUTF8(L"<Slides>" + std::to_wstring(m_pDocument->m_arSlides.size()) + L"</Slides>");
    oFile.WriteStringUTF8(L"<Notes>" + std::to_wstring(m_pDocument->m_arNotes.size()) + L"</Notes>");

    oFile.WriteStringUTF8(L"<HiddenSlides>0</HiddenSlides>\
                          <MMClips>2</MMClips>\
                          <ScaleCrop>false</ScaleCrop>\
                          <HeadingPairs>\
                          <vt:vector size=\"4\" baseType=\"variant\">");

	int nCountThemes = (int)m_pDocument->m_arThemes.size();
	int nCountSlides = (int)m_pDocument->m_arSlides.size();

    std::wstring strThemes = L"<vt:variant><vt:lpstr>Theme</vt:lpstr></vt:variant><vt:variant><vt:i4>" +std::to_wstring(nCountThemes) +
            L"</vt:i4></vt:variant>";

    std::wstring strSlides = L"<vt:variant><vt:lpstr>Slide Titles</vt:lpstr></vt:variant><vt:variant><vt:i4>" +
            std::to_wstring(nCountSlides) + L"</vt:i4></vt:variant></vt:vector></HeadingPairs>";

    std::wstring strTitles = L"<TitlesOfParts><vt:vector size=\"" + std::to_wstring(nCountSlides + nCountThemes) + L"\" baseType=\"lpstr\">";

    oFile.WriteStringUTF8(strThemes + strSlides + strTitles);

    std::wstring strMemory = _T("");
	for (int i = 1; i <= nCountThemes; ++i)
	{
        strMemory += L"<vt:lpstr>Theme " + std::to_wstring(i) + L"</vt:lpstr>";
	}
	for (int i = 1; i <= nCountSlides; ++i)
	{
        strMemory += L"<vt:lpstr>Slide " + std::to_wstring(i) + L"</vt:lpstr>";
	}

    std::wstring str5 = _T("</vt:vector>\
</TitlesOfParts>\
<Company></Company>\
<LinksUpToDate>false</LinksUpToDate>\
<SharedDoc>false</SharedDoc>\
<HyperlinksChanged>false</HyperlinksChanged>\
<AppVersion>4.4000</AppVersion>\
</Properties>");

	strMemory += str5;

	oFile.WriteStringUTF8(strMemory);
}

void NSPresentationEditor::CPPTXWriter::WritePresInfo()
{
	CFile oFile;
	
// tableStyles.xml
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt")  + FILE_SEPARATOR_STR + _T("tableStyles.xml"));
	
    oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                          <a:tblStyleLst xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" def=\"{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}\"/>");
	oFile.CloseFile();

// presProps.xml
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("presProps.xml"));
    oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                          <p:presentationPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
                          <p:extLst><p:ext uri=\"{E76CE94A-603C-4142-B9EB-6D1370010A27}\">\
                          <p14:discardImageEditData xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"0\" /></p:ext>\
                          <p:ext uri=\"{D31A062A-798A-4329-ABDD-BBA856620510}\">\
                          <p14:defaultImageDpi xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"220\" />\
                          </p:ext></p:extLst></p:presentationPr>");
	oFile.CloseFile();

// viewProps.xml
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("viewProps.xml"));
    oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
                          <p:viewPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
                          <p:normalViewPr><p:restoredLeft sz=\"15620\"/><p:restoredTop sz=\"94660\"/></p:normalViewPr><p:slideViewPr><p:cSldViewPr><p:cViewPr varScale=\"1\">\
                          <p:scale><a:sx n=\"104\" d=\"100\"/><a:sy n=\"104\" d=\"100\"/></p:scale><p:origin x=\"-1236\" y=\"-90\"/></p:cViewPr><p:guideLst>\
                          <p:guide orient=\"horz\" pos=\"2160\"/><p:guide pos=\"2880\"/></p:guideLst></p:cSldViewPr></p:slideViewPr><p:notesTextViewPr><p:cViewPr><p:scale>\
                          <a:sx n=\"1\" d=\"1\"/><a:sy n=\"1\" d=\"1\"/></p:scale><p:origin x=\"0\" y=\"0\"/></p:cViewPr></p:notesTextViewPr><p:gridSpacing cx=\"72008\" cy=\"72008\"/></p:viewPr>");
	oFile.CloseFile();

	// presentation.xml + _rels/presentation.xml.rels
    std::wstring strPresRels;

    std::wstring strPresMasters;
    std::wstring strPresSlides;
	std::wstring strNotesIDs;
    std::wstring strHandoutIDs;

	size_t nCountLayouts = 0;

	for (size_t nIndexTheme = 0; nIndexTheme < m_pDocument->m_arThemes.size(); ++nIndexTheme)
	{
        strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(2 * nIndexTheme + 1) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"slideMasters/slideMaster" +
                std::to_wstring(nIndexTheme + 1) + L".xml\"/>";

        strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(2 * nIndexTheme + 2) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme" +
                std::to_wstring(nIndexTheme + 1) + L".xml\"/>";

        strPresMasters += L"<p:sldMasterId id=\"" + std::to_wstring(0x80000000 + nCountLayouts) + L"\" r:id=\"rId" + std::to_wstring(2 * nIndexTheme + 1) + L"\"/>";

        nCountLayouts += m_pDocument->m_arThemes[nIndexTheme].m_arLayouts.size();
		nCountLayouts += 1;
	}

	int nCurrentRels = (int)(2 * m_pDocument->m_arThemes.size() + 1);
    	
	if (m_pDocument->m_pNotesMaster)
	{
		strNotesIDs = L"<p:notesMasterIdLst><p:notesMasterId r:id=\"rId" + std::to_wstring(nCurrentRels) + L"\"/></p:notesMasterIdLst>";
		strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"notesMasters/notesMaster1.xml\"/>";
		++nCurrentRels;
	}
	if (m_pDocument->m_pHandoutMaster)
	{
		strHandoutIDs = L"<p:handoutMasterIdLst><p:handoutMasterId r:id=\"rId" + std::to_wstring(nCurrentRels) + L"\"/></p:handoutMasterIdLst>";
		strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster\" Target=\"handoutMasters/handoutMaster1.xml\"/>";
		++nCurrentRels;
	}	
	for (size_t nIndexSlide = 0; nIndexSlide < m_pDocument->m_arSlides.size(); ++nIndexSlide, ++nCurrentRels)
	{
        strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels) +
                       L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"slides/slide" +
                       std::to_wstring(nIndexSlide + 1) + L".xml\" />";

        strPresSlides += L"<p:sldId id=\"" + std::to_wstring(256 + nIndexSlide) + L"\" r:id=\"rId" + std::to_wstring(nCurrentRels) + L"\"/>";
	}

    strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps\" Target=\"presProps.xml\"/>";
    strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles\" Target=\"tableStyles.xml\"/>";
    strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps\" Target=\"viewProps.xml\"/>";

	strPresRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">" 
		+ strPresRels + L"</Relationships>";

    std::wstring strPptRels = m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("_rels");

    NSDirectory::CreateDirectory(strPptRels);
	oFile.CreateFile(strPptRels + FILE_SEPARATOR_STR + _T("presentation.xml.rels"));
	oFile.WriteStringUTF8(strPresRels);
	oFile.CloseFile();

    std::wstring strSizePres = L"<p:sldSz cx=\"" + std::to_wstring(m_pDocument->m_oInfo.m_lUnitsHor) +
                                L"\" cy=\"" + std::to_wstring(m_pDocument->m_oInfo.m_lUnitsVer) +
            L"\" type=\"screen4x3\" /><p:notesSz cx=\"" + std::to_wstring(m_pDocument->m_oInfo.m_lUnitsVer) +
            L"\" cy=\"" + std::to_wstring(m_pDocument->m_oInfo.m_lUnitsHor) + L"\"/>";

    std::wstring strDefaultTextStyle = _T("<p:defaultTextStyle>");
   
	if (m_pDocument->m_arThemes.size() > 0)
	{
		strDefaultTextStyle += CStylesWriter::ConvertStyles(m_pDocument->m_arThemes[0].m_pStyles[0], m_pDocument->m_oInfo, 9);
	}
	strDefaultTextStyle += _T("</p:defaultTextStyle>");

    std::wstring strPres = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
	strPres += _T("<p:presentation xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" saveSubsetFonts=\"1\"");
		if ((m_pUserInfo) && (m_pUserInfo->m_bRtl))
		{
			strPres += _T(" rtl=\"1\"");
		}
		strPres += _T(">");
		strPres += _T("<p:sldMasterIdLst>") + strPresMasters + _T("</p:sldMasterIdLst>");
		strPres += strNotesIDs ;
		strPres += strHandoutIDs ;
		strPres +=_T("<p:sldIdLst>") + strPresSlides + _T("</p:sldIdLst>");
		strPres += strSizePres;
		strPres += strDefaultTextStyle;
	strPres +=_T("</p:presentation>");

    oFile.CreateFile(m_strTempDirectory+ FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR + _T("presentation.xml"));
	oFile.WriteStringUTF8(strPres);
	oFile.CloseFile();		
}

void NSPresentationEditor::CPPTXWriter::WriteAll()
{
    std::wstring strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;
	
    NSDirectory::CreateDirectory(strPptDirectory + _T("media"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("theme"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("slideMasters"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("slideMasters")		+ FILE_SEPARATOR_STR + _T("_rels"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("slideLayouts"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("slideLayouts")		+ FILE_SEPARATOR_STR + _T("_rels"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("slides"));
    NSDirectory::CreateDirectory(strPptDirectory + _T("slides")				+ FILE_SEPARATOR_STR + _T("_rels"));
    
	if (m_pDocument->m_pHandoutMaster)
	{
		NSDirectory::CreateDirectory(strPptDirectory + _T("handoutMasters"));
		NSDirectory::CreateDirectory(strPptDirectory + _T("handoutMasters")	+ FILE_SEPARATOR_STR + _T("_rels"));
	}
	if (m_pDocument->m_pNotesMaster)
	{
		NSDirectory::CreateDirectory(strPptDirectory + _T("notesMasters"));
		NSDirectory::CreateDirectory(strPptDirectory + _T("notesMasters")	+ FILE_SEPARATOR_STR + _T("_rels"));
	}
	if (!m_pDocument->m_arNotes.empty())
	{
		NSDirectory::CreateDirectory(strPptDirectory + _T("notesSlides"));
		NSDirectory::CreateDirectory(strPptDirectory + _T("notesSlides")	+ FILE_SEPARATOR_STR + _T("_rels"));
	}
	WriteThemes();
	
	WriteSlides();
	WriteNotes();
}

void NSPresentationEditor::CPPTXWriter::WriteThemes()
{
	int nStartLayout = 0, nIndexTheme = 0;

	for (size_t i = 0; i < m_pDocument->m_arThemes.size(); i++)
	{
		WriteTheme(&m_pDocument->m_arThemes[i], nIndexTheme, nStartLayout, 1);
	}

	WriteTheme(m_pDocument->m_pNotesMaster, nIndexTheme, nStartLayout, 2);
	WriteTheme(m_pDocument->m_pHandoutMaster, nIndexTheme, nStartLayout, 3);
}

void NSPresentationEditor::CPPTXWriter::WriteTheme(CTheme* pTheme, int & nIndexTheme, int & nStartLayout, int Type)
{		
	if (!pTheme) return;

	std::wstring strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;
	
	std::wstring strThemeFile = L"theme" + std::to_wstring(nIndexTheme + 1) + L".xml";
	strThemeFile = strPptDirectory + _T("theme") + FILE_SEPARATOR_STR + strThemeFile;
	
	CFile oFile;
	oFile.CreateFile(strThemeFile);

	NSPresentationEditor::CStringWriter oStringWriter;

	oStringWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\""));
	oStringWriter.WriteStringXML(pTheme->m_sThemeName);
	oStringWriter.WriteString(std::wstring(L"\"><a:themeElements>"));

	WriteColorScheme(oStringWriter, L"Default", pTheme->m_arColorScheme);

	oStringWriter.WriteString(std::wstring(L"<a:fontScheme name=\"default\"><a:majorFont><a:latin typeface=\""));
	oStringWriter.WriteStringXML(pTheme->m_arFonts[0].Name);
	oStringWriter.WriteString(std::wstring(L"\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/></a:majorFont>"));

	oStringWriter.WriteString(std::wstring(L"<a:minorFont><a:latin typeface=\""));
	
	if (pTheme->m_arFonts.size() > 1 )	oStringWriter.WriteString	(pTheme->m_arFonts[1].Name);
	else								oStringWriter.WriteStringXML(pTheme->m_arFonts[0].Name);

	oStringWriter.WriteString(std::wstring(L"\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/></a:minorFont>"));
	oStringWriter.WriteString(std::wstring(L"</a:fontScheme>"));
 
		oStringWriter.WriteString(std::wstring(L"<a:fmtScheme name=\"Default\">\
<a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\">\
<a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/>\
<a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/>\
</a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\">\
<a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/>\
<a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst>\
<a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/>\
</a:schemeClr></a:solidFill><a:prstDash val=\"solid\"/></a:ln><a:ln w=\"25400\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill>\
<a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/></a:ln><a:ln w=\"38100\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\
<a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr>\
</a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\
<a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr>\
</a:outerShdw></a:effectLst><a:scene3d><a:camera prst=\"orthographicFront\"><a:rot lat=\"0\" lon=\"0\" rev=\"0\"/></a:camera>\
<a:lightRig rig=\"threePt\" dir=\"t\"><a:rot lat=\"0\" lon=\"0\" rev=\"1200000\"/></a:lightRig></a:scene3d><a:sp3d><a:bevelT w=\"63500\" h=\"25400\"/>\
</a:sp3d></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\">\
<a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\">\
<a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\">\
<a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\">\
<a:fillToRect l=\"50000\" t=\"-80000\" r=\"50000\" b=\"180000\"/></a:path></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\">\
<a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\">\
<a:fillToRect l=\"50000\" t=\"50000\" r=\"50000\" b=\"50000\"/></a:path></a:gradFill></a:bgFillStyleLst></a:fmtScheme>"));

	oStringWriter.WriteString(std::wstring(L"</a:themeElements><a:objectDefaults/>"));
	oStringWriter.WriteString(std::wstring(L"<a:extraClrSchemeLst>"));
	
	for (size_t i = 0 ; i < pTheme->m_arExtraColorScheme.size(); i++)
	{
		std::wstring str = L" " + std::to_wstring(i + 1);
		WriteColorScheme(oStringWriter, pTheme->m_sThemeName + str, pTheme->m_arExtraColorScheme[i], true); //extra
	}
	
	oStringWriter.WriteString(std::wstring(L"</a:extraClrSchemeLst>"));
	oStringWriter.WriteString(std::wstring(L"</a:theme>"));

	oFile.WriteStringUTF8(oStringWriter.GetData());
	oFile.CloseFile();

	CRelsGenerator oRels(&m_oManager);
	int nCountLayouts = (int)pTheme->m_arLayouts.size();
	oRels.StartMaster(nIndexTheme, nStartLayout, nCountLayouts);
		
	CStringWriter oWriter;
	oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
	
	if (Type == 1)
	{
		oWriter.WriteString(L"<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
	}
	else if (Type == 2)
	{
		oWriter.WriteString(L"<p:notesMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
	}
	else if (Type == 3)
	{
		oWriter.WriteString(L"<p:handoutMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
	}
	oWriter.WriteString(L"<p:cSld>");

	if (pTheme->m_bIsBackground)
	{
		WriteBackground(oWriter, oRels, pTheme->m_oBackground);
	}
    oWriter.WriteString(L"<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");

	for (size_t nEl = 0; nEl < pTheme->m_arElements.size(); ++nEl)
	{
		if (isBodyPlaceholder(pTheme->m_arElements[nEl]->m_lPlaceholderType))
			pTheme->m_arElements[nEl]->m_lPlaceholderType =100; //body тип прописывать !!

		if (pTheme->m_arElements[nEl]->m_bBoundsEnabled == false)
			continue;
		
		WriteElement(oWriter, oRels, pTheme->m_arElements[nEl]);
	}

	oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

    std::wstring strOverrideColorScheme = _T("<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
	oWriter.WriteString(strOverrideColorScheme);

	if (Type == 1)
	{	
		oWriter.WriteString(std::wstring(L"<p:sldLayoutIdLst>"));

		size_t __nCountLayouts = 0;
		for (int nIndexLayout = 0; nIndexLayout < nCountLayouts; ++nIndexLayout)
		{
            oWriter.WriteString(L"<p:sldLayoutId id=\"" + std::to_wstring(0x80000000 + nIndexTheme + 1 + nStartLayout + nIndexLayout) + L"\" r:id=\"rId" + std::to_wstring(nIndexLayout + 1) + L"\"/>");

			WriteLayout(pTheme->m_arLayouts[nIndexLayout], nIndexLayout, nStartLayout, nIndexTheme);
		}

		oWriter.WriteString(std::wstring(L"</p:sldLayoutIdLst>"));
	}

	if (pTheme->m_bHasDate || pTheme->m_bHasFooter || pTheme->m_bHasSlideNumber)
	{
		oWriter.WriteString(std::wstring(L"<p:hf"));
		if (!pTheme->m_bHasDate)		oWriter.WriteString(std::wstring(L" dt=\"0\""));
		if (!pTheme->m_bHasSlideNumber) oWriter.WriteString(std::wstring(L" sldNum=\"0\""));
										oWriter.WriteString(std::wstring(L" hdr=\"0\""));
		if (!pTheme->m_bHasFooter)		oWriter.WriteString(std::wstring(L" ftr=\"0\""));
		oWriter.WriteString(std::wstring(L"/>"));
	}
	if (Type == 1)
	{	
		oWriter.WriteString(std::wstring(L"<p:txStyles>"));

		oWriter.WriteString(std::wstring(L"<p:titleStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[1], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:titleStyle>"));

		oWriter.WriteString(std::wstring(L"<p:bodyStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[2], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:bodyStyle>"));

		oWriter.WriteString(std::wstring(L"<p:otherStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[3], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:otherStyle>"));
		
		oWriter.WriteString(std::wstring(L"</p:txStyles>"));
	}
	else if (Type == 2)
	{	
		oWriter.WriteString(std::wstring(L"<p:notesStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[1], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:notesStyle>"));
	}

	std::wstring strSlideMasterFile;
	std::wstring strSlideMasterRelsFile;
	if (Type == 1)
	{
		oWriter.WriteString(std::wstring(L"</p:sldMaster>"));
		
		strSlideMasterFile = L"slideMaster" + std::to_wstring(nIndexTheme + 1) + L".xml";
		strSlideMasterFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;
		
		strSlideMasterRelsFile = L"slideMaster" + std::to_wstring(nIndexTheme + 1) + L".xml.rels";
		strSlideMasterRelsFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
	}
	else if (Type == 2)
	{
		oWriter.WriteString(std::wstring(L"</p:notesMaster>"));
	
		strSlideMasterFile = L"notesMaster1.xml";
		strSlideMasterFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;
		
		strSlideMasterRelsFile = L"notesMaster1.xml.rels";
		strSlideMasterRelsFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
	}
	else if (Type == 3)
	{
		oWriter.WriteString(std::wstring(L"</p:handoutMaster>"));
	
		strSlideMasterFile = L"handoutMaster1.xml";
		strSlideMasterFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;
		
		strSlideMasterRelsFile = L"handoutMaster1.xml.rels";
		strSlideMasterRelsFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
	}
	oFile.CreateFile(strSlideMasterFile);
		std::wstring strMaster = oWriter.GetData();
		oFile.WriteStringUTF8(strMaster);
	oFile.CloseFile();

	oRels.CloseRels();
	oRels.SaveRels(strSlideMasterRelsFile);				

	nStartLayout += nCountLayouts;
	nIndexTheme++;
}

void NSPresentationEditor::CPPTXWriter::WriteColorScheme(CStringWriter& oStringWriter, const std::wstring & name, const std::vector<CColor> & colors, bool extra)
{
	if (colors.size() < 1)
	{
        oStringWriter.WriteString(L"<a:clrScheme name=\"Default\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/>\
		</a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"1F497D\"/></a:dk2><a:lt2>\
		<a:srgbClr val=\"EEECE1\"/></a:lt2><a:accent1><a:srgbClr val=\"4F81BD\"/></a:accent1><a:accent2><a:srgbClr val=\"C0504D\"/>\
		</a:accent2><a:accent3><a:srgbClr val=\"9BBB59\"/></a:accent3><a:accent4><a:srgbClr val=\"8064A2\"/></a:accent4><a:accent5>\
		<a:srgbClr val=\"4BACC6\"/></a:accent5><a:accent6><a:srgbClr val=\"F79646\"/></a:accent6><a:hlink><a:srgbClr val=\"0000FF\"/>\
        </a:hlink><a:folHlink><a:srgbClr val=\"800080\"/></a:folHlink></a:clrScheme>");
		return;
	}

	if (extra)
        oStringWriter.WriteString(L"<a:extraClrScheme>");

    oStringWriter.WriteString(L"<a:clrScheme name=\"");
	
	oStringWriter.WriteStringXML(name);
    oStringWriter.WriteString(L"\">");
	
    oStringWriter.WriteString(L"<a:dk1><a:srgbClr val=\"" + XmlUtils::IntToString(colors[14].GetLONG_RGB(), L"%06X") + L"\"/></a:dk1>");
	
    oStringWriter.WriteString(L"<a:lt1><a:srgbClr val=\"" + XmlUtils::IntToString(colors[13].GetLONG_RGB(), L"%06X") + L"\"/></a:lt1>");
	
    oStringWriter.WriteString(L"<a:dk2><a:srgbClr val=\"" + XmlUtils::IntToString(colors[16].GetLONG_RGB(), L"%06X") + L"\"/></a:dk2>");
	
    oStringWriter.WriteString(L"<a:lt2><a:srgbClr val=\"" + XmlUtils::IntToString(colors[15].GetLONG_RGB(), L"%06X") + L"\"/></a:lt2>");
	
    oStringWriter.WriteString(L"<a:accent1><a:srgbClr val=\"" + XmlUtils::IntToString(colors[5].GetLONG_RGB(), L"%06X") + L"\"/></a:accent1>");
	
    oStringWriter.WriteString(L"<a:accent2><a:srgbClr val=\"" + XmlUtils::IntToString(colors[6].GetLONG_RGB(), L"%06X") + L"\"/></a:accent2>");
	
    oStringWriter.WriteString(L"<a:accent3><a:srgbClr val=\"" + XmlUtils::IntToString(colors[7].GetLONG_RGB(), L"%06X") + L"\"/></a:accent3>");
	
    oStringWriter.WriteString(L"<a:accent4><a:srgbClr val=\"" + XmlUtils::IntToString(colors[8].GetLONG_RGB(), L"%06X") + L"\"/></a:accent4>");
	
    oStringWriter.WriteString(L"<a:accent5><a:srgbClr val=\"" + XmlUtils::IntToString(colors[9].GetLONG_RGB(), L"%06X") + L"\"/></a:accent5>");
	
    oStringWriter.WriteString(L"<a:accent6><a:srgbClr val=\"" + XmlUtils::IntToString(colors[10].GetLONG_RGB(), L"%06X") + L"\"/></a:accent6>");
	
    oStringWriter.WriteString(L"<a:hlink><a:srgbClr val=\"" + XmlUtils::IntToString(colors[11].GetLONG_RGB(), L"%06X") + L"\"/></a:hlink>");
	
    oStringWriter.WriteString(L"<a:folHlink><a:srgbClr val=\"" + XmlUtils::IntToString(colors[12].GetLONG_RGB(), L"%06X") + L"\"/></a:folHlink>");

    oStringWriter.WriteString(L"</a:clrScheme>");
	if (extra)
	{
        oStringWriter.WriteString(L"<a:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" \
accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
        oStringWriter.WriteString(L"</a:extraClrScheme>");
	}
}

void NSPresentationEditor::CPPTXWriter::WriteBackground(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground)
{
	oWriter.WriteString(std::wstring(L"<p:bg><p:bgPr>"));

	m_pShapeWriter->SetRelsGenerator(&oRels);
	{
		oWriter.WriteString(m_pShapeWriter->ConvertBrush(oBackground));
	}
	oWriter.WriteString(std::wstring(L"</p:bgPr></p:bg>"));
}

void NSPresentationEditor::CPPTXWriter::WriteElement(CStringWriter& oWriter, CRelsGenerator& oRels, IElement* pElement, CLayout* pLayout)
{
	if (!pElement) return;

	pElement->m_oMetric = m_pDocument->m_oInfo;
	pElement->NormalizeCoordsByMetric();

	bool bObject = m_pShapeWriter->SetElement(pElement);

	if (bObject)
	{
		m_pShapeWriter->SetRelsGenerator(&oRels);

		if (NULL != pLayout)
		{
			if (-1 != pElement->m_lPlaceholderType)
			{
				size_t nCountElements = pLayout->m_arElements.size();
				for (size_t nIndex = 0; nIndex < nCountElements; ++nIndex)
				{
					if ((pElement->m_lPlaceholderType == pLayout->m_arElements[nIndex]->m_lPlaceholderType) &&
						(pElement->m_lPlaceholderID == pLayout->m_arElements[nIndex]->m_lPlaceholderID))
					{
						IElement* pElLayout = pLayout->m_arElements[nIndex];
						
						bool bIsEqualTransform = ((pElement->m_dRotate == pElLayout->m_dRotate) 
							&& (pElement->m_bFlipH == pElLayout->m_bFlipH) && (pElement->m_bFlipV == pElLayout->m_bFlipV));

						if (bIsEqualTransform)
						{
							if (pElement->m_rcBounds.IsEqual(pElLayout->m_rcBounds, 0.5))
								pElement->m_bBoundsEnabled = false;
						}

						break;
					}
				}
			}
		}
		oWriter.WriteString(m_pShapeWriter->ConvertShape());
	}
}

void NSPresentationEditor::CPPTXWriter::WriteLayout(CLayout& oLayout, int nIndexLayout, int nStartLayout, int nIndexTheme)
{
	CStringWriter oWriter;
	
	CRelsGenerator oRels(&m_oManager);
	oRels.StartLayout(nIndexTheme);

	oWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

    oWriter.WriteString(std::wstring(L"<p:sldLayout xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\""));
    oWriter.WriteString(std::wstring(L" type=\"") + oLayout.m_strLayoutType + _T("\""));
    oWriter.WriteString(std::wstring(L" showMasterSp=\"") + (oLayout.m_bShowMasterShapes ? _T("1") : _T("0")));
    oWriter.WriteString(std::wstring(L"\" preserve=\"1\"><p:cSld"));

	if ( oLayout.m_sName.empty() == false)
		oWriter.WriteString(std::wstring(L" name=\"") + oLayout.m_sName + std::wstring(L"\""));
	 oWriter.WriteString(std::wstring(L">"));

	if (oLayout.m_bIsBackground)
	{
		WriteBackground(oWriter, oRels, oLayout.m_oBackground);
	}

    std::wstring strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
	oWriter.WriteString(strElems);

	size_t nElements = oLayout.m_arElements.size();
	for (size_t nEl = 0; nEl < nElements; ++nEl)
	{
		WriteElement(oWriter, oRels,  oLayout.m_arElements[nEl]);
	}

	oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

	oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
	oWriter.WriteString(std::wstring(L"</p:sldLayout>"));

	oRels.CloseRels();

    std::wstring strXml = oWriter.GetData();
    std::wstring strFile = L"slideLayout" + std::to_wstring(nIndexLayout + nStartLayout + 1) + L".xml";
	
	CFile oFile;
    std::wstring strFileLayoutPath= m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slideLayouts") + FILE_SEPARATOR_STR;
	oFile.CreateFile(strFileLayoutPath  + strFile);
	oFile.WriteStringUTF8(strXml);
	oFile.CloseFile();

    strFile = L"slideLayout" + std::to_wstring(nIndexLayout + nStartLayout + 1) + L".xml.rels";
	oRels.SaveRels(strFileLayoutPath + _T("_rels") + FILE_SEPARATOR_STR + strFile);			
}
void NSPresentationEditor::CPPTXWriter::WriteSlide(int nIndexSlide)
{
	CStringWriter oWriter;
	CRelsGenerator oRels(&m_oManager);
	
	CSlide* pSlide = m_pDocument->m_arSlides[nIndexSlide];
	
	if (0 == pSlide->m_lThemeID)
		oRels.StartSlide(pSlide->m_lLayoutID, pSlide->m_lNotesID);
	else
	{
		int nLayout = pSlide->m_lLayoutID;
		for (int i = 0; i < pSlide->m_lThemeID; ++i)
			nLayout += (int)m_pDocument->m_arThemes[i].m_arLayouts.size();

		oRels.StartSlide(nLayout, pSlide->m_lNotesID);
	}

	oWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
	oWriter.WriteString(std::wstring(L"<p:sld xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\""));
	if (!pSlide->m_bShowMasterShapes)
		oWriter.WriteString(std::wstring(L" showMasterSp=\"0\""));
	oWriter.WriteString(std::wstring(L">"));
	
	oWriter.WriteString(std::wstring(L"<p:cSld"));

	if ( pSlide->m_sName.empty() == false)
		oWriter.WriteString(std::wstring(L" name=\"") + pSlide->m_sName + std::wstring(L"\""));

	oWriter.WriteString(std::wstring(L">"));

	if (pSlide->m_bIsBackground)
	{
		WriteBackground(oWriter, oRels, pSlide->m_oBackground);
	}

	oWriter.WriteString(std::wstring(L"<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>"));

	for (size_t nEl = 0; nEl < pSlide->m_arElements.size(); ++nEl)
	{
		WriteElement(oWriter, oRels,  pSlide->m_arElements[nEl], &m_pDocument->m_arThemes[pSlide->m_lThemeID].m_arLayouts[pSlide->m_lLayoutID]);
	}
	oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

	oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
	
	WriteTransition(oWriter, pSlide->m_oSlideShow.m_oTransition);

	oWriter.WriteString(std::wstring(L"<p:timing><p:tnLst><p:par><p:cTn id=\"1\" dur=\"indefinite\" restart=\"never\" nodeType=\"tmRoot\" /></p:par></p:tnLst></p:timing>"));
	
	oWriter.WriteString(std::wstring(L"</p:sld>"));

	oRels.CloseRels();

    std::wstring strXml = oWriter.GetData();
    std::wstring strFile = L"slide" + std::to_wstring(nIndexSlide + 1) + L".xml";
    std::wstring strFileSlidePath= m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slides")  + FILE_SEPARATOR_STR;

	CFile oFile;
	oFile.CreateFile(strFileSlidePath + strFile);
	oFile.WriteStringUTF8(strXml);
	oFile.CloseFile();

    strFile = L"slide" + std::to_wstring(nIndexSlide + 1) + L".xml.rels";
	oRels.SaveRels(strFileSlidePath + _T("_rels") + FILE_SEPARATOR_STR + strFile);
}

void NSPresentationEditor::CPPTXWriter::WriteTransition(CStringWriter& oWriter, CTransition& transition)
{
	if (!transition.m_nEffectType == 0xFF)	return;

	std::wstring type;
	
	std::wstring param_name, param_value;
	std::wstring param_name2, param_value2;
	
	switch(transition.m_nEffectType)
	{
	case 0:
		{
			type		= L"p:cut";		
			param_name	= L"thruBlk";
			param_value = transition.m_nEffectDirection ? L"true" : L"false";
		}break;
	case 1:
		{
			type		= L"p:random";		
		}break;
	case 2:	
		{
			type		= L"p:blinds";		
			param_name	= L"dir";
			param_value = transition.m_nEffectDirection ? L"vert" : L"horz";
		}break;
	case 3:	
		{
			type		= L"p:checker";		
			param_name	= L"dir";
			param_value = transition.m_nEffectDirection ? L"vert" : L"horz";
		}break;
	case 5:
		{
			type		= L"p:dissolve";		
			param_name	= L"thruBlk";
			param_value = transition.m_nEffectDirection ? L"true" : L"false";
		}break;
	case 6:
		{
			type		= L"p:fade";		
			param_name	= L"thruBlk";
			param_value = transition.m_nEffectDirection ? L"true" : L"false";
		}break;
	case 4:
	case 7:
		{
			if (transition.m_nEffectType == 4)	type = L"p:cover";		
			if (transition.m_nEffectType == 7)	type = L"p:pull";		
			param_name	= L"dir";
			switch(transition.m_nEffectDirection)
			{
				case 0:	param_value = L"r"; break;
				case 1:	param_value = L"b"; break;
				case 2:	param_value = L"l"; break;
				case 3:	param_value = L"t"; break;
				case 4:	param_value = L"br"; break;
				case 5:	param_value = L"bl"; break;
				case 6:	param_value = L"tr"; break;
				case 7:	param_value = L"tl"; break;
			}
		}break;
	case 8:
		{
			type		= L"p:randomBars";		
			param_name	= L"dir";
			param_value = transition.m_nEffectDirection ? L"vert" : L"horz";
		}break;
	case 9:
		{
			type		= L"p:strips";		
			param_name	= L"dir";
			switch(transition.m_nEffectDirection)
			{
				case 0:	param_value = L"ru"; break;
				case 1:	param_value = L"lu"; break;
				case 2:	param_value = L"rd"; break;
				case 3:	param_value = L"ld"; break;
			}
		}break;
	case 10:
	case 20:
		{
			if (transition.m_nEffectType == 10)	type = L"p:wipe";		
			if (transition.m_nEffectType == 20)	type = L"p:push";		
		
			param_name	= L"dir";
			switch(transition.m_nEffectDirection)
			{
				case 0:	param_value = L"l"; break;
				case 1:	param_value = L"u"; break;
				case 2:	param_value = L"r"; break;
				case 3:	param_value = L"d"; break;
			}
		}break;	
	case 11:
		{
			type		= L"p:zoom";		
			param_name	= L"dir";
			param_value = transition.m_nEffectDirection ? L"in" : L"out";
		}break;
	case 13:
		{
			type		= L"p:split";		
			param_name	= L"dir";
			param_name2	= L"orient";
			switch(transition.m_nEffectDirection)
			{
				case 0:	param_value = L"horz";	param_value2 = L"out";	break;
				case 1:	param_value = L"horz";	param_value2 = L"in";	break;
				case 2:	param_value = L"vert";	param_value2 = L"out";	break;
				case 3:	param_value = L"vert";	param_value2 = L"in";	break;
			}
		}break;
	case 17:
		{
			type = L"p:diamond";		
		}break;
	case 18:
		{
			type = L"p:plus";		
		}break;
	case 19:
		{
			type = L"p:wedge";		
		}break;

	case 21:
		{
			type		= L"p:comb";		
			param_name	= L"dir";
			param_value = transition.m_nEffectDirection ? L"vert" : L"horz";
		}break;
	case 22:
		{
			type = L"p:newsflash";		
		}break;
	case 23:
		{
			type = L"p:alphaFade";		
		}break;
	case 26:
		{
			type		= L"p:wheel";		
			param_name	= L"spokes";
			param_value = std::to_wstring(transition.m_nEffectDirection);
		}break;
	case 27:
		{
			type = L"p:circle";		
		}break;
	default:
		break;
	}

	if (type.empty()) return;
	oWriter.WriteString(std::wstring(L"<p:transition"));	
		switch (transition.m_nSpeed)
		{
		case 0x00:	oWriter.WriteString(L" spd=\"fast\"");	break;
		case 0x01:	oWriter.WriteString(L" spd=\"med\"");	break;
		case 0x02:	
		default:	oWriter.WriteString(L" spd=\"slow\"");	break;
		}
	oWriter.WriteString(L">");
	oWriter.WriteString(L"<" + type);
		if (!param_name.empty() && !param_value.empty())
		{
			oWriter.WriteString(L" " + param_name + L"=\"" + param_value + L"\"");
		}
		if (!param_name2.empty() && !param_value2.empty())
		{
			oWriter.WriteString(L" " + param_name2 + L"=\"" + param_value2 + L"\"");
		}
	oWriter.WriteString(L"/>");
	if (transition.m_bAudioPresent)
	{
		std::wstring rId = m_pShapeWriter->m_pRels->WriteAudio(transition.m_oAudio.m_strAudioFileName);
		oWriter.WriteString(std::wstring(L"<p:sndAc><p:stSnd>"));
		oWriter.WriteString(L"<p:snd r:embed=\"" + rId + L"\" name=\"" + transition.m_oAudio.m_sImageName + L"\"/>");
		oWriter.WriteString(std::wstring(L"</p:stSnd></p:sndAc>"));
	}
	oWriter.WriteString(std::wstring(L"</p:transition>"));
}

void NSPresentationEditor::CPPTXWriter::WriteNotes(int nIndexNotes)
{
	CStringWriter oWriter;
	CRelsGenerator oRels(&m_oManager);
	
	CSlide* pNotes = m_pDocument->m_arNotes[nIndexNotes];
	
	oRels.StartNotes(pNotes->m_lSlideID, m_pDocument->m_pNotesMaster != NULL);

	oWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
	oWriter.WriteString(std::wstring(L"<p:notes xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\""));
	if (!pNotes->m_bShowMasterShapes)
		oWriter.WriteString(std::wstring(L" showMasterSp=\"0\""));
	oWriter.WriteString(std::wstring(L">"));
	
	oWriter.WriteString(std::wstring(L"<p:cSld>"));

	if (pNotes->m_bIsBackground)
	{
		WriteBackground(oWriter, oRels, pNotes->m_oBackground);
	}

	oWriter.WriteString(std::wstring(L"<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>"));

	for (size_t nEl = 0; nEl < pNotes->m_arElements.size(); ++nEl)
	{
		WriteElement(oWriter, oRels,  pNotes->m_arElements[nEl], NULL);
	}
	oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

	oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
	oWriter.WriteString(std::wstring(L"</p:notes>"));

	oRels.CloseRels();

    std::wstring strXml = oWriter.GetData();
    std::wstring strFile = L"notesSlide" + std::to_wstring(nIndexNotes + 1) + L".xml";
    std::wstring strFileSlidePath = m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("notesSlides")  + FILE_SEPARATOR_STR;

	CFile oFile;
	oFile.CreateFile(strFileSlidePath + strFile);
	oFile.WriteStringUTF8(strXml);
	oFile.CloseFile();

    strFile = L"notesSlide" + std::to_wstring(nIndexNotes + 1) + L".xml.rels";
	oRels.SaveRels(strFileSlidePath + _T("_rels") + FILE_SEPARATOR_STR + strFile);
}

void NSPresentationEditor::CPPTXWriter::WriteSlides()
{
	for (size_t nIndexS = 0; nIndexS < m_pDocument->m_arSlides.size(); ++nIndexS)
	{
		WriteSlide((int)nIndexS);
	}
}
void NSPresentationEditor::CPPTXWriter::WriteNotes()
{
	for (size_t nIndexS = 0; nIndexS < m_pDocument->m_arNotes.size(); ++nIndexS)
	{
		WriteNotes((int)nIndexS);
	}
}
