#pragma once
#ifndef OOX_LOGIC_DRAWING_BODY_INCLUDE_H_
#define OOX_LOGIC_DRAWING_BODY_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

#include "DrawingTextProperties.h"
#include "DrawingParagraph.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// 20.1.2.2.40 txBody (Shape Text Body)
		//--------------------------------------------------------------------------------	
		class CTxBody : public WritingElementWithChilds<Drawing::CParagraph>
		{
		public:
			WritingElement_AdditionConstructors(CTxBody)
			CTxBody()
			{
			}
			virtual ~CTxBody()
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:bodyPr") == sName )
						m_oBodyPr = oReader;
					//if ( _T("a:lstStyle") == sName )
					//	m_oLstStyle = oReader;
					else if ( _T("a:p") == sName )
						m_arrItems.push_back( new Drawing::CParagraph( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_a_TextShapeBody;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Drawing::CTextBodyProperties>	m_oBodyPr;
			//nullable<OOX::Drawing::CTextListStyle>	m_oLstStyle;
		};
		//--------------------------------------------------------------------------------
		// 20.1.2.2.41 txSp (Text Shape)
		//--------------------------------------------------------------------------------	
		class CTxSp : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTxSp)
			CTxSp()
			{
			}
			virtual ~CTxSp()
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: –Â‡ÎËÁÓ‚‡Ú¸ fromXML(XmlUtils::CXmlNode& oNode)
			}
		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:txBody") == sName )
						m_oTxBody = oReader;
					//if ( _T("a:lstStyle") == sName )
					//	m_oLstStyle = oReader;

				}
			}

			virtual EElementType getType () const
			{
				return et_a_TextShape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Drawing::CTxBody>		m_oTxBody;
//extLst (Extension List) ß20.1.2.2.15
//useSpRect (Use Shape Text Rectangle) ß20.1.2.2.42
//xfrm (2D Transform for Individual Objects) ß20.1.7.6
		};
	} // namespace Drawing
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_BODY_INCLUDE_H_
