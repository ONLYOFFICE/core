#include "CEmfInterpretator.h"
#include "../../../../common/File.h"

namespace MetaFile
{

	CEmfInterpretator::CEmfInterpretator(const wchar_t* wsFilepath) :
		unFileSize(0), unNumberRecords(0), ushNuberDescriptors(0)
	{
		m_pOutStream = new NSFile::CFileBinary();
		m_pOutStream->CreateFileW(wsFilepath);
	}

	CEmfInterpretator::CEmfInterpretator(const CEmfInterpretator& oEmfInterpretator, const bool bIsLite)
		: m_pOutStream(oEmfInterpretator.m_pOutStream),
			unFileSize(0), unNumberRecords(0), ushNuberDescriptors(0)
	{
		if (!bIsLite)
		{
			unFileSize          = oEmfInterpretator.unFileSize;
			unNumberRecords     = oEmfInterpretator.unNumberRecords;
			ushNuberDescriptors = oEmfInterpretator.ushNuberDescriptors;
		}
	}

	CEmfInterpretator::~CEmfInterpretator()
	{
		m_pOutStream->CloseFile();
		delete m_pOutStream;
	}

	InterpretatorType CEmfInterpretator::GetType() const
	{
		return InterpretatorType::Emf;
	}

	void CEmfInterpretator::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		int unExplicitRecordSize = 88;
		int unType               = EMR_HEADER;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfHeader.oBounds);
		WriteRectangle(oTEmfHeader.oFrame);

		unsigned int unZero = 0;

		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulSignature, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulVersion,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulSize,      sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulRecords,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ushObjects,  sizeof (unsigned short));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ushReserved, sizeof (unsigned short));
		m_pOutStream->WriteFile((BYTE*)&unZero,                  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unZero,                  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unZero,                  sizeof (unsigned int));

		WriteSize(oTEmfHeader.oDevice);
		WriteSize(oTEmfHeader.oMillimeters);
	}

	void CEmfInterpretator::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 108 + oTEmfAlphaBlend.unCbBitsSrc + oTEmfAlphaBlend.unCbBmiSrc;
		int unType               = EMR_ALPHABLEND;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfAlphaBlend.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nXDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nYDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nCxDest,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nCyDest,    sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.uchBlendOperation,    sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.uchBlendFlags,        sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.uchSrcConstantAlpha,  sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.uchAlphaFormat,       sizeof (unsigned char));

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nXSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nYSrc,      sizeof (int));

		WriteForm(oTEmfAlphaBlend.oXformSrc);
		WriteColor(oTEmfAlphaBlend.oBkColor);

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.unUsageSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.unOffBmiSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.unCbBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.unOffBitsSrc,sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.unCbBitsSrc, sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nCxSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.nCySrc,      sizeof (int));

		if (oTEmfAlphaBlend.unCbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfAlphaBlend.unCbBmiSrc);

		if (oTEmfAlphaBlend.unCbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfAlphaBlend.unCbBmiSrc, sizeof (BYTE) * oTEmfAlphaBlend.unCbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 80 + oTEmfStretchDIBITS.unCbBitsSrc + oTEmfStretchDIBITS.unCbBmiSrc;
		int unType               = EMR_STRETCHDIBITS;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfStretchDIBITS.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nXDest,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nYDest,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nXSrc,        sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nYSrc,        sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nCxSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nCySrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.unOffBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.unCbBmiSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.unOffBitsSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.unCbBitsSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.unUsageSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.\
		                        unBitBltRasterOperation,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nCxDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.nCyDest,      sizeof (int));

		if (oTEmfStretchDIBITS.unCbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfStretchDIBITS.unCbBmiSrc);

		if (oTEmfStretchDIBITS.unCbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfStretchDIBITS.unCbBmiSrc, sizeof (BYTE) * oTEmfStretchDIBITS.unCbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 100 + oTEmfBitBlt.unCbBitsSrc + oTEmfBitBlt.unCbBmiSrc;
		int unType               = EMR_BITBLT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfBitBlt.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.nXDest,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.nYDest,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.nCxDest,   sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.nCyDest,   sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.\
								unBitBltRasterOperation,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.nXSrc,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.nYSrc,     sizeof (int));

		WriteForm(oTEmfBitBlt.oXfromSrc);
		WriteColor(oTEmfBitBlt.oBkColorSrc);

		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.unUsageSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.unOffBmiSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.unCbBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.unOffBitsSrc,sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.unCbBitsSrc, sizeof (unsigned int));

		if (oTEmfBitBlt.unCbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfBitBlt.unCbBmiSrc);

		if (oTEmfBitBlt.unCbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfBitBlt.unCbBmiSrc, sizeof (BYTE) * oTEmfBitBlt.unCbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 76 + oTEmfSetDiBitsToDevice.unCbBitsSrc + oTEmfSetDiBitsToDevice.unCbBmiSrc;
		int unType               = EMR_SETDIBITSTODEVICE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfSetDiBitsToDevice.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.nXDest,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.nYDest,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.nXSrc,        sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.nYSrc,        sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.nCxSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.nCySrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unOffBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unCbBmiSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unOffBitsSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unCbBitsSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unUsageSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unIStartScan, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.unCScans,     sizeof (unsigned int));

		if (oTEmfSetDiBitsToDevice.unCbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfSetDiBitsToDevice.unCbBmiSrc);

		if (oTEmfSetDiBitsToDevice.unCbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfSetDiBitsToDevice.unCbBmiSrc, sizeof (BYTE) * oTEmfSetDiBitsToDevice.unCbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 84 + oTEmfStretchBLT.unCbBitsSrc + oTEmfStretchBLT.unCbBmiSrc;
		int unType               = EMR_STRETCHBLT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfStretchBLT.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nXDest,  sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nYDest,  sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nCxDest, sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nCyDest, sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.\
		                        unBitBltRasterOperation,         sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nXSrc,   sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nYSrc,   sizeof (int));

		WriteForm(oTEmfStretchBLT.oXformSrc);
		WriteColor(oTEmfStretchBLT.oBkColorSrc);

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.unUsageSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.unOffBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.unCbBmiSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.unOffBitsSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.unCbBitsSrc,  sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nCxSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.nCySrc,       sizeof (int));

		if (oTEmfStretchBLT.unCbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfStretchBLT.unCbBmiSrc);

		if (oTEmfStretchBLT.unCbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfStretchBLT.unCbBmiSrc, sizeof (BYTE) * oTEmfStretchBLT.unCbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_EOF()
	{
		int unExplicitRecordSize = 20;
		int unType               = EMR_EOF;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		unsigned int unZero = 0;
		unsigned int unOffPalEntries = 16;

		m_pOutStream->WriteFile((BYTE*)&unZero,                sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unOffPalEntries,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (unsigned int));

		m_pOutStream->SeekFile(48);
		m_pOutStream->WriteFile((BYTE*)&unFileSize,            sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unNumberRecords,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&ushNuberDescriptors,   sizeof (unsigned short));

		m_pOutStream->CloseFile();
	}

	void CEmfInterpretator::HANDLE_EMR_SAVEDC()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_SAVEDC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_RESTOREDC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&nIndexDC,              sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
	{
		int unExplicitRecordSize = 36;
		int unType               = EMR_MODIFYWORLDTRANSFORM;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteForm(oXForm);

		m_pOutStream->WriteFile((BYTE*)&unMode,                sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
	{
		int unExplicitRecordSize = 32;
		int unType               = EMR_SETWORLDTRANSFORM;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteForm(oXForm);
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_CREATEBRUSHINDIRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,   sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBrushIndex,           sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&(pBrush->unBrushStyle), sizeof (unsigned int));

		WriteColor(pBrush->oColor);

		m_pOutStream->WriteFile((BYTE*)&(pBrush->unBrushHatch),  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETTEXTCOLOR(const TRGBA &oColor)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETTEXTCOLOR;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteColor(oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SELECTOBJECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unObjectIndex,  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont)
	{
		if (NULL == oLogFont)
			return;

		int unExplicitRecordSize = (oLogFont->IsFixedLength()) ? 332 : (368 + oLogFont->oDesignVector.unNumAxes * sizeof(int));
		int unType               = EMR_EXTCREATEFONTINDIRECTW;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unIndex,               sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.nHeight,          sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.nWidth,           sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.nEscapement,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.nOrientation,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.nWeight,          sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchItalic,        sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchUnderline,     sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchStrikeOut,     sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchCharSet,       sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchOutPrecision,  sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchClipPrecision, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchQuality,       sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->oLogFontEx.oLogFont.uchPitchAndFamily,sizeof (unsigned char));
		
		WriteString(oLogFont->oLogFontEx.oLogFont.ushFaceName, 32);
		WriteString(oLogFont->oLogFontEx.ushFullName,          64);
		WriteString(oLogFont->oLogFontEx.ushStyle,             32);
		WriteString(oLogFont->oLogFontEx.ushScript,            32);

		if (!oLogFont->IsFixedLength())
		{
			m_pOutStream->WriteFile((BYTE*)&oLogFont->oDesignVector.unSignature,     sizeof (unsigned int));
			m_pOutStream->WriteFile((BYTE*)&oLogFont->oDesignVector.unNumAxes,       sizeof (unsigned int));

			for (unsigned int i = 0; i < oLogFont->oDesignVector.unNumAxes; ++i)
				m_pOutStream->WriteFile((BYTE*)&oLogFont->oDesignVector.pValues[i], sizeof (int));
		}
	}

	void CEmfInterpretator::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETTEXTALIGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unAlign,               sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETBKMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBgMode,              sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_DELETEOBJECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unObjectIndex,  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETMITERLIMIT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unMeterLimit,          sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const std::vector<unsigned int>& arUnused)
	{
		if(NULL == pPen || arUnused.size() < 2)
			return;

		int unExplicitRecordSize = 52 + sizeof (unsigned int) * pPen->unNumStyleEntries + ((pPen->unPenStyle != PS_USERSTYLE) ? 4 : 0);
		int unType               = EMR_EXTCREATEPEN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPenIndex,            sizeof (unsigned int));
		//Заполняем нулями некоторые поля, так как не используем
		unsigned int unZero = 0;
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (unsigned int)); // offBmi
		m_pOutStream->WriteFile((BYTE*)&unZero,                sizeof (unsigned int)); // cbBmi
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (unsigned int)); // offBits
		m_pOutStream->WriteFile((BYTE*)&unZero,                sizeof (unsigned int)); // cbBits

		m_pOutStream->WriteFile((BYTE*)&pPen->unPenStyle,    sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&pPen->unWidth,       sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&arUnused[0],       sizeof (unsigned int)); // BrushStyle

		WriteColor(pPen->oColor);

		m_pOutStream->WriteFile((BYTE*)&arUnused[1],       sizeof (unsigned int)); // BrushHatch

		m_pOutStream->WriteFile((BYTE*)&pPen->unNumStyleEntries, sizeof (unsigned int));

		for (unsigned int i = 0; i < pPen->unNumStyleEntries; ++i)
			m_pOutStream->WriteFile((BYTE*)&pPen->pStyleEntry[i], sizeof (unsigned int));

		if (pPen->unPenStyle != PS_USERSTYLE)
			m_pOutStream->WriteFile((BYTE*)&unZero,            sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
	{
		if (NULL == pPen)
			return;

		const unsigned int unZero = 0;

		int unExplicitRecordSize = 28;
		int unType               = EMR_CREATEPEN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPenIndex,            sizeof (unsigned int));

                m_pOutStream->WriteFile((BYTE*)&pPen->unPenStyle,        sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unWidthX,              sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unZero,                sizeof (unsigned int));

		WriteColor(pPen->oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETPOLYFILLMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unFillMode,            sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_BEGINPATH()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_BEGINPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_ENDPATH()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_ENDPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_CLOSEFIGURE()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_CLOSEFIGURE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_FLATTENPATH()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_FLATTENPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_WIDENPATH()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_WIDENPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_ABORTPATH()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_ABORTPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_MOVETOEX(const TPointL &oPoint)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_MOVETOEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETARCDIRECTION;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unDirection,           sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_FILLPATH(const TRectL &oBounds)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_FILLPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);
	}

	void CEmfInterpretator::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETMAPMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unMapMode,             sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETWINDOWORGEX(const TPointL &oOrigin)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_SETWINDOWORGEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oOrigin);
	}

	void CEmfInterpretator::HANDLE_EMR_SETWINDOWEXTEX(const TSizeL &oExtent)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_SETWINDOWEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteSize(oExtent);
	}

	void CEmfInterpretator::HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_SCALEWINDOWEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&nXNum,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nXDenom,               sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nYNum,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nYDenom,               sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETVIEWPORTORGEX(const TPointL &oOrigin)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_SETVIEWPORTORGEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oOrigin);
	}

	void CEmfInterpretator::HANDLE_EMR_SETVIEWPORTEXTEX(const TSizeL &oExtent)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_SETVIEWPORTEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteSize(oExtent);
	}

	void CEmfInterpretator::HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_SCALEVIEWPORTEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&nXNum,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nXDenom,               sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nYNum,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nYDenom,               sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETSTRETCHBLTMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unStretchMode,         sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETICMMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unICMMode,             sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 32 + oDibBrush.unCbBmi + oDibBrush.unCbBits;
		int unType               = EMR_CREATEDIBPATTERNBRUSHPT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBrushIndex,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unUsage,     sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unOffBmi,    sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unCbBmi,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unOffBits,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unCbBits,    sizeof (unsigned int));

		if (oDibBrush.unCbBmi > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oDibBrush.unCbBmi);

		if (oDibBrush.unCbBits > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oDibBrush.unCbBmi, sizeof (BYTE) * oDibBrush.unCbBits);
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEMONOBRUSH(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 32 + oDibBrush.unCbBmi + oDibBrush.unCbBits;
		int unType               = EMR_CREATEMONOBRUSH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBrushIndex,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unUsage,     sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unOffBmi,    sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unCbBmi,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unOffBits,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.unCbBits,    sizeof (unsigned int));

		if (oDibBrush.unCbBmi > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oDibBrush.unCbBmi);

		if (oDibBrush.unCbBits > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oDibBrush.unCbBmi, sizeof (BYTE) * oDibBrush.unCbBits);
	}

	void CEmfInterpretator::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SELECTCLIPPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unRegionMode,          sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETBKCOLOR(const TRGBA &oColor)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETBKCOLOR;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteColor(oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_EXCLUDECLIPRECT(const TRectL &oClip)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_EXCLUDECLIPRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oClip);
	}

	void CEmfInterpretator::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		int unExplicitRecordSize = 16 + unRgnDataSize;
		int unType               = EMR_EXTSELECTCLIPRGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unRgnDataSize,         sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unRegionMode,          sizeof (unsigned int));

		if (unRgnDataSize > 0)
		{
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * unRgnDataSize);
			//                        oDataStream.Skip(unRgnDataSize);
		}
	}

	void CEmfInterpretator::HANDLE_EMR_SETMETARGN()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_SETMETARGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETROP2;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unRop2Mode,            sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette)
	{
		if (NULL == oEmfLogPalette)
			return;

		int unExplicitRecordSize = 16 + sizeof (TEmfLogPaletteEntry) * oEmfLogPalette->ushNumberOfEntries;
		int unType               = EMR_CREATEPALETTE;
		short unVersion          = 0x0300;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPaletteIndex,        sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&unVersion,                          sizeof (short));
		m_pOutStream->WriteFile((BYTE*)&oEmfLogPalette->ushNumberOfEntries, sizeof (unsigned int));

		for (unsigned int i = 0; i < oEmfLogPalette->ushNumberOfEntries; ++i)
			WriteRColor(oEmfLogPalette->pPaletteEntries[i]);
	}

	void CEmfInterpretator::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SELECTPALETTE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPaletteIndex,        sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_REALIZEPALETTE()
	{
		int unExplicitRecordSize = 8;
		int unType               = EMR_REALIZEPALETTE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_INTERSECTCLIPRECT(const TRectL &oClip)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_INTERSECTCLIPRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oClip);
	}

	void CEmfInterpretator::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
	{
		int unExplicitRecordSize = 12;
		int unType               = EMR_SETLAYOUT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unLayoutMode,          sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETBRUSHORGEX(const TPointL &oOrigin)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_SETBRUSHORGEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oOrigin);
	}

	void CEmfInterpretator::HANDLE_EMR_ANGLEARC(const TPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		int unExplicitRecordSize = 28;
		int unType               = EMR_ANGLEARC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oCenter);

		m_pOutStream->WriteFile((BYTE*)&unRadius,          sizeof (unsigned int));

		WriteFloat(dStartAngle);
		WriteFloat(dSweepAngle);
	}

	void CEmfInterpretator::HANDLE_EMR_ARC(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		int unExplicitRecordSize = 40;
		int unType               = EMR_ARC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_ARCTO(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		int unExplicitRecordSize = 40;
		int unType               = EMR_ARCTO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_CHORD(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		int unExplicitRecordSize = 40;
		int unType               = EMR_CHORD;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_ELLIPSE(const TRectL &oBox)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_ELLIPSE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
	}

	void CEmfInterpretator::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		const unsigned int unDxCount = oTEmfExtTextoutA.oEmrText.unOptions & ETO_PDY ? 2 * oTEmfExtTextoutA.oEmrText.unChars : oTEmfExtTextoutA.oEmrText.unChars;

		int unExplicitRecordSize = 76 + oTEmfExtTextoutA.oEmrText.unChars * 1 + unDxCount * 4;
		int unType               = EMR_EXTTEXTOUTA;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfExtTextoutA.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.unIGraphicsMode,    sizeof (unsigned int));

		WriteFloat(oTEmfExtTextoutA.dExScale);
		WriteFloat(oTEmfExtTextoutA.dExScale);

		WritePoint(oTEmfExtTextoutA.oEmrText.oReference);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.oEmrText.unChars,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.oEmrText.unOffString,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.oEmrText.unOptions,     sizeof (unsigned int));

		WriteRectangle(oTEmfExtTextoutA.oEmrText.oRectangle);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.oEmrText.unOffDx,       sizeof (unsigned int));

		if (oTEmfExtTextoutA.oEmrText.unChars == 0)
			return;

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.oEmrText.pOutputString,  sizeof (unsigned char) * oTEmfExtTextoutA.oEmrText.unChars);

		if (unDxCount > 0)
			m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.oEmrText.pOutputDx, sizeof (unsigned int) * unDxCount);
	}

	void CEmfInterpretator::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		const unsigned int unDxCount = oTEmfExtTextoutW.oEmrText.unOptions & ETO_PDY ? 2 * oTEmfExtTextoutW.oEmrText.unChars : oTEmfExtTextoutW.oEmrText.unChars;
		const unsigned int unAlign   = oTEmfExtTextoutW.oEmrText.unOffDx - 76 - 2 * oTEmfExtTextoutW.oEmrText.unChars;
		const unsigned int unZero    = 0;

		int unExplicitRecordSize     = 76 + oTEmfExtTextoutW.oEmrText.unChars * 2 + unDxCount * 4 + unAlign;
		int unType                   = EMR_EXTTEXTOUTW;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfExtTextoutW.oBounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.unIGraphicsMode,      sizeof (unsigned int));

		WriteFloat(oTEmfExtTextoutW.dExScale);
		WriteFloat(oTEmfExtTextoutW.dEyScale);

		WritePoint(oTEmfExtTextoutW.oEmrText.oReference);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.oEmrText.unChars,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.oEmrText.unOffString, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.oEmrText.unOptions,   sizeof (unsigned int));

		WriteRectangle(oTEmfExtTextoutW.oEmrText.oRectangle);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.oEmrText.unOffDx,     sizeof (unsigned int));

		if (oTEmfExtTextoutW.oEmrText.unChars == 0)
			return;

		m_pOutStream->WriteFile((BYTE*)oTEmfExtTextoutW.oEmrText.pOutputString, sizeof (unsigned short) * oTEmfExtTextoutW.oEmrText.unChars);

		for (unsigned int i = 0; i < unAlign; ++i)
			m_pOutStream->WriteFile((BYTE*)&unZero, sizeof (BYTE));

		if (unDxCount > 0)
			m_pOutStream->WriteFile((BYTE*)oTEmfExtTextoutW.oEmrText.pOutputDx, sizeof (unsigned int) * unDxCount);
	}

	void CEmfInterpretator::HANDLE_EMR_LINETO(const TPointL &oPoint)
	{
		int unExplicitRecordSize = 16;
		int unType               = EMR_LINETO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_PIE(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		int unExplicitRecordSize = 40;
		int unType               = EMR_PIE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIER(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 8 * unCountPoints;
		int unType               = EMR_POLYBEZIER;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIER(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 4 * unCountPoints;
		int unType               = EMR_POLYBEZIER16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIERTO(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 8 * unCountPoints;
		int unType               = EMR_POLYBEZIERTO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIERTO(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 4 * unCountPoints;
		int unType               = EMR_POLYBEZIERTO16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYDRAW(const TRectL &oBounds, TPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (NULL == arPoints || NULL == pAbTypes)
			return;

		int unExplicitRecordSize = 28 + 8 * unCount;
		int unType               = EMR_POLYDRAW;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCount,  sizeof (unsigned int));

		for (unsigned int i = 0; i < unCount; ++i)
			WritePoint(arPoints[i]);

		for (unsigned int i = 0; i < unCount; ++i)
			m_pOutStream->WriteFile((BYTE*)&pAbTypes[i],  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_POLYDRAW(const TRectL &oBounds, TPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (NULL == arPoints || NULL == pAbTypes)
			return;

		int unExplicitRecordSize = 28 + 4 * unCount;
		int unType               = EMR_POLYDRAW16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCount,  sizeof (unsigned int));

		for (unsigned int i = 0; i < unCount; ++i)
			WritePoint(arPoints[i]);

		for (unsigned int i = 0; i < unCount; ++i)
			m_pOutStream->WriteFile((BYTE*)&pAbTypes[i],  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_POLYGON(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 8 * unCountPoints;
		int unType               = EMR_POLYGON;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYGON(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 4 * unCountPoints;
		int unType               = EMR_POLYGON16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINE(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 8 * unCountPoints;
		int unType               = EMR_POLYLINE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINE(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 4 * unCountPoints;
		int unType               = EMR_POLYLINE16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINETO(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 8 * unCountPoints;
		int unType               = EMR_POLYLINETO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINETO(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize = 28 + 4 * unCountPoints;
		int unType               = EMR_POLYLINETO16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYGON(const TRectL &oBounds, const std::vector<std::vector<TPointL>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TPointL>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize = 32 + 4 * unNumberOfPolygons + 8 * unCountPoints;
		int unType               = EMR_POLYPOLYGON;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TPointL>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TPointL>& arTempPoints : arPoints)
			for (const TPointL& oPoint : arTempPoints)
				WritePoint(oPoint);

	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYGON(const TRectL &oBounds, const std::vector<std::vector<TPointS>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TPointS>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize = 32 + 4 * unNumberOfPolygons + 4 * unCountPoints;
		int unType               = EMR_POLYPOLYGON16;


		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TPointS>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TPointS>& arTempPoints : arPoints)
			for (const TPointS& oPoint : arTempPoints)
				WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYLINE(const TRectL &oBounds, const std::vector<std::vector<TPointL>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TPointL>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize = 32 + 4 * unNumberOfPolygons + 8 * unCountPoints;
		int unType               = EMR_POLYPOLYLINE;


		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TPointL>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TPointL>& arTempPoints : arPoints)
			for (const TPointL& oPoint : arTempPoints)
				WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYLINE(const TRectL &oBounds, const std::vector<std::vector<TPointS>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TPointS>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize = 32 + 4 * unNumberOfPolygons + 4 * unCountPoints;
		int unType               = EMR_POLYPOLYLINE16;


		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TPointS>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TPointS>& arTempPoints : arPoints)
			for (const TPointS& oPoint : arTempPoints)
				WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_RECTANGLE(const TRectL &oBox)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_RECTANGLE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
	}

	void CEmfInterpretator::HANDLE_EMR_ROUNDRECT(const TRectL &oBox, const TSizeL &oCorner)
	{
		int unExplicitRecordSize = 32;
		int unType               = EMR_ROUNDRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WriteSize(oCorner);
	}

	void CEmfInterpretator::HANDLE_EMR_SETPIXELV(const TPointL &oPoint, const TRGBA &oColor)
	{
		int unExplicitRecordSize = 20;
		int unType               = EMR_SETPIXELV;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oPoint);
		WriteColor(oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{
		int unExplicitRecordSize = 36 + ((oText.unFuOptions & ETO_NO_RECT) ? 0 : 16) +
		                           oText.unCChars * ((oText.unFuOptions & ETO_SMALL_CHARS) ? sizeof (unsigned char) : sizeof (unsigned short));
		int unType               = EMR_SMALLTEXTOUT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&oText.nX,              sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oText.nY,              sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oText.unCChars,        sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oText.unFuOptions,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oText.unIGraphicsMode, sizeof (unsigned int));

		WriteFloat(oText.dExScale);
		WriteFloat(oText.dEyScale);

		if (!(oText.unFuOptions & ETO_NO_RECT))
			WriteRectangle(oText.oBounds);

		if (oText.unCChars > 0)
		{
			if (oText.unFuOptions & ETO_SMALL_CHARS)
				m_pOutStream->WriteFile((BYTE*)&oText.pTextString ,sizeof (unsigned char) * oText.unCChars);
			else
				m_pOutStream->WriteFile((BYTE*)&oText.pTextString ,sizeof (unsigned short) * oText.unCChars);
		}
	}

	void CEmfInterpretator::HANDLE_EMR_STROKEANDFILLPATH(const TRectL &oBounds)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_STROKEANDFILLPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);
	}

	void CEmfInterpretator::HANDLE_EMR_STROKEPATH(const TRectL &oBounds)
	{
		int unExplicitRecordSize = 24;
		int unType               = EMR_STROKEPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);
	}

	void CEmfInterpretator::HANDLE_EMR_GRADIENTFILL(const std::vector<TTriVertex> &arVertex, const std::vector<std::pair<int, int> > &arIndexes, unsigned int unFillMode)
	{
	}

	void CEmfInterpretator::HANDLE_EMR_UNKNOWN(CDataStream &oDataStream)
	{
		unsigned int unRecordType, unRecordSize;

		oDataStream >> unRecordType;
		oDataStream >> unRecordSize;

		if (unRecordSize == 0) return;

		unFileSize += unRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unRecordType, sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unRecordSize, sizeof (int));

		if (unRecordSize > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * unRecordSize - 8);
	}

	void CEmfInterpretator::HANDLE_EMR_FILLRGN(const TRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		int unRgnDataSize           = 32 + arRects.size() * 16;
		int unExplicitRecordSize = 32 + unRgnDataSize;
		int unType               = EMR_FILLRGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unRgnDataSize,          sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unIhBrush,              sizeof (int));

		WriteRegionDataHeader(oRegionDataHeader);

		for (const TRectL &oRect : arRects)
			WriteRectangle(oRect);
	}

	void CEmfInterpretator::HANDLE_EMR_PAINTRGN(const TRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		int unRgnDataSize           = 32 + arRects.size() * 16;
		int unExplicitRecordSize = 28 + unRgnDataSize;
		int unType               = EMR_PAINTRGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unRgnDataSize,          sizeof (int));

		WriteRegionDataHeader(oRegionDataHeader);

		for (const TRectL &oRect : arRects)
			WriteRectangle(oRect);
	}

	void CEmfInterpretator::HANDLE_EMR_FRAMERGN(const TRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		int unRgnDataSize           = 40 + arRects.size() * 16;
		int unExplicitRecordSize = 28 + unRgnDataSize;
		int unType               = EMR_FRAMERGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unRgnDataSize, sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unIhBrush,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nWidth,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nHeight,      sizeof (int));

		WriteRegionDataHeader(oRegionDataHeader);

		for (const TRectL &oRect : arRects)
			WriteRectangle(oRect);
	}

	void CEmfInterpretator::WriteRegionDataHeader(const TRegionDataHeader &oRegionDataHeader)
	{
		m_pOutStream->WriteFile((BYTE*)&oRegionDataHeader.unSize,       sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oRegionDataHeader.unType,       sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oRegionDataHeader.unCountRects, sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oRegionDataHeader.unRgnSize,    sizeof(int));

		WriteRectangle(oRegionDataHeader.oBounds);
	}

	void CEmfInterpretator::WriteRectangle(const TRectL &oBounds)
	{
		m_pOutStream->WriteFile((BYTE*)&oBounds.Left,   sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oBounds.Top,    sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oBounds.Right,  sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oBounds.Bottom, sizeof(int));
	}

	void CEmfInterpretator::WriteRectangle(const TEmfPlusRect &oRect)
	{
		m_pOutStream->WriteFile((BYTE*)&oRect.shX,       sizeof(short));
		m_pOutStream->WriteFile((BYTE*)&oRect.shY,       sizeof(short));
		m_pOutStream->WriteFile((BYTE*)&oRect.shWidth,   sizeof(short));
		m_pOutStream->WriteFile((BYTE*)&oRect.shHeight,  sizeof(short));
	}

	void CEmfInterpretator::WriteRectangle(const TEmfPlusRectF &oRect)
	{
		m_pOutStream->WriteFile((BYTE*)&oRect.dX,       sizeof(float));
		m_pOutStream->WriteFile((BYTE*)&oRect.dY,       sizeof(float));
		m_pOutStream->WriteFile((BYTE*)&oRect.dWidth,   sizeof(float));
		m_pOutStream->WriteFile((BYTE*)&oRect.dHeight,  sizeof(float));
	}

	void CEmfInterpretator::WriteSize(const TSizeL &oSize)
	{
		m_pOutStream->WriteFile((BYTE*)&oSize.X,   sizeof(unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oSize.Y,   sizeof(unsigned int));
	}

	void CEmfInterpretator::WriteColor(const TRGBA &oColor)
	{
		m_pOutStream->WriteFile((BYTE*)&oColor.r, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oColor.g, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oColor.b, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oColor.a, sizeof (unsigned char));
	}

	void CEmfInterpretator::WriteRColor(const TEmfLogPaletteEntry &oRColor)
	{
		m_pOutStream->WriteFile((BYTE*)&oRColor.uchReserved,  sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oRColor.uchBlue,      sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oRColor.uchGreen,     sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oRColor.uchRed,       sizeof (unsigned char));
	}

	void CEmfInterpretator::WriteARGB(const TEmfPlusARGB &oARGB)
	{
		int nARGB = (oARGB.chBlue | (oARGB.chGreen << 8) | (oARGB.chRed << 16) | (oARGB.chAlpha << 24));
		m_pOutStream->WriteFile((BYTE*)&nARGB,  sizeof (int));
	}

	void CEmfInterpretator::WriteForm(const TXForm &oForm)
	{
		WriteFloat(oForm.M11);
		WriteFloat(oForm.M12);
		WriteFloat(oForm.M21);
		WriteFloat(oForm.M22);
		WriteFloat(oForm.Dx);
		WriteFloat(oForm.Dy);
	}

	void CEmfInterpretator::WritePoint(const TPointL &oPoint)
	{
		m_pOutStream->WriteFile((BYTE*)&oPoint.X, sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oPoint.Y, sizeof (int));
	}

	void CEmfInterpretator::WritePoint(const TPointS &oPoint)
	{
		m_pOutStream->WriteFile((BYTE*)&oPoint.X, sizeof (short));
		m_pOutStream->WriteFile((BYTE*)&oPoint.Y, sizeof (short));
	}

	void CEmfInterpretator::WritePoint(const TEmfPlusPointF &oPoint)
	{
		m_pOutStream->WriteFile((BYTE*)&oPoint.X, sizeof (float));
		m_pOutStream->WriteFile((BYTE*)&oPoint.Y, sizeof (float));
	}

	void CEmfInterpretator::WriteString(const unsigned short arshString[], const unsigned int &unSize)
	{
		unsigned int unZero = 0;
		if (NULL == arshString)
		{
			m_pOutStream->WriteFile((BYTE*)&unZero, sizeof (unsigned int) * 4);
			return;
		}

		unsigned short ushNULL = 52685; //0xCDCD

		for (unsigned int i = 0; i < unSize; ++i)
		{
			if (arshString[i] != ushNULL)
				m_pOutStream->WriteFile((BYTE*)&arshString[i], sizeof (unsigned short));
			else
				m_pOutStream->WriteFile((BYTE*)&unZero, sizeof (unsigned short));
		}
	}

	void CEmfInterpretator::WriteFloat(const float &fValue)
	{
		unsigned char arValue[4];

		arValue[0] = *((unsigned char*)(&fValue) + 0);
		arValue[1] = *((unsigned char*)(&fValue) + 1);
		arValue[2] = *((unsigned char*)(&fValue) + 2);
		arValue[3] = *((unsigned char*)(&fValue) + 3);

		m_pOutStream->WriteFile((BYTE*)arValue, sizeof (arValue));
	}
}


