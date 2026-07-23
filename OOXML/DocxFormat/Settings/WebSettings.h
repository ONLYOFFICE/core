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

#include "../File.h"
#include "../../Common/ComplexTypes.h"
#include "../../Common/SimpleTypes_Rtf.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// OptimizeForBrowser 17.15.2.34 (Part 1)
		//--------------------------------------------------------------------------------

		class COptimizeForBrowser : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COptimizeForBrowser)
			COptimizeForBrowser();
			virtual ~COptimizeForBrowser();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		public:
			nullable<SimpleTypes::COptimizeForBrowserTarget> m_oTarget;
			nullable<SimpleTypes::COnOff                   > m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	// TO DO: Complete this class

	//--------------------------------------------------------------------------------
	// CWebSettings 17.11.15
	//--------------------------------------------------------------------------------	

	class CWebSettings : public OOX::File
	{
	public:
		CWebSettings(OOX::Document *pMain);
		CWebSettings(OOX::Document *pMain, const CPath& oPath);
		virtual ~CWebSettings();

	public:
		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

	public:
		virtual const OOX::FileType type() const;
		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

	public:
		// Childs
		nullable<ComplexTypes::Word::COnOff2 > m_oAllowPNG;
		nullable<ComplexTypes::Word::COptimizeForBrowser             > m_oOptimizeForBrowser;
	};

} // namespace OOX
