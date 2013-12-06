#pragma once
#ifndef OOX_RELS_FILE_INCLUDE_H_
#define OOX_RELS_FILE_INCLUDE_H_

#include "property.h"
#include "RelationTable.h"
#include "./../FileType.h"
#include "./../RId.h"
#include "./../External/External.h"

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace OOX
{
	namespace Rels
	{
		class File
		{
		public:
			File();
			File(const OOX::CPath& filename);
			~File();

		public:
			void read(const OOX::CPath& filename);
			void write(const OOX::CPath& filename) const;
			const bool isValid() const;

		public:
			void registration(const RId& rId, const FileType& type, const OOX::CPath& filename);
			void registration(const RId& rId, const NSCommon::smart_ptr<External> external);

		private:
			const OOX::CPath createFileName(const OOX::CPath& filename) const;

		public:
			NSCommon::nullable<RelationTable> Relations;
		};
	} // namespace Rels
} // namespace OOX

#endif // OOX_RELS_FILE_INCLUDE_H_