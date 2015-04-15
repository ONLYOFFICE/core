#pragma once
#include "RtfProperty.h"
#include "RtfShape.h"

class RtfFontTable : public IDocumentElement, public ItemSingleContainer<RtfFont>
{
public:
	int DirectAddItem( RtfFont piRend)
	{
		m_aArray.push_back(piRend);
		return (int)m_aArray.size() - 1;
	}
	bool GetFont( int nId, RtfFont& oFont)
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( nId == m_aArray[i].m_nID )
			 {
				oFont =  m_aArray[i];
				return true;
				}
			 return false;
	 }
	bool GetFont( CString sName, RtfFont& oFont )
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( sName == m_aArray[i].m_sName )
			 {
				oFont =  m_aArray[i];
				return true;
				}
			 return false;
	 }
	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		if( m_aArray.size() > 0 )
		{
			for( int i = 0; i < (int)m_aArray.size(); i++ )
				sResult.Append(m_aArray[i].RenderToOOX(oRenderParameter));

		}
		return sResult;
	}
};
class RtfColorTable : public IDocumentElement, public ItemSingleContainer<RtfColor>
{
public: 
	RtfColorTable()
	{
	}
	int DirectAddItem( RtfColor piRend)
	{
		m_aArray.push_back(piRend);
		return (int)m_aArray.size() - 1;
	}
	int AddItem( RtfColor piRend)
	{
		int nIndex = ItemSingleContainer<RtfColor>::AddItem( piRend );
		return nIndex + 1;
	}
	bool GetColor( int nId, RtfColor& oColor)
	{
		 //0 - ID автоцвет
		nId--;
		 if( nId >= 0 && nId < (int)m_aArray.size() )
		 {
			 oColor = m_aArray[nId];
			 return true;
		 }
		 return false;
	}
	bool GetColor( RtfColor::ThemeColor oTheme, RtfColor& oColor)
	{
		for( int i = 0; i < (int)m_aArray.size(); i++ )
		if( oTheme == m_aArray[i].m_eTheme )
		{
			oColor = m_aArray[i];
			return true;
		}
		return false;
	}
	bool GetColor( CString sTheme, RtfColor& oColor)
	{
		 RtfColor::ThemeColor oTheme = RtfColor::TC_NONE;
		
		 if( true == RtfColor::GetThemeByString(sTheme,oTheme  ) )
		 {
			 for( int i = 0; i < (int)m_aArray.size(); i++ )
				 if( oTheme == m_aArray[i].m_eTheme )
				 {
				oColor = m_aArray[i];
					return true;
					}
		 }
		 return false;
	}
	bool GetColor( RtfColor oColor , int & nId)
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( m_aArray[i] == oColor )
			 {
				nId = i + 1;
				return true;
			 }
		 return false;
	 }
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		if( m_aArray.size() > 0 )
		{
			sResult.Append(_T("{\\colortbl;"));
			RenderParameter oNewParameter = oRenderParameter;
			oNewParameter.nType = RENDER_TO_RTF_PARAM_COLOR_TBL;
			for( int i = 0; i < (int)m_aArray.size(); i++ )
            {
                CString str = m_aArray[i].RenderToRtf( oNewParameter );
                sResult.AppendFormat(_T("%ls"), str.GetBuffer());
            }
			sResult.Append(_T("}"));
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		return _T("");
	}
};
class RtfStyleTable : public IDocumentElement, public ItemContainer<RtfStylePtr>
{
public: 
	bool GetStyle( int nId, RtfStylePtr& oStyle)
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( nId == m_aArray[i]->m_nID )
			 {
				oStyle =  m_aArray[i];
				return true;
				}
			 return false;
	 }
	bool GetStyle( CString sName, RtfStylePtr& oStyle )
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( sName == m_aArray[i]->m_sID )
			 {
				oStyle =  m_aArray[i];
				return true;
				}
			 return false;
	 }
	RtfStylePtr GetStyleResulting( RtfStylePtr oInputStyle )
	 {
		RtfStylePtr oResultStyle;
		RtfStyle::StyleType eStyleType = RtfStyle::st_none;
		int nStyleId = oInputStyle->m_nID;
		int nLinked = PROP_DEF;
		int nBaseOn = oInputStyle->m_nBasedOn;
		 if( RtfStyle::stCharacter == oInputStyle->m_eType )
		 {
			eStyleType = RtfStyle::stCharacter;
			oResultStyle = RtfCharStylePtr( new RtfCharStyle() );
		 }
		 else if( RtfStyle::stParagraph == oInputStyle->m_eType )
		 {
			eStyleType = RtfStyle::stParagraph;
			oResultStyle = RtfParagraphStylePtr( new RtfParagraphStyle() );
			nLinked = oInputStyle->m_nLink;//linked будем смотреть только у стилей параграфа, чтобы избежать рекурсии
		 }
		 else if( RtfStyle::stTable == oInputStyle->m_eType )
		 {
			eStyleType = RtfStyle::stTable;
			oResultStyle = RtfTableStylePtr( new RtfTableStyle() );
		 }
		 else
			 return oInputStyle;


		 RtfStylePtr oLinkedStyle;
		 //if( PROP_DEF != nLinked && nStyleId != nLinked)
		 //{
			// RtfStylePtr oTemStyle;
			// if( true == GetStyle( nLinked, oTemStyle) )
			//	oLinkedStyle = GetStyleResulting( oTemStyle );
		 //}
		 RtfStylePtr oBaseStyle;
		 if( PROP_DEF != nBaseOn && nStyleId != nBaseOn)
		 {
			 RtfStylePtr oTemStyle;
			 if( true == GetStyle( nBaseOn, oTemStyle) )
				oBaseStyle = GetStyleResulting( oTemStyle );
		 }
		 if( NULL == oLinkedStyle && NULL == oBaseStyle )
			 return oInputStyle;
		 else
		 {
			 //Опытным путем установлено - Base старше Link
			 if( NULL != oLinkedStyle )
			 {
				oResultStyle->Merge( oLinkedStyle );
			 }
			 if( NULL != oBaseStyle )
			 {
				oResultStyle->Merge( oBaseStyle );
			 }
			oResultStyle->Merge( oInputStyle );
		 }
		return oResultStyle;
	 }
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		if( m_aArray.size() > 0 )
		{
			sResult.Append(_T("{\\stylesheet"));
			for( int i = 0; i < (int)m_aArray.size(); i++ )
            {
                CString str = m_aArray[i]->RenderToRtf( oRenderParameter );
                sResult.AppendFormat(_T("%ls\n\n"), str.GetBuffer());
            }
			sResult.Append(_T("}"));
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			sResult.Append(m_aArray[i]->RenderToOOX(oRenderParameter));
		return sResult;
	}
};
//class RtfLatentStyleTable : public IDocumentElement, public ItemContainer<RtfStyleException>
//{
//public: int m_nCount;
//		int m_nLocked;
//		int m_nSemiHidden;
//		int m_nUnHiddenWhenUse;
//		int m_nQFormat;
//		int m_nPriority;
//		RtfLatentStyleTable()
//		{
//            m_nCount = PROP_DEF;
//            m_nLocked = PROP_DEF;
//            m_nSemiHidden = PROP_DEF;
//            m_nUnHiddenWhenUse = PROP_DEF;
//            m_nQFormat = PROP_DEF;
//            m_nPriority = PROP_DEF;
//		}
//		bool GetException( CString sName, RtfStyleException& oListProperty )
//		 {
//			 for( int i = 0; i < (int)m_aArray.size(); i++ )
//				 if( sName == m_aArray[i].m_sName )
//				 {
//					oListProperty =  m_aArray[i];
//					return true;
//				 }
//			 return false;
//		 }
//		CString RenderToRtf(RenderParameter oRenderParameter)
//		{
//			CString sResult;
//			if( m_aArray.size() > 0 )
//			{
//				sResult.Append(_T("{\\*\\latentstyles"));
//				if( PROP_DEF != m_nCount )
//					sResult.AppendFormat(_T("\\lsdstimax%d"),m_nCount);
//				if( PROP_DEF != m_nLocked )
//					sResult.AppendFormat(_T("\\lsdlockeddef%d"),m_nLocked);
//				if( PROP_DEF != m_nSemiHidden )
//					sResult.AppendFormat(_T("\\lsdsemihiddendef%d"),m_nSemiHidden);
//				if( PROP_DEF != m_nUnHiddenWhenUse )
//					sResult.AppendFormat(_T("\\lsdunhideuseddef%d"),m_nUnHiddenWhenUse);
//				if( PROP_DEF != m_nQFormat )
//					sResult.AppendFormat(_T("\\lsdqformatdef%d"),m_nQFormat);
//				if( PROP_DEF != m_nPriority )
//					sResult.AppendFormat(_T("\\lsdprioritydef%d"),m_nPriority);
//				sResult.Append(_T("{\\lsdlockedexcept "));
//				for( int i = 0; i < (int)m_aArray.size(); i++ )
//					sResult.Append(m_aArray[i].RenderToRtf( oRenderParameter ));
//				sResult.Append(_T("}"));
//				sResult.Append(_T("}"));
//			}
//			return sResult;
//		}
//		CString RenderToOOX(RenderParameter oRenderParameter)
//		{
//			XmlUtils::CXmlWriter oXmlWriter;
//			if( m_aArray.size() > 0 )
//			{
//				oXmlWriter.WriteNodeBegin(_T("w:latentStyles"),1);
//
//				if( PROP_DEF != m_nCount )
//					oXmlWriter.WriteAttribute(_T("w:count"),m_nCount);
//				if( PROP_DEF != m_nLocked )
//					oXmlWriter.WriteAttribute(_T("w:defLockedState"),m_nLocked);
//				if( PROP_DEF != m_nSemiHidden )
//					oXmlWriter.WriteAttribute(_T("w:defSemiHidden"),m_nSemiHidden);
//				if( PROP_DEF != m_nUnHiddenWhenUse )
//					oXmlWriter.WriteAttribute(_T("w:defUnhideWhenUsed"),m_nUnHiddenWhenUse);
//				if( PROP_DEF != m_nQFormat )
//					oXmlWriter.WriteAttribute(_T("w:defQFormat"),m_nQFormat);
//				if( PROP_DEF != m_nPriority )
//					oXmlWriter.WriteAttribute(_T("w:defUIPriority"),m_nPriority);
//				oXmlWriter.WriteNodeEnd(_T("w:latentStyles"),1,0);
//
//				for( int i = 0; i < (int)m_aArray.size(); i++ )
//					oXmlWriter.WriteString( m_aArray[i].RenderToOOX(oRenderParameter) );
//
//				oXmlWriter.WriteNodeEnd(_T("w:latentStyles"),0);
//			}
//			return oXmlWriter.GetXmlString();
//		}
//};
class RtfListTable : public IDocumentElement, public ItemContainer<RtfListProperty>
{
public:
	ItemContainer<RtfShapePtr> m_aPictureList;
	bool GetList( CString sName, RtfListProperty& oListProperty )
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( sName == m_aArray[i].m_sName )
			 {
				oListProperty =  m_aArray[i];
				return true;
			 }
		 return false;
	 }
	bool GetList( int nId, RtfListProperty& oListProperty )
	 {
		 for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( nId == m_aArray[i].m_nID )
			 {
				oListProperty =  m_aArray[i];
				return true;
			 }
		 return false;
	 }
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		if( m_aArray.size() > 0 )
		{
			sResult.Append(_T("{\\*\\listtable "));
			if( m_aPictureList.GetCount() > 0 )
			{
				sResult.Append(_T("{\\*\\listpicture") );
				for( int i = 0; i < (int)m_aPictureList.GetCount(); i++ )
                {
					sResult.Append( m_aPictureList[i]->RenderToRtf( oRenderParameter ) );
                }
				sResult.Append(_T("}") );
			}
			for( int i = 0; i < (int)m_aArray.size(); i++)
            {
                CString str = m_aArray[i].RenderToRtf( oRenderParameter );
                sResult.AppendFormat(_T("{%ls}"), str.GetBuffer() );
            }
			sResult.Append(_T("}"));
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter);
};
class RtfListOverrideTable : public IDocumentElement, public ItemContainer<RtfListOverrideProperty>
{
public: 
	bool GetList( int nId, RtfListOverrideProperty& oListOverrideProperty )
	{
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			 if( nId == m_aArray[i].m_nIndex )
			 {
				oListOverrideProperty =  m_aArray[i];
				return true;
			 }
		return false;
	}
		CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( m_aArray.size() > 0 )
			{
				sResult.Append(_T("{\\*\\listoverridetable"));
				for( int i = 0; i < (int)m_aArray.size(); i++)
                {
                    CString str = m_aArray[i].RenderToRtf( oRenderParameter );
                    sResult.AppendFormat(_T("{%ls}"), str.GetBuffer() );
                }
				sResult.Append(_T("}"));
			}
			return sResult;
		}
		CString RenderToOOX(RenderParameter oRenderParameter);
};
