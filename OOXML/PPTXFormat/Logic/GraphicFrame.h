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


#include "./../WrapperWritingElement.h"
#include "NvGraphicFramePr.h"
#include "Xfrm.h"
#include "Table/Table.h"
#include "SmartArt.h"
#include "Pic.h"
#include "../../XlsxFormat/Slicer/SlicerCacheExt.h"
#include "../../DocxFormat/Logic/RunContent.h"

namespace PPTX
{
	namespace Logic
	{
		class GraphicFrame : public WrapperWritingElement
		{
		public:
			GraphicFrame(std::wstring ns = L"p");
			virtual ~GraphicFrame();			
			
			virtual OOX::EElementType getType () const
			{
				return OOX::et_graphicFrame;
			}
			explicit GraphicFrame(XmlUtils::CXmlLiteReader& oReader);
			const GraphicFrame& operator =(XmlUtils::CXmlLiteReader& oReader);

			explicit GraphicFrame(XmlUtils::CXmlNode& node);
			const GraphicFrame& operator =(XmlUtils::CXmlNode& node);

			virtual void fromXML(XmlUtils::CXmlNode& node);
					bool fromXML3(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
					void fromXML2(XmlUtils::CXmlLiteReader& oReader);
					bool fromXML3(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes2(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes3(XmlUtils::CXmlLiteReader& oReader);
			
			virtual std::wstring toXML() const;
					std::wstring toXML2() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
					void toXmlWriter2(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void GetRect(Aggplus::RECT& pRect)const;
	
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, _T("macro"), macro)
				WritingElement_ReadAttributes_End(oReader)
			}
			bool IsEmpty()const;

			std::wstring			m_namespace;

			nullable<NvGraphicFramePr> nvGraphicFramePr;

			nullable<Xfrm>			xfrm;

			nullable_string			macro;
			nullable_string			vmlSpid;
			nullable<Pic>			olePic;

            nullable<Table>			table;
			nullable<SmartArt>		smartArt;
			nullable<ChartRec>		chartRec;
			nullable<OOX::Spreadsheet::CDrawingSlicer>		slicer;
			nullable<OOX::Spreadsheet::CDrawingSlicer>		slicerExt;

			nullable<OOX::Logic::CContentPart>				contentPart;

			SpTreeElem				element;

			std::wstring m_sRequires;//from mc:Choice

			std::wstring GetVmlXmlBySpid(smart_ptr<OOX::IFileContainer> & rels)	const;

			void ChartToOlePackageInStorage(OOX::IFileContainer* pRels, const std::wstring &sTempDirectory, int nCurrentGenerateId);
			std::wstring ChartToXlsx(OOX::IFileContainer* pRels, const std::wstring &sTempDirectory);
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

