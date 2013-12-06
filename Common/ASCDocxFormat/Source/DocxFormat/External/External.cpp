
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "External.h"
#include "../FileTypes.h"

namespace OOX
{
	External::External()
	{
	}
	
	External::External(const OOX::CPath& uri)
	{
		read(uri);
	}

	External::~External()
	{
	}

	void External::read(const OOX::CPath& uri)
	{
		m_uri = uri;
	}

	void External::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const OOX::CPath External::Uri() const
	{
		return m_uri;
	}

} // namespace OOX