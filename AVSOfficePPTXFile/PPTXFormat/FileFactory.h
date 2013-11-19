#pragma once
#ifndef PPTX_FILE_FACTORY_INCLUDE_H_
#define PPTX_FILE_FACTORY_INCLUDE_H_

#include "DocxFormat/File.h"

//namespace OOX {class File;}
namespace OOX {namespace Rels {class RelationShip;}}
namespace PPTX {class FileMap;}
namespace PPTX
{
	class FileFactory
	{
	public:
		static const smart_ptr<OOX::File> CreateFilePPTX(const OOX::CPath& path, const OOX::Rels::RelationShip& relation, FileMap& map);
		static const smart_ptr<OOX::File> CreateFilePPTX_OnlyMedia(const OOX::CPath& path, const OOX::Rels::RelationShip& relation);
	};
} // namespace PPTX

#endif // PPTX_FILE_FACTORY_INCLUDE_H_