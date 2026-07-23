/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

	RtfCharProperty m_oCharProperty;
	
	TextItemContainerPtr m_pTextItems;

	RtfFormFieldPtr m_pFormField;
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
