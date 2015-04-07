#include "SvmConverter.h"
#include "OfficeSvmFile.h"
#include "ATLDefine.h"

#define USE_IMAGESTUDIO
#ifdef USE_IMAGESTUDIO
#import "../../../Redist/ASCImageStudio3.dll"		rename_namespace("ImageStudio")
#endif

void ConvertSvmToImage( const boost::filesystem::wpath & oSource, const boost::filesystem::wpath & oDestination )
{
#ifdef USE_IMAGESTUDIO
	std::wstring sXmlExt = L"";
	std::wstring sExt = oDestination.extension();
	std::transform( sExt.begin(), sExt.end(), sExt.begin(), tolower);
	if( L".png" == sExt )
		sXmlExt = L"Png";
	else if( L".jpg" == sExt || L"jpeg" == sExt  )
		sXmlExt = L"Jpeg";
	else if( L".bmp" == sExt )
		sXmlExt = L"Bmp";
	else if( L".gif" == sExt )
		sXmlExt = L"Gif";
	else if( L".tif" == sExt || L"tiff" == sExt )
		sXmlExt = L"Tiff";
	else if( L".wmf" == sExt )
		sXmlExt = L"Wmf";
	else if( L".emf" == sExt )
		sXmlExt = L"Emf";
	else if( L".pcx" == sExt )
		sXmlExt = L"Pcx";
	else if( L".tga" == sExt )
		sXmlExt = L"Tga";
	else if( L".ras" == sExt )
		sXmlExt = L"Ras";
	else if( L".ipod" == sExt )
		sXmlExt = L"IPod";
	else if( L".psd" == sExt )
		sXmlExt = L"Psd";
 
	if( L"" == sXmlExt )
		return;
	
	CoInitialize( NULL );
	OfficeSvmFile oSvmFile;
	BSTR bstrSource = SysAllocString(oSource.string().c_str()) ;
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
	sXml.append( oDestination.string() );
	sXml.append( L"\" format=\"8888\"/>" );
	if( piTransform->SetXml( sXml.c_str() ) )
		if(piTransform->Transform())
			piTransform->GetResult( 0, &vImage );
	RELEASEINTERFACE( vImage.punkVal );
	RELEASEINTERFACE( piTransform );
	RELEASEINTERFACE( piImage );

	oSvmFile.Close();
	CoUninitialize();
#endif
}