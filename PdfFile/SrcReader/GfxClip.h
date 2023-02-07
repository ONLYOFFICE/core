
#ifndef CORE_GRCLIP_H
#define CORE_GRCLIP_H

#include <vector>
#include <string>
#include <math.h>
#include "../lib/xpdf/GfxState.h"
#include "MemoryUtils.h"

struct GfxClipMatrix
{
    double dA;
    double dB;
    double dC;
    double dD;
    double dE;
    double dF;

    void FromDoublePointer(double* pMatrix)
    {
        dA = pMatrix[0];
        dB = pMatrix[1];
        dC = pMatrix[2];
        dD = pMatrix[3];
        dE = pMatrix[4];
        dF = pMatrix[5];
    }
};

class GfxTextClip
{
public:

    GfxTextClip()
    {
        m_nTextsCount = m_nSize = 0;
        m_pTexts = NULL;
        m_pMatrix = NULL;
    }

    GfxTextClip *Copy()
    {
        return new GfxTextClip(this);
    }

    ~GfxTextClip()
    {
        for (int nIndex = 0; nIndex < m_nTextsCount; ++nIndex)
        {
            FreeWString(m_pTexts[nIndex].wsText);
            FreeWString(m_pTexts[nIndex].wsFontName);
            FreeWString(m_pTexts[nIndex].wsFontPath);
        }
        PdfReader::MemUtilsFree(m_pTexts);
        PdfReader::MemUtilsFree(m_pMatrix);
    }

    void Reset()
    {
        for (int nIndex = 0; nIndex < m_nTextsCount; ++nIndex)
        {
            FreeWString(m_pTexts[nIndex].wsText);
            FreeWString(m_pTexts[nIndex].wsFontName);
            FreeWString(m_pTexts[nIndex].wsFontPath);
        }
        PdfReader::MemUtilsFree(m_pTexts);
        PdfReader::MemUtilsFree(m_pMatrix);

        m_nTextsCount = m_nSize = 0;
        m_pTexts = NULL;
        m_pMatrix = NULL;
    }

    void
    ClipToText(std::wstring &wsFontName, std::wstring &wsFontPath, double dFontSize, int nFontStyle, double *pMatrix,
               std::wstring &wsText, double dX, double dY, double dWidth = 0, double dHeight = 0,
               double dBaseLineOffset = 0)
    {
        Resize(1);

        m_pTexts[m_nTextsCount].wsFontName = AllocWString(wsFontName);
        m_pTexts[m_nTextsCount].wsFontPath = AllocWString(wsFontPath);
        m_pTexts[m_nTextsCount].dFontSize = dFontSize;
        m_pTexts[m_nTextsCount].nFontStyle = nFontStyle;
        m_pTexts[m_nTextsCount].wsText = AllocWString(wsText);
        m_pTexts[m_nTextsCount].dX = dX;
        m_pTexts[m_nTextsCount].dY = dY;
        m_pTexts[m_nTextsCount].dWidth = dWidth;
        m_pTexts[m_nTextsCount].dHeight = dHeight;
        m_pTexts[m_nTextsCount].dBaseLineOffset = dBaseLineOffset;

        m_pMatrix[m_nTextsCount].FromDoublePointer(pMatrix);

        ++m_nTextsCount;
        return;

    }

    int GetTextsCount()
    {
        return m_nTextsCount;
    }

    wchar_t * GetText(int nTextIndex, double *pdX, double *pdY, double *pdWidth, double *pdHeight, double *pdBaseLineOffset,
                      wchar_t * *pwsFontName, wchar_t * *pwsFontPath, double *pdFontSize, int *pnFontStyle)
    {
        if (nTextIndex < 0 || nTextIndex >= m_nTextsCount)
            return NULL;

        *pdX = m_pTexts[nTextIndex].dX;
        *pdY = m_pTexts[nTextIndex].dY;
        *pdWidth = m_pTexts[nTextIndex].dWidth;
        *pdHeight = m_pTexts[nTextIndex].dHeight;
        *pdBaseLineOffset = m_pTexts[nTextIndex].dBaseLineOffset;

        *pwsFontName = m_pTexts[nTextIndex].wsFontName;
        *pwsFontPath = m_pTexts[nTextIndex].wsFontPath;
        *pdFontSize = m_pTexts[nTextIndex].dFontSize;
        *pnFontStyle = m_pTexts[nTextIndex].nFontStyle;

        return m_pTexts[nTextIndex].wsText;
    }

    double *GetMatrix(int nTextIndex)
    {
        if (nTextIndex < 0 || nTextIndex >= m_nTextsCount)
            return NULL;

        return m_pMatrix[nTextIndex].ToDoublePointer();
    }

    bool IsEqual(GfxTextClip *pClip)
    {
        if (pClip->m_nTextsCount != m_nTextsCount)
            return false;

        for (int nIndex = 0; nIndex < m_nTextsCount; nIndex++)
        {
            if (!m_pTexts[nIndex].IsEqual(pClip->m_pTexts[nIndex]) ||
                !m_pMatrix[nIndex].IsEqual(pClip->m_pMatrix[nIndex]))
                return false;
        }

        return true;
    }

public:

    GfxTextClip(GfxTextClip *pTextClip)
    {
        m_nTextsCount = pTextClip->m_nTextsCount;
        m_nSize = pTextClip->m_nSize;

        m_pTexts = (Text *) PdfReader::MemUtilsMallocArray(m_nSize, sizeof(Text));
        m_pMatrix = (Matrix *) PdfReader::MemUtilsMallocArray(m_nSize, sizeof(Matrix));

        for (int nIndex = 0; nIndex < m_nTextsCount; ++nIndex)
        {
            m_pTexts[nIndex].wsText = AllocWString(pTextClip->m_pTexts[nIndex].wsText);
            m_pTexts[nIndex].dX = pTextClip->m_pTexts[nIndex].dX;
            m_pTexts[nIndex].dY = pTextClip->m_pTexts[nIndex].dY;
            m_pTexts[nIndex].dWidth = pTextClip->m_pTexts[nIndex].dWidth;
            m_pTexts[nIndex].dHeight = pTextClip->m_pTexts[nIndex].dHeight;
            m_pTexts[nIndex].dBaseLineOffset = pTextClip->m_pTexts[nIndex].dBaseLineOffset;

            m_pTexts[nIndex].wsFontName = AllocWString(pTextClip->m_pTexts[nIndex].wsFontName);
            m_pTexts[nIndex].wsFontPath = AllocWString(pTextClip->m_pTexts[nIndex].wsFontPath);
            m_pTexts[nIndex].dFontSize = pTextClip->m_pTexts[nIndex].dFontSize;
            m_pTexts[nIndex].nFontStyle = pTextClip->m_pTexts[nIndex].nFontStyle;

            m_pMatrix[nIndex] = pTextClip->m_pMatrix[nIndex];
        }

    }

    void Resize(int nTextsCount)
    {
        if (m_nTextsCount + nTextsCount > m_nSize)
        {
            if (m_nSize == 0)
            {
                m_nSize = 32;
            }
            while (m_nSize < m_nTextsCount + nTextsCount)
            {
                m_nSize *= 2;
            }

            m_pTexts = (Text *) PdfReader::MemUtilsReallocArray(m_pTexts, m_nSize, sizeof(Text), m_nTextsCount);
            m_pMatrix = (Matrix *) PdfReader::MemUtilsReallocArray(m_pMatrix, m_nSize, sizeof(Matrix), m_nTextsCount);

            if (NULL == m_pTexts)
                m_pTexts = (Text *) PdfReader::MemUtilsMallocArray(m_nSize, sizeof(Text));
            else
                m_pTexts = (Text *) PdfReader::MemUtilsReallocArray(m_pTexts, m_nSize, sizeof(Text), m_nTextsCount);

            if (NULL == m_pMatrix)
                m_pMatrix = (Matrix *) PdfReader::MemUtilsMallocArray(m_nSize, sizeof(Matrix));
            else
                m_pMatrix = (Matrix *) PdfReader::MemUtilsReallocArray(m_pMatrix, m_nSize, sizeof(Matrix), m_nTextsCount);
        }
    }

    void Transform(double *pMatrix, double dX, double dY, double *pdX, double *pdY)
    {
        *pdX = dX * pMatrix[0] + dY * pMatrix[2] + pMatrix[4];
        *pdY = dX * pMatrix[1] + dY * pMatrix[3] + pMatrix[5];
    }

private:

#pragma pack(push, 1)

    struct Matrix
    {
        double dA;
        double dB;
        double dC;
        double dD;
        double dE;
        double dF;

        double *ToDoublePointer()
        {
            return &dA;
        }

        void FromDoublePointer(double *pMatrix)
        {
            dA = pMatrix[0];
            dB = pMatrix[1];
            dC = pMatrix[2];
            dD = pMatrix[3];
            dE = pMatrix[4];
            dF = pMatrix[5];
        }

        bool IsEqual(Matrix &oMatrix)
        {
            if (fabs(dA - oMatrix.dA) < 0.001 && fabs(dB - oMatrix.dB) < 0.001 && fabs(dC - oMatrix.dC) < 0.001 &&
                fabs(dD - oMatrix.dD) < 0.001 && fabs(dE - oMatrix.dE) < 0.001 && fabs(dF - oMatrix.dF) < 0.001)
                return true;

            return false;
        }
    };

    struct Text
    {
        wchar_t * wsFontName;
        wchar_t * wsFontPath;
        double dFontSize;
        int nFontStyle;

        wchar_t * wsText;
        double dX;
        double dY;
        double dWidth;
        double dHeight;
        double dBaseLineOffset;

        bool IsEqual(Text &oText)
        {
            if (fabs(dX - oText.dX) < 0.001 && fabs(dY - oText.dY) < 0.001 && fabs(dWidth - oText.dWidth) < 0.001 &&
                fabs(dHeight - oText.dHeight) < 0.001 && fabs(dBaseLineOffset - oText.dBaseLineOffset) < 0.001 &&
                std::wstring(wsText) == std::wstring(oText.wsText))
                return true;

            return false;
        }
    };

#pragma pack(pop)

private:

    Text *m_pTexts;
    Matrix *m_pMatrix;
    int m_nTextsCount;
    int m_nSize;


    static wchar_t* AllocWString(const std::wstring& wsString)
    {
        int nLen = wsString.length();
        wchar_t *wsResult = new wchar_t[nLen + 1];
        wsResult[nLen] = 0x0000;

        for (int nIndex = 0; nIndex < nLen; nIndex++)
        {
            wsResult[nIndex] = (wchar_t)wsString.at(nIndex);
        }

        return wsResult;
    }
    static wchar_t* AllocWString(const wchar_t* wsString)
    {
        return AllocWString(std::wstring(wsString));
    }
    static wchar_t* AllocWString(wchar_t* wsString)
    {
        return AllocWString(std::wstring(wsString));
    }
    static void FreeWString(wchar_t* wString)
    {
        if (wString)
            delete[] wString;
    }
};

class GfxClip
{
public:
    GfxClip()
    {
        m_pTextClip = new GfxTextClip();
    }

    ~GfxClip()
    {
        delete m_pTextClip;
    }

    void AddPath(GfxPath *pPath, double *Matrix, bool bEo)
    {
        if (pPath && Matrix)
        {
            m_vPaths.push_back(pPath->copy());
            m_vMatrix.push_back(GfxClipMatrix());
            m_vMatrix.back().FromDoublePointer(Matrix);
            m_vPathsClipEo.push_back(bEo);
        }

    }

    size_t GetPathNum()
    {
        return m_vPaths.size();
    }

    GfxPath *GetPath(int i)
    {
        return m_vPaths[i];
    }

    bool GetClipEo(int i)
    {
        return m_vPathsClipEo[i];
    }

    bool IsChanged()
    {
        return m_bChanged;
    }

    void SetChanged(bool b)
    {
        m_bChanged = b;
    }

    GfxTextClip *GetTextClip() const
    {
        return m_pTextClip;
    }

    GfxClip(const GfxClip &c) {
        m_pTextClip  = new GfxTextClip(c.GetTextClip());
    };

    std::vector<GfxClipMatrix> m_vMatrix;
private:


    std::vector<GfxPath *> m_vPaths;
    std::vector<bool> m_vPathsClipEo;
    bool m_bChanged;
    GfxTextClip *m_pTextClip;
};


#endif //CORE_GRCLIP_H
