#ifndef _EMF_OUTPUT_DEVICE_H
#define _EMF_OUTPUT_DEVICE_H

#include "../Wmf/WmfTypes.h"

namespace MetaFile
{

	class CEmfOutputDevice
	{
	public:

		CEmfOutputDevice() {}
		virtual ~CEmfOutputDevice() {}

		// Начало и окончание работы устройства
		virtual void Begin() = 0;
		virtual void End() = 0;

		// pBuffer - BGRA картинка размерами ulWidth, ulHeight, которую надо нарисовать в заданном ректе
		virtual void DrawBitmap(int lX, int lY, int lW, int lH, BYTE* pBuffer, unsigned int ulWidth, unsigned int ulHeight) = 0;

		virtual void DrawText(const wchar_t* wsText, unsigned int ulCharsCount, int lX, int lY, int nTextW, bool bWithOutLast) = 0;

		virtual void StartPath() = 0;
		virtual void MoveTo(int lX, int lY) = 0;
		virtual void LineTo(int lX, int lY) = 0;
		virtual void CurveTo(int lX1, int lY1, int lX2, int lY2, int lXe, int lYe) = 0;
		virtual void ArcTo(int lLeft, int lTop, int lRight, int lBottom, double dStartAngle, double dSweepAngle) = 0;
		virtual void ClosePath() = 0;
		virtual void DrawPath(int lType = 0) = 0;
		virtual void EndPath() = 0;

		virtual void ResetClip() = 0;
		virtual void IntersectClip(int lLeft, int lTop, int lRight, int lBottom) = 0;
		virtual void StartClipPath(unsigned int unMode) = 0;
		virtual void EndClipPath(unsigned int unMode) = 0;


		virtual void UpdateDC() = 0;
	};

}

#endif //_EMF_OUTPUT_DEVICE_H