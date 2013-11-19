#pragma once
#ifndef OOX_BORDERS_FILE_INCLUDE_H_
#define OOX_BORDERS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CBorderProp : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CBorderProp)
			CBorderProp()
			{
			}
			virtual ~CBorderProp()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
			}
			void toXML2(CStringWriter& writer, CString sName) const
			{
				writer.WriteStringC(_T("<"));
				writer.WriteStringC(sName);
				if(m_oStyle.IsInit())
				{
					CString sVal;sVal.Format(_T(" style=\"%s\""), m_oStyle->ToString());
					writer.WriteStringC(sVal);
				}
				writer.WriteStringC(_T(">"));
				if(m_oColor.IsInit())
					m_oColor->toXML2(writer, _T("color"));
				writer.WriteStringC(_T("</"));
				writer.WriteStringC(sName);
				writer.WriteStringC(_T(">"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("color") == sName )
						m_oColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_BorderProp;
			}
			bool IsEmpty()
			{
				return !(m_oStyle.IsInit() || m_oColor.IsInit());
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("style"),      m_oStyle )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CBorderStyle<>>	m_oStyle;

			nullable<CColor>						m_oColor;
		};

		class CBorder : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CBorder)
			CBorder()
			{
			}
			virtual ~CBorder()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<border"));
				if(m_oDiagonalDown.IsInit() && SimpleTypes::onoffTrue == m_oDiagonalDown->GetValue())
				{
					CString sVal;sVal.Format(_T(" diagonalDown=\"%s\""), m_oDiagonalDown->ToString2( SimpleTypes::onofftostring1));
					writer.WriteStringC(sVal);
				}
				if(m_oDiagonalUp.IsInit() && SimpleTypes::onoffTrue == m_oDiagonalUp->GetValue())
				{
					CString sVal;sVal.Format(_T(" diagonalUp=\"%s\""), m_oDiagonalUp->ToString2( SimpleTypes::onofftostring1));
					writer.WriteStringC(sVal);
				}
				writer.WriteStringC(_T(">"));
				if(m_oStart.IsInit() && false == m_oStart->IsEmpty())
					m_oStart->toXML2(writer, _T("left"));
				else
					writer.WriteStringC(_T("<left/>"));
				if(m_oEnd.IsInit() && false == m_oEnd->IsEmpty())
					m_oEnd->toXML2(writer, _T("right"));
				else
					writer.WriteStringC(_T("<right/>"));
				if(m_oTop.IsInit() && false == m_oTop->IsEmpty())
					m_oTop->toXML2(writer, _T("top"));
				else
					writer.WriteStringC(_T("<top/>"));
				if(m_oBottom.IsInit() && false == m_oBottom->IsEmpty())
					m_oBottom->toXML2(writer, _T("bottom"));
				else
					writer.WriteStringC(_T("<bottom/>"));
				if(m_oDiagonal.IsInit() && false == m_oDiagonal->IsEmpty())
					m_oDiagonal->toXML2(writer, _T("diagonal"));
				else
					writer.WriteStringC(_T("<diagonal/>"));
				if(m_oHorizontal.IsInit() && false == m_oHorizontal->IsEmpty())
					m_oHorizontal->toXML2(writer, _T("horizontal"));
				if(m_oVertical.IsInit() && false == m_oVertical->IsEmpty())
					m_oVertical->toXML2(writer, _T("vertical"));
				writer.WriteStringC(_T("</border>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("bottom") == sName )
						m_oBottom = oReader;
					else if ( _T("diagonal") == sName )
						m_oDiagonal = oReader;
					else if ( _T("end") == sName || _T("right") == sName )
						m_oEnd = oReader;
					else if ( _T("horizontal") == sName )
						m_oHorizontal = oReader;
					else if ( _T("start") == sName || _T("left") == sName )
						m_oStart = oReader;
					else if ( _T("top") == sName )
						m_oTop = oReader;
					else if ( _T("vertical") == sName )
						m_oVertical = oReader;

				}
			}

			virtual EElementType getType () const
			{
				return et_Border;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("diagonalDown"),      m_oDiagonalDown )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("diagonalUp"),      m_oDiagonalUp )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("outline"),      m_oOutline )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<>>				m_oDiagonalDown;
			nullable<SimpleTypes::COnOff<>>				m_oDiagonalUp;
			nullable<SimpleTypes::COnOff<>>				m_oOutline;

			nullable<CBorderProp>						m_oBottom;
			nullable<CBorderProp>						m_oDiagonal;
			nullable<CBorderProp>						m_oEnd;
			nullable<CBorderProp>						m_oHorizontal;
			nullable<CBorderProp>						m_oStart;
			nullable<CBorderProp>						m_oTop;
			nullable<CBorderProp>						m_oVertical;
		};

		class CBorders : public WritingElementWithChilds<CBorder>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CBorders)
			CBorders()
			{
			}
			virtual ~CBorders()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<borders"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteStringC(sVal);
				}
				writer.WriteStringC(_T(">"));
				for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteStringC(_T("</borders>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("border") == sName )
						m_arrItems.Add( new CBorder( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_Borders;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_BORDERS_FILE_INCLUDE_H_