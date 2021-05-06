#ifndef CXMLOUTPUT_H
#define CXMLOUTPUT_H

#include "../../xml/include/xmlutils.h"
#include "Emf/EmfTypes.h"
#include "Emf/EmfObjects.h"

namespace MetaFile
{
    enum TypeXmlOutput
    {
        IsWriter = 0,
        IsReader = 1
    };

    struct XmlArgument
    {
        std::wstring wsName;
        std::wstring wsValue;

        XmlArgument(const std::wstring& wsNameArgument, int nValue)
            : wsName(wsNameArgument),
              wsValue(std::to_wstring(nValue)){}

        XmlArgument(const std::wstring& wsNameArgument, unsigned int nValue)
            : wsName(wsNameArgument),
              wsValue(std::to_wstring(nValue)){}
    };

    class CXmlOutput
    {
    public:
        CXmlOutput(TypeXmlOutput oTypeXmlOutput);
        ~CXmlOutput();

        void Clear();

        bool IsWriter();
        bool IsReader();

        // Запись в Xml файл

        void WriteString(const std::wstring& wsValue);

        void SaveToFile(const std::wstring& wsPathToFile);

        void WriteNodeBegin(const std::wstring& wsNameNode, std::vector<XmlArgument> arArguments = {});

        void WriteAttribute(const std::wstring& wsNameAttribute, const std::wstring& wsValue);
        void WriteAttribute(const std::wstring& wsNameAttribute, unsigned int unValue);
        void WriteAttribute(const std::wstring& wsNameAttribute, int nValue);

        void WriteNodeEnd(const std::wstring& wsNameAttribute, bool bEmptyNode = false, bool bEndNode = true);

        void WriteNode(const std::wstring& wsNameNode,  const std::wstring& wsValueNode);
        void WriteNode(const std::wstring& wsNameNode,  unsigned short usValueNode);
        void WriteNode(const std::wstring& wsNameNode,  unsigned char ucValueNode);
        void WriteNode(const std::wstring& wsNameNode,  unsigned int unValueNode);
        void WriteNode(const std::wstring& wsNameNode,  const BYTE* pValueNode, unsigned int unSizeValue);
        void WriteNode(const std::wstring& wsNameNode,  double dValueNode);
        void WriteNode(const std::wstring& wsNameNode,  short shValueNode);
        void WriteNode(const std::wstring& wsNameNode,  int nValueNode);

        void WriteNode(const std::wstring& wsNameNode, std::vector<XmlArgument> arArguments = {});

        void WriteNode(const std::wstring& wsNameNode,  const TXForm&                   oNodeValue, std::vector<XmlArgument> arArguments = {});

        void WriteNode(const std::wstring& wsNameNode,  const TEmfRectL&                oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfSizeL&                oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfAlphaBlend&           oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfColor&                oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfStretchDIBITS&        oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfBitBlt&               oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfSetDiBitsToDevice&    oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfStretchBLT&           oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfDibPatternBrush&      oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfExtTextoutA&          oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfPointL&               oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfPointS&               oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfEmrText&              oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfExtTextoutW&          oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const TEmfSmallTextout&         oNodeValue, std::vector<XmlArgument> arArguments = {});

        void WriteNode(const std::wstring& wsNameNode,  const CEmfLogBrushEx&           oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,        CEmfLogFont&              oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const CEmfLogPen&               oNodeValue, std::vector<XmlArgument> arArguments = {});
        void WriteNode(const std::wstring& wsNameNode,  const CEmfLogPalette&           oNodeValue, std::vector<XmlArgument> arArguments = {});

    private:
        XmlUtils::CXmlWriter        *m_pXmlWriter;

        void WriteTXForm                (const TXForm&                  oTXForm);

        void WriteTEmfRectL             (const TEmfRectL&               oTEmfRectL);
        void WriteTEmfSizeL             (const TEmfSizeL&               oTEmfSizeL);
        void WriteTEmfAlphaBlend        (const TEmfAlphaBlend&          oTEmfAlphaBlend);
        void WriteTEmfColor             (const TEmfColor&               oTEmfColor);
        void WriteTEmfStretchDIBITS     (const TEmfStretchDIBITS&       oTEmfStretchDIBITS);
        void WriteTEmfBitBlt            (const TEmfBitBlt&              oTEmfBitBlt);
        void WriteTEmfSetDiBitsToDevice (const TEmfSetDiBitsToDevice&   oTEmfSetDiBitsToDevice);
        void WriteTEmfStretchBLT        (const TEmfStretchBLT&          oTEmfStretchBLT);
        void WriteTEmfDibPatternBrush   (const TEmfDibPatternBrush&     oTEmfDibPatternBrush);
        void WriteTEmfExtTextoutA       (const TEmfExtTextoutA&         oTEmfExtTextoutA);
        void WriteTEmfEmrText           (const TEmfEmrText&             oTEmfEmrText);
        void WriteTEmfPointL            (const TEmfPointL&              oTEmfPointL);
        void WriteTEmfPointS            (const TEmfPointS&              oTEmfPointS);
        void WriteTEmfExtTextoutW       (const TEmfExtTextoutW&         oTEmfExtTextoutW);
        void WriteTEmfSmallTextout      (const TEmfSmallTextout&        oTEmfSmallTextout);

        void WriteEmfLogBrushEx         (const CEmfLogBrushEx&          oEmfLogBrushEx);
        void WriteEmfLogFont            (      CEmfLogFont&             oEmfLogFont);
        void WriteEmfLogPen             (const CEmfLogPen&              oEmfLogPen);
        void WriteEmfLogPalette         (const CEmfLogPalette&          oEmfLogPalette);

        // Чтение из Xml файла

    public:
        bool ReadFromFile(const std::wstring wsPathToFile);
        void ReadArguments(unsigned int &unType,
                           unsigned int &unSize);
        void ReadNextRecord();

        void operator>>(TEmfHeader&     oTEmfHeader);
        void operator>>(TEmfAlphaBlend& oTEmfAlphaBlend);
        void operator>>(CEmfLogBrushEx& oCEmfLogBrushEx);
        void operator>>(TEmfBitBlt&     oTEmfBitBlt);
        void operator>>(CEmfLogFont&    oCEmfLogFont);
        void operator>>(TEmfExtTextoutW& oTEmfExtTextoutW);
        void operator>>(TEmfEmrText&    oTEmfEmrText);

        void operator>>(TXForm&         oTXForm);
        void operator>>(TEmfColor&      oTEmfColor);

        void operator>>(unsigned int&   unValue);

    private:
        XmlUtils::CXmlLiteReader    *m_pXmlLiteReader;

        void operator>>(TEmfRectL&          oTEmfRectL);
        void operator>>(TEmfSizeL&          oTEmfSizeL);
        void operator>>(TRect&              oTRect);
        void operator>>(TEmfLogFontEx&      oTEmfLogFontEx);
        void operator>>(TEmfLogFont&        oTEmfLogFont);
        void operator>>(TEmfDesignVector&   oTEmfDesignVector);
        void operator>>(TEmfPointL&         oTEmfPointL);

        void operator>>(int&            nValue);
        void operator>>(double&         dValue);
        void operator>>(unsigned short& ushValue);
        void operator>>(unsigned char&  ucValue);
        void operator>>(unsigned short  arushValue[]);
    };
}
#endif // CXMLOUTPUT_H
