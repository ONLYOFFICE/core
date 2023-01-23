#ifndef CEMFPARSER_H
#define CEMFPARSER_H

#include "CEmfParserBase.h"
#include "CEmfPlusParser.h"

namespace MetaFile
{
	class CEmfParser : public CEmfParserBase
	{
	public:
		CEmfParser();
		virtual ~CEmfParser();

		bool            ReadFromBuffer(BYTE* pBuffer, unsigned int unSize, const bool& bIsExternal = true) override;
		bool            OpenFromFile(const wchar_t* wsFilePath)     override;

		void            PlayFile()                                  override;
		void            Scan()                                      override;

		void            ClearFile()                                 override;

		EmfParserType   GetType()				                    override;
		void            SetStream(BYTE* pBuf, unsigned int unSize);

	public:
		virtual void SetInterpretator(IOutputDevice* pOutput) override;

	private:
		CEmfPlusParser  *m_pEmfPlusParser;

		bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight) override;
		void Read_EMR_HEADER();
		void Read_EMR_ALPHABLEND();
		void Read_EMR_STRETCHDIBITS();
		void Read_EMR_BITBLT();
		void Read_EMR_SETDIBITSTODEVICE();
		void Read_EMR_STRETCHBLT();
		void Read_EMR_EOF();
		void Read_EMR_UNKNOWN();
		void Read_EMR_SAVEDC();
		void Read_EMR_RESTOREDC();
		void Read_EMR_MODIFYWORLDTRANSFORM();
		void Read_EMR_SETWORLDTRANSFORM();
		void Read_EMR_CREATEBRUSHINDIRECT();
		void Read_EMR_SETTEXTCOLOR();
		void Read_EMR_SELECTOBJECT();
		void Read_EMR_EXTCREATEFONTINDIRECTW();
		void Read_EMR_SETTEXTALIGN();
		void Read_EMR_SETBKMODE();
		void Read_EMR_DELETEOBJECT();
		void Read_EMR_SETMITERLIMIT();
		void Read_EMR_EXTCREATEPEN();
		void Read_EMR_CREATEPEN();
		void Read_EMR_SETPOLYFILLMODE();
		void Read_EMR_BEGINPATH();
		void Read_EMR_ENDPATH();
		void Read_EMR_CLOSEFIGURE();
		void Read_EMR_FLATTENPATH();
		void Read_EMR_WIDENPATH();
		void Read_EMR_ABORTPATH();
		void Read_EMR_MOVETOEX();
		void Read_EMR_SETARCDIRECTION();
		void Read_EMR_FILLPATH();
		void Read_EMR_SETMAPMODE();
		void Read_EMR_SETWINDOWORGEX();
		void Read_EMR_SETWINDOWEXTEX();
		void Read_EMR_SCALEWINDOWEXTEX();
		void Read_EMR_SETVIEWPORTORGEX();
		void Read_EMR_SETVIEWPORTEXTEX();
		void Read_EMR_SCALEVIEWPORTEXTEX();
		void Read_EMR_SETSTRETCHBLTMODE();
		void Read_EMR_SETICMMODE();
		void Read_EMR_CREATEDIBPATTERNBRUSHPT();
		void Read_EMR_CREATEMONOBRUSH();
		void Read_EMR_SELECTCLIPPATH();
		void Read_EMR_SETBKCOLOR();
		void Read_EMR_EXCLUDECLIPRECT();
		void Read_EMR_EXTSELECTCLIPRGN();
		void Read_EMR_SETMETARGN();
		void Read_EMR_SETROP2();
		void Read_EMR_CREATEPALETTE();
		void Read_EMR_SELECTPALETTE();
		void Read_EMR_REALIZEPALETTE();
		void Read_EMR_INTERSECTCLIPRECT();
		void Read_EMR_SETLAYOUT();
		void Read_EMR_SETBRUSHORGEX();
		void Read_EMR_ANGLEARC();
		void Read_EMR_ARC_BASE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd, double& dStartAngle, double& dSweepAngle);
		void Read_EMR_ARC();
		void Read_EMR_ARCTO();
		void Read_EMR_CHORD();
		void Read_EMR_ELLIPSE();
		void Read_EMR_EXTTEXTOUTA();
		void Read_EMR_EXTTEXTOUTW();
		void Read_EMR_LINETO();
		void Read_EMR_PIE();
		void Read_EMR_POLYBEZIER();
		void Read_EMR_POLYBEZIER16();
		template<typename T>void Read_EMR_POLYBEZIER_BASE();
		void Read_EMR_POLYBEZIERTO();
		void Read_EMR_POLYBEZIERTO16();
		template<typename T>void Read_EMR_POLYBEZIERTO_BASE();
		void Read_EMR_POLYDRAW();
		void Read_EMR_POLYDRAW16();
		template<typename T>void Read_EMR_POLYDRAW_BASE();
		void Read_EMR_POLYGON();
		void Read_EMR_POLYGON16();
		template<typename T>void Read_EMR_POLYGON_BASE();
		void Read_EMR_POLYLINE();
		void Read_EMR_POLYLINE16();
		template<typename T>void Read_EMR_POLYLINE_BASE();
		void Read_EMR_POLYLINETO();
		void Read_EMR_POLYLINETO16();
		template<typename T>void Read_EMR_POLYLINETO_BASE();
		void Read_EMR_POLYPOLYGON();
		void Read_EMR_POLYPOLYGON16();
		template<typename T>void Read_EMR_POLYPOLYGON_BASE();
		void Read_EMR_POLYPOLYLINE();
		void Read_EMR_POLYPOLYLINE16();
		template<typename T>void Read_EMR_POLYPOLYLINE_BASE();
		void Read_EMR_POLYTEXTOUTA();
		void Read_EMR_POLYTEXTOUTW();
		void Read_EMR_RECTANGLE();
		void Read_EMR_ROUNDRECT();
		void Read_EMR_SETPIXELV();
		void Read_EMR_SMALLTEXTOUT();
		void Read_EMR_STROKEANDFILLPATH();
		void Read_EMR_STROKEPATH();

		void Read_EMR_COMMENT();
		void Read_EMR_FILLRGN();
		void Read_EMR_PAINTRGN();
		void Read_EMR_FRAMERGN();
	};
}

#endif // CEMFPARSER_H
