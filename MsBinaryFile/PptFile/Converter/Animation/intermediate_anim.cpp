#include "intermediate_anim.h"


_UINT32 PPT::Intermediate::SOldAnimation::getAnimDur() const
{
    _UINT32 dur = 500;
    if (!anim)
        return 0;

    const BYTE effect = anim->m_AnimationAtom.m_AnimEffect;
    const BYTE direct = anim->m_AnimationAtom.m_AnimEffectDirection;

    if (effect == 12 && ((direct >= 12 && direct <= 15) || direct == 27))   // Crawl In, Basic Swivel
        dur = 5000;
    else if (effect == 12 && direct == 28)
        dur = 1000;
    else if (effect == 14)
    {
        switch (direct)
        {
        case 0: dur = 75; break;
        case 1: dur = 500; break;
        case 2: dur = 1000; break;
        }
    } else if (effect == 19 || effect == 26)
        dur = 2000;

    return dur;
}
