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
    for (unsigned i = 0; i < m_arrRID.size(); i++)
    {
        auto* pRID = m_arrRID[i];
        *(m_arrSoundRef[i]) = getSoundPos(*pRID);

        auto searchIter(mapRIDs.find(*pRID));
        if (searchIter == mapRIDs.end())
        {
            unsigned soundPos = getSoundPos(*pRID);
            if (soundPos >= m_soundPaths.size())
                continue;

            paths.push_back(m_soundPaths[soundPos]);
            mapRIDs.insert(std::make_pair(*pRID, pRID));
            *pRID = m_RID++;;
        }
        else
        {
            *pRID = *(searchIter->second);
        }

        if (i < m_soundRIDCollection.size())
            *(m_soundRIDCollection[i]) = std::to_wstring(*pRID);
    }

    return paths;
}

void RIDManager::setRIDfromAnimation(Animation& anim)
{
    clearRIDSforSlide();

    setSoundRIDCollection(anim.m_pSoundContainer);
    if (anim.m_pPPT10)
        searchSound(anim.m_pPPT10->m_pExtTimeNodeContainer);
    else if (!anim.m_arrOldAnim.empty())
        searchSound(anim.m_arrOldAnim);
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

void RIDManager::searchSound (const std::vector<SOldAnimation> &arrAnim)
{
    for (auto anim : arrAnim)
    {
        if (anim.anim->m_AnimationAtom.m_fSound)
        {
            m_arrRID.push_back(&(anim.anim->m_AnimationAtom.m_SoundIdRef));
            m_arrSoundRef.push_back(&(anim.anim->m_AnimationAtom.m_SoundIdRef));
        }
    }
}

void RIDManager::addSound(CRecordClientVisualElementContainer* const pCVEC)
{
    if (!pCVEC) return;

    if (pCVEC->m_bVisualShapeAtom && pCVEC->m_oVisualShapeAtom.m_Type == TL_TVET_Audio)
    {
        m_arrRID.push_back(&(pCVEC->m_oVisualShapeAtom.m_nObjectIdRef));
        m_arrSoundRef.push_back(&(pCVEC->m_oVisualShapeAtom.m_nOldIdRef));
    }
}

void RIDManager::clearRIDSforSlide()
{
    m_arrRID.clear();
    m_arrSoundRef.clear();
    m_RID = 1;
}

// When we read old records, we take one array for all slides.
// New format has also this array with [audio1.wav, audio2.wav,...]
// but it also has slide rels with absolutly new 'rID' that bind with new format main array,
// and we need to get possition in old array for new 'rID'
// This function dose it.
int  RIDManager::getSoundPos(int oldID)
{
    auto searchIter = m_knownIDs.find(oldID);
    if (searchIter != m_knownIDs.end())
        return searchIter->second;

    unsigned nextNewId = m_knownIDs.size();
    m_knownIDs.insert(std::make_pair(oldID, nextNewId));
    return nextNewId;
}
