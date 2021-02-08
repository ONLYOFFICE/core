#include "ridmanager.h"

#include "../Records/SoundContainer.h"
#include <map>
#include <cmath>

RIDManager::RIDManager() :
    m_RID(1),
    m_haveSetedSoundRIDCollection(false)
{

}

std::vector<std::wstring> RIDManager::getPathesForSlideRels()
{
    std::vector<std::wstring> paths;
    if (m_arrRID.empty())
        return paths;


    std::map<_UINT32, _UINT32*> mapRIDs;    // first value is old rId : second new rId
    int i = 0;
    for (auto* pRID : m_arrRID)
    {
        auto searchIter(mapRIDs.find(*pRID));
        if (searchIter == mapRIDs.end())
        {
            mapRIDs.insert(std::make_pair(*pRID, pRID));
            *pRID = m_RID++;
        }
        else
            *pRID = *(searchIter->second);

        *(m_soundRIDCollection[i++]) = std::to_wstring(*pRID);
    }

    for (auto RID : mapRIDs)
        paths.push_back(m_soundPaths[RID.first - 1]);
    return paths;
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

void RIDManager::addSoundPath(std::wstring& path)
{
    m_soundPaths.push_back(path);
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
    m_RID = 1;
}
