
#include "VMLPictureMapping.h"

namespace DocFileFormat
{
	VMLPictureMapping::VMLPictureMapping(ConversionContext* ctx, XmlUtils::CXmlWriter* writer, bool olePreview, IMapping* caller, bool isBulletPicture) : PropertiesMapping(writer)
	{
		m_ctx				=	ctx;
		m_olePreview		=	olePreview;
		m_imageData			=	NULL;
		m_nImageId			=	0;
		m_caller			=	caller;
		m_isBulletPicture	=	isBulletPicture;

		m_imageData			=	new XMLTools::XMLElement<wchar_t>( _T( "v:imagedata" ) );

        //GUID guid			=	GUID_NULL;
//      wstring  guid;
//		LPOLESTR guidString =	NULL;

//		CoCreateGuid (&guid);
//		StringFromCLSID (guid, &guidString);

//        m_ShapeId			=	std::wstring(guidString);
//		CoTaskMemFree(guidString);
	}

	VMLPictureMapping::~VMLPictureMapping()
	{
		RELEASEOBJECT(m_imageData);
	}
}

namespace DocFileFormat
{
	void VMLPictureMapping::Apply( IVisitable* visited  )
	{
		PictureDescriptor* pict = static_cast<PictureDescriptor*>(visited);

		if (CopyPicture(pict->blipStoreEntry))
		{
			Shape* shape				=	static_cast<Shape*>(*(pict->shapeContainer->Children.begin()));
			list<OptionEntry> options	=	pict->shapeContainer->ExtractOptions();

			//v:shapetype
			PictureFrameType type;
			type.SetType(shape->Instance);
			VMLShapeTypeMapping* vmlShapeTypeMapping = new VMLShapeTypeMapping( m_pXmlWriter, m_isBulletPicture );
			type.Convert( vmlShapeTypeMapping );
			RELEASEOBJECT( vmlShapeTypeMapping );

			//v:shape
			m_pXmlWriter->WriteNodeBegin( _T( "v:shape" ), true );
			m_pXmlWriter->WriteAttribute( _T( "type" ), ( wstring( _T( "#" ) ) + VMLShapeTypeMapping::GenerateTypeId( &type ) ).c_str() );

			wstring style;

			double xScaling = pict->mx / 1000.0;
			double yScaling = pict->my / 1000.0;

			TwipsValue width( ( pict->dxaGoal - ( pict->dxaCropLeft + pict->dxaCropRight ) ) * xScaling );
			TwipsValue height( ( pict->dyaGoal - ( pict->dyaCropTop + pict->dyaCropBottom ) ) * yScaling );

			wstring widthString = FormatUtils::DoubleToWideString( width.ToPoints() );
			wstring heightString = FormatUtils::DoubleToWideString( height.ToPoints() );

			style = wstring( _T( "width:" ) ) + widthString + wstring( _T( "pt;" ) ) + wstring( _T( "height:" ) ) + heightString + wstring( _T( "pt;" ) );

			m_pXmlWriter->WriteAttribute( _T( "style" ), style.c_str() );

			m_pXmlWriter->WriteAttribute( _T( "id" ), m_ShapeId.c_str() );

			if (m_olePreview)
			{
				m_pXmlWriter->WriteAttribute( _T( "o:ole" ), _T( "" ) );
			}
			
			std::list<OptionEntry>::iterator end = options.end();
			for (std::list<OptionEntry>::iterator iter = options.begin(); iter != end; ++iter)
			{
				switch ( iter->pid )
				{
					//BORDERS

				case borderBottomColor:
					{
						RGBColor bottomColor( (int)iter->op, RedFirst );
						m_pXmlWriter->WriteAttribute( _T( "o:borderbottomcolor" ), ( wstring( _T( "#" ) ) + bottomColor.SixDigitHexCode ).c_str() );
					}
					break;

				case borderLeftColor:
					{  
						RGBColor leftColor( (int)iter->op, RedFirst );
						m_pXmlWriter->WriteAttribute( _T( "o:borderleftcolor" ), ( wstring( _T( "#" ) ) + leftColor.SixDigitHexCode ).c_str() );
					}  
					break;

				case borderRightColor:
					{  
						RGBColor rightColor( (int)iter->op, RedFirst );
						m_pXmlWriter->WriteAttribute( _T( "o:borderrightcolor" ), ( wstring( _T( "#" ) ) + rightColor.SixDigitHexCode ).c_str() );
					}
					break;

				case borderTopColor:
					{
						RGBColor topColor( (int)iter->op, RedFirst );
						m_pXmlWriter->WriteAttribute( _T( "o:bordertopcolor" ), ( wstring( _T( "#" ) ) + topColor.SixDigitHexCode ).c_str() );
					}
					break;

					//CROPPING

				case cropFromBottom:
					{  
						//cast to signed integer
						int cropBottom = (int)iter->op;
						appendValueAttribute(m_imageData, _T( "cropbottom" ), ( FormatUtils::IntToWideString( cropBottom ) + wstring( _T( "f" ) ) ).c_str() );
					}
					break;

				case cropFromLeft:
					{  
						//cast to signed integer
						int cropLeft = (int)iter->op;
						appendValueAttribute(m_imageData, _T( "cropleft" ), ( FormatUtils::IntToWideString( cropLeft ) + wstring( _T( "f" ) ) ).c_str());
					}
					break;

				case cropFromRight:
					{
						//cast to signed integer
						int cropRight = (int)iter->op;
						appendValueAttribute(m_imageData, _T( "cropright" ), ( FormatUtils::IntToWideString( cropRight ) + wstring( _T( "f" ) ) ).c_str());
					}
					break;

				case cropFromTop:
					{
						//cast to signed integer
						int cropTop = (int)iter->op;
						appendValueAttribute(m_imageData, _T( "croptop" ), ( FormatUtils::IntToWideString( cropTop ) + wstring( _T( "f" ) ) ).c_str());
					}
					break;
				}
			}

			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

			//v:imageData
			appendValueAttribute(m_imageData, _T( "r:id" ), ( wstring( _T( "rId" ) ) + FormatUtils::IntToWideString(m_nImageId) ).c_str());
			appendValueAttribute(m_imageData, _T( "o:title" ) , _T( "" ));
			m_pXmlWriter->WriteString(m_imageData->GetXMLString().c_str());

			//borders
			writePictureBorder( _T( "bordertop" ), pict->brcTop );
			writePictureBorder( _T( "borderleft" ), pict->brcLeft );
			writePictureBorder( _T( "borderbottom" ), pict->brcBottom );
			writePictureBorder( _T( "borderright" ), pict->brcRight );

			//close v:shape
			m_pXmlWriter->WriteNodeEnd( _T( "v:shape" ) );
		}
	}

	std::wstring VMLPictureMapping::GetShapeId () const
	{
		return m_ShapeId;
	}

	/// Writes a border element
	void VMLPictureMapping::writePictureBorder( const wchar_t* name, const BorderCode* brc )
	{
		m_pXmlWriter->WriteNodeBegin( ( wstring( _T( "w10:" ) ) + wstring( name ) ).c_str(), true );
		m_pXmlWriter->WriteAttribute( _T( "type" ), getBorderType( brc->brcType ).c_str() );
		m_pXmlWriter->WriteAttribute( _T( "width" ), FormatUtils::IntToWideString( brc->dptLineWidth ).c_str() );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), true );
	}

	/// Copies the picture from the binary stream to the zip archive 
	/// and creates the relationships for the image.
	bool VMLPictureMapping::CopyPicture (BlipStoreEntry* oBlipEntry)
	{
		bool result = false;

		// write the blip
		if ((oBlipEntry != NULL) && (oBlipEntry->Blip != NULL))
		{
			switch (oBlipEntry->btWin32)
			{
			case Global::msoblipEMF:
			case Global::msoblipWMF:
				{
					//it's a meta image
					MetafilePictBlip* metaBlip = static_cast<MetafilePictBlip*>(oBlipEntry->Blip);
					if (metaBlip)
					{
						//meta images can be compressed
						unsigned char* decompressed = NULL;
						int decompressedSize = 0;

						decompressedSize = metaBlip->Decompress(&decompressed);
						if (0 != decompressedSize && NULL != decompressed)
						{
							m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), std::vector<unsigned char>(decompressed, (decompressed + decompressedSize))));
							RELEASEARRAYOBJECTS(decompressed);
						}
					}
				}
				break;

			case Global::msoblipJPEG:
			case Global::msoblipCMYKJPEG:
			case Global::msoblipPNG:
			case Global::msoblipTIFF:
			case Global::msoblipDIB:
				{
					BitmapBlip* bitBlip = static_cast<BitmapBlip*>(oBlipEntry->Blip);
					if (bitBlip)
					{
						m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), 
							vector<unsigned char>(bitBlip->m_pvBits, (bitBlip->m_pvBits + bitBlip->pvBitsSize)), oBlipEntry->btWin32));
					}
				}
				break;

			default:
				{
					result = false;

					return result;
				}
				break;
			}

			m_nImageId	=	m_ctx->_docx->RegisterImage(m_caller, oBlipEntry->btWin32);

			result		=	true;
		}

		return result;
	}
}

namespace DocFileFormat
{
	std::wstring VMLPictureMapping::GetTargetExt (Global::BlipType nType)
	{
		switch (nType)
		{
			//case Global::msoblipDIB:
			//	return std::wstring( _T( ".bmp" ) );

			//case msoblipBMP:
			//  return wstring( _T( ".bmp" ) );

		case Global::msoblipEMF:
			return std::wstring(_T(".emf"));

			//case msoblipGIF:
			//  return wstring( _T( ".gif" ) );

			//case msoblipICON:
			//  return wstring( _T( ".ico" ) );

		case Global::msoblipJPEG:
		case Global::msoblipCMYKJPEG:
			return std::wstring(_T(".jpg"));

			//case msoblipPCX:
			//  return wstring( _T( ".pcx" ) );

		case Global::msoblipPNG:
			return std::wstring(_T(".png"));

		case Global::msoblipTIFF:
			return std::wstring(_T(".tif"));

		case Global::msoblipWMF:
			return std::wstring(_T(".wmf"));

		default:
			return std::wstring(_T(".png"));
		}
	}

	std::wstring VMLPictureMapping::GetContentType (Global::BlipType nType)
	{
		switch (nType)
		{
			//case msoblipBMP:
			//  return wstring( _T( "image/bmp" ) );

		case Global::msoblipEMF:
			return std::wstring(OpenXmlContentTypes::Emf);

			//case msoblipGIF:
			//  return wstring( _T( "image/gif" ) );

			//case msoblipICON:
			//  return wstring( _T( "image/x-icon" ) );

		case Global::msoblipJPEG:
		case Global::msoblipCMYKJPEG:
			return std::wstring(OpenXmlContentTypes::Jpeg);

			//case msoblipPCX:
			//  return wstring( _T( "image/pcx" ) );

		case Global::msoblipPNG:
			return std::wstring(OpenXmlContentTypes::Png);

		case Global::msoblipTIFF:
			return std::wstring(OpenXmlContentTypes::Tiff);

		case Global::msoblipWMF:
			return std::wstring(OpenXmlContentTypes::Wmf);

		default:
			return std::wstring(OpenXmlContentTypes::Png);
		}
	}
}
