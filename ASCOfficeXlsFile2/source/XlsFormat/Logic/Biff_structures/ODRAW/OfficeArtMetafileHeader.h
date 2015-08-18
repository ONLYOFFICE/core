#pragma once

#include "../BiffStructure.h"
#include "OfficeArtCOLORREF.h"
#include "../FixedPoint.h"

#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct tagPOINT
    {
        _INT32  x;
        _INT32  y;
    } POINT;

    typedef struct tagRECT
    {
        _INT32    left;
        _INT32    top;
        _INT32    right;
        _INT32    bottom;
    } RECT;
#else
	#include <windows.h>
#endif

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{

class OfficeArtMetafileHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtMetafileHeader)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtMetafileHeader;
	
	virtual void store(XLS::CFRecord& record);
	virtual void load(XLS::CFRecord& record);

	unsigned int cbSize;
	RECT rcBounds;
	POINT ptSize;
	unsigned int cbSave;
	unsigned char compression;
	unsigned char filter;
};

typedef boost::shared_ptr<OfficeArtMetafileHeader> OfficeArtMetafileHeaderPtr;







} // namespace XLS
