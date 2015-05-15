#ifndef _PDF_READER_CHARTYPES_H
#define _PDF_READER_CHARTYPES_H

namespace PdfReader
{
	// Unicode-символы.
	typedef unsigned int Unicode;

	// ƒл¤ элементов CID.
	typedef unsigned int CID;

	// ќбщий тип, рассчитан наодин из следюущих вариантов:
	// - 8-битные символы
	// - 16-битный CID
	// - Unicode
	typedef unsigned int CharCode;
}

#endif // _PDF_READER_CHARTYPES_H
