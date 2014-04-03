#pragma once
#ifndef OOX_CHARTLAYOUT_FILE_INCLUDE_H_
#define OOX_CHARTLAYOUT_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartManualLayout : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartManualLayout)
			CChartManualLayout() {}
			virtual ~CChartManualLayout() {}

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

					if ( _T("c:h") == sName )
						m_oH = oReader;
					else if ( _T("c:hMode") == sName )
						m_oHMode = oReader;
					else if ( _T("c:layoutTarget") == sName )
						m_oLayoutTarget = oReader;
					else if ( _T("c:w") == sName )
						m_oW = oReader;
					else if ( _T("c:wMode") == sName )
						m_oWMode = oReader;
					else if ( _T("c:x") == sName )
						m_oX = oReader;
					else if ( _T("c:xMode") == sName )
						m_oXMode = oReader;
					else if ( _T("c:y") == sName )
						m_oY = oReader;
					else if ( _T("c:yMode") == sName )
						m_oYMode = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_ManualLayout;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:

			// Attributes
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oH;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oHMode;
			nullable<ComplexTypes::Spreadsheet::CChartLayoutTarget> m_oLayoutTarget;
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oW;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oWMode;
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oX;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oXMode;
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oY;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oYMode;
		};
		class CChartLayout : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartLayout)
			CChartLayout() {}
			virtual ~CChartLayout() {}

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

					if ( _T("c:manualLayout") == sName )
						m_oManualLayout = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_Layout;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:

			// Attributes
			nullable<CChartManualLayout> m_oManualLayout;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CHARTLAYOUT_FILE_INCLUDE_H_