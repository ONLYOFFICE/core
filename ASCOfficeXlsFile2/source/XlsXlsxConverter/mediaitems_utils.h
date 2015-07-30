#pragma once

#include <string>

#include "external_items.h"

namespace oox {
namespace utils {
namespace media {

	bool is_internal(const std::wstring & uri);
	std::wstring get_rel_type(external_items::Type type);

}
}
}
