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
		int unExplicitRecordSize    = 88;
		int unType                  = EMR_HEADER;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfHeader.oBounds);
		WriteRectangle(oTEmfHeader.oFrame);

		unsigned int unZero = 0;

		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulSignature,        sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulVersion,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulSize,             sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ulRecords,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ushObjects,         sizeof (unsigned short));
		m_pOutStream->WriteFile((BYTE*)&oTEmfHeader.ushReserved,        sizeof (unsigned short));
		m_pOutStream->WriteFile((BYTE*)&unZero,                         sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unZero,                         sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unZero,                         sizeof (unsigned int));

		WriteSize(oTEmfHeader.oDevice);
		WriteSize(oTEmfHeader.oMillimeters);
	}

	void CEmfInterpretator::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 108 + oTEmfAlphaBlend.cbBitsSrc + oTEmfAlphaBlend.cbBmiSrc;
		int unType                  = EMR_ALPHABLEND;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfAlphaBlend.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.xDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.yDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.cxDest,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.cyDest,    sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.BlendOperation,    sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.BlendFlags,        sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.SrcConstantAlpha,  sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.AlphaFormat,       sizeof (unsigned char));

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.xSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.ySrc,      sizeof (int));

		WriteForm(oTEmfAlphaBlend.XformSrc);
		WriteColor(oTEmfAlphaBlend.BkColor);

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.UsageSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.offBmiSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.cbBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.offBitsSrc,sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.cbBitsSrc, sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.cxSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfAlphaBlend.cySrc,      sizeof (int));

		if (oTEmfAlphaBlend.cbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfAlphaBlend.cbBmiSrc);

		if (oTEmfAlphaBlend.cbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfAlphaBlend.cbBmiSrc, sizeof (BYTE) * oTEmfAlphaBlend.cbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 80 + oTEmfStretchDIBITS.cbBitsSrc + oTEmfStretchDIBITS.cbBmiSrc;
		int unType                  = EMR_STRETCHDIBITS;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfStretchDIBITS.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.xDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.yDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.xSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.ySrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.cxSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.cySrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.offBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.cbBmiSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.offBitsSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.cbBitsSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.UsageSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.\
								BitBltRasterOperation,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.cxDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchDIBITS.cyDest,     sizeof (int));

		if (oTEmfStretchDIBITS.cbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfStretchDIBITS.cbBmiSrc);

		if (oTEmfStretchDIBITS.cbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfStretchDIBITS.cbBmiSrc, sizeof (BYTE) * oTEmfStretchDIBITS.cbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 100 + oTEmfBitBlt.cbBitsSrc + oTEmfBitBlt.cbBmiSrc;
		int unType                  = EMR_BITBLT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfBitBlt.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.xDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.yDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.cxDest,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.cyDest,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.\
								BitBltRasterOperation,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.xSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.ySrc,      sizeof (int));

		WriteForm(oTEmfBitBlt.XfromSrc);
		WriteColor(oTEmfBitBlt.BkColorSrc);

		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.UsageSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.offBmiSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.cbBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.offBitsSrc,sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfBitBlt.cbBitsSrc, sizeof (unsigned int));

		if (oTEmfBitBlt.cbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfBitBlt.cbBmiSrc);

		if (oTEmfBitBlt.cbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfBitBlt.cbBmiSrc, sizeof (BYTE) * oTEmfBitBlt.cbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 76 + oTEmfSetDiBitsToDevice.cbBitsSrc + oTEmfSetDiBitsToDevice.cbBmiSrc;
		int unType                  = EMR_SETDIBITSTODEVICE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfSetDiBitsToDevice.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.xDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.yDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.xSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.ySrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.cxSrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.cySrc,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.offBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.cbBmiSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.offBitsSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.cbBitsSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.UsageSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.iStartScan, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfSetDiBitsToDevice.cScans,     sizeof (unsigned int));

		if (oTEmfSetDiBitsToDevice.cbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfSetDiBitsToDevice.cbBmiSrc);

		if (oTEmfSetDiBitsToDevice.cbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfSetDiBitsToDevice.cbBmiSrc, sizeof (BYTE) * oTEmfSetDiBitsToDevice.cbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 84 + oTEmfStretchBLT.cbBitsSrc + oTEmfStretchBLT.cbBmiSrc;
		int unType                  = EMR_STRETCHBLT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfStretchBLT.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.xDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.yDest,      sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.cxDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.cyDest,     sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.\
								BitBltRasterOperation,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.xSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.ySrc,       sizeof (int));

		WriteForm(oTEmfStretchBLT.XformSrc);
		WriteColor(oTEmfStretchBLT.BkColorSrc);

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.UsageSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.offBmiSrc,  sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.cbBmiSrc,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.offBitsSrc, sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.cbBitsSrc,  sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.cxSrc,       sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfStretchBLT.cySrc,       sizeof (int));

		if (oTEmfStretchBLT.cbBmiSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfStretchBLT.cbBmiSrc);

		if (oTEmfStretchBLT.cbBitsSrc > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oTEmfStretchBLT.cbBmiSrc, sizeof (BYTE) * oTEmfStretchBLT.cbBitsSrc);
	}

	void CEmfInterpretator::HANDLE_EMR_EOF()
	{
		int unExplicitRecordSize    = 20;
		int unType                  = EMR_EOF;

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
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_SAVEDC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_RESTOREDC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&nIndexDC,              sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
	{
		int unExplicitRecordSize    = 36;
		int unType                  = EMR_MODIFYWORLDTRANSFORM;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteForm(oXForm);

		m_pOutStream->WriteFile((BYTE*)&unMode,                sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
	{
		int unExplicitRecordSize    = 32;
		int unType                  = EMR_SETWORLDTRANSFORM;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteForm(oXForm);
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_CREATEBRUSHINDIRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBrushIndex,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&(pBrush->BrushStyle),  sizeof (unsigned int));

		WriteColor(pBrush->Color);

		m_pOutStream->WriteFile((BYTE*)&(pBrush->BrushHatch),  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETTEXTCOLOR;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteColor(oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SELECTOBJECT;

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

		int unExplicitRecordSize = (oLogFont->IsFixedLength()) ? 332
															   : (368 + oLogFont->DesignVector.NumAxes * sizeof(int));

		int unType                  = EMR_EXTCREATEFONTINDIRECTW;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unIndex,               sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Height,        sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Width,         sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Escapement,    sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Orientation,   sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Weight,        sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Italic,        sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Underline,     sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.StrikeOut,     sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.CharSet,       sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.OutPrecision,  sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.ClipPrecision, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.Quality,       sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oLogFont->LogFontEx.LogFont.PitchAndFamily,sizeof (unsigned char));

		WriteString(oLogFont->LogFontEx.LogFont.FaceName,   32);
		WriteString(oLogFont->LogFontEx.FullName,           64);
		WriteString(oLogFont->LogFontEx.Style,              32);
		WriteString(oLogFont->LogFontEx.Script,             32);

		if (!oLogFont->IsFixedLength())
		{
			m_pOutStream->WriteFile((BYTE*)&oLogFont->DesignVector.Signature,     sizeof (unsigned int));
			m_pOutStream->WriteFile((BYTE*)&oLogFont->DesignVector.NumAxes,       sizeof (unsigned int));

			for (unsigned int i = 0; i < oLogFont->DesignVector.NumAxes; ++i)
				m_pOutStream->WriteFile((BYTE*)&oLogFont->DesignVector.Values[i], sizeof (int));
		}
	}

	void CEmfInterpretator::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETTEXTALIGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unAlign,               sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETBKMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBgMode,              sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_DELETEOBJECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unObjectIndex,  sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETMITERLIMIT;

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

		int unExplicitRecordSize    = 52 + sizeof (unsigned int) * pPen->NumStyleEntries + ((pPen->PenStyle != PS_USERSTYLE) ? 4 : 0);
		int unType                  = EMR_EXTCREATEPEN;

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

		m_pOutStream->WriteFile((BYTE*)&pPen->PenStyle,    sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&pPen->Width,       sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&arUnused[0],       sizeof (unsigned int)); // BrushStyle

		WriteColor(pPen->Color);

		m_pOutStream->WriteFile((BYTE*)&arUnused[1],       sizeof (unsigned int)); // BrushHatch

		m_pOutStream->WriteFile((BYTE*)&pPen->NumStyleEntries, sizeof (unsigned int));

		for (unsigned int i = 0; i < pPen->NumStyleEntries; ++i)
			m_pOutStream->WriteFile((BYTE*)&pPen->StyleEntry[i], sizeof (unsigned int));

		if (pPen->PenStyle != PS_USERSTYLE)
			m_pOutStream->WriteFile((BYTE*)&unZero,            sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
	{
		if (NULL == pPen)
			return;

		const unsigned int unZero = 0;

		int unExplicitRecordSize    = 28;
		int unType                  = EMR_CREATEPEN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPenIndex,            sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&pPen->PenStyle,        sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unWidthX,              sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unZero,                sizeof (unsigned int));

		WriteColor(pPen->Color);
	}

	void CEmfInterpretator::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETPOLYFILLMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unFillMode,            sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_BEGINPATH()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_BEGINPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_ENDPATH()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_ENDPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_CLOSEFIGURE()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_CLOSEFIGURE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_FLATTENPATH()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_FLATTENPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_WIDENPATH()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_WIDENPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_ABORTPATH()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_ABORTPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_MOVETOEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETARCDIRECTION;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unDirection,           sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_FILLPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);
	}

	void CEmfInterpretator::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETMAPMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unMapMode,             sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_SETWINDOWORGEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oOrigin);
	}

	void CEmfInterpretator::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_SETWINDOWEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteSize(oExtent);
	}

	void CEmfInterpretator::HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_SCALEWINDOWEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&nXNum,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nXDenom,               sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nYNum,                 sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&nYDenom,               sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_SETVIEWPORTORGEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oOrigin);
	}

	void CEmfInterpretator::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_SETVIEWPORTEXTEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteSize(oExtent);
	}

	void CEmfInterpretator::HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_SCALEVIEWPORTEXTEX;

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
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETSTRETCHBLTMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unStretchMode,         sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETICMMODE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unICMMode,             sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 32 + oDibBrush.cbBmi + oDibBrush.cbBits;
		int unType                  = EMR_CREATEDIBPATTERNBRUSHPT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBrushIndex,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.Usage,       sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oDibBrush.offBmi,      sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.cbBmi,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.offBits,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.cbBits,      sizeof (unsigned int));

		if (oDibBrush.cbBmi > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oDibBrush.cbBmi);

		if (oDibBrush.cbBits > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oDibBrush.cbBmi, sizeof (BYTE) * oDibBrush.cbBits);
	}

	void CEmfInterpretator::HANDLE_EMR_CREATEMONOBRUSH(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 32 + oDibBrush.cbBmi + oDibBrush.cbBits;
		int unType                  = EMR_CREATEMONOBRUSH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unBrushIndex,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.Usage,       sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&oDibBrush.offBmi,      sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.cbBmi,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.offBits,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oDibBrush.cbBits,      sizeof (unsigned int));

		if (oDibBrush.cbBmi > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr(), sizeof (BYTE) * oDibBrush.cbBmi);

		if (oDibBrush.cbBits > 0)
			m_pOutStream->WriteFile(oDataStream.GetCurPtr() + oDibBrush.cbBmi, sizeof (BYTE) * oDibBrush.cbBits);
	}

	void CEmfInterpretator::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SELECTCLIPPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unRegionMode,          sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETBKCOLOR;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteColor(oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_EXCLUDECLIPRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oClip);
	}

	void CEmfInterpretator::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		int unExplicitRecordSize    = 16 + unRgnDataSize;
		int unType                  = EMR_EXTSELECTCLIPRGN;

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
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_SETMETARGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETROP2;

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

		int unExplicitRecordSize    = 16 + sizeof (TEmfLogPaletteEntry) * oEmfLogPalette->NumberOfEntries;
		int unType                  = EMR_CREATEPALETTE;
		short unVersion             = 0x0300;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;
		++ushNuberDescriptors;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPaletteIndex,        sizeof (unsigned int));

		m_pOutStream->WriteFile((BYTE*)&unVersion,                         sizeof (short));
		m_pOutStream->WriteFile((BYTE*)&oEmfLogPalette->NumberOfEntries,   sizeof (unsigned int));

		for (unsigned int i = 0; i < oEmfLogPalette->NumberOfEntries; ++i)
			WriteRColor(oEmfLogPalette->PaletteEntries[i]);
	}

	void CEmfInterpretator::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SELECTPALETTE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unPaletteIndex,        sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_REALIZEPALETTE()
	{
		int unExplicitRecordSize    = 8;
		int unType                  = EMR_REALIZEPALETTE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));
	}

	void CEmfInterpretator::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_INTERSECTCLIPRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oClip);
	}

	void CEmfInterpretator::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
	{
		int unExplicitRecordSize    = 12;
		int unType                  = EMR_SETLAYOUT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&unLayoutMode,          sizeof (unsigned int));
	}

	void CEmfInterpretator::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_SETBRUSHORGEX;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oOrigin);
	}

	void CEmfInterpretator::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		int unExplicitRecordSize    = 28;
		int unType                  = EMR_ANGLEARC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oCenter);

		m_pOutStream->WriteFile((BYTE*)&unRadius,          sizeof (unsigned int));

		WriteFloat(dStartAngle);
		WriteFloat(dSweepAngle);
	}

	void CEmfInterpretator::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		int unExplicitRecordSize    = 40;
		int unType                  = EMR_ARC;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		int unExplicitRecordSize    = 40;
		int unType                  = EMR_ARCTO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		int unExplicitRecordSize    = 40;
		int unType                  = EMR_CHORD;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_ELLIPSE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
	}

	void CEmfInterpretator::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		const unsigned int unDxCount = oTEmfExtTextoutA.aEmrText.Options & ETO_PDY ? 2 * oTEmfExtTextoutA.aEmrText.Chars : oTEmfExtTextoutA.aEmrText.Chars;

		int unExplicitRecordSize    = 76 + oTEmfExtTextoutA.aEmrText.Chars * 1 + unDxCount * 4;
		int unType                  = EMR_EXTTEXTOUTA;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfExtTextoutA.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.iGraphicsMode,    sizeof (unsigned int));

		WriteFloat(oTEmfExtTextoutA.exScale);
		WriteFloat(oTEmfExtTextoutA.exScale);

		WritePoint(oTEmfExtTextoutA.aEmrText.Reference);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.aEmrText.Chars,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.aEmrText.offString,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.aEmrText.Options,     sizeof (unsigned int));

		WriteRectangle(oTEmfExtTextoutA.aEmrText.Rectangle);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.aEmrText.offDx,       sizeof (unsigned int));

		if (oTEmfExtTextoutA.aEmrText.Chars == 0)
			return;

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.aEmrText.OutputString,sizeof (unsigned char) * oTEmfExtTextoutA.aEmrText.Chars);

		if (unDxCount > 0)
			m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutA.aEmrText.OutputDx,sizeof (unsigned int) * unDxCount);
	}

	void CEmfInterpretator::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		const unsigned int unDxCount = oTEmfExtTextoutW.wEmrText.Options & ETO_PDY ? 2 * oTEmfExtTextoutW.wEmrText.Chars : oTEmfExtTextoutW.wEmrText.Chars;
		const unsigned int unAlign   = oTEmfExtTextoutW.wEmrText.offDx - 76 - 2 * oTEmfExtTextoutW.wEmrText.Chars;
		const unsigned int unZero    = 0;

		int unExplicitRecordSize     = 76 + oTEmfExtTextoutW.wEmrText.Chars * 2 + unDxCount * 4 + unAlign;
		int unType                   = EMR_EXTTEXTOUTW;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oTEmfExtTextoutW.Bounds);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.iGraphicsMode,    sizeof (unsigned int));

		WriteFloat(oTEmfExtTextoutW.exScale);
		WriteFloat(oTEmfExtTextoutW.eyScale);

		WritePoint(oTEmfExtTextoutW.wEmrText.Reference);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.wEmrText.Chars,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.wEmrText.offString,   sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.wEmrText.Options,     sizeof (unsigned int));

		WriteRectangle(oTEmfExtTextoutW.wEmrText.Rectangle);

		m_pOutStream->WriteFile((BYTE*)&oTEmfExtTextoutW.wEmrText.offDx,       sizeof (unsigned int));

		if (oTEmfExtTextoutW.wEmrText.Chars == 0)
			return;

		m_pOutStream->WriteFile((BYTE*)oTEmfExtTextoutW.wEmrText.OutputString, sizeof (unsigned short) * oTEmfExtTextoutW.wEmrText.Chars);

		for (unsigned int i = 0; i < unAlign; ++i)
			m_pOutStream->WriteFile((BYTE*)&unZero, sizeof (BYTE));

		if (unDxCount > 0)
			m_pOutStream->WriteFile((BYTE*)oTEmfExtTextoutW.wEmrText.OutputDx, sizeof (unsigned int) * unDxCount);
	}

	void CEmfInterpretator::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
	{
		int unExplicitRecordSize    = 16;
		int unType                  = EMR_LINETO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		int unExplicitRecordSize    = 40;
		int unType                  = EMR_PIE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WritePoint(oStart);
		WritePoint(oEnd);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 8 * unCountPoints;
		int unType                  = EMR_POLYBEZIER;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 4 * unCountPoints;
		int unType                  = EMR_POLYBEZIER16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 8 * unCountPoints;
		int unType                  = EMR_POLYBEZIERTO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 4 * unCountPoints;
		int unType                  = EMR_POLYBEZIERTO16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (NULL == arPoints || NULL == pAbTypes)
			return;

		int unExplicitRecordSize    = 28 + 8 * unCount;
		int unType                  = EMR_POLYDRAW;

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

	void CEmfInterpretator::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (NULL == arPoints || NULL == pAbTypes)
			return;

		int unExplicitRecordSize    = 28 + 4 * unCount;
		int unType                  = EMR_POLYDRAW16;

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

	void CEmfInterpretator::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 8 * unCountPoints;
		int unType                  = EMR_POLYGON;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 4 * unCountPoints;
		int unType                  = EMR_POLYGON16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 8 * unCountPoints;
		int unType                  = EMR_POLYLINE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 4 * unCountPoints;
		int unType                  = EMR_POLYLINE16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 8 * unCountPoints;
		int unType                  = EMR_POLYLINETO;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointL& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		unsigned int unCountPoints = arPoints.size();

		int unExplicitRecordSize    = 28 + 4 * unCountPoints;
		int unType                  = EMR_POLYLINETO16;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unCountPoints,  sizeof (unsigned int));

		for (const TEmfPointS& oPoint : arPoints)
			WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TEmfPointL>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize    = 32 + 4 * unNumberOfPolygons + 8 * unCountPoints;
		int unType                  = EMR_POLYPOLYGON;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TEmfPointL>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TEmfPointL>& arTempPoints : arPoints)
			for (const TEmfPointL& oPoint : arTempPoints)
				WritePoint(oPoint);

	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TEmfPointS>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize    = 32 + 4 * unNumberOfPolygons + 4 * unCountPoints;
		int unType                  = EMR_POLYPOLYGON16;


		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TEmfPointS>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TEmfPointS>& arTempPoints : arPoints)
			for (const TEmfPointS& oPoint : arTempPoints)
				WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TEmfPointL>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize    = 32 + 4 * unNumberOfPolygons + 8 * unCountPoints;
		int unType                  = EMR_POLYPOLYLINE;


		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TEmfPointL>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TEmfPointL>& arTempPoints : arPoints)
			for (const TEmfPointL& oPoint : arTempPoints)
				WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		const unsigned int unNumberOfPolygons = arPoints.size();

		unsigned int unCountPoints = 0;

		for (const std::vector<TEmfPointS>& arTempPoints : arPoints)
			unCountPoints += arTempPoints.size();

		int unExplicitRecordSize    = 32 + 4 * unNumberOfPolygons + 4 * unCountPoints;
		int unType                  = EMR_POLYPOLYLINE16;


		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unNumberOfPolygons,     sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&unCountPoints,          sizeof (unsigned int));

		for (const std::vector<TEmfPointS>& arTempPoints : arPoints)
		{
			unsigned int  unPointCount = arTempPoints.size();
			m_pOutStream->WriteFile((BYTE*)&unPointCount,   sizeof (unsigned int));
		}

		for (const std::vector<TEmfPointS>& arTempPoints : arPoints)
			for (const TEmfPointS& oPoint : arTempPoints)
				WritePoint(oPoint);
	}

	void CEmfInterpretator::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_RECTANGLE;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
	}

	void CEmfInterpretator::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
	{
		int unExplicitRecordSize    = 32;
		int unType                  = EMR_ROUNDRECT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBox);
		WriteSize(oCorner);
	}

	void CEmfInterpretator::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
	{
		int unExplicitRecordSize    = 20;
		int unType                  = EMR_SETPIXELV;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WritePoint(oPoint);
		WriteColor(oColor);
	}

	void CEmfInterpretator::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{
		int unExplicitRecordSize    = 36 + ((oText.fuOptions & ETO_NO_RECT) ? 0 : 16) +
				oText.cChars * ((oText.fuOptions & ETO_SMALL_CHARS) ? sizeof (unsigned char)
																	: sizeof (unsigned short));
		int unType                  = EMR_SMALLTEXTOUT;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		m_pOutStream->WriteFile((BYTE*)&oText.x,               sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oText.y,               sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oText.cChars,          sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oText.fuOptions,       sizeof (unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oText.iGraphicsMode,   sizeof (unsigned int));

		WriteFloat(oText.exScale);
		WriteFloat(oText.eyScale);

		if (!(oText.fuOptions & ETO_NO_RECT))
			WriteRectangle(oText.Bounds);

		if (oText.cChars > 0)
		{
			if (oText.fuOptions & ETO_SMALL_CHARS)
				m_pOutStream->WriteFile((BYTE*)&oText.TextString ,sizeof (unsigned char) * oText.cChars);
			else
				m_pOutStream->WriteFile((BYTE*)&oText.TextString ,sizeof (unsigned short) * oText.cChars);
		}
	}

	void CEmfInterpretator::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_STROKEANDFILLPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);
	}

	void CEmfInterpretator::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
	{
		int unExplicitRecordSize    = 24;
		int unType                  = EMR_STROKEPATH;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);
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

	void CEmfInterpretator::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		int unRgnDataSize           = 32 + arRects.size() * 16;
		int unExplicitRecordSize    = 32 + unRgnDataSize;
		int unType                  = EMR_FILLRGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unRgnDataSize,          sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unIhBrush,              sizeof (int));

		WriteRegionDataHeader(oRegionDataHeader);

		for (const TEmfRectL &oRect : arRects)
			WriteRectangle(oRect);
	}

	void CEmfInterpretator::HANDLE_EMR_PAINTRGN(const TEmfRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		int unRgnDataSize           = 32 + arRects.size() * 16;
		int unExplicitRecordSize    = 28 + unRgnDataSize;
		int unType                  = EMR_PAINTRGN;

		unFileSize += unExplicitRecordSize;
		++unNumberRecords;

		m_pOutStream->WriteFile((BYTE*)&unType,                sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&unExplicitRecordSize,  sizeof (int));

		WriteRectangle(oBounds);

		m_pOutStream->WriteFile((BYTE*)&unRgnDataSize,          sizeof (int));

		WriteRegionDataHeader(oRegionDataHeader);

		for (const TEmfRectL &oRect : arRects)
			WriteRectangle(oRect);
	}

	void CEmfInterpretator::HANDLE_EMR_FRAMERGN(const TEmfRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		int unRgnDataSize           = 40 + arRects.size() * 16;
		int unExplicitRecordSize    = 28 + unRgnDataSize;
		int unType                  = EMR_FRAMERGN;

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

		for (const TEmfRectL &oRect : arRects)
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

	void CEmfInterpretator::WriteRectangle(const TEmfRectL &oBounds)
	{
		m_pOutStream->WriteFile((BYTE*)&oBounds.lLeft,   sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oBounds.lTop,    sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oBounds.lRight,  sizeof(int));
		m_pOutStream->WriteFile((BYTE*)&oBounds.lBottom, sizeof(int));
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

	void CEmfInterpretator::WriteSize(const TEmfSizeL &oSize)
	{
		m_pOutStream->WriteFile((BYTE*)&oSize.cx,   sizeof(unsigned int));
		m_pOutStream->WriteFile((BYTE*)&oSize.cy,   sizeof(unsigned int));
	}

	void CEmfInterpretator::WriteColor(const TEmfColor &oColor)
	{
		m_pOutStream->WriteFile((BYTE*)&oColor.r, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oColor.g, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oColor.b, sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oColor.a, sizeof (unsigned char));
	}

	void CEmfInterpretator::WriteRColor(const TEmfLogPaletteEntry &oRColor)
	{
		m_pOutStream->WriteFile((BYTE*)&oRColor.Reserved,  sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oRColor.Blue,      sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oRColor.Green,     sizeof (unsigned char));
		m_pOutStream->WriteFile((BYTE*)&oRColor.Red,       sizeof (unsigned char));
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

	void CEmfInterpretator::WritePoint(const TEmfPointL &oPoint)
	{
		m_pOutStream->WriteFile((BYTE*)&oPoint.x, sizeof (int));
		m_pOutStream->WriteFile((BYTE*)&oPoint.y, sizeof (int));
	}

	void CEmfInterpretator::WritePoint(const TEmfPointS &oPoint)
	{
		m_pOutStream->WriteFile((BYTE*)&oPoint.x, sizeof (short));
		m_pOutStream->WriteFile((BYTE*)&oPoint.y, sizeof (short));
	}

	void CEmfInterpretator::WritePoint(const TEmfPlusPointR &oPoint)
	{
		//TODO: реализовать
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


