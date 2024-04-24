/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#ifndef CSVGPARSER_H
#define CSVGPARSER_H

#include "../../graphics/pro/Fonts.h"
#include "../../../common/Directory.h"
#include "../../../xml/include/xmlutils.h"

#include "SvgObjects/CContainer.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser
	{
	public:
		CSvgParser();
		~CSvgParser();

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		bool LoadFromFile(const std::wstring& wsFile, CGraphicsContainer* pContainer, CSvgFile* pFile) const;
		bool LoadFromString(const std::wstring& wsContentent, CGraphicsContainer* pContainer, CSvgFile* pFile) const;
		bool LoadFromXmlNode(XmlUtils::CXmlNode& oElement, CGraphicsContainer* pContainer, CSvgFile* pFile) const;

		template <class ObjectType>
		bool ReadObject(XmlUtils::CXmlNode& oElement, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;
	private:
		template <class ObjectType>
		bool ReadChildrens(XmlUtils::CXmlNode& oElement, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;

		bool ScanStyles(XmlUtils::CXmlNode& oElement, CSvgFile* pFile) const;

		void UpdateStyles(CObject* pObject, CSvgFile* pFile) const;
		bool MarkObject(CObject* pObject, CSvgFile* pFile) const;
		template <class ObjectType>
		bool AddObject(ObjectType* pObject, CContainer<ObjectType>* pContainer) const;

		NSFonts::IFontManager  *m_pFontManager;
	};
}

#endif // CSVGPARSER_H
