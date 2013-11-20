#pragma once
#ifndef OOX_RELS_FILE_INCLUDE_H_
#define OOX_RELS_FILE_INCLUDE_H_

#include "property.h"
#include "RelationTable.h"
#include "./../FileType.h"
#include "./../RId.h"
#include "./../External/External.h"
#include <boost/shared_ptr.hpp>


namespace OOX
{
	namespace Rels
	{
		class File
		{
		public:
			File();
			File(const boost::filesystem::wpath& filename);
			~File();

		public:
			void read(const boost::filesystem::wpath& filename);
			void write(const boost::filesystem::wpath& filename) const;
			const bool isValid() const;

		public:
			void registration(const RId& rId, const FileType& type, const boost::filesystem::wpath& filename);
			void registration(const RId& rId, const boost::shared_ptr<External> external);

		private:
			const boost::filesystem::wpath createFileName(const boost::filesystem::wpath& filename) const;

		public:
			property<RelationTable>	Relations;
		};
	} // namespace Rels
} // namespace OOX

#endif // OOX_RELS_FILE_INCLUDE_H_