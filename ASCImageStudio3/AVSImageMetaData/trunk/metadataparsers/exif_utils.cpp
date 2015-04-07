#include "stdafx.h"
#include "exif_utils.h"
#include <string>

namespace AVS
{
	namespace Utils
	{
		int match(const std::string& regEntry, const std::string& key)
		{
			// Todo: make the comparisons case insensitive

			// Handle exact match (this is only necessary because of the different
			// return value - the following algorithm also finds exact matches)
			if (regEntry == key) {
				return static_cast<int>(key.size()) + 2;
			}
			std::string uKey = key;
			std::string uReg = regEntry;

			int count = 0;                          // number of matching characters
			std::string::size_type ei = 0;          // index in the registry entry
			std::string::size_type ki = 0;          // index in the key

			while (ei != std::string::npos) {

				std::string::size_type pos = uReg.find('*', ei);
				if (pos != ei) {
					std::string ss = pos == std::string::npos ?
						uReg.substr(ei) : uReg.substr(ei, pos - ei);

					if (ki == std::string::npos) {
						return 0;
					}

					bool found = false;
					// Find the substr ss in the key starting from index ki.
					// Take care of the special cases
					//   + where the substr must match the key from beg to end,
					//   + from beg,
					//   + to end
					//   + and where it can be anywhere in the key.
					// If found, ki is adjusted to the position in the key after ss.
					if (ei == 0 && pos == std::string::npos) { // ei == 0 => ki == 0
						if (0 == uKey.compare(ss)) {
							found = true;
							ki = std::string::npos;
						}
					}
					else if (ei == 0) { // ei == 0 => ki == 0
						if (0 == uKey.compare(0, ss.size(), ss)) {
							found = true;
							ki = ss.size();
						}
					}
					else if (pos == std::string::npos) {
						if (   ss.size() <= uKey.size()
							&& ki <= uKey.size() - ss.size()) {
								if (0 == uKey.compare(
									uKey.size() - ss.size(), ss.size(), ss)) {
										found = true;
										ki = std::string::npos;
								}
						}
					}
					else {
						std::string::size_type idx = uKey.find(ss, ki);
						if (idx != std::string::npos) {
							found = true;
							ki = idx + ss.size();
						}
					}

					if (found) {
						count += static_cast<int>(ss.size());
					}
					else {
						return 0;
					}
				} // if the substr is not empty

				ei = pos == std::string::npos ? std::string::npos : pos + 1;

			} // while ei doesn't point to the end of the registry entry

			return count + 1;

		} // MakerNoteFactory::match
	}
}