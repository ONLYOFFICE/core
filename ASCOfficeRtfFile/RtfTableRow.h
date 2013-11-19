#pragma once 
#include "Basic.h"
#include "XmlUtils.h"
#include "RtfTableCell.h"

class RtfTableRow : public IDocumentElement, public ItemContainer< RtfTableCellPtr >
{
public: RtfRowProperty m_oProperty;
	public: int GetType()
			{
				return TYPE_RTF_TABLE_ROW;
			}
	public: RtfTableRow()
			{
			}
public: CString RenderToRtf(RenderParameter oRenderParameter)
			{
				CString sResult;
				sResult.Append(_T("\n"));
				if( RENDER_TO_RTF_PARAM_NESTED == oRenderParameter.nType )
				{

					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
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
					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
					{
						sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
					}
					sResult.Append(_T("\\row"));
				}
				return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
			{
				XmlUtils::CXmlWriter oXmlWriter;
				oXmlWriter.WriteNodeBegin(_T("w:tr"),0);
				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

				oXmlWriter.WriteString( m_oProperty.RenderToOOX(oNewParam) );
				for(int i = 0 ; i < (int)m_aArray.GetCount(); i++)
				{
					oXmlWriter.WriteString( m_aArray[i]->RenderToOOX(oNewParam) );
				}
				oXmlWriter.WriteNodeEnd(_T("w:tr"));
				return oXmlWriter.GetXmlString();
			}

};

typedef boost::shared_ptr<RtfTableRow> RtfTableRowPtr;