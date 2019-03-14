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
#ifndef PPTX_LOGIC_SLIDE_SMARTART_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_SMARTART_INCLUDE_H_

#include "./SpTree.h"

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

				m_diag = oSrc.m_diag;
				return *this;
			}

			virtual OOX::EElementType getType () const
			{
				return OOX::et_dgm_DiagrammParts;
			}
			
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_Read_if	  ( oReader, _T("r:cs"), id_color)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:dm"), id_data)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:lo"), id_layout)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:qs"), id_style)
				WritingElement_ReadAttributes_End	( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				//FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"r:dm", id_data);
				XmlMacroReadAttributeBase(node, L"r:cs", id_color);
				XmlMacroReadAttributeBase(node, L"r:lo", id_layout);
				XmlMacroReadAttributeBase(node, L"r:qs", id_style);
				//FillParentPointersForChilds();
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{				
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				pReader->SkipRecord();
			}

			nullable<OOX::RId>				id_data;
			nullable<OOX::RId>				id_color;
			nullable<OOX::RId>				id_layout;
			nullable<OOX::RId>				id_style;

			nullable<PPTX::Logic::SpTree>	m_diag;
			smart_ptr<OOX::IFileContainer>	m_pFileContainer;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(m_diag.IsInit())
					m_diag->SetParentPointer(this);
			}

		public:
			void LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter = NULL);
			bool LoadDrawing(OOX::IFileContainer* pRels);
		};

		class ChartRec : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(ChartRec)
			
			ChartRec() : m_bChartEx(false), m_bData(false), m_lChartNumber(0)
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
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("r:id"), id_data )
				WritingElement_ReadAttributes_End	( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				std::wstring ns = XmlUtils::GetNamespace(oReader.GetName());

				m_bData = false;
				m_bChartEx = false;
				
				ReadAttributes( oReader );
				FillParentPointersForChilds();
				
				if (id_data.IsInit())
				{
					m_bData = true;
				}

				if (ns == L"cx")
				{
					m_bChartEx = true;
				}
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_bData = false;
				m_bChartEx = false;

				std::wstring ns = XmlUtils::GetNamespace(node.GetName());

				XmlMacroReadAttributeBase(node, L"r:id", id_data);
				FillParentPointersForChilds();

				if (id_data.IsInit())
				{
					m_bData = true;
				}
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

			LONG				m_lChartNumber;
			bool				m_bData;
			bool				m_bChartEx;
		protected:
			virtual void FillParentPointersForChilds()
			{				
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_SMARTART_INCLUDE_H_
