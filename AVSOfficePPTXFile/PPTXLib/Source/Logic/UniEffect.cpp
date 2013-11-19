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

		UniEffect::UniEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const UniEffect& UniEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void UniEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if(name == "") return;
			switch(name[0])
			{
			case 'a':
				{
					if (name == "alphaCeiling"){ Effect.reset(new Logic::AlphaCeiling(node)); break; }	//1
					if (name == "alphaFloor"){ Effect.reset(new Logic::AlphaFloor(node)); break; }		//2
					if (name == "alphaRepl"){ Effect.reset(new Logic::AlphaRepl(node)); break; }		//3
					if (name == "alphaOutset"){ Effect.reset(new Logic::AlphaOutset(node)); break; }	//4
					if (name == "alphaModFix"){ Effect.reset(new Logic::AlphaModFix(node)); break; }	//5
					if (name == "alphaBiLevel"){ Effect.reset(new Logic::AlphaBiLevel(node)); break; }	//6
					if (name == "alphaInv"){ Effect.reset(new Logic::AlphaInv(node)); break; }			//7
					if (name == "alphaMod"){ Effect.reset(new Logic::AlphaMod(node)); break; }			//8
					break;
				}
			case 'b':
				{
					if (name == "blur"){ Effect.reset(new Logic::Blur(node)); break; }					//9
					if (name == "biLevel"){ Effect.reset(new Logic::BiLevel(node)); break; }			//10
					if (name == "blend"){ Effect.reset(new Logic::Blend(node)); break; }				//11
					break;
				}
			case 'c':
				{
					if (name == "cont"){ Effect.reset(new Logic::EffectDag(node)); break; }				//12
					if (name == "clrRepl"){ Effect.reset(new Logic::ClrRepl(node)); break; }			//13
					if (name == "clrChange"){ Effect.reset(new Logic::ClrChange(node)); break; }		//14
					break;
				}
			case 'd':
				{
					if (name == "duotone"){ Effect.reset(new Logic::Duotone(node)); break; }			//15
					break;
				}
			case 'e':
				{
					if (name == "effect"){ Effect.reset(new Logic::EffectElement(node)); break; }		//16
					break;
				}
			case 'f':
				{
					if (name == "fillOverlay"){ Effect.reset(new Logic::FillOverlay(node)); break; }	//17
					if (name == "fill"){ Effect.reset(new Logic::FillEffect(node)); break; }			//18
					break;
				}
			case 'g':
				{
					if (name == "glow"){ Effect.reset(new Logic::Glow(node)); break; }					//19
					if (name == "grayscl"){ Effect.reset(new Logic::Grayscl(node)); break; }			//20
					break;
				}
			case 'h':
				{
					if (name == "hsl"){ Effect.reset(new Logic::HslEffect(node)); break; }				//21
					break;
				}
			case 'i':
				{
					if (name == "innerShdw"){ Effect.reset(new Logic::InnerShdw(node)); break; }		//22
					break;
				}
			case 'l':
				{
					if (name == "lum"){ Effect.reset(new Logic::LumEffect(node)); break; }				//23
					break;
				}
			case 'o':
				{
					if (name == "outerShdw"){ Effect.reset(new Logic::OuterShdw(node)); break; }		//24
					break;
				}
			case 'p':
				{
					if (name == "prstShdw"){ Effect.reset(new Logic::PrstShdw(node)); break; }			//25
					break;
				}
			case 'r':
				{
					if (name == "reflection"){ Effect.reset(new Logic::Reflection(node)); break; }		//26
					if (name == "relOff"){ Effect.reset(new Logic::RelOff(node)); break; }				//27
					break;
				}
			case 's':
				{
					if (name == "softEdge"){ Effect.reset(new Logic::SoftEdge(node)); break; }			//28
					break;
				}
			case 't':
				{
					if (name == "tint"){ Effect.reset(new Logic::TintEffect(node)); break; }			//29
					break;
				}
			case 'x':
				{
					if (name == "xfrm"){ Effect.reset(new Logic::XfrmEffect(node)); break; }			//30
					break;
				}
			default:
				{
					Effect.reset();
					break;
				}
			}
		}

		void UniEffect::GetEffectFrom(const XML::XElement& element){
			if(element.element("outerShdw").exist())
				Effect.reset(new Logic::OuterShdw(element.element("outerShdw")));			//1
			else if(element.element("glow").exist())
				Effect.reset(new Logic::Glow(element.element("glow")));						//2
			else if(element.element("duotone").exist())
				Effect.reset(new Logic::Duotone(element.element("duotone")));				//3
			else if(element.element("xfrm").exist())
				Effect.reset(new Logic::XfrmEffect(element.element("xfrm")));				//4
			else if(element.element("blur").exist())
				Effect.reset(new Logic::Blur(element.element("blur")));						//5
			else if(element.element("prstShdw").exist())
				Effect.reset(new Logic::PrstShdw(element.element("prstShdw")));				//6
			else if(element.element("innerShdw").exist())
				Effect.reset(new Logic::InnerShdw(element.element("innerShdw")));			//7
			else if(element.element("reflection").exist())
				Effect.reset(new Logic::Reflection(element.element("reflection")));			//8
			else if(element.element("softEdge").exist())
				Effect.reset(new Logic::SoftEdge(element.element("softEdge")));				//9
			else if(element.element("fillOverlay").exist())
				Effect.reset(new Logic::FillOverlay(element.element("fillOverlay")));		//10
			else if(element.element("alphaCeiling").exist())
				Effect.reset(new Logic::AlphaCeiling(element.element("alphaCeiling")));		//11
			else if(element.element("alphaFloor").exist())
				Effect.reset(new Logic::AlphaFloor(element.element("alphaFloor")));			//12
			else if(element.element("tint").exist())
				Effect.reset(new Logic::TintEffect(element.element("tint")));				//13
			else if(element.element("relOff").exist())
				Effect.reset(new Logic::RelOff(element.element("relOff")));					//14
			else if(element.element("lum").exist())
				Effect.reset(new Logic::LumEffect(element.element("lum")));					//15
			else if(element.element("hsl").exist())
				Effect.reset(new Logic::HslEffect(element.element("hsl")));					//16
			else if(element.element("grayscl").exist())
				Effect.reset(new Logic::Grayscl(element.element("grayscl")));				//17
			else if(element.element("effect").exist())
				Effect.reset(new Logic::EffectElement(element.element("effect")));			//18
			else if(element.element("alphaRepl").exist())
				Effect.reset(new Logic::AlphaRepl(element.element("alphaRepl")));			//19
			else if(element.element("alphaOutset").exist())
				Effect.reset(new Logic::AlphaOutset(element.element("alphaOutset")));		//20
			else if(element.element("alphaModFix").exist())
				Effect.reset(new Logic::AlphaModFix(element.element("alphaModFix")));		//21
			else if(element.element("alphaBiLevel").exist())
				Effect.reset(new Logic::AlphaBiLevel(element.element("alphaBiLevel")));		//22
			else if(element.element("biLevel").exist())
				Effect.reset(new Logic::BiLevel(element.element("biLevel")));				//23
			else if(element.element("cont").exist())
				Effect.reset(new Logic::EffectDag(element.element("cont")));				//24
			else if(element.element("fill").exist())
				Effect.reset(new Logic::FillEffect(element.element("fill")));				//25
			else if(element.element("clrRepl").exist())
				Effect.reset(new Logic::ClrRepl(element.element("clrRepl")));				//26
			else if(element.element("clrChange").exist())
				Effect.reset(new Logic::ClrChange(element.element("clrChange")));			//27
			else if(element.element("alphaInv").exist())
				Effect.reset(new Logic::AlphaInv(element.element("alphaInv")));				//28
			else if(element.element("alphaMod").exist())
				Effect.reset(new Logic::AlphaMod(element.element("alphaMod")));				//29
			else if(element.element("blend").exist())
				Effect.reset(new Logic::Blend(element.element("blend")));						//5
			else Effect.reset();
		}

		const XML::XNode UniEffect::toXML() const
		{
			return XML::Write(Effect);
		}
	} // namespace Logic
} // namespace PPTX