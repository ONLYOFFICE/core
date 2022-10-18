#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/Transition.h"
#include "../../../ASCOfficePPTXFile/Editor/Drawing/SlideShow.h"
#include "../PPTXWriter/ImageManager.h"


namespace PPT {
namespace Converter {
class Transition
{
public:
    Transition(const PPT_FORMAT::CSlideShowInfo &SSInfo, PPT_FORMAT::CRelsGenerator* pRels);
    PPTX::Logic::Transition Convert();

private:
    void ConvertClick();
    void ConvertSpd();
    void ConvertDuration();
    void ConvertEffect();
    void ConvertAudioEffect();

    void InitSound();
    bool HasAudio()const;
    void WriteSoundName();
    std::wstring GetAudioRId();
    void WriteAudioRId();

private:
    const PPT_FORMAT::CSlideShowInfo &slideShowInfo;
    const CTransition &oldTransition;
    PPT_FORMAT::CRelsGenerator* pRels;

    PPTX::Logic::Transition newTransition;
    std::wstring param_name, param_value;
    std::wstring param_name2, param_value2;
};

}
}
