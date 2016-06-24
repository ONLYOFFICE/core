/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once 
#include "RtfParagraph.h"
#include "Writer/OOXRelsWriter.h"

class OOXFieldBegin : public IDocumentElement
{
public: 
	int m_bDirty;
	int m_bLock;
	RtfCharProperty m_oCharProp;
	OOXFieldBegin()
	{
		SetDefault();
	}
	int GetType()
	{
		return TYPE_OOX_FIELD;
	}
	void SetDefault()
	{
		m_bDirty = PROP_DEF;
		m_bLock = PROP_DEF;
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult.Append(_T("{\\field "));
		RENDER_RTF_BOOL( m_bDirty, sResult, _T("flddirty") )
		RENDER_RTF_BOOL( m_bLock, sResult, _T("fldlock") )
		sResult.Append(_T("{\\*\\fldinst"));
		sResult.Append( m_oCharProp.RenderToRtf(oRenderParameter) );
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
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
            sResult.AppendFormat( _T("{\\*\\datafield %ls}"), m_sData.GetBuffer());
		sResult.Append(_T("}"));

        CString str = m_oResult->RenderToRtf( oRenderParameter ) ;
        sResult.AppendFormat(_T("{\\fldrslt %ls}"),  str.GetBuffer());
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
			//поверяем на наличие гиперссылки
			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
			CString sInsertText = m_oInsert->RenderToOOX( oNewParam );
			int nIndex = sInsertText.Find( _T("HYPERLINK") );
			if( -1 != nIndex )
			{
				CString sHyperlink = sInsertText;
                sHyperlink.Delete( nIndex, 9/*(int)_tcslen( _T("HYPERLINK") )*/ );

				int nSplash = sHyperlink.Find( _T("\\") );
				if (nSplash > 0)
				{
					sHyperlink = sHyperlink.Left(nSplash);
				}
		
				//оставляем только одну ссылку
				sHyperlink.Remove( '\"' );
				sHyperlink.Trim();
				//заменяем пробелы на %20
				sHyperlink.Replace( _T(" "), _T("%20") );

				//добавляем в rels
				OOXRelsWriter* poRelsWriter = static_cast<OOXRelsWriter*>( oRenderParameter.poRels );
				CString sId = poRelsWriter->AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink"), Utils::PrepareToXML( sHyperlink ), false );
				//добавляем гиперссылку в документ
                sResult.AppendFormat( _T("<w:hyperlink r:id=\"%ls\" >"), sId.GetBuffer() );
				oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
				sResult.Append(m_oResult->RenderToOOX(oNewParam));
				sResult.Append( _T("</w:hyperlink>") );
			}
			else
			{
				nIndex = sInsertText.Find( _T("PRIVATE") );
				if( m_oResult->GetCount() <= 1 && nIndex < 0)
				{
					RenderParameter oNewParametr = oRenderParameter;
					oNewParametr.nType = RENDER_TO_OOX_PARAM_PLAIN;
					//sResult.Append(_T("<w:r>"));

                    CString str = Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ).Trim();
                    sResult.AppendFormat(_T("<w:fldSimple w:instr=\"%ls\">"), str.GetBuffer() );
					RenderParameter oNewParam = oRenderParameter;
					oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
					sResult.Append(m_oResult->RenderToOOX(oNewParam));
					sResult.AppendFormat(_T("</w:fldSimple>"));
					//sResult.Append(_T("</w:r>"));
				}
				else
				{
					//так добавляются лишние параграфы
					RenderParameter oNewParametr = oRenderParameter;
					oNewParametr.nType = RENDER_TO_OOX_PARAM_PLAIN;
					sResult.Append(_T("<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>"));

                    CString str = Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) );

                    sResult.AppendFormat(_T("<w:r><w:instrText xml:space=\"preserve\">%ls</w:instrText></w:r>"), str.GetBuffer());
					sResult.Append(_T("<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>"));
					//заканчиваем этот параграф
					sResult.Append(_T("</w:p>"));
					//пишем параграфы содержания
					oNewParametr.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
					sResult.Append(m_oResult->RenderToOOX(oNewParametr));
					//заканчиваем Field
					sResult.Append(_T("<w:p>"));
					sResult.Append(_T("<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>"));

					////пишем параграфы содержания
					//RenderParameter oNewParametr = oRenderParameter;
					//oNewParametr.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
					//sResult.Append(m_oResult->RenderToOOX(oNewParametr));

					//CString sFieldBegin;
					//sFieldBegin.Append(_T("<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>"));
					//sFieldBegin.AppendFormat(_T("<w:r><w:instrText xml:space=\"preserve\">%ls</w:instrText></w:r>"), Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ));
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
					//sFieldEnd.AppendFormat(_T("<w:r><w:instrText xml:space=\"preserve\">%ls</w:instrText></w:r>"), Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ));
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
