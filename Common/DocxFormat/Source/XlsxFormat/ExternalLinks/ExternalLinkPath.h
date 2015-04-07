#pragma once
#ifndef OOX_EXTERNALLINKPATH_INCLUDE_H_
#define OOX_EXTERNALLINKPATH_INCLUDE_H_

#include "../../DocxFormat/External/External.h"


namespace OOX
{
	namespace Spreadsheet
	{
		class ExternalLinkPath : public External
		{
		public:
			ExternalLinkPath()
			{
			}
			ExternalLinkPath(const CPath& uri)
			{
				read(uri);
			}
			~ExternalLinkPath()
			{
			}

		public:
			virtual const FileType type() const
			{
				return FileTypes::ExternalLinkPath;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
		};
	}// namespace Spreadsheet
} // namespace OOX

#endif // OOX_EXTERNALLINKPATH_INCLUDE_H_