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
#include "RtfOle.h"
#include "../OOXml/Writer/OOXWriter.h"
#include "../OOXml/Writer/OOXRelsWriter.h"
#include "RtfDocument.h"
#include "../../Common/OfficeFileFormatChecker.h"

RtfOle::RtfOle()
{
	SetDefault();
}
RtfOle::~RtfOle()
{
	SetDefault();
}

int RtfOle::GetType( )
{
	return TYPE_RTF_OLE;
}
bool RtfOle::IsValid()
{
	return PROP_DEF != m_nWidth && PROP_DEF != m_nHeight && L"" != m_sOleFilename;
			/*&& ::GetFileAttributes( m_sOleFilename ) != DWORD( -1 )*/
}

std::wstring RtfOle::RenderToOOX(RenderParameter oRenderParameter)
{
	if( false == IsValid() )	return L"";
	
    std::wstring sResult;

	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
	if( RENDER_TO_OOX_PARAM_OLE_ONLY == oRenderParameter.nType )
	{
		sResult += RenderToOOXOnlyOle(oRenderParameter);
	}
	else
	{
		RtfCharProperty * pCharProps = &m_oCharProperty;
//------------------------------------------
// todooo общая часть с RtfChar
		bool bInsert = false;
		bool bDelete = false;

		if (pCharProps->m_nRevised != PROP_DEF)
		{
			bInsert = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(pCharProps->m_nRevauth);
            std::wstring sDate(RtfUtility::convertDateTime(pCharProps->m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			pCharProps->m_nRevised = PROP_DEF;
		}
		if (pCharProps->m_nDeleted != PROP_DEF)
		{
			bDelete = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(pCharProps->m_nRevauthDel);
            std::wstring sDate(RtfUtility::convertDateTime(pCharProps->m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			pCharProps->m_nDeleted = PROP_DEF;
		}
//----------
		sResult += L"<w:r>";
		
        std::wstring sCharProp = pCharProps->RenderToOOX(oRenderParameter);

        if (!sCharProp .empty())
		{
			sResult += L"<w:rPr>";
				sResult += sCharProp;
			sResult += L"</w:rPr>";
		}
		
        sResult += L"<w:object w:dxaOrig=\"" + std::to_wstring(m_nWidth) +
                          L"\" w:dyaOrig=\"" + std::to_wstring(m_nHeight) + L"\">";
		
		RenderParameter oNewRenderParameter = oRenderParameter;
		oNewRenderParameter.nType = RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2;

		if (m_oResultShape)
		{
			m_oResultShape->m_bIsOle = true;

			sResult += m_oResultShape->RenderToOOX(oNewRenderParameter);

			oNewRenderParameter.nValue = m_oResultShape->m_nID;
		}
		sResult += RenderToOOXOnlyOle(oNewRenderParameter);

		sResult += L"</w:object>";
		sResult += L"</w:r>";
		
		if (bDelete)sResult += L"</w:del>";
		if (bInsert)sResult += L"</w:ins>";
	}
	return sResult;
}
std::wstring RtfOle::RenderToOOXOnlyOle(RenderParameter oRenderParameter)
{
    std::wstring sResult;

	OOXWriter		* poOOXWriter	= static_cast<OOXWriter*>		(oRenderParameter.poWriter);
	OOXRelsWriter	* poRelsWriter	= static_cast<OOXRelsWriter*>	(oRenderParameter.poRels);
	RtfDocument		* poDocument	= static_cast<RtfDocument*>		(oRenderParameter.poDocument);

	sResult += L"<o:OLEObject";
	switch ( m_eOleType )
	{
		case ot_link:	sResult += L" Type=\"Link\"";	break;
		default: 		sResult += L" Type=\"Embed\"";	break;
	}
    sResult += L" ProgID=\"" + m_sOleClass + L"\"";
    sResult += L" ShapeID=\"_x0000_s" + std::to_wstring(poDocument->GetShapeId( oRenderParameter.nValue )) + L"\"";
	sResult += L" DrawAspect=\"Content\"";
    sResult += L" ObjectID=\"" + poDocument->m_oIdGenerator.Generate_OleId() + L"\"";

    std::wstring sExtension	= L"bin";
    std::wstring sMime		= L"application/vnd.openxmlformats-officedocument.oleObject";
	std::wstring sRelsType	= L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject";

	COfficeFileFormatChecker checker;

	if (checker.isOfficeFile(m_sOleFilename))
	{
		switch (checker.nFileType)
		{
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS:
			{
				sExtension = L"xls";
				sMime = L"application/vnd.ms-excel";
			}break;
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
			{
				sExtension = L"xlsx";
				sMime = L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
				sRelsType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package";
			}break;
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:
			{
				sExtension = L"doc";
				sMime = L"application/msword";
			}break;
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
			{
				sExtension = L"docx";
				sMime = L"application/vnd.openxmlformats-officedocument.wordprocessingml.document";
				sRelsType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package";

			}break;
		}
	}

    std::wstring sFilenameRels;
    sFilenameRels += L"oleObject" + std::to_wstring(poDocument->m_oIdGenerator.Generate_OleIndex()) + L".";
	sFilenameRels += sExtension;
	
    std::wstring sFilenameFull = poOOXWriter->m_sTargetFolder + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"embeddings";
	
    NSDirectory::CreateDirectory( sFilenameFull );
	
	sFilenameFull += FILE_SEPARATOR_STR + sFilenameRels;
	sFilenameRels = L"embeddings/" + sFilenameRels;

	Utils::CopyDirOrFile( m_sOleFilename, sFilenameFull );

	poOOXWriter->m_oContentTypes.AddExtension( sMime, sExtension);
    std::wstring srId = poRelsWriter->AddRelationship( sRelsType, sFilenameRels);
    
	sResult += L" r:id=\"" + srId + L"\"";
	sResult += L"/>";
	return sResult;
}

std::wstring RtfOle::RenderToRtf(RenderParameter oRenderParameter)
{
	if( !IsValid() ) return L"";

    std::wstring sResult;
	
	sResult += m_oCharProperty.RenderToRtf( oRenderParameter );
	sResult += L"{\\object";
	
	if( PROP_DEF != m_eOleType )
	{
		switch( m_eOleType )
		{
			case ot_emb:	sResult += L"\\objemb";		break;
			case ot_link:	sResult += L"\\objlink";	break;
		}
	}
	//sResult += L"\\f13";
	sResult += L"\\objsetsize";

	RENDER_RTF_INT( m_nWidth, sResult, L"objw" );
	RENDER_RTF_INT( m_nHeight, sResult, L"objh" );

    if( !m_sOleClass.empty() )
        sResult += L"{\\*\\objclass " + m_sOleClass + L"}";
	
    if( !m_sOleFilename.empty() )
    {
        std::wstring str = RtfUtility::RtfInternalEncoder::Encode( m_sOleFilename );
        sResult += L"{\\*\\objdata " + str + L"}";
    }
//-------------------------------------
	if( NULL != m_oResultShape )
	{
        std::wstring str = m_oResultShape->RenderToRtf( oRenderParameter );
		sResult += L"{\\result {\\pard\\plain" + str + L"}}";
	}
	//else if( NULL != m_oResultPicture )
	//{
	//		std::wstring str = m_oResultPicture->RenderToRtf( oRenderParameter );
	//		sResult += L"{\\result {\\*\\shppict" + str  + L"}}";
	//}
	sResult += L"}";
	return sResult;
}

void RtfOle::SetFilename( std::wstring sFilename )
{
	m_sOleFilename = sFilename;
}
void RtfOle::SetDefault()
{
	m_eOleType	 = ot_none;
	m_nShapeId	= PROP_DEF;
	m_nWidth	= PROP_DEF;
	m_nHeight	= PROP_DEF;

	Utils::RemoveDirOrFile( m_sOleFilename );
	m_sOleFilename = L"";

	m_oCharProperty.SetDefault();
}
