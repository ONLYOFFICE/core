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
#include "Basic.h"

#include "RtfProperty.h"
#include "RtfParagraph.h"
#include "RtfShape.h"

class RtfMath : public IDocumentElement, public ItemContainer<IDocumentElementPtr>
{
public: 
    std::string         m_sRtfName;
    std::string         m_sOOXName;

	bool				m_bHeader; //чтобы отличать заголовок от вложенных групп

	bool				m_bIsVal;
	bool				m_bIsBool;
	
	RtfParagraph		m_oVal;
	RtfShapePtr			m_oPicture;

	RtfMath( )
	{
		m_bHeader = true;
		m_bIsVal = m_bIsBool = false;
	}
	RtfMath(const RtfMath& oMath )
	{
		(*this) = oMath;
	}
	RtfMath& operator=(const RtfMath& oMath )
	{
		m_bHeader	= oMath.m_bHeader;
		m_bIsVal	= oMath.m_bIsVal;
		m_bIsBool	= oMath.m_bIsBool;

		m_sRtfName	= oMath.m_sRtfName;
		m_sOOXName	= oMath.m_sOOXName;
	
		m_aArray.clear();
		m_aArray	= oMath.m_aArray;

		return (*this);
	}
	bool IsEmpty()
	{
        return m_sRtfName.empty();
	}
	bool IsValid()
	{
        return (false == m_sRtfName.empty());
	}
    bool IsRtfControlPropertyBool	( std::string sControl );
    bool IsRtfControlProperty		( std::string sControl );
    bool IsRtfControlWord			( std::string sControl );

    void SetRtfName(std::string sName)
	{
		m_sRtfName = sName;
		
		m_sOOXName = sName;
        m_sOOXName.insert(m_sOOXName.begin() + 1, L':');
	}

	void	SetOOXType		(int type);
	
    std::wstring RenderToRtf		(RenderParameter oRenderParameter);
    std::wstring RenderToOOX		(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfMath> RtfMathPtr;
