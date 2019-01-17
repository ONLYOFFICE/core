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

#include "Sepx.h"
#include "Header.h"
#include "Footer.h"

namespace ASCDocFileFormat
{
	class SectionProperties
	{
	public:
		explicit SectionProperties(const Sepx& _sepx) : sepx(_sepx)
		{
			SetEvenPageHeader(Header());
			SetOddPageHeader(Header());
			SetEvenPageFooter(Footer());
			SetOddPageFooter(Footer());
			SetFirstPageHeader(Header());
			SetFirstPageFooter(Footer());
		}

		SectionProperties(const SectionProperties& sp) : sepx(sp.sepx)
		{
			if (sp.evenPageHeader.IsInit())		SetEvenPageHeader(sp.evenPageHeader.as<Header>());
			if (sp.oddPageHeader.IsInit())		SetOddPageHeader(sp.oddPageHeader.as<Header>());
			if (sp.evenPageFooter.IsInit())		SetEvenPageFooter(sp.evenPageFooter.as<Footer>());
			if (sp.oddPageFooter.IsInit())		SetOddPageFooter(sp.oddPageFooter.as<Footer>());
			if (sp.firstPageHeader.IsInit())	SetFirstPageHeader(sp.firstPageHeader.as<Header>());
			if (sp.firstPageFooter.IsInit())	SetFirstPageFooter(sp.firstPageFooter.as<Footer>());
		}

		inline const Sepx& GetSepx() const
		{
			return sepx;
		}

		inline void SetEvenPageHeader(const Header& _header)
		{
			evenPageHeader = _header;
		}

		inline void SetOddPageHeader(const Header& _header)
		{
			oddPageHeader = _header;
		}

		inline void SetEvenPageFooter(const Footer& _footer)
		{
			evenPageFooter = _footer;
		}

		inline void SetOddPageFooter(const Footer& _footer)
		{
			oddPageFooter = _footer;
		}

		inline void SetFirstPageHeader(const Header& _header)
		{
			firstPageHeader = _header;
		}

		inline void SetFirstPageFooter(const Footer& _footer)
		{
			firstPageFooter = _footer;
		}

		inline const Header* GetEvenPageHeader() const
		{
			return static_cast<const Header*>(evenPageHeader.Get());
		}

		inline const Header* GetOddPageHeader() const
		{
			return static_cast<const Header*>(oddPageHeader.Get());
		}

		inline const Footer* GetEvenPageFooter() const
		{
			return static_cast<const Footer*>(evenPageFooter.Get());
		}

		inline const Footer* GetOddPageFooter() const
		{
			return static_cast<const Footer*>(oddPageFooter.Get());
		}

		inline const Header* GetFirstPageHeader() const
		{
			return static_cast<const Header*>(firstPageHeader.Get());
		}

		inline const Footer* GetFirstPageFooter() const
		{
			return static_cast<const Footer*>(firstPageFooter.Get());
		}

	private:
		
		Sepx		sepx;
		TextItem	evenPageHeader;
		TextItem	oddPageHeader;
		TextItem	evenPageFooter;
		TextItem	oddPageFooter;
		TextItem	firstPageHeader;
		TextItem	firstPageFooter;
	};
}