#include "OldAnimationParser.h"


PPT::Intermediate::OldAnimationParser::OldAnimationParser(SlideAnimation& slideAnim, std::list<Animation> &rawOldAnimation) :
    slideAnim(slideAnim),
    lstAnimations(rawOldAnimation)
{}

void PPT::Intermediate::OldAnimationParser::Parse()
{
    if (lstAnimations.empty())
        return;
}
