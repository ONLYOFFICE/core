#ifndef CEMFINTERPRETARORBASE_H
#define CEMFINTERPRETARORBASE_H

#include "../EmfTypes.h"
#include "../EmfObjects.h"
#include "../../Common/IOutputDevice.h"

namespace MetaFile
{
    class CEmfInterpretarorBase : public IOutputDevice
    {
    public:
        virtual void SaveValue (TEmfAlphaBlend&)        = 0;
        virtual void SaveValue (TEmfStretchDIBITS&)     = 0;
        virtual void SaveValue (TEmfBitBlt&)            = 0;
        virtual void SaveValue (TEmfSetDiBitsToDevice&) = 0;
        virtual void SaveValue (TEmfStretchBLT&)        = 0;
        virtual void SaveValue (TXForm&)                = 0;
        virtual void SaveValue (CEmfLogBrushEx&)        = 0;
        virtual void SaveValue (TEmfColor&)             = 0;
        virtual void SaveValue (CEmfLogFont&)           = 0;
        virtual void SaveValue (TEmfPointL&)            = 0;
        virtual void SaveValue (TEmfPointS&)            = 0;
        virtual void SaveValue (TEmfRectL&)             = 0;
        virtual void SaveValue (TEmfSizeL&)             = 0;
        virtual void SaveValue (TEmfDibPatternBrush&)   = 0;
        virtual void SaveValue (CEmfLogPalette&)        = 0;
        virtual void SaveValue (TEmfExtTextoutA&)       = 0;
        virtual void SaveValue (TEmfExtTextoutW&)       = 0;
        virtual void SaveValue (TEmfPolyTextoutA&)      = 0;
        virtual void SaveValue (TEmfPolyTextoutW&)      = 0;
        virtual void SaveValue (TEmfSmallTextout&)      = 0;

        virtual void SaveValue (unsigned short&)   = 0;
        virtual void SaveValue (unsigned char&)    = 0;
        virtual void SaveValue (unsigned int&)     = 0;
        virtual void SaveValue (double&)           = 0;
        virtual void SaveValue (int&)              = 0;
    };
}

#endif // CEMFINTERPRETARORBASE_H
