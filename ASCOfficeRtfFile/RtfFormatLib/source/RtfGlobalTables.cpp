#include "RtfGlobalTables.h"
#include "RtfDocument.h"

CString RtfFontTable::RenderToRtf(RenderParameter oRenderParameter)
		{
			////записывает default charset и codepage
			//RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
			//if( PROP_DEF != poDocument->m_oProperty.m_nAnsiCodePage )
			//{
			//	int nCodepage = poDocument->m_oProperty.m_nAnsiCodePage;
			//	int nCharset = RtfUtility::CodepageToCharset( nCodepage );
			//	for( int i = 0; i < (int)m_aArray.size(); i++ )
			//		{
			//			m_aArray[i].m_nCharset = nCharset;
			//			m_aArray[i].m_nCodePage = nCodepage;
			//		}
			//}
			
			CString sResult;
			if( m_aArray.size() > 0 )
			{
				sResult.Append(_T("{\\fonttbl"));
				RenderParameter oNewParameter = oRenderParameter;
				oNewParameter.nType = RENDER_TO_RTF_PARAM_FONT_TBL;
				for( int i = 0; i < (int)m_aArray.size(); i++ )
					sResult.AppendFormat(_T("%ls"),m_aArray[i].RenderToRtf( oNewParameter ));
				sResult.Append(_T("}"));
			}
			return sResult;
		}
CString RtfListTable::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( m_aArray.size() > 0 )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_SHAPE_WSHAPE;
		for( int i = 0; i < (int)m_aPictureList.GetCount(); i++ )
		{
			sResult.AppendFormat(_T("<w:numPicBullet w:numPicBulletId=\"%d\">"), i );
			sResult.Append( m_aPictureList[i]->RenderToOOX(oNewParam) );
			sResult.Append(_T("</w:numPicBullet>") );
		}
		for( int i = 0; i < (int)m_aArray.size(); i++)
			sResult.Append( m_aArray[i].RenderToOOX(oRenderParameter) );
	}
	else
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_OLDLIST_ABS;
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		for( int i = 0; i < (int)poDocument->m_aOldLists.size(); i++ )
			sResult.Append( poDocument->m_aOldLists[i]->RenderToOOX( oNewParam ) );
	}
	return sResult;
}
CString RtfListOverrideTable::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( m_aArray.size() > 0 )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		for( int i = 0; i < (int)m_aArray.size(); i++)
			sResult.Append( m_aArray[i].RenderToOOX(oNewParam) );
	}
	else
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_OLDLIST_OVR;
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		for( int i = 0; i < (int)poDocument->m_aOldLists.size(); i++ )
			sResult.Append( poDocument->m_aOldLists[i]->RenderToOOX( oNewParam ) );
	}
	return sResult;
}