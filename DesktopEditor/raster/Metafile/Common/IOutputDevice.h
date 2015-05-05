#ifndef _METAFILE_COMMON_IOUTPUTDEVICE_H
#define _METAFILE_COMMON_IOUTPUTDEVICE_H

#include "MetaFileTypes.h"

namespace MetaFile
{
	class IOutputDevice
	{
	public:

		IOutputDevice() {}
		virtual ~IOutputDevice() {}

		// Начало и окончание работы устройства
		virtual void Begin() = 0;
		virtual void End() = 0;

		// pBuffer - BGRA картинка размерами ulWidth, ulHeight, которую надо нарисовать в заданном ректе
		virtual void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight) = 0;

		virtual void DrawString(std::wstring& wsText, unsigned int unCharsCount, double dX, double dY, int nTextW, bool bWithOutLast) = 0;

		virtual void StartPath() = 0;
		virtual void MoveTo(double dX, double dY) = 0;
		virtual void LineTo(double dX, double dY) = 0;
		virtual void CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe) = 0;
		virtual void ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle) = 0;
		virtual void ClosePath() = 0;
		virtual void DrawPath(int nType = 0) = 0;
		virtual void EndPath() = 0;

		virtual void ResetClip() = 0;
		virtual void IntersectClip(double dLeft, double dTop, double dRight, double dBottom) = 0;
		virtual void StartClipPath(unsigned int unMode, int nFillMode = -1) = 0;
		virtual void EndClipPath(unsigned int unMode) = 0;

		virtual void UpdateDC() = 0;
	};

}

#endif //_METAFILE_COMMON_IOUTPUTDEVICE_H
