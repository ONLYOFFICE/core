#include "ridmanager.h"

#include "../Records/SoundContainer.h"

RIDManager::RIDManager() :
    m_haveSetedSoundRIDCollection(false)
{

}

void RIDManager::setRIDfromAnimation(Animation& anim)
{
    clearRIDSforSlide();

    setSoundRIDCollection(anim.m_pSoundContainer);
    searchSound(anim.m_oPPT10.m_pExtTimeNodeContainer);
}

void RIDManager::setSoundRIDCollection(CRecordSoundCollectionContainer* pColection)
{
    if (!pColection) return;

    if (!m_haveSetedSoundRIDCollection)
    {
        for (auto* pRec : pColection->m_arRecords)
        {
            auto soundCont = dynamic_cast<CRecordSoundContainer*>(pRec);
            if (soundCont)
            {
                auto recStr = dynamic_cast<CRecordCString*>(soundCont->m_arRecords[2]);
                if (recStr)
                    m_soundRIDCollection.push_back(&(recStr->m_strText));
            }
        }
        m_haveSetedSoundRIDCollection = true;
    }
}

void RIDManager::searchSound (CRecordExtTimeNodeContainer* const pETNC)
{
    if (!pETNC) return;

    if (pETNC->m_haveClientVisualElement && pETNC->m_pClientVisualElement)
        addSound(pETNC->m_pClientVisualElement);

    for (auto* pSubEffect : pETNC->m_arrRgSubEffect)
        addSound(pSubEffect->m_pClientVisualElement);

    for (auto* pETNCChildren : pETNC->m_arrRgExtTimeNodeChildren)
        searchSound(pETNCChildren);
}

void RIDManager::addSound(CRecordClientVisualElementContainer* const pCVEC)
{
    if (!pCVEC) return;

    if (pCVEC->m_bVisualShapeAtom)
        m_arrRID.push_back(&(pCVEC->m_oVisualShapeAtom.m_nObjectIdRef));
}

void RIDManager::clearRIDSforSlide()
{
    m_arrRID.clear();
    m_arrSoundRef.clear();
}
