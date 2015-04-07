#pragma once 
#include "Basic.h"

#include "RtfTableCell.h"

class RtfTableRow : public IDocumentElement, public ItemContainer< RtfTableCellPtr >
{
public: 
	RtfRowProperty m_oProperty;
	int GetType()
	{
		return TYPE_RTF_TABLE_ROW;
	}
	RtfTableRow()
	{
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult.Append(_T("\n"));
		if( RENDER_TO_RTF_PARAM_NESTED == oRenderParameter.nType )
		{

			for( int i = 0; i < (int)m_aArray.size(); i++ )
			{
				sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
			}
			sResult.Append(_T("{\\*\\nesttableprops"));
			sResult.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
			sResult.Append(_T("\\nestrow}{\\nonesttables \\par}"));
		}
		else
		{
			sResult.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
			for( int i = 0; i < (int)m_aArray.size(); i++ )
			{
				sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
			}
			sResult.Append(_T("\\row"));
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		XmlUtils::CXmlWriter oXmlWriter;
		oXmlWriter.WriteNodeBegin(_T("w:tr"),0);
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		CString sRowProp = m_oProperty.RenderToOOX(oNewParam);
		if( false == sRowProp.IsEmpty() )
		{
			CString sXml;
			sXml.Format(_T("<w:trPr>%ls</w:trPr>"), sRowProp);
			oXmlWriter.WriteString(sXml);
		}
		for(int i = 0 ; i < (int)m_aArray.size(); i++)
		{
			oXmlWriter.WriteString( m_aArray[i]->RenderToOOX(oNewParam) );
		}
		oXmlWriter.WriteNodeEnd(_T("w:tr"));
		return oXmlWriter.GetXmlString();
	}

};


typedef boost::shared_ptr<RtfTableRow> RtfTableRowPtr;