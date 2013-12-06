#pragma once
#ifndef OOX_FILE_FACTORY_INCLUDE_H_
#define OOX_FILE_FACTORY_INCLUDE_H_

#include "../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace OOX
{
	class File;
	namespace Rels {class RelationShip;}

	const NSCommon::smart_ptr<File> CreateFile(const OOX::CPath& path, const Rels::RelationShip& relation);
} // namespace OOX

#endif // OOX_FILE_FACTORY_INCLUDE_H_