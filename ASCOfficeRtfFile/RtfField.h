#pragma once 
#include "RtfParagraph.h"
#include "OOXRelsWriter.h"

class OOXFieldBegin : public IDocumentElement
{
public: int m_bDirty;
public: int m_bLock;
public: RtfCharProperty m_oCharProp;
public: OOXFieldBegin()
		{
			SetDefault();
		}
public: int GetType()
		{
			return TYPE_OOX_FIELD;
		}
public: void SetDefault()
		{
			m_bDirty = PROP_DEF;
			m_bLock = PROP_DEF;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append(_T("{\\field "));
			RENDER_RTF_BOOL( m_bDirty, sResult, _T("flddirty") )
			RENDER_RTF_BOOL( m_bLock, sResult, _T("fldlock") )
			sResult.Append(_T("{\\*\\fldinst"));
			sResult.Append( m_oCharProp.RenderToRtf(oRenderParameter) );
			return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append( _T("<w:r>") );
			sResult.Append( _T("<w:fldChar w:fldCharType=\"separate\"") );
			RENDER_OOX_INT_ATTRIBUTE( m_bDirty, sResult, _T("dirty") )
			RENDER_OOX_INT_ATTRIBUTE( m_bLock, sResult, _T("fldLock") )
			sResult.Append( _T("/>") );
			sResult.Append( _T("</w:r>") );
			return sResult;
		}
};
class OOXFieldInsertText : public IDocumentElement
{
public: RtfCharPtr m_oText;
public: int GetType()
		{
			return TYPE_OOX_FIELD;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			if( NULL != m_oText )
				return m_oText->RenderToRtf( oRenderParameter );
			else
				return _T("");
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			if( NULL != m_oText )
			{
				CString sResult;
				sResult.Append( _T("<w:r>") );
				sResult.Append( _T("<w:instrText>") );
				oRenderParameter.nType = RENDER_TO_RTF_PARAM_CHAR;
				oRenderParameter.nValue = RENDER_TO_RTF_PARAM_NO_PAR;
				sResult.Append( m_oText->RenderToOOX( oRenderParameter ) );
				sResult.Append( _T("</w:instrText>") );
				sResult.Append( _T("</w:r>") );
				return sResult;
			}
			else
				return _T("");
		}
};
class OOXFieldSeparate : public IDocumentElement
{
public: int GetType()
		{
			return TYPE_OOX_FIELD;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			return _T("}{\\fldrslt");
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			return _T("<w:fldChar w:fldCharType=\"separate\" />");
		}
};
class OOXFieldEnd : public IDocumentElement
{
public: int GetType()
		{
			return TYPE_OOX_FIELD;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			return _T("}}");
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			return _T("<w:fldChar w:fldCharType=\"end\" />");
		}
};


class RtfField : public IDocumentElement
{
public: typedef enum{
			fm_none,
			fm_flddirty,//\flddirty	A formatting change has been made to the field result since the field was last updated.
			fm_fldedit,	//\fldedit	Text has been added to, or removed from, the field result since the field was last updated.
			fm_fldlock,	//\fldlock	Field is locked and cannot be updated.
			fm_fldpriv	//\fldpriv	Result is not in a form suitable for display (for example, binary data used by fields whose result is a picture).
		}FieldMode;

public: FieldMode m_eMode;
public: TextItemContainerPtr m_oInsert;
public: bool m_bReferenceToEndnote;
public: CString m_sData;
public: TextItemContainerPtr m_oResult;
public: bool m_bTextOnly;
public: RtfField()
		{
			SetDefault();
		}

public: int GetType()
		{
			return TYPE_RTF_FIELD;
		}

public: bool IsValid()
		{
			return true;
			//return false == m_oInsert.IsValid() && false == m_oResult.IsValid(); 
		}
public: void SetDefaultRtf()
		{
			SetDefault();
		}
public: void SetDefaultOOX()
		{
			SetDefault();
		}
public: void SetDefault()
		{
			m_eMode = fm_none;
			m_bReferenceToEndnote = false;
			m_sData = _T("");
			m_oInsert = TextItemContainerPtr( new TextItemContainer() );
			m_oResult = TextItemContainerPtr( new TextItemContainer() );
			m_bTextOnly = false;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append(_T("{\\field "));
			if( fm_none != m_eMode )
			{
				switch( m_eMode )
				{
					case fm_flddirty: sResult.Append(_T("{\\flddirty "));break;
					case fm_fldedit: sResult.Append(_T("{\\fldedit "));break;
					case fm_fldlock: sResult.Append(_T("{\\fldlock "));break;
					case fm_fldpriv: sResult.Append(_T("{\\fldpriv "));break;
				}
			}
			sResult.Append(_T("{\\*\\fldinst "));
			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_RTF_PARAM_CHAR;
			oNewParam.nValue = RENDER_TO_RTF_PARAM_NO_PAR;
			sResult.Append( m_oInsert->RenderToRtf( oNewParam ) );
			if( true == m_bReferenceToEndnote )
				sResult.Append( _T("\\fldalt") );
			if( false == m_sData.IsEmpty() )
				sResult.AppendFormat( _T("{\\*\\datafield %s}"), m_sData);
			sResult.Append(_T("}"));

			sResult.AppendFormat(_T("{\\fldrslt %s}"),  m_oResult->RenderToRtf( oRenderParameter ) );
			sResult.Append(_T("}"));
			return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( true == m_bTextOnly )
			{
				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
				sResult.Append(m_oResult->RenderToOOX(oNewParam));
			}
			else
			{
				//повер€ем на наличие гиперссылки
				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
				CString sInsertText = m_oInsert->RenderToOOX( oNewParam );
				int nIndex = sInsertText.Find( _T("HYPERLINK") );
				if( -1 != nIndex )
				{
					//оставл€ем только одну ссылку
					CString sHyperlink = sInsertText;
					sHyperlink.Delete( nIndex, (int)_tcslen( _T("HYPERLINK") ) );
					sHyperlink.Remove( '\"' );
					sHyperlink.Trim();
					//замен€ем пробелы на %20
					sHyperlink.Replace( _T(" "), _T("%20") );

					//добавл€ем в rels
					OOXRelsWriter* poRelsWriter = static_cast<OOXRelsWriter*>( oRenderParameter.poRels );
					CString sId = poRelsWriter->AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink"), Utils::PrepareToXML( sHyperlink ), false );
					//добавл€ем гиперссылку в документ
					sResult.AppendFormat( _T("<w:hyperlink r:id=\"%s\" >"), sId );
					oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
					sResult.Append(m_oResult->RenderToOOX(oNewParam));
					sResult.Append( _T("</w:hyperlink>") );
				}
				else
				{
					if( m_oResult->GetCount() <= 1 )
					{
						RenderParameter oNewParametr = oRenderParameter;
						oNewParametr.nType = RENDER_TO_OOX_PARAM_PLAIN;
						//sResult.Append(_T("<w:r>"));
						sResult.AppendFormat(_T("<w:fldSimple w:instr=\"%s\">"), Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ).Trim() );
						RenderParameter oNewParam = oRenderParameter;
						oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
						sResult.Append(m_oResult->RenderToOOX(oNewParam));
						sResult.AppendFormat(_T("</w:fldSimple>"));
						//sResult.Append(_T("</w:r>"));
					}
					else
					{
						//так добавл€ютс€ лишние параграфы
						RenderParameter oNewParametr = oRenderParameter;
						oNewParametr.nType = RENDER_TO_OOX_PARAM_PLAIN;
						sResult.Append(_T("<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>"));
						sResult.AppendFormat(_T("<w:r><w:instrText xml:space=\"preserve\">%s</w:instrText></w:r>"), Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ));
						sResult.Append(_T("<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>"));
						//заканчиваем этот параграф
						sResult.Append(_T("</w:p>"));
						//пишем параграфы содержани€
						oNewParametr.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
						sResult.Append(m_oResult->RenderToOOX(oNewParametr));
						//заканчиваем Field
						sResult.Append(_T("<w:p>"));
						sResult.Append(_T("<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>"));

						////пишем параграфы содержани€
						//RenderParameter oNewParametr = oRenderParameter;
						//oNewParametr.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
						//sResult.Append(m_oResult->RenderToOOX(oNewParametr));

						//CString sFieldBegin;
						//sFieldBegin.Append(_T("<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>"));
						//sFieldBegin.AppendFormat(_T("<w:r><w:instrText xml:space=\"preserve\">%s</w:instrText></w:r>"), Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ));
						//sFieldBegin.Append(_T("<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>"));
						////пишем после первого w:pPr
						//CString sFindStr = _T("</w:pPr>");
						//int nIndex = sResult.Find( sFindStr );
						//if( nIndex >= 0 && nIndex < sResult.GetLength() )
						//	sResult.Inset( sFieldBegin, nIndex + sFindStr.GetLength() );
						//else//пишем после первого w:p
						//{
						//	sFindStr = _T("<w:p>");
						//	nIndex = sResult.Find( sFindStr );
						//	if( nIndex >= 0 && nIndex < sResult.GetLength() )
						//		sResult.Inset( sFieldBegin, nIndex + sFindStr.GetLength() );
						//}
						//CString sFieldEnd;
						//sFieldEnd.Append(_T("<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>"));
						//sFieldEnd.AppendFormat(_T("<w:r><w:instrText xml:space=\"preserve\">%s</w:instrText></w:r>"), Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ));
						//sFieldEnd.Append(_T("<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>"));
						////пишем после последнего w:pPr
					}
				}
			}
			return sResult;
		}
};
typedef boost::shared_ptr<RtfField> RtfFieldPtr;
typedef boost::shared_ptr<OOXFieldBegin> OOXFieldBeginPtr;
typedef boost::shared_ptr<OOXFieldInsertText> OOXFieldInsertTextPtr;
typedef boost::shared_ptr<OOXFieldSeparate> OOXFieldSeparatePtr;
typedef boost::shared_ptr<OOXFieldEnd> OOXFieldEndPtr;