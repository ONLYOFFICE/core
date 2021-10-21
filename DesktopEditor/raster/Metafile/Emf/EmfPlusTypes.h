#ifndef EMFPLUSTYPES_H
#define EMFPLUSTYPES_H

#include "EmfTypes.h"

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
                        shWidth  = 1024;
                        shHeight = 1024;
                }

                TEmfPlusRect(const TEmfPlusRect& oRect)
                {
                        shX      = oRect.shX;
                        shY      = oRect.shY;
                        shWidth  = oRect.shWidth;
                        shHeight = oRect.shHeight;
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

        #define TEmfPlusRectF   TRectD
        #define TEmfPlusPoint   TEmfPointS
        #define TEmfPlusPointF  TEmfPointD
        #define TEmfPlusXForm   TEmfXForm
}

#endif // EMFPLUSTYPES_H
