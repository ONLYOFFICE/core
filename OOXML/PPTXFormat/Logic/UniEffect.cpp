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


#include "UniEffect.h"
#include "Effects/OuterShdw.h"		//1
#include "Effects/Glow.h"			//2
#include "Effects/Duotone.h"		//3
#include "Effects/XfrmEffect.h"		//4
#include "Effects/Blur.h"			//5
#include "Effects/PrstShdw.h"		//6
#include "Effects/InnerShdw.h"		//7
#include "Effects/Reflection.h"		//8
#include "Effects/SoftEdge.h"		//9
#include "Effects/FillOverlay.h"	//10
#include "Effects/AlphaCeiling.h"	//11
#include "Effects/AlphaFloor.h"		//12
#include "Effects/TintEffect.h"		//13
#include "Effects/RelOff.h"			//14
#include "Effects/LumEffect.h"		//15
#include "Effects/HslEffect.h"		//16
#include "Effects/Grayscl.h"		//17
#include "Effects/EffectElement.h"	//18
#include "Effects/AlphaRepl.h"		//19
#include "Effects/AlphaOutset.h"	//20
#include "Effects/AlphaModFix.h"	//21
#include "Effects/AlphaBiLevel.h"	//22
#include "Effects/BiLevel.h"		//23
#include "EffectDag.h"				//24
#include "Effects/FillEffect.h"		//25
#include "Effects/ClrRepl.h"		//26
#include "Effects/ClrChange.h"		//27
#include "Effects/AlphaInv.h"		//28
#include "Effects/AlphaMod.h"		//29
#include "Effects/Blend.h"			//30

namespace PPTX
{
	namespace Logic
	{
		UniEffect::UniEffect()
		{
			Effect.reset();
		}

		UniEffect::~UniEffect()
		{
		}
		void UniEffect::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Effect.is_init())
				Effect->toPPTY(pWriter);
		}
		void UniEffect::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG pos = pReader->GetPos();
			ULONG rec_len = pReader->GetULong();
			if (0 == rec_len)
				return;
			
			BYTE rec = pReader->GetUChar();

			switch(rec)
			{
			case EFFECT_TYPE_ALPHAMODFIX:	Effect = new PPTX::Logic::AlphaModFix(); break;
			case EFFECT_TYPE_DUOTONE:		Effect = new PPTX::Logic::Duotone(); break;
			case EFFECT_TYPE_OUTERSHDW:		Effect = new PPTX::Logic::OuterShdw(); break;
			case EFFECT_TYPE_GLOW:			Effect = new PPTX::Logic::Glow(); break;
			case EFFECT_TYPE_XFRM:			Effect = new PPTX::Logic::XfrmEffect(); break;
			case EFFECT_TYPE_BLUR:			Effect = new PPTX::Logic::Blur(); break;
			case EFFECT_TYPE_PRSTSHDW:		Effect = new PPTX::Logic::PrstShdw(); break;
			case EFFECT_TYPE_INNERSHDW:		Effect = new PPTX::Logic::InnerShdw(); break;
			case EFFECT_TYPE_REFLECTION:	Effect = new PPTX::Logic::Reflection(); break;
			case EFFECT_TYPE_SOFTEDGE:		Effect = new PPTX::Logic::SoftEdge(); break;
			case EFFECT_TYPE_FILLOVERLAY:	Effect = new PPTX::Logic::FillOverlay(); break;
			case EFFECT_TYPE_ALPHACEILING:	Effect = new PPTX::Logic::AlphaCeiling(); break;
			case EFFECT_TYPE_ALPHAFLOOR:	Effect = new PPTX::Logic::AlphaFloor(); break;
			case EFFECT_TYPE_TINTEFFECT:	Effect = new PPTX::Logic::TintEffect(); break;
			case EFFECT_TYPE_RELOFF:		Effect = new PPTX::Logic::RelOff(); break;
			case EFFECT_TYPE_LUM:			Effect = new PPTX::Logic::LumEffect(); break;
			case EFFECT_TYPE_HSL:			Effect = new PPTX::Logic::HslEffect(); break;
			case EFFECT_TYPE_GRAYSCL:		Effect = new PPTX::Logic::Grayscl(); break;
			case EFFECT_TYPE_ALPHAREPL:		Effect = new PPTX::Logic::AlphaRepl(); break;
			case EFFECT_TYPE_ALPHAOUTSET:	Effect = new PPTX::Logic::AlphaOutset(); break;
			case EFFECT_TYPE_ALPHABILEVEL:	Effect = new PPTX::Logic::AlphaBiLevel(); break;
			case EFFECT_TYPE_BILEVEL:		Effect = new PPTX::Logic::BiLevel(); break;
			case EFFECT_TYPE_FILL:			Effect = new PPTX::Logic::FillEffect(); break;
			case EFFECT_TYPE_CLRREPL:		Effect = new PPTX::Logic::ClrRepl(); break;
			case EFFECT_TYPE_CLRCHANGE:		Effect = new PPTX::Logic::ClrChange(); break;
			case EFFECT_TYPE_ALPHAINV:		Effect = new PPTX::Logic::AlphaInv(); break;
			case EFFECT_TYPE_ALPHAMOD:		Effect = new PPTX::Logic::AlphaMod(); break;
			case EFFECT_TYPE_BLEND:			Effect = new PPTX::Logic::Blend(); break;
			}
			
			pReader->Seek(pos);
			if (Effect.is_init())
			{
				Effect->fromPPTY(pReader);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		UniEffect::UniEffect(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const UniEffect& UniEffect::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void UniEffect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
            std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());
			
			switch ((int)name[0])
			{
			case 'a':
				{
					if (name == _T("alphaCeiling"))	{ Effect.reset(new Logic::AlphaCeiling(oReader)); break; }	//1
					if (name == _T("alphaFloor"))	{ Effect.reset(new Logic::AlphaFloor(oReader)); break; }	//2
					if (name == _T("alphaRepl"))	{ Effect.reset(new Logic::AlphaRepl(oReader)); break; }		//3
					if (name == _T("alphaOutset"))	{ Effect.reset(new Logic::AlphaOutset(oReader)); break; }	//4
					if (name == _T("alphaModFix"))	{ Effect.reset(new Logic::AlphaModFix(oReader)); break; }	//5
					if (name == _T("alphaBiLevel"))	{ Effect.reset(new Logic::AlphaBiLevel(oReader)); break; }	//6
					if (name == _T("alphaInv"))		{ Effect.reset(new Logic::AlphaInv(oReader)); break; }		//7
					if (name == _T("alphaMod"))		{ Effect.reset(new Logic::AlphaMod(oReader)); break; }		//8
					break;
				}
			case 'b':
				{
					if (name == _T("blur"))		{ Effect.reset(new Logic::Blur(oReader)); break; }				//9
					if (name == _T("biLevel"))	{ Effect.reset(new Logic::BiLevel(oReader)); break; }			//10
					if (name == _T("blend"))	{ Effect.reset(new Logic::Blend(oReader)); break; }				//11
					break;
				}
			case 'c':
				{
					if (name == _T("cont"))		{ Effect.reset(new Logic::EffectDag(oReader)); break; }			//12
					if (name == _T("clrRepl"))	{ Effect.reset(new Logic::ClrRepl(oReader)); break; }			//13
					if (name == _T("clrChange")){ Effect.reset(new Logic::ClrChange(oReader)); break; }			//14
					break;
				}
			case 'd':
				{
					if (name == _T("duotone"))	{ Effect.reset(new Logic::Duotone(oReader)); break; }			//15
					break;
				}
			case 'e':
				{
					if (name == _T("effect"))	{ Effect.reset(new Logic::EffectElement(oReader)); break; }		//16
					break;
				}
			case 'f':
				{
					if (name == _T("fillOverlay")){ Effect.reset(new Logic::FillOverlay(oReader)); break; }		//17
					if (name == _T("fill"))		{ Effect.reset(new Logic::FillEffect(oReader)); break; }		//18
					break;
				}
			case 'g':
				{
					if (name == _T("glow"))		{ Effect.reset(new Logic::Glow(oReader)); break; }				//19
					if (name == _T("grayscl"))	{ Effect.reset(new Logic::Grayscl(oReader)); break; }			//20
					break;
				}
			case 'h':
				{
					if (name == _T("hsl"))		{ Effect.reset(new Logic::HslEffect(oReader)); break; }			//21
					break;
				}
			case 'i':
				{
					if (name == _T("innerShdw")){ Effect.reset(new Logic::InnerShdw(oReader)); break; }		//22
					break;
				}
			case 'l':
				{
					if (name == _T("lum"))	{ Effect.reset(new Logic::LumEffect(oReader)); break; }			//23
					break;
				}
			case 'o':
				{
					if (name == _T("outerShdw")){ Effect.reset(new Logic::OuterShdw(oReader)); break; }		//24
					break;
				}
			case 'p':
				{
					if (name == _T("prstShdw")){ Effect.reset(new Logic::PrstShdw(oReader)); break; }			//25
					break;
				}
			case 'r':
				{
					if (name == _T("reflection"))	{ Effect.reset(new Logic::Reflection(oReader)); break; }	//26
					if (name == _T("relOff"))		{ Effect.reset(new Logic::RelOff(oReader)); break; }		//27
					break;
				}
			case 's':
				{
					if (name == _T("softEdge"))		{ Effect.reset(new Logic::SoftEdge(oReader)); break; }		//28
					break;
				}
			case 't':
				{
					if (name == _T("tint"))			{ Effect.reset(new Logic::TintEffect(oReader)); break; }	//29
					break;
				}
			case 'x':
				{
					if (name == _T("xfrm"))			{ Effect.reset(new Logic::XfrmEffect(oReader)); break; }	//30
					break;
				}
			}
		}
		void UniEffect::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("")) 
				return;
			switch ((int)name[0])
			{
			case 'a':
				{
					if (name == _T("alphaCeiling")){ Effect.reset(new Logic::AlphaCeiling(node)); break; }	//1
					if (name == _T("alphaFloor")){ Effect.reset(new Logic::AlphaFloor(node)); break; }		//2
					if (name == _T("alphaRepl")){ Effect.reset(new Logic::AlphaRepl(node)); break; }		//3
					if (name == _T("alphaOutset")){ Effect.reset(new Logic::AlphaOutset(node)); break; }	//4
					if (name == _T("alphaModFix")){ Effect.reset(new Logic::AlphaModFix(node)); break; }	//5
					if (name == _T("alphaBiLevel")){ Effect.reset(new Logic::AlphaBiLevel(node)); break; }	//6
					if (name == _T("alphaInv")){ Effect.reset(new Logic::AlphaInv(node)); break; }			//7
					if (name == _T("alphaMod")){ Effect.reset(new Logic::AlphaMod(node)); break; }			//8
					break;
				}
			case 'b':
				{
					if (name == _T("blur")){ Effect.reset(new Logic::Blur(node)); break; }					//9
					if (name == _T("biLevel")){ Effect.reset(new Logic::BiLevel(node)); break; }			//10
					if (name == _T("blend")){ Effect.reset(new Logic::Blend(node)); break; }				//11
					break;
				}
			case 'c':
				{
					if (name == _T("cont")){ Effect.reset(new Logic::EffectDag(node)); break; }				//12
					if (name == _T("clrRepl")){ Effect.reset(new Logic::ClrRepl(node)); break; }			//13
					if (name == _T("clrChange")){ Effect.reset(new Logic::ClrChange(node)); break; }		//14
					break;
				}
			case 'd':
				{
					if (name == _T("duotone")){ Effect.reset(new Logic::Duotone(node)); break; }			//15
					break;
				}
			case 'e':
				{
					if (name == _T("effect")){ Effect.reset(new Logic::EffectElement(node)); break; }		//16
					break;
				}
			case 'f':
				{
					if (name == _T("fillOverlay")){ Effect.reset(new Logic::FillOverlay(node)); break; }	//17
					if (name == _T("fill")){ Effect.reset(new Logic::FillEffect(node)); break; }			//18
					break;
				}
			case 'g':
				{
					if (name == _T("glow")){ Effect.reset(new Logic::Glow(node)); break; }					//19
					if (name == _T("grayscl")){ Effect.reset(new Logic::Grayscl(node)); break; }			//20
					break;
				}
			case 'h':
				{
					if (name == _T("hsl")){ Effect.reset(new Logic::HslEffect(node)); break; }				//21
					break;
				}
			case 'i':
				{
					if (name == _T("innerShdw")){ Effect.reset(new Logic::InnerShdw(node)); break; }		//22
					break;
				}
			case 'l':
				{
					if (name == _T("lum")){ Effect.reset(new Logic::LumEffect(node)); break; }				//23
					break;
				}
			case 'o':
				{
					if (name == _T("outerShdw")){ Effect.reset(new Logic::OuterShdw(node)); break; }		//24
					break;
				}
			case 'p':
				{
					if (name == _T("prstShdw")){ Effect.reset(new Logic::PrstShdw(node)); break; }			//25
					break;
				}
			case 'r':
				{
					if (name == _T("reflection")){ Effect.reset(new Logic::Reflection(node)); break; }		//26
					if (name == _T("relOff")){ Effect.reset(new Logic::RelOff(node)); break; }				//27
					break;
				}
			case 's':
				{
					if (name == _T("softEdge")){ Effect.reset(new Logic::SoftEdge(node)); break; }			//28
					break;
				}
			case 't':
				{
					if (name == _T("tint")){ Effect.reset(new Logic::TintEffect(node)); break; }			//29
					break;
				}
			case 'x':
				{
					if (name == _T("xfrm")){ Effect.reset(new Logic::XfrmEffect(node)); break; }			//30
					break;
				}
			default:
				{
					Effect.reset();
					break;
				}
			}
		}

		void UniEffect::GetEffectFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			
			if(element.GetNode(_T("a:outerShdw"), oNode))
				Effect.reset(new Logic::OuterShdw(oNode));				//1
			else if(element.GetNode(_T("a:glow"), oNode))
				Effect.reset(new Logic::Glow(oNode));					//2
			else if(element.GetNode(_T("a:duotone"), oNode))
				Effect.reset(new Logic::Duotone(oNode));				//3
			else if(element.GetNode(_T("a:xfrm"), oNode))
				Effect.reset(new Logic::XfrmEffect(oNode));				//4
			else if(element.GetNode(_T("a:blur"), oNode))
				Effect.reset(new Logic::Blur(oNode));					//5
			else if(element.GetNode(_T("a:prstShdw"), oNode))
				Effect.reset(new Logic::PrstShdw(oNode));				//6
			else if(element.GetNode(_T("a:innerShdw"), oNode))
				Effect.reset(new Logic::InnerShdw(oNode));				//7
			else if(element.GetNode(_T("a:reflection"), oNode))
				Effect.reset(new Logic::Reflection(oNode));				//8
			else if(element.GetNode(_T("a:softEdge"), oNode))
				Effect.reset(new Logic::SoftEdge(oNode));				//9
			else if(element.GetNode(_T("a:fillOverlay"), oNode))
				Effect.reset(new Logic::FillOverlay(oNode));			//10
			else if(element.GetNode(_T("a:alphaCeiling"), oNode))
				Effect.reset(new Logic::AlphaCeiling(oNode));			//11
			else if(element.GetNode(_T("a:alphaFloor"), oNode))
				Effect.reset(new Logic::AlphaFloor(oNode));				//12
			else if(element.GetNode(_T("a:tint"), oNode))
				Effect.reset(new Logic::TintEffect(oNode));				//13
			else if(element.GetNode(_T("a:relOff"), oNode))
				Effect.reset(new Logic::RelOff(oNode));					//14
			else if(element.GetNode(_T("a:lum"), oNode))
				Effect.reset(new Logic::LumEffect(oNode));				//15
			else if(element.GetNode(_T("a:hsl"), oNode))
				Effect.reset(new Logic::HslEffect(oNode));				//16
			else if(element.GetNode(_T("a:grayscl"), oNode))
				Effect.reset(new Logic::Grayscl(oNode));				//17
			else if(element.GetNode(_T("a:effect"), oNode))
				Effect.reset(new Logic::EffectElement(oNode));			//18
			else if(element.GetNode(_T("a:alphaRepl"), oNode))
				Effect.reset(new Logic::AlphaRepl(oNode));				//19
			else if(element.GetNode(_T("a:alphaOutset"), oNode))
				Effect.reset(new Logic::AlphaOutset(oNode));			//20
			else if(element.GetNode(_T("a:alphaModFix"), oNode))
				Effect.reset(new Logic::AlphaModFix(oNode));			//21
			else if(element.GetNode(_T("a:alphaBiLevel"), oNode))
				Effect.reset(new Logic::AlphaBiLevel(oNode));			//22
			else if(element.GetNode(_T("a:biLevel"), oNode))
				Effect.reset(new Logic::BiLevel(oNode));				//23
			else if(element.GetNode(_T("a:cont"), oNode))
				Effect.reset(new Logic::EffectDag(oNode));				//24
			else if(element.GetNode(_T("a:fill"), oNode))
				Effect.reset(new Logic::FillEffect(oNode));				//25
			else if(element.GetNode(_T("a:clrRepl"), oNode))
				Effect.reset(new Logic::ClrRepl(oNode));				//26
			else if(element.GetNode(_T("a:clrChange"), oNode))
				Effect.reset(new Logic::ClrChange(oNode));				//27
			else if(element.GetNode(_T("a:alphaInv"), oNode))
				Effect.reset(new Logic::AlphaInv(oNode));				//28
			else if(element.GetNode(_T("a:alphaMod"), oNode))
				Effect.reset(new Logic::AlphaMod(oNode));				//29
			else if(element.GetNode(_T("a:blend"), oNode))
				Effect.reset(new Logic::Blend(oNode));					//5
			else Effect.reset();
		}

	} // namespace Logic
} // namespace PPTX