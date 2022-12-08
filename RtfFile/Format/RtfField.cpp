/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include <boost/algorithm/string.hpp>

#include "RtfField.h"

#include "../OOXml/Writer/OOXWriter.h"
#include "../OOXml/Writer/OOXRelsWriter.h"

OOXFieldBegin::OOXFieldBegin()
{
	SetDefault();
}
int OOXFieldBegin::GetType()
{
	return TYPE_OOX_FIELD;
}
void OOXFieldBegin::SetDefault()
{
	m_bDirty	= PROP_DEF;
	m_bLock		= PROP_DEF;

	m_oCharProperty.SetDefault();
}
std::wstring OOXFieldBegin::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	sResult += L"{\\field ";
	RENDER_RTF_BOOL( m_bDirty, sResult, L"flddirty" )
	RENDER_RTF_BOOL( m_bLock, sResult, L"fldlock" )
	sResult += L"{\\*\\fldinst";
	
	sResult +=  m_oCharProperty.RenderToRtf(oRenderParameter);
	return sResult;
}
std::wstring OOXFieldBegin::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	sResult +=  L"<w:r>";
        std::wstring props = m_oCharProperty.RenderToOOX(oRenderParameter);
        if (props.empty())
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

int OOXFieldInsertText::GetType()
{
	return TYPE_OOX_FIELD;
}
std::wstring OOXFieldInsertText::RenderToRtf(RenderParameter oRenderParameter)
{
	if( NULL != m_oText )
		return m_oText->RenderToRtf( oRenderParameter );
	else
		return L"";
}
std::wstring OOXFieldInsertText::RenderToOOX(RenderParameter oRenderParameter)
{
	if( NULL != m_oText )
	{
        std::wstring sResult;
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

int OOXFieldSeparate::GetType()
{
	return TYPE_OOX_FIELD;
}
std::wstring OOXFieldSeparate::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	return L"}{\\fldrslt";
}
std::wstring OOXFieldSeparate::RenderToOOX(RenderParameter oRenderParameter)
{
	return L"<w:fldChar w:fldCharType=\"separate\"/>";
}

int OOXFieldEnd::GetType()
{
	return TYPE_OOX_FIELD;
}
std::wstring OOXFieldEnd::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	return L"}}";
}
std::wstring OOXFieldEnd::RenderToOOX(RenderParameter oRenderParameter)
{
	return L"<w:fldChar w:fldCharType=\"end\"/>";
}

RtfFormField::RtfFormField()
{
}
int RtfFormField::GetType()
{
	return TYPE_OOX_FORMFIELD;
}
std::wstring RtfFormField::RenderToRtf(RenderParameter oRenderParameter)
{
	return L"";
}
std::wstring RtfFormField::RenderToOOX(RenderParameter oRenderParameter)
{
	return L"";
}

RtfFieldInst::RtfFieldInst()
{
	SetDefault();
}
void RtfFieldInst::SetDefaultRtf()
{
	SetDefault();
}
void RtfFieldInst::SetDefaultOOX()
{
	SetDefault();
}
void RtfFieldInst::SetDefault()
{
	m_pTextItems = TextItemContainerPtr( new TextItemContainer() );
}
std::wstring RtfFieldInst::RenderToRtf(RenderParameter oRenderParameter)
{
	return L"";
}
std::wstring RtfFieldInst::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	pRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		pOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
	if (m_pTextItems)
		return m_pTextItems->RenderToOOX(oRenderParameter);
	else 
		return L"";
}

RtfField::RtfField()
{
	SetDefault();
}
int RtfField::GetType()
{
	return TYPE_RTF_FIELD;
}
bool RtfField::IsValid()
{
	return true;
	//return false == m_oInsert.IsValid() && false == m_oResult.IsValid();
}
void RtfField::SetDefaultRtf()
{
	SetDefault();
}
void RtfField::SetDefaultOOX()
{
	SetDefault();
}
void RtfField::SetDefault()
{
	m_eMode					= fm_none;
	m_bReferenceToEndnote	= false;
	m_bTextOnly				= false;
	m_sData					= L"";

	m_pResult				= RtfFieldInstPtr(new RtfFieldInst());
	m_pInsert				= RtfFieldInstPtr(new RtfFieldInst());

	m_oCharProperty.SetDefault();
}
std::wstring RtfField::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
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
	
	sResult += m_pInsert->m_pTextItems->RenderToRtf( oNewParam );
	
	if( true == m_bReferenceToEndnote )
		sResult +=  L"\\fldalt";

    if( !m_sData.empty() )
        sResult += L"{\\*\\datafield " + m_sData + L"}";
	
	sResult += L"}";

    std::wstring str = m_pResult->m_pTextItems->RenderToRtf( oRenderParameter ) ;
   
	sResult += L"{\\fldrslt " + str + L"}";
	sResult += L"}";
	return sResult;
}
std::wstring RtfField::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	pRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		pOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
    std::wstring sResult;
	
	if( true == m_bTextOnly )
	{
		RenderParameter oNewParam	= oRenderParameter;
		oNewParam.nType				= RENDER_TO_OOX_PARAM_RUN;

		sResult += m_pResult->m_pTextItems->RenderToOOX(oNewParam);
	}
	else
	{
		bool bInsert = false;
		bool bDelete = false;

        std::wstring sAuthor, sDate;

		if (m_pInsert->m_oCharProperty.m_nRevised != PROP_DEF)
		{
			bInsert = true;
			
			sAuthor = pRtfDocument->m_oRevisionTable.GetAuthor(m_pInsert->m_oCharProperty.m_nRevauth);
            sDate	= std::wstring(RtfUtility::convertDateTime(m_pInsert->m_oCharProperty.m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(pOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_pInsert->m_oCharProperty.m_nRevised = PROP_DEF;
		}
		if (m_pInsert->m_oCharProperty.m_nDeleted != PROP_DEF)
		{
			bDelete = true;
			
			sAuthor = pRtfDocument->m_oRevisionTable.GetAuthor(m_pInsert->m_oCharProperty.m_nRevauthDel);
            sDate	= std::wstring(RtfUtility::convertDateTime(m_pInsert->m_oCharProperty.m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(pOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_pInsert->m_oCharProperty.m_nDeleted = PROP_DEF;
		}
		//поверяем на наличие гиперссылки
		RenderParameter oNewParam	= oRenderParameter;
		oNewParam.nType				= RENDER_TO_OOX_PARAM_PLAIN;
		
        std::wstring sInsertText = m_pInsert->m_pTextItems->RenderToOOX( oNewParam );
		
        size_t nIndex = sInsertText.find( L"HYPERLINK" );
		if( std::wstring::npos != nIndex )
		{
            std::wstring sHyperlink = sInsertText;
            sHyperlink.erase( nIndex, 9/*(int)_tcslen( L"HYPERLINK" )*/ );

            size_t nSplash = sHyperlink.find( L"\\" );
			if (std::wstring::npos != nSplash)
			{
                sHyperlink = sHyperlink.substr(0, nSplash);
			}
	
		//оставляем только одну ссылку
            XmlUtils::replace_all(sHyperlink, L"\"", L"" );
            boost::algorithm::trim(sHyperlink);
		//заменяем пробелы на %20
            XmlUtils::replace_all(sHyperlink, L" ", L"%20" );

		//добавляем в rels
			OOXRelsWriter* poRelsWriter = static_cast<OOXRelsWriter*>( oRenderParameter.poRels );
            std::wstring sId = poRelsWriter->AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", XmlUtils::EncodeXmlString( sHyperlink ), false );
		//добавляем гиперссылку в документ

            sResult += L"<w:hyperlink r:id=\"" + sId + L"\" >";
			oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
			
			sResult += m_pResult->RenderToOOX(oNewParam);
			sResult += L"</w:hyperlink>";
		}
		else if ((m_pInsert->m_pFormField) && (m_pInsert->m_pFormField->type == 2))
		{
			_UINT32 id = 0x7fff + pOOXWriter->m_nFormFieldId++;
			std::wstring text_ = m_pInsert->m_pFormField->defres < m_pInsert->m_pFormField->list.size() ? 
				m_pInsert->m_pFormField->list[m_pInsert->m_pFormField->defres] : L"";
			
			sResult += L"<w:sdt><w:sdtPr><w:alias w:val=\"\"/><w:id w:val=\"" + std::to_wstring(id) + L"\"/><w:dropDownList>";
			for (size_t i = 0; i < m_pInsert->m_pFormField->list.size(); i++)
			{
				sResult += L"<w:listItem w:value=\"" + XmlUtils::EncodeXmlString(m_pInsert->m_pFormField->list[i]) + L"\" w:displayText=\"" + m_pInsert->m_pFormField->list[i] + L"\"/>";
			}
			sResult += L"</w:dropDownList></w:sdtPr><w:sdtEndPr/><w:sdtContent>";
			sResult += L"<w:r><w:t>" + XmlUtils::EncodeXmlString(text_) + L"</w:t></w:r>";
			sResult += L"</w:sdtContent></w:sdt>";
		}
		else
		{
			RenderParameter oNewParametr	= oRenderParameter;
			oNewParametr.nType				= RENDER_TO_OOX_PARAM_PLAIN;

			std::wstring props = m_pResult->m_oCharProperty.RenderToOOX(oRenderParameter);
            if (!props.empty()) props = L"<w:rPr>" + props + L"</w:rPr>";

	//начинаем Field
            sResult += L"<w:r>";
            if (!props.empty())
				sResult += props;			
			sResult += L"<w:fldChar w:fldCharType=\"begin\"";
			if (m_pInsert->m_pFormField)
			{
				sResult += L"><w:ffData>";
				sResult += L"<w:name w:val=\"" + XmlUtils::EncodeXmlString(m_pInsert->m_pFormField->name) + L"\"/>";
				sResult += L"<w:enabled/>";
				
				if (m_pInsert->m_pFormField->recalc == 1) 
					sResult += L"<w:calcOnExit/>";
				else
					sResult += L"<w:calcOnExit w:val=\"0\"/>";

				if (false == m_pInsert->m_pFormField->entrymcr.empty())
					sResult += L"<w:entryMacro w:val=\"" + m_pInsert->m_pFormField->entrymcr + L"\"/>";
				if (false == m_pInsert->m_pFormField->exitmcr.empty())
					sResult += L"<w:exitMacro w:val=\"" + m_pInsert->m_pFormField->exitmcr + L"\"/>";

				if (false == m_pInsert->m_pFormField->helptext.empty())
					sResult += L"<w:helpText w:type=\"text\" w:val=\"" + XmlUtils::EncodeXmlString(m_pInsert->m_pFormField->helptext) + L"\"/>";
				if (false == m_pInsert->m_pFormField->stattext.empty())
					sResult += L"<w:statusText w:type=\"text\" w:val=\"" + XmlUtils::EncodeXmlString(m_pInsert->m_pFormField->stattext) + L"\"/>";
				
				if (m_pInsert->m_pFormField->type == 0)
				{
					sResult += L"<w:textInput>";
					switch (m_pInsert->m_pFormField->typetx)
					{
					case 1: sResult += L"<w:type w:val=\"number\"/>"; break;
					case 2: sResult += L"<w:type w:val=\"date\"/>"; break;
					case 3: sResult += L"<w:type w:val=\"currentDate\"/>"; break;
					case 4: sResult += L"<w:type w:val=\"currentTime\"/>"; break;
					case 5: sResult += L"<w:type w:val=\"calculated\"/>"; break;
					}
					if (false == m_pInsert->m_pFormField->deftext.empty())
						sResult += L"<w:default w:val=\"" + XmlUtils::EncodeXmlString(m_pInsert->m_pFormField->deftext) + L"\"/>";
					if (false == m_pInsert->m_pFormField->format.empty())
						sResult += L"<w:format w:val=\"" + XmlUtils::EncodeXmlString(m_pInsert->m_pFormField->format) + L"\"/>";
					if (m_pInsert->m_pFormField->maxlen > 0)
						sResult += L"<w:maxLength w:val=\"" + std::to_wstring(m_pInsert->m_pFormField->maxlen) + L"\"/>";
					sResult += L"</w:textInput>";
				}
				else if (m_pInsert->m_pFormField->type == 1)
				{
					sResult += L"<w:checkBox>";
					if (m_pInsert->m_pFormField->sizeCheckBox == 1)
						sResult += L"<w:size w:val=\"" + std::to_wstring(m_pInsert->m_pFormField->hps) + L"\"/>";
					else 
						sResult += L"<w:sizeAuto/>";
					sResult += L"<w:default w:val=\"0\"/>";
					if (m_pInsert->m_pFormField->res == 1)
					{
						sResult += L"<w:checked/>";
					}
					else
					{
						sResult += L"<w:checked w:val=\"0\"/>";
					}

					sResult += L"</w:checkBox>";
				}
				sResult += L"</w:ffData></w:fldChar>";
			}
			else sResult += L"/>";

			sResult += L"</w:r>";
	//-----------
            sResult += L"<w:r>";
            if (!props.empty())
				sResult += props;			
			
			sResult += L"<w:instrText xml:space=\"preserve\">";

			if (m_pInsert->m_pTextItems)
			{
                sResult += XmlUtils::EncodeXmlString( m_pInsert->m_pTextItems->RenderToOOX(oNewParametr), true );
			}
			sResult += L"</w:instrText></w:r>";
			
	// разделитель
			sResult += L"<w:r>";
			sResult += L"<w:fldChar w:fldCharType=\"separate\"/></w:r>";
			
	//пишем содержание-кэш		
			if ((m_pResult->m_pTextItems) && (m_pResult->m_pTextItems->GetCount() > 0))
			{
				oNewParametr.nType = RENDER_TO_OOX_PARAM_RUN;
				sResult +=  m_pResult->m_pTextItems->m_aArray[0]->RenderToOOX(oNewParametr);

				for (int i = 1; i < m_pResult->m_pTextItems->GetCount(); i++)
				{
					RtfParagraph *paragraph = dynamic_cast<RtfParagraph *>(m_pResult->m_pTextItems->m_aArray[i].get());
					if (paragraph)
					{
						if (bDelete) sResult += L"</w:del>";
						if (bInsert) sResult += L"</w:ins>";

						bDelete = bInsert = false;

						sResult += L"</w:p>";
						sResult += L"<w:p>";
							sResult += L"<w:pPr>";						
								sResult += paragraph->m_oProperty.RenderToOOX(oRenderParameter);
							sResult += L"</w:pPr>";
					}

					sResult +=  m_pResult->m_pTextItems->m_aArray[i]->RenderToOOX(oNewParametr);
				}
			}
			else
			{
				sResult += L"<w:r>";
                if (!props.empty())
					sResult += props;
				sResult += L"</w:r>";
			}
	
	//заканчиваем Field
			sResult += L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
		}
		if (bDelete) sResult += L"</w:del>";
		if (bInsert) sResult += L"</w:ins>";
	}
	return sResult;
}
