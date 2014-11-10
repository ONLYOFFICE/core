#include "AlternateContent.h"
#include "Run.h"

namespace OOX
{
	namespace Logic
	{
		void CAlternateContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			m_sXml.Init();
			m_sXml->Append(oReader.GetOuterXml());
			CString sXml;
			sXml.Format(_T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">%s</root>"), m_sXml.get());
			XmlUtils::CXmlLiteReader oSubReader;
			oSubReader.FromString(sXml);
			oSubReader.ReadNextNode();//root
			oSubReader.ReadNextNode();//AlternateContent

			int nParentDepth = oSubReader.GetDepth();
			while( oSubReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oSubReader.GetName();
				if ( _T("mc:Choice") == sName )
				{
					CRun altRun(oSubReader);
					
					for (unsigned int i = 0; i < altRun.m_arrItems.size(); ++i)
						m_arrChoiceItems.push_back(altRun.m_arrItems[i]);

					for (unsigned int i = 0; i < altRun.m_arrSpreadsheetItems.size(); ++i)
						m_arrSpreadsheetChoiceItems.push_back(altRun.m_arrSpreadsheetItems[i]);
					
					//без RemoveAll они очистятся вместе с altRun
					altRun.m_arrItems.clear();
					altRun.m_arrSpreadsheetItems.clear();
				}
				else if ( _T("mc:Fallback") == sName )
				{
					CRun altRun(oSubReader);
					
					for (unsigned int i = 0; i < altRun.m_arrItems.size(); ++i)
						m_arrFallbackItems.push_back(altRun.m_arrItems[i]);

					for (unsigned int i = 0; i < altRun.m_arrSpreadsheetItems.size(); ++i)
						m_arrSpreadsheetFallbackItems.push_back(altRun.m_arrSpreadsheetItems[i]);
					
					//без RemoveAll они очистятся вместе с altRun
					altRun.m_arrItems.clear();
					altRun.m_arrSpreadsheetItems.clear();
				}
			}
		}
	} // namespace Words
} // namespace OOX