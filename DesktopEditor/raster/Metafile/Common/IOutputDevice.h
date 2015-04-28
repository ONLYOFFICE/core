#ifndef _METAFILE_COMMON_IOUTPUTDEVICE_H
#define _METAFILE_COMMON_IOUTPUTDEVICE_H

#include "../Wmf/WmfTypes.h"
#include <string>

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
		virtual void DrawBitmap(int nX, int nY, int nW, int nH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight) = 0;

		virtual void DrawText(std::wstring& wsText, unsigned int unCharsCount, int nX, int nY, int nTextW, bool bWithOutLast) = 0;

		virtual void StartPath() = 0;
		virtual void MoveTo(int nX, int nY) = 0;
		virtual void LineTo(int nX, int nY) = 0;
		virtual void CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe) = 0;
		virtual void ArcTo(int nLeft, int nTop, int nRight, int nBottom, double dStartAngle, double dSweepAngle) = 0;
		virtual void ClosePath() = 0;
		virtual void DrawPath(int nType = 0) = 0;
		virtual void EndPath() = 0;

		virtual void ResetClip() = 0;
		virtual void IntersectClip(int nLeft, int nTop, int nRight, int nBottom) = 0;
		virtual void StartClipPath(unsigned int unMode) = 0;
		virtual void EndClipPath(unsigned int unMode) = 0;

		virtual void UpdateDC() = 0;
	};

}

#endif //_METAFILE_COMMON_IOUTPUTDEVICE_H
