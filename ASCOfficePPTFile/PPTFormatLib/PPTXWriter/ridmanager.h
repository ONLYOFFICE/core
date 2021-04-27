#ifndef RIDMANAGER_H
#define RIDMANAGER_H

#include "Animation.h"

// This class is heeded for rId audio in the first.
// And later maybe will be used for video.
class RIDManager
{
public:
    RIDManager();

    // This methods take all rid by ref and correct it
    // Look at the example
    /*
        It for animation
                we have rIDs in ppt ETNC | we need (add some constants(1) and rewrite array)
        1 slide 1 2 3 2 4 1 1               1 slide 2 3 4 3 5 2 2
        2 slide 3 3 5 6 6                   2 slide 2 2 3 4 4
        3 slide 7                           3 slide 2


        we have soundCollection. It's an array [1,2,3,4,5,6,7] with rIDs inside
        |
        we need new arrays with [1 2 3 4], [3 5 6], [7]. It for _rels

        and write in _rels (rId : assciate with audio №)
        1 rels 2:1 3:2 4:3 5:4
        2 rels 2:3 3:5 4:6
        3 rels 2:7
    */
    // It sets in first
    void addSoundPath(std::wstring& path);

    void setRIDfromTransitions(int &soundRID);
    void setRIDfromAnimation(Animation& anim);
    void setRID(int RID)  // It's num was added to output //// add some constants(1)
        {m_RID = RID;}
    std::vector<std::wstring> getPathesForSlideRels(); // Correct RIDs and get paths for rels in current slide

private:
    // It set once
    void setSoundRIDCollection(CRecordSoundCollectionContainer* pColection);

    int  getSoundPos(int oldID);

    void searchSound (CRecordExtTimeNodeContainer* const pETNC);
    void searchSound (const std::vector<SOldAnimation> &arrAnim);
    void addSound(CRecordClientVisualElementContainer* const pCVEC);
    void clearRIDSforSlide();
private:
    // for input --> rewrite --> output
    std::vector<_UINT32*> m_arrRID;         // It's for one slide animation
    std::vector<_UINT32*> m_arrSoundRef;    // It's for one slide _rels
    int                   m_RID;            // This is current rID, it adds for generating note in slede rels

    bool m_haveSetedSoundRIDCollection;
    std::vector<std::wstring*> m_soundRIDCollection; // It sets once
    std::vector<std::wstring > m_soundPaths; // It sets once
    // read from file id : needed id
    std::map<int, int>         m_knownIDs;  // It's seting in the whole life object period
};

#endif // RIDMANAGER_H
