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

        bool            OpenFromFile(const wchar_t* wsFilePath)     override;
        void            SetError()                                  override;
        void            PlayFile()                                  override;
        void            Scan()                                      override;
        bool            CheckError()                                override;
        void            ClearFile()                                 override;
        void            SetFontManager(CFontManager* pFontManager)  override;

        EmfParserType   GetType()				    override;

//        void ReadValue (TEmfAlphaBlend& oTEmfAlphaBlend)               override;
//        void ReadValue (TEmfStretchDIBITS& oTEmfStretchDIBITS)         override;
//        void ReadValue (TEmfBitBlt& oTEmfBitBlt)                       override;
//        void ReadValue (TEmfSetDiBitsToDevice& oTEmfSetDiBitsToDevice) override;
//        void ReadValue (TEmfStretchBLT& oTEmfStretchBLT)               override;
//        void ReadValue (TXForm& oTXForm)                               override;
//        void ReadValue (CEmfLogBrushEx& oCEmfLogBrushEx)               override;
//        void ReadValue (TEmfColor& oTEmfColor)                         override;
//        void ReadValue (CEmfLogFont& oCEmfLogFont)                     override;
//        void ReadValue (TEmfPointL& oTEmfPointL)                       override;
//        void ReadValue (TEmfPointS& oTEmfPointS)                       override;
//        void ReadValue (TEmfRectL& oTEmfRectL)                         override;
//        void ReadValue (TEmfSizeL& oTEmfSizeL)                         override;
//        void ReadValue (TEmfDibPatternBrush& oTEmfDibPatternBrush)     override;
//        void ReadValue (CEmfLogPalette& oCEmfLogPalette)               override;
//        void ReadValue (TEmfExtTextoutA& oTEmfExtTextoutA)             override;
//        void ReadValue (TEmfExtTextoutW& oTEmfExtTextoutW)             override;
//        void ReadValue (TEmfPolyTextoutA& oTEmfPolyTextoutA)           override;
//        void ReadValue (TEmfPolyTextoutW& oTEmfPolyTextoutW)           override;
//        void ReadValue (TEmfSmallTextout& oTEmfSmallTextout)           override;
//        void ReadValue (CDataStream& oDataStream)                      override;

//        void ReadValue (unsigned short& ushValue)  override;
//        void ReadValue (unsigned char& ucValue)    override;
//        void ReadValue (unsigned int&   unValue)   override;
//        void ReadValue (double& dValue)            override;
//        void ReadValue (int& nValue)               override;

//        void ReadValue (unsigned int& unValue1, unsigned int& unValue2) override;
    private:
        bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight) override;

        CXmlOutput  *m_pOutput;        
    };
}

#endif // CEMFXPARSER_H
