#pragma once

#include "FileInformationBlock.h"
#include "VirtualStreamReader.h"
#include "StyleSheetInformation.h"
#include "StyleSheetDescription.h"
#include "IVisitable.h"

namespace DocFileFormat
{
	class StyleSheet : public IVisitable
	{
	public:  
		virtual ~StyleSheet();
		/// Parses the streams to retrieve a StyleSheet.
		StyleSheet(FileInformationBlock* fib, POLE::Stream* tableStream, POLE::Stream* dataStream);

	public:  

		/// The StyleSheetInformation of the stylesheet.
		StyleSheetInformation*			stshi;
		/// The list contains all styles.
		vector<StyleSheetDescription*>*	Styles;
	};
}