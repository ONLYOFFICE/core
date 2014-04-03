#pragma once
#ifndef OOX_POS_FILE_INCLUDE_H_
#define OOX_POS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CPos : public WritingElementWithChilds<>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPos)
			CPos()
			{
			}
			virtual ~CPos()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<xdr:pos"));
				if(m_oX.IsInit())
				{
					CString sVal;sVal.Format(_T(" x=\"%I64d\""), m_oX->ToEmu());
					writer.WriteString(sVal);
				}
				if(m_oY.IsInit())
				{
					CString sVal;sVal.Format(_T(" y=\"%I64d\""), m_oY->ToEmu());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_FromTo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("x"),      m_oX )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("y"),      m_oY )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CEmu>			m_oX;
			nullable<SimpleTypes::CEmu>			m_oY;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_POS_FILE_INCLUDE_H_