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
#ifndef PPTX_LOGIC_MOVETO_INCLUDE_H_
#define PPTX_LOGIC_MOVETO_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{

		class MoveTo : public PathBase
		{
		public:
			WritingElement_AdditionConstructors(MoveTo)
			PPTX_LOGIC_BASE2(MoveTo)

			MoveTo& operator=(const MoveTo& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				x = oSrc.x;
				y = oSrc.y;
				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_moveTo;
			}			
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;
					
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (sName == L"a:pt")
					{
						ReadAttributes2(oReader);
					}
				}
			}

			void ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:pt"), oNode))
				{
					x = oNode.GetAttribute(_T("x"));
					y = oNode.GetAttribute(_T("y"));
				}
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:moveTo"));
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pt"));
				
				pWriter->EndNode(_T("a:moveTo"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_MOVETO);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x);
				pWriter->WriteString1(1, y);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			std::wstring x;
			std::wstring y;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual std::wstring GetODString()const
			{
                std::wstring str = L"<pt x=\"" + x + L"\" y=\"" + y + L"\" />";
				return _T("<moveTo>") + str + _T("</moveTo>"); 
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_MOVETO_INCLUDE_H_
