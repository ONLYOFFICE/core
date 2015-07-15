#pragma once

#include <string>

#include "mediaitems.h"

namespace oox {
namespace utils {
namespace media {

	bool is_internal(const std::wstring & uri, const std::wstring & packetRoot);
	std::wstring get_rel_type(mediaitems::Type type);

}
}
}
