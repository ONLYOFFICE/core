﻿/*
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
#ifndef PPTX_LOGIC_ANIMMOTION_INCLUDE_H_
#define PPTX_LOGIC_ANIMMOTION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "./../../Limit/TLOrigin.h"
#include "./../../Limit/TLPathEditMode.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimMotion : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimMotion)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cBhvr = node.ReadNode(_T("p:cBhvr"));
				
				XmlUtils::CXmlNode oNodeBy;
				if (node.GetNode(_T("p:by"), oNodeBy))
				{
                    XmlMacroReadAttributeBase(oNodeBy, L"x", byX);
                    XmlMacroReadAttributeBase(oNodeBy, L"y", byY);
				}
				XmlUtils::CXmlNode oNodeFrom;
				if (node.GetNode(_T("p:from"), oNodeFrom))
				{
                    XmlMacroReadAttributeBase(oNodeFrom, L"x", fromX);
                    XmlMacroReadAttributeBase(oNodeFrom, L"y", fromY);
				}
				XmlUtils::CXmlNode oNodeTo;
				if (node.GetNode(_T("p:to"), oNodeTo))
				{
                    XmlMacroReadAttributeBase(oNodeTo, L"x", toX);
                    XmlMacroReadAttributeBase(oNodeTo, L"y", toY);
				}
				XmlUtils::CXmlNode oNodeCtr;
				if (node.GetNode(_T("p:rCtr"), oNodeCtr))
                {
                    XmlMacroReadAttributeBase(oNodeCtr, L"x", rCtrX);
                    XmlMacroReadAttributeBase(oNodeCtr, L"y", rCtrY);
				}

                XmlMacroReadAttributeBase(node, L"path", path);
                XmlMacroReadAttributeBase(node, L"ptsTypes", ptsTypes);
                XmlMacroReadAttributeBase(node, L"rAng", rAng);
                XmlMacroReadAttributeBase(node, L"origin", origin);
                XmlMacroReadAttributeBase(node, L"pathEditMode", pathEditMode);

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("origin"), origin);
				oAttr.Write(_T("path"), path);
				oAttr.WriteLimitNullable(_T("pathEditMode"), pathEditMode);
				oAttr.Write(_T("rAng"), rAng);
				oAttr.Write(_T("ptsTypes"), ptsTypes);

				XmlUtils::CNodeValue oValue;
				
				if (byX.IsInit() || byY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), byX);
					oAttr1.Write(_T("y"), byY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:by"), oAttr1);
				}
				if (fromX.IsInit() || fromY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), fromX);
					oAttr1.Write(_T("y"), fromY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:from"), oAttr1);
				}
				if (toX.IsInit() || toY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), toX);
					oAttr1.Write(_T("y"), toY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:to"), oAttr1);
				}
				
				oValue.Write(cBhvr);
				
				if (rCtrX.IsInit() || rCtrY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), rCtrX);
					oAttr1.Write(_T("y"), rCtrY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:rCtr"), oAttr1);
				}

				return XmlUtils::CreateNode(_T("p:animMotion"), oAttr, oValue);
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_p_animMotion;
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(toXML());
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					pWriter->WriteLimit2(0, origin);
					pWriter->WriteLimit2(1, pathEditMode);
					pWriter->WriteString2(2, path);
					pWriter->WriteString2(3, ptsTypes);
					pWriter->WriteInt2(4, byX);
					pWriter->WriteInt2(5, byY);
					pWriter->WriteInt2(6, fromX);
					pWriter->WriteInt2(7, fromY);				
					pWriter->WriteInt2(8, toX);
					pWriter->WriteInt2(9, toY);
					pWriter->WriteInt2(10, rCtrX);
					pWriter->WriteInt2(11, rCtrY);
					pWriter->WriteInt2(12, rAng);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, cBhvr);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // attribute start
				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					else if (0 == _at)	origin = pReader->GetUChar();
					else if (1 == _at)	pathEditMode = pReader->GetUChar();
					else if (2 == _at)	path = pReader->GetString2();
					else if (3 == _at)	ptsTypes = pReader->GetString2();
					else if (4 == _at)	byX = pReader->GetLong();
					else if (5 == _at)	byY = pReader->GetLong();
					else if (6 == _at)	fromX = pReader->GetLong();
					else if (7 == _at)	fromY = pReader->GetLong();
					else if (8 == _at)	toX = pReader->GetLong();
					else if (9 == _at)	toY = pReader->GetLong();
					else if (10 == _at)	rCtrX = pReader->GetLong();
					else if (11 == _at)	rCtrY = pReader->GetLong();
					else if (12 == _at)	rAng = pReader->GetLong();
				}
				while (pReader->GetPos() < end)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							cBhvr.fromPPTY(pReader);
						}break;
						default:
						{
							pReader->SkipRecord();
						}break;
					}
				}
				pReader->Seek(end);
			}

			CBhvr					cBhvr;

			nullable_int			byX;
			nullable_int			byY;
			nullable_int			fromX;
			nullable_int			fromY;
			nullable_int			toX;
			nullable_int			toY;
			nullable_int			rCtrX;
			nullable_int			rCtrY;

			nullable_limit<Limit::TLOrigin>			origin; //ST_TLAnimateMotionBehaviorOrigin
			nullable_string							path;	//M = move to, L = line to, C = curve to, Z=close loop, E=end 
															//UPPERCASE = absolute coords, lowercase = relative coords 
															//Thus total allowed set = {M,L,C,Z,E,m,l,c,z,e) 
															//W3C XML Schema string 
			nullable_limit<Limit::TLPathEditMode>	pathEditMode;	//ST_TLAnimateMotionPathEditMode
			nullable_string							ptsTypes;		//A = Auto, F = Corner, T = Straight, S = Smooth 
																	//UPPERCASE = Straight Line follows point, lowercase = curve follows point. 
																	//Thus, the total allowed set = {A,F,T,S,a,f,t,s} 
																	//W3C XML Schema string 
			nullable_int			rAng;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMMOTION_INCLUDE_H
