#pragma once
#ifndef OOX_PHONETIC_FILE_INCLUDE_H_
#define OOX_PHONETIC_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Text.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		class CPhonetic : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPhonetic)
			CPhonetic()
			{
			}
			virtual ~CPhonetic()
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

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual EElementType getType () const
			{
				return et_PhoneticPr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("alignment"),      m_oAlignment )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("fontId"),      m_oFontId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
				nullable<SimpleTypes::Spreadsheet::CPhoneticAlignment<>>	m_oAlignment;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>				m_oFontId;
				nullable<SimpleTypes::Spreadsheet::CPhoneticType<>>			m_oType;
		};

		class CRPh : public WritingElementWithChilds<CText>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CRPh)
			CRPh()
			{
			}
			virtual ~CRPh()
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
					CWCharWrapper sName = oReader.GetName();

					if ( _T("t") == sName )
						m_arrItems.Add( new CText( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_rPh;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("eb"),      m_oEb )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sb"),      m_oSb )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oEb;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oSb;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_PHONETIC_FILE_INCLUDE_H_