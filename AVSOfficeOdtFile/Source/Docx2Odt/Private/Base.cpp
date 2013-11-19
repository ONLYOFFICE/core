
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Base.h"


namespace Docx2Odt
{
	namespace Private
	{
		Base::Base(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: m_odtFolder(odtFolder),
				m_docxFileContainer(docxFileContainer)
		{
		}
	} // namespace Private
} // namespace Docx2Odt