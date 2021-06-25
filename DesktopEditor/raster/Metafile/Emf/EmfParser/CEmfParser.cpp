#include "CEmfParser.h"

namespace MetaFile
{
    CEmfParser::CEmfParser():
        m_pStream(new CDataStream)
    {}

    CEmfParser::~CEmfParser()
    {
        m_pStream = NULL;
    }

    bool CEmfParser::IsValid() const
    {
        return m_pStream->IsValid();
    }

    bool CEmfParser::IsEof() const
    {
        return m_pStream->IsEof();
    }

    unsigned int CEmfParser::CanRead()
    {
        return m_pStream->CanRead();
    }

    void CEmfParser::Skip(unsigned int unSkip)
    {
        m_pStream->Skip(unSkip);
    }

    unsigned int CEmfParser::Tell()
    {
        return m_pStream->Tell();
    }

    void CEmfParser::SeekToStart()
    {
        m_pStream->SeekToStart();
    }

    void CEmfParser::SetStream(CDataStream &pDataStream)
    {
        m_pStream = &pDataStream;
    }

    CDataStream *CEmfParser::GetDataStream() const
    {
        return m_pStream;
    }

    EmfParserType CEmfParser::GetParserType() const
    {
        return EmfParserType::EmfParser;
    }

    void CEmfParser::ReadValue(TEmfAlphaBlend &oTEmfAlphaBlend)
    {
        *m_pStream >> oTEmfAlphaBlend;
    }

    void CEmfParser::ReadValue(TEmfStretchDIBITS &oTEmfStretchDIBITS)
    {
        *m_pStream >> oTEmfStretchDIBITS;
    }

    void CEmfParser::ReadValue(TEmfBitBlt &oTEmfBitBlt)
    {
        *m_pStream >> oTEmfBitBlt;
    }

    void CEmfParser::ReadValue(TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
    {
        *m_pStream >> oTEmfSetDiBitsToDevice;
    }

    void CEmfParser::ReadValue(TEmfStretchBLT &oTEmfStretchBLT)
    {
        *m_pStream >> oTEmfStretchBLT;
    }

    void CEmfParser::ReadValue(TXForm &oTXForm)
    {
        *m_pStream >> oTXForm;
    }

    void CEmfParser::ReadValue(CEmfLogBrushEx &oCEmfLogBrushEx)
    {
        *m_pStream >> oCEmfLogBrushEx;
    }

    void CEmfParser::ReadValue(TEmfColor &oTEmfColor)
    {
        *m_pStream >> oTEmfColor;
    }

    void CEmfParser::ReadValue(CEmfLogFont &oCEmfLogFont)
    {
        *m_pStream >> oCEmfLogFont;
    }

    void CEmfParser::ReadValue(TEmfPointL &oTEmfPointL)
    {
        *m_pStream >> oTEmfPointL;
    }

    void CEmfParser::ReadValue(TEmfPointS &oTEmfPointS)
    {
        *m_pStream >> oTEmfPointS;
    }


    void CEmfParser::ReadValue(TEmfRectL &oTEmfRectL)
    {
        *m_pStream >> oTEmfRectL;
    }

    void CEmfParser::ReadValue(TEmfSizeL &oTEmfSizeL)
    {
        *m_pStream >> oTEmfSizeL;
    }

    void CEmfParser::ReadValue(TEmfDibPatternBrush &oTEmfDibPatternBrush)
    {
        *m_pStream >> oTEmfDibPatternBrush;
    }

    void CEmfParser::ReadValue(CEmfLogPalette &oCEmfLogPalette)
    {
        *m_pStream >> oCEmfLogPalette;
    }

    void CEmfParser::ReadValue(TEmfExtTextoutA &oTEmfExtTextoutA)
    {
        *m_pStream >> oTEmfExtTextoutA;
    }

    void CEmfParser::ReadValue(TEmfExtTextoutW &oTEmfExtTextoutW)
    {
        *m_pStream >> oTEmfExtTextoutW;
    }

    void CEmfParser::ReadValue(TEmfPolyTextoutA &oTEmfPolyTextoutA)
    {
        *m_pStream >> oTEmfPolyTextoutA;
    }

    void CEmfParser::ReadValue(TEmfPolyTextoutW &oTEmfPolyTextoutW)
    {
        *m_pStream >> oTEmfPolyTextoutW;
    }

    void CEmfParser::ReadValue(TEmfSmallTextout &oTEmfSmallTextout)
    {
        *m_pStream >> oTEmfSmallTextout;
    }

    void CEmfParser::ReadValue(CDataStream &oDataStream)
    {
        //Ничего не делаем
    }

    void CEmfParser::ReadValue(unsigned short &ushValue)
    {
        *m_pStream >> ushValue;
    }

    void CEmfParser::ReadValue(unsigned char &ucValue)
    {
        *m_pStream >> ucValue;
    }

    void CEmfParser::ReadValue(unsigned int &unValue)
    {
        *m_pStream >> unValue;
    }

    void CEmfParser::ReadValue(double &dValue)
    {
        *m_pStream >> dValue;
    }

    void CEmfParser::ReadValue(int &nValue)
    {
        *m_pStream >> nValue;
    }

    void CEmfParser::ReadValue(unsigned int &unValue1, unsigned int &unValue2)
    {
        *m_pStream >> unValue1;
        *m_pStream >> unValue2;
    }
}
