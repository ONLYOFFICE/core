

#include "VMLShapeMapping.h"
#include "TextboxMapping.h"

#include "OfficeDrawing/Shapetypes/LineType.h"
#include "OfficeDrawing/Shapetypes/OvalType.h"
#include "OfficeDrawing/Shapetypes/RectangleType.h"
#include "OfficeDrawing/Shapetypes/RoundedRectangleType.h"

#include "OfficeDrawing/OfficeArtClientTextbox.h"
#include "OfficeDrawing/DiagramBooleanProperties.h"
#include "OfficeDrawing/GeometryBooleanProperties.h"
#include "OfficeDrawing/ShadowStyleBooleanProperties.h"
#include "OfficeDrawing/GeometryBooleanProperties.h"
#include "OfficeDrawing/FillStyleBooleanProperties.h"
#include "OfficeDrawing/GeometryBooleanProperties.h"
#include "OfficeDrawing/FillStyleBooleanProperties.h"
#include "OfficeDrawing/LineStyleBooleanProperties.h"
#include "OfficeDrawing/GeometryTextBooleanProperties.h"
#include "OfficeDrawing/GroupShapeBooleanProperties.h"
#include "OfficeDrawing/ProtectionBooleanProperties.h"

#include "../../DesktopEditor/common/String.h"

namespace DocFileFormat
{
	VMLShapeMapping::VMLShapeMapping (ConversionContext* pConv, XmlUtils::CXmlWriter* pWriter, Spa* pSpa, PictureDescriptor* pPicture, IMapping* pCaller, bool bullet) : PropertiesMapping(pWriter)
	{		
		m_bBullet			=	bullet;

		m_pSpa				=	pSpa;
		m_pCaller			=	pCaller;
		m_pBlipStore		=	NULL;

		m_ctx				=	pConv;
		m_pict				=	pPicture;
		m_nImageId			=	0;

		m_imagedata			=	XMLTools::XMLElement<wchar_t>(_T("v:imagedata"));
		m_fill				=	XMLTools::XMLElement<wchar_t>(_T("v:fill"));
		m_stroke			=	XMLTools::XMLElement<wchar_t>(_T("v:stroke"));
		m_shadow			=	XMLTools::XMLElement<wchar_t>(_T("v:shadow"));
		m_3dstyle			=	XMLTools::XMLElement<wchar_t>(_T("o:extrusion"));
		m_textpath			=	XMLTools::XMLElement<wchar_t>(_T("v:textpath"));

		Record* recBs		=	NULL;
		if ((m_ctx) && (m_ctx->_doc))
		{
			OfficeArtContent* officeArt = m_ctx->_doc->GetOfficeArt();
			if (officeArt)
			{
				const DrawingGroup* group = officeArt->GetDrawingGroup();
				if (group)
				{
					recBs = group->FirstChildWithType<BlipStoreContainer>();
				}
			}
		}

		if (recBs)
		{
			m_pBlipStore	=	static_cast<BlipStoreContainer*>(recBs);
		}
	}

	VMLShapeMapping::~VMLShapeMapping()
	{
	}
}

namespace DocFileFormat
{
	void VMLShapeMapping::Apply(IVisitable* visited)
	{
		ShapeContainer* pContainer = static_cast<ShapeContainer*>(visited);
		if ((pContainer != NULL) && (!pContainer->Children.empty()))
		{
			Record* firstRecord = pContainer->Children[0];
			if (firstRecord != NULL)
			{
				if (typeid(*firstRecord) == typeid(Shape))
				{
					// It's a single shape
					WriteShape(pContainer);
				}
				else if (typeid(*firstRecord) == typeid(GroupShapeRecord))
				{ 
					// Its a group of shapes
					WriteGroup(static_cast<GroupContainer*>(pContainer->_ParentRecord));
				}
			}
		}
	}

	/// Converts a group of shapes
	void VMLShapeMapping::WriteGroup (const GroupContainer* container)
	{
		if ((container != NULL) && (!container->Children.empty()))
		{
			ShapeContainer* groupShape		=	static_cast<ShapeContainer*>(container->Children[0]);
			GroupShapeRecord* gsr			=	static_cast<GroupShapeRecord*>(groupShape->Children[0]);
			Shape* shape					=	static_cast<Shape*>(groupShape->Children[1]);
			std::list<OptionEntry> options	=	groupShape->ExtractOptions();
			ChildAnchor* anchor				=	groupShape->FirstChildWithType<ChildAnchor>();

			m_pXmlWriter->WriteNodeBegin( _T( "v:group" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "id" ), GetShapeID(shape).c_str());
			m_pXmlWriter->WriteAttribute( _T( "style" ), FormatUtils::XmlEncode(buildStyle(shape, anchor, options, container->Index)).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "coordorigin" ), ( FormatUtils::IntToWideString(gsr->rcgBounds.topLeftAngle.x) + _T( "," ) + FormatUtils::IntToWideString( gsr->rcgBounds.topLeftAngle.y)).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "coordsize" ), ( FormatUtils::IntToWideString(gsr->rcgBounds.size.cx) + _T( "," ) + FormatUtils::IntToWideString(gsr->rcgBounds.size.cy)).c_str() );

			// Write wrap coords			
			std::list<OptionEntry>::const_iterator end = options.end();
			for (std::list<OptionEntry>::const_iterator iter = options.begin(); iter != end; ++iter)
			{
				switch (iter->pid)
				{
				case pWrapPolygonVertices:
					{
						std::wstring wrapCoords	= getWrapCoords(*iter);
						if (wrapCoords.length())
							m_pXmlWriter->WriteAttribute(_T( "wrapcoords" ), wrapCoords.c_str());
					}
					break;
				}
			}

			m_pXmlWriter->WriteNodeEnd(_T( "" ), TRUE, FALSE);

			// Convert the shapes/groups in the group
			
			size_t count = container->Children.size();
			for (size_t i = 1; i < count; ++i)
			{
				if (container->Children[i])
				{
					if (typeid(*container->Children[i]) == typeid(ShapeContainer))
					{
						ShapeContainer* pChildShape	= static_cast<ShapeContainer*>(container->Children[i]);
						if (pChildShape)
						{
							VMLShapeMapping vmlShapeMapping(m_ctx, m_pXmlWriter, m_pSpa, NULL,  m_pCaller);
							pChildShape->Convert(&vmlShapeMapping);
						}
					}
					else if (typeid(*container->Children[i]) == typeid(GroupContainer))
					{
						GroupContainer* pChildGroup = static_cast<GroupContainer*>(container->Children[i]);
						m_pSpa = NULL;
						WriteGroup(pChildGroup);
					}
				}
			}

			// Write wrap

			if (m_pSpa)
			{
				std::wstring wrap = getWrapType(m_pSpa);
				if (wrap != _T( "through" ))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w10:wrap" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "type" ), wrap.c_str() );
					m_pXmlWriter->WriteNodeEnd	( _T( "w10:wrap" ), TRUE );
				}
			}

			m_pXmlWriter->WriteNodeEnd( _T( "v:group" ) );
		}
	}

	/// Converts a single shape
	void VMLShapeMapping::WriteShape(const ShapeContainer* pContainer)
	{
		if ((NULL != pContainer) && (!pContainer->Children.empty()))
		{
			Shape* pShape						=	static_cast<Shape*>(pContainer->Children[0]);
			if (pShape)
			{
				bool freeform					=	true;

				std::list<OptionEntry> options	=	pContainer->ExtractOptions();
				ChildAnchor* pAnchor			=	pContainer->FirstChildWithType<ChildAnchor>();
				ClientAnchor* clientAnchor		=	pContainer->FirstChildWithType<ClientAnchor>();

				WriteBeginShapeNode (pShape);

				m_pXmlWriter->WriteAttribute ( _T( "id"), GetShapeID(pShape).c_str());

				if (pShape->GetShapeType())
				{
					freeform					=	false;
					m_pXmlWriter->WriteAttribute( _T("type"), (std::wstring(_T("#")) + VMLShapeTypeMapping::GenerateTypeId(pShape->GetShapeType())).c_str());
				}

				m_pXmlWriter->WriteAttribute( _T("style"), FormatUtils::XmlEncode(buildStyle(pShape, pAnchor, options, pContainer->Index)).c_str());

				if (pShape->is<LineType>())
				{
					//append "from" and  "to" attributes
					m_pXmlWriter->WriteAttribute(_T("from"), GetLineFrom(pAnchor).c_str());
					m_pXmlWriter->WriteAttribute(_T("to"),	GetLineTo(pAnchor).c_str());
				}

				if (m_bBullet)
				{
                     m_pXmlWriter->WriteAttribute(_T("o:bullet"), _T("t"));
				}

				
				EmuValue ShadowOffsetX;
				EmuValue ShadowOffsetY;
				EmuValue SecondShadowOffsetX;
				EmuValue SecondShadowOffsetY;
				EmuValue ViewPointX;
				EmuValue ViewPointY;
				EmuValue ViewPointZ;

				double viewPointOriginX	=	0;
				double viewPointOriginY	=	0;
				double ShadowOriginX	=	0;
				double ShadowOriginY	=	0;
				unsigned int xCoord		=	0;
				unsigned int yCoord		=	0;
				bool stroked			=	true;
				bool filled				=	true;
				bool hasTextbox			=	false;
				bool layoutInCell		=	false;
				
				int ndxTextLeft			=	-1;
				int ndyTextTop			=	-1;
				int ndxTextRight		=	-1;
				int ndyTextBottom		=	-1;

				bool bHavePath			=	false;
				int	nAdjValues			=	0;
				int	nLTxID				=	-1;

				std::wstring sTextboxStyle;
				
				std::wstring					adjValues[8];
				ShadowStyleBooleanProperties	shadowBoolean(0);
				std::vector<CString>			arrInscribe;

				std::list<OptionEntry>::const_iterator end = options.end();
				for (std::list<OptionEntry>::const_iterator iter = options.begin(); iter != end; ++iter)
				{
					switch (iter->pid)
					{
			//BOOLEANS
					case geometryBooleans:
						{
							GeometryBooleanProperties booleans(iter->op);
							if (booleans.fUsefLineOK && !booleans.fLineOK)
							{
								stroked	=	false;
							}

							if (!(booleans.fUsefFillOK && booleans.fFillOK))
							{
								filled	=	false;
							}
						}
						break;

					case fillStyleBooleanProperties:
						{
							FillStyleBooleanProperties booleans(iter->op);
							if (booleans.fUsefFilled && !booleans.fFilled)
							{
								filled = false;
							}
						}
						break;

					case lineStyleBooleans:
						{
							LineStyleBooleanProperties booleans(iter->op);
							if (booleans.fUsefLine && !booleans.fLine)
							{
								stroked = false;
							}
						}
						break;

					case protectionBooleans:
						{
							ProtectionBooleanProperties booleans(iter->op);
						}
						break;

					case diagramBooleans:
						{
							DiagramBooleanProperties booleans(iter->op);
						}
						break;

					case groupShapeBooleans:
						{
							GroupShapeBooleanProperties booleans(iter->op);
							if (booleans.fUsefLayoutInCell)
							{
								layoutInCell = booleans.fLayoutInCell;
							}
						}
						break;

						// GEOMETRY

					case adjustValue:
						{
							adjValues[0]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,1);
						}
						break;

					case adjust2Value:
						{
							adjValues[1]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,2);
						}
						break;

					case adjust3Value:
						{
							adjValues[2]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,3);
						}
						break;

					case adjust4Value:
						{
							adjValues[3]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,4);
						}
						break;

					case adjust5Value:
						{
							adjValues[4]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,5);
						}
						break;

					case adjust6Value:
						{
							adjValues[5]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,6);
						}
						break;

					case adjust7Value:
						{
							adjValues[6]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,7);
						}
						break;

					case adjust8Value:
						{
							adjValues[7]	=	FormatUtils::IntToWideString( (int)iter->op );
                            nAdjValues		=	(std::max)(nAdjValues,8);
						}
						break;

					case pWrapPolygonVertices:
						{
							std::wstring wrapCoords = getWrapCoords(*iter);

							if (!wrapCoords.empty())
							{
								m_pXmlWriter->WriteAttribute( _T( "wrapcoords" ), wrapCoords.c_str() );
							}
						}
						break;

					case geoRight:
						{
							xCoord = iter->op;
						}
						break;

					case geoBottom:
						{
							yCoord = iter->op;
						}
						break;

					case pGuides:
						{
						}
						break;

					case pInscribe:
						{
							arrInscribe = GetTextRectangles(*iter);
						}
						break;

						// OUTLINE

					case lineColor:
						{
							RGBColor lineColor((int)iter->op, RedFirst);
							m_pXmlWriter->WriteAttribute( _T("strokecolor"), (wstring(_T("#")) + lineColor.SixDigitHexCode).c_str());
						}
						break;

					case lineWidth:
						{
							EmuValue eLineWidth ((int)iter->op );
							CString sWidth; sWidth.Format(_T("%fpt"), eLineWidth.ToPoints());
							m_pXmlWriter->WriteAttribute(_T("strokeweight"), sWidth);
						}
						break;

					case lineDashing:
						{
							appendValueAttribute(&m_stroke, _T( "dashstyle" ), FormatUtils::MapValueToWideString( iter->op, &Global::DashStyleMap[0][0], 11, 16 ).c_str() );
						}
						break;

					case lineStyle:
						{
							appendValueAttribute(&m_stroke, _T( "linestyle" ), getLineStyle( iter->op ).c_str());
						}
						break;

					case lineEndArrowhead:
						{
							appendValueAttribute(&m_stroke, _T( "endarrow" ), getArrowStyle( iter->op ).c_str());
						}
						break;

					case lineEndArrowLength:
						{
							appendValueAttribute(&m_stroke, _T( "endarrowlength" ), getArrowLength( iter->op ).c_str());
						}
						break;

					case lineEndArrowWidth:
						{
							appendValueAttribute(&m_stroke, _T( "endarrowwidth" ), getArrowWidth( iter->op ).c_str());
						}
						break;

					case lineStartArrowhead:
						{
							appendValueAttribute(&m_stroke, _T( "startarrow" ), getArrowStyle( iter->op ).c_str());
						}
						break;

					case lineStartArrowLength:
						{
							appendValueAttribute(&m_stroke, _T( "startarrowlength" ), getArrowLength( iter->op ).c_str());
						}
						break;

					case lineStartArrowWidth:
						{
							appendValueAttribute(&m_stroke, _T( "startarrowwidth" ), getArrowWidth( iter->op ).c_str());
						}
						break;

						// FILL

					case fillColor:
						{
							RGBColor fillColor((int)iter->op, RedFirst);
							m_pXmlWriter->WriteAttribute(_T( "fillcolor" ), ( std::wstring( _T( "#" ) ) + fillColor.SixDigitHexCode ).c_str());
						}
						break;

					case fillBackColor:
						{
							RGBColor fillBackColor( (int)iter->op, RedFirst );
							appendValueAttribute(&m_fill, _T( "color2" ), ( std::wstring( _T( "#" ) ) + fillBackColor.SixDigitHexCode ).c_str());
						}
						break;

					case fillAngle:
						{
							FixedPointNumber fllAngl( iter->op );
							appendValueAttribute(&m_fill, _T( "angle" ), FormatUtils::DoubleToWideString( fllAngl.ToAngle() ).c_str());
						}
						break;

					case fillShadeType:
						{
							appendValueAttribute(&m_fill, _T( "method" ), getFillMethod( iter->op ).c_str());
						}
						break;

					case fillShadeColors:
						{
							appendValueAttribute(&m_fill, _T( "colors" ), getFillColorString( iter->opComplex, iter->op ).c_str());
						}
						break;

					case fillFocus:
						{
							appendValueAttribute(&m_fill, _T( "focus" ), ( FormatUtils::IntToWideString( iter->op ) + _T( "%" ) ).c_str());
						}
						break;

					case fillType:
						{
							appendValueAttribute(&m_fill, _T( "type" ), getFillType( iter->op ).c_str());
						}
						break;

					case fillBlip:
						{
							BlipStoreEntry* pFillBlip = NULL;

							if ((m_pict != NULL ) && (m_pict->blipStoreEntry != NULL ))
							{
								// Word Art Texture
								//fillBlip = this->_pict->BlipStoreEntry;
							}
							else if ( (m_pBlipStore != NULL) && ( (iter->op - 1) < m_pBlipStore->Children.size() ) )
							{
								pFillBlip = static_cast<BlipStoreEntry*>(m_pBlipStore->Children[iter->op - 1]);
							}

							if ( (pFillBlip != NULL) && copyPicture(pFillBlip) )
							{
								appendValueAttribute(&m_fill, _T( "r:id" ), ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString(m_nImageId) ).c_str());
								appendValueAttribute(&m_imagedata, _T( "o:title" ), _T( "" ));
							}
						}
						break;

					case fillOpacity:
						{
							appendValueAttribute(&m_fill, _T( "opacity" ), ( FormatUtils::IntToWideString( iter->op ) + _T( "f" ) ).c_str());
						}
						break;
					case fillBackOpacity:
						{
							appendValueAttribute(&m_fill, _T("opacity2"), (FormatUtils::IntToWideString(iter->op) + _T("f")).c_str());
						}
						break;

						// SHADOW

					case shadowType:
						{
							appendValueAttribute(&m_shadow, _T("type"), getShadowType(iter->op).c_str());
						}
						break;

					case shadowColor:
						{
							RGBColor shadowColor((int)iter->op, RedFirst);
							appendValueAttribute(&m_shadow, _T( "color" ), ( std::wstring( _T( "#" ) ) + shadowColor.SixDigitHexCode ).c_str());
						}
						break;

					case shadowOffsetX:
						{
							ShadowOffsetX = EmuValue( (int)iter->op );
						}
						break;

					case shadowSecondOffsetX:
						{
							SecondShadowOffsetX = EmuValue( (int)iter->op );
						}
						break;

					case shadowOffsetY:
						{
							ShadowOffsetY = EmuValue( (int)iter->op );
						}
						break;

					case shadowSecondOffsetY:
						{
							SecondShadowOffsetY = EmuValue( (int)iter->op );
						}
						break;

					case shadowOriginX:
						{
							ShadowOriginX = ( iter->op / pow( (double)2, (double)16 ) );
						}
						break;

					case shadowOriginY:
						{
							ShadowOriginY = (iter->op / pow( (double)2, (double)16));
						}
						break;

					case shadowOpacity:
						{
							double shadowOpa = (iter->op / pow( (double)2, (double)16));

							appendValueAttribute(&m_shadow, _T( "opacity" ), FormatUtils::DoubleToFormattedWideString( shadowOpa, _T( "%.2f" ) ).c_str());
						}
						break;

					case shadowStyleBooleanProperties:
						{
							shadowBoolean	=	ShadowStyleBooleanProperties(iter->op);
						}
						break;

						// PICTURE

					case Pib:
						{
							int index = (int)( iter->op - 1 );

							if ((m_pBlipStore != NULL) && (index < (int)m_pBlipStore->Children.size()))
							{
								BlipStoreEntry* oBlip = static_cast<BlipStoreEntry*>(m_pBlipStore->Children[index]);
								if (copyPicture(oBlip))
								{
									appendValueAttribute(&m_imagedata, _T( "r:id" ), ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString(m_nImageId) ).c_str());
								}
							}
						}
						break;

					case pibName:
						{
							std::wstring name;
							FormatUtils::GetSTLCollectionFromBytes<std::wstring>(&name, iter->opComplex, iter->op, ENCODING_UNICODE);
							appendValueAttribute(&m_imagedata, _T( "o:title" ), FormatUtils::XmlEncode(name).c_str());
						}
						break;

						// 3D STYLE

					case f3D:
					case threeDStyleBooleanProperties:
					case threeDObjectBooleanProperties:
						break;

					case c3DExtrudeBackward:
						{
							EmuValue backwardValue( (int)iter->op );
							appendValueAttribute(&m_3dstyle, _T( "backdepth" ), FormatUtils::DoubleToWideString( backwardValue.ToPoints() ).c_str());
						}
						break; 

					case c3DSkewAngle:
						{
							FixedPointNumber skewAngle( iter->op );
							appendValueAttribute(&m_3dstyle, _T( "skewangle" ), FormatUtils::DoubleToWideString( skewAngle.ToAngle() ).c_str());
						}
						break;

					case c3DXViewpoint:
						{
							ViewPointX = EmuValue( FixedPointNumber( iter->op ).Integral );
						}
						break;

					case c3DYViewpoint:
						{
							ViewPointY = EmuValue( FixedPointNumber( iter->op ).Integral );
						}
						break;

					case c3DZViewpoint:
						{
							ViewPointZ = EmuValue( FixedPointNumber( iter->op ).Integral );
						}
						break;

					case c3DOriginX:
						{
							FixedPointNumber dOriginX( iter->op );
							viewPointOriginX = ( dOriginX.Integral / 65536.0 );
						}
						break;

					case c3DOriginY:
						{
							FixedPointNumber dOriginY( iter->op );
							viewPointOriginY = (dOriginY.Integral / 65536.0 );
						}
						break;

			// TEXTBOX

					case lTxid:
						{
							hasTextbox	=	true;
							nLTxID		=	(((iter->op) >> 16) & 0xFFFF);
						}
						break;
					case dxTextLeft:	{ndxTextLeft	= (int)iter->op;break;}
					case dyTextTop:		{ndyTextTop		= (int)iter->op;break;}
					case dxTextRight:	{ndxTextRight	= (int)iter->op;break;}
					case dyTextBottom:	{ndyTextBottom	= (int)iter->op;break;}
					case txflTextFlow:
					{
						switch(iter->op)
						{
						case 0:
						case 4://обычный 							
							break;
						case 1:
						case 5://верт (склони голову направо)						
							appendStyleProperty(&sTextboxStyle, L"layout-flow", L"vertical");
							break;
						case 2://верт (склони голову налево)	
							appendStyleProperty(&sTextboxStyle, L"layout-flow", L"vertical");
							appendStyleProperty(&sTextboxStyle, L"mso-layout-flow-alt", L"bottom-to-top");
							break;
						}
					}break;	
	// TEXT PATH (Word Art)

					case gtextUNICODE:
					{
						std::wstring text = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)iter->opComplex, (iter->op)/2);

						text = FormatUtils::XmlEncode(text);
						text = ReplaceString(text, _T("\n"), _T("&#xA;"));
						appendValueAttribute(&m_textpath, L"string", text.c_str());
					}break;

					case gtextFont:
					{
						std::wstring font = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)iter->opComplex, (iter->op)/2);
						font = std::wstring(_T("\"")) + font + std::wstring(_T("\""));
						appendStyleProperty(&m_textPathStyle, L"font-family", font);
					}break;
					case gtextSize:
					{
						std::wstring fontSize = FormatUtils::IntToWideString(iter->op/65535);
						appendStyleProperty(&m_textPathStyle, L"font-size", fontSize + L"pt");
					}break;
					case gtextSpacing:
					{
						std::wstring spacing = FormatUtils::IntToWideString(iter->op);
						appendStyleProperty(&m_textPathStyle, L"v-text-spacing", spacing + L"f");
					}break;
					case geometryTextBooleanProperties:
					{
						GeometryTextBooleanProperties props(iter->op);
						if (props.fUsegtextFBestFit && props.gtextFBestFit)
						{
							appendValueAttribute(&m_textpath, L"fitshape", _T("t"));
						}
						if (props.fUsegtextFShrinkFit && props.gtextFShrinkFit)
						{
							appendValueAttribute(&m_textpath, L"trim", _T("t"));
						}
						if (props.fUsegtextFVertical && props.gtextFVertical)
						{
							appendStyleProperty(&m_textPathStyle, L"v-rotate-letters", L"t");
							//_twistDimension = true;
						}
						if (props.fUsegtextFKern && props.gtextFKern)
						{
							appendStyleProperty(&m_textPathStyle, L"v-text-kern", L"t");
						}
						if (props.fUsegtextFItalic && props.gtextFItalic)
						{
							appendStyleProperty(&m_textPathStyle, L"font-style", L"italic");
						}
						if (props.fUsegtextFBold && props.gtextFBold)
						{
							appendStyleProperty(&m_textPathStyle, L"font-weight", L"bold");
						}
					}break;

		  // PATH
					case shapePath :
					{
						bHavePath			=	true;

						std::wstring path	=	ParsePath(options);

						if (false == path.empty())
							m_pXmlWriter->WriteAttribute (_T( "path" ), path.c_str());
					}break;
					}
				}

				if (false == bHavePath)		//	фигура может быть задана только наборами вершин и индексов
				{
					std::wstring path	=	ParsePath(options);

					if (false == path.empty())
						m_pXmlWriter->WriteAttribute (_T( "path" ), path.c_str());
				}

				if ( !filled )
				{
					m_pXmlWriter->WriteAttribute( _T( "filled" ), _T( "f" ) );
				}

				if ( !stroked )
				{
					m_pXmlWriter->WriteAttribute( _T( "stroked" ), _T( "f" ) );
				}

				if (!layoutInCell) 
				{
					m_pXmlWriter->WriteAttribute(_T("o:allowincell"), _T("f"));
				}

				if ( ( xCoord > 0 ) && ( yCoord > 0 ) )
				{
					m_pXmlWriter->WriteAttribute( _T( "coordsize" ), ( FormatUtils::IntToWideString( xCoord ) + _T( "," ) + FormatUtils::IntToWideString( yCoord ) ).c_str() );
				}

				///	<!--		DOCX TAG	'adj'		-->

				int nCode	=	0;
				if (pShape->GetShapeType())
				{
					nCode	=	pShape->GetShapeType()->GetTypeCode();
				}

				if (DocFileFormat::msosptRoundRectangle == nCode)
				{
					if (nAdjValues)												
					{
						m_pXmlWriter->WriteAttribute(L"arcsize", adjValues[0].c_str());
					}
				}
				else
				{
					if (nAdjValues)												
					{
						std::wstring adjTag	= adjValues[0];

						for (int i = 1; i < nAdjValues; ++i)
							adjTag += std::wstring(L",") + adjValues[i];

						m_pXmlWriter->WriteAttribute(L"adj", adjTag.c_str());
					}
				}

				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				//build shadow offsets
				wstring offset;

				if ( ShadowOffsetX != 0 )
				{
					offset += FormatUtils::DoubleToWideString( ShadowOffsetX.ToPoints() );
					offset += _T( "pt" );
				}

				if ( ShadowOffsetY != 0 )
				{
					offset += _T( "," );
					offset += FormatUtils::DoubleToWideString( ShadowOffsetY.ToPoints() );
					offset += _T( "pt" );
				}

				if ( !offset.empty() )
				{
					appendValueAttribute(&m_shadow, _T( "offset" ), offset.c_str());
				}

				wstring offset2;

				if ( SecondShadowOffsetX != 0 )
				{
					offset2 += FormatUtils::DoubleToWideString( SecondShadowOffsetX.ToPoints() );
					offset2 += _T( "pt" );
				}

				if ( SecondShadowOffsetY != 0 )
				{
					offset2 += _T( "," );
					offset2 += FormatUtils::DoubleToWideString(SecondShadowOffsetY.ToPoints());
					offset2 += _T( "pt" );
				}

				if (!offset2.empty())
				{
					appendValueAttribute(&m_shadow, _T("offset2"), offset2.c_str());
				}

				//build shadow origin
				if ( ( ShadowOriginX != 0 ) && ( ShadowOriginY != 0 ) )
				{
					appendValueAttribute(&m_shadow, _T("origin"), (FormatUtils::DoubleToWideString(shadowOriginX) + std::wstring(_T( "," )) + FormatUtils::DoubleToWideString(shadowOriginY)).c_str());
				}

				// write shadow
				if (m_shadow.GetAttributeCount() > 0)
				{
					if (shadowBoolean.fShadow)
					{
						appendValueAttribute(&m_shadow, _T( "on" ), _T( "t" ));
					}

					m_pXmlWriter->WriteString(m_shadow.GetXMLString().c_str());
				}

				//write 3d style 
				if (m_3dstyle.GetAttributeCount() > 0)
				{
					appendValueAttribute(&m_3dstyle, _T( "v:ext" ), _T( "view" ));
					appendValueAttribute(&m_3dstyle, _T( "on" ), _T( "t" ));

					//write the viewpoint
					if ( ( ViewPointX != 0 ) || ( ViewPointY != 0 ) || ( ViewPointZ != 0 ) )
					{
						wstring viewPoint;

						if ( ViewPointX != 0 )
						{
							viewPoint += FormatUtils::IntToWideString( ViewPointX );
						}

						if ( ViewPointY != 0 )
						{
							viewPoint += _T( "," );
							viewPoint += FormatUtils::IntToWideString( ViewPointY );
						}

						if ( ViewPointZ != 0 )
						{
							viewPoint += _T( "," );
							viewPoint += FormatUtils::IntToWideString( ViewPointZ );
						}

						appendValueAttribute(&m_3dstyle, _T( "viewpoint" ), viewPoint.c_str());
					}

					// write the viewpointorigin
					if ( ( viewPointOriginX != 0 ) || ( viewPointOriginY != 0 ) )
					{
						std::wstring viewPointOrigin;

						if ( viewPointOriginX != 0 )
						{
							viewPointOrigin += FormatUtils::DoubleToFormattedWideString( viewPointOriginX, _T( "%.2f" ) );
						}

						if ( viewPointOriginY != 0 )
						{
							viewPointOrigin += _T( "," );
							viewPointOrigin += FormatUtils::DoubleToFormattedWideString( viewPointOriginY, _T( "%.2f" ) );
						}

						appendValueAttribute(&m_3dstyle, _T( "viewpointorigin" ), viewPointOrigin.c_str());
					}

					m_pXmlWriter->WriteString(m_3dstyle.GetXMLString().c_str());
				}

				// write wrap
				if (m_pSpa)
				{
					std::wstring wrap = getWrapType(m_pSpa);

					if (wrap != _T("through"))
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w10:wrap" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "type" ), wrap.c_str() );
						m_pXmlWriter->WriteNodeEnd	( _T( "w10:wrap" ), TRUE );
					}
				}

				// write stroke
				if (m_stroke.GetAttributeCount())
				{
					m_pXmlWriter->WriteString(m_stroke.GetXMLString().c_str());
				}

				// write fill
				if (m_fill.GetAttributeCount())
				{
					m_pXmlWriter->WriteString(m_fill.GetXMLString().c_str());
				}

				// text path
				if (m_textpath.GetAttributeCount())
				{
					appendValueAttribute(&m_textpath, _T( "style" ), FormatUtils::XmlEncode(m_textPathStyle).c_str());
					m_pXmlWriter->WriteString(m_textpath.GetXMLString().c_str());
				}

				// write imagedata
				if (m_imagedata.GetAttributeCount())
				{
					m_pXmlWriter->WriteString(m_imagedata.GetXMLString().c_str());
				}

				if (freeform)
				{
					if (arrInscribe.size())
					{
						m_pXmlWriter->WriteNodeBegin(L"v:path", true);
						m_pXmlWriter->WriteAttribute(L"textboxrect", arrInscribe[0]);
						m_pXmlWriter->WriteNodeEnd(L"", true);
					}
				}

				// TEXTBOX

				OfficeArtClientTextbox* pTextBox = pContainer->FirstChildWithType<OfficeArtClientTextbox>();
				if (pTextBox)
				{
					// Word text box

					//Word appends a OfficeArtClientTextbox record to the container. 
					//This record stores the index of the textbox.

					int nIndex = pTextBox->GetIndex();
					if (nIndex)
					{
						TextboxMapping textboxMapping(m_ctx, nIndex - 1, m_pXmlWriter, m_pCaller);
						textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
						textboxMapping.SetTextboxStyle(sTextboxStyle);

						m_ctx->_doc->Convert(&textboxMapping);
					}
				}
				else if( hasTextbox )
				{
					//Open Office textbox

					//Open Office doesn't append a OfficeArtClientTextbox record to the container.
					//We don't know how Word gets the relation to the text, but we assume that the first textbox in the document
					//get the index 0, the second textbox gets the index 1 (and so on).

					if (-1 != nLTxID)
					{
						TextboxMapping textboxMapping(m_ctx, nLTxID - 1, m_pXmlWriter, m_pCaller);
						textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
						m_ctx->_doc->Convert(&textboxMapping);
					}
					else
					{
						TextboxMapping textboxMapping(m_ctx, m_pXmlWriter, m_pCaller);
						textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
						m_ctx->_doc->Convert(&textboxMapping);
					}
				}

				WriteEndShapeNode(pShape);

				//ShapeType 
				if (NULL != pShape->GetShapeType() && !m_bBullet)
				{
					VMLShapeTypeMapping oXmlMapper(m_pXmlWriter);
					pShape->GetShapeType()->Convert(&oXmlMapper);
				}
			}
		}
	}

	//
	void VMLShapeMapping::WriteBeginShapeNode (const Shape* pShape)
	{
		if (NULL != pShape)
		{
			if (pShape->is<OvalType>())
			{
				//OVAL
				m_pXmlWriter->WriteNodeBegin( _T( "v:oval" ), TRUE );
			}
			else if (pShape->is<RoundedRectangleType>())
			{
				//ROUNDED RECT
				m_pXmlWriter->WriteNodeBegin( _T( "v:roundrect" ), TRUE );
			}
			else if (pShape->is<RectangleType>())
			{
				//RECT
				m_pXmlWriter->WriteNodeBegin( _T( "v:rect" ), TRUE );
			}
			else if (pShape->is<LineType>())
			{
				//LINE
				m_pXmlWriter->WriteNodeBegin(_T( "v:line" ), TRUE);
			}
			else
			{
				m_pXmlWriter->WriteNodeBegin(_T( "v:shape" ), TRUE);
			}
		}
	}

	void VMLShapeMapping::WriteEndShapeNode (const Shape* pShape)
	{
		if (NULL != pShape)
		{
			if (pShape->is<OvalType>())
			{
				m_pXmlWriter->WriteNodeEnd( _T( "v:oval" ) );
			}
			else if (pShape->is<RoundedRectangleType>())
			{
				m_pXmlWriter->WriteNodeEnd( _T( "v:roundrect" ) );
			}
			else if (pShape->is<RectangleType>())
			{
				m_pXmlWriter->WriteNodeEnd( _T( "v:rect" ) );
			}
			else if (pShape->is<LineType>())
			{
				m_pXmlWriter->WriteNodeEnd( _T( "v:line" ) );
			}
			else
			{
				m_pXmlWriter->WriteNodeEnd( _T( "v:shape" ) );
			}
		}
	}

	std::wstring VMLShapeMapping::GetShapeID(const Shape* pShape) const
	{
		std::wstring strXmlAttr;

		if (NULL != pShape)
		{
			strXmlAttr += std::wstring(_T("_x0000_s" ));
			strXmlAttr += FormatUtils::IntToWideString(pShape->GetShapeID());
		}

		return strXmlAttr;
	}

	std::wstring VMLShapeMapping::GetLineFrom(const ChildAnchor* pAnchor) const
	{
		//Если линия находится в группе, то координаты должны быть в Twips
		//Если линия находится в группе, то координаты должны быть в других единицах измерения (например в twips)

		std::wstring strXmlFrom;

		if (NULL != pAnchor)
		{
			strXmlFrom += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.x);
			strXmlFrom += _T( "," );
			strXmlFrom += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.y);
		}
		else if (m_pSpa)
		{
			TwipsValue oLeft(m_pSpa->xaLeft);
			TwipsValue oTop(m_pSpa->yaTop);

			strXmlFrom += FormatUtils::DoubleToWideString(oLeft.ToPoints());
			strXmlFrom += _T( "pt," );
			strXmlFrom += FormatUtils::DoubleToWideString(oTop.ToPoints());
			strXmlFrom += _T( "pt" );
		}

		return strXmlFrom;
	}

	std::wstring VMLShapeMapping::GetLineTo(const ChildAnchor* pAnchor) const
	{
		//Если линия находится в группе, то координаты должны быть в Twips
		//Если линия находится в группе, то координаты должны быть в других единицах измерения (например в twips)

		std::wstring strXmlTo;

		if (NULL != pAnchor)
		{
			strXmlTo += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.x + pAnchor->rcgBounds.size.cx);
			strXmlTo += _T( "," );
			strXmlTo += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.y + pAnchor->rcgBounds.size.cy);
		}
		else if (m_pSpa)
		{
			TwipsValue oRight (m_pSpa->xaRight);
			TwipsValue oBottom (m_pSpa->yaBottom);

			strXmlTo += FormatUtils::DoubleToWideString(oRight.ToPoints());
			strXmlTo += _T( "pt," );
			strXmlTo += FormatUtils::DoubleToWideString(oBottom.ToPoints());
			strXmlTo += _T( "pt" );
		}

		return strXmlTo;
	}

	/// Build the VML wrapcoords string for a given pWrapPolygonVertices
	std::wstring VMLShapeMapping::getWrapCoords(const OptionEntry& pWrapPolygonVertices) const
	{
		std::wstring coords;

		MemoryStream oStream(pWrapPolygonVertices.opComplex, pWrapPolygonVertices.op);
		std::list<int> arrVertices;

		unsigned short nElems		=	oStream.ReadUInt16();
		unsigned short nElemsAlloc	=	oStream.ReadUInt16();
		unsigned short cbElem		=	oStream.ReadUInt16();

		if ( ( nElems > 0 ) && ( cbElem > 0 ) && ( nElems <= nElemsAlloc ) )
		{
			//!!!TODO: read the Int32 coordinates!!!
			while (oStream.GetPosition() < oStream.GetSize())
			{
				arrVertices.push_back(oStream.ReadInt32());
			}

			std::list<int>::const_iterator end = arrVertices.end();
			for (std::list<int>::const_iterator iter = arrVertices.begin(); iter != end; ++iter)
			{
				coords += FormatUtils::IntToWideString(*iter);
				coords += _T( "," );
			}

			coords.erase(coords.size() - 1);
		}

		return coords;
	}

	/// Copies the picture from the binary stream to the zip archive 
	/// and creates the relationships for the image.
	bool VMLShapeMapping::copyPicture(const BlipStoreEntry* oBlip)
	{
		bool result = false;

		//write the blip
		if (oBlip)
		{
			VirtualStreamReader reader(m_ctx->_doc->WordDocumentStream, oBlip->foDelay);

			switch (oBlip->btWin32)
			{
			case Global::msoblipEMF:
			case Global::msoblipWMF:
				{
					//it's a meta image
					MetafilePictBlip* metaBlip = static_cast<MetafilePictBlip*>(RecordFactory::ReadRecord(&reader, 0));
					if (metaBlip)
					{
						//meta images can be compressed
						unsigned char* decompressed = NULL;
						int decompressedSize = 0;

						decompressedSize = metaBlip->Decompress(&decompressed);
						if (0 != decompressedSize && NULL != decompressed)
						{
							m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlip->btWin32), vector<unsigned char>(decompressed, (decompressed + decompressedSize))));
							RELEASEARRAYOBJECTS(decompressed);
						}

						RELEASEOBJECT(metaBlip);
					}
				}
				break;

			case Global::msoblipJPEG:
			case Global::msoblipCMYKJPEG:
			case Global::msoblipPNG:
			case Global::msoblipTIFF:
			case Global::msoblipDIB:
				{
					//it's a bitmap image
					BitmapBlip* bitBlip = static_cast<BitmapBlip*>(RecordFactory::ReadRecord(&reader, 0));
					if (bitBlip)
					{
						m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlip->btWin32), 
							vector<unsigned char>(bitBlip->m_pvBits, (bitBlip->m_pvBits + bitBlip->pvBitsSize)), oBlip->btWin32));
						RELEASEOBJECT (bitBlip);
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

			m_nImageId	=	m_ctx->_docx->RegisterImage (m_pCaller, oBlip->btWin32);

			result		=	true;
		}

		return result;
	}

	std::wstring VMLShapeMapping::GetTargetExt(Global::BlipType _type) const
	{
		switch ( _type )
		{
			//case msoblipBMP:
			//  return std::wstring( _T( ".bmp" ) );

		case Global::msoblipEMF:
			return std::wstring( _T( ".emf" ) );

			//case msoblipGIF:
			//  return std::wstring( _T( ".gif" ) );

			//case msoblipICON:
			//  return std::wstring( _T( ".ico" ) );

		case Global::msoblipJPEG:
		case Global::msoblipCMYKJPEG:
			return std::wstring( _T( ".jpg" ) );

			//case msoblipPCX:
			//  return std::wstring( _T( ".pcx" ) );

		case Global::msoblipPNG:
			return std::wstring( _T( ".png" ) );

		case Global::msoblipTIFF:
			return std::wstring( _T( ".tif" ) );

		case Global::msoblipWMF:
			return std::wstring( _T( ".wmf" ) );

		default:
			return std::wstring( _T( ".png" ) );
		}
	}

	void VMLShapeMapping::AppendDimensionToStyle(std::wstring* style, const PictureDescriptor* pict, bool twistDimensions) const
	{
		if ( ( style != NULL ) && ( pict != NULL ) )
		{
			double xScaling = pict->mx / 1000.0;
			double yScaling = pict->my / 1000.0;

			TwipsValue width( ( pict->dxaGoal - ( pict->dxaCropLeft + pict->dxaCropRight ) ) * xScaling );
			TwipsValue height( ( pict->dyaGoal - ( pict->dyaCropTop + pict->dyaCropBottom ) ) * yScaling );

			if ( twistDimensions )
			{
				width = TwipsValue( ( pict->dyaGoal - ( pict->dyaCropTop + pict->dyaCropBottom ) ) * yScaling );
				height = TwipsValue( ( pict->dxaGoal - ( pict->dxaCropLeft + pict->dxaCropRight ) ) * xScaling );
			}

			wstring widthString = FormatUtils::DoubleToWideString( width.ToPoints() );
			wstring heightString = FormatUtils::DoubleToWideString( height.ToPoints() );

			style->operator += ( std::wstring( _T( "width:" ) ) + widthString + std::wstring( _T( "pt;" ) ) );
			style->operator += ( std::wstring( _T( "height:" ) ) + heightString + std::wstring( _T( "pt;" ) ) );
		}
	}

	void VMLShapeMapping::AppendDimensionToStyle(std::wstring* style, const Spa* pSpa, bool twistDimensions) const
	{
		if ( ( style != NULL ) && (pSpa != NULL ) )
		{
			//append size and position ...
			if (pSpa->fAnchorLock )
			{
				appendStyleProperty( style, _T( "position" ), _T( "static" ) );
			}
			else
			{
				appendStyleProperty( style, _T( "position" ), _T( "absolute" ) );
			}

			TwipsValue left	 (pSpa->xaLeft);
			TwipsValue top	 (pSpa->yaTop);
			TwipsValue width (pSpa->xaRight  -	pSpa->xaLeft);
			TwipsValue height(pSpa->yaBottom -	pSpa->yaTop);

			if (twistDimensions)
			{
				width	=	TwipsValue(pSpa->yaBottom	-	pSpa->yaTop);
				height	=	TwipsValue(pSpa->xaRight	-	pSpa->xaLeft);

				left	=	TwipsValue((pSpa->xaRight	+	pSpa->xaLeft) * 0.5 - (pSpa->yaBottom	-	pSpa->yaTop)  * 0.5);
				top		=	TwipsValue((pSpa->yaBottom	+	pSpa->yaTop)  * 0.5 - (pSpa->xaRight	-	pSpa->xaLeft) * 0.5);
			}

			// //ATLTRACE (L"left : %f, top : %f\n", left.ToPoints(), top.ToPoints());

			appendStyleProperty (style, _T( "margin-left" ), ( FormatUtils::DoubleToWideString(left.ToPoints())		+ std::wstring( _T( "pt" ) ) ));
			appendStyleProperty (style, _T( "margin-top" ),	 ( FormatUtils::DoubleToWideString(top.ToPoints())		+ std::wstring( _T( "pt" ) ) ));
			appendStyleProperty (style, _T( "width" ),		 ( FormatUtils::DoubleToWideString(width.ToPoints())	+ std::wstring( _T( "pt" ) ) ));
			appendStyleProperty (style, _T( "height" ),		 ( FormatUtils::DoubleToWideString(height.ToPoints())	+ std::wstring( _T( "pt" ) ) ));
		}
	}

	void VMLShapeMapping::AppendDimensionToStyle(std::wstring* style, const ChildAnchor* anchor, bool twistDimensions) const
	{
		if ((style != NULL) && (anchor != NULL))
		{
			ASCDocFormatUtils::Rectangle bounds = anchor->rcgBounds;

			if (twistDimensions)
			{
				bounds.topLeftAngle.x	=	static_cast<LONG>(((anchor->Right	+	anchor->Left) * 0.5	- (anchor->Bottom	-	anchor->Top)  * 0.5));
				bounds.topLeftAngle.y	=	static_cast<LONG>(((anchor->Bottom	+	anchor->Top)  * 0.5 - (anchor->Right	-	anchor->Left) * 0.5));
			}

			appendStyleProperty(style, _T("position"), _T("absolute"));
			appendStyleProperty(style, _T("left"),	FormatUtils::IntToWideString(bounds.topLeftAngle.x));
			appendStyleProperty(style, _T("top"),	FormatUtils::IntToWideString(bounds.topLeftAngle.y));

			if (twistDimensions)
			{
				appendStyleProperty(style, _T("width"),	 FormatUtils::IntToWideString(bounds.size.cy));
				appendStyleProperty(style, _T("height"), FormatUtils::IntToWideString(bounds.size.cx));
			}
			else
			{
				appendStyleProperty(style, _T("width"),  FormatUtils::IntToWideString(bounds.size.cx));
				appendStyleProperty(style, _T("height"), FormatUtils::IntToWideString(bounds.size.cy));
			}
		}
	}

	void VMLShapeMapping::appendStyleProperty(std::wstring* b, const std::wstring& propName, const std::wstring& propValue) const
	{
		if ( b != NULL )
		{
			b->operator += ( propName );
			b->operator += ( _T( ":" ) );
			b->operator += ( propValue );
			b->operator +=( _T( ";" ) );
		}
	}

	std::wstring VMLShapeMapping::getTextboxAnchor(unsigned int anchor) const
	{
		switch ( anchor )
		{
		case 0:
			{
				//msoanchorTop
				return _T("top");
			}
			break;

		case 1:
			{
				//msoanchorMiddle
				return _T("middle");
			}
			break;

		case 2:
			{
				//msoanchorBottom
				return _T("bottom");
			}
			break;

		case 3:
			{
				//msoanchorTopCentered
				return _T("top-center");
			}
			break;

		case 4:
			{
				//msoanchorMiddleCentered
				return _T("middle-center");
			}
			break;

		case 5:
			{
				//msoanchorBottomCentered
				return _T("bottom-center");
			}
			break;

		case 6:
			{
				//msoanchorTopBaseline
				return _T("top-baseline");
			}
			break;

		case 7:
			{
				//msoanchorBottomBaseline
				return _T("bottom-baseline");
			}
			break;

		case 8:
			{
				//msoanchorTopCenteredBaseline
				return _T("top-center-baseline");
			}
			break;

		case 9:
			{
				//msoanchorBottomCenteredBaseline
				return _T("bottom-center-baseline");
			}
			break;

		default:
			{
				return _T("top");
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::mapVerticalPosition(PositionVertical vPos) const
	{
		switch ( vPos )
		{
			/*case msopvAbs:
			{
			return _T( "absolute" );
			}
			break;*/

		case msopvTop:
			{
				return _T( "top" );
			}
			break;

		case msopvCenter:
			{
				return _T( "center" );
			}
			break;

		case msopvBottom:
			{
				return _T( "bottom" );
			}
			break;

		case msopvInside:
			{
				return _T( "inside" );
			}
			break;

		case msopvOutside:
			{
				return _T( "outside" );
			}
			break;

		default:
			{
				return _T( "absolute" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::mapVerticalPositionRelative(PositionVerticalRelative vRel) const
	{
		switch ( vRel )
		{
		case msoprvMargin:
			{
				return _T( "margin" );
			}
			break;

		case msoprvPage:
			{
				return _T( "page" );
			}
			break;

		case msoprvText:
			{
				return _T( "text" );
			}
			break;

		case msoprvLine:
			{
				return _T( "line" );
			}
			break;

		default:
			{
				return _T( "margin" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::mapHorizontalPosition(PositionHorizontal hPos) const
	{
		switch ( hPos )
		{
			/*case msophAbs:
			{
			return _T( "absolute" );
			}
			break;*/

		case msophLeft:
			{
				return _T( "left" );
			}
			break;

		case msophCenter:
			{
				return _T( "center" );
			}
			break;

		case msophRight:
			{
				return _T( "right" );
			}
			break;

		case msophInside:
			{
				return _T( "inside" );
			}
			break;

		case msophOutside:
			{
				return _T( "outside" );
			}
			break;

		default:
			{
				return _T( "absolute" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::mapHorizontalPositionRelative( PositionHorizontalRelative hRel ) const
	{
		switch ( hRel ) 
		{
		case msoprhMargin:
			{
				return _T( "margin" );
			}
			break;

		case msoprhPage:
			{
				return _T( "page" );
			}
			break;

		case msoprhText:
			{
				return _T( "text" );
			}
			break;

		case msoprhChar:
			{
				return _T( "char" );
			}
			break;

		default:
			{
				return _T( "margin" );
			}
			break;
		}
	}

	void VMLShapeMapping::AppendOptionsToStyle (std::wstring* oStyle, const std::list<OptionEntry>& options) const
	{
		std::list<OptionEntry>::const_iterator end = options.end();
		for (std::list<OptionEntry>::const_iterator iter = options.begin(); iter != end; ++iter)
		{
			switch (iter->pid)
			{
				//	POSITIONING
			case posh:
				{
					appendStyleProperty(oStyle, _T("mso-position-horizontal"), mapHorizontalPosition((PositionHorizontal)iter->op));
				}
				break;

			case posrelh:
				{
					appendStyleProperty(oStyle, _T("mso-position-horizontal-relative"), mapHorizontalPositionRelative((PositionHorizontalRelative)iter->op));
				}
				break;

			case posv:
				{
					appendStyleProperty(oStyle, _T("mso-position-vertical"), mapVerticalPosition((PositionVertical)iter->op));
				}
				break;

			case posrelv:
				{
					appendStyleProperty(oStyle, _T("mso-position-vertical-relative"), mapVerticalPositionRelative((PositionVerticalRelative)iter->op));
				}
				break;

				//	BOOLEANS

			case groupShapeBooleans:
				{
					GroupShapeBooleanProperties groupShapeBooleans(iter->op);

					if (groupShapeBooleans.fUsefBehindDocument && groupShapeBooleans.fBehindDocument)
					{
						//The shape is behind the text, so the z-index must be negative.
						appendStyleProperty(oStyle, _T( "z-index" ), _T( "-1" ) );
					}

					if (groupShapeBooleans.fHidden && groupShapeBooleans.fUsefHidden)
					{
						appendStyleProperty(oStyle, _T( "visibility" ), _T( "hidden" ));
					}
				}
				break;

				//	GEOMETRY

			case PropertyId_rotation:
				{
					double dAngle = (double)((int)iter->op) / 65535.0;

					if (dAngle < -360.0)
						dAngle += 360.0;

					// //ATLTRACE (L"angle : %f\n", dAngle);

					appendStyleProperty(oStyle, _T( "rotation" ), FormatUtils::DoubleToWideString(dAngle));
				}
				break;

				//	TEXTBOX	

			case anchorText:
				{
					appendStyleProperty(oStyle, _T("v-text-anchor"), getTextboxAnchor(iter->op));
				}
				break;

				//	WRAP DISTANCE

			case dxWrapDistLeft:
				{
					appendStyleProperty(oStyle, _T("mso-wrap-distance-left"), (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(_T("pt"))));
				}
				break;

			case dxWrapDistRight:
				{
					appendStyleProperty(oStyle, _T("mso-wrap-distance-right"), (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(_T("pt"))));
				}
				break;

			case dyWrapDistBottom:
				{
					appendStyleProperty(oStyle, _T("mso-wrap-distance-bottom"), (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(_T("pt"))));
				}
				break;

			case dyWrapDistTop:
				{
					appendStyleProperty(oStyle, _T("mso-wrap-distance-top"), (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(_T("pt"))));
				}
				break;
			}
		}
	}

	//
	std::wstring VMLShapeMapping::buildStyle (const Shape* shape, const ChildAnchor* anchor, const std::list<OptionEntry>& options, int zIndex) const
	{
		std::wstring style;

		// Check if some properties are set that cause the dimensions to be twisted
		bool twistDimensions = false;
		std::list<OptionEntry>::const_iterator end = options.end();
		for (std::list<OptionEntry>::const_iterator iter = options.begin(); iter != end; ++iter)
		{
			if (geometryTextBooleanProperties ==  iter->pid)
			{
				GeometryTextBooleanProperties props(iter->op);

				if (props.fUsegtextFVertical && props.gtextFVertical)
				{
					twistDimensions = true;
				}
			}

			if (PropertyId_rotation ==  iter->pid)
			{
				double dAngle = (double)((int)iter->op) / 65535.0;

				if (dAngle < -360.0)
					dAngle += 360.0;

				if ((dAngle >= 45.0 && dAngle <= 135.0) || (dAngle >= 225.0 && dAngle <= 315.0) || (dAngle <= -45.0 && dAngle >= -135.0) || (dAngle <= -225.0 && dAngle >= -315.0))
					twistDimensions	=	true;

				//ATLTRACE (L"angle : %f\n", dAngle);
			}
		}

		//don't append the dimension info to lines, 
		// because they have "from" and "to" attributes to decline the dimension
		if(!shape->is<LineType>())
		{
			if ( (m_pSpa != NULL) && ( anchor == NULL ) )
			{
				//this shape is placed directly in the document, 
				//so use the FSPA to build the style
				AppendDimensionToStyle(&style, m_pSpa, twistDimensions);
			}
			else if (anchor)
			{
				//the style is part of a group, 
				//so use the anchor
				AppendDimensionToStyle(&style, anchor, twistDimensions);
			}
			else if (m_pict)
			{
				// it is some kind of PICT shape (e.g. WordArt)
				AppendDimensionToStyle(&style, m_pict, twistDimensions);
			}
		}
		else
		{
			//если не написать тип позиции, то будет inline
			if ( anchor != NULL )
			{
				appendStyleProperty( &style, _T( "position" ), _T( "absolute" ) );
			}
			else if (m_pSpa)
			{
				//append size and position ...
				if (m_pSpa->fAnchorLock)
				{
					appendStyleProperty( &style, _T( "position" ), _T( "static" ) );
				}
				else
				{
					appendStyleProperty( &style, _T( "position" ), _T( "absolute" ) );
				}
			}
		}

		if ( shape->fFlipH )
		{
			appendStyleProperty( &style, _T( "flip" ), _T( "x" ) );
		}

		if ( shape->fFlipV )
		{
			appendStyleProperty( &style, _T( "flip" ), _T( "y" ) );
		}

		AppendOptionsToStyle( &style, options );
		
		if (!this->m_bBullet)appendStyleProperty( &style, _T( "z-index" ), FormatUtils::IntToWideString(zIndex + 0x7ffff));

		return style;
	}

	std::wstring VMLShapeMapping::getLineStyle(unsigned int p) const
	{
		switch (p)
		{
		case 0:
			{
				return _T( "single" );
			}
			break;

		case 1:
			{
				return _T( "thinThin" );
			}
			break;

		case 2:
			{
				return _T( "thinThick" );
			}
			break;

		case 3:
			{
				return _T( "thickThin" );
			}
			break;

		case 4:
			{
				return _T( "thickBetweenThin" );
			}
			break;

		default:
			{
				return _T( "single" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::getArrowStyle(unsigned int op) const
	{
		switch ( op )
		{
		default:
			{
				//msolineNoEnd
				return _T( "none" );
			}
			break;

		case 1:
			{
				//msolineArrowEnd
				return _T( "block" );
			}
			break;

		case 2:
			{
				//msolineArrowStealthEnd
				return _T( "classic" );
			}
			break;

		case 3:
			{
				//msolineArrowDiamondEnd
				return _T( "diamond" );
			}
			break;

		case 4:
			{
				//msolineArrowOvalEnd
				return _T( "oval" );
			}
			break;

		case 5:
			{
				//msolineArrowOpenEnd
				return _T( "open" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::getArrowLength(unsigned int op) const
	{
		switch ( op )
		{
		default:
			{
				//msolineShortArrow
				return _T( "short" );
			}
			break;

		case 1:
			{
				//msolineMediumLengthArrow
				return _T( "medium" );
			}
			break;

		case 2:
			{
				//msolineLongArrow
				return _T( "long" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::getArrowWidth(unsigned int op) const
	{
		switch ( op )
		{
		default:
			{
				//msolineNarrowArrow
				return _T( "narrow" );
			}
			break;

		case 1:
			{
				//msolineMediumWidthArrow
				return _T( "medium" );
			}
			break;

		case 2:
			{
				//msolineWideArrow
				return _T( "wide" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::getFillMethod(unsigned int p) const
	{
		short val = (short)( ( p & 0xFFFF0000 ) >> 28 );

		switch ( val )
		{
		case 0:
			{
				return _T( "none" );
			}
			break;

		case 1:
			{
				return _T( "any" );
			}
			break;

		case 2:
			{
				return _T( "linear" );
			}
			break;

		case 4:
			{
				return _T( "linear sigma" );
			}
			break;

		default:
			{
				return _T( "any" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::getFillColorString(const unsigned char* p, unsigned int size)  const
	{
		std::wstring result;

		if ( ( p != NULL ) && ( size > 0 ) )
		{
			// parse the IMsoArray
			unsigned short nElems		=	FormatUtils::BytesToUInt16(p, 0, size);
			unsigned short nElemsAlloc	=	FormatUtils::BytesToUInt16(p, 2, size);
			unsigned short cb			=	FormatUtils::BytesToUInt16(p, 4, size);

			for ( unsigned short i = 0; i < nElems; i++ )
			{
				int pos = ( 6 + ( i * cb ) );

				RGBColor color(FormatUtils::BytesToInt32(p, pos, size ), RedFirst);
				int colorPos = FormatUtils::BytesToInt32(p, ( pos + 4 ), size);

				result += FormatUtils::IntToWideString(colorPos);
				result += _T( "f #" );
				result += color.SixDigitHexCode;
				result += _T( ";" );
			}
		}

		return result;
	}

	/// Returns the OpenXML fill type of a fill effect
	std::wstring VMLShapeMapping::getFillType(unsigned int p) const
	{
		switch ( p )
		{
		case 0:
			{
				return _T( "solid" );
			}
			break;

		case 1:
			{
				return _T( "tile" );
			}
			break;

		case 2:
			{
				return _T( "pattern" );
			}
			break;

		case 3:
			{
				return _T( "frame" );
			}
			break;

		case 4:
			{
				return _T( "gradient" );
			}
			break;

		case 5:
			{
				return _T( "gradientRadial" );
			}
			break;

		case 6:
			{
				return _T( "gradientRadial" );
			}
			break;

		case 7:
			{
				return _T( "gradient" );
			}
			break;

		case 9:
			{
				return _T( "solid" );
			}
			break;

		default:
			{
				return _T( "solid" );
			}
			break;
		}
	}

	std::wstring VMLShapeMapping::getShadowType(unsigned int p) const
	{
		switch ( p )
		{
		case 0:
			{
				return _T( "single" );
			}
			break;

		case 1:
			{
				return _T( "double" );
			}
			break;

		case 2:
			{
				return _T( "perspective" );
			}
			break;

		case 3:
			{
				return _T( "shaperelative" );
			}
			break;

		case 4:
			{
				return _T( "drawingrelative" );
			}
			break;

		case 5:
			{
				return _T( "emboss" );
			}
			break;

		default:
			{
				return _T( "single" );
			}
			break;
		}
	}

	/// Returns the OpenXML wrap type of the shape
	std::wstring VMLShapeMapping::getWrapType (const Spa* pSpa) const
	{
		// spec values
		// 0 = like 2 but doesn't equire absolute object
		// 1 = no text next to shape
		// 2 = wrap around absolute object
		// 3 = wrap as if no object present
		// 4 = wrap tightly areound object
		// 5 = wrap tightly but allow holes

		std::wstring wrapType = _T("none");

		if (pSpa != NULL)
		{
			switch (pSpa->wr)
			{
			case 0:
			case 2:
				{
					wrapType = _T( "square" );
				}
				break;

			case 1:
				{
					wrapType = _T( "topAndBottom" );
				}
				break;

			case 3:
				{
					wrapType = _T( "through" );
				}
				break;

			case 4:
			case 5:
				{
					wrapType = _T( "tight" );
				}
				break;

			default:
				{
					wrapType = _T( "none" );
				}
				break;
			}
		}

		return wrapType;
	}

	std::wstring VMLShapeMapping::ParsePath (const std::list<OptionEntry>& options) const
	{
		const unsigned char* pVP		=	NULL;
		unsigned int nVP	=	0;
		const unsigned char* pSI		=	NULL;
		unsigned int nSI	=	0;
		
		std::list<OptionEntry>::const_iterator end = options.end();
		for (std::list<OptionEntry>::const_iterator iter = options.begin(); iter != end; ++iter)
		{
			if (iter->pid == pVertices)
			{
				pVP =	iter->opComplex;
				nVP	=	iter->op;	
			}

			if (iter->pid == pSegmentInfo)
			{
				pSI =	iter->opComplex;
				nSI	=	iter->op;	
			}
		}

		PathParser oParser (pSI, nSI, pVP, nVP);
		return oParser.GetVmlPath();
	}

	//
	std::vector<CString> VMLShapeMapping::GetTextRectangles(const OptionEntry& inscribe) const 
	{
		MemoryStream reader(inscribe.opComplex, inscribe.op + 6);

		unsigned short elems		=	reader.ReadUInt16();
		unsigned short allocElems	=	reader.ReadUInt16();
		unsigned short cb			=	reader.ReadUInt16();
	
		std::vector<CString> rectangles;

		if (16 != cb) return rectangles; // TODO: доделать

		int count = (inscribe.op) / 16;

		for (int i = 0; i < count; ++i)
		{
			RECT rc;

			rc.top		=	reader.ReadInt32();
			rc.left		=	reader.ReadInt32();
			rc.right	=	reader.ReadInt32();
			rc.bottom	=	reader.ReadInt32();

			CString rectangle;
			rectangle.Format(L"%d,%d,%d,%d", rc.top, rc.left, rc.right, rc.bottom);  
			rectangles.push_back(rectangle);
		}

		return rectangles;
	}
}
