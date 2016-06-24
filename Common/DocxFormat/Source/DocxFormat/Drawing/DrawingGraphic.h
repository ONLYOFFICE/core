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
#pragma once
#ifndef OOX_LOGIC_DRAWING_GAPHIC_INFO_INCLUDE_H_
#define OOX_LOGIC_DRAWING_GAPHIC_INFO_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "DrawingExt.h"
#include "DrawingStyles.h"

#include "DrawingTransform.h"

#include "DrawingPicture.h"
#include "DrawingShape.h"
#include "../Logic/Shape.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CGraphic 20.1.2.2.16; 20.1.2.2.17 (Part 1)
		//--------------------------------------------------------------------------------	
		enum EGraphicType
		{
			// Согласно спецификации Docx стр.3456 (20.4) элементами графики могут быть только
			graphictypePicture      = 0, // Картинки		20.2
			graphictypeLockedCanvas = 1, // Канвы			20.3
			graphictypeChart        = 2, // Графики			21.2
			graphictypeDiagram      = 3, // Диаграммы		21.4
			graphictypeShape	    = 4, // Шейпы			20.4
			graphictypeGroupShape   = 5, // Группы Шейпов	20.4
		};

		class CGraphic : public WritingElementWithChilds<WritingElement> //в данном случае псевдо - так как ДОЛЖЕН быть тока 1 элемент
		{
		public:
			WritingElement_AdditionConstructors(CGraphic)
			CGraphic()
			{
			}
			virtual ~CGraphic()
			{
			}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CGraphic::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nGraphicDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nGraphicDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:graphicData") == sName )
					{
						// 20.1.2.2.17 graphicData (Graphic Object Data)
						//Child Elements/Subclause = Any element in any namespace/n/a
						ReadAttributes( oReader );
						
						if ( oReader.IsEmptyNode() )
							break;

						int nGraphicDataDepth = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nGraphicDataDepth ) )
						{
							sName = oReader.GetName();
							if ( _T("pic:pic") == sName )
							{
								m_arrItems.push_back( new OOX::Drawing::CPicture( oReader ));
								m_eGraphicType = graphictypePicture;
							}
							else if ( _T("c:chart") == sName )
							{
								m_arrItems.push_back(  new OOX::Drawing::CChart( oReader ));
								m_eGraphicType = graphictypeChart;
							}
							else if ( _T("wps:wsp") == sName )
							{
								m_arrItems.push_back(  new OOX::Logic::CShape( oReader ));
								m_eGraphicType = graphictypeShape;
							}
							else if ( _T("wpg:wgp") == sName )
							{
								m_arrItems.push_back( new OOX::Logic::CGroupShape( oReader ));
								m_eGraphicType = graphictypeGroupShape;
							}
							else if ( _T("wpc:wpc") == sName )
							{
								m_arrItems.push_back( new OOX::Logic::CLockedCanvas( oReader ));
								m_eGraphicType = graphictypeLockedCanvas;
							}
							else if ( _T("lc:lockedCanvas") == sName )
							{
								m_arrItems.push_back( new OOX::Drawing::CLockedCanvas( oReader ));
								m_eGraphicType = graphictypeLockedCanvas;
							}
							else if ( _T("dgm:relIds") == sName )
							{
								m_arrItems.push_back( new OOX::Drawing::CDiagrammParts( oReader ));
								m_eGraphicType = graphictypeDiagram;
							}
						}
					} 
				}
			}
			virtual CString      toXML() const
			{
				// TO DO: Реализовать CGraphic::toXML()
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_graphic;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("uri"), m_sUri )
				WritingElement_ReadAttributes_End( oReader )
			}

        public:
			EGraphicType                     m_eGraphicType;
			// Attributes
			nullable<CString>                m_sUri;
			//Child
		};
	} // Drawing

} // OOX


#endif // OOX_LOGIC_DRAWING_GAPHIC_INFO_INCLUDE_H_
