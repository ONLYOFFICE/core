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
#include "Align.h"


namespace Common
{

	Align::Align()
		: m_type(right)
	{
	}

	
	const Align::Type Align::type() const
	{
		return m_type;
	}


	const Align Align::Right()
	{
		return Align(right);
	}


	const Align Align::Left()
	{
		return Align(left);
	}


	const Align Align::Center()
	{
		return Align(center);
	}


	const Align Align::Both()
	{
		return Align(both);
	}


	const bool Align::isRight() const
	{
		return m_type == right;
	}


	const bool Align::isLeft() const
	{
		return m_type == left;
	}


	const bool Align::isCenter() const
	{
		return m_type == center;
	}


	const bool Align::isBoth() const
	{
		return m_type == both;
	}


	void Align::setRight()
	{
		m_type = right;
	}


	void Align::setLeft()
	{
		m_type = left;
	}


	void Align::setCenter()
	{
		m_type = center;
	}


	void Align::setBoth()
	{
		m_type = both;
	}


	Align::Align(const Align::Type type)
		: m_type(type)
	{
	}


	void Align::fromBase(const Align& align)
	{
		m_type = align.m_type;
	}

} // namespace Common