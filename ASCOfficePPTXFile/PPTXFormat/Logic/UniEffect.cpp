//#include "./stdafx.h"

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
		}

		UniEffect::~UniEffect()
		{
			//Effect.reset();
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

		void UniEffect::fromXML(XmlUtils::CXmlNode& node)
		{
			CString name = XmlUtils::GetNameNoNS(node.GetName());

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

		CString UniEffect::toXML() const
		{
			if (Effect.IsInit())
				return Effect->toXML();
			return _T("");
		}
	} // namespace Logic
} // namespace PPTX