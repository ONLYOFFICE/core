
#ifndef CORE_GRCLIP_H
#define CORE_GRCLIP_H

#include <vector>
#include "../lib/xpdf/GfxState.h"

class GfxClip
{
public:
    void AddPath(GfxPath *pPath, double *Matrix, bool bEo)
    {
        m_vPaths.push_back(pPath);
        m_vMatrix.push_back(Matrix);
        m_vPathsClipEo.push_back(bEo);
    }

    size_t GetPathNum()
    {
        return m_vPaths.size();
    }

    GfxPath* GetPath(int i)
    {
        return m_vPaths[i];
    }

    bool GetClipEo(int i)
    {
        return m_vPathsClipEo[i];
    }

    double* GetMatrix(int i)
    {
        return m_vMatrix[i];
    }

private:
    std::vector<GfxPath*> m_vPaths;
    std::vector<bool> m_vPathsClipEo;
    std::vector<double*> m_vMatrix;
};


#endif //CORE_GRCLIP_H
