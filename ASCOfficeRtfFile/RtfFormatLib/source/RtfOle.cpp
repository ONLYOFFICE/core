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
#include "RtfOle.h"
#include "Writer/OOXWriter.h"
#include "Writer/OOXRelsWriter.h"
#include "RtfDocument.h"


CString RtfOle::RenderToOOX(RenderParameter oRenderParameter)
{
	if( false == IsValid() )
		return _T("");

	CString sResult;
	if( RENDER_TO_OOX_PARAM_OLE_ONLY == oRenderParameter.nType )
	{
		sResult.Append( RenderToOOXOnlyOle(oRenderParameter) );
	}
	else
	{
		sResult.Append( _T("<w:r>") );
		sResult.AppendFormat( _T("<w:object w:dxaOrig=\"%d\" w:dyaOrig=\"%d\">"), m_nWidth, m_nHeight );
		
		RenderParameter oNewRenderParameter = oRenderParameter;
		oNewRenderParameter.nType = RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2;
		m_oResultPic->m_bIsOle = true;
		sResult.Append( m_oResultPic->RenderToOOX(oNewRenderParameter) );

		oNewRenderParameter.nValue = m_oResultPic->m_nID;
		sResult.Append( RenderToOOXOnlyOle(oNewRenderParameter) );

		sResult.Append( _T("</w:object>") );
		sResult.Append( _T("</w:r>") );
	}
	return sResult;
}
CString RtfOle::RenderToOOXOnlyOle(RenderParameter oRenderParameter)
{
	CString sResult;

	OOXWriter* poOOXWriter			= static_cast<OOXWriter*>(oRenderParameter.poWriter);
	OOXRelsWriter* poRelsWriter		= static_cast<OOXRelsWriter*>(oRenderParameter.poRels);
	RtfDocument* poDocument			= static_cast<RtfDocument*>(oRenderParameter.poDocument);

	sResult.Append( _T("<o:OLEObject") );
	switch ( m_eOleType )
	{
		case ot_link: sResult.Append( _T(" Type=\"Link\"") );break;
		default: sResult.Append( _T(" Type=\"Embed\"") );break;
	}
    sResult.AppendFormat( _T(" ProgID=\"%ls\""), m_sOleClass.GetBuffer() );
    sResult.AppendFormat( _T(" ShapeID=\"_x0000_s%d\""), poDocument->GetShapeId( oRenderParameter.nValue ) );
	sResult.Append( _T(" DrawAspect=\"Content\"") );
    sResult.AppendFormat( _T(" ObjectID=\"%ls\""), poDocument->m_oIdGenerator.Generate_OleId().GetBuffer() );

	CString sExtension = _T("bin");
	CString sMime = _T("application/vnd.openxmlformats-officedocument.oleObject");
	
	CString sFilenameRels;
    sFilenameRels.AppendFormat( _T("oleObject%d.%ls"), poDocument->m_oIdGenerator.Generate_OleIndex(), sExtension.GetBuffer());
	
	CString sFilenameFull = poOOXWriter->m_sTargetFolder + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + _T("embeddings");
	
	FileSystem::Directory::CreateDirectory( sFilenameFull );
	
	sFilenameFull += FILE_SEPARATOR_STR + sFilenameRels;
	sFilenameRels = _T("embeddings/") + sFilenameRels;

	Utils::CopyDirOrFile( m_sOleFilename, sFilenameFull );

	poOOXWriter->m_oContentTypes.AddExtension( sMime, sExtension);
    CString srId = poRelsWriter->AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject"), sFilenameRels);
    sResult.AppendFormat( _T(" r:id=\"%ls\""), srId.GetBuffer() );
	sResult.Append( _T("/>") );
	return sResult;
}

#if defined(_WIN32) || defined(_WIN64)
	DWORD CALLBACK OlePut1(LPOLESTREAM oStream, const void FAR* pTarget, DWORD dwRead)
	{
		return 0;
	}
	DWORD CALLBACK OleGet2(LPOLESTREAM oStream, void FAR* pTarget, DWORD dwRead)
	{
		return 0;
	}
	DWORD CALLBACK OleGet1(LPOLESTREAM oStream, void FAR* pTarget, DWORD dwRead)
	{
		RtfOle1ToOle2Stream* piStream = static_cast<RtfOle1ToOle2Stream*>(oStream);
		if( piStream->nCurPos + (int)dwRead > piStream->nBufferSize )
			return 0;
		memcpy( pTarget, (piStream->pBuffer + piStream->nCurPos) , dwRead );
		piStream->nCurPos += dwRead;
		return dwRead;
	}
	DWORD CALLBACK OlePut2(LPOLESTREAM oStream, const void FAR* pTarget, DWORD dwWrite)
	{
		RtfOle2ToOle1Stream* piStream = static_cast<RtfOle2ToOle1Stream*>(oStream);
		BYTE* pSource = (BYTE*)pTarget;
		for( int i = 0; i < (int)dwWrite; i++ )
			piStream->aBuffer.push_back( pSource[i] );
		return dwWrite;
	}
#endif