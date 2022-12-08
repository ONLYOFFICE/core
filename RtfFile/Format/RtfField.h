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

	OOXFieldBegin();

	int GetType();
	void SetDefault();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class OOXFieldInsertText : public IDocumentElement
{
public: 
	RtfCharPtr m_oText;
	
	int GetType();

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class OOXFieldSeparate : public IDocumentElement
{
public:
	int GetType();

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class OOXFieldEnd : public IDocumentElement
{
public: 
	int GetType();

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfFormField : public IDocumentElement
{
public:
	RtfFormField();

	int type = PROP_DEF;
	int ownhelp = PROP_DEF;
	int ownstat = PROP_DEF;
	int prot = PROP_DEF;
	int sizeCheckBox = PROP_DEF; //Auto, Exact
	int typetx = PROP_DEF;
	int recalc = PROP_DEF;
	int haslistbox = PROP_DEF;
	int maxlen = PROP_DEF;
	int hps = PROP_DEF;
	int defres = PROP_DEF;
	int res = PROP_DEF;

	std::wstring name;
	std::wstring deftext;
	std::wstring format;
	std::wstring helptext;
	std::wstring stattext;
	std::wstring entrymcr;
	std::wstring exitmcr;
	std::vector<std::wstring> list;
	std::wstring datafield;

	int GetType();

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfFormField>	RtfFormFieldPtr;

class RtfFieldInst : public IDocumentElement
{
public:
	RtfFieldInst();

	void SetDefaultRtf();
	void SetDefaultOOX();
	void SetDefault();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);

	RtfCharProperty			m_oCharProperty;
	TextItemContainerPtr	m_pTextItems;

	RtfFormFieldPtr			m_pFormField;
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
    std::wstring			m_sData;
	
	RtfFieldInstPtr			m_pInsert;
	RtfFieldInstPtr			m_pResult;
	
	bool					m_bTextOnly;
	RtfCharProperty			m_oCharProperty;

	RtfField();

	int GetType();
	bool IsValid();

	void SetDefaultRtf();
	void SetDefaultOOX();
	void SetDefault();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfField>				RtfFieldPtr;
typedef boost::shared_ptr<OOXFieldBegin>		OOXFieldBeginPtr;
typedef boost::shared_ptr<OOXFieldInsertText>	OOXFieldInsertTextPtr;
typedef boost::shared_ptr<OOXFieldSeparate>		OOXFieldSeparatePtr;
typedef boost::shared_ptr<OOXFieldEnd>			OOXFieldEndPtr;
