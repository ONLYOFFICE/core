#pragma once
#ifndef OOX_FROMTO_FILE_INCLUDE_H_
#define OOX_FROMTO_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFromTo : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFromTo)
			CFromTo()
			{
			}
			virtual ~CFromTo()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
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

					if ( _T("xdr:col") == sName )
						m_oCol = oReader.GetText2();
					else if ( _T("xdr:colOff") == sName )
						m_oColOff = oReader.GetText2();
					else if ( _T("xdr:row") == sName )
						m_oRow = oReader.GetText2();
					else if ( _T("xdr:rowOff") == sName )
						m_oRowOff = oReader.GetText2();
				}
			}

			virtual EElementType getType () const
			{
				return et_FromTo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCol;
			nullable<SimpleTypes::CEmu>			m_oColOff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRow;
			nullable<SimpleTypes::CEmu>			m_oRowOff;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_FROMTO_FILE_INCLUDE_H_