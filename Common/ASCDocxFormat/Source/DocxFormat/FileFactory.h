#pragma once
#ifndef OOX_FILE_FACTORY_INCLUDE_H_
#define OOX_FILE_FACTORY_INCLUDE_H_

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>


namespace OOX
{
	class File;
	namespace Rels {class RelationShip;}

	const boost::shared_ptr<File> CreateFile(const boost::filesystem::wpath& path, const Rels::RelationShip& relation);
} // namespace OOX

#endif // OOX_FILE_FACTORY_INCLUDE_H_