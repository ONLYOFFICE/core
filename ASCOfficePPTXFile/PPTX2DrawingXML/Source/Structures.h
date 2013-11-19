#pragma once
#ifndef PPTX2DRAWINGXML_STRUCTURES_INCLUDE_H_
#define PPTX2DRAWINGXML_STRUCTURES_INCLUDE_H_

#include "stdafx.h"

namespace PPTX2DrawingXML
{
	struct TableCellData
	{
		int x;
		int y;
		int width;
		int height;
		int colsCount;
		int rowsCount;
		int colNumber;
		int rowNumber;
		bool firstCol;
		bool lastCol;
		bool firstRow;
		bool lastRow;
		bool band1H;
		bool band1V;
	};
} // namespace PPTX2DrawingXML

#endif //PPTX2DRAWINGXML_STRUCTURES_INCLUDE_H_