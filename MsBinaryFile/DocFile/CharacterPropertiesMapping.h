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
        void convertSprms( std::list<SinglePropertyModifier>* sprms, XMLTools::XMLElement* parent );
		std::list<CharacterPropertyExceptions*> buildHierarchy( const StyleSheet* styleSheet, unsigned short istdStart );
		bool applyToggleHierachy( const SinglePropertyModifier& sprm );
		bool toogleValue( bool currentValue, unsigned char toggle );

	protected:
		/// CHPX flags are special flags because the can be 0,1,128 and 129,
		/// so this method overrides the appendFlagElement method.
        virtual void appendFlagElement( XMLTools::XMLElement* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique );

	private:
		XMLTools::CStringXmlWriter			pRunPr;

		WordDocument*					_doc;
        XMLTools::XMLElement*           _rPr;
		unsigned short					_currentIstd;
		RevisionData*					_revisionData;
		bool							_styleChpx;

		ParagraphPropertyExceptions*			_currentPapx;
		std::list<CharacterPropertyExceptions*>	_hierarchy;

		bool _isRunStyleNeeded;
		bool _isOwnRPr;

		std::wstring m_sAsciiFont;
		std::wstring m_sEastAsiaFont;
		std::wstring m_shAnsiFont;
		std::wstring m_sCsFont;
	};
}
