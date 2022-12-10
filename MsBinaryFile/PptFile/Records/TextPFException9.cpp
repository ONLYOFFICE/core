#include "TextPFException9.h"

using namespace PPT;

void STextAutoNumberScheme::ReadFromStream(POLE::Stream *pStream)
{
    m_eScheme   = (TextAutoNumberSchemeEnum)StreamUtils::ReadSHORT(pStream);
    m_nStartNum = StreamUtils::ReadSHORT(pStream);
}

std::wstring STextAutoNumberScheme::SchemeToStr() const
{
    switch (m_eScheme)
    {
    case ANM_AlphaLcPeriod:{ return              L"alphaLcPeriod"; break;}
    case ANM_AlphaUcPeriod:{ return              L"alphaUcPeriod"; break;}
    case ANM_ArabicParenRight:{ return           L"arabicParenR"; break;}
    case ANM_ArabicPeriod:{ return               L"arabicPeriod"; break;}
    case ANM_RomanLcParenBoth:{ return           L"romanLcParenBoth"; break;}
    case ANM_RomanLcParenRight:{ return          L"romanLcParenR"; break;}
    case ANM_RomanLcPeriod:{ return              L"romanLcPeriod"; break;}
    case ANM_RomanUcPeriod:{ return              L"romanUcPeriod"; break;}
    case ANM_AlphaLcParenBoth:{ return           L"alphaLcParenBoth"; break;}
    case ANM_AlphaLcParenRight:{ return          L"alphaLcParenR"; break;}
    case ANM_AlphaUcParenBoth:{ return           L"alphaUcParenBoth"; break;}
    case ANM_AlphaUcParenRight:{ return          L"alphaUcParenR"; break;}
    case ANM_ArabicParenBoth:{ return            L"arabicParenBoth"; break;}
    case ANM_ArabicPlain:{ return                L"arabicPlain"; break;}
    case ANM_RomanUcParenBoth:{ return           L"romanUcParenBoth"; break;}
    case ANM_RomanUcParenRight:{ return          L"romanUcParenR"; break;}
    case ANM_ChsPlain:{ return                   L"ea1ChsPlain"; break;}
    case ANM_ChsPeriod:{ return                  L"ea1ChsPeriod"; break;}
    case ANM_CircleNumDBPlain:{ return           L"circleNumDbPlain"; break;}
    case ANM_CircleNumWDBWhitePlain:{ return     L"circleNumWdWhitePlain"; break;}
    case ANM_CircleNumWDBBlackPlain:{ return     L"circleNumWdBlackPlain"; break;}
    case ANM_ChtPlain:{ return                   L"ea1ChtPlain"; break;}
    case ANM_ChtPeriod:{ return                  L"ea1ChtPeriod"; break;}
    case ANM_Arabic1Minus:{ return               L"arabic1Minus"; break;}
    case ANM_Arabic2Minus:{ return               L"arabic2Minus"; break;}
    case ANM_Hebrew2Minus:{ return               L"hebrew2Minus"; break;}
    case ANM_JpnKorPlain:{ return                L"ea1JpnKorPlain"; break;}
    case ANM_JpnKorPeriod:{ return               L"ea1JpnKorPeriod"; break;}
    case ANM_ArabicDbPlain:{ return              L"arabicDbPlain"; break;}
    case ANM_ArabicDbPeriod:{ return             L"arabicDbPeriod"; break;}
    case ANM_ThaiAlphaPeriod:{ return            L"thaiAlphaPeriod"; break;}
    case ANM_ThaiAlphaParenRight:{ return        L"thaiAlphaParenR"; break;}
    case ANM_ThaiAlphaParenBoth:{ return         L"thaiAlphaParenBoth"; break;}
    case ANM_ThaiNumPeriod:{ return              L"thaiNumPeriod"; break;}
    case ANM_ThaiNumParenRight:{ return          L"thaiNumParenR"; break;}
    case ANM_ThaiNumParenBoth:{ return           L"thaiNumParenBoth"; break;}
    case ANM_HindiAlphaPeriod:{ return           L"hindiAlphaPeriod"; break;}
    case ANM_HindiNumPeriod:{ return             L"hindiNumPeriod"; break;}
    case ANM_JpnChsDBPeriod:{ return             L"ea1JpnChsDbPeriod"; break;}
    case ANM_HindiNumParenRight:{ return         L"hindiNumParenR"; break;}
    case ANM_HindiAlpha1Period:{ return          L"hindiAlpha1Period"; break;}
    }
    return L"alphaUcPeriod";
}

void STextPFException9::ReadFromStream(POLE::Stream *pStream)
{
    m_masks.ReadFromStream(pStream);

    if (m_masks.m_bulletBlip)   // 0x0800000 2^23
        m_optBulletBlipRef = StreamUtils::ReadSHORT(pStream);

    if (m_masks.m_bulletHasScheme)  // 0x2000000 2^25
        m_optfBulletHasAutoNumber = (bool)StreamUtils::ReadSHORT(pStream);

    if(m_masks.m_bulletScheme)  // 0x1000000 2^24
    {
        auto pBulletAutoNumberScheme = new STextAutoNumberScheme;
        pBulletAutoNumberScheme->ReadFromStream(pStream);
        m_optBulletAutoNumberScheme = pBulletAutoNumberScheme;
    }
    else if (m_optfBulletHasAutoNumber.get_value_or(false))
    {
        auto pBulletAutoNumberScheme = new STextAutoNumberScheme;
        pBulletAutoNumberScheme->m_nStartNum = 1;
        pBulletAutoNumberScheme->m_eScheme = ANM_ArabicPeriod;
        m_optBulletAutoNumberScheme = pBulletAutoNumberScheme;
    }

}
