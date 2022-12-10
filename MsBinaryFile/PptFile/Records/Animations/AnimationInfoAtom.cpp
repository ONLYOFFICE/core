#include "AnimationInfoAtom.h"

using namespace PPT;

struct SFlagsAH {

    USHORT m_fA_H;

    USHORT getH()const {return (m_fA_H & 0xC000) >> 14;}
    USHORT getG()const {return (m_fA_H & 0x3000) >> 12;}
    USHORT getF()const {return (m_fA_H & 0xC00) >> 10;}
    USHORT getE()const {return (m_fA_H & 0x300) >> 8;}
    USHORT getD()const {return (m_fA_H & 0xC0) >> 6;}
    USHORT getC()const {return (m_fA_H & 0x30) >> 4;}
    USHORT getB()const {return (m_fA_H & 0xC) >> 2;}
    USHORT getA()const {return (m_fA_H & 0x3) >> 0;}



    void ReadFromStream(POLE::Stream* pStream) {
        m_fA_H = StreamUtils::ReadWORD(pStream);
    }
};


CRecordAnimationInfoAtom::CRecordAnimationInfoAtom()
{

}

CRecordAnimationInfoAtom::~CRecordAnimationInfoAtom()
{

}

void CRecordAnimationInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sDimColor.ReadFromStream(pStream);

    SFlagsAH    fAH;
    fAH.ReadFromStream(pStream);

    m_fReverse      = fAH.getA();
    m_fAutomatic    = fAH.getB();
    m_fSound        = fAH.getC();
    m_fStopSound    = fAH.getD();
    m_fPlay         = fAH.getE();
    m_fSynchronous  = fAH.getF();
    m_fHide         = fAH.getG();
    m_fAnimateBg    = fAH.getH();

    m_Reserved                  = StreamUtils::ReadWORD(pStream);

    m_SoundIdRef                = StreamUtils::ReadDWORD(pStream);
    m_DelayTime                 = StreamUtils::ReadDWORD(pStream);

    m_OrderID                   = StreamUtils::ReadWORD(pStream);
    m_SlideCount                = StreamUtils::ReadWORD(pStream);

    m_AnimBuildType             = StreamUtils::ReadBYTE(pStream);
    m_AnimEffect                = StreamUtils::ReadBYTE(pStream);
    m_AnimEffectDirection       = StreamUtils::ReadBYTE(pStream);
    m_AnimAfterEffect           = StreamUtils::ReadBYTE(pStream);
    m_TextBuildSubEffect        = StreamUtils::ReadBYTE(pStream);
    m_OleVerb                   = StreamUtils::ReadBYTE(pStream);


    StreamUtils::StreamSkip(2, pStream);
    m_OldSoundIdRef = m_SoundIdRef;

    StreamUtils::StreamSkipBack(36, pStream);
    auto tempStr = StreamUtils::ReadStringA(pStream, 36);
    std::copy(tempStr.begin(), tempStr.end(), asByteArr.begin());
}
