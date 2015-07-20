
#include "METADATA.h"
#include <Logic/Biff_unions/MDTINFO.h>
#include <Logic/Biff_unions/MDXSTR.h>
#include <Logic/Biff_unions/MDBLOCK.h>
#include <Logic/Biff_unions/MDXTUPLESET.h>
#include <Logic/Biff_records/MDXProp.h>
#include <Logic/Biff_records/MDXKPI.h>

namespace XLS
{;


METADATA::METADATA()
{
}


METADATA::~METADATA()
{
}


class Parenthesis_METADATA_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_METADATA_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_METADATA_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<MDXTUPLESET>())
		{
			if(!proc.optional<MDXProp>())
			{
				if(!proc.optional<MDXKPI>())
				{
					return false;
				}
			}
		}
		return true;
	};
};

BaseObjectPtr METADATA::clone()
{
	return BaseObjectPtr(new METADATA(*this));
}


// METADATA = *MDTINFO *MDXSTR *(MDXTUPLESET / MDXProp / MDXKPI) *MDBLOCK
const bool METADATA::loadContent(BinProcessor& proc)
{
	if(
		proc.repeated<MDTINFO>(0, 0) ||
		proc.repeated<MDXSTR>(0, 0) ||
		proc.repeated<Parenthesis_METADATA_1>(0, 0) ||
		proc.repeated<MDBLOCK>(0, 0))
	{
		return true;
	}
	return false;
}

} // namespace XLS

