#pragma once
#ifndef OOX_XLSXFILE_FACTORY_INCLUDE_H_
#define OOX_XLSXFILE_FACTORY_INCLUDE_H_

#include "../Base/SmartPtr.h"
#include "../SystemUtility/SystemUtility.h"

namespace OOX
{
	class File;
	namespace Rels
	{
		class CRelationShip;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		NSCommon::smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const CPath& oPath, const OOX::Rels::CRelationShip& oRelation);
        NSCommon::smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath,  OOX::Rels::CRelationShip* pRelation);
	}
} // namespace OOX

#endif // OOX_XLSXFILE_FACTORY_INCLUDE_H_
