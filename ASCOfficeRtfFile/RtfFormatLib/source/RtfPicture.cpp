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
	////"наши" wmf не раст€гиваютс€
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

	//сохран€ем в темповую директорию и загружаем файл как текст
	RtfWriter * poWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter );
	IUnknown* piTempPict = NULL;
	
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
	RELEASEINTERFACE( piTempPict );

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

	sResult.Append( RtfInternalEncoder::Encode( m_sPicFilename ) );

	sResult.Append(_T("}"));
	return sResult;
}
CString RtfPicture::RenderToOOX(RenderParameter oRenderParameter)
{
	if( false == IsValid() )
		return _T("");
	
	OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
	OOXRelsWriter* poRelsWriter = static_cast<OOXRelsWriter*>(oRenderParameter.poRels);
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	CString sFilenameFull;
	CString sFilenameRels;
	CString sExtension;
	CString sMime;
	switch( eDataType )
	{
		case dt_png: sExtension = _T("png"); sMime = _T("image/png");break;
		case dt_jpg: sExtension = _T("jpg"); sMime = _T("image/jpg");break;
		case dt_wmf: sExtension = _T("wmf"); sMime = _T("image/x-wmf");break;
		case dt_emf: sExtension = _T("emf"); sMime = _T("image/x-emf");break;
	}
	sFilenameRels.AppendFormat( _T("media/Image%d.%ls"), poRtfDocument->m_oIdGenerator.Generate_ImageIndex(), sExtension);
	sFilenameFull = poOOXWriter->m_sTargetFolder + _T("/") + poOOXWriter->m_sDocumentFolder + _T("/") +sFilenameRels;
	CreateDirectory( poOOXWriter->m_sTargetFolder + _T("/") + poOOXWriter->m_sDocumentFolder + _T("/media"), NULL);

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
//bool RtfPicture::LoadPicture( IUnknown** piImage, CString sPath )
//{
//	ImageStudio::IImageTransformsPtr piTransform = NULL;
//	HRESULT hRes = piTransform.CreateInstance( ImageStudio::CLSID_ImageTransforms );
//	if( FAILED(hRes) || NULL == piTransform )
//		return false;
//
//	VARIANT vImage;
//	CString sTransformation = _T("<ImageFile-LoadImage sourcepath='") + sPath + _T("'/>");
//	BSTR bstrLoad = sTransformation.AllocSysString();
//	if( piTransform->SetXml(bstrLoad) )
//		if(piTransform->Transform())
//			piTransform->GetResult( 0, &vImage );
//	(*piImage) = vImage.punkVal;
//	SysFreeString( bstrLoad );
//	if( NULL == (*piImage) )
//		return false;
//
//	//ImageFile::IImageFile3* piImageFile = NULL;
//	//CoCreateInstance( __uuidof(ImageFile::ImageFile3) , NULL, CLSCTX_INPROC, __uuidof(ImageFile::IImageFile3), (void**)(&piImageFile));
//	//BSTR bstrFilePath = sTempFile.AllocSysString();
//	//piImageFile->SaveImage2( &m_piPicture, IMAGEFORMAT_PNG, bstrFilePath);
//	//SysFreeString( bstrFilePath );
//	return true;
//}
//bool RtfPicture::SavePicture( IUnknown* piImage, CString sPath, long nFormat )
//{
//	if( NULL == piImage )
//		return false;
//	ImageStudio::IImageTransformsPtr piTransform = NULL;
//	HRESULT hRes = piTransform.CreateInstance( ImageStudio::CLSID_ImageTransforms );
//	if( FAILED(hRes) || NULL == piTransform )
//		return false;
//	CString sFormat = _T("");
//	switch( nFormat )
//	{
//	case IMAGEFORMAT_WMF: sFormat = _T("Wmf");break;
//	case IMAGEFORMAT_PNG: sFormat = _T("Png");break;
//	case IMAGEFORMAT_JPE: sFormat = _T("Jpg");break;
//	}
//	if( _T("") == sFormat )
//		return false;
//
//	VARIANT vImage;
//	vImage.vt = VT_UNKNOWN;
//	vImage.punkVal = piImage;
//	piTransform->SetSource( 0, vImage);
//
//	CString sTransformation = _T("<ImageFile-SaveAs");
//	sTransformation.Append( sFormat );
//	sTransformation.Append( _T(" destinationpath=\"") );
//	sTransformation.Append( sPath );
//	sTransformation.Append( _T("\" format=\"8888\" />") );
//
//	BSTR bstrLoad = sTransformation.AllocSysString();
//
//	if( piTransform->SetXml(bstrLoad) )
//		if(piTransform->Transform())
//			piTransform->GetResult( 0, &vImage );
//	RELEASEINTERFACE( vImage.punkVal );
//	SysFreeString( bstrLoad );
//
//	//ImageFile::IImageFile3* piImageFile = NULL;
//	//CoCreateInstance( __uuidof(ImageFile::ImageFile3) , NULL, CLSCTX_INPROC, __uuidof(ImageFile::IImageFile3), (void**)(&piImageFile));
//	//BSTR bstrFilePath = sTempFile.AllocSysString();
//	//piImageFile->SaveImage2( &m_piPicture, IMAGEFORMAT_PNG, bstrFilePath);
//	//SysFreeString( bstrFilePath );
//	return true;
//}