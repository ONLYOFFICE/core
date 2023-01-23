#pragma once
#include <vector>

// у класса T должен быть метод IsBigger, IsBiggerOrEqual
template<typename T>
void SortElements(std::vector<T*>& oArray)
{
    int nSize = (int)oArray.size();

    // handle 0, 1 and 2 elements
    if (nSize <= 1)
        return;
    if (nSize == 2)
    {
        if (oArray[0]->IsBigger(oArray[1]))
        {
            std::swap(oArray[0], oArray[1]);
        }
        return;
    }

    T* tTemp;

    // arrange elements as tree with greater elements appearing first
    int nIndex = (nSize >> 1) - 1, nCurr = 0, nNext = 0;
    int nLast = nSize - 1;
    int nHalf = nSize >> 1;
    do
    {
        // save element at start of chain
        tTemp = oArray[nIndex];

        nCurr = nIndex;
        while (nCurr < nHalf)
        {
            nNext = (nCurr << 1) + 1;
            if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                nNext++;
            if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                break;

            // promote element in chain
            oArray[nCurr] = oArray[nNext];
            nCurr = nNext;
        }

        // restore element at end of chain
        oArray[nCurr] = tTemp;
    }
    while (nIndex--);

    // sequentially reduce tree size by removing maximum element and rebalancing
    nIndex = nSize;
    while (--nIndex)
    {
        // save element at start of chain
        tTemp = oArray[nIndex];
        oArray[nIndex] = oArray[0];

        nCurr = 0;
        nLast = nIndex - 1;
        nHalf = nIndex >> 1;
        while (nCurr < nHalf)
        {
            nNext = (nCurr << 1) + 1;
            if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                nNext++;
            if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                break;

            // promote element in chain
            oArray[nCurr] = oArray[nNext];
            nCurr = nNext;
        }

        // restore element at end of chain
        oArray[nCurr] = tTemp;
    }
}
