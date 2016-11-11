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
#include "Writer/OOXWriter.h"
#include "Writer/OOXRelsWriter.h"

class OOXFieldBegin : public IDocumentElement
{
public: 
	int				m_bDirty;
	int				m_bLock;
	RtfCharProperty m_oCharProperty;

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
		m_bDirty	= PROP_DEF;
		m_bLock		= PROP_DEF;
		
		m_oCharProperty.SetDefault();
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult += L"{\\field ";
		RENDER_RTF_BOOL( m_bDirty, sResult, L"flddirty" )
		RENDER_RTF_BOOL( m_bLock, sResult, L"fldlock" )
		sResult += L"{\\*\\fldinst";
		
		sResult +=  m_oCharProperty.RenderToRtf(oRenderParameter);
		return sResult;
	}
	
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		
		sResult +=  L"<w:r>";
			CString props = m_oCharProperty.RenderToOOX(oRenderParameter);
			if (props.IsEmpty())
			{
				sResult +=  L"<w:rPr>";
					sResult += props;
				sResult +=  L"</w:rPr>";
			}
			
			sResult +=  L"<w:fldChar w:fldCharType=\"separate\"";
				RENDER_OOX_INT_ATTRIBUTE( m_bDirty, sResult, L"dirty" )
				RENDER_OOX_INT_ATTRIBUTE( m_bLock, sResult, L"fldLock" )
			sResult +=  L"/>";
		sResult +=  L"</w:r>";
		
		return sResult;
	}
};
class OOXFieldInsertText : public IDocumentElement
{
public: 
	RtfCharPtr m_oText;
	
	int GetType()
	{
		return TYPE_OOX_FIELD;
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		if( NULL != m_oText )
			return m_oText->RenderToRtf( oRenderParameter );
		else
			return L"";
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		if( NULL != m_oText )
		{
			CString sResult;
			sResult += L"<w:r>";
			sResult += L"<w:instrText>";
			
			oRenderParameter.nType	= RENDER_TO_RTF_PARAM_CHAR;
			oRenderParameter.nValue	= RENDER_TO_RTF_PARAM_NO_PAR;
			
			sResult += m_oText->RenderToOOX( oRenderParameter );
			
			sResult +=  L"</w:instrText>";
			sResult += L"</w:r>";
			return sResult;
		}
		else
			return L"";
	}
};

class OOXFieldSeparate : public IDocumentElement
{
public:
	int GetType()
	{
		return TYPE_OOX_FIELD;
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		return L"}{\\fldrslt";
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		return L"<w:fldChar w:fldCharType=\"separate\"/>";
	}
};

class OOXFieldEnd : public IDocumentElement
{
public: 
	int GetType()
	{
		return TYPE_OOX_FIELD;
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		return L"}}";
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		return L"<w:fldChar w:fldCharType=\"end\"/>";
	}
};


class RtfField : public IDocumentElement
{
public: 
	enum _FieldMode
	{
		fm_none,
		fm_flddirty,	// flddirty	A formatting change has been made to the field result since the field was last updated.
		fm_fldedit,		// fldedit	Text has been added to, or removed from, the field result since the field was last updated.
		fm_fldlock,		// fldlock	Field is locked and cannot be updated.
		fm_fldpriv		// fldpriv	Result is not in a form suitable for display (for example, binary data used by fields whose result is a picture).
	};

	_FieldMode				m_eMode;
	TextItemContainerPtr	m_oInsert;
	bool					m_bReferenceToEndnote;
	CString					m_sData;
	TextItemContainerPtr	m_oResult;
	bool					m_bTextOnly;
	RtfCharProperty			m_oCharProperty;

	RtfField()
	{
		SetDefault();
	}

	int GetType()
	{
		return TYPE_RTF_FIELD;
	}

	bool IsValid()
	{
		return true;
		//return false == m_oInsert.IsValid() && false == m_oResult.IsValid(); 
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
	{
		m_eMode					= fm_none;
		m_bReferenceToEndnote	= false;
		m_bTextOnly				= false;
		m_sData					= L"";

		m_oInsert				= TextItemContainerPtr( new TextItemContainer() );
		m_oResult				= TextItemContainerPtr( new TextItemContainer() );

		m_oCharProperty.SetDefault();
	}

	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult += L"{\\field ";
		
		if( fm_none != m_eMode )
		{
			switch( m_eMode )
			{
				case fm_flddirty:	sResult += L"{\\flddirty ";	break;
				case fm_fldedit:	sResult += L"{\\fldedit ";	break;
				case fm_fldlock:	sResult += L"{\\fldlock ";	break;
				case fm_fldpriv:	sResult += L"{\\fldpriv ";	break;
			}	
		}
		sResult += L"{\\*\\fldinst ";
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType		= RENDER_TO_RTF_PARAM_PLAIN;//RENDER_TO_RTF_PARAM_CHAR;
		oNewParam.nValue	= RENDER_TO_RTF_PARAM_NO_PAR;
		
		sResult += m_oInsert->RenderToRtf( oNewParam );
		
		if( true == m_bReferenceToEndnote )
			sResult +=  L"\\fldalt";

		if( !m_sData.IsEmpty() )
            sResult += L"{\\*\\datafield " + m_sData + L"}";
		
		sResult += L"}";

        CString str = m_oResult->RenderToRtf( oRenderParameter ) ;
        sResult += L"{\\fldrslt " + str + L"}";
		sResult += L"}";
		return sResult;
	}

	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
		OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
		
		CString sResult;
		
		if( true == m_bTextOnly )
		{
			RenderParameter oNewParam	= oRenderParameter;
			oNewParam.nType				= RENDER_TO_OOX_PARAM_RUN;

			sResult += m_oResult->RenderToOOX(oNewParam);
		}
		else
		{
			bool bInsert = false;
			bool bDelete = false;

			if (m_oCharProperty.m_nRevised != PROP_DEF)
			{
				bInsert = true;
				
				CString sAuthor = m_oCharProperty.m_nRevauth != PROP_DEF ? poRtfDocument->m_oRevisionTable[ m_oCharProperty.m_nRevauth] : L"";
				CString sDate(RtfUtility::convertDateTime(m_oCharProperty.m_nRevdttm).c_str());
				
				sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
				m_oCharProperty.m_nRevised = PROP_DEF;
			}
			if (m_oCharProperty.m_nDeleted != PROP_DEF)
			{
				bDelete = true;
				
				CString sAuthor = m_oCharProperty.m_nRevauthDel != PROP_DEF ? poRtfDocument->m_oRevisionTable[ m_oCharProperty.m_nRevauthDel ] : L"";
				CString sDate(RtfUtility::convertDateTime(m_oCharProperty.m_nRevdttmDel).c_str());
				
				sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
				m_oCharProperty.m_nDeleted = PROP_DEF;
			}
			//поверяем на наличие гиперссылки
			RenderParameter oNewParam	= oRenderParameter;
			oNewParam.nType				= RENDER_TO_OOX_PARAM_PLAIN;
			
			CString sInsertText = m_oInsert->RenderToOOX( oNewParam );
			
			int nIndex = sInsertText.Find( L"HYPERLINK" );			
			if( -1 != nIndex )
			{
				CString sHyperlink = sInsertText;
                sHyperlink.Delete( nIndex, 9/*(int)_tcslen( L"HYPERLINK" )*/ );

				int nSplash = sHyperlink.Find( L"\\" );
				if (nSplash > 0)
				{
					sHyperlink = sHyperlink.Left(nSplash);
				}
		
			//оставляем только одну ссылку
				sHyperlink.Remove( '\"' );
				sHyperlink.Trim();
			//заменяем пробелы на %20
				sHyperlink.Replace( L" ", L"%20" );

			//добавляем в rels
				OOXRelsWriter* poRelsWriter = static_cast<OOXRelsWriter*>( oRenderParameter.poRels );
				CString sId = poRelsWriter->AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", Utils::PrepareToXML( sHyperlink ), false );
			//добавляем гиперссылку в документ


                sResult += L"<w:hyperlink r:id=\"" + sId + L"\" >";
				oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
				
				sResult += m_oResult->RenderToOOX(oNewParam);
				sResult += L"</w:hyperlink>";
			}
			else
			{
				nIndex = sInsertText.Find( L"PRIVATE" );
				if( m_oResult->GetCount() <= 1 && nIndex < 0)
				{
					RenderParameter oNewParametr	= oRenderParameter;
					oNewParametr.nType				= RENDER_TO_OOX_PARAM_PLAIN;
					//sResult += L"<w:r>");

                    CString str = Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) ).Trim();
                    
					sResult += L"<w:fldSimple w:instr=\"";
					sResult += str;
					sResult += L"\">";
					
					RenderParameter oNewParam	= oRenderParameter;
					oNewParam.nType				= RENDER_TO_OOX_PARAM_RUN;
					
					sResult += m_oResult->RenderToOOX(oNewParam);
					sResult.AppendFormat(L"</w:fldSimple>");
					//sResult += L"</w:r>");
				}
				else
				{
				//так добавляются лишние параграфы
					RenderParameter oNewParametr	= oRenderParameter;
					oNewParametr.nType				= RENDER_TO_OOX_PARAM_PLAIN;

					CString props = m_oCharProperty.RenderToOOX(oRenderParameter);
					if (!props.IsEmpty()) props = L"<w:rPr>" + props + L"</w:rPr>";

                    sResult += L"<w:r>";
					if (!props.IsEmpty())	
						sResult += props;
						sResult += L"<w:fldChar w:fldCharType=\"begin\"/>";
					sResult += L"</w:r>";

                    CString str = Utils::PrepareToXML( m_oInsert->RenderToOOX(oNewParametr) );

                    sResult += L"<w:r>";
					if (!props.IsEmpty())	
						sResult += props;
						sResult += L"<w:instrText xml:space=\"preserve\">";
					sResult += str;
					sResult += L"</w:instrText></w:r>";
					
					sResult += L"<w:r>";
					if (!props.IsEmpty())	
						sResult += props;
						sResult += L"<w:fldChar w:fldCharType=\"separate\"/></w:r>";
					//заканчиваем этот параграф
					sResult += L"</w:p>";
					
					//пишем параграфы содержания					
					oNewParametr.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
					sResult += m_oResult->RenderToOOX(oNewParametr);
					//заканчиваем Field
					sResult += L"<w:p>";
					sResult += L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
				}
			}
			if (bDelete)sResult += L"</w:del>";
			if (bInsert)sResult += L"</w:ins>";
		}
		return sResult;
	}
};
typedef boost::shared_ptr<RtfField>				RtfFieldPtr;
typedef boost::shared_ptr<OOXFieldBegin>		OOXFieldBeginPtr;
typedef boost::shared_ptr<OOXFieldInsertText>	OOXFieldInsertTextPtr;
typedef boost::shared_ptr<OOXFieldSeparate>		OOXFieldSeparatePtr;
typedef boost::shared_ptr<OOXFieldEnd>			OOXFieldEndPtr;
