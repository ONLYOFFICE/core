#ifndef EMFPLUSTYPES_H
#define EMFPLUSTYPES_H

namespace MetaFile
{
        struct TEmfPlusRectR
        {
                char chX;
                char chY;
                char chWidth;
                char chHeight;

                TEmfPlusRectR()
                {
                        chX      = 0;
                        chY      = 0;
                        chWidth  = 0;
                        chHeight = 0;
                }

                TEmfPlusRectR(const TEmfPlusRectR& oRect)
                {
                        chX      = oRect.chX;
                        chY      = oRect.chY;
                        chWidth  = oRect.chWidth;
                        chHeight = oRect.chHeight;
                }
        };

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
              float fX;
              float fY;
              float fWidth;
              float fHeight;

              TEmfPlusRectF()
              {
                      fX        = 0;
                      fY        = 0;
                      fWidth    = 0;
                      fHeight   = 0;
              }
              TEmfPlusRectF(const TEmfPlusRectF& oRect)
              {
                      fX        = oRect.fX;
                      fY        = oRect.fY;
                      fWidth    = oRect.fWidth;
                      fHeight   = oRect.fHeight;
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
                float fX;
                float fY;

                TEmfPlusPointF()
                {
                        fX = 0;
                        fY = 0;
                }

                TEmfPlusPointF(const TEmfPlusPointF& oPoint)
                {
                        fX = oPoint.fX;
                        fY = oPoint.fY;
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

        struct TGUID
        {
                int nData1;
                short shData2;
                short shData3;
                long long int llnData4;

                TGUID()
                {
                        nData1   = 0;
                        shData2  = 0;
                        shData3  = 0;
                        llnData4 = 0;
                }

                TGUID(const TGUID& oTGUID)
                {
                        nData1   = oTGUID.nData1;
                        shData2  = oTGUID.shData2;
                        shData3  = oTGUID.shData3;
                        llnData4 = oTGUID.llnData4;
                }
        };

        struct TEmfPlusTransformMatrix
        {
                float fM11;
                float fM12;
                float fM21;
                float fM22;
                float fdX;
                float fdY;

                TEmfPlusTransformMatrix()
                {
                        fM11 = 0;
                        fM12 = 0;
                        fM21 = 0;
                        fM22 = 0;
                        fdX  = 0;
                        fdY  = 0;
                }

                TEmfPlusTransformMatrix(const TEmfPlusTransformMatrix& oTEmfPlusTransformMatrix)
                {
                        fM11 = oTEmfPlusTransformMatrix.fM11;
                        fM12 = oTEmfPlusTransformMatrix.fM12;
                        fM21 = oTEmfPlusTransformMatrix.fM21;
                        fM22 = oTEmfPlusTransformMatrix.fM22;
                        fdX  = oTEmfPlusTransformMatrix.fdX;
                        fdY  = oTEmfPlusTransformMatrix.fdY;
                }
        };
}

#endif // EMFPLUSTYPES_H
