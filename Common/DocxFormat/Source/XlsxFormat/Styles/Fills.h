#pragma once
#ifndef OOX_FILLS_FILE_INCLUDE_H_
#define OOX_FILLS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPatternFill : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPatternFill)
			CPatternFill()
			{
			}
			virtual ~CPatternFill()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<patternFill"));
				if(m_oPatternType.IsInit())
				{
					CString sVal;sVal.Format(_T(" patternType=\"%s\""), m_oPatternType->ToString());
					writer.WriteString(sVal);
				}
				if(m_oBgColor.IsInit() || m_oFgColor.IsInit())
				{
					writer.WriteString(_T(">"));
					if(m_oBgColor.IsInit() && m_oFgColor.IsInit())
					{
						m_oFgColor->toXML2(writer, _T("fgColor"));
						m_oBgColor->toXML2(writer, _T("bgColor"));
					}
					else if(m_oFgColor.IsInit())
					{
						m_oFgColor->toXML2(writer, _T("fgColor"));
						m_oFgColor->toXML2(writer, _T("bgColor"));
					}
					else if(m_oBgColor.IsInit())
					{
						m_oBgColor->toXML2(writer, _T("fgColor"));
						m_oBgColor->toXML2(writer, _T("bgColor"));
					}
					
					writer.WriteString(_T("</patternFill>"));
				}
				else
					writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("bgColor") == sName )
						m_oBgColor = oReader;
					else if ( _T("fgColor") == sName )
						m_oFgColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_PatternFill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("patternType"),      m_oPatternType )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CPatternType<>>	m_oPatternType;
			nullable<CColor>									m_oBgColor;
			nullable<CColor>									m_oFgColor;
		};
		class CGradientStop : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGradientStop)
			CGradientStop()
			{
			}
			virtual ~CGradientStop()
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("color") == sName )
						m_oColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_GradientStop;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("position"),      m_oPosition )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDouble>		m_oPosition;
			nullable<CColor>					m_oColor;
		};
		class CGradientFill : public WritingElementWithChilds<CGradientStop>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGradientFill)
			CGradientFill()
			{
			}
			virtual ~CGradientFill()
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("stop") == sName )
						m_arrItems.push_back( new CGradientStop( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_GradientFill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("bottom"),      m_oBottom )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("degree"),      m_oDegree )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("left"),      m_oLeft )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("right"),      m_oRight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("top"),      m_oTop )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDouble>		m_oBottom;
			nullable<SimpleTypes::CDouble>		m_oDegree;
			nullable<SimpleTypes::CDouble>		m_oLeft;
			nullable<SimpleTypes::CDouble>		m_oRight;
			nullable<SimpleTypes::CDouble>		m_oTop;
			nullable<SimpleTypes::Spreadsheet::CGradientType<>>		m_oType;
		};
		class CFill : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFill)
			CFill()
			{
			}
			virtual ~CFill()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<fill>"));
				if(m_oPatternFill.IsInit())
					m_oPatternFill->toXML(writer);
				writer.WriteString(_T("</fill>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("gradientFill") == sName )
						m_oGradientFill = oReader;
					else if ( _T("patternFill") == sName )
						m_oPatternFill = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_Fill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CGradientFill>		m_oGradientFill;
			nullable<CPatternFill>		m_oPatternFill;
		};
		class CFills : public WritingElementWithChilds<CFill>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFills)
			CFills()
			{
			}
			virtual ~CFills()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<fills"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</fills>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("fill") == sName )
						m_arrItems.push_back( new CFill( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_Fills;
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

#endif // OOX_FILLS_FILE_INCLUDE_H_