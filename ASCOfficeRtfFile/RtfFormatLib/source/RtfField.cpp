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

#include "Writer/OOXWriter.h"
#include "Writer/OOXRelsWriter.h"

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
std::wstring RtfFieldInst::RenderToRtf(RenderParameter oRenderParameter)
{
	return L"";
}
std::wstring RtfFieldInst::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
	if (m_pTextItems)
		return m_pTextItems->RenderToOOX(oRenderParameter);
	else 
		return L"";
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
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
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
			
			sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_pInsert->m_oCharProperty.m_nRevauth);
            sDate	= std::wstring(RtfUtility::convertDateTime(m_pInsert->m_oCharProperty.m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_pInsert->m_oCharProperty.m_nRevised = PROP_DEF;
		}
		if (m_pInsert->m_oCharProperty.m_nDeleted != PROP_DEF)
		{
			bDelete = true;
			
			sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_pInsert->m_oCharProperty.m_nRevauthDel);
            sDate	= std::wstring(RtfUtility::convertDateTime(m_pInsert->m_oCharProperty.m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
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
			sResult += L"<w:fldChar w:fldCharType=\"begin\"/>";
			sResult += L"</w:r>";
	//-----------
            sResult += L"<w:r>";
			
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
