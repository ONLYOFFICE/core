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
		virtual void DrawBitmap(long lX, long lY, long lW, long lH, BYTE* pBuffer, unsigned long ulWidth, unsigned long ulHeight) = 0;

		virtual void DrawText(const wchar_t* wsText, unsigned long ulCharsCount, long lX, long lY) = 0;

		virtual void StartPath() = 0;
		virtual void MoveTo(long lX, long lY) = 0;
		virtual void LineTo(long lX, long lY) = 0;
		virtual void ClosePath() = 0;
		virtual void DrawPath() = 0;
		virtual void EndPath() = 0;
	};

}

#endif //_EMF_OUTPUT_DEVICE_H