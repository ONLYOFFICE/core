#pragma once
#ifndef OOX_CHARTTITLE_FILE_INCLUDE_H_
#define OOX_CHARTTITLE_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartText : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartText)
			CChartText() {}
			virtual ~CChartText() {}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<a:t"));
				if(-1 != m_sText.Find(' ') || -1 != m_sText.Find('\n'))
					writer.WriteStringC(_T(" xml:space=\"preserve\""));
				writer.WriteStringC(_T(">"));
				writer.WriteStringC(XmlUtils::EncodeXmlString(m_sText));
				writer.WriteStringC(_T("</a:t>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			CString ToString() const
			{
				return m_sText;
			}
			virtual EElementType getType() const
			{
				return et_a_Text;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
				{
					if ( _T("xml:space") == wsName )
					{
						m_oSpace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		public:

			// Attributes
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;

			// Value
			CString                             m_sText;

		};
		class CChartRun : public WritingElementWithChilds<CChartText>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartRun)
			CChartRun()
			{
			}
			virtual ~CChartRun()
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("a:t") == sName )
						m_arrItems.Add( new CChartText( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_a_Run;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CChartParagraph : public WritingElementWithChilds<CChartRun>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartParagraph)
			CChartParagraph()
			{
			}
			virtual ~CChartParagraph()
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("a:r") == sName )
						m_arrItems.Add( new CChartRun( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_a_Paragraph;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CChartRich : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartRich)
			CChartRich() {}
			virtual ~CChartRich() {}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				if(m_oXml.IsInit())
					writer.WriteStringC(m_oXml.get());
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;
				m_oXml.Init();
				m_oXml->AppendFormat(_T("<c:rich xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">%s</c:rich>"), oReader.GetInnerXml());
			}
			virtual EElementType getType() const
			{
				return et_c_Rich;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CString> m_oXml;
		};
		class CChartTx : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartTx)
			CChartTx() {}
			virtual ~CChartTx() {}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<c:tx>"));
				if(m_oRich.IsInit())
					m_oRich->toXML(writer);
				writer.WriteStringC(_T("</c:tx>"));
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

					if ( _T("c:rich") == sName )
						m_oRich = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_Tx;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:

			// Attributes
			nullable<CChartRich> m_oRich;
		};
		class CChartTitle : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartTitle)
			CChartTitle()
			{
			}
			virtual ~CChartTitle()
			{
			}
		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<c:title>"));
				if(m_oTx.IsInit())
					m_oTx->toXML(writer);
				writer.WriteStringC(_T("<c:layout/><c:overlay val=\"0\"/>"));
				if(m_oTxPr.IsInit())
					m_oTxPr->toXML(writer);
				writer.WriteStringC(_T("</c:title>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("c:tx") == sName )
						m_oTx = oReader;
					else if ( _T("c:txPr") == sName )
						m_oTxPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_c_Title;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CChartTx>						m_oTx;
			nullable<CChartRich>					m_oTxPr;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CHARTTITLE_FILE_INCLUDE_H_