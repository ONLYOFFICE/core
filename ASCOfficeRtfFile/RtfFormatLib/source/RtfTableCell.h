#pragma once 

#include "Basic.h"
#include "RtfProperty.h"

class RtfTableCell :  public ITextItemContainer
{
public: 
	RtfCellProperty m_oProperty;
	
	int GetType( )
	{
		return TYPE_RTF_TABLE_CELL;
	}

	RtfTableCell()
	{
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString result;

		for(int i =0 ; i < (int)m_aArray.size();i++)
		{
			if( m_aArray[i]->GetType() == TYPE_RTF_PARAGRAPH )
			{
				result.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
				if( i != m_aArray.size() - 1 )
					result.Append( _T("\\par") );
			}
			else
			{
				RenderParameter oNewParameter = oRenderParameter;
				oNewParameter.nType = RENDER_TO_RTF_PARAM_NESTED;
				result.Append( m_aArray[i]->RenderToRtf( oNewParameter ) );
			}


		}
		if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
			result.Append( _T("\\cell"));
		else
			result.Append( _T("\\nestcell{\\nonesttables  }")); //todo как бы вернуть
		return result;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult.Append(_T("<w:tc>"));

		CString sProp = m_oProperty.RenderToOOX( oRenderParameter );
		if( _T("") != sProp )
		{
			sResult.Append(_T("<w:tcPr>"));
			sResult.Append(sProp);
			sResult.Append(_T("</w:tcPr>"));
		}

		for( int i = 0; i < (int)m_aArray.size(); i++ )
		{
			sResult.Append( m_aArray[i]->RenderToOOX( oRenderParameter) );
		}
		sResult.Append(_T("</w:tc>"));
		return sResult;
	}
};

typedef boost::shared_ptr<RtfTableCell> RtfTableCellPtr;