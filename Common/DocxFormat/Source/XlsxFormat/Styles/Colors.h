#pragma once
#ifndef OOX_COLORS_FILE_INCLUDE_H_
#define OOX_COLORS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CColors : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CColors)
			CColors()
			{
			}
			virtual ~CColors()
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

					if ( _T("indexedColors") == sName )
						m_oIndexedColors = oReader;
					else if ( _T("mruColors") == sName )
						m_oMruColors = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_Colors;
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

			nullable<CIndexedColors>		m_oIndexedColors;
			nullable<CMruColors>		m_oMruColors;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_COLORS_FILE_INCLUDE_H_