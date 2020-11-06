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
#include "Logic/Annotations.h"
#include "Logic/Paragraph.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"
#include "Logic/Pict.h"
#include "Math/oMathPara.h"
#include "Math/OMath.h"
#include "External/HyperLink.h"

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
				CBackground(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CBackground()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_background;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CHexColor<>        >	m_oColor;
			nullable<SimpleTypes::CThemeColor<>      >	m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<>  >	m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>  >	m_oThemeTint;

			nullable<OOX::Logic::CDrawing            >	m_oDrawing;
			nullable<OOX::Vml::CBackground>				m_oBackground;
		};

		//Word 2003 XML Reference
		class CBgPict : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBgPict)
				CBgPict(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CBgPict()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const
			{
				return L"";
			}

			virtual EElementType getType() const
			{
				return et_w_bgPict;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<OOX::Logic::CBinData>		m_oBinData;
			nullable<SimpleTypes::CHexColor<>>	m_oColor;
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
		virtual ~CDocument()
		{
			ClearItems();
		}
		const CDocument& operator =(const XmlUtils::CXmlNode& oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CDocument& operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		virtual void read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		
		void CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth);

		virtual void read(const CPath& oRootPath, const CPath& oPath);
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
		virtual const OOX::FileType type() const
		{
			if (m_bMacroEnabled)	return FileTypes::DocumentMacro;
			else					return FileTypes::Document;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& GetReadPath()
		{
			return m_oReadPath;
		}
		void ClearItems();
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const;
		virtual EElementType getType() const
		{
			return et_w_document;
		}
//---------------------------------------------------------------------------------------------
		bool									m_bMacroEnabled = false;
		CPath									m_oReadPath;
		unsigned int							m_nDrawingMaxZIndex = 0;

		SimpleTypes::CConformanceClass<SimpleTypes::conformanceclassTransitional> m_oConformance;

		nullable<SimpleTypes::CXmlSpace<>>		m_oSpace;

		nullable<OOX::Logic::CSectionProperty>	m_oSectPr;
		nullable<OOX::Logic::CBackground>		m_oBackground;

        std::vector<WritingElement*>			m_arrItems;
	};
} // namespace OOX

