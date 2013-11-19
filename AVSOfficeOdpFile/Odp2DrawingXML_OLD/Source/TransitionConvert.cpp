#include "stdafx.h"
#include "./../Converter.h"
#include "../OdpFormat/Source/Content/Slide.h"
#include "../OdpFormat/Source/Content/Animation/Par.h"

namespace Odp2DrawingXML
{
	void Converter::TransitionConvert(const DWORD SldNum, CDrawingDocument& pDocument)const
	{
		bool converted = false;
		const Odp::Content::Slide& pSlide = m_Folder->Content->body->Slides->at(SldNum);
		std::string SlideId = pSlide.Id.get_value_or("");
		if((SlideId != "") && (pSlide.Animation.is_init()))
		{
			for(size_t i = 0; i < pSlide.Animation->Childs->size(); i++)
			{
				if(pSlide.Animation->Childs->at(i).is<Odp::Content::Par>())
				{
					if(pSlide.Animation->Childs->at(i).as<Odp::Content::Par>().Begin.get_value_or("") == SlideId + ".begin")
					{
						for(size_t j = 0; j < pSlide.Animation->Childs->at(i).as<Odp::Content::Par>().Childs->size(); j++)
						{
							if(pSlide.Animation->Childs->at(i).as<Odp::Content::Par>().Childs->at(j).is<Odp::Content::TransitionFilter>())
							{
								TransitionParse(pSlide.Animation->Childs->at(i).as<Odp::Content::Par>().Childs->at(j).as<Odp::Content::TransitionFilter>(),
									SldNum, SlideId, pDocument);
								converted = true;
								break;
							}
						}
					}
				}
				if(converted)
					break;
			}
		}
	}

	void Converter::TransitionParse(const Odp::Content::TransitionFilter& pTransition, const DWORD SldNum, const std::string& SlideId, CDrawingDocument& pDocument)const
	{
		std::string type = pTransition.Type.get_value_or("");
		if(type != "")
		{
			ODCSlideShowSlideInfoAtom lDestTransition;
			std::string subtype = pTransition.SubType.get_value_or("");
			std::string direction = pTransition.Direction.get_value_or("forward");
			if(type == "barWipe")
			{
				if(subtype == "fadeOverColor")
				{
					lDestTransition.m_nEffectType = 0;
					lDestTransition.m_nEffectDirection = 1;
				}
				else
				{
					lDestTransition.m_nEffectType = 10;
					if(subtype == "leftToRight")
						lDestTransition.m_nEffectDirection = 0;
					else
						lDestTransition.m_nEffectDirection = 1;
					if(direction == "reverse")
						lDestTransition.m_nEffectDirection += 2;
				}
			}
			else if(type == "pinWheelWipe")
			{
				lDestTransition.m_nEffectType = 26;
				if(subtype == "oneBlade")
					lDestTransition.m_nEffectDirection = 1;
				else if(subtype == "twoBladeVertical")
					lDestTransition.m_nEffectDirection = 2;
				else if(subtype == "threeBlade")
					lDestTransition.m_nEffectDirection = 3;
				else if(subtype == "fourBlade")
					lDestTransition.m_nEffectDirection = 4;
				else if(subtype == "eightBlade")
					lDestTransition.m_nEffectDirection = 8;
				else lDestTransition.m_nEffectDirection = 2;
			}
			else if(type == "slideWipe")
			{
				if(direction == "reverse")
					lDestTransition.m_nEffectType = 4;
				else
					lDestTransition.m_nEffectType = 7;

				if(subtype == "fromTop")
					lDestTransition.m_nEffectDirection = 1;
				else if(subtype == "fromRight")
					lDestTransition.m_nEffectDirection = 2;
				else if(subtype == "fromLeft")
					lDestTransition.m_nEffectDirection = 0;
				else if(subtype == "fromBottom")
					lDestTransition.m_nEffectDirection = 3;
				else if(subtype == "fromTopRight")
					lDestTransition.m_nEffectDirection = 5;
				else if(subtype == "fromBottomRight")
					lDestTransition.m_nEffectDirection = 7;
				else if(subtype == "fromTopLeft")
					lDestTransition.m_nEffectDirection = 4;
				else if(subtype == "fromBottomLeft")
					lDestTransition.m_nEffectDirection = 6;
				else lDestTransition.m_nEffectDirection = 0;
			}
			else if(type == "randomBarWipe")
			{
				lDestTransition.m_nEffectType = 8;
				lDestTransition.m_nEffectDirection = (subtype == "vertical")? 1 : 0;
			}
			else if(type == "checkerBoardWipe")
			{
				lDestTransition.m_nEffectType = 3;
				lDestTransition.m_nEffectDirection = (subtype == "down")? 1 : 0;
			}
			else if((type == "fourBoxWipe") && (subtype == "cornersOut"))
			{
				lDestTransition.m_nEffectType = 18;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if((type == "irisWipe") && (subtype == "diamond"))
			{
				lDestTransition.m_nEffectType = 17;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if((type == "ellipseWipe") && (subtype == "circle"))
			{
				lDestTransition.m_nEffectType = 27;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if((type == "irisWipe") && (subtype == "rectangle"))
			{
				lDestTransition.m_nEffectType = 13;
				lDestTransition.m_nEffectDirection = (direction == "reverse")?1:0;
			}
			else if((type == "fanWipe") && (subtype == "centerTop"))
			{
				lDestTransition.m_nEffectType = 19;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(type == "blindsWipe")
			{
				lDestTransition.m_nEffectType = 2;
				lDestTransition.m_nEffectDirection = (subtype == "vertical")?1:0;
			}
			else if(type == "fade")// && (subtype == "fadeOverColor")
			{
				lDestTransition.m_nEffectType = 6;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(type == "dissolve")
			{
				lDestTransition.m_nEffectType = 5;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(type == "random")
			{
				lDestTransition.m_nEffectType = 1;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(type == "pushWipe")
			{
				lDestTransition.m_nEffectType = 21;
				lDestTransition.m_nEffectDirection = (subtype == "combVertical")?1:0;
				if(subtype == "fromTop")
				{
					lDestTransition.m_nEffectType = 20;
					lDestTransition.m_nEffectDirection = 1;
				}
				else if(subtype == "fromRight")
				{
					lDestTransition.m_nEffectType = 20;
					lDestTransition.m_nEffectDirection = 2;
				}
				else if(subtype == "fromLeft")
				{
					lDestTransition.m_nEffectType = 20;
					lDestTransition.m_nEffectDirection = 0;
				}
				else if(subtype == "fromBottom")
				{
					lDestTransition.m_nEffectType = 20;
					lDestTransition.m_nEffectDirection = 3;
				}
			}
			else if(type == "waterfallWipe")
			{
				lDestTransition.m_nEffectType = 9;

				if((subtype == "horizontalRight") && (direction == "reverse"))
					lDestTransition.m_nEffectDirection = 4;
				else if((subtype == "horizontalLeft") && (direction == "reverse"))
					lDestTransition.m_nEffectDirection = 5;
				else if(subtype == "horizontalLeft")
					lDestTransition.m_nEffectDirection = 6;
				else if(subtype == "horizontalRight")
					lDestTransition.m_nEffectDirection = 7;
			}
			/*
			else if(type == "barnDoorWipe")//subtype = "vertical"/"horizontal" + direction = "reverse"/""
			*/
			else
			{
				lDestTransition.m_nEffectType = 0;
				lDestTransition.m_nEffectDirection = 0;
			}


			lDestTransition.m_nTime = 500;//1;//GetTransitionSpeedFromStr(pTransition.spd.get_value_or("fast"));
			pDocument.m_mapTransitions.Add(SldNum, lDestTransition);
		}
	}
}