#pragma once

#include "MediaFormatDefine.h"

#define PLANE_COUNT			4

struct SUncompressedVideoFrame
{
	BYTE* Plane[PLANE_COUNT];
	LONG Stride[PLANE_COUNT];

	LONG Width;
	LONG Height;

	LONG ColorSpace;
	LONG AspectX;
	LONG AspectY;
	BOOL Interlaced;

	double kx;
	double ky;

	SUncompressedVideoFrame()
		: Width(320)
		, Height(240)
		, ColorSpace(CSP_BGRA|CSP_VFLIP)
		, AspectX(0)
		, AspectY(1)
		, Interlaced(false)
		, kx(1.0)
		, ky(1.0)
	{
		for (int i=0;i<PLANE_COUNT;i++)
		{
            Plane[i] = NULL;
			Stride[i] = 0;
		}
	}
	SUncompressedVideoFrame& operator= (const SUncompressedVideoFrame& x)
	{
		for (int i=0;i<PLANE_COUNT;i++)
		{
            Plane[i] = x.Plane[i];
			Stride[i] = x.Stride[i];
		}

		Width = x.Width;
		Height = x.Height;

		ColorSpace = x.ColorSpace;
		AspectX = x.AspectX;
		AspectY = x.AspectY;
		Interlaced = x.Interlaced;

		kx = x.kx;
		ky = x.ky;

		return *this;
	}
};