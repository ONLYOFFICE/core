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
#include "Basic.h"

#include "RtfProperty.h"
#include "RtfParagraph.h"
#include "RtfShape.h"

class RtfMath : public IDocumentElement, public ItemContainer<IDocumentElementPtr>
{
public: 
    std::string         m_sRtfName;
    std::string         m_sOOXName;

	bool				m_bHeader; //to distinguish header from nested groups

	bool				m_bIsVal = false;
	bool				m_bIsChar = false; // for write
	bool				m_bIsBool = false; // for read
	
	RtfParagraph		m_oVal;
	RtfShapePtr			m_oPicture;

	std::wstring         m_sVal;

	RtfMath( );
	RtfMath(const RtfMath& oMath );

	RtfMath& operator=(const RtfMath& oMath );
	bool IsEmpty();
	bool IsValid();

    bool IsRtfControlPropertyBool	( std::string sControl );
    bool IsRtfControlProperty		( std::string sControl );
    bool IsRtfControlWord			( std::string sControl );

	void SetRtfName(std::string sName);

	void	SetOOXType		(int type);
	
    std::wstring RenderToRtf		(RenderParameter oRenderParameter);
    std::wstring RenderToOOX		(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfMath> RtfMathPtr;
