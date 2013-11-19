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
#include "DrawingShape.h"
#include "DrawingTransform.h"
#include "DrawingPicture.h"
#include "DrawingCoreInfo.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CGraphic 20.1.2.2.16;20.1.2.2.17 (Part 1)
		//--------------------------------------------------------------------------------	
		enum EGraphicType
		{
			// Согласно спецификации Docx стр.3456 (20.4) элементами графики могут быть только
			graphictypePicture      = 0, // Картинки  20.2
			graphictypeLockedCanvas = 1, // Канвы     20.3
			graphictypeChart        = 2, // Графики   21.2
			graphictypeDiagram      = 3, // Диаграммы 21.4
		};

		class CGraphic : public WritingElement
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
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:graphicData") == sName )
					{
						ReadAttributes( oReader );
						
						if ( oReader.IsEmptyNode() )
							break;

						int nGraphicDataDepth = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nGraphicDataDepth ) )
						{
							sName = oReader.GetName();
							if ( _T("pic:pic") == sName )
							{
								m_oPicture     = oReader;
								m_eGraphicType = graphictypePicture;
							}
							else if ( _T("c:chart") == sName )
							{
								m_oChart     = oReader;
								m_eGraphicType = graphictypeChart;
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

			// Childs
			nullable<OOX::Drawing::CPicture> m_oPicture;
			nullable<OOX::Drawing::CChart> m_oChart;

		};
	} // Drawing

} // OOX


#endif // OOX_LOGIC_DRAWING_GAPHIC_INFO_INCLUDE_H_