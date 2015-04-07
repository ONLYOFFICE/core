#pragma once
#ifndef OOX_LOGIC_DRAWING_PICTURE_INCLUDE_H_
#define OOX_LOGIC_DRAWING_PICTURE_INCLUDE_H_

#include "DrawingCoreInfo.h"
#include "DrawingEffects.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CPictureNonVisual 20.2.2.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPictureNonVisual : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPictureNonVisual)
			CPictureNonVisual()
			{
				m_eType = et_Unknown;
			}
			virtual ~CPictureNonVisual()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;

				// TO DO : Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;

                CString sName = oReader.GetName();
				if ( _T("pic:nvPicPr") == sName )
					m_eType = et_pic_nvPicPr;
				else
					return;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();

					if ( _T("pic:cNvPicPr") == sName )
						m_oCNvPicPr = oReader;
					else if ( _T("pic:cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;

				if ( et_pic_nvPicPr == m_eType )
					sResult = _T("<pic:nvPicPr>");
				else
					return _T("");

				sResult += m_oCNvPr.toXML();
				sResult += m_oCNvPicPr.toXML();

				if ( et_pic_nvPicPr == m_eType )
					sResult += _T("</pic:nvPicPr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		public:

			EElementType      m_eType;

			// Childs
			OOX::Drawing::CNonVisualPictureProperties m_oCNvPicPr;
			OOX::Drawing::CNonVisualDrawingProps      m_oCNvPr;
		};
		//--------------------------------------------------------------------------------
		// CPicture 20.2.2.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPicture : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPicture)
			CPicture()
			{
			}
			virtual ~CPicture()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPicture::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) ) 
				{
                    CString sName = oReader.GetName();
					if ( _T("pic:nvPicPr") == sName )
						m_oNvPicPr = oReader;
					else if ( _T("pic:blipFill") == sName )
						m_oBlipFill = oReader;
					else if ( _T("pic:spPr") == sName )
						m_oSpPr = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<pic:pic>");
				sResult += m_oNvPicPr.toXML();
				sResult += m_oBlipFill.toXML();
				sResult += m_oSpPr.toXML();
				sResult += _T("</pic:pic>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_pic_pic;
			}

		public:

			// Childs
			OOX::Drawing::CBlipFillProperties m_oBlipFill;
			OOX::Drawing::CPictureNonVisual   m_oNvPicPr;
			OOX::Drawing::CShapeProperties    m_oSpPr;

		};

		class CChart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CChart)
			CChart()
			{
			}
			virtual ~CChart()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPicture::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_c_chart;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"), m_oRId )
					WritingElement_ReadAttributes_End( oReader )
			}
		public:

			// Childs
			nullable<SimpleTypes::CRelationshipId> m_oRId;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_PICTURE_INCLUDE_H_
