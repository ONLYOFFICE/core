#include "webp.h"
#include "../../common/File.h"

#include "../../Common/3dParty/webp/libwebp/src/webp/decode.h"

#include <limits>
#include <memory>

namespace NSWebP {
	constexpr int MAX_INT_SIZE = std::numeric_limits<int32_t>::max();
	constexpr int BUFFER_SIZE = 4096;

	class Stream
	{
	public:
		Stream() {}
		Stream(FILE *_file) : file(_file) {}
		Stream(BYTE *_buffer, const DWORD& _size) : buffer(_buffer), size(_size) {}

		size_t Read(BYTE *data) {
			if (file)
				return fread(data, 1, BUFFER_SIZE, file);
			else if (buffer && size > 0 && cur < size) {
				DWORD tmp_size = size < BUFFER_SIZE ? size : BUFFER_SIZE;
				memcpy(data, buffer + cur, tmp_size);
				cur = tmp_size;
				return tmp_size;
			} else
				return 0;
		}

	private:
		FILE* file{nullptr};
		BYTE* buffer{nullptr};
		DWORD size{0};
		DWORD cur{0};
	};

	bool CWebPFile::isWebP(const std::wstring& fileName)
	{
		NSFile::CFileBinary file;
		if (!file.OpenFile(fileName))
			return false;

		BYTE data[4];
		fseek(file.GetFileNative(), 8, SEEK_CUR);
		int bytes_read = fread(&data, 1, 4, file.GetFileNative());
		if (bytes_read < 4)
		{
			file.CloseFile();
			return false;
		}

		if (strncmp((const char*) data, "WEBP", 4) != 0)
		{
			file.CloseFile();
			return false;
		}

		file.CloseFile();
		return true;
	}

	bool CWebPFile::isWebP(BYTE* buffer, const DWORD& size)
	{
		if (size < 12)
			return false;

		if (strncmp((const char*) buffer + 8, "WEBP", 4) != 0)
			return false;

		return true;
	}

	bool CWebPFile::Open(CBgraFrame* frame, const std::wstring& fileName, bool isRGBA)
	{
		NSFile::CFileBinary file;
		if (!file.OpenFile(fileName))
			return false;

		Stream* stream = new Stream(file.GetFileNative());
		auto status = Decode(stream, frame, isRGBA);
		delete stream;
		file.CloseFile();
		return status;
	}

	bool CWebPFile::Open(CBgraFrame* frame, BYTE* buffer, const DWORD& size, bool isRGBA)
	{
		Stream* stream = new Stream(buffer, size);
		auto status = Decode(stream, frame, isRGBA);
		delete stream;
		return status;
	}

	bool CWebPFile::Decode(Stream* stream, CBgraFrame* frame, bool isRGBA)
	{
		WebPDecoderConfig cfg;
		if (!WebPInitDecoderConfig(&cfg))
			return false;

		std::vector<BYTE> data;
		while(true)
		{
			size_t last_size = data.size();
			data.resize(last_size + BUFFER_SIZE);
			auto bytes_read = stream->Read(data.data() + last_size);

			if (bytes_read <= 0)
			{
				WebPFreeDecBuffer(&cfg.output);
				return false;
			}

			data.resize(last_size + bytes_read);
			auto status = WebPGetFeatures(data.data(), data.size(), &cfg.input);

			if (status == VP8_STATUS_OK)
				break;

			if (status == VP8_STATUS_NOT_ENOUGH_DATA)
				continue;

			WebPFreeDecBuffer(&cfg.output);
			return false;
		}

		const int& width	 = cfg.input.width;
		const int& height	 = cfg.input.height;
		const int& has_alpha = cfg.input.has_alpha;

		if (width > MAX_INT_SIZE / 8 || height > MAX_INT_SIZE / 8)
		{
			WebPFreeDecBuffer(&cfg.output);
			return false;
		}

		cfg.output.width				= width;
		cfg.output.height				= height;
		cfg.output.is_external_memory	= 1;
		std::vector<BYTE> tmp_rgba_data(width * height * 4);

		if (has_alpha)
		{
			if (isRGBA)
				cfg.output.colorspace = MODE_RGBA;
			else
				cfg.output.colorspace = MODE_BGRA;
		}
		else
		{
			if (isRGBA)
				cfg.output.colorspace = MODE_RGB;
			else
				cfg.output.colorspace = MODE_BGR;
		}

		cfg.output.u.RGBA.rgba	 = tmp_rgba_data.data();
		cfg.output.u.RGBA.stride = width * 4;
		cfg.output.u.RGBA.size	 = tmp_rgba_data.size();

		bool success = true;
		std::unique_ptr<WebPIDecoder, decltype(&WebPIDelete)> decoder(WebPIDecode(nullptr, 0, &cfg), WebPIDelete);
		while(true)
		{
			auto status = WebPIAppend(decoder.get(), data.data(), data.size());
			if (status == VP8_STATUS_OK)
				break;

			if (status != VP8_STATUS_SUSPENDED)
			{
				success = false;
				break;
			}

			data.resize(BUFFER_SIZE);
			auto bytes_read = stream->Read(data.data());
			if (bytes_read <= 0)
			{
				success = false;
				break;
			}
			data.resize(bytes_read);
		}

		BYTE* frame_data = new BYTE[width * height * 4];
		frame->put_Data(frame_data);
		frame->put_Width(width);
		frame->put_Height(height);
		frame->put_Stride(4 * width);

		for (size_t i = 0; i < height; ++i)
		{
			const BYTE* row = tmp_rgba_data.data() + width * 4 * i;
			for (size_t j = 0; j < width; ++j)
			{
				frame_data[(i * width + j) * 4 + 0] = row[0];
				frame_data[(i * width + j) * 4 + 1] = row[1];
				frame_data[(i * width + j) * 4 + 2] = row[2];
				frame_data[(i * width + j) * 4 + 3] = has_alpha ? row[3] : 255;
				row += has_alpha ? 4 : 3;
			}
		}

		WebPFreeDecBuffer(&cfg.output);
		return success;
	}
}
