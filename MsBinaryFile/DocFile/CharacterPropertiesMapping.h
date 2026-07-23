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

#include "IMapping.h"
#include "RevisionData.h"
#include "ParagraphPropertyExceptions.h"
#include "PropertiesMapping.h"
#include "LanguageId.h"
#include "LanguageIdMapping.h"
#include "StyleSheetMapping.h"
#include "BorderCode.h"
#include "ShadingDescriptor.h"
#include "WordDocument.h"

namespace DocFileFormat
{
	static const wchar_t SuperscriptIndex[3][12] =
	{
        L"baseline",
        L"superscript",
        L"subscript"
	};

	class CharacterPropertiesMapping: public PropertiesMapping, public IMapping
	{
	public:
		CharacterPropertiesMapping( XMLTools::CStringXmlWriter* writer, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded = true );
        CharacterPropertiesMapping( XMLTools::XMLElement* rPr, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded = true );
		virtual ~CharacterPropertiesMapping();
		void Apply( IVisitable* chpx );
		bool CheckIsSymbolFont();

		bool	_webHidden;
		bool	_isRTL;
	private:
        void convertSprms( std::vector<SinglePropertyModifier>* sprms, XMLTools::XMLElement* parent );
		std::list<CharacterPropertyExceptions*> buildHierarchy( const StyleSheet* styleSheet, unsigned short istdStart );
		bool applyToggleHierachy( const SinglePropertyModifier& sprm );
		bool toogleValue( bool currentValue, unsigned char toggle );

	protected:
		/// CHPX flags are special flags because the can be 0,1,128 and 129,
		/// so this method overrides the appendFlagElement method.
        virtual void appendFlagElement( XMLTools::XMLElement* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique );

	private:
		XMLTools::CStringXmlWriter pRunPr;

		WordDocument*					_doc;
        XMLTools::XMLElement*           _rPr;
		unsigned short					_currentIstd;
		RevisionData*					_revisionData;
		bool							_styleChpx;

		ParagraphPropertyExceptions* _currentPapx;
		std::list<CharacterPropertyExceptions*> _hierarchy;

		bool _isRunStyleNeeded;
		bool _isOwnRPr;

		std::wstring m_sAsciiFont;
		std::wstring m_sEastAsiaFont;
		std::wstring m_shAnsiFont;
		std::wstring m_sCsFont;
	};
}
