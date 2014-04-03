#pragma once
#ifndef OOX_BOOKVIEWS_FILE_INCLUDE_H_
#define OOX_BOOKVIEWS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CWorkbookView : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CWorkbookView)
			CWorkbookView()
			{
			}
			virtual ~CWorkbookView()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<workbookView"));
				if(m_oXWindow.IsInit())
				{
					CString sVal;sVal.Format(_T(" xWindow=\"%d\""), m_oXWindow->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oYWindow.IsInit())
				{
					CString sVal;sVal.Format(_T(" yWindow=\"%d\""), m_oYWindow->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oWindowWidth.IsInit())
				{
					CString sVal;sVal.Format(_T(" windowWidth=\"%d\""), m_oWindowWidth->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oWindowHeight.IsInit())
				{
					CString sVal;sVal.Format(_T(" windowHeight=\"%d\""), m_oWindowHeight->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oActiveTab.IsInit())
				{
					CString sVal;sVal.Format(_T(" activeTab=\"%d\""), m_oActiveTab->GetValue());
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
				return et_WorkbookView;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("activeTab"),      m_oActiveTab )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("autoFilterDateGrouping"),      m_oAutoFilterDateGrouping )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("firstSheet"),      m_oFirstSheet )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("minimized"),      m_oMinimized )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showHorizontalScroll"),      m_oShowHorizontalScroll )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showSheetTabs"),      m_oShowSheetTabs )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showVerticalScroll"),      m_oShowVerticalScroll )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tabRatio"),      m_oTabRatio )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("visibility"),      m_oVisibility )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowHeight"),      m_oWindowHeight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowWidth"),      m_oWindowWidth )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xWindow"),      m_oXWindow )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("yWindow"),      m_oYWindow )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oActiveTab;
				nullable<SimpleTypes::COnOff<>>						m_oAutoFilterDateGrouping;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oFirstSheet;
				nullable<SimpleTypes::COnOff<>>						m_oMinimized;
				nullable<SimpleTypes::COnOff<>>						m_oShowHorizontalScroll;
				nullable<SimpleTypes::COnOff<>>						m_oShowSheetTabs;
				nullable<SimpleTypes::COnOff<>>						m_oShowVerticalScroll;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTabRatio;
				nullable<SimpleTypes::Spreadsheet::CVisibleType<>>	m_oVisibility;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWindowHeight;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWindowWidth;
				nullable<SimpleTypes::CDecimalNumber<>>				m_oXWindow;
				nullable<SimpleTypes::CDecimalNumber<>>				m_oYWindow;
		};

		class CBookViews : public WritingElementWithChilds<CWorkbookView>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CBookViews)
			CBookViews()
			{
			}
			virtual ~CBookViews()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<bookViews>"));
				for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</bookViews>"));
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

					if ( _T("workbookView") == sName )
						m_arrItems.Add( new CWorkbookView( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_BookViews;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_BOOKVIEWS_FILE_INCLUDE_H_