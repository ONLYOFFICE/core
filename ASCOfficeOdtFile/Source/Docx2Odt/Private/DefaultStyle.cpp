
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "DefaultStyle.h"


namespace Docx2Odt
{
	namespace Private
	{
		DefaultStyle::DefaultStyle(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}
	} // namespace Private
} // namespace Docx2Odt