#pragma once
#ifndef PPTX_FILE_FACTORY_INCLUDE_H_
#define PPTX_FILE_FACTORY_INCLUDE_H_

#include "DocxFormat/File.h"

//namespace PPTX {class File;}
namespace PPTX {namespace Rels {class RelationShip;}}
namespace PPTX {class FileMap;}
namespace PPTX
{
	class FileFactory
	{
	public:
		static const smart_ptr<PPTX::File> CreateFilePPTX(const OOX::CPath& path, const PPTX::Rels::RelationShip& relation, FileMap& map);
		static const smart_ptr<PPTX::File> CreateFilePPTX_OnlyMedia(const OOX::CPath& path, const PPTX::Rels::RelationShip& relation);
	};
} // namespace PPTX

#endif // PPTX_FILE_FACTORY_INCLUDE_H_