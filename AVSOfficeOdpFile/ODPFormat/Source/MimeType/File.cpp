#include "File.h"
#include <fstream>
#include <stdexcept>
#include "Log.h"
#include "Exception/log_runtime_error.h"


namespace Odp
{
	namespace MimeType
	{
		static const std::string s_content = "application/vnd.oasis.opendocument.presentation";
		static const boost::filesystem::wpath s_filename = L"mimetype";

		File::File()
		{
		}

		File::~File()
		{
		}

		const bool File::exist(const boost::filesystem::wpath& path)
		{
			return boost::filesystem::exists(path/s_filename);
		}

		void File::read(const boost::filesystem::wpath& path)
		{
		}

		void File::write(const boost::filesystem::wpath& path) const
		{
			std::ofstream file((path/s_filename).string().c_str());
			if (!file.bad())
			{
				file << s_content;
				file.close();
			}
			else
				throw log_runtime_error("can't create mimetype");
		}

		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			std::ifstream file((path/s_filename).string().c_str());
			if (file.bad())
			{
				Log::warning(L"bad open mimetype file " + (path/s_filename).string());
				return false;
			}

			std::string content;
			file >> content;
			file.close();

			if (s_content == content)
				return true;

			Log::warning(L"error content mimetype " + (path/s_filename).string());
			return false;
		}

	} // namespace MimeType
} // namespace Odp