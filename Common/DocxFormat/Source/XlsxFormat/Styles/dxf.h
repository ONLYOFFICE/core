#pragma once
#ifndef OOX_DXFS_FILE_INCLUDE_H_
#define OOX_DXFS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Borders.h"
#include "Fills.h"
#include "Fonts.h"
#include "NumFmts.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CDxf : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CDxf)
			CDxf()
			{
			}
			virtual ~CDxf()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(CString(_T("<dxf>")));
				if(m_oFont.IsInit())
					m_oFont->toXML(writer);
				if(m_oNumFmt.IsInit())
					m_oNumFmt->toXML(writer);
				if(m_oFill.IsInit())
					m_oFill->toXML(writer);
				if(m_oAlignment.IsInit())
					m_oAlignment->toXML(writer);
				if(m_oBorder.IsInit())
					m_oBorder->toXML(writer);
				if(m_oProtection.IsInit())
					m_oProtection->toXML(writer);
				writer.WriteString(CString(_T("</dxf>")));
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

					if ( _T("alignment") == sName )
						m_oAlignment = oReader;
					else if ( _T("border") == sName )
						m_oBorder = oReader;
					else if ( _T("fill") == sName )
						m_oFill = oReader;
					else if ( _T("font") == sName )
						m_oFont = oReader;
					else if ( _T("numFmt") == sName )
						m_oNumFmt = oReader;
					else if ( _T("protection") == sName )
						m_oProtection = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_Dxf;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CAligment>		m_oAlignment;
			nullable<CBorder>		m_oBorder;
			nullable<CFill>			m_oFill;
			nullable<CFont>			m_oFont;
			nullable<CNumFmt>		m_oNumFmt;
			nullable<CProtection>	m_oProtection;
		};
		class CDxfs : public WritingElementWithChilds<CDxf>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CDxfs)
			CDxfs()
			{
			}
			virtual ~CDxfs()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<dxfs"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</dxfs>"));
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

					if ( _T("dxf") == sName )
						m_arrItems.push_back( new CDxf(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_Dxfs;
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

#endif // OOX_DXFS_FILE_INCLUDE_H_