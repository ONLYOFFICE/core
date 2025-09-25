#ifndef CXMLOUTPUT_H
#define CXMLOUTPUT_H

#include "../../xml/include/xmlutils.h"
#include "Common/MetaFileUtils.h"
#include "Emf/EmfTypes.h"
#include "Emf/EmfObjects.h"

namespace MetaFile
{
	enum TypeXmlOutput
	{
		IsWriter = 0,
		IsReader = 1
	};

	class CXmlOutput
	{
	public:
		CXmlOutput(TypeXmlOutput oTypeXmlOutput);
		~CXmlOutput();

		void Clear();

		bool IsWriter()     const;
		bool IsReader()     const;
		bool IsValid()      const;
		bool IsEmptyNode()  const;
		bool MoveToStart();

		// Запись в Xml файл

		void WriteString(const std::wstring& wsValue);

		void SaveToFile(const std::wstring& wsPathToFile);

		void WriteNodeBegin(const std::wstring& wsNameNode);

		void WriteAttribute(const std::wstring& wsNameAttribute, const std::wstring& wsValue);
		void WriteAttribute(const std::wstring& wsNameAttribute, unsigned int unValue);
		void WriteAttribute(const std::wstring& wsNameAttribute, int nValue);

		void WriteNodeEnd(const std::wstring& wsNameAttribute, bool bEmptyNode = false, bool bEndNode = true);

		void WriteNode(const std::wstring& wsNameNode,  const std::wstring& wsValueNode);
		void WriteNode(const std::wstring& wsNameNode,  unsigned short usValueNode);
		void WriteNode(const std::wstring& wsNameNode,  unsigned char ucValueNode);
		void WriteNode(const std::wstring& wsNameNode,  unsigned int unValueNode);
		void WriteNode(const std::wstring& wsNameNode,  CDataStream& oDataStream, unsigned int unSizeValue);
		void WriteNode(const std::wstring& wsNameNode,  double dValueNode);
		void WriteNode(const std::wstring& wsNameNode,  short shValueNode);
		void WriteNode(const std::wstring& wsNameNode,  int nValueNode);

		void WriteNode(const std::wstring& wsNameNode);

		void WriteNode(const std::wstring& wsNameNode,  const TXForm&                   oNodeValue);

		void WriteNode(const std::wstring& wsNameNode,  const TRectL&                   oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfPlusRect&             oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfPlusRectF&            oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TSizeL&                   oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfAlphaBlend&           oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TRGBA&                    oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfPlusARGB&             oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfStretchDIBITS&        oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfBitBlt&               oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfSetDiBitsToDevice&    oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfStretchBLT&           oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfDibPatternBrush&      oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfExtTextoutA&          oNodeValue);
//		void WriteNode(const std::wstring& wsNameNode,  const TPointL&                  oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TPointS&                  oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfPlusPointF&           oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmrTextW&                oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmrTextA&                oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfExtTextoutW&          oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const TEmfSmallTextout&         oNodeValue);

		void WriteNode(const std::wstring& wsNameNode,  const CEmfLogBrushEx&           oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,        CEmfLogFont&              oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const CEmfLogPen&               oNodeValue);
		void WriteNode(const std::wstring& wsNameNode,  const CEmfLogPalette&           oNodeValue);

		void WriteNode(const std::wstring& wsNameNode, const std::vector<TEmfPlusPointF>& arValues);
		void WriteNode(const std::wstring& wsNameNode, const TRegionDataHeader& oRegionDataHeader, const std::vector<TRectL>& arRects);

	private:
		XmlUtils::CXmlWriter        *m_pXmlWriter;

		void WriteTXForm                (const TXForm&                  oTXForm);

		void WriteTRectL                (const TRectL&                  oTRectL);
		void WriteTEmfPlusRect          (const TEmfPlusRect&            oTEmfPlusRect);
		void WriteTEmfPlusRectF         (const TEmfPlusRectF&           oTEmfPlusRectF);
		void WriteTSizeL                (const TSizeL&                  oTSizeL);
		void WriteTEmfAlphaBlend        (const TEmfAlphaBlend&          oTEmfAlphaBlend);
		void WriteTRGBA                 (const TRGBA&                   oTRGBA);
		void WriteARGB                  (const TEmfPlusARGB&            oTEmfPlusARGB);
		void WriteTEmfStretchDIBITS     (const TEmfStretchDIBITS&       oTEmfStretchDIBITS);
		void WriteTEmfBitBlt            (const TEmfBitBlt&              oTEmfBitBlt);
		void WriteTEmfSetDiBitsToDevice (const TEmfSetDiBitsToDevice&   oTEmfSetDiBitsToDevice);
		void WriteTEmfStretchBLT        (const TEmfStretchBLT&          oTEmfStretchBLT);
		void WriteTEmfDibPatternBrush   (const TEmfDibPatternBrush&     oTEmfDibPatternBrush);
		void WriteTEmfExtTextoutA       (const TEmfExtTextoutA&         oTEmfExtTextoutA);
		void WriteTEmrText              (const TEmrTextW&               oTEmfEmrText);
		void WriteTEmrText              (const TEmrTextA&               oTEmfEmrText);
		void WriteTPointL               (const TPointL&                 oTPointL);
		void WriteTPointS               (const TPointS&                 oTPointS);
		void WriteTEmfPlusPointF        (const TEmfPlusPointF&          oTEmfPlusPointF);
		void WriteTEmfExtTextoutW       (const TEmfExtTextoutW&         oTEmfExtTextoutW);
		void WriteTEmfSmallTextout      (const TEmfSmallTextout&        oTEmfSmallTextout);
		void WriteTRegionDataHeader     (const TRegionDataHeader&       oTRegionDataHeader);

		void WriteEmfLogBrushEx         (const CEmfLogBrushEx&          oEmfLogBrushEx);
		void WriteEmfLogFont            (      CEmfLogFont&             oEmfLogFont);
		void WriteEmfLogPen             (const CEmfLogPen&              oEmfLogPen);
		void WriteEmfLogPalette         (const CEmfLogPalette&          oEmfLogPalette);

		void WriteNode(const std::wstring& wsNameNode, const unsigned int* pUnValues, const unsigned int& unSize);

		// Чтение из Xml файла

	public:
		bool ReadFromFile(const std::wstring &wsPathToFile);
		bool ReadFromBuffer(wchar_t* pBuffer);
		void ReadArguments(unsigned int &unType,
						   unsigned int &unSize);
		bool ReadNextRecord();
		bool ReadNextNode();

		bool IsRecord() const;

		std::wstring GetName() const;

		void operator>>(TEmfHeader&             oTEmfHeader);
		void operator>>(TEmfAlphaBlend&         oTEmfAlphaBlend);
		void operator>>(CEmfLogBrushEx&         oCEmfLogBrushEx);
		void operator>>(TEmfBitBlt&             oTEmfBitBlt);
		void operator>>(TEmfStretchDIBITS&      oTEmfStretchDIBITS);
		void operator>>(TEmfSetDiBitsToDevice&  oTEmfSetDiBitsToDevice);
		void operator>>(TEmfStretchBLT&         oTEmfStretchBLT);
		void operator>>(CEmfLogFont&            oCEmfLogFont);
		void operator>>(TEmfExtTextoutW&        oTEmfExtTextoutW);
		void operator>>(TEmfExtTextoutA&        oTEmfExtTextoutA);
		void operator>>(TEmfSmallTextout&       oTEmfSmallTextout);
		void operator>>(TEmrTextW&              oTEmrText);
		void operator>>(TEmrTextA&              oTEmrText);
		void operator>>(TEmfDibPatternBrush&    oTEmfDibPatternBrush);
		void operator>>(CEmfLogPalette&         oCEmfLogPalette);
		void operator>>(CDataStream&            oCDataStream);

		void operator>>(TXForm&      oTXForm);
		void operator>>(TRGBA&       oTRGBA);
		void operator>>(TPointL&     oTPointL);
		void operator>>(TPointS&     oTPointS);
//		void operator>>(TSizeL&      oTSizeL);
		void operator>>(TRectL&      oTRectL);

		void operator>>(std::vector<TPointL> &arPoints);
		void operator>>(std::vector<TPointS> &arPoints);
		void operator>>(std::vector<TRectL>  &arRects);
		void operator>>(std::vector<std::vector<TPointL>>& arPoints);
		void operator>>(std::vector<std::vector<TPointS>>& arPoints);

		void operator>>(unsigned short& ushValue);
		void operator>>(unsigned char&  ucValue);
		void operator>>(unsigned int&   unValue);
		void operator>>(double&         dValue);
		void operator>>(int&            nValue);

	private:
		XmlUtils::CXmlLiteReader    *m_pXmlLiteReader;

		void operator>>(TEmfLogFontEx&      oTEmfLogFontEx);
		void operator>>(TEmfLogFont&        oTEmfLogFont);
		void operator>>(TEmfDesignVector&   oTEmfDesignVector);
		void operator>>(TEmfLogPaletteEntry&oTEmfLogPaletteEntry);

		void operator>>(short&          shValue);
		void operator>>(unsigned short  arushValue[]);
		void operator>>(unsigned char   aruchValue[]);
		void operator>>(std::wstring&   wsValue);
		void operator>>(std::string&    sValue);

		void ReadPoint(TPointL& oPoint);
		void ReadPoint(TPointS& oPoint);
		void ReadDx(unsigned int  arunValue[], const unsigned int& unCount);
	};
}
#endif // CXMLOUTPUT_H
