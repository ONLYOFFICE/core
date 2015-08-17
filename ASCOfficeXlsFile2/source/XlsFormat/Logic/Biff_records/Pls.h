#pragma once

#include "BiffRecordContinued.h"

#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct tagPOINT
    {
        long  x;
        long  y;
    } POINT;

    typedef struct _devicemodeW
    {
        short  dmDeviceName[/*CCHDEVICENAME*/32];//utf16
        _UINT16 dmSpecVersion;
        _UINT16 dmDriverVersion;
        _UINT16 dmSize;
        _UINT16 dmDriverExtra;
        _UINT32 dmFields;
        union
        {
          /* printer only fields */
          struct
          {
            short dmOrientation;
            short dmPaperSize;
            short dmPaperLength;
            short dmPaperWidth;
            short dmScale;
            short dmCopies;
            short dmDefaultSource;
            short dmPrintQuality;
          };
          /* display only fields */
          struct
          {
            POINT dmPosition;
            _UINT32  dmDisplayOrientation;
            _UINT32  dmDisplayFixedOutput;
          };
        };
        short dmColor;
        short dmDuplex;
        short dmYResolution;
        short dmTTOption;
        short dmCollate;
        short  dmFormName[/*CCHFORMNAME*/32];//utf16
        _UINT16   dmLogPixels;
        _UINT32  dmBitsPerPel;
        _UINT32  dmPelsWidth;
        _UINT32  dmPelsHeight;
        union {
            _UINT32  dmDisplayFlags;
            _UINT32  dmNup;
        };
        _UINT32  dmDisplayFrequency;
    #if(WINVER >= 0x0400)
        _UINT32  dmICMMethod;
        _UINT32  dmICMIntent;
        _UINT32  dmMediaType;
        _UINT32  dmDitherType;
        _UINT32  dmReserved1;
        _UINT32  dmReserved2;
    #if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
        _UINT32  dmPanningWidth;
        _UINT32  dmPanningHeight;
    #endif
    #endif /* WINVER >= 0x0400 */
    } DEVMODE;
#endif

namespace XLS
{


// Logical representation of Pls record in BIFF8
class Pls: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pls)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pls)
public:
	Pls(BaseObject* parent);
	~Pls();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePls;

//-----------------------------
	DEVMODE rgb;
	BIFF_LONG bin_data_id;
	BaseObject* parent_;


};

} // namespace XLS

