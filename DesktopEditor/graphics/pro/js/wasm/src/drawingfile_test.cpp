#include <iostream>

#include "../../../../raster/BgraFrame.h"
#include "../../../../raster/ImageFileFormatChecker.h"
#include "../../../../../common/File.h"
#include "../../../../../common/StringBuilder.h"
#include "drawingfile.cpp"

unsigned char READ_BYTE(BYTE* x)
{
	return x ? x[0] : 1;
}
unsigned short READ_SHORT(BYTE* x)
{
	return x ? (x[0] | x[1] << 8) : 2;
}
unsigned int READ_INT(BYTE* x)
{
	return x ? (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24) : 4;
}
std::string arrAnnots[] = {"Text", "Link", "FreeText", "Line", "Square", "Circle", "Polygon", "PolyLine", "Highlight", "Underline", "Squiggly", "StrikeOut", "Stamp", "Caret", "Ink", "Popup",
						   "FileAttachment", "Sound", "Movie", "Widget", "Screen", "PrinterMark", "TrapNet", "Watermark", "3D", "Redact", "unknown", "button", "radiobutton", "checkbox", "text",
						   "combobox", "listbox", "signature"};

void ReadAction(BYTE* pWidgets, int& i)
{
	int nPathLength = READ_BYTE(pWidgets + i);
	i += 1;
	std::string arrAction[] = {"Unknown", "GoTo", "GoToR", "GoToE", "Launch", "Thread", "URI", "Sound", "Movie", "Hide",
							   "Named", "SubmitForm", "ResetForm", "ImportData", "JavaScript", "SetOCGState", "Rendition",
							   "Trans", "GoTo3DView"};
	std::string sType = arrAction[nPathLength];
	std::cout << "Type " << sType << ", ";

	if (sType == "JavaScript")
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "JS " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	else if (sType == "GoTo")
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Page " << nPathLength << ", ";

		int nKind = READ_BYTE(pWidgets + i);
		i += 1;
		std::cout << "kind " << nKind << ", ";
		switch (nKind)
		{
		case 0:
		case 2:
		case 3:
		case 6:
		case 7:
		{
			int nFlags = READ_BYTE(pWidgets + i);
			i += 1;
			if (nFlags & (1 << 0))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "left " << (double)nPathLength / 100.0 << ", ";
			}
			if (nFlags & (1 << 1))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "top " << (double)nPathLength / 100.0 << ", ";
			}
			if (nFlags & (1 << 2))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "zoom " << (double)nPathLength / 100.0 << ", ";
			}
			break;
		}
		case 4:
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "left " << (double)nPathLength / 100.0 << ", ";

			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "bottom " << (double)nPathLength / 100.0 << ", ";

			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "right " << (double)nPathLength / 100.0 << ", ";

			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "top " << (double)nPathLength / 100.0 << ", ";
			break;
		}
		case 1:
		case 5:
		default:
			break;
		}
	}
	else if (sType == "Named")
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Named " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	else if (sType == "URI")
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "URL " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	else if (sType == "Hide")
	{
		nPathLength = READ_BYTE(pWidgets + i);
		i += 1;
		std::cout << "Hide flag " << nPathLength << ", ";

		int nHideLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "THide: ";

		for (int j = 0; j < nHideLength; ++j)
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
	}
	else if (sType == "ResetForm")
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "ResetForm flag " << nPathLength << ", ";

		int nResetLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "annots: ";

		for (int j = 0; j < nResetLength; ++j)
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout  << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
	}

	nPathLength = READ_BYTE(pWidgets + i);
	i += 1;
	if (nPathLength)
		ReadAction(pWidgets, i);
}

void ReadAnnot(BYTE* pWidgets, int& i)
{
	int nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "Annot - AP " << nPathLength << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "Flag " << nPathLength << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "Page " << nPathLength << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "X1 " << (double)nPathLength / 10000.0 << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "Y1 " << (double)nPathLength / 10000.0 << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "X2 " << (double)nPathLength / 10000.0 << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "Y2 " << (double)nPathLength / 10000.0 << ", ";

	nPathLength = READ_INT(pWidgets + i);
	i += 4;
	std::cout << "Annot Flags " << nPathLength << ", ";
	int nFlags = nPathLength;

	if (nFlags & (1 << 0))
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Unique name " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	if (nFlags & (1 << 1))
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Contents " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	if (nFlags & (1 << 2))
	{
		std::string arrEffects[] = {"S", "C"};
		nPathLength = READ_BYTE(pWidgets + i);
		i += 1;
		std::cout << "BE " << arrEffects[nPathLength] << " ";

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << (double)nPathLength / 100.0 << ", ";
	}
	if (nFlags & (1 << 3))
	{
		int nCLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "C ";

		for (int j = 0; j < nCLength; ++j)
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << (double)nPathLength / 10000.0 << " ";
		}
		std::cout << ", ";
	}
	if (nFlags & (1 << 4))
	{
		std::string arrBorder[] = {"solid", "beveled", "dashed", "inset", "underline"};
		int nBorderType = READ_BYTE(pWidgets + i);
		i += 1;
		std::cout << "Border type " << arrBorder[nBorderType] << " ";

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "width " << (double)nPathLength / 100.0 << ", ";

		if (nBorderType == 2)
		{
			int nDash = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Dash Pattern";

			for (int j = 0; j < nDash; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << " " << (double)nPathLength / 100.0;
			}
			std::cout << ", ";
		}
	}
	if (nFlags & (1 << 5))
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Last modified " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	if (nFlags & (1 << 6))
		std::cout << "YES AP, ";
	else
		std::cout << "NO AP, ";
	if (nFlags & (1 << 7))
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "User ID " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
	if (nFlags & (1 << 9))
	{
		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "OMetadata " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;
	}
}

void ReadInteractiveForms(BYTE* pWidgets, int& i)
{
	int nCOLength = READ_INT(pWidgets + i);
	i += 4;
	if (nCOLength > 0)
		std::cout << "CO ";
	for (int j = 0; j < nCOLength; ++j)
	{
		int nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << nPathLength << ", ";
	}
	if (nCOLength > 0)
		std::cout << std::endl;

	// Parents

	int nParentsLength = READ_INT(pWidgets + i);
	i += 4;
	if (nParentsLength > 0)
		std::cout << "Parents" << std::endl;
	for (int j = 0; j < nParentsLength; ++j)
	{
		int nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "# " << nPathLength << ", ";

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		int nFlags = nPathLength;

		if (nFlags & (1 << 0))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "T " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 1))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "V " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 2))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "DV " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 3))
		{
			int nILength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "I";

			for (int j = 0; j < nILength; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << " " << nPathLength;
			}
			std::cout << ", ";
		}
		if (nFlags & (1 << 4))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Parent " << nPathLength << ", ";
		}
		if (nFlags & (1 << 5))
		{
			int nILength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "V";

			for (int j = 0; j < nILength; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << " " << std::string((char*)(pWidgets + i), nPathLength);
				i += nPathLength;
			}
			std::cout << ", ";
		}
		if (nFlags & (1 << 6))
		{
			int nILength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Opt [";

			for (int j = 0; j < nILength; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << " " << std::string((char*)(pWidgets + i), nPathLength);
				i += nPathLength;

				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << " " << std::string((char*)(pWidgets + i), nPathLength);
				i += nPathLength;
			}
			std::cout << " ], ";
		}
		if (nFlags & (1 << 7))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Ff " << nPathLength << ", ";
		}
		if (nFlags & (1 << 8))
		{
			int nActLength = READ_INT(pWidgets + i);
			i += 4;
			for (int j = 0; j < nActLength; ++j)
			{
				std::cout << std::endl;
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << std::to_string(j) << " Action " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;

				ReadAction(pWidgets, i);
			}
			std::cout << std::endl;
		}
		if (nFlags & (1 << 9))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "MaxLen " << nPathLength << ", ";
		}
		if (nFlags & (1 << 10))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "TU " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 11))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "MEOptions " << nPathLength << ", ";
		}

		std::cout << std::endl;
	}
	if (nParentsLength > 0)
		std::cout << std::endl;

	int nAnnots = READ_INT(pWidgets + i);
	i += 4;

	for (int q = 0; q < nAnnots; ++q)
	{
		int nPathLength = READ_BYTE(pWidgets + i);
		i += 1;
		std::string sType = arrAnnots[nPathLength];
		std::cout << "Widget type " << sType << ", ";

		// Annot

		ReadAnnot(pWidgets, i);

		// Widget

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Font: name " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
		i += nPathLength;

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "size " << (double)nPathLength / 100.0 << ", ";

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "style " << nPathLength << ", ";

		int nTCLength = READ_INT(pWidgets + i);
		i += 4;
		if (nTCLength)
		{
			std::cout << "color";
			for (int j = 0; j < nTCLength; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << " " << (double)nPathLength / 10000.0;
			}
			std::cout << ", ";
		}

		std::string arrQ[] = {"left-justified", "centered", "right-justified"};
		nPathLength = READ_BYTE(pWidgets + i);
		i += 1;
		std::cout << "Q " << arrQ[nPathLength] << ", ";

		int nFieldFlag = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Field Flag " << nFieldFlag << ", ";

		nPathLength = READ_INT(pWidgets + i);
		i += 4;
		std::cout << "Flags " << nPathLength << ", ";
		DWORD nFlags = nPathLength;

		if (nFlags & (1 << 0))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "TU " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 1))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "DS " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 2))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Actual font " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 3))
		{
			std::string arrHighlighting[] = {"none", "invert", "push", "outline"};
			nPathLength = READ_BYTE(pWidgets + i);
			i += 1;
			std::cout << "Highlight " << arrHighlighting[nPathLength] << ", ";
		}
		if (nFlags & (1 << 4))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Font key " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 5))
		{
			int nBCLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "BC ";

			for (int j = 0; j < nBCLength; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << (double)nPathLength / 10000.0 << " ";
			}
			std::cout << ", ";
		}
		if (nFlags & (1 << 6))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "R " << nPathLength << ", ";
		}
		if (nFlags & (1 << 7))
		{
			int nBCLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "BG ";

			for (int j = 0; j < nBCLength; ++j)
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << (double)nPathLength / 10000.0 << " ";
			}
			std::cout << ", ";
		}
		if (nFlags & (1 << 8))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "DV " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 17))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Parent " << nPathLength << ", ";
		}
		if (nFlags & (1 << 18))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Name " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 19))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Font button " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
		}
		if (nFlags & (1 << 21))
		{
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "MEOptions " << nPathLength << ", ";
		}

		//Action

		int nActLength = READ_INT(pWidgets + i);
		i += 4;
		for (int j = 0; j < nActLength; ++j)
		{
			std::cout << std::endl;
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << std::to_string(j) << " Action " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;

			ReadAction(pWidgets, i);
		}
		std::cout << std::endl;

		// Widget types

		if (sType == "button")
		{
			if (nFlags & (1 << 9))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "Value " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}

			int nIFFlag = READ_INT(pWidgets + i);
			i += 4;

			if (nFlags & (1 << 10))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "CA " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 11))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "RC " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 12))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "AC " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 13))
			{
				std::string arrTP[] = {"textOnly", "iconOnly", "iconTextV", "textIconV", "iconTextH", "textIconH", "overlay"};
				nPathLength = READ_BYTE(pWidgets + i);
				i += 1;
				std::cout << "TP " << arrTP[nPathLength] << ", ";
			}
			if (nIFFlag & (1 << 0))
			{
				if (nIFFlag & (1 << 1))
				{
					std::string arrSW[] = {"A", "N", "B", "S"};
					nPathLength = READ_BYTE(pWidgets + i);
					i += 1;
					std::cout << "SW " << arrSW[nPathLength] << ", ";
				}
				if (nIFFlag & (1 << 2))
				{
					std::string arrS[] = {"P", "A"};
					nPathLength = READ_BYTE(pWidgets + i);
					i += 1;
					std::cout << "S " << arrS[nPathLength] << ", ";
				}
				if (nIFFlag & (1 << 3))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "A " << (double)nPathLength / 100.0 << " ";

					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << (double)nPathLength / 100.0 << ", ";
				}
				std::cout << "FB " << (nIFFlag & (1 << 4)) << ", ";
			}
		}
		else if (sType == "checkbox" || sType == "radiobutton")
		{
			if (nFlags & (1 << 9))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "Value " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}

			std::string arrStyle[] = {"check", "cross", "diamond", "circle", "star", "square"};
			nPathLength = READ_BYTE(pWidgets + i);
			i += 1;
			std::cout << "Style " << arrStyle[nPathLength] << ", ";

			if (nFlags & (1 << 10))
			{
				int nOptLength = READ_INT(pWidgets + i);
				i += 4;
				for (int j = 0; j < nOptLength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << std::to_string(j) << " Opt1 " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;

					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << std::to_string(j) << " Opt2 " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
			}
			if (nFlags & (1 << 14))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "AP.N.Yes " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
		}
		else if (sType == "text")
		{
			if (nFlags & (1 << 9))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "Value " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 10))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "MaxLen " << nPathLength << ", ";
			}
			if (nFlags & (1 << 11))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "RichValue " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
		}
		else if (sType == "combobox" || sType == "listbox")
		{
			if (nFlags & (1 << 9))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "Value " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 10))
			{
				int nOptLength = READ_INT(pWidgets + i);
				i += 4;
				for (int j = 0; j < nOptLength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << std::to_string(j) << " Opt1 " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;

					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << std::to_string(j) << " Opt2 " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
			}
			if (nFlags & (1 << 11))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "TI " << nPathLength << ", ";
			}
			if (nFlags & (1 << 12))
			{
				int nILength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "I";
				for (int j = 0; j < nILength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << " " << nPathLength;
				}
				std::cout << ", ";
			}
			if (nFlags & (1 << 13))
			{
				int nILength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "Value";
				for (int j = 0; j < nILength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << " " << std::string((char*)(pWidgets + i), nPathLength);
					i += nPathLength;
				}
				std::cout << ", ";
			}
		}
		else if (sType == "signature")
		{
			if (nFlags & (1 << 9))
				std::cout << "SIG, ";
		}
		std::cout << std::endl << std::endl;
	}
}

void ReadAnnotAP(BYTE* pWidgetsAP, int& i)
{
	int nAP = READ_INT(pWidgetsAP + i);
	i += 4;
	std::cout << "AP " << nAP << ", ";

	int nPathLength = READ_INT(pWidgetsAP + i);
	i += 4;
	std::cout << "X " << (double)nPathLength / 100.0 << ", ";

	nPathLength = READ_INT(pWidgetsAP + i);
	i += 4;
	std::cout << "Y " << (double)nPathLength / 100.0 << ", ";

	int nWidgetWidth = READ_INT(pWidgetsAP + i);
	i += 4;
	std::cout << "W " << nWidgetWidth << ", ";

	int nWidgetHeight = READ_INT(pWidgetsAP + i);
	i += 4;
	std::cout << "H " << nWidgetHeight << ", ";

	int nAPLength = READ_INT(pWidgetsAP + i);
	i += 4;
	if (nAPLength > 0)
		std::cout << "APName ";

	for (int j = 0; j < nAPLength; ++j)
	{
		nPathLength = READ_INT(pWidgetsAP + i);
		i += 4;
		std::string sAPName = std::string((char*)(pWidgetsAP + i), nPathLength);
		i += nPathLength;

		nPathLength = READ_INT(pWidgetsAP + i);
		i += 4;
		sAPName += nPathLength ? ("." + std::string((char*)(pWidgetsAP + i), nPathLength)) : "";
		i += nPathLength;

		std::cout << sAPName << ", ";
		unsigned long long npBgraData1 = READ_INT(pWidgetsAP + i);
		i += 4;
		unsigned long long npBgraData2 = READ_INT(pWidgetsAP + i);
		i += 4;

		BYTE* res = (BYTE*)(npBgraData2 << 32 | npBgraData1);
		CBgraFrame oFrame;
		oFrame.put_Data(res);
		oFrame.put_Width(nWidgetWidth);
		oFrame.put_Height(nWidgetHeight);
		oFrame.put_Stride(4 * nWidgetWidth);
		oFrame.put_IsRGBA(true);
		oFrame.SaveFile(NSFile::GetProcessDirectory() + L"/res_" + std::to_wstring(nAP) + L"_" + UTF8_TO_U(sAPName) + L".png", _CXIMAGE_FORMAT_PNG);
		oFrame.ClearNoAttack();
		RELEASEARRAYOBJECTS(res);

		nPathLength = READ_BYTE(pWidgetsAP + i);
		i += 1;
		std::string arrBlendMode[] = { "Normal", "Multiply", "Screen", "Overlay", "Darken", "Lighten", "ColorDodge", "ColorBurn", "HardLight",
									   "SoftLight", "Difference", "Exclusion", "Hue", "Saturation", "Color", "Luminosity" };
		std::cout << "Type " << arrBlendMode[nPathLength];
	}
	std::cout << std::endl;
}

void ReadFileAttachment(BYTE* pAnnots, int& i, int n)
{
	std::cout << "EF FileAttachment" << n << ".txt, ";
	int nFileLength = READ_INT(pAnnots + i);
	i += 4;

	unsigned long long npFile1 = READ_INT(pAnnots + i);
	i += 4;
	unsigned long long npFile2 = READ_INT(pAnnots + i);
	i += 4;

	BYTE* res = (BYTE*)(npFile2 << 32 | npFile1);

	NSFile::CFileBinary oFile;
	if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/FileAttachment" + std::to_wstring(n) + L".txt"))
		oFile.WriteFile(res, nFileLength);
	oFile.CloseFile();

	RELEASEARRAYOBJECTS(res);
}

void ReadInteractiveFormsFonts(CDrawingFile* pGrFile, int nType)
{
	BYTE* pFonts = GetInteractiveFormsFonts(pGrFile, nType);
	int nLength = READ_INT(pFonts);
	int i = 4;
	nLength -= 4;

	while (i < nLength)
	{
		int nFontsLength = READ_INT(pFonts + i);
		i += 4;
		std::cout << "Fonts";

		for (int j = 0; j < nFontsLength; ++j)
		{
			std::cout << std::endl;

			int nPathLength = READ_INT(pFonts + i);
			i += 4;
			std::string sFontName = std::string((char*)(pFonts + i), nPathLength);
			std::cout << " " << sFontName << " ";
			i += nPathLength;

			BYTE* pFont = GetFontBinary(pGrFile, (char*)sFontName.c_str());
			int nLength2 = READ_INT(pFont);
			int i2 = 4;
			nLength2 -= 4;

			while (i2 < nLength2)
			{
				int nFontLength = READ_INT(pFont + i2);
				i2 += 4;

				unsigned long long npFont1 = READ_INT(pFont + i2);
				i2 += 4;
				unsigned long long npFont2 = READ_INT(pFont + i2);
				i2 += 4;

				BYTE* res = (BYTE*)(npFont2 << 32 | npFont1);

				NSFile::CFileBinary oFile;
				if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/font" + std::to_wstring(j) + L".txt"))
					oFile.WriteFile(res, nFontLength);
				oFile.CloseFile();

				std::cout << "font" << j << ".txt";
			}

			if (pFont)
				free(pFont);
		}
		std::cout << std::endl;
	}

	if (pFonts)
		free(pFonts);
}

bool GetFromBase64(const std::wstring& sPath, BYTE** pBuffer, int* nBufferLen)
{
	NSFile::CFileBinary oFile;
	if (oFile.OpenFile(sPath))
	{
		DWORD dwFileSize = oFile.GetFileSize();
		BYTE* pFileContent = new BYTE[dwFileSize];
		if (!pFileContent)
		{
			oFile.CloseFile();
			return false;
		}

		DWORD dwReaded;
		if (!oFile.ReadFile(pFileContent, dwFileSize, dwReaded))
			return false;

		*nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
		*pBuffer = new BYTE[*nBufferLen];
		if (!(*pBuffer))
		{
			RELEASEARRAYOBJECTS(pFileContent);
			return false;
		}

		if (!NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, *pBuffer, nBufferLen))
			return false;
	}
	else
		return false;
	oFile.CloseFile();
	return true;
}
#include "../../../../../fontengine/ApplicationFontsWorker.h"
#include "../../../../../common/Directory.h"

int main(int argc, char* argv[])
{
	// CHECK SYSTEM FONTS
	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
	//oWorker.m_arAdditionalFolders.push_back(L"D:\\GIT\\core-fonts");
	oWorker.m_bIsNeedThumbnails = false;

	if (!NSDirectory::Exists(oWorker.m_sDirectory))
	{
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);
		NSFonts::IApplicationFonts* pFonts = oWorker.Check();
		RELEASEINTERFACE(pFonts);
	}

	// INITIALIZE FONTS
	if (true)
	{
		BYTE* pFontSelection = NULL;
		DWORD nFontSelectionLen = 0;
		if (NSFile::CFileBinary::ReadAllBytes(NSFile::GetProcessDirectory() + L"/fonts_cache/font_selection.bin", &pFontSelection, nFontSelectionLen))
		{
			char* pBase64 = NULL;
			int nBase64Len = 0;
			NSFile::CBase64Converter::Encode(pFontSelection, (int)nFontSelectionLen, pBase64, nBase64Len, NSBase64::B64_BASE64_FLAG_NOCRLF);

			InitializeFontsBase64((BYTE*)pBase64, nBase64Len);

			RELEASEARRAYOBJECTS(pBase64);
		}
		RELEASEARRAYOBJECTS(pFontSelection);
	}

	// OPEN FILE
	std::wstring sFilePath = NSFile::GetProcessDirectory() + L"/test.pdf";

	BYTE* pFileData = NULL;
	DWORD nFileDataLen = 0;
	if (!NSFile::CFileBinary::ReadAllBytes(sFilePath, &pFileData, nFileDataLen))
		return 1;

	CDrawingFile* pGrFile = Open(pFileData, (LONG)nFileDataLen, "");
	int nError = GetErrorCode(pGrFile);

	if (nError != 0)
	{
		Close(pGrFile);
		if (nError == 4)
		{
			std::string sPassword = "123456";
			pGrFile = Open(pFileData, nFileDataLen, sPassword.c_str());
		}
		else
		{
			RELEASEARRAYOBJECTS(pFileData);
			return 1;
		}
	}

	RELEASEARRAYOBJECTS(pFileData);

	// SPLIT & MERGE
	if (false)
	{
		int nBufferLen = NULL;
		BYTE* pBuffer = NULL;

		if (true && GetFromBase64(NSFile::GetProcessDirectory() + L"/split.txt", &pBuffer, &nBufferLen))
		{
			std::vector<int> arrPages = { 2 };
			BYTE* pSplitPages = SplitPages(pGrFile, arrPages.data(), arrPages.size(), pBuffer, nBufferLen);
			int nLength = READ_INT(pSplitPages);

			if (nLength > 4)
			{
				NSFile::CFileBinary oFile;
				if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/split.pdf"))
					oFile.WriteFile(pSplitPages + 4, nLength - 4);
				oFile.CloseFile();
			}
			RELEASEARRAYOBJECTS(pSplitPages);
		}
		RELEASEARRAYOBJECTS(pBuffer);

		if (true)
		{
			NSFile::CFileBinary oFile;
			if (oFile.OpenFile(NSFile::GetProcessDirectory() + L"/split.pdf"))
			{
				DWORD dwFileSize = oFile.GetFileSize();
				BYTE* pFileContent = (BYTE*)malloc(dwFileSize);

				DWORD dwReaded;
				if (oFile.ReadFile(pFileContent, dwFileSize, dwReaded))
					MergePages(pGrFile, pFileContent, dwReaded, 0, "merge1");
			}
			oFile.CloseFile();
		}
	}

	// INFO
	BYTE* pInfo = GetInfo(pGrFile);
	int nLength = READ_INT(pInfo);
	nLength -= 4;

	int nPagesCount = 0;
	int nTestPage = argc > 1 ? atoi(argv[1]) : 0;
	int nWidth = 100;
	int nHeight = 100;

	if (nLength > 0)
	{
		std::cout << "MaxID " << READ_INT(pInfo + 4);

		nPagesCount = READ_INT(pInfo + 8);
		if (nPagesCount > 0)
		{
			nWidth  = READ_INT(pInfo + nTestPage * 16 + 12);
			nHeight = READ_INT(pInfo + nTestPage * 16 + 16);
			int dpi = READ_INT(pInfo + nTestPage * 16 + 20);
			int rotate = READ_INT(pInfo + nTestPage * 16 + 24);
			//nWidth  *= 2;
			//nHeight *= 2;
			std::cout << " Page " << nTestPage << " width " << nWidth << " height " << nHeight << " dpi " << dpi << " rotate " << rotate << std::endl;

			nLength = READ_INT(pInfo + nPagesCount * 16 + 12);
			std::cout << "json "<< std::string((char*)(pInfo + nPagesCount * 16 + 16), nLength) << std::endl << std::endl;
		}
	}

	// CMAP
	BYTE* pCMapData = NULL;
	if (IsNeedCMap(pGrFile))
	{
		DWORD nCMapDataLength = 0;
		if (NSFile::CFileBinary::ReadAllBytes(NSFile::GetProcessDirectory() + L"/cmap.bin", &pCMapData, nCMapDataLength))
		{
			SetCMapData(pGrFile, pCMapData, nCMapDataLength);
		}
	}

	// OWNER PASSWORD
	if (false)
	{
		std::string sPassword = "gfhjkmgfhjkm";
		std::cout << "CheckPerm 4 Edit "  << CheckPerm(pGrFile, 4) << std::endl;
		std::cout << "CheckPerm 4 Print " << CheckPerm(pGrFile, 3) << std::endl;

		std::cout << "CheckOwnerPassword " << CheckOwnerPassword(pGrFile, sPassword.c_str()) << std::endl;
		std::cout << "CheckPerm 4 Edit "  << CheckPerm(pGrFile, 4) << std::endl;
		std::cout << "CheckPerm 4 Print " << CheckPerm(pGrFile, 3) << std::endl;
	}

	BYTE* pColor = new BYTE[12] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	// REDACT
	if (false)
	{
		int pRect[8] = { 307499, 217499, 307499, 1124999, 1799999, 1124999, 1799999, 217499 };
		if (!RedactPage(pGrFile, nTestPage, pRect, 1, pColor, 12))
			std::cout << "Redact false" << std::endl;
	}

	int i = nTestPage;
	//for (int i = 0; i < nPagesCount; ++i)
	{
		// RASTER
		if (true)
		{
			nWidth  = READ_INT(pInfo + i * 16 + 12);
			nHeight = READ_INT(pInfo + i * 16 + 16);

			//nWidth  *= 3;
			//nHeight *= 3;

			BYTE* res = NULL;
			res = GetPixmap(pGrFile, i, nWidth, nHeight, 0xFFFFFF);

			CBgraFrame oFrame;
			oFrame.put_Data(res);
			oFrame.put_Width(nWidth);
			oFrame.put_Height(nHeight);
			oFrame.put_Stride(4 * nWidth);
			oFrame.put_IsRGBA(true);
			oFrame.SaveFile(NSFile::GetProcessDirectory() + L"/res/res" + std::to_wstring(i) + L".png", _CXIMAGE_FORMAT_PNG);
			oFrame.ClearNoAttack();

			RELEASEARRAYOBJECTS(res);
		}
	}
	free(pInfo);

	// LINKS
	if (false && nPagesCount > 0)
	{
		BYTE* pLinks = GetLinks(pGrFile, nTestPage);
		nLength = READ_INT(pLinks);
		DWORD i = 4;
		nLength -= 4;
		while (i < nLength)
		{
			int nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout <<  "Link " << std::string((char*)(pLinks + i), nPathLength);
			i += nPathLength;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " Ydest " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " X " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " Y " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " W " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " H " << (double)nPathLength / 100.0 << std::endl;
		}

		std::cout << std::endl;

		if (pLinks)
			free(pLinks);
	}

	// STRUCTURE
	if (false)
	{
		BYTE* pStructure = GetStructure(pGrFile);
		nLength = READ_INT(pStructure);
		DWORD i = 4;
		nLength -= 4;
		while (i < nLength)
		{
			int nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Page " << nPathLength << ", ";
			nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Level " << nPathLength << ", ";
			nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Y " << (double)nPathLength / 100.0 << ", ";
			nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Description " << std::string((char*)(pStructure + i), nPathLength) << std::endl;
			i += nPathLength;
		}

		std::cout << std::endl;

		if (pStructure)
			free(pStructure);
	}

	// GLYPHS
	if (false && nPagesCount > 0)
	{
		BYTE* pGlyphs = GetGlyphs(pGrFile, nTestPage);
		nLength = READ_INT(pGlyphs);
		int i = 4;
		nLength -= 20;

		int nPathLength = READ_INT(pGlyphs + i);
		i += 4;
		std::cout << "Stats Paragraphs " << nPathLength;
		nPathLength = READ_INT(pGlyphs + i);
		i += 4;
		std::cout << " Words " << nPathLength;
		nPathLength = READ_INT(pGlyphs + i);
		i += 4;
		std::cout << " Symbols " << nPathLength;
		nPathLength = READ_INT(pGlyphs + i);
		i += 4;
		std::cout << " Spaces " << nPathLength << std::endl;

		while (i < nLength)
		{
			int nPathLength = READ_INT(pGlyphs + i);
			i += 4;
			std::cout << "Line X " << (double)nPathLength / 10000.0;
			nPathLength = READ_INT(pGlyphs + i);
			i += 4;
			std::cout << " Y " << (double)nPathLength / 10000.0;
			nPathLength = READ_BYTE(pGlyphs + i);
			i += 1;
			if (nPathLength)
			{
				nPathLength = READ_INT(pGlyphs + i);
				i += 4;
				std::cout << " Ex " << (double)nPathLength / 10000.0;
				nPathLength = READ_INT(pGlyphs + i);
				i += 4;
				std::cout << " Ey " << (double)nPathLength / 10000.0;
			}
			nPathLength = READ_INT(pGlyphs + i);
			i += 4;
			std::cout << " Ascent " << (double)nPathLength / 10000.0;
			nPathLength = READ_INT(pGlyphs + i);
			i += 4;
			std::cout << " Descent " << (double)nPathLength / 10000.0;
			nPathLength = READ_INT(pGlyphs + i);
			i += 4;
			std::cout << " LineWidth " << (double)nPathLength / 10000.0;
			int nCharLength = READ_INT(pGlyphs + i);
			i += 4;
			std::cout << " Chars:" << std::endl;
			for (int j = 0; j < nCharLength; ++j)
			{
				int nCharX = 0;
				if (j)
				{
					nCharX = READ_INT(pGlyphs + i);
					i += 4;
				}
				nPathLength = READ_INT(pGlyphs + i);
				i += 4;
				std::cout << "  Unicode " << nPathLength;
				nPathLength = READ_INT(pGlyphs + i);
				i += 4;
				std::cout << " width " << (double)nPathLength / 10000.0;
				if (nCharX)
					std::cout << " charX " << (double)nCharX / 10000.0;
				std::cout << std::endl;
			}
		}

		DestroyTextInfo(pGrFile);
	}

	// INTERACTIVE FORMS
	if (true)
	{
		ReadInteractiveFormsFonts(pGrFile, 1);
		ReadInteractiveFormsFonts(pGrFile, 2);
		std::cout << std::endl;

		BYTE* pWidgets = GetInteractiveFormsInfo(pGrFile);
		nLength = READ_INT(pWidgets);
		int i = 4;
		nLength -= 4;

		while (i < nLength)
		{
			std::cout << "[" << std::endl;
			ReadInteractiveForms(pWidgets, i);
			std::cout << "]" << std::endl;
		}

		if (pWidgets)
			free(pWidgets);

		BYTE* pWidgetsAP = GetInteractiveFormsAP(pGrFile, nWidth, nHeight, 0xFFFFFF, nTestPage, -1, -1, -1);
		nLength = READ_INT(pWidgetsAP);
		i = 4;
		nLength -= 4;

		while (i < nLength)
		{
			ReadAnnotAP(pWidgetsAP, i);
		}

		if (pWidgetsAP)
			free(pWidgetsAP);

		int bBase64 = 1;
		BYTE* pWidgetsMK = GetButtonIcons(pGrFile, 0xFFFFFF, nTestPage, bBase64, -1, -1);
		nLength = READ_INT(pWidgetsMK);
		i = 4;
		nLength -= 4;

		while (i < nLength)
		{
			int nAP = READ_INT(pWidgetsMK + i);
			i += 4;
			std::cout << "AP " << nAP << ", ";
			int nMKLength = READ_INT(pWidgetsMK + i);
			i += 4;
			for (int j = 0; j < nMKLength; ++j)
			{
				int nPathLength = READ_INT(pWidgetsMK + i);
				i += 4;
				std::string sMKName = std::string((char*)(pWidgetsMK + i), nPathLength);
				i += nPathLength;
				std::cout << "MK " << sMKName << ", ";

				nPathLength = READ_INT(pWidgetsMK + i);
				i += 4;
				std::cout << "# " << nPathLength << ", ";

				nPathLength = READ_BYTE(pWidgetsMK + i);
				i += 1;
				if (!nPathLength)
					continue;

				int nWidgetWidth = READ_INT(pWidgetsMK + i);
				i += 4;
				std::cout << "W " << nWidgetWidth << ", ";
				int nWidgetHeight = READ_INT(pWidgetsMK + i);
				i += 4;
				std::cout << "H " << nWidgetHeight << ", ";

				if (bBase64)
				{
					nPathLength = READ_INT(pWidgetsMK + i);
					i += 4;
					BYTE* pBase64 = pWidgetsMK + i;
					i += nPathLength;

					int nLenDst = NSBase64::Base64DecodeGetRequiredLength(nPathLength);
					BYTE* pDataDst = new BYTE[nLenDst];

					if (NSBase64::Base64Decode((const char*)pBase64, nPathLength, pDataDst, &nLenDst))
					{
						NSFile::CFileBinary oFile;
						if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/res_" + std::to_wstring(nAP) + L"_MK_" + UTF8_TO_U(sMKName) + L".png"))
						{
							oFile.WriteFile(pDataDst, nLenDst);
							oFile.CloseFile();
						}
					}
					RELEASEARRAYOBJECTS(pDataDst);
				}
				else
				{
					unsigned long long npBgraData1 = READ_INT(pWidgetsMK + i);
					i += 4;
					unsigned long long npBgraData2 = READ_INT(pWidgetsMK + i);
					i += 4;

					BYTE* res = (BYTE*)(npBgraData2 << 32 | npBgraData1);
					CBgraFrame oFrame;
					oFrame.put_Data(res);
					oFrame.put_Width(nWidgetWidth);
					oFrame.put_Height(nWidgetHeight);
					oFrame.put_Stride(4 * nWidgetWidth);
					oFrame.put_IsRGBA(true);
					oFrame.SaveFile(NSFile::GetProcessDirectory() + L"/res_" + std::to_wstring(nAP) + L"_MK_" + UTF8_TO_U(sMKName) + L".png", _CXIMAGE_FORMAT_PNG);
					oFrame.ClearNoAttack();
					RELEASEARRAYOBJECTS(res);
				}
			}
			std::cout << std::endl;
		}

		if (pWidgetsMK)
			free(pWidgetsMK);
	}

	// ANNOTS
	if (true)
	{
		BYTE* pAnnots = GetAnnotationsInfo(pGrFile, -1);
		nLength = READ_INT(pAnnots);
		int i = 4;
		nLength -= 4;

		std::cout << std::endl;

		while (i < nLength)
		{
			int nAnnotLength = READ_INT(pAnnots + i);
			i += 4;
			for (int i2 = 0; i2 < nAnnotLength; ++i2)
			{
				std::cout << "[" << std::endl;
				int nPathLength = READ_BYTE(pAnnots + i);
				i += 1;
				std::string sType = arrAnnots[nPathLength];
				std::cout << "Type " << sType << ", ";

				ReadAnnot(pAnnots, i);

				// Markup

				DWORD nFlags = 0;
				if ((nPathLength < 18 && nPathLength != 1 && nPathLength != 15) || nPathLength == 25)
				{
					nFlags = READ_INT(pAnnots + i);
					i += 4;

					if (nFlags & (1 << 0))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Popup " << nPathLength << ", ";
					}
					if (nFlags & (1 << 1))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "User " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 2))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "CA " << (double)nPathLength / 100.0 << ", ";
					}
					if (nFlags & (1 << 3))
					{
						std::cout << "RC {";

						int nFontLength = READ_INT(pAnnots + i);
						i += 4;
						for (int j = 0; j < nFontLength; ++j)
						{
							std::cout << std::endl << "span" << j << " { ";

							nPathLength = READ_BYTE(pAnnots + i);
							i += 1;
							std::string arrTextAlign[] = {"Left", "Center", "Right", "Justify"};
							std::cout << "text-align: " << arrTextAlign[nPathLength];

							std::cout << "; font-style:";
							int nFontFlag = READ_INT(pAnnots + i);
							i += 4;
							if (nFontFlag & (1 << 0))
								std::cout << "Bold ";
							if (nFontFlag & (1 << 1))
								std::cout << "Italic ";
							if (nFontFlag & (1 << 3))
								std::cout << "Strike ";
							if (nFontFlag & (1 << 4))
								std::cout << "Underline ";
							if (nFontFlag & (1 << 5))
							{
								nPathLength = READ_INT(pAnnots + i);
								i += 4;
								std::cout << "; vertical-align:" << (double)nPathLength / 100.0;
							}
							if (nFontFlag & (1 << 6))
							{
								nPathLength = READ_INT(pAnnots + i);
								i += 4;
								std::cout << "; font-actual:" << std::string((char*)(pAnnots + i), nPathLength) << "; ";
								i += nPathLength;
							}
							if (nFontFlag & (1 << 7))
								std::cout << "; dir:rtl; ";

							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << "; font-size:" << (double)nPathLength / 100.0;

							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << "; font-color:" << (double)nPathLength / 10000.0 << " ";
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 10000.0 << " ";
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 10000.0 << "; ";

							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::string sFontName((char*)(pAnnots + i), nPathLength);
							std::cout << "font-family:" << sFontName << "; ";
							i += nPathLength;

							BYTE* pFont = GetFontBinary(pGrFile, (char*)sFontName.c_str());
							if (pFont)
							{
								std::cout << "FIND; ";
								free(pFont);
							}
							else
								std::cout << "NO; ";

							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::string sText = std::string((char*)(pAnnots + i), nPathLength);
							NSStringUtils::string_replaceA(sText, "\r", "\n");
							std::cout << "text:" << sText << " ";
							i += nPathLength;

							std::cout << "} ";
						}
						std::cout << "}, " << std::endl;
					}
					if (nFlags & (1 << 4))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "CreationDate " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 5))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Ref to " << nPathLength << ", ";
					}
					if (nFlags & (1 << 6))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::cout << "Reason " << nPathLength << ", ";
					}
					if (nFlags & (1 << 7))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Subj " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
				}

				if (sType == "Text")
				{
					if (nFlags & (1 << 15))
						std::cout << "Open true, ";
					else
						std::cout << "Open false, ";
					if (nFlags & (1 << 16))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrIcon[] = {"Check", "Checkmark", "Circle", "Comment", "Cross", "CrossHairs", "Help", "Insert", "Key", "NewParagraph", "Note", "Paragraph", "RightArrow", "RightPointer", "Star", "UpArrow", "UpLeftArrow"};
						std::cout << "Icon " << arrIcon[nPathLength] << ", ";
					}
					if (nFlags & (1 << 17))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrStateModel[] = {"Marked", "Review"};
						std::cout << "State model " << arrStateModel[nPathLength] << ", ";
					}
					if (nFlags & (1 << 18))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrState[] = {"Marked", "Unmarked", "Accepted", "Rejected", "Cancelled", "Completed", "None"};
						std::cout << "State " << arrState[nPathLength] << ", ";
					}
				}
				else if (sType == "Line")
				{
					std::cout << "L";
					for (int j = 0; j < 4; ++j)
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << " " << (double)nPathLength / 100.0;
					}
					std::cout << ", ";
					if (nFlags & (1 << 15))
					{
						std::cout << "LE ";
						for (int j = 0; j < 2; ++j)
						{
							nPathLength = READ_BYTE(pAnnots + i);
							i += 1;
							std::string arrLE[] = {"Square", "Circle", "Diamond", "OpenArrow", "ClosedArrow", "None", "Butt", "ROpenArrow", "RClosedArrow", "Slash"};
							std::cout << arrLE[nPathLength] << " ";
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 16))
					{
						int nICLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "IC";

						for (int j = 0; j < nICLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 10000.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 17))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "LL " << (double)nPathLength / 100.0 << ", ";
					}
					if (nFlags & (1 << 18))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "LLE " << (double)nPathLength / 100.0 << ", ";
					}
					if (nFlags & (1 << 19))
						std::cout << "Cap true, ";
					else
						std::cout << "Cap false, ";
					if (nFlags & (1 << 20))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrIT[] = {"LineDimension", "LineArrow"};
						std::cout << "IT " << arrIT[nPathLength] << ", ";
					}
					if (nFlags & (1 << 21))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "LLO " << (double)nPathLength / 100.0 << ", ";
					}
					if (nFlags & (1 << 22))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrCP[] = {"Inline", "Top"};
						std::cout << "CP " << arrCP[nPathLength] << ", ";
					}
					if (nFlags & (1 << 23))
					{
						std::cout << "CO ";
						for (int j = 0; j < 2; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 100.0 << " ";
						}
						std::cout << ", ";
					}
				}
				else if (sType == "Ink")
				{
					int nInkLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "InkList ";

					for (int j = 0; j < nInkLength; ++j)
					{
						int nInkJLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "[ ";

						for (int k = 0; k < nInkJLength; ++k)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 100.0 << " ";
						}
						std::cout << "] ";
					}
					std::cout << ", ";
				}
				else if (sType == "Highlight" ||
						 sType == "Underline" ||
						 sType == "Squiggly"  ||
						 sType == "StrikeOut")
				{
					std::cout << "QuadPoints";
					int nQuadPointsLength = READ_INT(pAnnots + i);
					i += 4;

					for (int j = 0; j < nQuadPointsLength; ++j)
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << " " << (double)nPathLength / 100.0;
					}
					std::cout << ", ";
				}
				else if (sType == "Square" ||
						 sType == "Circle")
				{
					if (nFlags & (1 << 15))
					{
						std::cout << "RD";
						for (int j = 0; j < 4; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 100.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 16))
					{
						int nICLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "IC ";

						for (int j = 0; j < nICLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 10000.0 << " ";
						}
						std::cout << ", ";
					}
				}
				else if (sType == "Polygon" ||
						 sType == "PolyLine")
				{
					int nVerticesLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Vertices";

					for (int j = 0; j < nVerticesLength; ++j)
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << " " << (double)nPathLength / 100.0;
					}
					std::cout << ", ";

					if (nFlags & (1 << 15))
					{
						std::cout << "LE";
						for (int j = 0; j < 2; ++j)
						{
							nPathLength = READ_BYTE(pAnnots + i);
							i += 1;
							std::string arrLE[] = {"Square", "Circle", "Diamond", "OpenArrow", "ClosedArrow", "None", "Butt", "ROpenArrow", "RClosedArrow", "Slash"};
							std::cout << " " << arrLE[nPathLength];
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 16))
					{
						int nICLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "IC";

						for (int j = 0; j < nICLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 10000.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 20))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrIT[] = {"PolygonCloud", "PolyLineDimension", "PolygonDimension"};
						std::cout << "IT " << arrIT[nPathLength] << ", ";
					}
				}
				else if (sType == "Popup")
				{
					nFlags = READ_INT(pAnnots + i);
					i += 4;
					if (nFlags & (1 << 0))
						std::cout << "Open true, ";
					else
						std::cout << "Open false, ";
					if (nFlags & (1 << 1))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Popup parent " << nPathLength << ", ";
					}
				}
				else if (sType == "FreeText")
				{
					std::string arrQ[] = {"left-justified", "centered", "right-justified"};
					nPathLength = READ_BYTE(pAnnots + i);
					i += 1;
					std::cout << "Q " << arrQ[nPathLength] << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Rotate " << nPathLength << ", ";

					if (nFlags & (1 << 15))
					{
						std::cout << "RD";
						for (int j = 0; j < 4; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 100.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 16))
					{
						int nCLLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "CL";

						for (int j = 0; j < nCLLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 100.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 17))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "DS " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 18))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrLE[] = {"Square", "Circle", "Diamond", "OpenArrow", "ClosedArrow", "None", "Butt", "ROpenArrow", "RClosedArrow", "Slash"};
						std::cout << "LE " << arrLE[nPathLength] << ", ";
					}
					if (nFlags & (1 << 20))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrIT[] = {"FreeText", "FreeTextCallout", "FreeTextTypeWriter"};
						std::cout << "IT " << arrIT[nPathLength] << ", ";
					}
					if (nFlags & (1 << 21))
					{
						int nCLLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "C from DA:";

						for (int j = 0; j < nCLLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 10000.0;
						}
						std::cout << ", ";
					}
				}
				else if (sType == "Caret")
				{
					if (nFlags & (1 << 15))
					{
						std::cout << "RD";
						for (int j = 0; j < 4; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 100.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 16))
					{
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::string arrSy[] = {"None", "P", "S"};
						std::cout << "Sy " << arrSy[nPathLength] << ", ";
					}
				}
				else if (sType == "FileAttachment")
				{
					if (nFlags & (1 << 15))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Name " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 16))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "FS " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 17))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "F " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 18))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "UF " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 19))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "DOS " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 20))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Mac " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 21))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Unix " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 22))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "ID " << std::string((char*)(pAnnots + i), nPathLength);
						i += nPathLength;

						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << " " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 23))
						std::cout << "V true, ";
					else
						std::cout << "V false, ";
					if (nFlags & (1 << 24))
					{
						int nFlag = READ_INT(pAnnots + i);
						i += 4;

						if (nFlag & (1 << 0))
							ReadFileAttachment(pAnnots, i, 0);
						if (nFlag & (1 << 1))
							ReadFileAttachment(pAnnots, i, 1);
						if (nFlag & (1 << 2))
							ReadFileAttachment(pAnnots, i, 2);
						if (nFlag & (1 << 3))
							ReadFileAttachment(pAnnots, i, 3);
						if (nFlag & (1 << 4))
							ReadFileAttachment(pAnnots, i, 4);
					}
					if (nFlags & (1 << 26))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "Desc " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
				}
				else if (sType == "Stamp")
				{
					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Icon " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
					i += nPathLength;

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Rotate " << nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "X1 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Y1 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "X2 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Y2 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "X3 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Y3 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "X4 " << (double)nPathLength / 10000.0 << ", ";

					nPathLength = READ_INT(pAnnots + i);
					i += 4;
					std::cout << "Y4 " << (double)nPathLength / 10000.0 << ", ";
				}
				else if (sType == "Redact")
				{
					if (nFlags & (1 << 15))
					{
						std::cout << "QuadPoints";
						int nQuadPointsLength = READ_INT(pAnnots + i);
						i += 4;

						for (int j = 0; j < nQuadPointsLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 100.0;
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 16))
					{
						int nICLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "IC ";

						for (int j = 0; j < nICLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 10000.0 << " ";
						}
						std::cout << ", ";
					}
					if (nFlags & (1 << 17))
					{
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "OverlayText " << std::string((char*)(pAnnots + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 18))
						std::cout << "Repeat true, ";
					else
						std::cout << "Repeat false, ";
					if (nFlags & (1 << 19))
					{
						std::string arrQ[] = {"left-justified", "centered", "right-justified"};
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::cout << "Q " << arrQ[nPathLength] << ", ";
					}
					if (nFlags & (1 << 20))
					{
						int nICLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << "DA color ";

						for (int j = 0; j < nICLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << (double)nPathLength / 10000.0 << " ";
						}
						std::cout << ", size ";
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << (double)nPathLength / 100.0 << ", font ";
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << std::string((char*)(pAnnots + i), nPathLength) << " actual ";
						i += nPathLength;
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << std::string((char*)(pAnnots + i), nPathLength) << ", style ";
						i += nPathLength;
						nPathLength = READ_INT(pAnnots + i);
						i += 4;
						std::cout << nPathLength << ", ";
					}
				}
				else if (sType == "Link")
				{
					nFlags = READ_INT(pAnnots + i);
					i += 4;
					if (nFlags & (1 << 0))
					{
						std::cout << std::endl << "A ";
						ReadAction(pAnnots, i);
						std::cout << std::endl;
					}
					if (nFlags & (1 << 1))
					{
						std::cout << std::endl << "PA ";
						ReadAction(pAnnots, i);
						std::cout << std::endl;
					}
					if (nFlags & (1 << 2))
					{
						std::string arrHighlighting[] = {"none", "invert", "push", "outline"};
						nPathLength = READ_BYTE(pAnnots + i);
						i += 1;
						std::cout << "Highlight " << arrHighlighting[nPathLength] << ", ";
					}
					if (nFlags & (1 << 3))
					{
						std::cout << "QuadPoints";
						int nQuadPointsLength = READ_INT(pAnnots + i);
						i += 4;

						for (int j = 0; j < nQuadPointsLength; ++j)
						{
							nPathLength = READ_INT(pAnnots + i);
							i += 4;
							std::cout << " " << (double)nPathLength / 100.0;
						}
						std::cout << ", ";
					}
				}

				std::cout << std::endl << "]" << std::endl;
			}
		}

		if (pAnnots)
			free(pAnnots);

		BYTE* pAnnotAP = GetAnnotationsAP(pGrFile, nWidth, nHeight, 0xFFFFFF, nTestPage, -1, -1);
		nLength = READ_INT(pAnnotAP);
		i = 4;
		nLength -= 4;

		while (i < nLength)
		{
			ReadAnnotAP(pAnnotAP, i);
		}

		if (pAnnotAP)
			free(pAnnotAP);
	}

	// SCAN PAGE
	if (false)
	{
		BYTE* pScan = ScanPage(pGrFile, nTestPage, 1);
		if (pScan)
			free(pScan);
	}

	Close(pGrFile);

	return 0;
}
