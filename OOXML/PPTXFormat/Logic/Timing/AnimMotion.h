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

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual OOX::EElementType getType() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			CBhvr					cBhvr;

			nullable_int			byX;
			nullable_int			byY;
			nullable_int			fromX;
			nullable_int			fromY;
			nullable_int			toX;
			nullable_int			toY;
			nullable_int			rCtrX;
			nullable_int			rCtrY;

			nullable_limit<Limit::TLOrigin>			origin;			//ST_TLAnimateMotionBehaviorOrigin
			nullable_string							path;			//M = move to, L = line to, C = curve to, Z=close loop, E=end
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
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMMOTION_INCLUDE_H
