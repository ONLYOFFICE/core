#pragma once
#include "OOXReader.h"
#include "RtfDocument.h"
#include "OOXReaderBasic.h"

	class OOXHeaderReader
	{
	public: OOXHeaderReader()
			{
			}
	public: bool Parse(ReaderParameter oParam, CString sRID, TextItemContainerPtr& oOutput, bool bHeader );
	};
