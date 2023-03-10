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
#pragma once

#include "../Drawing/Document.h"

#include "../../../OOXML/Binary/Presentation/DefaultNotesMaster.h"
#include "../../../OOXML/Binary/Presentation/DefaultNotesTheme.h"

#include "../../../OOXML/PPTXFormat/NotesMaster.h"
#include "../../../OOXML/PPTXFormat/SlideMaster.h"
#include "../../../OOXML/PPTXFormat/HandoutMaster.h"

#include "TableWriter.h"
#include "../Reader/PPTDocumentInfo.h"

#include "ShapeWriter.h"
#include "StylesWriter.h"

#include "Converter.h"
#include "../Converter/timing.h"
#include "../Converter/Animation/AnimationParser.h"
#include "../Converter/transition.h"

#include "../../Common/Utils/simple_xml_writer.h"

#include "../Reader/RoundTripExtractor.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/regex.hpp>

typedef boost::uuids::detail::md5 MD5;

namespace PPT
{
	static std::string md5(const BYTE* pData, const ULONG dataLen)
	{
		std::string strHash;
		boost::uuids::detail::md5 hash;
		boost::uuids::detail::md5::digest_type digest;

		hash.process_bytes(pData, dataLen);
		hash.get_digest(digest);

		const auto charDigest = reinterpret_cast<const char *>(&digest);
		boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type),
			std::back_inserter(strHash));

		return strHash;
	}
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
        <cp:revision>1</cp:revision>\
        </cp:coreProperties>");
}


CPPTXWriter::CPPTXWriter()
{
    m_strTempDirectory	= NSDirectory::GetTempPath() + FILE_SEPARATOR_STR + _T("TempPPTX");
    m_strDstFileName	= NSDirectory::GetTempPath() + FILE_SEPARATOR_STR + _T("Test.pptx");

    m_pDocument			= NULL;
    m_pUserInfo			= NULL;

    m_pShapeWriter = new CShapeWriter();
}

CPPTXWriter::~CPPTXWriter()
{
    RELEASEOBJECT(m_pShapeWriter);
}

void CPPTXWriter::CreateFile(CPPTUserInfo* pUserInfo)
{
    m_pUserInfo = pUserInfo;

    m_pDocument = dynamic_cast<CDocument*>(pUserInfo);

    m_oManager.Clear();
    m_oManager.SetDstMedia(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR);

    m_pShapeWriter->InitNextId();

    NSDirectory::CreateDirectory(m_strTempDirectory);
    NSFile::CFileBinary oFile;
    std::wstring strMemory = _T("");

    // _rels
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels"));

    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels")  + FILE_SEPARATOR_STR + _T(".rels"));
    strMemory = NSPPTXWriterConst::g_string_rels_presentation;
    oFile.WriteStringUTF8(strMemory);
    oFile.CloseFile();

    // docProps
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps"));

    // core
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml"));
    if (m_xmlCore.empty())
		m_xmlCore = NSPPTXWriterConst::g_string_core;
    oFile.WriteStringUTF8(m_xmlCore);
    oFile.CloseFile();

    // app
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml"));
    WriteApp(oFile);
    oFile.CloseFile();

    // content types
    WriteContentTypes();

    // ppt
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt"));
    WritePresInfo();

    WriteAll();
}

void CPPTXWriter::CreateFile(CDocument* pDocument)
{
    m_pDocument = pDocument;
    m_oManager.Clear();
    m_oManager.SetDstMedia(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR);

    m_pShapeWriter->InitNextId();

    NSDirectory::CreateDirectory(m_strTempDirectory);
    NSFile::CFileBinary oFile;
    std::wstring strMemory = _T("");

    // _rels
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels"));

    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels")  + FILE_SEPARATOR_STR + _T(".rels"));
    strMemory = NSPPTXWriterConst::g_string_rels_presentation;
    oFile.WriteStringUTF8(strMemory);
    oFile.CloseFile();

    // docProps
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps"));

    // core
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml"));
    strMemory = NSPPTXWriterConst::g_string_core;
    oFile.WriteStringUTF8(strMemory);
    oFile.CloseFile();

    // app
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml"));
    WriteApp(oFile);
    oFile.CloseFile();

    // content types
    WriteContentTypes();

    // ppt
    NSDirectory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt"));
    WritePresInfo();

    WriteAll();
}

void CPPTXWriter::CloseFile()
{
    m_oManager.Clear();
}


void CPPTXWriter::WriteContentTypes()
{
    std::wstring strContentTypes = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
            <Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\
            <Default Extension=\"bmp\" ContentType=\"image/bmp\"/>\
            <Default Extension=\"png\" ContentType=\"image/png\"/>\
            <Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/>\
            <Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/>\
            <Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>\
            <Default Extension=\"xml\" ContentType=\"application/xml\"/>\
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
            <Default Extension=\"jpg\" ContentType=\"image/jpeg\"/>";

    if (m_pDocument->m_bMacros)
    {
        strContentTypes += L"<Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml\" />\
                <Override PartName=\"/ppt/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\" />";
    }
    else
    {
        strContentTypes += L"<Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" />";
    }
    strContentTypes += L"<Override PartName=\"/ppt/presProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" />\
            <Override PartName=\"/ppt/viewProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml\" />\
            <Override PartName=\"/ppt/tableStyles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml\"/>\
            <Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\" />\
            <Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\" />";

    int nIndexLayout = 1, nIndexTheme = 1;

    for (size_t nT = 0; nT < m_pDocument->m_arThemes.size(); nT++, nIndexTheme++)
    {
        strContentTypes += L"<Override PartName=\"/ppt/theme/theme" + std::to_wstring(nIndexTheme) + \
                L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\" />";

        strContentTypes += L"<Override PartName=\"/ppt/slideMasters/slideMaster" + std::to_wstring(nIndexTheme) + \
                L".xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\"/>";


        for (size_t nL = 0; nL < m_pDocument->m_arThemes[nT]->m_arLayouts.size(); nL++, nIndexLayout++)
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

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("[Content_Types].xml"));
    oFile.WriteStringUTF8(strContentTypes);
    oFile.CloseFile();
}


void CPPTXWriter::WriteApp(NSFile::CFileBinary& oFile)
{
	if (m_xmlApp.empty())
	{
		std::wstringstream strm;

		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"Properties")
			{
				CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties");
				CP_XML_ATTR(L"xmlns:vt", L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes");

				CP_XML_NODE(L"TotalTime")
				{
					CP_XML_STREAM() << 0;
				}
				CP_XML_NODE(L"Words")
				{
					CP_XML_STREAM() << 0;
				}
				std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
				if (sApplication.empty())
					sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
				std::string s = VALUE2STR(INTVER);
				sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif
				CP_XML_NODE(L"Application")
				{
					CP_XML_STREAM() << sApplication;
				}
				//CP_XML_NODE(L"AppVersion")
				//{
				//	CP_XML_STREAM() << L"1.0";
				//}
				CP_XML_NODE(L"Paragraphs")
				{
					CP_XML_STREAM() << 0;
				}
				CP_XML_NODE(L"PresentationFormat")
				{
					CP_XML_STREAM() << L"On-screen Show (4:3)";
				}
				CP_XML_NODE(L"Slides")
				{
					CP_XML_STREAM() << m_pDocument->m_arSlides.size();
				}
				CP_XML_NODE(L"Notes")
				{
					CP_XML_STREAM() << m_pDocument->m_arNotes.size();
				}
				CP_XML_NODE(L"HiddenSlides")
				{
					CP_XML_STREAM() << 0;
				}
				CP_XML_NODE(L"MMClips")
				{
					CP_XML_STREAM() << 0;
				}
				CP_XML_NODE(L"ScaleCrop")
				{
					CP_XML_STREAM() << L"false";
				}
				CP_XML_NODE(L"HeadingPairs")
				{
					CP_XML_NODE(L"vt:vector")
					{
						CP_XML_ATTR(L"size", 4);
						CP_XML_ATTR(L"baseType", L"variant");

						CP_XML_NODE(L"vt:variant")
						{
							CP_XML_NODE(L"vt:lpstr")
							{
								CP_XML_STREAM() << L"Theme";
							}
						}
						CP_XML_NODE(L"vt:variant")
						{
							CP_XML_NODE(L"vt:i4")
							{
								CP_XML_STREAM() << m_pDocument->m_arThemes.size();
							}
						}
						CP_XML_NODE(L"vt:variant")
						{
							CP_XML_NODE(L"vt:lpstr")
							{
								CP_XML_STREAM() << L"Slide Titles";
							}
						}
						CP_XML_NODE(L"vt:variant")
						{
							CP_XML_NODE(L"vt:i4")
								CP_XML_STREAM() << m_pDocument->m_arSlides.size();
						}
					}
				}
				CP_XML_NODE(L"TitlesOfParts")
				{
					CP_XML_NODE(L"vt:vector")
					{
						CP_XML_ATTR(L"size", m_pDocument->m_arSlides.size() + m_pDocument->m_arThemes.size());
						CP_XML_ATTR(L"baseType", L"lpstr");

						for (size_t i = 1; i <= m_pDocument->m_arThemes.size(); ++i)
						{
							CP_XML_NODE(L"vt:lpstr")
							{
								CP_XML_STREAM() << L"Theme " << i;
							}
						}
						for (size_t i = 1; i <= m_pDocument->m_arSlides.size(); ++i)
						{
							CP_XML_NODE(L"vt:lpstr")
							{
								CP_XML_STREAM() << L"Slide " << i;
							}
						}
					}
				}

				//CP_XML_NODE(L"Company");
				CP_XML_NODE(L"LinksUpToDate")
				{
					CP_XML_STREAM() << L"false";
				}
				CP_XML_NODE(L"SharedDoc")
				{
					CP_XML_STREAM() << L"false";
				}
				CP_XML_NODE(L"HyperlinksChanged")
				{
					CP_XML_STREAM() << L"false";
				}
			}
		}

		m_xmlApp = strm.str();
	}
    oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
    oFile.WriteStringUTF8(m_xmlApp);
}

void CPPTXWriter::WritePresInfo()
{
    NSFile::CFileBinary oFile;

    // tableStyles.xml
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt")  + FILE_SEPARATOR_STR + _T("tableStyles.xml"));

    oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                          <a:tblStyleLst xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" def=\"{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}\"/>");
    oFile.CloseFile();

    // presProps.xml
    oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("presProps.xml"));
    oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                          <p:presentationPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
            <p:extLst><p:ext uri=\"{E76CE94A-603C-4142-B9EB-6D1370010A27}\">\
            <p14:discardImageEditData xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"0\" /></p:ext>\
            <p:ext uri=\"{D31A062A-798A-4329-ABDD-BBA856620510}\">\
            <p14:defaultImageDpi xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"220\" />\
            </p:ext></p:extLst></p:presentationPr>");
            oFile.CloseFile();

            // viewProps.xml
            oFile.CreateFileW(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("viewProps.xml"));
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

        nCountLayouts += m_pDocument->m_arThemes[nIndexTheme]->m_arLayouts.size();
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

    if (m_pDocument->m_bMacros)
    {
        std::wstring strVbaProject = m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt")  + FILE_SEPARATOR_STR + _T("vbaProject.bin");

        if (CDirectory::CopyFile(m_pDocument->m_sVbaProjectFile, strVbaProject))
        {
            strPresRels += L"<Relationship Id=\"rId" + std::to_wstring(nCurrentRels++) + L"\" Type=\"http://schemas.microsoft.com/office/2006/relationships/vbaProject\" Target=\"vbaProject.bin\"/>";
        }
    }
    strPresRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
            + strPresRels + L"</Relationships>";

    std::wstring strPptRels = m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("_rels");

    NSDirectory::CreateDirectory(strPptRels);
    oFile.CreateFileW(strPptRels + FILE_SEPARATOR_STR + _T("presentation.xml.rels"));
    oFile.WriteStringUTF8(strPresRels);
    oFile.CloseFile();

    std::wstring strSizePres = L"<p:sldSz cx=\"" + std::to_wstring(m_pDocument->m_lSlideWidth) +
            L"\" cy=\"" + std::to_wstring(m_pDocument->m_lSlideHeight) +
            L"\" type=\"screen4x3\" /><p:notesSz cx=\"" + std::to_wstring(m_pDocument->m_lNotesWidth) +
            L"\" cy=\"" + std::to_wstring(m_pDocument->m_lNotesHeight) + L"\"/>";

    std::wstring strDefaultTextStyle = _T("<p:defaultTextStyle>");

    if (false == m_pDocument->m_arThemes.empty())
    {
        CStylesWriter styleWriter(m_pDocument->m_arThemes[0].get());
        strDefaultTextStyle += styleWriter.ConvertStyles(m_pDocument->m_arThemes[0]->m_pStyles[0], 9);
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

    oFile.CreateFileW(m_strTempDirectory+ FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR + _T("presentation.xml"));
    oFile.WriteStringUTF8(strPres);
    oFile.CloseFile();
}

void CPPTXWriter::WriteAll()
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

// todo reforming and refactoring!
void CPPTXWriter::WriteThemes()
{
    int nStartLayout = 0, nIndexTheme = 0;

    if (HasRoundTrips() && m_pDocument->m_arThemes.size())
    {
        std::unordered_set<std::string> writedFilesHash;
        for (const auto& oIterSlide : m_pUserInfo->m_mapMasters)
        {
            WriteRoundTripTheme(oIterSlide.second, writedFilesHash, nIndexTheme, nStartLayout);
        }

        writedFilesHash.clear();
        for (const auto& oIterSlide : m_pUserInfo->m_mapNotesMasters)
            WriteRoundTripTheme(oIterSlide.second, writedFilesHash, nIndexTheme, nStartLayout);
        writedFilesHash.clear();
        for (const auto& oIterSlide : m_pUserInfo->m_mapHandoutMasters)
            WriteRoundTripTheme(oIterSlide.second, writedFilesHash, nIndexTheme, nStartLayout);
    }
    else
    {
        for (size_t i = 0; i < m_pDocument->m_arThemes.size(); i++)
        {
            m_pShapeWriter->m_pTheme = m_pDocument->m_arThemes[i].get();
            WriteTheme(m_pDocument->m_arThemes[i], nIndexTheme, nStartLayout);
            m_pShapeWriter->m_pTheme = NULL;
        }

        WriteTheme(m_pDocument->m_pNotesMaster, nIndexTheme, nStartLayout);
        WriteTheme(m_pDocument->m_pHandoutMaster, nIndexTheme, nStartLayout);
    }
}

bool CPPTXWriter::HasRoundTrips() const
{
    if (m_pUserInfo == nullptr || m_pUserInfo->m_mapMasters.empty() || m_pUserInfo->m_mapMasters.begin()->second == nullptr)
        return false;

    std::vector<RoundTripTheme12Atom*> arrRTTheme;
    std::vector<RoundTripContentMasterInfo12Atom*> arrRTLayouts;
    std::vector<RoundTripNotesMasterTextStyles12Atom*> arrRTNotes;
    auto pSlide = m_pUserInfo->m_mapMasters.begin()->second;
    pSlide->GetRecordsByType(&arrRTLayouts, false, false);
    pSlide->GetRecordsByType(&arrRTTheme, false, true);
    pSlide->GetRecordsByType(&arrRTNotes, false, true);

    if (m_pDocument->m_pNotesMaster && arrRTNotes.empty())
        return false;

    if (m_pDocument->m_pHandoutMaster)
    {
        for (const auto& oIterSlide : m_pUserInfo->m_mapHandoutMasters)
        {
            std::vector<RoundTripTheme12Atom*> arrRTTheme;
            oIterSlide.second->GetRecordsByType(&arrRTTheme, false, true);
            if (arrRTTheme.empty())
                return false;
        }
    }


    return arrRTTheme.size() && arrRTLayouts.size();
}

bool CPPTXWriter::WriteRoundTripTheme(const CRecordSlide *pSlide, std::unordered_set<std::string>& writedFilesHash, int & nIndexTheme, int & nStartLayout)
{
    if (!pSlide)
        return false;

    // Write Theme
    CRelsGenerator themeRels(&m_oManager);
    std::wstring strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;
    std::wstring strThemeDirectory = strPptDirectory + FILE_SEPARATOR + _T("theme");
    if (nIndexTheme == 0)
        NSDirectory::CreateDirectory(strThemeDirectory);

    std::vector<RoundTripTheme12Atom*> arrRTTheme;
    std::vector<RoundTripColorMapping12Atom*> arrRTColor;
    std::vector<RoundTripContentMasterInfo12Atom*> arrRTLayouts;
    std::vector<RoundTripOArtTextStyles12Atom*> arrRTMaster;
    std::vector<RoundTripNotesMasterTextStyles12Atom*> arrRTNotes;
    pSlide->GetRecordsByType(&arrRTLayouts, false, false);
    pSlide->GetRecordsByType(&arrRTTheme, false, true);
    pSlide->GetRecordsByType(&arrRTColor, false, true);
    pSlide->GetRecordsByType(&arrRTMaster, false, true);
    pSlide->GetRecordsByType(&arrRTNotes, false, true);

    const int oldThemeSize = m_pDocument->m_arThemes.size();
    const unsigned oldThemeIndex = nIndexTheme < oldThemeSize ? nIndexTheme : oldThemeSize-1;
    if (arrRTLayouts.size())
        m_pShapeWriter->m_pTheme = m_pDocument->m_arThemes[oldThemeIndex].get();
    else if (arrRTNotes.size())
        m_pShapeWriter->m_pTheme = m_pDocument->m_pNotesMaster.get();
    else if (m_pDocument->m_pHandoutMaster.get())
        m_pShapeWriter->m_pTheme = m_pDocument->m_pHandoutMaster.get();
    if (m_pShapeWriter->m_pTheme == nullptr)
        return false;

    auto themeType = m_pShapeWriter->m_pTheme->m_eType;

    if (arrRTTheme.empty())
        return false;

    RoundTripExtractor extractor(arrRTTheme[0]);
    std::wstring oneThemePathS = std::wstring (L"theme") + FILE_SEPARATOR_STR;
    std::wstring twoThemePathS = oneThemePathS + std::wstring (L"theme") + FILE_SEPARATOR_STR;
    auto strThemePath = extractor.getOneFile(twoThemePathS + L"theme1.xml");
    if (strThemePath.empty())
        return false;

    auto themeRelsPath = extractor.getOneFile(twoThemePathS + L"_rels" + FILE_SEPARATOR_STR + L"theme1.xml.rels");
    auto arrImagesPaths = extractor.find( L".*image[0-9]+.*");

    BYTE *utf8Data = NULL;
    ULONG utf8DataSize = 0;
    bool wasThemeWrite = false;
    bool needRels = false;

    // read file bytes
    NSFile::CFileBinary::ReadAllBytes(strThemePath, &utf8Data, utf8DataSize);
    // compare hash
    char* pointerToThemeElems = strstr((char*)utf8Data, "<a:themeElements>");
    UINT hashShift = pointerToThemeElems ? pointerToThemeElems - (char*)utf8Data : 0;
   
	auto strHash = md5(utf8Data+hashShift, utf8DataSize-hashShift);

    needRels = (int)std::string(utf8Data, utf8Data + utf8DataSize).find("rId") != -1;

    // cp file with new name or write bytes
    if (writedFilesHash.find(strHash) == writedFilesHash.end())
    {
        std::wstring strThemeFile = L"theme" + std::to_wstring(++nIndexTheme) + L".xml";
        std::wstring outputThemePath = strThemeDirectory + FILE_SEPARATOR_STR + strThemeFile;
        NSFile::CFileBinary file;
        file.CreateFileW(outputThemePath);
        file.WriteFile(utf8Data, utf8DataSize);
        file.CloseFile();

        // clear bytes
        RELEASEOBJECT(utf8Data);
        utf8DataSize = 0;

        writedFilesHash.insert(strHash);
        wasThemeWrite = true;

        for (auto& strImagePath : arrImagesPaths)
        {
            themeRels.WriteImage(strImagePath);
        }
    }

    //    if (wasThemeWrite == false)
    //        return;

    // write theme _rels
    if (!themeRelsPath.empty() && needRels)
    {
        std::wstring relsFolder = strPptDirectory + L"theme" + FILE_SEPARATOR_STR  + L"_rels" + FILE_SEPARATOR_STR;
        std::wstring relsName = L"theme" + std::to_wstring(nIndexTheme) + L".xml.rels";
        NSDirectory::CreateDirectory(relsFolder);

        NSFile::CFileBinary::ReadAllBytes(themeRelsPath, &utf8Data, utf8DataSize);
        NSFile::CFileBinary oFile;
        oFile.CreateFileW(relsFolder + relsName);
        oFile.WriteFile(utf8Data, utf8DataSize);

        oFile.CloseFile();
        RELEASEOBJECT(utf8Data);
        utf8DataSize = 0;
    }

    // get slide master ID
    std::vector<RoundTripOriginalMainMasterId12Atom*> arrMasterID;
    pSlide->GetRecordsByType(&arrMasterID, false, true);

    // Write layouts
    if (themeType == _typeMaster::typeMaster)
    {
        std::wstring strOutputLayoutsPath = strPptDirectory + FILE_SEPARATOR_STR + L"slideLayouts" + FILE_SEPARATOR_STR;
        std::wstring strOutputRelsLayoutsPath = strOutputLayoutsPath + L"_rels" + FILE_SEPARATOR_STR;

        if (nIndexTheme == 1)
        {
            NSDirectory::CreateDirectory(strOutputLayoutsPath);
            NSDirectory::CreateDirectory(strOutputRelsLayoutsPath);
        }
    }

    // Write Masters
    if (wasThemeWrite)
    {
        CStringWriter oWriter;
        CRelsGenerator oRels(&m_oManager);
        int nCountLayouts = m_pShapeWriter->m_pTheme->m_arLayouts.size();
        oRels.StartMaster(nIndexTheme-1, nStartLayout, nCountLayouts);
        auto* pTheme = m_pShapeWriter->m_pTheme;
        oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");

        if (pTheme->m_eType == typeMaster)
        {
            oWriter.WriteString(L"<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
        }
        else if (pTheme->m_eType == typeNotesMaster)
        {
            oWriter.WriteString(L"<p:notesMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
        }
        else if (pTheme->m_eType == typeHandoutMaster)
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

        CGroupElement *pGroupElement = !pTheme->m_arElements.empty() ? dynamic_cast<CGroupElement *>(pTheme->m_arElements[0].get()) : NULL;

        size_t start_index = 0;
        if (pGroupElement)
        {
            for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
            {
                if (isBodyPlaceholder(pGroupElement->m_pChildElements[i]->m_lPlaceholderType))
                    pGroupElement->m_pChildElements[i]->m_lPlaceholderType = 100; //body тип прописывать !!

                //if (pGroupElement->m_pChildElements[i]->m_bAnchorEnabled == false &&
                //	pGroupElement->m_pChildElements[i]->m_bChildAnchorEnabled == false)
                //	continue;

                //if (pTheme->m_eType == typeNotesMaster)
                //{
                //	pGroupElement->m_pChildElements[i]->m_lPlaceholderID = -1;
                //}
                //else if (pTheme->m_eType == typeHandoutMaster)
                //{
                //	pGroupElement->m_pChildElements[i]->m_lPlaceholderID = -1;
                //	pGroupElement->m_pChildElements[i]->m_lPlaceholderSizePreset = -1;
                //}
                WriteElement(oWriter, oRels, pGroupElement->m_pChildElements[i]);
            }

            start_index = 1;
        }

        for (size_t i = start_index; i < pTheme->m_arElements.size(); ++i)
        {
            if (isBodyPlaceholder(pTheme->m_arElements[i]->m_lPlaceholderType))
                pTheme->m_arElements[i]->m_lPlaceholderType = 100; //body тип прописывать !!

            //if (pTheme->m_arElements[i]->m_bAnchorEnabled == false &&
            //	pTheme->m_arElements[i]->m_bChildAnchorEnabled == false)
            //	continue;

            //if (pTheme->m_eType == typeNotesMaster)
            //{
            //	pTheme->m_arElements[i]->m_lPlaceholderID = -1;
            //}
            //else if (pTheme->m_eType == typeHandoutMaster)
            //{
            //	pTheme->m_arElements[i]->m_lPlaceholderID = -1;
            //	pTheme->m_arElements[i]->m_lPlaceholderSizePreset = -1;
            //}

            WriteElement(oWriter, oRels, pTheme->m_arElements[i]);
        }

        oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

        //    std::wstring strOverrideColorScheme = _T("<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
        oWriter.WriteString(arrRTColor[0]->getPClrMap());

        if (pTheme->m_eType == typeMaster)
        {
            oWriter.WriteString(std::wstring(L"<p:sldLayoutIdLst>"));

            for (int nIndexLayout = 0; nIndexLayout < nCountLayouts; ++nIndexLayout)
            {
                const unsigned sldLayoutId = 0x80000000 + nIndexTheme + nStartLayout + nIndexLayout;
                oWriter.WriteString(L"<p:sldLayoutId id=\"" + std::to_wstring(sldLayoutId) + L"\" r:id=\"rId" + std::to_wstring(nIndexLayout + 1) + L"\"/>");

                WriteLayout(pTheme->m_arLayouts[nIndexLayout], nIndexLayout, nStartLayout, nIndexTheme-1);
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
        CStylesWriter styleWriter;
        styleWriter.m_pTheme = pTheme;


        // inserted roundtrip master
        // add media here rId
        if (pTheme->m_eType == typeMaster && arrRTMaster.size())
        {

            RoundTripExtractor extractorMaster(arrRTMaster[0]);
            auto masterPath = extractorMaster.getOneFile(std::wstring(L"drs") + FILE_SEPARATOR_STR + L"slideMasters" + FILE_SEPARATOR_STR + L"slideMaster1.xml");
            auto mediaPathes = extractorMaster.find(L".*image[0-9]+.*");
            int rIdShift = oRels.getRId() - 1;
            for (auto& strMediaPath : mediaPathes)
            {
                oRels.WriteImage(strMediaPath);
            }

            std::wstring utf8strMaster;
            NSFile::CFileBinary::ReadAllTextUtf8(masterPath, utf8strMaster);
            auto txStylesIter = utf8strMaster.find(L"<p:txStyles");
            if (txStylesIter != (UINT)-1)
            {
                auto strTxStyles = utf8strMaster.substr(txStylesIter);

                UINT proccesed = 0;
                UINT rIdIter = 0;
                UINT rIdIterEnd = 0;
                const std::wstring searchStrEmbed = L"<a:blip r:embed=\"rId";
                while (true)
                {
                    rIdIter = strTxStyles.find(searchStrEmbed, proccesed);
                    if (rIdIter == (UINT)-1) break;
                    proccesed = rIdIter + searchStrEmbed.size();
                    rIdIterEnd = strTxStyles.find(L"\"", proccesed);

                    auto strId = strTxStyles.substr(proccesed, rIdIterEnd - proccesed);
                    unsigned numId = std::stoi(strId) + rIdShift;
                    strId = std::to_wstring(numId);

                    strTxStyles.erase(proccesed, rIdIterEnd - proccesed);
                    strTxStyles.insert(proccesed, strId);
                }
                oWriter.WriteString(strTxStyles);
            }
        }
        else if (pTheme->m_eType == typeNotesMaster)
        {
            oWriter.WriteString(std::wstring(L"<p:notesStyle>"));

            RoundTripExtractor extractorNotes(arrRTNotes[0]);
            auto masterPath = extractorNotes.getOneFile(std::wstring(L"drs") + FILE_SEPARATOR_STR + L"slideMasters" + FILE_SEPARATOR_STR + L"slideMaster1.xml");
            std::wstring utf8strNotes;
            NSFile::CFileBinary::ReadAllTextUtf8(masterPath, utf8strNotes);
            auto lvl1pPrIter = utf8strNotes.find(L"<a:lvl1pPr");
            auto bodyStyleIter = utf8strNotes.find(L"</p:bodyStyle>");
            if (lvl1pPrIter != (UINT)-1 && bodyStyleIter != (UINT)-1)
                oWriter.WriteString(utf8strNotes.substr(lvl1pPrIter, bodyStyleIter - lvl1pPrIter));

            oWriter.WriteString(std::wstring(L"</p:notesStyle>"));
        }

        std::wstring strSlideMasterFile;
        std::wstring strSlideMasterRelsFile;
        if (pTheme->m_eType == typeMaster)
        {
            oWriter.WriteString(std::wstring(L"</p:sldMaster>"));

            strSlideMasterFile = L"slideMaster" + std::to_wstring(nIndexTheme) + L".xml";
            strSlideMasterFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

            strSlideMasterRelsFile = L"slideMaster" + std::to_wstring(nIndexTheme) + L".xml.rels";
            strSlideMasterRelsFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
        }
        else if (pTheme->m_eType == typeNotesMaster)
        {
            oWriter.WriteString(std::wstring(L"</p:notesMaster>"));

            strSlideMasterFile = L"notesMaster1.xml";
            strSlideMasterFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

            strSlideMasterRelsFile = L"notesMaster1.xml.rels";
            strSlideMasterRelsFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
        }
        else if (pTheme->m_eType == typeHandoutMaster)
        {
            oWriter.WriteString(std::wstring(L"</p:handoutMaster>"));

            strSlideMasterFile = L"handoutMaster1.xml";
            strSlideMasterFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

            strSlideMasterRelsFile = L"handoutMaster1.xml.rels";
            strSlideMasterRelsFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
        }
        NSFile::CFileBinary oFile;
        oFile.CreateFileW(strSlideMasterFile);
        std::wstring strMaster = oWriter.GetData();
        oFile.WriteStringUTF8(strMaster);
        oFile.CloseFile();

        oRels.CloseRels();
        oRels.SaveRels(strSlideMasterRelsFile);

        nStartLayout += nCountLayouts;
    }

    m_pShapeWriter->m_pTheme = NULL;

    return true;
}

void CPPTXWriter::WriteTheme(CThemePtr pTheme, int & nIndexTheme, int & nStartLayout)
{
    if (!pTheme) return;

    std::wstring strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;

    std::wstring strThemeFile = L"theme" + std::to_wstring(nIndexTheme + 1) + L".xml";
    strThemeFile = strPptDirectory + _T("theme") + FILE_SEPARATOR_STR + strThemeFile;

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strThemeFile);

    CStringWriter oStringWriter;

    oStringWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\""));
    oStringWriter.WriteStringXML(pTheme->m_sThemeName);
    oStringWriter.WriteString(std::wstring(L"\"><a:themeElements>"));

    WriteColorScheme(oStringWriter, L"Default", pTheme->m_arColorScheme);

    if (!pTheme->m_arFonts.empty())
    {
        oStringWriter.WriteString(std::wstring(L"<a:fontScheme name=\"default\"><a:majorFont><a:latin typeface=\""));
        oStringWriter.WriteStringXML(pTheme->m_arFonts[0].Name);
        oStringWriter.WriteString(std::wstring(L"\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/></a:majorFont>"));

        oStringWriter.WriteString(std::wstring(L"<a:minorFont><a:latin typeface=\""));

        if (pTheme->m_arFonts.size() > 1 )	oStringWriter.WriteString	(pTheme->m_arFonts[1].Name);
        else                                oStringWriter.WriteStringXML(pTheme->m_arFonts[0].Name);
    }

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

    if (pTheme->m_arExtraColorScheme.size())
    {
        oStringWriter.WriteString(std::wstring(L"<a:extraClrSchemeLst>"));

        for (size_t i = 0 ; i < pTheme->m_arExtraColorScheme.size(); i++)
        {
            std::wstring str = L" " + std::to_wstring(i + 1);
            WriteColorScheme(oStringWriter, pTheme->m_sThemeName + str, pTheme->m_arExtraColorScheme[i], true); //extra
        }

        oStringWriter.WriteString(std::wstring(L"</a:extraClrSchemeLst>"));
    } else
        oStringWriter.WriteString(std::wstring(L"<a:extraClrSchemeLst/>"));

    oStringWriter.WriteString(std::wstring(L"</a:theme>"));

    oFile.WriteStringUTF8(oStringWriter.GetData());
    oFile.CloseFile();

    CRelsGenerator oRels(&m_oManager);
    int nCountLayouts = (int)pTheme->m_arLayouts.size();
    oRels.StartMaster(nIndexTheme, nStartLayout, nCountLayouts);

    CStringWriter oWriter;
    oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");

    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(L"<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
    }
    else if (pTheme->m_eType == typeNotesMaster)
    {
        oWriter.WriteString(L"<p:notesMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
    }
    else if (pTheme->m_eType == typeHandoutMaster)
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

    CGroupElement *pGroupElement = !pTheme->m_arElements.empty() ? dynamic_cast<CGroupElement *>(pTheme->m_arElements[0].get()) : NULL;

    size_t start_index = 0;
    if (pGroupElement)
    {
        for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
        {
            if (isBodyPlaceholder(pGroupElement->m_pChildElements[i]->m_lPlaceholderType))
                pGroupElement->m_pChildElements[i]->m_lPlaceholderType = 100; //body тип прописывать !!

            //if (pGroupElement->m_pChildElements[i]->m_bAnchorEnabled == false &&
            //	pGroupElement->m_pChildElements[i]->m_bChildAnchorEnabled == false)
            //	continue;

            //if (pTheme->m_eType == typeNotesMaster)
            //{
            //	pGroupElement->m_pChildElements[i]->m_lPlaceholderID = -1;
            //}
            //else if (pTheme->m_eType == typeHandoutMaster)
            //{
            //	pGroupElement->m_pChildElements[i]->m_lPlaceholderID = -1;
            //	pGroupElement->m_pChildElements[i]->m_lPlaceholderSizePreset = -1;
            //}
            WriteElement(oWriter, oRels, pGroupElement->m_pChildElements[i]);
        }

        start_index = 1;
    }

    for (size_t i = start_index; i < pTheme->m_arElements.size(); ++i)
    {
        if (isBodyPlaceholder(pTheme->m_arElements[i]->m_lPlaceholderType))
            pTheme->m_arElements[i]->m_lPlaceholderType = 100; //body тип прописывать !!

        //if (pTheme->m_arElements[i]->m_bAnchorEnabled == false &&
        //	pTheme->m_arElements[i]->m_bChildAnchorEnabled == false)
        //	continue;

        //if (pTheme->m_eType == typeNotesMaster)
        //{
        //	pTheme->m_arElements[i]->m_lPlaceholderID = -1;
        //}
        //else if (pTheme->m_eType == typeHandoutMaster)
        //{
        //	pTheme->m_arElements[i]->m_lPlaceholderID = -1;
        //	pTheme->m_arElements[i]->m_lPlaceholderSizePreset = -1;
        //}

        WriteElement(oWriter, oRels, pTheme->m_arElements[i]);
    }

    oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

    std::wstring strOverrideColorScheme = _T("<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
    oWriter.WriteString(strOverrideColorScheme);

    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(std::wstring(L"<p:sldLayoutIdLst>"));

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
    CStylesWriter styleWriter;
    styleWriter.m_pTheme = pTheme.get();

    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(std::wstring(L"<p:txStyles>"));

        oWriter.WriteString(std::wstring(L"<p:titleStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[1], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:titleStyle>"));

        oWriter.WriteString(std::wstring(L"<p:bodyStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[2], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:bodyStyle>"));

        oWriter.WriteString(std::wstring(L"<p:otherStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[3], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:otherStyle>"));

        oWriter.WriteString(std::wstring(L"</p:txStyles>"));
    }
    else if (pTheme->m_eType == typeNotesMaster)
    {
        oWriter.WriteString(std::wstring(L"<p:notesStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[1], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:notesStyle>"));
    }

    std::wstring strSlideMasterFile;
    std::wstring strSlideMasterRelsFile;
    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(std::wstring(L"</p:sldMaster>"));

        strSlideMasterFile = L"slideMaster" + std::to_wstring(nIndexTheme + 1) + L".xml";
        strSlideMasterFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

        strSlideMasterRelsFile = L"slideMaster" + std::to_wstring(nIndexTheme + 1) + L".xml.rels";
        strSlideMasterRelsFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
    }
    else if (pTheme->m_eType == typeNotesMaster)
    {
        oWriter.WriteString(std::wstring(L"</p:notesMaster>"));

        strSlideMasterFile = L"notesMaster1.xml";
        strSlideMasterFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

        strSlideMasterRelsFile = L"notesMaster1.xml.rels";
        strSlideMasterRelsFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
    }
    else if (pTheme->m_eType == typeHandoutMaster)
    {
        oWriter.WriteString(std::wstring(L"</p:handoutMaster>"));

        strSlideMasterFile = L"handoutMaster1.xml";
        strSlideMasterFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

        strSlideMasterRelsFile = L"handoutMaster1.xml.rels";
        strSlideMasterRelsFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
    }
    oFile.CreateFileW(strSlideMasterFile);
    std::wstring strMaster = oWriter.GetData();
    oFile.WriteStringUTF8(strMaster);
    oFile.CloseFile();

    oRels.CloseRels();
    oRels.SaveRels(strSlideMasterRelsFile);

    nStartLayout += nCountLayouts;
    nIndexTheme++;
}

void CPPTXWriter::WriteColorScheme(CStringWriter& oStringWriter, const std::wstring & name, const std::vector<CColor> & colors, bool extra)
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

    oStringWriter.WriteString(L"<a:dk1><a:srgbClr val=\"" + XmlUtils::ToString(colors[14].GetLONG_RGB(), L"%06X") + L"\"/></a:dk1>");

    oStringWriter.WriteString(L"<a:lt1><a:srgbClr val=\"" + XmlUtils::ToString(colors[13].GetLONG_RGB(), L"%06X") + L"\"/></a:lt1>");

    oStringWriter.WriteString(L"<a:dk2><a:srgbClr val=\"" + XmlUtils::ToString(colors[16].GetLONG_RGB(), L"%06X") + L"\"/></a:dk2>");

    oStringWriter.WriteString(L"<a:lt2><a:srgbClr val=\"" + XmlUtils::ToString(colors[15].GetLONG_RGB(), L"%06X") + L"\"/></a:lt2>");

    oStringWriter.WriteString(L"<a:accent1><a:srgbClr val=\"" + XmlUtils::ToString(colors[5].GetLONG_RGB(), L"%06X") + L"\"/></a:accent1>");

    oStringWriter.WriteString(L"<a:accent2><a:srgbClr val=\"" + XmlUtils::ToString(colors[6].GetLONG_RGB(), L"%06X") + L"\"/></a:accent2>");

    oStringWriter.WriteString(L"<a:accent3><a:srgbClr val=\"" + XmlUtils::ToString(colors[7].GetLONG_RGB(), L"%06X") + L"\"/></a:accent3>");

    oStringWriter.WriteString(L"<a:accent4><a:srgbClr val=\"" + XmlUtils::ToString(colors[8].GetLONG_RGB(), L"%06X") + L"\"/></a:accent4>");

    oStringWriter.WriteString(L"<a:accent5><a:srgbClr val=\"" + XmlUtils::ToString(colors[9].GetLONG_RGB(), L"%06X") + L"\"/></a:accent5>");

    oStringWriter.WriteString(L"<a:accent6><a:srgbClr val=\"" + XmlUtils::ToString(colors[10].GetLONG_RGB(), L"%06X") + L"\"/></a:accent6>");

    oStringWriter.WriteString(L"<a:hlink><a:srgbClr val=\"" + XmlUtils::ToString(colors[11].GetLONG_RGB(), L"%06X") + L"\"/></a:hlink>");

    oStringWriter.WriteString(L"<a:folHlink><a:srgbClr val=\"" + XmlUtils::ToString(colors[12].GetLONG_RGB(), L"%06X") + L"\"/></a:folHlink>");

    oStringWriter.WriteString(L"</a:clrScheme>");
    if (extra)
    {
        oStringWriter.WriteString(L"<a:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" \
                                  accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
        oStringWriter.WriteString(L"</a:extraClrScheme>");
    }
}

void CPPTXWriter::WriteBackground(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground)
{
    m_pShapeWriter->SetRelsGenerator(&oRels);
	std::wstring sBg = m_pShapeWriter->ConvertBrush(oBackground);
    
	if (false == sBg.empty())
	{
		oWriter.WriteString(std::wstring(L"<p:bg><p:bgPr>"));
		oWriter.WriteString(sBg);
		oWriter.WriteString(std::wstring(L"</p:bgPr></p:bg>"));
	}
}
void CPPTXWriter::WriteGroup(CStringWriter& oWriter, CRelsGenerator& oRels, CElementPtr pElement, CLayout* pLayout)
{
    CGroupElement *pGroupElement = dynamic_cast<CGroupElement*>(pElement.get());

    m_pShapeWriter->SetElement(pElement);
    oWriter.WriteString(m_pShapeWriter->ConvertGroup());

    for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); i++)
    {
        WriteElement(oWriter, oRels, pGroupElement->m_pChildElements[i], pLayout);
    }
    oWriter.WriteString(L"</p:grpSp>");
}
void CPPTXWriter::WriteTable(CStringWriter& oWriter, CRelsGenerator& oRels, CElementPtr pElement, CLayout* pLayout)
{
    CTableElement *pTableElement = dynamic_cast<CTableElement*>(pElement.get());


    PPTX::Logic::GraphicFrame gf;
    TableWriter table(pTableElement, &oRels);
    if (pTableElement->m_xmlRawData.empty())
    {
        table.Convert(gf);
        oWriter.WriteString(gf.toXML());
    } else
    {
        oWriter.WriteString(table.getXmlForGraphicFrame(pElement->m_lID, pElement->m_lPlaceholderID));
    }
}

void CPPTXWriter::WriteElement(CStringWriter& oWriter, CRelsGenerator& oRels, CElementPtr pElement, CLayout* pLayout)
{
    if (!pElement) return;


    CTableElement *pTableElement = dynamic_cast<CTableElement*>(pElement.get());
    if (pTableElement)
    {
        return WriteTable(oWriter, oRels, pElement, pLayout);
    }

    CGroupElement *pGroupElement = dynamic_cast<CGroupElement*>(pElement.get());
    if (pGroupElement)
    {
        return WriteGroup(oWriter, oRels, pElement, pLayout);
    }

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
                        CElementPtr pElLayout = pLayout->m_arElements[nIndex];

                        bool bIsEqualTransform = ((pElement->m_dRotate == pElLayout->m_dRotate)
                                                  && (pElement->m_bFlipH == pElLayout->m_bFlipH) && (pElement->m_bFlipV == pElLayout->m_bFlipV));

                        if (bIsEqualTransform)
                        {
                            if (pElement->m_bAnchorEnabled == pElLayout->m_bAnchorEnabled && pElLayout->m_bAnchorEnabled == true)
                            {
                                if (pElement->m_rcAnchor.IsEqual(pElLayout->m_rcAnchor, 0.5))
                                    pElement->m_bAnchorEnabled = false;
                            }
                            if (pElement->m_bChildAnchorEnabled == pElLayout->m_bChildAnchorEnabled && pElLayout->m_bChildAnchorEnabled == true)
                            {
                                if (pElement->m_rcChildAnchor.IsEqual(pElLayout->m_rcChildAnchor, 0.5))
                                    pElement->m_bChildAnchorEnabled = false;
                            }
                        }

                        break;
                    }
                }
            }
        }
        oWriter.WriteString(m_pShapeWriter->ConvertShape());
    }
}

void CPPTXWriter::WriteLayout(CLayoutPtr pLayout, int nIndexLayout, int nStartLayout, int nIndexTheme)
{
    if (!pLayout) return;

    CStringWriter oWriter;

    CRelsGenerator oRels(&m_oManager);
    oRels.StartLayout(nIndexTheme);

    oWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

    oWriter.WriteString(std::wstring(L"<p:sldLayout xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\""));
    oWriter.WriteString(std::wstring(L" type=\"") + pLayout->m_strLayoutType + _T("\""));
    oWriter.WriteString(std::wstring(L" showMasterSp=\"") + (pLayout->m_bShowMasterShapes ? _T("1") : _T("0")));
    oWriter.WriteString(std::wstring(L"\" preserve=\"1\"><p:cSld"));

    if ( pLayout->m_sName.empty() == false)
        oWriter.WriteString(std::wstring(L" name=\"") + pLayout->m_sName + std::wstring(L"\""));
    oWriter.WriteString(std::wstring(L">"));

    if (pLayout->m_bIsBackground)
    {
        WriteBackground(oWriter, oRels, pLayout->m_oBackground);
    }

    std::wstring strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
                               <a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
    oWriter.WriteString(strElems);

    size_t start_index = 0;

    if (pLayout->m_bIsTitleMaster)
    {
        CGroupElement *pGroupElement = (!pLayout->m_arElements.empty()) ? dynamic_cast<CGroupElement *>(pLayout->m_arElements[0].get()) : NULL;

        if (pGroupElement)
        {
            for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
            {
                WriteElement(oWriter, oRels, pGroupElement->m_pChildElements[i]);
            }

            start_index = 1;
        }
    }

    for (size_t i = start_index; i < pLayout->m_arElements.size(); ++i)
    {
        WriteElement(oWriter, oRels, pLayout->m_arElements[i]);
    }
    oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

    oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
    oWriter.WriteString(std::wstring(L"</p:sldLayout>"));

    oRels.CloseRels();

    std::wstring strXml = oWriter.GetData();
    std::wstring strFile = L"slideLayout" + std::to_wstring(nIndexLayout + nStartLayout + 1) + L".xml";

    NSFile::CFileBinary oFile;
    std::wstring strFileLayoutPath= m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slideLayouts") + FILE_SEPARATOR_STR;
    oFile.CreateFileW(strFileLayoutPath  + strFile);
    oFile.WriteStringUTF8(strXml);
    oFile.CloseFile();

    strFile = L"slideLayout" + std::to_wstring(nIndexLayout + nStartLayout + 1) + L".xml.rels";
    oRels.SaveRels(strFileLayoutPath + _T("_rels") + FILE_SEPARATOR_STR + strFile);
}
void CPPTXWriter::WriteSlide(int nIndexSlide)
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
        {
            nLayout += (int)m_pDocument->m_arThemes[i]->m_arLayouts.size();
        }

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

    CGroupElement *pGroupElement = !pSlide->m_arElements.empty() ? dynamic_cast<CGroupElement *>(pSlide->m_arElements[0].get()) : NULL;

    size_t start_index = 0;

    if (pGroupElement)
    {
        for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
        {
            auto& element = pGroupElement->m_pChildElements[i];
            WriteElement(oWriter, oRels, element);
        }

        start_index = 1;
    }

    for (size_t i = start_index; i < pSlide->m_arElements.size(); ++i)
    {
        auto& element = pSlide->m_arElements[i];
        WriteElement(oWriter, oRels, element);
    }

    oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

    oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));

    WriteTransition(oWriter, pSlide->m_oSlideShow);

    // TODO write new method and class for timing
    WriteTiming(oWriter, oRels, nIndexSlide);


    oWriter.WriteString(std::wstring(L"</p:sld>"));




    oRels.CloseRels();

    std::wstring strXml = oWriter.GetData();
    std::wstring strFile = L"slide" + std::to_wstring(nIndexSlide + 1) + L".xml";
    std::wstring strFileSlidePath= m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slides")  + FILE_SEPARATOR_STR;

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strFileSlidePath + strFile);
    oFile.WriteStringUTF8(strXml);
    oFile.CloseFile();

    strFile = L"slide" + std::to_wstring(nIndexSlide + 1) + L".xml.rels";
    oRels.SaveRels(strFileSlidePath + _T("_rels") + FILE_SEPARATOR_STR + strFile);
}

void CPPTXWriter::WriteTransition(CStringWriter& oWriter, CSlideShowInfo &oSSInfo)
{
    Converter::Transition transitionConverter(oSSInfo, m_pShapeWriter->m_pRels);
    auto transition = transitionConverter.Convert();
    oWriter.WriteString(transition.toXML());
}

void CPPTXWriter::WriteNotes(int nIndexNotes)
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

    CGroupElement *pGroupElement = !pNotes->m_arElements.empty() ? dynamic_cast<CGroupElement *>(pNotes->m_arElements[0].get()) : NULL;

    size_t start_index = 0;

    if (pGroupElement)
    {
        for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
        {
            WriteElement(oWriter, oRels, pGroupElement->m_pChildElements[i]);
        }

        start_index = 1;
    }

    for (size_t i = start_index; i < pNotes->m_arElements.size(); ++i)
    {
        WriteElement(oWriter, oRels, pNotes->m_arElements[i]);
    }

    oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

    oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
    oWriter.WriteString(std::wstring(L"</p:notes>"));

    oRels.CloseRels();

    std::wstring strXml = oWriter.GetData();
    std::wstring strFile = L"notesSlide" + std::to_wstring(nIndexNotes + 1) + L".xml";
    std::wstring strFileSlidePath = m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("notesSlides")  + FILE_SEPARATOR_STR;

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strFileSlidePath + strFile);
    oFile.WriteStringUTF8(strXml);
    oFile.CloseFile();

    strFile = L"notesSlide" + std::to_wstring(nIndexNotes + 1) + L".xml.rels";
    oRels.SaveRels(strFileSlidePath + _T("_rels") + FILE_SEPARATOR_STR + strFile);
}

void CPPTXWriter::WriteSlides()
{
    m_oManager.WriteAudioCollection(m_pUserInfo->m_oExMedia.m_arAudioCollection);
    for (size_t nIndexS = 0; nIndexS < m_pDocument->m_arSlides.size(); ++nIndexS)
    {
        WriteSlide((int)nIndexS);
    }
}
void CPPTXWriter::WriteNotes()
{
    for (size_t nIndexS = 0; nIndexS < m_pDocument->m_arNotes.size(); ++nIndexS)
    {
        WriteNotes((int)nIndexS);
    }
}

void CPPTXWriter::WriteLayoutAfterTheme(CThemePtr pTheme, const int nIndexTheme, int &nStartLayout)
{//nIndexTheme тут уже +1
    CRelsGenerator oRels(&m_oManager);
    int nCountLayouts = (int)pTheme->m_arLayouts.size();
    oRels.StartMaster(nIndexTheme - 1, nStartLayout, nCountLayouts);

    CStringWriter oWriter;
    oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");

    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(L"<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
    }
    else if (pTheme->m_eType == typeNotesMaster)
    {
        oWriter.WriteString(L"<p:notesMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">");
    }
    else if (pTheme->m_eType == typeHandoutMaster)
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

    CGroupElement *pGroupElement = !pTheme->m_arElements.empty() ? dynamic_cast<CGroupElement *>(pTheme->m_arElements[0].get()) : NULL;

    size_t start_index = 0;
    if (pGroupElement)
    {
        for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
        {
            if (isBodyPlaceholder(pGroupElement->m_pChildElements[i]->m_lPlaceholderType))
                pGroupElement->m_pChildElements[i]->m_lPlaceholderType = 100; //body тип прописывать !!

            //if (pGroupElement->m_pChildElements[i]->m_bAnchorEnabled == false &&
            //	pGroupElement->m_pChildElements[i]->m_bChildAnchorEnabled == false)
            //	continue;

            //if (pTheme->m_eType == typeNotesMaster)
            //{
            //	pGroupElement->m_pChildElements[i]->m_lPlaceholderID = -1;
            //}
            //else if (pTheme->m_eType == typeHandoutMaster)
            //{
            //	pGroupElement->m_pChildElements[i]->m_lPlaceholderID = -1;
            //	pGroupElement->m_pChildElements[i]->m_lPlaceholderSizePreset = -1;
            //}
            WriteElement(oWriter, oRels, pGroupElement->m_pChildElements[i]);
        }

        start_index = 1;
    }

    for (size_t i = start_index; i < pTheme->m_arElements.size(); ++i)
    {
        if (isBodyPlaceholder(pTheme->m_arElements[i]->m_lPlaceholderType))
            pTheme->m_arElements[i]->m_lPlaceholderType = 100; //body тип прописывать !!

        //if (pTheme->m_arElements[i]->m_bAnchorEnabled == false &&
        //	pTheme->m_arElements[i]->m_bChildAnchorEnabled == false)
        //	continue;

        //if (pTheme->m_eType == typeNotesMaster)
        //{
        //	pTheme->m_arElements[i]->m_lPlaceholderID = -1;
        //}
        //else if (pTheme->m_eType == typeHandoutMaster)
        //{
        //	pTheme->m_arElements[i]->m_lPlaceholderID = -1;
        //	pTheme->m_arElements[i]->m_lPlaceholderSizePreset = -1;
        //}

        WriteElement(oWriter, oRels, pTheme->m_arElements[i]);
    }

    oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

    std::wstring strOverrideColorScheme = _T("<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
    oWriter.WriteString(strOverrideColorScheme);

    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(std::wstring(L"<p:sldLayoutIdLst>"));

        size_t __nCountLayouts = 0;
        for (int nIndexLayout = 0; nIndexLayout < nCountLayouts; ++nIndexLayout)
        {
            oWriter.WriteString(L"<p:sldLayoutId id=\"" + std::to_wstring(0x80000000 + nIndexTheme + 1 + nStartLayout + nIndexLayout) + L"\" r:id=\"rId" + std::to_wstring(nIndexLayout + 1) + L"\"/>");

            WriteLayout(pTheme->m_arLayouts[nIndexLayout], nIndexLayout, nStartLayout, nIndexTheme - 1);
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
    CStylesWriter styleWriter;
    styleWriter.m_pTheme = pTheme.get();

    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(std::wstring(L"<p:txStyles>"));

        oWriter.WriteString(std::wstring(L"<p:titleStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[1], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:titleStyle>"));

        oWriter.WriteString(std::wstring(L"<p:bodyStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[2], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:bodyStyle>"));

        oWriter.WriteString(std::wstring(L"<p:otherStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[3], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:otherStyle>"));

        oWriter.WriteString(std::wstring(L"</p:txStyles>"));
    }
    else if (pTheme->m_eType == typeNotesMaster)
    {
        oWriter.WriteString(std::wstring(L"<p:notesStyle>"));
        styleWriter.ConvertStyles(pTheme->m_pStyles[1], oWriter, 9);
        oWriter.WriteString(std::wstring(L"</p:notesStyle>"));
    }

    std::wstring strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;

    std::wstring strSlideMasterFile;
    std::wstring strSlideMasterRelsFile;
    if (pTheme->m_eType == typeMaster)
    {
        oWriter.WriteString(std::wstring(L"</p:sldMaster>"));

        strSlideMasterFile = L"slideMaster" + std::to_wstring(nIndexTheme) + L".xml";
        strSlideMasterFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

        strSlideMasterRelsFile = L"slideMaster" + std::to_wstring(nIndexTheme) + L".xml.rels";
        strSlideMasterRelsFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
    }
    else if (pTheme->m_eType == typeNotesMaster)
    {
        oWriter.WriteString(std::wstring(L"</p:notesMaster>"));

        strSlideMasterFile = L"notesMaster1.xml";
        strSlideMasterFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

        strSlideMasterRelsFile = L"notesMaster1.xml.rels";
        strSlideMasterRelsFile = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
    }
    else if (pTheme->m_eType == typeHandoutMaster)
    {
        oWriter.WriteString(std::wstring(L"</p:handoutMaster>"));

        strSlideMasterFile = L"handoutMaster1.xml";
        strSlideMasterFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

        strSlideMasterRelsFile = L"handoutMaster1.xml.rels";
        strSlideMasterRelsFile = strPptDirectory + _T("handoutMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;
    }

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strSlideMasterFile);
    std::wstring strMaster = oWriter.GetData();
    oFile.WriteStringUTF8(strMaster);
    oFile.CloseFile();

    oRels.CloseRels();
    oRels.SaveRels(strSlideMasterRelsFile);

    nStartLayout += nCountLayouts;
}


void CPPTXWriter::WriteTiming(CStringWriter& oWriter, CRelsGenerator &oRels, int nIndexSlide)
{
    auto slide_iter = m_pUserInfo->m_mapSlides.find(m_pUserInfo->m_arrSlidesOrder[nIndexSlide]);
    CSlide* pCSlide = m_pDocument->m_arSlides[nIndexSlide];

    auto intermediateSlideAnimation = Intermediate::ParseSlideAnimation(slide_iter->second, pCSlide);
    auto timing =
            Converter::Timing(intermediateSlideAnimation).
            Convert(&(m_pUserInfo->m_oExMedia), &oRels);
    oWriter.WriteString(timing.toXML());
}

std::vector<std::wstring> CPPTXWriter::GrepPaths(const std::vector<std::wstring> &paths, const std::wstring &strRegEx)
{
    std::vector<std::wstring> filtredPaths;
    try
    {
        boost::wregex regEx(strRegEx);
        boost::wsmatch wSmath;
        for (const auto& path : paths)
        {
            if (boost::regex_match(path, wSmath, regEx))
                filtredPaths.push_back(path);
        }
    } catch(...) {}

    return filtredPaths;
}
}
