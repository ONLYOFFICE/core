#ifndef _WMF_OUTPUT_DEVICE_H
#define _WMF_OUTPUT_DEVICE_H

#include "WmfTypes.h"

class CWmfOutputDevice
{
public:

	CWmfOutputDevice() {}
	virtual ~CWmfOutputDevice() {}

	virtual bool IsSupportPolypolygon() { return false; }

	// Начало и окончание работы устройства
	virtual void Begin() = 0;
	virtual void End  () = 0;

	virtual void Flood_Interior(TWmfFlood*) = 0;
	virtual void Flood_Exterior(TWmfFlood*) = 0;

	virtual void Draw_Pixel      (TWmfDrawPixel*)     = 0;
	virtual void Draw_Pie        (TWmfDrawArc*)       = 0;
	virtual void Draw_Chord      (TWmfDrawArc*)       = 0;
	virtual void Draw_Arc        (TWmfDrawArc*)       = 0;
	virtual void Draw_Ellipse    (TWmfDrawArc*)       = 0;
	virtual void Draw_Line       (TWmfDrawLine*)      = 0;
	virtual void Poly_Line       (TWmfPolyLine*)      = 0;
	virtual void Draw_Polygon    (TWmfPolyLine*)      = 0;
	virtual void Draw_Polypolygon(TWmfPolyPoly*)      = 0;
	virtual void Draw_Rectangle  (TWmfDrawRectangle*) = 0;

	virtual void Rop_Draw(TWmfROPDraw*) = 0;
	virtual void Bmp_Draw(TWmfBMPDraw*) = 0;
	virtual void Bmp_Read(TWmfBMPRead*) = 0;
	virtual void Bmp_Free(TWmfBMP*)     = 0;

	virtual void Draw_Text(TWmfDrawText*) = 0;

	virtual void Region_Frame(TWmfPolyRectangle*) = 0;
	virtual void Region_Paint(TWmfPolyRectangle*) = 0;
	virtual void Region_Clip (TWmfPolyRectangle*) = 0;
private:

};

#endif /* _WMF_OUTPUT_DEVICE_H */