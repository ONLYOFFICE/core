#pragma once
#ifndef OOX_CHARTSTYLE_FILE_INCLUDE_H_
#define OOX_CHARTSTYLE_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartStyle : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartStyle)
			CChartStyle()
			{
			}
			virtual ~CChartStyle()
			{
			}
		public:
			virtual CString      toXML() const
			{
				int nStyle = 0;
				if(m_oVal.IsInit())
					nStyle = m_oVal->GetValue();
				if(false == (nStyle >= 1 && nStyle <= 48))
					nStyle = 2;
				CString sXml;
				sXml.Format(_T("<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\"><mc:Choice Requires=\"c14\" xmlns:c14=\"http://schemas.microsoft.com/office/drawing/2007/8/2/chart\"><c14:style val=\"%d\"/></mc:Choice><mc:Fallback><c:style val=\"%d\"/></mc:Fallback></mc:AlternateContent>"), 100 + nStyle, nStyle);
				return sXml;
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(toXML());
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_c_ChartStyle;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"), m_oVal )
					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>						m_oVal;
		};
	} // namespace Spreadsheet
} // namespace OOX

#endif // OOX_CHARTSTYLE_FILE_INCLUDE_H_