/*
 * (c) Copyright Ascensio System SIA 2010-2018
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


#include "VMLShapeMapping.h"
#include "TextboxMapping.h"

#include "OfficeDrawing/Shapetypes/LineType.h"
#include "OfficeDrawing/Shapetypes/OvalType.h"
#include "OfficeDrawing/Shapetypes/RectangleType.h"
#include "OfficeDrawing/Shapetypes/RoundedRectangleType.h"

#include "OfficeDrawing/threeDBooleanProperties.h"
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

#include "DrawingPrimitives.h"

#include "../../DesktopEditor/common/String.h"

#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
	VMLShapeMapping::VMLShapeMapping (ConversionContext* pConv, XMLTools::CStringXmlWriter* pWriter, Spa* pSpa, PictureDescriptor* pPicture, IMapping* pCaller, bool isInlineShape) : PropertiesMapping(pWriter)
	{		
		m_isInlineShape		=	isInlineShape;
		m_isBullete			=	false;

		m_pSpa				=	pSpa;
		m_pCaller			=	pCaller;
		m_pBlipStore		=	NULL;

		m_context			=	pConv;
		m_pict				=	pPicture;
		m_nImageId			=	0;

        m_imagedata			=	XMLTools::XMLElement(L"v:imagedata");
        m_fill				=	XMLTools::XMLElement(L"v:fill");
        m_stroke			=	XMLTools::XMLElement(L"v:stroke");
        m_shadow			=	XMLTools::XMLElement(L"v:shadow");
        m_3dstyle			=	XMLTools::XMLElement(L"o:extrusion");
        m_textpath			=	XMLTools::XMLElement(L"v:textpath");

		Record* recBs		=	NULL;
		if ((m_context) && (m_context->_doc))
		{
			OfficeArtContent* officeArt = m_context->_doc->GetOfficeArt();
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
		DrawingPrimitives * primitives = dynamic_cast<DrawingPrimitives *>(visited);
		
		if ( primitives ) return ApplyPrimitives(primitives);

//----------------------------------------------------------------------------------------------

		ShapeContainer* pContainer = dynamic_cast<ShapeContainer*>(visited);
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
			
			ChildAnchor* anchor					=	groupShape->FirstChildWithType<ChildAnchor>();			
			std::vector<OptionEntryPtr> options	=	groupShape->ExtractOptions();

			m_shapeId = GetShapeID(shape);

			m_pXmlWriter->WriteNodeBegin( L"v:group", true );
			m_pXmlWriter->WriteAttribute( L"id", m_shapeId);
			m_pXmlWriter->WriteAttribute( L"style", FormatUtils::XmlEncode(buildStyle(shape, anchor, options, container->Index)));
			m_pXmlWriter->WriteAttribute( L"coordorigin", ( FormatUtils::IntToWideString(gsr->rcgBounds.topLeftAngle.x) + L"," + FormatUtils::IntToWideString( gsr->rcgBounds.topLeftAngle.y)));
			m_pXmlWriter->WriteAttribute( L"coordsize", ( FormatUtils::IntToWideString(gsr->rcgBounds.size.cx) + L"," + FormatUtils::IntToWideString(gsr->rcgBounds.size.cy)));

			// Write wrap coords			
			for (size_t i = 0; i < options.size(); i++)
			{
				switch (options[i]->pid)
				{
					case pWrapPolygonVertices:
					{
						std::wstring wrapCoords	= GetWrapCoords(options[i]);
						if (wrapCoords.length())
							m_pXmlWriter->WriteAttribute(L"wrapcoords", wrapCoords);
					}
					break;
				}
			}

			m_pXmlWriter->WriteNodeEnd(L"", true, false);

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
							VMLShapeMapping vmlShapeMapping(m_context, m_pXmlWriter, m_pSpa, NULL,  m_pCaller);
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
				if (wrap != L"through")
				{
					m_pXmlWriter->WriteNodeBegin( L"w10:wrap",	true );
					m_pXmlWriter->WriteAttribute( L"type",		wrap);
					m_pXmlWriter->WriteNodeEnd	( L"w10:wrap",	true );
				}
			}

			m_pXmlWriter->WriteNodeEnd( L"v:group" );
		}
	}

	/// Converts a single shape
	void VMLShapeMapping::WriteShape(const ShapeContainer* pContainer)
	{
		if ((NULL == pContainer) || (pContainer->Children.empty())) return;

		Shape* pShape =	static_cast<Shape*>(pContainer->Children[0]);
		if (!pShape) return;
		
		int				indexOLE = -1;
		bool			freeform =	true;
		std::wstring	sShapeId;

		std::vector<OptionEntryPtr> options	=	pContainer->ExtractOptions();
		ChildAnchor* pAnchor				=	pContainer->FirstChildWithType<ChildAnchor>();
		ClientAnchor* clientAnchor			=	pContainer->FirstChildWithType<ClientAnchor>();

		WriteBeginShapeNode (pShape);

		m_shapeId = GetShapeID(pShape);

		count_vml_objects++;

		if (m_shapeId.empty())
			m_shapeId =	std::wstring(L"_x0000_s") + FormatUtils::IntToWideString(1024 + count_vml_objects);
		
		m_pXmlWriter->WriteAttribute ( L"id", m_shapeId );

		if ( !pShape->fBackground )
		{
			if (pShape->GetShapeType())
			{
				freeform =	false;
				m_pXmlWriter->WriteAttribute( L"type", (std::wstring(L"#") + VMLShapeTypeMapping::GenerateTypeId(pShape->GetShapeType())));
			}
			m_pXmlWriter->WriteAttribute( L"style", FormatUtils::XmlEncode(buildStyle(pShape, pAnchor, options, pContainer->Index)));

			if (pShape->is<LineType>())
			{
				//append "from" and  "to" attributes
				m_pXmlWriter->WriteAttribute(L"from", GetLineFrom(pAnchor));
				m_pXmlWriter->WriteAttribute(L"to",	GetLineTo(pAnchor));
			}

			if (m_isBullete)
			{
				 m_pXmlWriter->WriteAttribute(L"o:bullet", L"t");
			}
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
		
		bool bStroked			=	true;
		bool bFilled			=	true;
		bool hasTextbox			=	false;
		bool layoutInCell		=	true; //anmeldebogenfos.doc
		bool b3D				=	false;
		bool bShadow			=	false;
		
		int ndxTextLeft			=	-1;
		int ndyTextTop			=	-1;
		int ndxTextRight		=	-1;
		int ndyTextBottom		=	-1;

		bool bHavePath			=	false;
		int nShapePath			=	-1;
		int	nAdjValues			=	0;
		int	nLTxID				=	-1;

		std::wstring				sTextboxStyle;
				
		OptionEntryPtr				opSegmentInfo;
		OptionEntryPtr				opVerticles;
		OptionEntryPtr				opInscribe;
		OptionEntryPtr				opConnectAngles;
		OptionEntryPtr				opConnectLocs;

		ThreeDStyleBooleanProperties threeDStyleProps_(0);

		for (size_t i = 0; i < options.size(); i++)
		{
			OptionEntryPtr & iter = options[i];
			switch (iter->pid)
			{
	//BOOLEANS
			case geometryBooleans:
				{
					GeometryBooleanProperties booleans(iter->op);
					if (booleans.fUsefLineOK && !booleans.fLineOK)
					{
						bStroked	=	false;
					}
					if (booleans.fUsefFillOK && !booleans.fFillOK)
					{
						bFilled	=	false;
					}
					if (booleans.fUsef3DOK && booleans.f3DOK)
					{
						b3D	=	true;
					}
					if (booleans.fUsefShadowOK && booleans.fShadowOK)
					{
						bShadow	=	true;
					}
				}
				break;
			case fillStyleBooleanProperties:
				{
					FillStyleBooleanProperties booleans(iter->op);
					if (booleans.fUsefFilled && !booleans.fFilled)
					{
						bFilled = false;
					}

					if (booleans.fUsefUseShapeAnchor && booleans.fUseShapeAnchor)
					{
						appendValueAttribute(&m_fill, L"rotate", L"t");
					}
				}break;
			case lineStyleBooleans:
				{
					LineStyleBooleanProperties booleans(iter->op);
					if (booleans.fUsefLine && !booleans.fLine)
					{
						bStroked = false;
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
			case shapePath :
				{
					bHavePath =	true;
					nShapePath = iter->op;
				}break;
			case pVertices:
				{
					opVerticles = iter;
				}break;
			case pSegmentInfo:
				{
					opSegmentInfo = iter;
				}break;
			case pGuides:
				{
					GetGuides(iter);
				}break;
			case pConnectionSites:
				{
					opConnectLocs = iter;
				}break;
			case pConnectionSitesDir:
				{
					opConnectAngles	= iter;
				}break;
			case pInscribe:
				{
					opInscribe	= iter;
				}break;
			case adjustValue:
				{
					m_nAdjValues[0] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues,1);
				}
				break;
			case adjust2Value:
				{
					m_nAdjValues[1] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues, 2);
				}
				break;
			case adjust3Value:
				{
					m_nAdjValues[2] = (int)iter->op;
                    nAdjValues		=(std::max)(nAdjValues, 3);
				}break;
			case adjust4Value:
				{
					m_nAdjValues[3] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues, 4);
				}break;
			case adjust5Value:
				{
					m_nAdjValues[4] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues, 5);
				}break;
			case adjust6Value:
				{
					m_nAdjValues[5] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues, 6);
				}break;
			case adjust7Value:
				{
					m_nAdjValues[6] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues, 7);
				}break;
			case adjust8Value:
				{
					m_nAdjValues[7] = (int)iter->op;
                    nAdjValues		= (std::max)(nAdjValues, 8);
				}break;
			case pWrapPolygonVertices:
				{
					std::wstring wrapCoords = GetWrapCoords(iter);

					if (!wrapCoords.empty())
					{
						m_pXmlWriter->WriteAttribute( L"wrapcoords", wrapCoords);
					}
				}break;
			case geoRight:
				{
					xCoord = iter->op;
				}break;
			case geoBottom:
				{
					yCoord = iter->op;
				}break;
// LINE
			case lineColor:
				{
					RGBColor lineColor((int)iter->op, RedFirst);
					if ( !pShape->fBackground )
						m_pXmlWriter->WriteAttribute( L"strokecolor", (std::wstring(L"#") + lineColor.SixDigitHexCode));
				}break;
			case lineWidth:
				{
					if (iter->op > 0)
					{
						EmuValue eLineWidth ((int)iter->op );
						m_pXmlWriter->WriteAttribute(L"strokeweight", FormatUtils::DoubleToWideString(eLineWidth.ToPoints()) + L"pt");
					}
				}break;
			case lineDashing:
				{
					appendValueAttribute(&m_stroke, L"dashstyle", FormatUtils::MapValueToWideString( iter->op, &Global::DashStyleMap[0][0], 11, 16 ) );
				}break;
			case lineStyle:
				{
					appendValueAttribute(&m_stroke, L"linestyle", getLineStyle( iter->op ));
				}break;
			case lineEndArrowhead:
				{
					appendValueAttribute(&m_stroke, L"endarrow", getArrowStyle( iter->op ));
				}break;
			case lineEndArrowLength:
				{
					appendValueAttribute(&m_stroke, L"endarrowlength", getArrowLength( iter->op ));
				}break;
			case lineEndArrowWidth:
				{
					appendValueAttribute(&m_stroke, L"endarrowwidth", getArrowWidth( iter->op ));
				}break;
			case lineStartArrowhead:
				{
					appendValueAttribute(&m_stroke, L"startarrow", getArrowStyle( iter->op ));
				}break;
			case lineStartArrowLength:
				{
					appendValueAttribute(&m_stroke, L"startarrowlength", getArrowLength( iter->op ));
				}break;
			case lineStartArrowWidth:
				{
					appendValueAttribute(&m_stroke, L"startarrowwidth", getArrowWidth( iter->op ));
				}break;
			case cxstyle:
				{
					if (pShape->GetShapeType() == NULL)
					{
						freeform = false;
						m_pXmlWriter->WriteAttribute(L"type", L"#_x0000_t32");
					}
					switch(iter->op)
					{
						case 0:	m_pXmlWriter->WriteAttribute(L"o:connectortype", L"straight");	break;
						case 1:	m_pXmlWriter->WriteAttribute(L"o:connectortype", L"elbow");		break;
						case 2:	m_pXmlWriter->WriteAttribute(L"o:connectortype", L"curved");	break;
						case 3:	m_pXmlWriter->WriteAttribute(L"o:connectortype", L"none");		break;
					}
				}break;
// FILL
			case fillColor:
				{
					RGBColor fillColor((int)iter->op, RedFirst);
					m_pXmlWriter->WriteAttribute(L"fillcolor", ( std::wstring( L"#" )  + fillColor.SixDigitHexCode ));
				}break;
			case fillBackColor:
				{
					RGBColor fillBackColor( (int)iter->op, RedFirst );
					appendValueAttribute(&m_fill, L"color2", ( std::wstring( L"#" )  + fillBackColor.SixDigitHexCode ));
				}break;
			case fillAngle:
				{
					FixedPointNumber fllAngl( iter->op );
					appendValueAttribute(&m_fill, L"angle", FormatUtils::DoubleToWideString( fllAngl.ToAngle() ));
				}break;
			case fillShadeType:
				{
					appendValueAttribute(&m_fill, L"method", getFillMethod( iter->op ));
				}break;
			case fillShadeColors:
				{
					appendValueAttribute(&m_fill, L"colors", getFillColorString( iter->opComplex.get(), iter->op ));
				}break;
			case fillFocus:
				{
					appendValueAttribute(&m_fill, L"focus", ( FormatUtils::IntToWideString( iter->op ) + L"%"  ));
					appendValueAttribute(&m_fill, L"focussize", L"");
				}break;
			case fillType:
				{
					appendValueAttribute(&m_fill, L"type", getFillType( iter->op ));
				}break;
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
						appendValueAttribute(&m_fill, L"r:id", std::wstring(( L"rId" ) + FormatUtils::IntToWideString(m_nImageId) ));
					}
				}break;
			case fillBlipName:
				{
					std::wstring name;
					FormatUtils::GetSTLCollectionFromBytes<std::wstring>(&name, iter->opComplex.get(), iter->op, ENCODING_UTF16);
					if (!name.empty())
						appendValueAttribute(&m_fill, L"o:title", FormatUtils::XmlEncode(name));

				}break;
			case fillOpacity:
				{
					appendValueAttribute(&m_fill, L"opacity", ( FormatUtils::IntToWideString( iter->op ) + L"f" ));
				}
				break;
			case fillBackOpacity:
				{
					appendValueAttribute(&m_fill, L"o:opacity2", (FormatUtils::IntToWideString(iter->op) + L"f"));
				}break;
// SHADOW
			case shadowType:
				{
					appendValueAttribute(&m_shadow, L"type", getShadowType(iter->op));
				}break;

			case shadowColor:
				{
					RGBColor shadowColor((int)iter->op, RedFirst);
					appendValueAttribute(&m_shadow, L"color", ( std::wstring( L"#" ) + shadowColor.SixDigitHexCode ));
				}break;
			case shadowOffsetX:
				{
					ShadowOffsetX = EmuValue( (int)iter->op );
				}break;
			case shadowSecondOffsetX:
				{
					SecondShadowOffsetX = EmuValue( (int)iter->op );
				}break;
			case shadowOffsetY:
				{
					ShadowOffsetY = EmuValue( (int)iter->op );
				}break;
			case shadowSecondOffsetY:
				{
					SecondShadowOffsetY = EmuValue( (int)iter->op );
				}break;
			case shadowOriginX:
				{
					ShadowOriginX = ( iter->op / pow( (double)2, (double)16 ) );
				}break;
			case shadowOriginY:
				{
					ShadowOriginY = (iter->op / pow( (double)2, (double)16));
				}break;
			case shadowOpacity:
				{
					double shadowOpa = (iter->op / pow( (double)2, (double)16));

					appendValueAttribute(&m_shadow, L"opacity", FormatUtils::DoubleToFormattedWideString( shadowOpa, L"%.2f" ));
				}break;
			case shadowStyleBooleanProperties:
				{
					ShadowStyleBooleanProperties props(iter->op);

				}break;
// OLE
			case pictureId:
				{
					indexOLE = iter->op;
				}break;
// PICTURE
			case Pib:
				{
					int index = (int)( iter->op - 1 );

					if ((m_pBlipStore != NULL) && (index < (int)m_pBlipStore->Children.size()))
					{
						BlipStoreEntry* oBlip = static_cast<BlipStoreEntry*>(m_pBlipStore->Children[index]);
						if (copyPicture(oBlip))
						{
							appendValueAttribute(&m_imagedata, L"r:id", ( std::wstring( L"rId" ) + FormatUtils::IntToWideString(m_nImageId) ));
						}
					}
				}break;
			case pibName:
				{
					std::wstring name;
					FormatUtils::GetSTLCollectionFromBytes<std::wstring>(&name, iter->opComplex.get(), iter->op, ENCODING_UTF16);
					if (!name.empty())
						appendValueAttribute(&m_imagedata, L"o:title", FormatUtils::XmlEncode(name));
				}break;
// 3D STYLE
			case threeDStyleBooleanProperties:
				{
                    threeDStyleProps_ = ThreeDStyleBooleanProperties(iter->op);
				}break;
			case threeDObjectBooleanProperties:
				{
                    ThreeDObjectBooleanProperties booleans(iter->op);

					if (booleans.fUsef3D && !booleans.f3D) b3D = false;
				}break;
			case c3DRenderMode:
				{
				}break;
			case c3DExtrudeBackward:
				{
					EmuValue backwardValue( (int)iter->op );
					std::wstring depth = FormatUtils::DoubleToWideString( backwardValue.ToPoints() ) + L"pt";
					appendValueAttribute(&m_3dstyle, L"backdepth", depth);
				}break; 
			case c3DAmbientIntensity:
				{
					std::wstring intens = FormatUtils::IntToWideString((int)iter->op) + L"f";
					appendValueAttribute(&m_3dstyle, L"brightness", intens);
				}break; 
			case c3DSpecularAmt:
				{
					std::wstring amt = FormatUtils::IntToWideString((int)iter->op) + L"f";
					appendValueAttribute(&m_3dstyle, L"specularity", amt);
				}break; 
			case c3DDiffuseAmt:
				{
					std::wstring amt = FormatUtils::IntToWideString((int)iter->op) + L"f";
					appendValueAttribute(&m_3dstyle, L"diffusity", amt);
				}break; 
			case c3DKeyIntensity:
				{
					std::wstring amt = FormatUtils::IntToWideString((int)iter->op);
					appendValueAttribute(&m_3dstyle, L"lightlevel", amt);
				}break; 	
			case c3DExtrusionColor:
				{
					std::wstring color = FormatUtils::IntToFormattedWideString(iter->op, L"#%06x");
					appendValueAttribute(&m_3dstyle, L"color", color);
				}break;
			case c3DSkewAngle:
				if (threeDStyleProps_.fUsefc3DParallel && threeDStyleProps_.fc3DParallel)
				{
					FixedPointNumber skewAngle( iter->op );
					appendValueAttribute(&m_3dstyle, L"skewangle", FormatUtils::DoubleToWideString( skewAngle.ToAngle() ));
				}break;
			case c3DXViewpoint:
				if (threeDStyleProps_.fUsefc3DParallel && !threeDStyleProps_.fc3DParallel)
				{
					ViewPointX = EmuValue( FixedPointNumber( iter->op ).Integral );
				}break;
			case c3DYViewpoint:
				if (threeDStyleProps_.fUsefc3DParallel && !threeDStyleProps_.fc3DParallel)
				{				
					ViewPointY = EmuValue( FixedPointNumber( iter->op ).Integral );
				}break;
			case c3DZViewpoint:
				if (threeDStyleProps_.fUsefc3DParallel && !threeDStyleProps_.fc3DParallel)
				{
					ViewPointZ = EmuValue( FixedPointNumber( iter->op ).Integral );
				}break;
			case c3DOriginX:
				if (threeDStyleProps_.fUsefc3DParallel && !threeDStyleProps_.fc3DParallel)
				{
					FixedPointNumber dOriginX( iter->op );
					viewPointOriginX = ( dOriginX.Integral / 65536.0 );
				}break;
			case c3DOriginY:
				if (threeDStyleProps_.fUsefc3DParallel && !threeDStyleProps_.fc3DParallel)
				{
					FixedPointNumber dOriginY( iter->op );
					viewPointOriginY = (dOriginY.Integral / 65536.0 );
				}break;
// TEXTBOX
			case lTxid:
				{
					hasTextbox	=	true;
					nLTxID		=	(((iter->op) >> 16) & 0xFFFF);
				}break;
			case dxTextLeft:	{ndxTextLeft	= (int)iter->op;	break;}
			case dyTextTop:		{ndyTextTop		= (int)iter->op;	break;}
			case dxTextRight:	{ndxTextRight	= (int)iter->op;	break;}
			case dyTextBottom:	{ndyTextBottom	= (int)iter->op;	break;}
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
// Word Art
			case gtextUNICODE:
				{
					std::wstring text = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)iter->opComplex.get(), (iter->op)/2);

					text = FormatUtils::XmlEncode(text);

					if (std::wstring::npos != text.find(L"\n"))
					{
						m_textpath.AppendText(text);
					}
					text = ReplaceString(text, L"\n", L"&#xA;");						
					appendValueAttribute(&m_textpath, L"string", text);
				}break;
			case gtextFont:
				{
					std::wstring font = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)iter->opComplex.get(), (iter->op)/2);
					size_t i = font.size();
					while (i > 0)
					{
						if (font[i-1] != 0) break;
						i--;
					}
					if (i < font.size()) font.erase(font.begin() + i, font.end());

					font = std::wstring(L"\"") + font + std::wstring(L"\"");
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
						appendValueAttribute(&m_textpath, L"fitshape", L"t");
					}
					if (props.fUsegtextFShrinkFit && props.gtextFShrinkFit)
					{
						appendValueAttribute(&m_textpath, L"trim", L"t");
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
			default:
				{
					int val = iter->op;
				}break;
			}
		}

		if (opVerticles && opSegmentInfo)
		{
			const unsigned char*	pVP	=	opVerticles->opComplex.get();
			unsigned int			nVP	=	opVerticles->op;
			const unsigned char*	pSI	=	opSegmentInfo->opComplex.get();
			unsigned int			nSI	=	opSegmentInfo->op;
		
			PathParser oParser (pSI, nSI, pVP, nVP, m_arrGuides);
			std::wstring path	=	oParser.GetVmlPath();

			if (false == path.empty())
				m_pXmlWriter->WriteAttribute (L"path", path);
		}
		if (freeform && (xCoord == 0 || yCoord == 0 ))
		{
			xCoord = 21600;
			yCoord = 21600;
		}
		if ( !bFilled )
		{
			m_pXmlWriter->WriteAttribute( L"filled", L"f" );
		}

		if ( !bStroked )
		{
			m_pXmlWriter->WriteAttribute( L"stroked", L"f" );
		}

		if (!layoutInCell) 
		{
			m_pXmlWriter->WriteAttribute(L"o:allowincell", L"f");
		}

		if ( xCoord > 0 && yCoord > 0 )
		{
			m_pXmlWriter->WriteAttribute( L"coordsize", ( FormatUtils::IntToWideString( xCoord ) + L"," + FormatUtils::IntToWideString( yCoord ) ));
		} 

		int nCode	=	0;
		if (pShape->GetShapeType())
		{
			nCode	=	pShape->GetShapeType()->GetTypeCode();
		}

		if (DocFileFormat::msosptRoundRectangle == nCode)
		{
			if (nAdjValues)												
			{
				m_pXmlWriter->WriteAttribute(L"arcsize", m_nAdjValues[0]);
			}
		}
		else
		{
			if (nAdjValues > 0)												
			{
				std::wstring adjTag	= std::to_wstring(m_nAdjValues[0]);

				for (int i = 1; i < nAdjValues; ++i)
					adjTag += L"," + std::to_wstring(m_nAdjValues[i]);

				m_pXmlWriter->WriteAttribute(L"adj", adjTag);
			}
		}

		m_pXmlWriter->WriteNodeEnd( L"", true, false );

//build shadow offsets
		std::wstring offset;

		if ( ShadowOffsetX != 0 )
		{
			offset += FormatUtils::DoubleToWideString( ShadowOffsetX.ToPoints() );
			offset += L"pt";
		}
		if ( ShadowOffsetY != 0 )
		{
			offset += L",";
			offset += FormatUtils::DoubleToWideString( ShadowOffsetY.ToPoints() );
			offset += L"pt";
		}
		if ( !offset.empty() )
		{
			appendValueAttribute(&m_shadow, L"offset", offset);
		}

		std::wstring offset2;

		if ( SecondShadowOffsetX != 0 )
		{
			offset2 += FormatUtils::DoubleToWideString( SecondShadowOffsetX.ToPoints() );
			offset2 += L"pt";
		}

		if ( SecondShadowOffsetY != 0 )
		{
			offset2 += L",";
			offset2 += FormatUtils::DoubleToWideString(SecondShadowOffsetY.ToPoints());
			offset2 += L"pt";
		}

		if (!offset2.empty())
		{
			appendValueAttribute(&m_shadow, L"offset2", offset2);
		}

//build shadow origin
		if ( ( ShadowOriginX != 0 ) && ( ShadowOriginY != 0 ) )
		{
			appendValueAttribute(&m_shadow, L"origin", (FormatUtils::DoubleToWideString(shadowOriginX) + std::wstring(L"," ) + FormatUtils::DoubleToWideString(shadowOriginY)));
		}

// write shadow
		if (m_shadow.GetAttributeCount() > 0)
		{
			appendValueAttribute(&m_shadow, L"on", bShadow ? L"t" : L"f" );
			m_pXmlWriter->WriteString(m_shadow.GetXMLString());
		}

//write the viewpoint
		if ( ( ViewPointX != 0 ) || ( ViewPointY != 0 ) || ( ViewPointZ != 0 ) )
		{
			std::wstring viewPoint;

			if ( ViewPointX != 0 )
			{
				viewPoint += FormatUtils::IntToWideString( ViewPointX ) + L"pt";
			}
			viewPoint += L",";
			if ( ViewPointY != 0 )
			{
				viewPoint += FormatUtils::IntToWideString( ViewPointY ) + L"pt";
			}
			viewPoint += L",";
			if ( ViewPointZ != 0 )
			{
				viewPoint += FormatUtils::IntToWideString( ViewPointZ ) + L"pt";
			}

			appendValueAttribute(&m_3dstyle, L"viewpoint", viewPoint);
		}
// write the viewpointorigin
		if ( ( viewPointOriginX != 0 ) || ( viewPointOriginY != 0 ) )
		{
			std::wstring viewPointOrigin;

			if ( viewPointOriginX != 0 )
			{
				viewPointOrigin += FormatUtils::DoubleToFormattedWideString( viewPointOriginX, L"%.2f" );
			}

			if ( viewPointOriginY != 0 )
			{
				viewPointOrigin += L",";
				viewPointOrigin += FormatUtils::DoubleToFormattedWideString( viewPointOriginY, L"%.2f" );
			}

			appendValueAttribute(&m_3dstyle, L"viewpointorigin", viewPointOrigin);
		}

//write 3d style 
		if (m_3dstyle.GetAttributeCount() > 0)
		{
			if (b3D)
			{
				appendValueAttribute(&m_3dstyle, L"v:ext", L"view" ); //??? вытащить
			}
			appendValueAttribute(&m_3dstyle, L"on", b3D ? L"t" : L"f" );
			m_pXmlWriter->WriteString(m_3dstyle.GetXMLString());
		}
// write wrap
		if (m_pSpa)
		{
			std::wstring wrap = getWrapType(m_pSpa);

			if (wrap != L"through")
			{
				m_pXmlWriter->WriteNodeBegin( L"w10:wrap", true );
				m_pXmlWriter->WriteAttribute( L"type", wrap );
				m_pXmlWriter->WriteNodeEnd	( L"w10:wrap", true );
			}
		}
// write fill
		if (m_fill.GetAttributeCount())
		{
			m_pXmlWriter->WriteString(m_fill.GetXMLString());
		}		
// write imagedata
		if (m_imagedata.GetAttributeCount())
		{
			m_pXmlWriter->WriteString(m_imagedata.GetXMLString());
		}		
		
		if ( pShape->fBackground )	
		{
			WriteEndShapeNode(pShape);
			return;
		}
// write stroke
		if (m_stroke.GetAttributeCount())
		{
			m_pXmlWriter->WriteString(m_stroke.GetXMLString());
		}
// text path
		if (m_textpath.GetAttributeCount())
		{
			appendValueAttribute(&m_textpath, L"style", FormatUtils::XmlEncode(m_textPathStyle));
			m_pXmlWriter->WriteString(m_textpath.GetXMLString());
		}
		if (freeform)
		{
			if (opInscribe || opConnectAngles || opConnectLocs)
			{
				std::vector<std::wstring>	arrInscribe			= GetTextRectangles(opInscribe);
				std::wstring				strConnectAngles	= GetConnectAngles(opConnectAngles);
				std::wstring				strConnectLocs		= GetConnectLocs(opConnectLocs);
				
				m_pXmlWriter->WriteNodeBegin(L"v:path", true);
				if (!arrInscribe.empty())
					m_pXmlWriter->WriteAttribute(L"textboxrect", arrInscribe[0]);
				if (!strConnectAngles.empty() || !strConnectLocs.empty())
				{
					m_pXmlWriter->WriteAttribute(L"o:connecttype", L"custom");
					if (!strConnectLocs.empty())
						m_pXmlWriter->WriteAttribute(L"o:connectlocs", strConnectLocs);
					if (!strConnectAngles.empty())
						m_pXmlWriter->WriteAttribute(L"o:connectangles", strConnectAngles);
				}

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
				TextboxMapping textboxMapping(m_context, nIndex - 1, m_pXmlWriter, m_pCaller);
				textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
				textboxMapping.SetTextboxStyle(sTextboxStyle);

				m_context->_doc->Convert(&textboxMapping);
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
				TextboxMapping textboxMapping(m_context, nLTxID - 1, m_pXmlWriter, m_pCaller);
				textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
				m_context->_doc->Convert(&textboxMapping);
			}
			else
			{
				TextboxMapping textboxMapping(m_context, m_pXmlWriter, m_pCaller);
				textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
				m_context->_doc->Convert(&textboxMapping);
			}
		}
		WriteEndShapeNode(pShape);
// OLE
		if (indexOLE >= 0 && pShape->fOleShape) //4571833.doc
		{
			XMLTools::CStringXmlWriter txtBoxWrapper;
			
			TextboxMapping textboxMapping(m_context, (indexOLE >> 16) - 1, &txtBoxWrapper, m_pCaller);

			textboxMapping.m_shapeIdOwner = m_shapeId;

			m_context->_doc->Convert(&textboxMapping);

			m_pXmlWriter->WriteString(textboxMapping.getOLEObject());
		}

//ShapeType 
		if (NULL != pShape->GetShapeType() /* && !m_isInlineShape*/) //bullete only???
		{
			VMLShapeTypeMapping oXmlMapper(m_pXmlWriter, m_isInlineShape);
			pShape->GetShapeType()->Convert(&oXmlMapper);
		}
	}

	//
	void VMLShapeMapping::WriteBeginShapeNode (const Shape* pShape)
	{
		if (NULL != pShape)
		{
			if (pShape->fBackground)
			{
				m_pXmlWriter->WriteNodeBegin( L"v:background", true );
			}
			else if (pShape->is<OvalType>())
			{
				m_pXmlWriter->WriteNodeBegin( L"v:oval", true );
			}
			else if (pShape->is<RoundedRectangleType>())
			{
				m_pXmlWriter->WriteNodeBegin( L"v:roundrect", true );
			}
			else if (pShape->is<RectangleType>())
			{
				m_pXmlWriter->WriteNodeBegin( L"v:rect", true );
			}
			else if (pShape->is<LineType>())
			{
				m_pXmlWriter->WriteNodeBegin(L"v:line", true);
			}
			else
			{
				m_pXmlWriter->WriteNodeBegin(L"v:shape", true);
			}
		}
	}

	void VMLShapeMapping::WriteEndShapeNode (const Shape* pShape)
	{
		if (NULL != pShape)
		{
			if (pShape->fBackground)
			{
				m_pXmlWriter->WriteNodeEnd( L"v:background" );
			}
			else if (pShape->is<OvalType>())
			{
				m_pXmlWriter->WriteNodeEnd( L"v:oval" );
			}
			else if (pShape->is<RoundedRectangleType>())
			{
				m_pXmlWriter->WriteNodeEnd( L"v:roundrect" );
			}
			else if (pShape->is<RectangleType>())
			{
				m_pXmlWriter->WriteNodeEnd( L"v:rect" );
			}
			else if (pShape->is<LineType>())
			{
				m_pXmlWriter->WriteNodeEnd( L"v:line" );
			}
			else
			{
				m_pXmlWriter->WriteNodeEnd( L"v:shape" );
			}
		}
	}

	std::wstring VMLShapeMapping::GetShapeID(const Shape* pShape) const
	{
		std::wstring strXmlAttr;

		if (NULL != pShape)
		{
			strXmlAttr += std::wstring(L"_x0000_s");
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
			strXmlFrom += L",";
			strXmlFrom += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.y);
		}
		else if (m_pSpa)
		{
			TwipsValue oLeft(m_pSpa->xaLeft);
			TwipsValue oTop(m_pSpa->yaTop);

			strXmlFrom += FormatUtils::DoubleToWideString(oLeft.ToPoints());
			strXmlFrom += L"pt,";
			strXmlFrom += FormatUtils::DoubleToWideString(oTop.ToPoints());
			strXmlFrom += L"pt";
		}

		return strXmlFrom;
	}

	std::wstring VMLShapeMapping::GetLineTo(const ChildAnchor* pAnchor) const
	{
		//Если линия находится в группе, то координаты должны быть в других единицах измерения (например в twips)

		std::wstring strXmlTo;

		if (NULL != pAnchor)
		{
			strXmlTo += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.x + pAnchor->rcgBounds.size.cx);
			strXmlTo += L",";
			strXmlTo += FormatUtils::IntToWideString(pAnchor->rcgBounds.topLeftAngle.y + pAnchor->rcgBounds.size.cy);
		}
		else if (m_pSpa)
		{
			TwipsValue oRight (m_pSpa->xaRight);
			TwipsValue oBottom (m_pSpa->yaBottom);

			strXmlTo += FormatUtils::DoubleToWideString(oRight.ToPoints());
			strXmlTo += L"pt,";
			strXmlTo += FormatUtils::DoubleToWideString(oBottom.ToPoints());
			strXmlTo += L"pt";
		}

		return strXmlTo;
	}

	/// Build the VML wrapcoords string for a given pWrapPolygonVertices
	std::wstring VMLShapeMapping::GetWrapCoords(const OptionEntryPtr& pWrapPolygonVertices) const
	{
		if (!pWrapPolygonVertices) return L"";

		std::wstring coords;

		MemoryStream oStream(pWrapPolygonVertices->opComplex.get(), pWrapPolygonVertices->op);
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
				coords += L",";
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
			VirtualStreamReader reader(m_context->_doc->WordDocumentStream, oBlip->foDelay, m_context->_doc->nWordVersion);

			switch (oBlip->btWin32)
			{
			case Global::msoblipEMF:
			case Global::msoblipWMF:
			case Global::msoblipPICT:
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
							m_context->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlip->btWin32), std::vector<unsigned char>(decompressed, (decompressed + decompressedSize))));
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
					if ((bitBlip) && (bitBlip->m_pvBits))
					{
						m_context->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlip->btWin32), 
							std::vector<unsigned char>(bitBlip->m_pvBits, (bitBlip->m_pvBits + bitBlip->pvBitsSize)), oBlip->btWin32));
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

			m_nImageId	=	m_context->_docx->RegisterImage (m_pCaller, oBlip->btWin32);

			result		=	true;
		}

		return result;
	}

	std::wstring VMLShapeMapping::GetTargetExt(Global::BlipType _type) const
	{
		switch ( _type )
		{
			case Global::msoblipDIB:
			  return std::wstring( L".bmp" );

			case Global::msoblipEMF:
				return std::wstring( L".emf" );

				//case msoblipGIF:
				//  return std::wstring( L".gif" );

				//case msoblipICON:
				//  return std::wstring( L".ico" );

			case Global::msoblipJPEG:
			case Global::msoblipCMYKJPEG:
				return std::wstring( L".jpg" );

				//case msoblipPCX:
				//  return std::wstring( L".pcx" );

			case Global::msoblipPNG:
				return std::wstring( L".png" );

			case Global::msoblipTIFF:
				return std::wstring( L".tif" );

			case Global::msoblipWMF:
				return std::wstring( L".wmf" );

			case Global::msoblipPICT:
				return std::wstring( L".pcz" );

			default:
				return std::wstring( L".png" );
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

			std::wstring widthString = FormatUtils::DoubleToWideString( width.ToPoints() );
			std::wstring heightString = FormatUtils::DoubleToWideString( height.ToPoints() );

			style->operator += ( std::wstring( L"width:" ) + widthString + std::wstring( L"pt;" ) );
			style->operator += ( std::wstring( L"height:" ) + heightString + std::wstring( L"pt;" ) );
		}
	}

	void VMLShapeMapping::AppendDimensionToStyle(std::wstring* style, const Spa* pSpa, bool twistDimensions) const
	{
		if ( ( style != NULL ) && (pSpa != NULL ) )
		{
			//append size and position ...

			//if (pSpa->fAnchorLock )
			//if (pSpa->bx == TEXT && pSpa->by == TEXT)
			//{
			//	appendStyleProperty( style, L"position", L"static" );
			//}
			//else
			//{
				appendStyleProperty( style, L"position", L"absolute" );
			//}

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

			appendStyleProperty (style, L"margin-left", ( FormatUtils::DoubleToWideString(left.ToPoints())		+ std::wstring( L"pt" )  ));
			appendStyleProperty (style, L"margin-top",	 ( FormatUtils::DoubleToWideString(top.ToPoints())		+ std::wstring( L"pt" )  ));
			appendStyleProperty (style, L"width",		 ( FormatUtils::DoubleToWideString(width.ToPoints())	+ std::wstring( L"pt" )  ));
			appendStyleProperty (style, L"height",		 ( FormatUtils::DoubleToWideString(height.ToPoints())	+ std::wstring( L"pt" )  ));
		}
	}

	void VMLShapeMapping::AppendDimensionToStyle(std::wstring* style, const ChildAnchor* anchor, bool twistDimensions) const
	{
		if ((style != NULL) && (anchor != NULL))
		{
			DocFileFormat::Rectangle bounds = anchor->rcgBounds;

			if (twistDimensions)
			{
				bounds.topLeftAngle.x	=	static_cast<LONG>(((anchor->Right	+	anchor->Left) * 0.5	- (anchor->Bottom	-	anchor->Top)  * 0.5));
				bounds.topLeftAngle.y	=	static_cast<LONG>(((anchor->Bottom	+	anchor->Top)  * 0.5 - (anchor->Right	-	anchor->Left) * 0.5));
			}

			appendStyleProperty(style, L"position", L"absolute");
			appendStyleProperty(style, L"left",	FormatUtils::IntToWideString(bounds.topLeftAngle.x));
			appendStyleProperty(style, L"top",	FormatUtils::IntToWideString(bounds.topLeftAngle.y));

			if (twistDimensions)
			{
				appendStyleProperty(style, L"width",	 FormatUtils::IntToWideString(bounds.size.cy));
				appendStyleProperty(style, L"height", FormatUtils::IntToWideString(bounds.size.cx));
			}
			else
			{
				appendStyleProperty(style, L"width",  FormatUtils::IntToWideString(bounds.size.cx));
				appendStyleProperty(style, L"height", FormatUtils::IntToWideString(bounds.size.cy));
			}
		}
	}

	void VMLShapeMapping::appendStyleProperty(std::wstring* b, const std::wstring& propName, const std::wstring& propValue) const
	{
		if ( b != NULL )
		{
			b->operator += ( propName );
			b->operator += ( L":" );
			b->operator += ( propValue );
			b->operator +=( L";" );
		}
	}

	std::wstring VMLShapeMapping::getTextboxAnchor(unsigned int anchor) const
	{
		switch ( anchor )
		{
			case 0:	return L"top";
			case 1:	return L"middle";
			case 2:	return L"bottom";
			case 3:	return L"top-center";
			case 4:	return L"middle-center";
			case 5:	return L"bottom-center";
			case 6:	return L"top-baseline";
			case 7:	return L"bottom-baseline";
			case 8:	return L"top-center-baseline";
			case 9:	return L"bottom-center-baseline";
			default:
				return L"top";
		}
	}

    std::wstring VMLShapeMapping::mapVerticalPosition(PositionVertical vPos)
	{
		switch ( vPos )
		{
			/*case msopvAbs:	return L"absolute" );*/
			case msopvTop:		return L"top";
			case msopvCenter:	return L"center";
			case msopvBottom:	return L"bottom";
			case msopvInside:	return L"inside";
			case msopvOutside:	return L"outside";
			default:
				return L"absolute";
		}
	}

    std::wstring VMLShapeMapping::mapVerticalPositionRelative(int vRel_)
	{
		PositionVerticalRelative vRel = (PositionVerticalRelative)vRel_;
		switch ( vRel )
		{
			case msoprvMargin:	return L"margin";
			case msoprvPage:	return L"page";
			case msoprvText:	return L"text";
			case msoprvLine:	return L"line";
			default:
				return L"margin";
		}
	}

    std::wstring VMLShapeMapping::mapHorizontalPosition(PositionHorizontal hPos)
	{
		switch ( hPos )
		{
			/*case msophAbs:	return L"absolute";*/
			case msophLeft:		return L"left";
			case msophCenter:	return L"center";
			case msophRight:	return L"right";
			case msophInside:	return L"inside";
			case msophOutside:	return L"outside";
			default:
				return L"absolute";
		}
	}

    std::wstring VMLShapeMapping::mapHorizontalPositionRelative(int hRel_ )
	{
		PositionHorizontalRelative hRel = (PositionHorizontalRelative )hRel_;
		switch ( hRel ) 
		{
			case msoprhMargin:	return L"margin";
			case msoprhPage:	return L"page";
			case msoprhText:	return L"text";
			case msoprhChar:	return L"char";
			default:
				return L"margin";
		}
	}

	void VMLShapeMapping::AppendOptionsToStyle (std::wstring* oStyle, const std::vector<OptionEntryPtr>& options, int zIndex) const
	{
		bool bRelH = false;
		bool bRelV = false;

		bool bPosH = false;
		bool bPosV = false;

		bool bZIndex = false;

		for (size_t i = 0; i < options.size(); i++)
		{
			const OptionEntryPtr & iter = options[i];
			switch (iter->pid)
			{
//	POSITIONING
			case posh:
				{
					appendStyleProperty(oStyle, L"mso-position-horizontal", mapHorizontalPosition((PositionHorizontal)iter->op));
					bPosH = true;
				}break;
			case posrelh:
				{
					appendStyleProperty(oStyle, L"mso-position-horizontal-relative", mapHorizontalPositionRelative((PositionHorizontalRelative)iter->op));
					bRelH = true;
				}break;
			case posv:
				{
					appendStyleProperty(oStyle, L"mso-position-vertical", mapVerticalPosition((PositionVertical)iter->op));
					bPosV = true;
				}break;
			case posrelv:
				{
					appendStyleProperty(oStyle, L"mso-position-vertical-relative", mapVerticalPositionRelative((PositionVerticalRelative)iter->op));
					bRelV = true;
				}break;
//	BOOLEANS
			case groupShapeBooleans:
				{
					GroupShapeBooleanProperties groupShapeBooleans(iter->op);

					if (groupShapeBooleans.fUsefBehindDocument && groupShapeBooleans.fBehindDocument && !bZIndex)
					{
						//The shape is behind the text, so the z-index must be negative.
						appendStyleProperty(oStyle, L"z-index", L"-1" );
						bZIndex = true;
					}
					else if (!m_isInlineShape && !bZIndex)
					{
						appendStyleProperty( oStyle, L"z-index", FormatUtils::IntToWideString(zIndex + 0x7ffff));
						bZIndex = true;
					}

					if (groupShapeBooleans.fHidden && groupShapeBooleans.fUsefHidden)
					{
						appendStyleProperty(oStyle, L"visibility", L"hidden" );
					}
				}break;
// GEOMETRY
			case PropertyId_rotation:
				{
					double dAngle = (double)((int)iter->op) / 65535.0;

					if (dAngle < -360.0)
						dAngle += 360.0;

					appendStyleProperty(oStyle, L"rotation", FormatUtils::DoubleToWideString(dAngle));
				}break;
// TEXTBOX	
			case anchorText:
				{
					appendStyleProperty(oStyle, L"v-text-anchor", getTextboxAnchor(iter->op));
				}break;
// WRAP DISTANCE
			case dxWrapDistLeft:
				{
					appendStyleProperty(oStyle, L"mso-wrap-distance-left", (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(L"pt")));
				}break;
			case dxWrapDistRight:
				{
					appendStyleProperty(oStyle, L"mso-wrap-distance-right", (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(L"pt")));
				}break;
			case dyWrapDistBottom:
				{
					appendStyleProperty(oStyle, L"mso-wrap-distance-bottom", (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(L"pt")));
				}break;
			case dyWrapDistTop:
				{
					appendStyleProperty(oStyle, L"mso-wrap-distance-top", (FormatUtils::DoubleToWideString(EmuValue((int)iter->op).ToPoints()) + std::wstring(L"pt")));
				}break;
			}
		}
		
		if (!bRelH && m_pSpa)
		{
			appendStyleProperty(oStyle, L"mso-position-horizontal-relative", mapHorizontalPositionRelative(m_pSpa->bx));
		}
		if (!bRelV && m_pSpa)
		{
			appendStyleProperty(oStyle, L"mso-position-vertical-relative", mapVerticalPositionRelative(m_pSpa->by));
		}
		if (!m_isInlineShape && !bZIndex)
		{
			appendStyleProperty( oStyle, L"z-index", FormatUtils::IntToWideString(zIndex + 0x7ffff));
			bZIndex = true;
		}

		//if (!bPosH)
		//{
		//	appendStyleProperty(oStyle, L"mso-position-horizontal", L"absolute" );
		//}
		//if (!bPosV)
		//{
		//	appendStyleProperty(oStyle, L"mso-position-vertical", L"absolute" );
		//}
	}

	//
	std::wstring VMLShapeMapping::buildStyle (const Shape* shape, const ChildAnchor* anchor, const std::vector<OptionEntryPtr>& options, int zIndex) const
	{
		std::wstring style;

		bool twistDimensions = false;
		
		for (size_t i = 0; i < options.size(); i++)
		{
			const OptionEntryPtr & iter = options[i];

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
				appendStyleProperty( &style, L"position", L"absolute" );
			}
			else if (m_pSpa)
			{
				//append size and position ...
				//if (m_pSpa->fAnchorLock)//это возможность смены привязки , а не ее тип

				//if (m_pSpa->bx == TEXT && m_pSpa->by == TEXT)
				//{
				//	appendStyleProperty( &style, L"position", L"static" );
				//}
				//else
				{
					appendStyleProperty( &style, L"position", L"absolute" );
				}
			}
		}

		if ( shape->fFlipH )
		{
			appendStyleProperty( &style, L"flip", L"x" );
		}

		if ( shape->fFlipV )
		{
			appendStyleProperty( &style, L"flip", L"y" );
		}

		AppendOptionsToStyle( &style, options, zIndex );
		
		return style;
	}

	std::wstring VMLShapeMapping::getLineStyle(unsigned int p) const
	{
		switch (p)
		{
			case 0:		return L"single";
			case 1:		return L"thinThin";
			case 2:		return L"thinThick";
			case 3:		return L"thickThin";
			case 4:		return L"thickBetweenThin";
			default:	return L"single";
		}
	}

	std::wstring VMLShapeMapping::getArrowStyle(unsigned int op) const
	{
		switch ( op )
		{
			default:	return L"none";
			case 1:		return L"block";
			case 2:		return L"classic";
			case 3:		return L"diamond";
			case 4:		return L"oval";
			case 5:		return L"open";
		}
	}

	std::wstring VMLShapeMapping::getArrowLength(unsigned int op) const
	{
		switch ( op )
		{
			default:	return L"short";
			case 1:		return L"medium";
			case 2:		return L"long";
		}
	}

	std::wstring VMLShapeMapping::getArrowWidth(unsigned int op) const
	{
		switch ( op )
		{
			default:	return L"narrow";
			case 1:		return L"medium";
			case 2:		return L"wide";
		}
	}

	std::wstring VMLShapeMapping::getFillMethod(unsigned int p) const
	{
		short val = (short)( ( p & 0xFFFF0000 ) >> 28 );

		switch ( val )
		{
		case 0:	return L"none";
		case 1:	return L"any";
		case 2:	return L"linear";
		case 4:	return L"linear sigma";
		default:
			return L"any";
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
				result += L"f #";
				result += color.SixDigitHexCode;
				result += L";";
			}
		}

		return result;
	}

	/// Returns the OpenXML fill type of a fill effect
	std::wstring VMLShapeMapping::getFillType(unsigned int p) const
	{
		switch ( p )
		{
		case 0:	return L"solid";
		case 1:	return L"pattern";
		case 2:	return L"tile";
		case 3:	return L"frame";
		case 4:	return L"gradient";
		case 5:	return L"gradientRadial";
		case 6:	return L"gradientRadial";
		case 7:	return L"gradient";
		case 9:
		default:
				return L"solid";
		}
	}

	std::wstring VMLShapeMapping::getShadowType(unsigned int p) const
	{
		switch ( p )
		{
		case 0:	return L"single";
		case 1:	return L"double";
		case 2:	return L"perspective";
		case 3:	return L"shaperelative";
		case 4:	return L"drawingrelative";
		case 5:	return L"emboss";
		default:
			return L"single";
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

		std::wstring wrapType = L"none";

		if (pSpa != NULL)
		{
			switch (pSpa->wr)
			{
				case 0:
				case 2:	wrapType = L"square";		break;
				case 1:	wrapType = L"topAndBottom";	break;
				case 3:	wrapType = L"through";		break;
				case 4:
				case 5:	wrapType = L"tight";		break;
				default:
						wrapType = L"none";			break;
			}
		}

		return wrapType;
	}
	std::wstring VMLShapeMapping::GetConnectAngles(const OptionEntryPtr& opAngles) const
	{
		if (!opAngles)				return L"";
		if (!opAngles->opComplex)	return L"";
		
		MemoryStream reader(opAngles->opComplex.get(), opAngles->op);

		unsigned short nElems		=	reader.ReadUInt16();
		unsigned short nElemsAlloc	=	reader.ReadUInt16();
		unsigned short nElemSize	=	reader.ReadUInt16();

		bool bTruncated = false;

        if (0xFFF0 == nElemSize)
        {
            nElemSize = 4;
            bTruncated = true;
        }

        long dwSize = nElems * nElemSize;

        if (opAngles->op - 6 != (dwSize))
        {
            bool b = false;
        }
		if (nElemSize < 1) return L"";

		int count = dwSize / nElemSize; 

		std::wstring angles;
		for (int i = 0; i < count; ++i)
		{
            DWORD v = reader.ReadUInt32();
			double val = (double)((WORD)(v >> 16) + ((WORD)(v) / 65536.0));
			angles += std::to_wstring((int)val) + (i < (count - 1) ? L"," : L"");
		}
		return angles;
	}
	int VMLShapeMapping::UpdateFromGuides(const int val)  const
	{
		int new_val = val;
        LONG lMinF = (LONG)0x80000000;
        if (lMinF <= val)
        {
            int index = (DWORD)val - 0x80000000;
		
			if (index >= 0 && index < (int)m_arrGuides.size())
			{
				new_val = m_arrGuides[index].param3;
			}

        }
		return new_val;
	}
	void VMLShapeMapping::GetGuides( const OptionEntryPtr& opGuides )
	{
		if (!opGuides)				return;
		if (!opGuides->opComplex)	return;
		
		MemoryStream reader(opGuides->opComplex.get(), opGuides->op);

		unsigned short nElems		=	reader.ReadUInt16();
		unsigned short nElemsAlloc	=	reader.ReadUInt16();
		unsigned short nElemSize	=	reader.ReadUInt16();

		bool bTruncated = false;

        if (0xFFF0 == nElemSize)
        {
            nElemSize = 4;
            bTruncated = true;
        }
		if (nElemSize == 0)
		{
			nElemSize = 2; //enredobar.doc
		}
        long dwSize = nElems * nElemSize;

        if (opGuides->op - 6 != (dwSize))
        {
            bool b = false;
			if (nElems > 0x7fff)
			{
				dwSize = (opGuides->op - 6);
			}
        }
		int count = dwSize / nElemSize;  //1x (int or short)
		for (int i = 0; i < count; ++i)
		{
			_guides g;
			WORD flags	= reader.ReadUInt16();

			g.type = flags & 0x1FFF;

			g.param_type1 = (unsigned char)(flags & 0x04);
			g.param_type2 = (unsigned char)(flags & 0x02);
			g.param_type3 = (unsigned char)(flags & 0x01);

			g.param1	= reader.ReadUInt16();
			g.param2	= reader.ReadUInt16();
			g.param3	= reader.ReadUInt16();

			m_arrGuides.push_back(g);
		}
	}

	std::wstring VMLShapeMapping::GetConnectLocs( const OptionEntryPtr& opLocs ) const
	{
		if (!opLocs)			return L"";
		if (!opLocs->opComplex) return L"";

		MemoryStream reader(opLocs->opComplex.get(), opLocs->op);

		unsigned short nElems		=	reader.ReadUInt16();
		unsigned short nElemsAlloc	=	reader.ReadUInt16();
		unsigned short nElemSize	=	reader.ReadUInt16();

		bool bTruncated = false;

        if (0xFFF0 == nElemSize)
        {
            nElemSize = 4;
            bTruncated = true;
        }
		if (nElemSize == 0)
		{
			nElemSize = 2; //enredobar.doc
		}
        long dwSize = nElems * nElemSize;

        if (opLocs->op - 6 != (dwSize))
        {
            bool b = false;
			if (nElems > 0x7fff)
			{
				dwSize = (opLocs->op - 6);
			}
        }
		int count = dwSize / nElemSize;  //2x (int or short)
		
		std::wstring locs;
		for (int i = 0; i < count; ++i)
		{
			POINT pt;

			if (bTruncated)
			{
				pt.x =	reader.ReadInt16();
				pt.y =	reader.ReadInt16();
			}
			else
			{
				pt.x =	reader.ReadInt32();
				pt.y =	reader.ReadInt32();
			}
           
			pt.x = UpdateFromGuides(pt.x);
			pt.y = UpdateFromGuides(pt.y);
			
			locs += std::to_wstring(pt.x) + L"," + std::to_wstring(pt.y) + (i < (count - 1) ? L";" : L"");
		}

		return locs;
	}

	std::vector<std::wstring> VMLShapeMapping::GetTextRectangles( const OptionEntryPtr& opInscribe ) const
	{
		std::vector<std::wstring> rectangles;
		
		if (!opInscribe) return rectangles;
		if (!opInscribe->opComplex) return rectangles;

		MemoryStream reader(opInscribe->opComplex.get(), opInscribe->op);

		unsigned short nElems		=	reader.ReadUInt16();
		unsigned short nElemsAlloc	=	reader.ReadUInt16();
		unsigned short nElemSize	=	reader.ReadUInt16();

		bool bTruncated = false;

        if (0xFFF0 == nElemSize)
        {
            nElemSize = 4;
            bTruncated = true;
        }
        else nElemSize = 2;

        long dwSize = nElems * nElemSize;

        if (opInscribe->op - 6 != (dwSize))
        {
            bool b = false;
        }
		int count = dwSize / nElemSize; //4x (int or short)
		
		for (int i = 0; i < count; ++i)
		{
			RECT rc;

			if (bTruncated)
			{
				rc.top		=	reader.ReadInt16();
				rc.left		=	reader.ReadInt16();
				rc.right	=	reader.ReadInt16();
				rc.bottom	=	reader.ReadInt16();
			}
			else
			{
				rc.top		=	reader.ReadInt32();
				rc.left		=	reader.ReadInt32();
				rc.right	=	reader.ReadInt32();
				rc.bottom	=	reader.ReadInt32();
			}
			rc.top		= UpdateFromGuides(rc.top);
			rc.left		= UpdateFromGuides(rc.left);
			rc.right	= UpdateFromGuides(rc.right);
			rc.bottom	= UpdateFromGuides(rc.bottom);

			rectangles.push_back(	std::to_wstring(rc.top)		+ L","	+ std::to_wstring(rc.left) + L"," + 
									std::to_wstring(rc.right)	+ L","	+ std::to_wstring(rc.bottom));
		}

		return rectangles;
	}
//------------------------------------------------------------------------------------------------------
	
	void VMLShapeMapping::ApplyPrimitives(DrawingPrimitives * primitives)
	{
		size_t index = 0;

		while(true)
		{
			index = ApplyPrimitive(primitives, index);
			if (index > primitives->size() - 1)
				break;
		}
	}

	static int currentTextBoxIndex = 1;
	
	size_t VMLShapeMapping::ApplyPrimitive(DrawingPrimitives * primitives, size_t index)
	{
		if (!primitives) return index++;
		if (index >= primitives->size()) return index++;

		DrawingPrimitive *primitive = dynamic_cast<DrawingPrimitive *>(primitives->at(index));
		
		m_pXmlWriter->WriteNodeBegin(primitive->strVmlType, true );
	
		if (primitive->type == 0x0007)
		{
			DrawingPrimitiveCTextBox * callout = dynamic_cast<DrawingPrimitiveCTextBox *>(primitive);
			if ((callout) && (callout->txbx))
			{
				//временно обычный текстбокс
				callout->txbx->xa += callout->xa;
				callout->txbx->ya += callout->ya;

				WritePrimitiveProps(dynamic_cast<DrawingPrimitive*>(callout->txbx), (index==0?true:false));
			}
			//todooo нарисовать кастомный шейп
		}
		else 
			WritePrimitiveProps(primitive, (index==0?true:false));

		
		if (primitive->type == 0x0000) 
		{
			index++;
			
			while(true)
			{
				if (index > primitives->size() - 1)
					break;			
				
				primitive = dynamic_cast<DrawingPrimitive *>(primitives->at(index));

				if (primitive->type == 0x0008)
				{
					break;
				}

				index = ApplyPrimitive(primitives, index);
			}
		}	

		if (primitive->type == 0x0002 || primitive->type == 0x0007) 
		{
			int nLTxID = currentTextBoxIndex++;;

			if (-1 != nLTxID)
			{
				TextboxMapping textboxMapping(m_context, nLTxID - 1, m_pXmlWriter, m_pCaller);
				//textboxMapping.SetInset(ndxTextLeft, ndyTextTop, ndxTextRight, ndyTextBottom);
				m_context->_doc->Convert(&textboxMapping);
			}
		}
		
		m_pXmlWriter->WriteNodeEnd( primitive->strVmlType );
	
		index++;
		return index;
	}

	void VMLShapeMapping::WritePrimitiveProps (DrawingPrimitive	* primitive, bool root)
	{
		TwipsValue x( primitive->xa );
		TwipsValue y( primitive->ya );
		TwipsValue w( primitive->dxa );
		TwipsValue h( primitive->dya );

		std::wstring strId = std::wstring(L"_x0000_s") + FormatUtils::IntToWideString(1024 + count_vml_objects);
		
		count_vml_objects++;
		//m_pXmlWriter->WriteAttribute ( L"id")	, strId);
		m_pXmlWriter->WriteAttribute ( L"o:spid", strId);

		std::wstring strStyle = L"position:absolute;visibility:visible;mso-wrap-style:square;";

		DrawingPrimitiveLine * line = dynamic_cast<DrawingPrimitiveLine *>(primitive);
		if (line)
		{
			TwipsValue x1( line->xaStart );
			TwipsValue y1( line->yaStart );
			TwipsValue x2( line->xaEnd );
			TwipsValue y2( line->yaEnd );
						
			std::wstring strStart	=  FormatUtils::IntToWideString(line->xaStart + primitive->xa) + L"," + FormatUtils::IntToWideString(line->yaStart + primitive->ya); 
			std::wstring strEnd		=  FormatUtils::IntToWideString(line->xaEnd + primitive->xa) + L"," + FormatUtils::IntToWideString(line->yaEnd + primitive->ya);

			m_pXmlWriter->WriteAttribute(L"from",  strStart);
			m_pXmlWriter->WriteAttribute(L"to",	strEnd);
		}
		else
		{
			if (root)
			{
				//strStyle += L"left:"		+ FormatUtils::IntToWideString( x.ToPoints()) + L"pt;";
				//strStyle += L"top:"		+ FormatUtils::IntToWideString( y.ToPoints()) + L"pt;";
				strStyle +=	L"width:"		+ FormatUtils::IntToWideString( (int)w.ToPoints()) + L"pt;";
				strStyle +=	L"height:"		+ FormatUtils::IntToWideString( (int)h.ToPoints()) + L"pt;";

				strStyle += L"margin-left:"	+ FormatUtils::IntToWideString( (int)x.ToPoints()) + L"pt;";
				strStyle +=	L"margin-top:"	+ FormatUtils::IntToWideString( (int)y.ToPoints()) + L"pt;";

				std::wstring xMargin;
				std::wstring yMargin;
				if (m_pSpa->bx == PAGE) xMargin = L"page;";
				if (m_pSpa->by == PAGE) yMargin = L"page;";
				
				if (m_pSpa->bx == MARGIN) xMargin = L"margin;";
				if (m_pSpa->by == MARGIN) yMargin = L"margin;";

				if (!xMargin.empty()) strStyle += L"mso-position-horizontal-relative:" + xMargin;
				if (!yMargin.empty()) strStyle += L"mso-position-vertical-relative:" + yMargin;

				std::wstring strSize = FormatUtils::IntToWideString(primitive->dxa) + L"," + FormatUtils::IntToWideString(primitive->dya);
				std::wstring strOrigin = FormatUtils::IntToWideString(primitive->xa) + L"," + FormatUtils::IntToWideString(primitive->ya);
				
				m_pXmlWriter->WriteAttribute( L"coordsize", strSize);
				//m_pXmlWriter->WriteAttribute( L"coordorigin"), strOrigin);
			}
			else
			{
				strStyle += L"left:"	+ FormatUtils::IntToWideString( primitive->xa)	+ L";";
				strStyle += L"top:"		+ FormatUtils::IntToWideString( primitive->ya)	+ L";";
				strStyle += L"width:"	+ FormatUtils::IntToWideString( primitive->dxa) + L";";
				strStyle += L"height:"	+ FormatUtils::IntToWideString( primitive->dya) + L";";
			}
		}
		if (primitive->fillPattern == 0)
			m_pXmlWriter->WriteAttribute( L"filled", L"f");
		
		if (primitive->type > 1)
		{
			m_pXmlWriter->WriteAttribute( L"fillColor", FormatUtils::IntToFormattedWideString(primitive->fillFore, L"#%06x"));
		}
		m_pXmlWriter->WriteAttribute( L"style", strStyle);

		std::wstring strStrokeWeight = FormatUtils::IntToWideString(primitive->lineWeight / 20) + L"pt";
		if (primitive->lineWeight > 20)
			m_pXmlWriter->WriteAttribute( L"strokeweight", strStrokeWeight);

		if (primitive->type > 0)
			m_pXmlWriter->WriteAttribute( L"strokecolor", FormatUtils::IntToFormattedWideString(primitive->lineColor, L"#%06x"));

		m_pXmlWriter->WriteNodeEnd( L"", true, false );
		
		if (primitive->type > 1 && primitive->fillPattern > 1)
		{
			m_pXmlWriter->WriteNodeBegin(L"v:fill", true );
				m_pXmlWriter->WriteAttribute( L"color2", FormatUtils::IntToFormattedWideString(primitive->fillBack, L"#%06x"));
				m_pXmlWriter->WriteAttribute( L"type", L"pattern");
			m_pXmlWriter->WriteNodeEnd( L"", true, false );
			m_pXmlWriter->WriteNodeEnd( L"v:fill" );
		}

		if (primitive->lineStyle > 1)
		{
			m_pXmlWriter->WriteNodeBegin(L"v:stroke", true );
				std::wstring strDashStyle = FormatUtils::IntToWideString(primitive->lineStyle) + L" 1";
				m_pXmlWriter->WriteAttribute( L"dashstyle", strDashStyle);
			m_pXmlWriter->WriteNodeEnd( L"", true, false );
			m_pXmlWriter->WriteNodeEnd( L"v:stroke" );
		}
	}

}
