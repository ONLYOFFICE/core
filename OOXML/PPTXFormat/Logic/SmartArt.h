﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#pragma once

#include "./SpTree.h"

namespace OOX
{
	class CDiagramData;
	
	namespace Diagram
	{
		class CBg;
	}
}

namespace PPTX
{
	namespace Logic
	{
		class SmartArt : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(SmartArt)

			SmartArt();
			SmartArt& operator=(const SmartArt& oSrc);

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			bool LoadDrawing(OOX::IFileContainer* pRels);
			void LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter = NULL);

			nullable<OOX::RId> id_data;
			nullable<OOX::RId> id_color;
			nullable<OOX::RId> id_layout;
			nullable<OOX::RId> id_style;

			nullable<OOX::Diagram::CBg> m_oDataBg;
			nullable<PPTX::Logic::SpTree> m_oDrawing;
			
			smart_ptr<OOX::IFileContainer> m_pDrawingContainer;
			smart_ptr<OOX::IFileContainer> m_pDataContainer;

		protected:
			virtual void FillParentPointersForChilds();

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			smart_ptr<OOX::File> FindDiagramDrawing(OOX::CDiagramData* pDiagramData) const;
		};

		class ChartRec : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(ChartRec)
			
			ChartRec();
			ChartRec(const ChartRec& oSrc);

			ChartRec& operator=(const ChartRec& oSrc);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable<OOX::RId>	id_data;

			bool				m_bChartEx = false;
			int					m_nCountCharts = 0;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX
