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

#include "./SpTree.h"

namespace OOX
{
	class CDiagramData;
}

namespace PPTX
{
	namespace Logic
	{
		class SmartArt : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(SmartArt)

			SmartArt()
			{
			}

			SmartArt& operator=(const SmartArt& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}

			virtual OOX::EElementType getType () const
			{
				return OOX::et_dgm_DiagrammParts;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"r:dm", id_data);
				XmlMacroReadAttributeBase(node, L"r:cs", id_color);
				XmlMacroReadAttributeBase(node, L"r:lo", id_layout);
				XmlMacroReadAttributeBase(node, L"r:qs", id_style);
			}
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

			nullable<PPTX::Logic::SpTree> m_oDrawing;
			smart_ptr<OOX::IFileContainer> m_pDrawingContainer;
		protected:
			virtual void FillParentPointersForChilds()
			{
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, (L"r:cs"), id_color)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:dm"), id_data)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:lo"), id_layout)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:qs"), id_style)
				WritingElement_ReadAttributes_End(oReader)
			}
			smart_ptr<OOX::File> FindDiagramDrawing(OOX::CDiagramData* pDiagramData) const;
		};

		class ChartRec : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(ChartRec)
			
			ChartRec()
			{
			}

			ChartRec(const ChartRec& oSrc) 
			{ 
				*this = oSrc; 
			}
			ChartRec& operator=(const ChartRec& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, L"id", id_data )
				WritingElement_ReadAttributes_End_No_NS	( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				std::wstring ns = XmlUtils::GetNamespace(oReader.GetName());

				m_bChartEx = false;
				
				ReadAttributes( oReader );
				FillParentPointersForChilds();
				
				if (ns == L"cx")
				{
					m_bChartEx = true;
				}
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_bChartEx = false;

				std::wstring ns = XmlUtils::GetNamespace(node.GetName());

				XmlMacroReadAttributeBase(node, L"r:id", id_data);
				FillParentPointersForChilds();

				if (ns == L"cx")
				{
					m_bChartEx = true;
				}
			}
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable<OOX::RId>	id_data;

			bool				m_bChartEx = false;
			int					m_nCountCharts = 0;
		protected:
			virtual void FillParentPointersForChilds()
			{				
			}
		};
	} // namespace Logic
} // namespace PPTX
