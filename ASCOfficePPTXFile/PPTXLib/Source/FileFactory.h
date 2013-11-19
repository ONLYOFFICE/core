#pragma once
#ifndef PPTX_FILE_FACTORY_INCLUDE_H_
#define PPTX_FILE_FACTORY_INCLUDE_H_

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "DocxFormat/File.h"

//namespace OOX {class File;}
namespace OOX {namespace Rels {class RelationShip;}}
namespace PPTX {class FileMap;}
namespace PPTX
{
	class FileFactory
	{
	public:
		static const boost::shared_ptr<OOX::File> CreateFilePPTX(const boost::filesystem::wpath& path, const OOX::Rels::RelationShip& relation, FileMap& map);
	};
} // namespace PPTX

#endif // PPTX_FILE_FACTORY_INCLUDE_H_