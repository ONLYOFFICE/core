#ifndef EMFPLUSTYPES_H
#define EMFPLUSTYPES_H

namespace MetaFile
{
        struct TEmfPlusRect
        {
                short shX;
                short shY;
                short shWidth;
                short shHeight;

                TEmfPlusRect()
                {
                        shX      = 0;
                        shY      = 0;
                        shWidth  = 0;
                        shHeight = 0;
                }

                TEmfPlusRect(const TEmfPlusRect& oRect)
                {
                        shX      = oRect.shX;
                        shY      = oRect.shY;
                        shWidth  = oRect.shWidth;
                        shHeight = oRect.shHeight;
                }
        };

        struct TEmfPlusRectF
        {
              double dX;
              double dY;
              double dWidth;
              double dHeight;

              TEmfPlusRectF()
              {
                      dX        = 0;
                      dY        = 0;
                      dWidth    = 0;
                      dHeight   = 0;
              }
              TEmfPlusRectF(const TEmfPlusRectF& oRect)
              {
                      dX        = oRect.dX;
                      dY        = oRect.dY;
                      dWidth    = oRect.dWidth;
                      dHeight   = oRect.dHeight;
              }
        };

        struct TEmfPlusARGB
        {
                unsigned char chBlue;
                unsigned char chGreen;
                unsigned char chRed;
                unsigned char chAlpha;

                TEmfPlusARGB()
                {
                        chBlue  = 0;
                        chGreen = 0;
                        chRed   = 0;
                        chAlpha = 0;
                }

                TEmfPlusARGB(const TEmfPlusARGB& oARGB)
                {
                        chBlue  = oARGB.chBlue;
                        chGreen = oARGB.chGreen;
                        chRed   = oARGB.chRed;
                        chAlpha = oARGB.chAlpha;
                }
        };


        struct TEmfPlusPointR
        {

        };

        struct TEmfPlusPointF
        {
                double dX;
                double dY;

                TEmfPlusPointF()
                {
                        dX = 0;
                        dY = 0;
                }

                TEmfPlusPointF(const TEmfPlusPointF& oPoint)
                {
                        dX = oPoint.dX;
                        dY = oPoint.dY;
                }
        };

        struct TEmfPlusPoint
        {
                short shX;
                short shY;

                TEmfPlusPoint()
                {
                        shX = 0;
                        shY = 0;
                }

                TEmfPlusPoint(const TEmfPlusPoint& oPoint)
                {
                        shX = oPoint.shX;
                        shY = oPoint.shY;
                }
        };
}

#endif // EMFPLUSTYPES_H
