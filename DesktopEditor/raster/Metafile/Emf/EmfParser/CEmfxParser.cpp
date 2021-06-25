#include "CEmfxParser.h"

#include <iostream>

namespace MetaFile
{
    CEmfxParser::CEmfxParser():
        m_pOutput(new CXmlOutput(TypeXmlOutput::IsReader))
    {}

    CEmfxParser::~CEmfxParser()
    {
        delete m_pOutput;
    }

    bool CEmfxParser::IsValid() const
    {
        return m_pOutput->IsValid();
    }

    bool CEmfxParser::IsEof() const
    {
        return false;
    }

    unsigned int CEmfxParser::CanRead()
    {
        return 8;
    }

    void CEmfxParser::Skip(unsigned int unSkip)
    {

    }

    unsigned int CEmfxParser::Tell()
    {
        return 0;
    }

    void CEmfxParser::SeekToStart()
    {

    }

    void CEmfxParser::SetStream(CDataStream &pDataStream)
    {
        m_pOutput->ReadFromFile(L"test.xml");
    }

    CDataStream *CEmfxParser::GetDataStream() const
    {
        return NULL;
    }

    EmfParserType CEmfxParser::GetParserType() const
    {
        return EmfParserType::EmfxParser;
    }

    void CEmfxParser::ReadValue(TEmfAlphaBlend &oTEmfAlphaBlend)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfAlphaBlend;
    }

    void CEmfxParser::ReadValue(TEmfStretchDIBITS &oTEmfStretchDIBITS)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfStretchDIBITS;
    }

    void CEmfxParser::ReadValue(TEmfBitBlt &oTEmfBitBlt)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfBitBlt;
    }

    void CEmfxParser::ReadValue(TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfSetDiBitsToDevice;
    }

    void CEmfxParser::ReadValue(TEmfStretchBLT &oTEmfStretchBLT)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfStretchBLT;
    }

    void CEmfxParser::ReadValue(TXForm &oTXForm)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTXForm;
    }

    void CEmfxParser::ReadValue(CEmfLogBrushEx &oCEmfLogBrushEx)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oCEmfLogBrushEx;
    }

    void CEmfxParser::ReadValue(TEmfColor &oTEmfColor)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfColor;
    }

    void CEmfxParser::ReadValue(CEmfLogFont &oCEmfLogFont)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oCEmfLogFont;
    }

    void CEmfxParser::ReadValue(TEmfPointL &oTEmfPointL)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfPointL;
    }

    void CEmfxParser::ReadValue(TEmfPointS &oTEmfPointS)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfPointS;
    }

    void CEmfxParser::ReadValue(TEmfRectL &oTEmfRectL)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfRectL;
    }

    void CEmfxParser::ReadValue(TEmfSizeL &oTEmfSizeL)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfSizeL;
    }

    void CEmfxParser::ReadValue(TEmfDibPatternBrush &oTEmfDibPatternBrush)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfDibPatternBrush;
    }

    void CEmfxParser::ReadValue(CEmfLogPalette &oCEmfLogPalette)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oCEmfLogPalette;
    }

    void CEmfxParser::ReadValue(TEmfExtTextoutA &oTEmfExtTextoutA)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfExtTextoutA;
    }

    void CEmfxParser::ReadValue(TEmfExtTextoutW &oTEmfExtTextoutW)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfExtTextoutW;
    }

    void CEmfxParser::ReadValue(TEmfPolyTextoutA &oTEmfPolyTextoutA)
    {
        //TODO: Реализовать
//        if (NULL != m_pOutput)
//            *m_pOutput >> oTEmfPolyTextoutA;
    }

    void CEmfxParser::ReadValue(TEmfPolyTextoutW &oTEmfPolyTextoutW)
    {
        //TODO: Реализовать
//        if (NULL != m_pOutput)
        //            *m_pOutput >> oTEmfPolyTextoutW;
    }

    void CEmfxParser::ReadValue(TEmfSmallTextout &oTEmfSmallTextout)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oTEmfSmallTextout;
    }

    void CEmfxParser::ReadValue(CDataStream &oDataStream)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> oDataStream;
    }

    void CEmfxParser::ReadValue(unsigned short &ushValue)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> ushValue;
    }

    void CEmfxParser::ReadValue(unsigned char &ucValue)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> ucValue;
    }

    void CEmfxParser::ReadValue(unsigned int &unValue)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> unValue;
    }

    void CEmfxParser::ReadValue(double &dValue)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> dValue;
    }

    void CEmfxParser::ReadValue(int &nValue)
    {
        if (NULL != m_pOutput)
            *m_pOutput >> nValue;
    }

    void CEmfxParser::ReadValue(unsigned int &unValue1, unsigned int &unValue2)
    {
        while (!m_pOutput->IsRecord())
            m_pOutput->ReadNextNode();

        if (NULL != m_pOutput)
                m_pOutput->ReadArguments(unValue1, unValue2);

        if (m_pOutput->IsEmptyNode())
            m_pOutput->ReadNextNode();
    }

}
