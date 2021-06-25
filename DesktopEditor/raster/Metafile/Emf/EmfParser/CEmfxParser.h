#ifndef CEMFXPARSER_H
#define CEMFXPARSER_H

#include "CEmfParserBase.h"
#include "../../CXmlOutput.h"

namespace MetaFile
{
    class CEmfxParser : public CEmfParserBase
    {
    public:
        CEmfxParser();
        virtual ~CEmfxParser();

        bool            IsValid()                             const override;
        bool            IsEof()                               const override;
        unsigned int    CanRead()                                   override;
        void            Skip(unsigned int unSkip)                   override;
        unsigned int    Tell()                                      override;
        void            SeekToStart()                               override;
        void            SetStream (CDataStream& pDataStream)        override;
        CDataStream*    GetDataStream()                       const override;
        EmfParserType   GetParserType()                       const override;

        void ReadValue (TEmfAlphaBlend& oTEmfAlphaBlend)               override;
        void ReadValue (TEmfStretchDIBITS& oTEmfStretchDIBITS)         override;
        void ReadValue (TEmfBitBlt& oTEmfBitBlt)                       override;
        void ReadValue (TEmfSetDiBitsToDevice& oTEmfSetDiBitsToDevice) override;
        void ReadValue (TEmfStretchBLT& oTEmfStretchBLT)               override;
        void ReadValue (TXForm& oTXForm)                               override;
        void ReadValue (CEmfLogBrushEx& oCEmfLogBrushEx)               override;
        void ReadValue (TEmfColor& oTEmfColor)                         override;
        void ReadValue (CEmfLogFont& oCEmfLogFont)                     override;
        void ReadValue (TEmfPointL& oTEmfPointL)                       override;
        void ReadValue (TEmfPointS& oTEmfPointS)                       override;
        void ReadValue (TEmfRectL& oTEmfRectL)                         override;
        void ReadValue (TEmfSizeL& oTEmfSizeL)                         override;
        void ReadValue (TEmfDibPatternBrush& oTEmfDibPatternBrush)     override;
        void ReadValue (CEmfLogPalette& oCEmfLogPalette)               override;
        void ReadValue (TEmfExtTextoutA& oTEmfExtTextoutA)             override;
        void ReadValue (TEmfExtTextoutW& oTEmfExtTextoutW)             override;
        void ReadValue (TEmfPolyTextoutA& oTEmfPolyTextoutA)           override;
        void ReadValue (TEmfPolyTextoutW& oTEmfPolyTextoutW)           override;
        void ReadValue (TEmfSmallTextout& oTEmfSmallTextout)           override;
        void ReadValue (CDataStream& oDataStream)                      override;

        void ReadValue (unsigned short& ushValue)  override;
        void ReadValue (unsigned char& ucValue)    override;
        void ReadValue (unsigned int&   unValue)   override;
        void ReadValue (double& dValue)            override;
        void ReadValue (int& nValue)               override;

        void ReadValue (unsigned int& unValue1, unsigned int& unValue2) override;
    private:
        CXmlOutput  *m_pOutput;
    };
}

#endif // CEMFXPARSER_H
