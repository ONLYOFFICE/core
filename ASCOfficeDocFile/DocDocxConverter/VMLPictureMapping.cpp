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

#include "VMLPictureMapping.h"

#include "OfficeDrawing/GeometryBooleanProperties.h"
#include "OfficeDrawing/GeometryTextBooleanProperties.h"

#include "../../DesktopEditor/common/String.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Document.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Document.h"


namespace DocFileFormat
{
	bool ParseEmbeddedEquation( const std::string & xmlString, std::wstring & newXmlString)
	{
		newXmlString.clear();
		std::wstring sTempXmlFile = FileSystem::Directory::CreateTempFileWithUniqueName(
                                                    FileSystem::Directory::GetTempPath(), L"emb");

		sTempXmlFile += L".xml";
		
		NSFile::CFileBinary file; 
		file.CreateFileW(sTempXmlFile);
		file.WriteFile((BYTE*)xmlString.c_str(), xmlString.size());
		file.CloseFile();

		OOX::CPath path(sTempXmlFile.c_str());
		OOX::CDocument docEmbedded(path, path);

		bool res = false;
		for (int i = 0 ; i < docEmbedded.m_arrItems.size(); i++)
		{
			if (docEmbedded.m_arrItems[i]->getType() == OOX::et_w_p)
			{
				OOX::Logic::CParagraph *paragraph = dynamic_cast<OOX::Logic::CParagraph *>(docEmbedded.m_arrItems[i]);

				for (int j = 0; (paragraph) && (j < paragraph->m_arrItems.size()); j++)
				{
					if (paragraph->m_arrItems[j]->getType() == OOX::et_m_oMath)
					{
						res = true;
						newXmlString = paragraph->m_arrItems[j]->toXML().GetBuffer();
						break;
					}
					else if (paragraph->m_arrItems[j]->getType() == OOX::et_m_oMathPara)
					{
						OOX::Logic::COMathPara *mathPara = dynamic_cast<OOX::Logic::COMathPara *>(paragraph->m_arrItems[j]);
						
						for (int k = 0; (mathPara) && (k < mathPara->m_arrItems.size()); k++)
						{
							if (mathPara->m_arrItems[k]->getType() == OOX::et_m_oMath)
							{
								res = true;
								newXmlString = mathPara->m_arrItems[k]->toXML().GetBuffer();
								break;
							}
						}
					}
					if (res) break;
				}
				if (res) break;
			}
		}
		return res;
	}
//---------------------------------------------------------------
	static int count_vml_objects = 0;
	void VMLPictureMapping::appendStyleProperty(std::wstring* b, const std::wstring& propName, const std::wstring& propValue) const
	{
		if ( b != NULL )
		{
			b->operator += ( propName );
			b->operator += ( _T( ":" ) );
			b->operator += ( propValue );
			b->operator +=( _T( ";" ) );
		}
	}

	VMLPictureMapping::VMLPictureMapping(ConversionContext* ctx, XmlUtils::CXmlWriter* writer, bool olePreview, IMapping* caller, bool isBulletPicture) : PropertiesMapping(writer)
	{
		m_ctx				=	ctx;
		m_isOlePreview		=	olePreview;
		m_imageData			=	NULL;
		m_nImageId			=	0;
		m_caller			=	caller;
		m_isBulletPicture	=	isBulletPicture;
		
		m_isEquation		=	false;
		m_isEmbedded		=	false;

		m_imageData			=	new XMLTools::XMLElement<wchar_t>( _T( "v:imagedata" ) );

		m_ShapeId			=	std::wstring(L"_x0000_i") + FormatUtils::IntToWideString(1024 + (count_vml_objects++));
	}

	VMLPictureMapping::~VMLPictureMapping()
	{
		RELEASEOBJECT(m_imageData);
	}

	void VMLPictureMapping::Apply( IVisitable* visited  )
	{
		PictureDescriptor* pict = static_cast<PictureDescriptor*>(visited);

		if ((pict->shapeContainer || pict->blipStoreEntry) && pict->shapeContainer->Children.size() > 0)
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

			if (m_isOlePreview)
			{
				m_pXmlWriter->WriteAttribute( _T( "o:ole" ), _T( "" ) );
			}
			else if (m_isBulletPicture)
			{
                m_pXmlWriter->WriteAttribute( _T( "o:bullet" ), _T( "1" ) );
			}
			
			std::list<OptionEntry>::iterator end = options.end();
			for (std::list<OptionEntry>::iterator iter = options.begin(); iter != end; ++iter)
			{
				switch ( iter->pid )
				{
				case wzEquationXML:
					{
						m_isEquation = true;
						m_isEmbedded = true;
						
						m_embeddedData = std::string((char*)iter->opComplex, iter->op);

						if (ParseEmbeddedEquation( m_embeddedData, m_equationXml))
						{
							m_isEmbedded = false;
						}
					}break;
				case metroBlob:
					{
						//встроенная неведомая хуйня
						m_isEmbedded = true;
						m_embeddedData = std::string((char*)iter->opComplex, iter->op);
					}break;
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

			if (CopyPicture(pict->blipStoreEntry))
			{
				//v:imageData
				appendValueAttribute(m_imageData, _T( "r:id" ), ( wstring( _T( "rId" ) ) + FormatUtils::IntToWideString(m_nImageId) ).c_str());
				appendValueAttribute(m_imageData, _T( "o:title" ) , _T( "" ));
				m_pXmlWriter->WriteString(m_imageData->GetXMLString().c_str());
			}

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
		m_pXmlWriter->WriteNodeEnd	( _T( "" ), true );
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
					{//decompress inside MetafilePictBlip
						unsigned char *newData	= NULL;
						int newDataSize = metaBlip->oMetaFile.ToBuffer(newData);
						
						m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), std::vector<unsigned char>(newData, (newData + newDataSize))));
						
						RELEASEARRAYOBJECTS(newData);
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
