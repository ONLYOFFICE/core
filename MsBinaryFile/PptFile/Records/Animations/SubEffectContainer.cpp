#include "SubEffectContainer.h"

using namespace PPT;

//--------------------------------------------------------------------------------
#define SUBEFFECT_CASE(RECORD_TYPE, P_VAR, CLASS_RECORD_NAME, FLAG)				\
    case RECORD_TYPE: { P_VAR = new CLASS_RECORD_NAME();                        \
        P_VAR->ReadFromStream(ReadHeader, pStream);                             \
        FLAG = true;                                                            \
        break; }                                                                \
//-------------------------------------------------------------------------------

CRecordSubEffectContainer::CRecordSubEffectContainer() :
    m_pTimePropertyList(nullptr),
    m_pTimeColorBehavior(nullptr),
    m_pTimeSetBehavior(nullptr),
    m_pTimeCommandBehavior(nullptr),
    m_pClientVisualElement(nullptr),

    m_haveTimePropertyList(false),
    m_haveColorBehavior(false),
    m_haveSetBehavior(false),
    m_haveCommandBehavior(false),
    m_haveClientVisualElement(false)
{

}

CRecordSubEffectContainer::~CRecordSubEffectContainer()
{
    RELEASEOBJECT(m_pTimePropertyList);
    RELEASEOBJECT(m_pTimeColorBehavior);
    RELEASEOBJECT(m_pTimeSetBehavior);
    RELEASEOBJECT(m_pTimeCommandBehavior);
    RELEASEOBJECT(m_pClientVisualElement);

    for (auto pRecord : m_arrRgBeginTimeCondition)
        RELEASEOBJECT(pRecord);
    for (auto pRecord : m_arrRgEndTimeCondition)
        RELEASEOBJECT(pRecord);
    for (auto pRecord : m_arrRgTimeModifierAtom)
        RELEASEOBJECT(pRecord);
}

void CRecordSubEffectContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    SRecordHeader	ReadHeader;

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oTimeNodeAtom.ReadFromStream ( ReadHeader, pStream );

    UINT lCurLen	=	m_oTimeNodeAtom.m_oHeader.RecLen + 8;
    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen +=	8 + ReadHeader.RecLen;

        switch (ReadHeader.RecType)
        {
        SUBEFFECT_CASE(RT_TimePropertyList, m_pTimePropertyList,
                       CRecordTimePropertyList4TimeNodeContainer, m_haveTimePropertyList)
        SUBEFFECT_CASE(RT_TimeColorBehaviorContainer, m_pTimeColorBehavior,
                       CRecordTimeColorBehaviorContainer, m_haveColorBehavior)
        SUBEFFECT_CASE(RT_TimeSetBehaviorContainer, m_pTimeSetBehavior,
                       CRecordTimeSetBehaviorContainer, m_haveSetBehavior)
        SUBEFFECT_CASE(RT_TimeCommandBehaviorContainer, m_pTimeCommandBehavior,
                       CRecordTimeCommandBehaviorContainer, m_haveCommandBehavior)
        SUBEFFECT_CASE(RT_TimeClientVisualElement, m_pClientVisualElement,
                       CRecordClientVisualElementContainer, m_haveClientVisualElement)

        case RT_TimeConditionContainer:
        {
            CRecordTimeConditionContainer* pTimeCondition =
            new CRecordTimeConditionContainer();
            pTimeCondition->ReadFromStream(ReadHeader, pStream);
            unsigned short recInst = ReadHeader.RecInstance;

            if (recInst == TL_CT_Begin)
                m_arrRgBeginTimeCondition.push_back(pTimeCondition);

            else if (recInst == TL_CT_End)
                m_arrRgEndTimeCondition.push_back(pTimeCondition);
            else
                throw ;

            break; // A lot of records. Look at instance
        }

        case RT_TimeModifier:
        {
            CRecordTimeModifierAtom* pModAtom = new CRecordTimeModifierAtom();
            pModAtom->ReadFromStream(ReadHeader, pStream);
            m_arrRgTimeModifierAtom.push_back(pModAtom);
            break;
        }

        default:
            throw ;
        }
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);

}
