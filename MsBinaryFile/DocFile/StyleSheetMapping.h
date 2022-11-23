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

#include "WordDocument.h"
#include "StyleSheet.h"
#include "AbstractOpenXmlMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CharacterPropertiesMapping.h"
#include "TablePropertiesMapping.h"
#include "ContentTypes.h"
#include "ConversionContext.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class StyleSheetMapping: public AbstractOpenXmlMapping, public IMapping
	{
	private: 
		ConversionContext*	_ctx;
		WordDocument*		m_document;

	public: 
		StyleSheetMapping( ConversionContext* ctx );
		void Apply( IVisitable* visited );
		/// Generates a style id for custom style names or returns the build-in identifier for build-in styles.
		static std::wstring MakeStyleId( StyleSheetDescription* std );
		static std::map<std::wstring, std::wstring> m_mapStyleId;
        static NSCriticalSection::CRITICAL_SECTION_SMART m_mapStyleIdLock;
		virtual ~StyleSheetMapping();

	private:
		std::map<std::wstring, char> mapCheckReservedNames;
		void writeRunDefaults( StyleSheet* sheet );
		void writeParagraphDefaults( StyleSheet* sheet );
		/// Chooses the correct style name.
		/// Word 2007 needs the identifier instead of the stylename for translating it into the UI language.
		std::wstring getStyleName( StyleSheetDescription* std );
		/// Writes the "NormalTable" default style
		void writeNormalTableStyle();
	};
}
