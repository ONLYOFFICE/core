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
#include "Index.h"

 
namespace Common
{

	Index::Index() 
		: m_type(normal)
	{
	}


	const Index::Type Index::type() const
	{
		return m_type;
	}


	const Index Index::Super()	
	{
		return Index(super);
	}


	const Index Index::Normal()
	{
		return Index(normal);
	}


	const Index Index::Sub()
	{
		return Index(sub);
	}


	const bool Index::isSuper() const
	{
		return m_type == super;
	}


	const bool Index::isNormal() const
	{
		return m_type == normal;
	}


	const bool Index::isSub() const
	{
		return m_type == sub;
	}


	void Index::setSuper()
	{
		m_type = super;
	}


	void Index::setNormal()
	{
		m_type = normal;
	}


	void Index::setSub()
	{
		m_type = sub;
	}


	Index::Index(const Index::Type type) 
		: m_type(type)
	{
	}


	void Index::fromBase(const Index& index)
	{
		m_type = index.m_type;
	}

} // namespace Common