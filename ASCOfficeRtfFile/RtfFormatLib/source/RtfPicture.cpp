#include "RtfPicture.h"
#include "Writer/OOXWriter.h"
#include "Utils.h"

CString RtfPicture::GenerateWMF(RenderParameter oRenderParameter)
{
	CString sResult;
	sResult.Append(_T("{\\pict"));
	//if(-1 != m_nShapeId)
	//	sResult.AppendFormat(_T("{\\*\\picprop\\shplid%d%ls}"), m_nShapeId, m_oShapeProp.RenderToRtf( oRenderParameter ) );
	//else
	//	sResult.AppendFormat(_T("{\\*\\picprop%ls}"), m_oShapeProp.RenderToRtf( oRenderParameter ) );
	////"наши" wmf не растягиваются
	RENDER_RTF_INT( 100, sResult, _T("picscalex") )
		RENDER_RTF_INT( 100, sResult, _T("picscaley") )

		//RENDER_RTF_INT( (int)m_dScaleX, sResult, _T("picscalex") )
		//RENDER_RTF_INT( (int)m_dScaleY, sResult, _T("picscaley") )
		RENDER_RTF_INT( m_nCropL, sResult, _T("piccropl") )
		RENDER_RTF_INT( m_nCropT, sResult, _T("piccropt") )
		RENDER_RTF_INT( m_nCropR, sResult, _T("piccropr") )
		RENDER_RTF_INT( m_nCropB, sResult, _T("piccropb") )

		RENDER_RTF_INT( m_nWidth, sResult, _T("picw") )
		RENDER_RTF_INT( m_nHeight, sResult, _T("pich") )
		RENDER_RTF_INT( m_nWidthGoal, sResult, _T("picwgoal") )
		RENDER_RTF_INT( m_nHeightGoal, sResult, _T("pichgoal") )

		sResult.Append(_T("\\wmetafile8"));

	sResult.Append( _T(" "));

	//сохраняем в темповую директорию и загружаем файл как текст
	RtfWriter * poWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter );
    //IUnknown* piTempPict = NULL;
	
	//todooo ??? переписать
	//if( true == LoadPicture( &piTempPict, m_sPicFilename ) )
	//{
	//	CString sTempFile = Utils::CreateTempFile( poWriter->m_sTempFolder );
	//	if( true == SavePicture( piTempPict, sTempFile, IMAGEFORMAT_WMF ) )
	//	{
	//		m_aTempFiles.push_back( sTempFile );
	//		sResult.Append( RtfInternalEncoder::Encode( sTempFile ) );
	//	}
	//}
    //RELEASEINTERFACE( piTempPict );

	sResult.Append(_T("}"));
	return sResult;
}
CString RtfPicture::RenderToRtf(RenderParameter oRenderParameter)		
{
	if( false == IsValid() )
		return _T("");
	CString sResult;

	sResult.Append(_T("{\\pict"));
	//if(-1 != m_nShapeId)
	//	sResult.AppendFormat(_T("{\\*\\picprop\\shplid%d%ls}"), m_nShapeId, m_oShapeProp.RenderToRtf( oRenderParameter ) );
	//else
	//	sResult.AppendFormat(_T("{\\*\\picprop%ls}"), m_oShapeProp.RenderToRtf( oRenderParameter ) );
	RENDER_RTF_INT( (int)m_dScaleX, sResult, _T("picscalex") )
		RENDER_RTF_INT( (int)m_dScaleY, sResult, _T("picscaley") )
		RENDER_RTF_INT( m_nCropL, sResult, _T("piccropl") )
		RENDER_RTF_INT( m_nCropT, sResult, _T("piccropt") )
		RENDER_RTF_INT( m_nCropR, sResult, _T("piccropr") )
		RENDER_RTF_INT( m_nCropB, sResult, _T("piccropb") )

		RENDER_RTF_INT( m_nWidth, sResult, _T("picw") )
		RENDER_RTF_INT( m_nHeight, sResult, _T("pich") )
		RENDER_RTF_INT( m_nWidthGoal, sResult, _T("picwgoal") )
		RENDER_RTF_INT( m_nHeightGoal, sResult, _T("pichgoal") )

		switch( eDataType )
		{
			case dt_emf: sResult.Append(_T("\\emfblip"));break;
			case dt_wmf: sResult.Append(_T("\\wmetafile8"));break;
			case dt_png: sResult.Append(_T("\\pngblip"));break;
			case dt_jpg: sResult.Append(_T("\\jpegblip"));break;
				//case dt_bmp: sResult.Append(_T("wbitmap8"));break;
		}
	sResult.Append( _T(" "));

    sResult.Append( RtfUtility::RtfInternalEncoder::Encode( m_sPicFilename ) );

	sResult.Append(_T("}"));
	return sResult;
}
CString RtfPicture::RenderToOOX(RenderParameter oRenderParameter)
{
	if( false == IsValid() )
		return _T("");
	
	OOXWriter* poOOXWriter			= static_cast<OOXWriter*>(oRenderParameter.poWriter);
	OOXRelsWriter* poRelsWriter		= static_cast<OOXRelsWriter*>(oRenderParameter.poRels);
	RtfDocument* poRtfDocument		= static_cast<RtfDocument*>(oRenderParameter.poDocument);
	
	CString sExtension;	
	CString sMime;
	switch( eDataType )
	{
		case dt_png: sExtension = _T("png"); sMime = _T("image/png");break;
		case dt_jpg: sExtension = _T("jpg"); sMime = _T("image/jpg");break;
		case dt_wmf: sExtension = _T("wmf"); sMime = _T("image/x-wmf");break;
		case dt_emf: sExtension = _T("emf"); sMime = _T("image/x-emf");break;
	}

	CString sFilenameRels;
    sFilenameRels.AppendFormat( _T("Image%d.%ls"), poRtfDocument->m_oIdGenerator.Generate_ImageIndex(), sExtension.GetBuffer());
	CString sFilenameFull = poOOXWriter->m_sTargetFolder + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR +_T("media");
	
	FileSystem::Directory::CreateDirectory( sFilenameFull );
	
	sFilenameFull += FILE_SEPARATOR_STR + sFilenameRels;
	sFilenameRels = _T("media/") + sFilenameRels;

	if( m_sPicFilename != sFilenameFull )
		Utils::CopyDirOrFile( m_sPicFilename, sFilenameFull );
	else
		m_bIsCopy = false;

	poOOXWriter->m_oContentTypes.AddExtension( sMime, sExtension);

	CString srId = poRelsWriter->AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"), sFilenameRels);

	//CString sResult;
	//sResult.AppendFormat( _T("<v:imagedata r:id=\"%ls\""), srId );

	return srId;
}