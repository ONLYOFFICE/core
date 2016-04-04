#include "DrawingExt.h"
#include "../../XlsxFormat/Worksheets/Sparkline.h"

namespace OOX
{
	namespace Drawing
	{
        void COfficeArtExtension::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ((m_oUri.IsInit()) && (*m_oUri == _T("{63B3BB69-23CF-44E3-9099-C40C66FF867C}") || *m_oUri == _T("{05C60535-1F16-4fd2-B633-F4F36F0B64E0}")))//2.2.6.2 Legacy Object Wrapper
			{
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (sName == _T("compatExt"))//2.3.1.2 compatExt
					{	//attributes spid -https://msdn.microsoft.com/en-us/library/hh657207(v=office.12).aspx
						m_oCompatExt = oReader;
					}
					else if (sName == _T("sparklineGroups"))
					{
						m_oSparklineGroups = oReader;
					}
				}
			}
			else
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		}
        CString COfficeArtExtension::toXML() const
        {
            return toXMLWithNS(_T("a:"));
        }
        CString COfficeArtExtension::toXMLWithNS(const CString& sNamespace) const
		{
            CString sResult = _T("<");
            sResult += sNamespace;
            sResult += _T("ext");
            sResult += m_sAdditionalNamespace;

			if ( m_oUri.IsInit() )
			{
                sResult += _T(" uri=\"");
				sResult += m_oUri->GetString();
				sResult += _T("\">");
			}
			else
				sResult += _T(">");

			if(m_oCompatExt.IsInit())
			{
				sResult += m_oCompatExt->toXML();
			}
			if(m_oSparklineGroups.IsInit())
			{
                XmlUtils::CStringWriter writer;
				m_oSparklineGroups->toXML(writer);
                sResult += writer.GetData();
			}

            sResult += _T("</");
            sResult += sNamespace;
            sResult += _T("ext>");

			return sResult;
		}
	}
}
