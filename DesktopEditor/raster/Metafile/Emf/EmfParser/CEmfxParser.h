#ifndef CEMFXPARSER_H
#define CEMFXPARSER_H

#include "CEmfParserBase.h"
#include "../../CXmlOutput.h"

namespace MetaFile
{
    class CEmfxParser : public CEmfParserBase
    {
    public:
        virtual bool            IsValid()                 const = 0;
        virtual bool            IsEof()                   const = 0;
        virtual unsigned int    CanRead()                       = 0;
        virtual void            Skip(unsigned int)              = 0;
        virtual unsigned int    Tell()                          = 0;
        virtual void            SeekToStart()                   = 0;
        virtual void            SetStream (CDataStream&)        = 0;

        virtual void ReadValue (TEmfAlphaBlend&)        = 0;
        virtual void ReadValue (TEmfStretchDIBITS&)     = 0;
        virtual void ReadValue (TEmfBitBlt&)            = 0;
        virtual void ReadValue (TEmfSetDiBitsToDevice&) = 0;
        virtual void ReadValue (TEmfStretchBLT&)        = 0;
        virtual void ReadValue (TXForm&)                = 0;
        virtual void ReadValue (CEmfLogBrushEx&)        = 0;
        virtual void ReadValue (TEmfColor&)             = 0;
        virtual void ReadValue (CEmfLogFont&)           = 0;
        virtual void ReadValue (TEmfPointL&)            = 0;
        virtual void ReadValue (TEmfPointS&)            = 0;
        virtual void ReadValue (TEmfRectL&)             = 0;
        virtual void ReadValue (TEmfSizeL&)             = 0;
        virtual void ReadValue (TEmfDibPatternBrush&)   = 0;
        virtual void ReadValue (CEmfLogPalette&)        = 0;
        virtual void ReadValue (TEmfExtTextoutA&)       = 0;
        virtual void ReadValue (TEmfExtTextoutW&)       = 0;
        virtual void ReadValue (TEmfPolyTextoutA&)      = 0;
        virtual void ReadValue (TEmfPolyTextoutW&)      = 0;
        virtual void ReadValue (TEmfSmallTextout&)      = 0;

        virtual void ReadValue (unsigned short&)   = 0;
        virtual void ReadValue (unsigned char&)    = 0;
        virtual void ReadValue (unsigned int&)     = 0;
        virtual void ReadValue (double&)           = 0;
        virtual void ReadValue (int&)              = 0;

    private:
    };
}

#endif // CEMFXPARSER_H
