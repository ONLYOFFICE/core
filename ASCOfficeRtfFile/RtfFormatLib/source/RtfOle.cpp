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
