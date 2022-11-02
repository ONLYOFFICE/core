#pragma once

#include "TimeNode.h"
#include "intermediate_anim.h"

namespace PPT {
namespace Intermediate {
class OldAnimationParser
{
public:
    OldAnimationParser(SlideAnimation& slideAnim, std::list<Animation>& rawOldAnimation);
    void Parse(); // todo change signature

private:
    SlideAnimation& slideAnim;
    const std::list<Animation>& lstAnimations;
};
}
}
