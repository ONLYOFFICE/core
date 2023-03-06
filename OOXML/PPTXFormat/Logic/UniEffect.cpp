/*
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

		OOX::EElementType UniEffect::getType () const
		{
			if (Effect.is_init())
				return Effect->getType();
			else return OOX::et_Unknown;
		}

		std::wstring UniEffect::toXML() const
		{
			if (Effect.IsInit())
				return Effect->toXML();
			return _T("");
		}
		void UniEffect::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (Effect.is_init())
				Effect->toXmlWriter(pWriter);
		}
		void UniEffect::InitPointer(WrapperWritingElement* pElem)
		{
			Effect = pElem;
		}
		void UniEffect::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Effect->SetParentPointer(pParent);
		}
		void UniEffect::FillParentPointersForChilds()
		{
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
					if (name == _T("alphaCeiling"))	{ Effect.reset(CreatePtrXmlContent<Logic::AlphaCeiling>(oReader)); break; }	//1
					if (name == _T("alphaFloor"))	{ Effect.reset(CreatePtrXmlContent<Logic::AlphaFloor>(oReader)); break; }	//2
					if (name == _T("alphaRepl"))	{ Effect.reset(CreatePtrXmlContent<Logic::AlphaRepl>(oReader)); break; }		//3
					if (name == _T("alphaOutset"))	{ Effect.reset(CreatePtrXmlContent<Logic::AlphaOutset>(oReader)); break; }	//4
					if (name == _T("alphaModFix"))	{ Effect.reset(CreatePtrXmlContent<Logic::AlphaModFix>(oReader)); break; }	//5
					if (name == _T("alphaBiLevel"))	{ Effect.reset(CreatePtrXmlContent<Logic::AlphaBiLevel>(oReader)); break; }	//6
					if (name == _T("alphaInv"))		{ Effect.reset(CreatePtrXmlContent<Logic::AlphaInv>(oReader)); break; }		//7
					if (name == _T("alphaMod"))		{ Effect.reset(CreatePtrXmlContent<Logic::AlphaMod>(oReader)); break; }		//8
					break;
				}
			case 'b':
				{
					if (name == _T("blur"))		{ Effect.reset(CreatePtrXmlContent<Logic::Blur>(oReader)); break; }				//9
					if (name == _T("biLevel"))	{ Effect.reset(CreatePtrXmlContent<Logic::BiLevel>(oReader)); break; }			//10
					if (name == _T("blend"))	{ Effect.reset(CreatePtrXmlContent<Logic::Blend>(oReader)); break; }				//11
					break;
				}
			case 'c':
				{
					if (name == _T("cont"))		{ Effect.reset(CreatePtrXmlContent<Logic::EffectDag>(oReader)); break; }			//12
					if (name == _T("clrRepl"))	{ Effect.reset(CreatePtrXmlContent<Logic::ClrRepl>(oReader)); break; }			//13
					if (name == _T("clrChange")){ Effect.reset(CreatePtrXmlContent<Logic::ClrChange>(oReader)); break; }			//14
					break;
				}
			case 'd':
				{
					if (name == _T("duotone"))	{ Effect.reset(CreatePtrXmlContent<Logic::Duotone>(oReader)); break; }			//15
					break;
				}
			case 'e':
				{
					if (name == _T("effect"))	{ Effect.reset(CreatePtrXmlContent<Logic::EffectElement>(oReader)); break; }		//16
					break;
				}
			case 'f':
				{
					if (name == _T("fillOverlay")){ Effect.reset(CreatePtrXmlContent<Logic::FillOverlay>(oReader)); break; }		//17
					if (name == _T("fill"))		{ Effect.reset(CreatePtrXmlContent<Logic::FillEffect>(oReader)); break; }		//18
					break;
				}
			case 'g':
				{
					if (name == _T("glow"))		{ Effect.reset(CreatePtrXmlContent<Logic::Glow>(oReader)); break; }				//19
					if (name == _T("grayscl"))	{ Effect.reset(CreatePtrXmlContent<Logic::Grayscl>(oReader)); break; }			//20
					break;
				}
			case 'h':
				{
					if (name == _T("hsl"))		{ Effect.reset(CreatePtrXmlContent<Logic::HslEffect>(oReader)); break; }			//21
					break;
				}
			case 'i':
				{
					if (name == _T("innerShdw")){ Effect.reset(CreatePtrXmlContent<Logic::InnerShdw>(oReader)); break; }		//22
					break;
				}
			case 'l':
				{
					if (name == _T("lum"))	{ Effect.reset(CreatePtrXmlContent<Logic::LumEffect>(oReader)); break; }			//23
					break;
				}
			case 'o':
				{
					if (name == _T("outerShdw")){ Effect.reset(CreatePtrXmlContent<Logic::OuterShdw>(oReader)); break; }		//24
					break;
				}
			case 'p':
				{
					if (name == _T("prstShdw")){ Effect.reset(CreatePtrXmlContent<Logic::PrstShdw>(oReader)); break; }			//25
					break;
				}
			case 'r':
				{
					if (name == _T("reflection"))	{ Effect.reset(CreatePtrXmlContent<Logic::Reflection>(oReader)); break; }	//26
					if (name == _T("relOff"))		{ Effect.reset(CreatePtrXmlContent<Logic::RelOff>(oReader)); break; }		//27
					break;
				}
			case 's':
				{
					if (name == _T("softEdge"))		{ Effect.reset(CreatePtrXmlContent<Logic::SoftEdge>(oReader)); break; }		//28
					break;
				}
			case 't':
				{
					if (name == _T("tint"))			{ Effect.reset(CreatePtrXmlContent<Logic::TintEffect>(oReader)); break; }	//29
					break;
				}
			case 'x':
				{
					if (name == _T("xfrm"))			{ Effect.reset(CreatePtrXmlContent<Logic::XfrmEffect>(oReader)); break; }	//30
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
					if (name == _T("alphaCeiling")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaCeiling>(node)); break; }	//1
					if (name == _T("alphaFloor")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaFloor>(node)); break; }		//2
					if (name == _T("alphaRepl")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaRepl>(node)); break; }		//3
					if (name == _T("alphaOutset")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaOutset>(node)); break; }	//4
					if (name == _T("alphaModFix")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaModFix>(node)); break; }	//5
					if (name == _T("alphaBiLevel")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaBiLevel>(node)); break; }	//6
					if (name == _T("alphaInv")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaInv>(node)); break; }			//7
					if (name == _T("alphaMod")){ Effect.reset(CreatePtrXmlContent<Logic::AlphaMod>(node)); break; }			//8
					break;
				}
			case 'b':
				{
					if (name == _T("blur")){ Effect.reset(CreatePtrXmlContent<Logic::Blur>(node)); break; }					//9
					if (name == _T("biLevel")){ Effect.reset(CreatePtrXmlContent<Logic::BiLevel>(node)); break; }			//10
					if (name == _T("blend")){ Effect.reset(CreatePtrXmlContent<Logic::Blend>(node)); break; }				//11
					break;
				}
			case 'c':
				{
					if (name == _T("cont")){ Effect.reset(CreatePtrXmlContent<Logic::EffectDag>(node)); break; }				//12
					if (name == _T("clrRepl")){ Effect.reset(CreatePtrXmlContent<Logic::ClrRepl>(node)); break; }			//13
					if (name == _T("clrChange")){ Effect.reset(CreatePtrXmlContent<Logic::ClrChange>(node)); break; }		//14
					break;
				}
			case 'd':
				{
					if (name == _T("duotone")){ Effect.reset(CreatePtrXmlContent<Logic::Duotone>(node)); break; }			//15
					break;
				}
			case 'e':
				{
					if (name == _T("effect")){ Effect.reset(CreatePtrXmlContent<Logic::EffectElement>(node)); break; }		//16
					break;
				}
			case 'f':
				{
					if (name == _T("fillOverlay")){ Effect.reset(CreatePtrXmlContent<Logic::FillOverlay>(node)); break; }	//17
					if (name == _T("fill")){ Effect.reset(CreatePtrXmlContent<Logic::FillEffect>(node)); break; }			//18
					break;
				}
			case 'g':
				{
					if (name == _T("glow")){ Effect.reset(CreatePtrXmlContent<Logic::Glow>(node)); break; }					//19
					if (name == _T("grayscl")){ Effect.reset(CreatePtrXmlContent<Logic::Grayscl>(node)); break; }			//20
					break;
				}
			case 'h':
				{
					if (name == _T("hsl")){ Effect.reset(CreatePtrXmlContent<Logic::HslEffect>(node)); break; }				//21
					break;
				}
			case 'i':
				{
					if (name == _T("innerShdw")){ Effect.reset(CreatePtrXmlContent<Logic::InnerShdw>(node)); break; }		//22
					break;
				}
			case 'l':
				{
					if (name == _T("lum")){ Effect.reset(CreatePtrXmlContent<Logic::LumEffect>(node)); break; }				//23
					break;
				}
			case 'o':
				{
					if (name == _T("outerShdw")){ Effect.reset(CreatePtrXmlContent<Logic::OuterShdw>(node)); break; }		//24
					break;
				}
			case 'p':
				{
					if (name == _T("prstShdw")){ Effect.reset(CreatePtrXmlContent<Logic::PrstShdw>(node)); break; }			//25
					break;
				}
			case 'r':
				{
					if (name == _T("reflection")){ Effect.reset(CreatePtrXmlContent<Logic::Reflection>(node)); break; }		//26
					if (name == _T("relOff")){ Effect.reset(CreatePtrXmlContent<Logic::RelOff>(node)); break; }				//27
					break;
				}
			case 's':
				{
					if (name == _T("softEdge")){ Effect.reset(CreatePtrXmlContent<Logic::SoftEdge>(node)); break; }			//28
					break;
				}
			case 't':
				{
					if (name == _T("tint")){ Effect.reset(CreatePtrXmlContent<Logic::TintEffect>(node)); break; }			//29
					break;
				}
			case 'x':
				{
					if (name == _T("xfrm")){ Effect.reset(CreatePtrXmlContent<Logic::XfrmEffect>(node)); break; }			//30
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
				Effect.reset(CreatePtrXmlContent<Logic::OuterShdw>(oNode));				//1
			else if(element.GetNode(_T("a:glow"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::Glow>(oNode));					//2
			else if(element.GetNode(_T("a:duotone"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::Duotone>(oNode));				//3
			else if(element.GetNode(_T("a:xfrm"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::XfrmEffect>(oNode));				//4
			else if(element.GetNode(_T("a:blur"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::Blur>(oNode));					//5
			else if(element.GetNode(_T("a:prstShdw"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::PrstShdw>(oNode));				//6
			else if(element.GetNode(_T("a:innerShdw"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::InnerShdw>(oNode));				//7
			else if(element.GetNode(_T("a:reflection"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::Reflection>(oNode));				//8
			else if(element.GetNode(_T("a:softEdge"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::SoftEdge>(oNode));				//9
			else if(element.GetNode(_T("a:fillOverlay"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::FillOverlay>(oNode));			//10
			else if(element.GetNode(_T("a:alphaCeiling"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaCeiling>(oNode));			//11
			else if(element.GetNode(_T("a:alphaFloor"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaFloor>(oNode));				//12
			else if(element.GetNode(_T("a:tint"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::TintEffect>(oNode));				//13
			else if(element.GetNode(_T("a:relOff"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::RelOff>(oNode));					//14
			else if(element.GetNode(_T("a:lum"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::LumEffect>(oNode));				//15
			else if(element.GetNode(_T("a:hsl"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::HslEffect>(oNode));				//16
			else if(element.GetNode(_T("a:grayscl"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::Grayscl>(oNode));				//17
			else if(element.GetNode(_T("a:effect"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::EffectElement>(oNode));			//18
			else if(element.GetNode(_T("a:alphaRepl"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaRepl>(oNode));				//19
			else if(element.GetNode(_T("a:alphaOutset"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaOutset>(oNode));			//20
			else if(element.GetNode(_T("a:alphaModFix"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaModFix>(oNode));			//21
			else if(element.GetNode(_T("a:alphaBiLevel"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaBiLevel>(oNode));			//22
			else if(element.GetNode(_T("a:biLevel"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::BiLevel>(oNode));				//23
			else if(element.GetNode(_T("a:cont"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::EffectDag>(oNode));				//24
			else if(element.GetNode(_T("a:fill"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::FillEffect>(oNode));				//25
			else if(element.GetNode(_T("a:clrRepl"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::ClrRepl>(oNode));				//26
			else if(element.GetNode(_T("a:clrChange"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::ClrChange>(oNode));				//27
			else if(element.GetNode(_T("a:alphaInv"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaInv>(oNode));				//28
			else if(element.GetNode(_T("a:alphaMod"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::AlphaMod>(oNode));				//29
			else if(element.GetNode(_T("a:blend"), oNode))
				Effect.reset(CreatePtrXmlContent<Logic::Blend>(oNode));					//5
			else Effect.reset();
		}

	} // namespace Logic
} // namespace PPTX
