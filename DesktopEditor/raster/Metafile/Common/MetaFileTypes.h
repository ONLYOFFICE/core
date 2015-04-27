#ifndef _METAFILE_COMMON_METAFILETYPES_H
#define _METAFILE_COMMON_METAFILETYPES_H

namespace MetaFile
{
	struct TRect
	{
		long lLeft;
		long lTop;
		long lRight;
		long lBottom;
	};

	struct TPointL
	{
		int x;
		int y;
	};

	struct TPointD
	{
		double x;
		double y;
	};

	struct TColor
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
}

#endif //_METAFILE_COMMON_METAFILETYPES_H