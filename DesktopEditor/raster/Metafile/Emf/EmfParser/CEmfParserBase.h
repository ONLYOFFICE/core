#ifndef CEMFPARSERBASE_H
#define CEMFPARSERBASE_H

#include "../EmfTypes.h"
#include "../EmfObjects.h"
#include "../../Common/MetaFileUtils.h"
#include "../EmfInterpretator/CEmfInterpretatorAll.h"

#include "../EmfPlayer.h"
#include "../EmfPath.h"

#include "../../Common/MetaFile.h"

#include "../../../../fontengine/FontManager.h"

#ifdef _DEBUG
    #include <iostream>
    #include  <algorithm>
    #include <cstdlib>
#endif

#if defined(DrawText)
    #undef DrawText
#endif

namespace MetaFile
{
        enum EmfParserType
        {
                EmfParser,
                EmfxParser
        };

        class CEmfPlayer;

        class  CEmfParserBase : public IMetaFileBase
        {
        public:
                CEmfParserBase();
                ~CEmfParserBase();

                virtual bool            OpenFromFile(const wchar_t*)    = 0;
                virtual void            PlayFile()                      = 0;
                virtual void            Scan()                          = 0;

                virtual EmfParserType   GetType()                       = 0;

                void		PlayMetaFile()				override;
                void		ClearFile()				override;
                TRect*		GetDCBounds()				override;
                double		GetPixelHeight()			override;
                double		GetPixelWidth()				override;
                int		GetTextColor()				override;
                IFont*		GetFont()				override;
                IBrush*		GetBrush()				override;
                IPen*		GetPen()				override;
                unsigned int	GetTextAlign()				override;
                unsigned int	GetTextBgMode()				override;
                int		GetTextBgColor()			override;
                unsigned int	GetFillMode()				override;
                TPointD		GetCurPos()				override;
                TXForm*		GetInverseTransform()			override;
                TXForm*		GetTransform(int = GM_ADVANCED)         override;
                unsigned int	GetMiterLimit()                         override;
                unsigned int	GetRop2Mode()				override;
                IClip*		GetClip()				override;
                int		GetCharSpace()				override;
                bool		IsWindowFlippedY()			override;
                bool		IsWindowFlippedX()			override;

                void SetInterpretator(IOutputDevice* pOutput);
                void SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType);
                void SetInterpretator(IOutputDevice* pOutput, const wchar_t *wsFilePath);

                //Работа с изображениями
                void ImageProcessing(const TEmfAlphaBlend       &oTEmfAlphaBlend);
                void ImageProcessing(const TEmfStretchDIBITS    &oTEmfStretchDIBITS);
                void ImageProcessing(const TEmfBitBlt           &oTEmfBitBlt);
                void ImageProcessing(const TEmfSetDiBitsToDevice&oTEmfSetDiBitsToDevice);
                void ImageProcessing(const TEmfStretchBLT       &oTEmfStretchBLT);
                void ImageProcessing(const TEmfDibPatternBrush  &oTEmfDibPatternBrush, unsigned int ulBrushIndex);
                void DrawImage(int nX, int nY, int nW, int nH, BYTE *pImageBuffer, unsigned int unImageW, unsigned int unImageH);
                //----------------------

                void TranslatePoint(TEmfPointL &oPoint, double &dX, double &dY);
                void TranslatePoint(int nX, int nY, double &dX, double &dY);

                void UpdateOutputDC();
                void ClosePath();

                void MoveTo(int nX, int nY);
                void MoveTo(TEmfPointL &oPoint);
                void MoveTo(TEmfPointS &oPoint);

                void LineTo(int nX, int nY);
                void LineTo(TEmfPointL &oPoint);
                void LineTo(TEmfPointS &oPoint);

                void CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe);
                void CurveTo(TEmfPointS &oPoint1, TEmfPointS &oPoint2, TEmfPointS &oPointE);
                void CurveTo(TEmfPointL &oPoint1, TEmfPointL &oPoint2, TEmfPointL &oPointE);

                void ArcTo(int nL, int nT, int nR, int nB, double dStart, double dSweep);

                void DrawPath(bool bStroke, bool bFill);
                void DrawText(std::wstring &wsString, unsigned int unCharsCount, int _nX, int _nY, int *pnDx, int iGraphicsMode);
                void DrawTextA(TEmfEmrText &oText, int iGraphicsMode);
                void DrawTextW(TEmfEmrText &oText, int iGraphicsMode);

                CEmfDC*     GetDC();
                TEmfRectL*  GetBounds();

        private:
                friend class CEmfPlayer;
                friend class CEmfClip;

                friend class CEmfParser;
                friend class CEmfxParser;

                TEmfHeader        m_oHeader;
                unsigned int      m_ulRecordSize;
                CEmfDC*           m_pDC;
                CEmfPlayer        m_oPlayer;
                CEmfPath*         m_pPath;
                TEmfXForm         m_oTransform;

                CEmfInterpretatorBase   *m_pInterpretator;

        private:
                virtual bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight) = 0;
        };

}
#endif // CEMFPARSERBASE_H
