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
#include "../Base/Nullable.h"

#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"

#include "WritingElement.h"
#include "File.h"
#include "FileTypes.h"
#include "IFileContainer.h"

#include "Logic/DocParts.h"
#include "Logic/SectionProperty.h"
#include "Logic/Pict.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Background 17.2.1 (Part 1)
		//--------------------------------------------------------------------------------

		class CBackground : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBackground)
			CBackground(OOX::Document *pMain = NULL);
			virtual ~CBackground();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHexColor          >	m_oColor;
			nullable<SimpleTypes::CThemeColor        >	m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber    >	m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber    >	m_oThemeTint;

			nullable<OOX::Logic::CDrawing            >	m_oDrawing;
			nullable<OOX::Vml::CBackground>				m_oBackground;
		};

		//Word 2003 XML Reference
		class CBgPict : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBgPict)
			CBgPict(OOX::Document *pMain = NULL);
			virtual ~CBgPict();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<OOX::Logic::CBinData>		m_oBinData;
			nullable<SimpleTypes::CHexColor>	m_oColor;
			nullable_string						m_oBackgroundType;
			nullable<OOX::Vml::CBackground>		m_oBackground;
		};

	}

	//--------------------------------------------------------------------------------
	// CDocument 17.2.3 (Part 1)
	//--------------------------------------------------------------------------------
	class CDocument : public OOX::File, public IFileContainer, public WritingElement
	{
	public:
		CDocument(OOX::Document *pMain = NULL, OOX::FileType type = OOX::FileTypes::Document);
		CDocument(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath, OOX::FileType type = OOX::FileTypes::Document);
		CDocument(XmlUtils::CXmlNode& oNode);
		CDocument(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CDocument();

		const CDocument& operator =(const XmlUtils::CXmlNode& oNode);
		const CDocument& operator =(const XmlUtils::CXmlLiteReader& oReader);

		virtual void read(const CPath& oPath);
		
		void CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth);

		virtual void read(const CPath& oRootPath, const CPath& oPath);
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		const CPath& GetReadPath();

		void ClearItems();
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		virtual void fromXML(XmlUtils::CXmlNode& oNode);

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

//---------------------------------------------------------------------------------------------
		bool									m_bMacroEnabled = false;
		CPath									m_oReadPath;
		unsigned int							m_nDrawingMaxZIndex = 0;

		SimpleTypes::CConformanceClass			m_oConformance;

		nullable<SimpleTypes::CXmlSpace>		m_oSpace;

		nullable<OOX::Logic::CSectionProperty>	m_oSectPr;
		nullable<OOX::Logic::CBackground>		m_oBackground;

        std::vector<WritingElement*>			m_arrItems; 
//---------------------------------------------------------------------------------------------
		struct _section
		{
			size_t start_elm = 0;
			size_t end_elm = 0;
			WritingElement* sect = NULL;
		};
		std::vector<_section> m_arrSections; // дублирование ... для удобства конвертаций

	};

} // namespace OOX

