#include "heif.h"
#include "../../common/File.h"

#include <functional>

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define defer(code) Defer CONCAT(_defer_, __COUNTER__)([&](){code;})

class Defer {
	std::function<void()> func;
public:
	explicit Defer(std::function<void()> func) : func(func) {}
	~Defer() { func(); }
};

namespace NSHeif {
	NSUnicodeConverter::CUnicodeConverter CHeifFile::m_oConverter{};

	bool CHeifFile::isHeif(const std::wstring& fileName)
	{
		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););
		return !IsError(heif_context_read_from_file(ctx, m_oConverter.fromUnicode(fileName, "UTF-8").c_str(), nullptr));
	}

	bool CHeifFile::isHeif(BYTE* buffer, DWORD size)
	{
		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););
		return !IsError(heif_context_read_from_memory_without_copy(ctx, buffer, size, nullptr));
	}

	bool CHeifFile::Open(CBgraFrame *frame, const std::wstring& fileName, bool isRGBA)
	{
		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););
		if (IsError(heif_context_read_from_file(ctx, m_oConverter.fromUnicode(fileName, "UTF-8").c_str(), nullptr)))
			return false;
		return Decode(ctx, frame, isRGBA);
	}

	bool CHeifFile::Open(CBgraFrame *frame, BYTE* buffer, DWORD size, bool isRGBA)
	{
		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););
		if (IsError(heif_context_read_from_memory_without_copy(ctx, buffer, size, nullptr)))
			return false;
		return Decode(ctx, frame, isRGBA);
	}

	bool CHeifFile::Save(const BYTE* source, int width, int height, int sourceStride, const std::wstring& dstPath, bool isRGBA)
	{
		if (!source)
			return false;

		heif_image* img;
		defer(heif_image_release(img););

		if (IsError(heif_image_create(width, height, heif_colorspace_RGB, heif_chroma_interleaved_RGB, &img)))
			return false;

		if (IsError(heif_image_add_plane(img, heif_channel_interleaved, width, height, 24)))
			return false;

		int stride;
		BYTE* data = heif_image_get_plane(img, heif_channel_interleaved, &stride);

		if (!data || stride == 0)
			return false;

		for (size_t i = 0; i < height; ++i)
		{
			const BYTE* row = source + (height - i - 1) * (sourceStride < 0 ? -sourceStride : sourceStride);
			for (size_t j = 0; j < width; ++j)
			{
				data[(i * width + j) * 3 + (isRGBA ? 0 : 2)] = row[(width - j - 1) * 4 + 0];
				data[(i * width + j) * 3 + 1] = row[(width - j - 1) * 4 + 1];
				data[(i * width + j) * 3 + (isRGBA ? 2 : 0)] = row[(width - j - 1) * 4 + 2];
			}
		}

		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););

		heif_encoder* encoder;
		defer(heif_encoder_release(encoder););

		if (IsError(heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder)))
			return false;

		if (IsError(heif_context_encode_image(ctx, img, encoder, nullptr, nullptr)))
			return false;

		if (IsError(heif_context_write_to_file(ctx, m_oConverter.fromUnicode(dstPath, "UTF-8").c_str())))
			return false;

		return true;
	}

	inline bool CHeifFile::IsError(heif_error err)
	{
		return err.code != heif_error_Ok;
	}

	inline bool CHeifFile::Decode(heif_context* ctx, CBgraFrame* frame, bool isRGBA)
	{
		heif_image_handle* handle;
		defer(heif_image_handle_release(handle););

		if (IsError(heif_context_get_primary_image_handle(ctx, &handle)))
			return false;

		heif_image* img;
		defer(heif_image_release(img););

		if (IsError(heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_444, nullptr)))
			return false;

		int width = heif_image_get_primary_width(img);
		int height = heif_image_get_primary_height(img);

		int stride_R, stride_G, stride_B;
		const BYTE* source_R = heif_image_get_plane_readonly(img, heif_channel_R, &stride_R);
		const BYTE* source_G = heif_image_get_plane_readonly(img, heif_channel_G, &stride_G);
		const BYTE* source_B = heif_image_get_plane_readonly(img, heif_channel_B, &stride_B);

		if (stride_R == 0 || !source_R)
			return false;

		BYTE* data = new BYTE[4 * width * height];

		frame->put_Width(width);
		frame->put_Height(height);
		frame->put_Stride(4 * width);
		frame->put_Data(data);

		for (size_t i = 0; i < height; ++i)
		{
			const BYTE* row_R = source_R + i * stride_R;
			const BYTE* row_G = source_G + i * stride_G;
			const BYTE* row_B = source_B + i * stride_B;
			for (size_t j = 0; j < width; ++j)
			{
				data[(i * width + j) * 4 + (isRGBA ? 0 : 2)] = row_R[j];
				data[(i * width + j) * 4 + 1] = row_G[j];
				data[(i * width + j) * 4 + (isRGBA ? 2 : 0)] = row_B[j];
				data[(i * width + j) * 4 + 3] = 255;
			}
		}

		return true;
	}
}
