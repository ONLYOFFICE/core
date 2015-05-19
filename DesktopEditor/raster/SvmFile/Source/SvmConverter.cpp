#include "SvmConverter.h"
#include "SvmFile.h"
#include "ATLDefine.h"

#import "../../../Redist/ASCImageStudio3.dll"		rename_namespace("ImageStudio")

void ConvertSvmToImage( const std::wstring & oSource, const std::wstring & oDestination )
{
	std::wstring sXmlExt = L"Jpeg";

	CoInitialize( NULL );
	OfficeSvmFile oSvmFile;
	BSTR bstrSource = SysAllocString(oSource.c_str()) ;
	int nRes = oSvmFile.Open(bstrSource);
	SysFreeString(bstrSource);
	if( NOERROR != nRes )
		return;
	MediaCore::IAVSUncompressedVideoFrame* piImage = NULL;
	piImage = oSvmFile.GetThumbnail();
	if( NULL == piImage )
		return;

	ImageStudio::IImageTransforms* piTransform = NULL;
	HRESULT hRes = CoCreateInstance( __uuidof( ImageStudio::ImageTransforms ), NULL, CLSCTX_INPROC, __uuidof( ImageStudio::IImageTransforms ), (void**)(&piTransform));
	if( NULL == piTransform )
		return;
	VARIANT vImage;
	vImage.punkVal = piImage;
	vImage.vt = VT_UNKNOWN;
	piTransform->SetSource( 0, vImage );
	
	std::wstring sXml;
	sXml.append( L"<ImageFile-SaveAs" );
	sXml.append( sXmlExt );
	sXml.append( L" destinationpath=\"" );
	sXml.append( oDestination );
	sXml.append( L"\" format=\"8888\"/>" );
	
	if( piTransform->SetXml( sXml.c_str() ) )
		if(piTransform->Transform())
			piTransform->GetResult( 0, &vImage );

	RELEASEINTERFACE( vImage.punkVal );
	RELEASEINTERFACE( piTransform );
	RELEASEINTERFACE( piImage );

	oSvmFile.Close();
	CoUninitialize();
}