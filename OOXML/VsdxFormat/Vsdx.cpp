/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "Vsdx.h"

#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"

#include "FileTypes_Draw.h"

#include "../../DesktopEditor/common/SystemUtils.h"

#include "VisioDocument.h"
#include "VisioConnections.h"
#include "Shapes.h"


OOX::Draw::CVsdx::CVsdx() : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
{
}
OOX::Draw::CVsdx::CVsdx(const CPath& oFilePath) : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
{
	m_bVisioPages = true;
	Read( oFilePath );
}
OOX::Draw::CVsdx::~CVsdx()
{

}	
bool OOX::Draw::CVsdx::Read(const CPath& oFilePath)
{
	m_sDocumentPath = oFilePath.GetPath();

	OOX::CRels oRels(oFilePath / FILE_SEPARATOR_STR);
	IFileContainer::Read(oRels, oFilePath, oFilePath);

	m_pDocument = Find(FileTypes::Document).smart_dynamic_cast<Draw::CDocumentFile>();
	m_pApp = Find(OOX::FileTypes::App).smart_dynamic_cast<CApp>();
	m_pCore = Find(OOX::FileTypes::Core).smart_dynamic_cast<CCore>();
	return true;
}
bool OOX::Draw::CVsdx::Write(const CPath& oDirPath, OOX::CContentTypes &oContentTypes)
{  
    //CPath oVisioPath = oDirPath / m_pWorkbook->DefaultDirectory();
    //WriteWorkbook(oVisioPath);

	IFileContainer::Write(oDirPath / L"" , OOX::CPath(_T("")), oContentTypes);

    oContentTypes.Write(oDirPath);
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------
OOX::Draw::CValidation::CValidation(OOX::Document* pMain) : OOX::File(pMain)
{
}
OOX::Draw::CValidation::CValidation(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
{
	read(uri.GetDirectory(), uri);
}
OOX::Draw::CValidation::CValidation(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
{
	read(oRootPath, oPath);
}
OOX::Draw::CValidation::~CValidation()
{
}
void OOX::Draw::CValidation::read(const CPath& oFilePath)
{
	CPath oRootPath;
	read(oRootPath, oFilePath);
}
void OOX::Draw::CValidation::read(const CPath& oRootPath, const CPath& oFilePath)
{
	XmlUtils::CXmlLiteReader oReader;

	if (!oReader.FromFile(oFilePath.GetPath()))
		return;

	if (!oReader.ReadNextNode())
		return;

	m_strFilename = oFilePath.GetPath();

	std::wstring sName = oReader.GetName();
	//if (L"dgm:dataModel" == sName && !oReader.IsEmptyNode())
	//{
	//	m_oDataModel = oReader;
	//}
	//if (false == id_drawing.IsInit() && m_oDataModel.IsInit())
	//{
	//	for (size_t i = 0; m_oDataModel->m_oExtLst.IsInit() && (i < m_oDataModel->m_oExtLst->m_arrExt.size()); i++)
	//	{
	//		if (m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt.IsInit())
	//		{
	//			id_drawing = m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt->m_oRelId;
	//			break;
	//		}
	//	}
	//}
}
void OOX::Draw::CValidation::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
{
	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

	oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
	toXmlWriter(&oXmlWriter);

	NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

	oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
}
const OOX::FileType OOX::Draw::CValidation::type() const
{
	return FileTypes::Validation;
}
const OOX::CPath OOX::Draw::CValidation::DefaultDirectory() const
{
	return type().DefaultDirectory();
}
const OOX::CPath OOX::Draw::CValidation::DefaultFileName() const
{
	return type().DefaultFileName();
}
void OOX::Draw::CValidation::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
{
	LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

	while (pReader->GetPos() < end)
	{
		BYTE _rec = pReader->GetUChar();

		switch (_rec)
		{
		case 0:
		{
			//m_oDataModel.Init();
			//m_oDataModel->fromPPTY(pReader);

		}break;
		default:
		{
			pReader->SkipRecord();
		}break;
		}
	}
	pReader->Seek(end);
}
void OOX::Draw::CValidation::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	//pWriter->WriteRecord2(0, m_oDataModel);
}
void OOX::Draw::CValidation::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
{
	//if (m_oDataModel.IsInit())
	//	m_oDataModel->toXmlWriter(pWriter);
}
//-----------------------------------------------------------------------------------------------------------------------------
OOX::Draw::CComments::CComments(OOX::Document* pMain) : OOX::File(pMain)
{
}
OOX::Draw::CComments::CComments(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
{
	read(uri.GetDirectory(), uri);
}
OOX::Draw::CComments::CComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
{
	read(oRootPath, oPath);
}
OOX::Draw::CComments::~CComments()
{
}
void OOX::Draw::CComments::read(const CPath& oFilePath)
{
	CPath oRootPath;
	read(oRootPath, oFilePath);
}
void OOX::Draw::CComments::read(const CPath& oRootPath, const CPath& oFilePath)
{
	XmlUtils::CXmlLiteReader oReader;

	if (!oReader.FromFile(oFilePath.GetPath()))
		return;

	if (!oReader.ReadNextNode())
		return;

	m_strFilename = oFilePath.GetPath();

	std::wstring sName = oReader.GetName();
	//if (L"dgm:dataModel" == sName && !oReader.IsEmptyNode())
	//{
	//	m_oDataModel = oReader;
	//}
	//if (false == id_drawing.IsInit() && m_oDataModel.IsInit())
	//{
	//	for (size_t i = 0; m_oDataModel->m_oExtLst.IsInit() && (i < m_oDataModel->m_oExtLst->m_arrExt.size()); i++)
	//	{
	//		if (m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt.IsInit())
	//		{
	//			id_drawing = m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt->m_oRelId;
	//			break;
	//		}
	//	}
	//}
}
void OOX::Draw::CComments::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
{
	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

	oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
	toXmlWriter(&oXmlWriter);

	NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

	oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
}
const OOX::FileType OOX::Draw::CComments::type() const
{
	return FileTypes::Comments;
}
const OOX::CPath OOX::Draw::CComments::DefaultDirectory() const
{
	return type().DefaultDirectory();
}
const OOX::CPath OOX::Draw::CComments::DefaultFileName() const
{
	return type().DefaultFileName();
}
void OOX::Draw::CComments::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
{
	LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

	while (pReader->GetPos() < end)
	{
		BYTE _rec = pReader->GetUChar();

		switch (_rec)
		{
		case 0:
		{
			//m_oDataModel.Init();
			//m_oDataModel->fromPPTY(pReader);

		}break;
		default:
		{
			pReader->SkipRecord();
		}break;
		}
	}
	pReader->Seek(end);
}
void OOX::Draw::CComments::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	//pWriter->WriteRecord2(0, m_oDataModel);
}
void OOX::Draw::CComments::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
{
	//if (m_oDataModel.IsInit())
	//	m_oDataModel->toXmlWriter(pWriter);
}
//-----------------------------------------------------------------------------------------------------------------------------
