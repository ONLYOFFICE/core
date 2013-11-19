
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

	
	External::External(const boost::filesystem::wpath& uri)
	{
		read(uri);
	}


	External::~External()
	{
	}


	void External::read(const boost::filesystem::wpath& uri)
	{
		m_uri = uri;
	}


	void External::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
	}


	const boost::filesystem::wpath External::Uri() const
	{
		return m_uri;
	}

} // namespace OOX