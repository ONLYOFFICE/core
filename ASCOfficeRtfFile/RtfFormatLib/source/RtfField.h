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
#pragma once 
#include "RtfParagraph.h"

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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
class OOXFieldInsertText : public IDocumentElement
{
public: 
	RtfCharPtr m_oText;
	
	int GetType()
	{
		return TYPE_OOX_FIELD;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter)
	{
		if( NULL != m_oText )
			return m_oText->RenderToRtf( oRenderParameter );
		else
			return L"";
	}
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class OOXFieldSeparate : public IDocumentElement
{
public:
	int GetType()
	{
		return TYPE_OOX_FIELD;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter)
	{
        std::wstring sResult;
		return L"}{\\fldrslt";
	}
    std::wstring RenderToOOX(RenderParameter oRenderParameter)
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter)
	{
        std::wstring sResult;
		return L"}}";
	}
    std::wstring RenderToOOX(RenderParameter oRenderParameter)
	{
		return L"<w:fldChar w:fldCharType=\"end\"/>";
	}
};



class RtfFieldInst : public IDocumentElement
{
public:
	RtfFieldInst() 
	{
		SetDefault();
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
		m_pTextItems = TextItemContainerPtr( new TextItemContainer() );
	}

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);

	RtfCharProperty			m_oCharProperty;
	TextItemContainerPtr	m_pTextItems;
};
typedef boost::shared_ptr<RtfFieldInst>	RtfFieldInstPtr;

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
	bool					m_bReferenceToEndnote;
    std::wstring					m_sData;
	
	RtfFieldInstPtr			m_pInsert;
	RtfFieldInstPtr			m_pResult;
	
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

		m_pResult				= RtfFieldInstPtr(new RtfFieldInst());
		m_pInsert				= RtfFieldInstPtr(new RtfFieldInst());
		
		m_oCharProperty.SetDefault();
	}

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfField>				RtfFieldPtr;
typedef boost::shared_ptr<OOXFieldBegin>		OOXFieldBeginPtr;
typedef boost::shared_ptr<OOXFieldInsertText>	OOXFieldInsertTextPtr;
typedef boost::shared_ptr<OOXFieldSeparate>		OOXFieldSeparatePtr;
typedef boost::shared_ptr<OOXFieldEnd>			OOXFieldEndPtr;
