#pragma once
#include "Basic.h"
#include "RtfChar.h"
#include "RtfWriter.h"


class RtfParagraph;
typedef boost::shared_ptr<RtfParagraph> RtfParagraphPtr;

class RtfOldList : IRenderableProperty
{
public: typedef enum{ lt_none, lt_blt, lt_level, lt_body, lt_cont }LevelType;
public: typedef enum{ lj_none, lj_left, lj_center, lj_right }LevelJust;

public: LevelType m_eLevelType;
public: int m_nLevelType;
public: LevelJust m_eLevelJust;
public: RtfParagraphPtr m_oLevelText;//фомат текста списка

public: int m_nLs;
public: int m_nIlvl;

public: RtfParagraphPtr m_oText;//замещающий текст
public: RtfOldList()
		{
			SetDefault();
		}
public: bool CanConvertToNumbering();
public: void SetDefault();
public: CString RenderToRtf(RenderParameter oRenderParameter);
public: CString RenderToOOX(RenderParameter oRenderParameter);
public: bool operator==( const RtfOldList& oOldList );
};
typedef boost::shared_ptr<RtfOldList> RtfOldListPtr;

class RtfParagraph : public ITextItem, public ItemContainer< IDocumentElementPtr >
{
	public: RtfParagraphProperty m_oProperty;
	public: RtfOldListPtr m_oOldList;
	public: int GetType( )
			{
				return TYPE_RTF_PARAGRAPH;
			}
	public: RtfParagraph()
			{
			}
public: int AddItem( IDocumentElementPtr piRend )
		{
			if( TYPE_RTF_CHAR == piRend->GetType() )
			{
				if( m_aArray.GetCount() > 0 && TYPE_RTF_CHAR == m_aArray[ m_aArray.GetCount() - 1 ]->GetType() )//соединяем два текста с одинаковыми свойствами
				{
					RtfCharPtr oCurChar = boost::shared_static_cast<RtfChar,IDocumentElement>( piRend );
					RtfCharPtr oPrevChar = boost::shared_static_cast<RtfChar,IDocumentElement>( m_aArray[ m_aArray.GetCount() - 1 ] );
					if( oCurChar->m_oProperty == oPrevChar->m_oProperty )
					{
						oPrevChar->AddText( oCurChar->GetText() );
						return (int)m_aArray.GetCount() - 1;
					}
				}				
			}
			ItemContainer< IDocumentElementPtr >::AddItem( piRend );
			return (int)m_aArray.GetCount() - 1;
		}
	public: CString RenderToRtf(RenderParameter oRenderParameter)
			{
				CString sResult ;
				if( RENDER_TO_RTF_PARAM_CHAR == oRenderParameter.nType )
				{
					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
						sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
				}
				else
				{
					sResult.Append(_T("\\pard\\plain"));
					sResult.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
					if( NULL != m_oOldList )
						sResult.Append( m_oOldList->RenderToRtf( oRenderParameter ) );
					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
						sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
					sResult.Append( m_oProperty.m_oCharProperty.RenderToRtf( oRenderParameter ) );

					//if( RENDER_TO_RTF_PARAM_NO_PAR != oRenderParameter.nValue )
					//	sResult.Append(_T("\\par"));
				}
				return sResult;
			}
	public: CString RenderToOOX(RenderParameter oRenderParameter)
			{
				CString sResult ;
				if( RENDER_TO_OOX_PARAM_PLAIN == oRenderParameter.nType )
				{
					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
						sResult.Append( m_aArray[i]->RenderToOOX(oRenderParameter) );
				}
				else if( RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType )
				{
					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
						sResult.Append( m_aArray[i]->RenderToOOX(oRenderParameter) );
				}
				else
				{
					bool bCanConvertToNumbering = false;
					if( NULL != m_oOldList )
						bCanConvertToNumbering = m_oOldList->CanConvertToNumbering();

					sResult.Append( _T("<w:p>") );
					sResult.Append( _T("<w:pPr>") );
					sResult.Append( m_oProperty.RenderToOOX(oRenderParameter) );

					if( NULL != m_oOldList )
					{
						//для OldList
						if( true == bCanConvertToNumbering )
							sResult.Append(  m_oOldList->RenderToOOX( oRenderParameter ) );
					}
					sResult.Append( _T("</w:pPr>") );

					if( NULL != m_oOldList )
					{
						//для OldList
						if( false == bCanConvertToNumbering && NULL != m_oOldList->m_oText)
						{
							RtfCharProperty oCharProp = m_oProperty.m_oCharProperty;
							if( NULL != m_oOldList->m_oText )
								oCharProp.m_nFont = m_oOldList->m_oText->m_oProperty.m_oCharProperty.m_nFont;

							RenderParameter oNewParam = oRenderParameter;
							oNewParam.nType = RENDER_TO_OOX_PARAM_TEXT;

							for( int i = 0; i < m_oOldList->m_oText->GetCount(); i++ )
							{
								sResult.Append( _T("<w:r>") );
								sResult.Append( _T("<w:rPr>") );
								sResult.Append( oCharProp.RenderToOOX(oRenderParameter) );
								sResult.Append( _T("</w:rPr>") );

								sResult.Append( m_oOldList->m_oText->m_aArray[ i ]->RenderToOOX(oNewParam) );
								sResult.Append( _T("</w:r>") );
							}
						}
					}

					RenderParameter oNewParam = oRenderParameter;
					oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
					for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
						sResult.Append( m_aArray[i]->RenderToOOX(oNewParam) );
					sResult.Append( _T("</w:p>") );
				}
				return sResult;
			}
};

