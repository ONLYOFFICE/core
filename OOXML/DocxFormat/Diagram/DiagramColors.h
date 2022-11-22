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

#include "DiagramData.h"


namespace OOX
{
	namespace Diagram
	{
		class CClrLst : public WritingElementWithChilds<PPTX::Logic::UniColor>
		{
		public:
			WritingElement_AdditionConstructors(CClrLst)
			CClrLst() {}
			virtual ~CClrLst() {}
			virtual std::wstring toXML() const
			{
				NSBinPptxRW::CXmlWriter oWriter;
				toXmlWriter(&oWriter);

				return oWriter.GetXmlString();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType() const
			{
				return et_dgm_ClrLst;
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			std::wstring node_name;

			nullable<SimpleTypes::CHueDirType> m_oHueDir;
			nullable<SimpleTypes::CColorMethod> m_oMeth;
		};

		class CColorStyleLbl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorStyleLbl)

			CColorStyleLbl() {}
			virtual ~CColorStyleLbl() {}
			virtual std::wstring toXML() const
			{
				NSBinPptxRW::CXmlWriter oWriter;
				toXmlWriter(&oWriter);

				return oWriter.GetXmlString();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType() const
			{
				return et_dgm_ColorStyleLbl;
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_sName;

			nullable<Diagram::CClrLst> m_oEffectClrLst;
			nullable<Diagram::CClrLst> m_oFillClrLst;
			nullable<Diagram::CClrLst> m_oLinClrLst;
			nullable<Diagram::CClrLst> m_oTxEffectClrLst;
			nullable<Diagram::CClrLst> m_oTxFillClrLst;
			nullable<Diagram::CClrLst> m_oTxLinClrLst;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
	}
    class CDiagramColors : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CDiagramColors(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
		{
		}
		CDiagramColors(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
		{
			read(uri.GetDirectory(), uri);
		}
		CDiagramColors(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
		{
			read(oRootPath, oPath);
		}
		virtual ~CDiagramColors()
		{
		}
		virtual void read(const CPath& oFilePath)
		{
			CPath oRootPath;
			read(oRootPath, oFilePath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const
		{
			return FileTypes::DiagramColors;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
//--------------------------------------------
		nullable_string m_sUniqueId;
		nullable_string m_sMinVer;

		nullable<Diagram::CCatLst> m_oCatLst;
		nullable<Diagram::CText> m_oDesc;
		nullable<Diagram::CText> m_oTitle;
		std::vector<Diagram::CColorStyleLbl*> m_arStyleLbl;
		
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
	};
} // namespace OOX
