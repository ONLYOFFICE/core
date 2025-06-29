#include "heif.h"
#include "../../Common/3dParty/heif/libheif/libheif/api/libheif/heif.h"
#include "../../UnicodeConverter/UnicodeConverter.h"
#include "../../common/File.h"

namespace NSHeif {
	bool CHeifFile::isHeif(const std::wstring& fileName)
	{
		NSUnicodeConverter::CUnicodeConverter converter;

		heif_context* ctx = heif_context_alloc();
		bool status = heif_context_read_from_file(ctx, converter.fromUnicode(fileName, "UTF-8").c_str(), nullptr).code == heif_error_Ok;

		heif_context_free(ctx);
		return status;
	}

	bool CHeifFile::isHeif(BYTE* buffer, size_t size)
	{
		NSFile::CFileBinary file;
		std::wstring tmp_file = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"heif");

		if (!file.CreateFile(tmp_file))
			return false;

		file.WriteFile(buffer, size);
		file.CloseFile();

		return isHeif(tmp_file);
	}

	bool CHeifFile::Open(CBgraFrame *frame, const std::wstring& fileName, bool isRGBA)
	{
		NSUnicodeConverter::CUnicodeConverter converter;

		heif_context* ctx = heif_context_alloc();
		if (heif_context_read_from_file(ctx, converter.fromUnicode(fileName, "UTF-8").c_str(), nullptr).code != heif_error_Ok)
			return false;

		heif_image_handle* handle;
		if (heif_context_get_primary_image_handle(ctx, &handle).code != heif_error_Ok)
			return false;

		heif_image* img;
		if (heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr).code != heif_error_Ok)
			return false;

		int width = heif_image_get_primary_width(img);
		int height = heif_image_get_primary_height(img);

		int stride;
		const BYTE* source = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

		if (stride == 0 || source == nullptr)
			return false;

		BYTE* data = new BYTE[stride * height];

		frame->put_Width(width);
		frame->put_Height(height);
		frame->put_Stride(stride);
		frame->put_Data(data);
		frame->put_IsRGBA(isRGBA);
		memcpy(data, source, stride * height);

		heif_image_release(img);
		heif_image_handle_release(handle);
		heif_context_free(ctx);

		return true;
	}

	bool CHeifFile::Open(CBgraFrame *frame, BYTE* buffer, size_t size, bool isRGBA)
	{
		NSFile::CFileBinary file;
		std::wstring tmp_file = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"heif");

		if (!file.CreateFile(tmp_file))
			return false;

		file.WriteFile(buffer, size);
		file.CloseFile();

		bool status = false;
		if (Open(frame, tmp_file, isRGBA))
			status = true;

		if (NSFile::CFileBinary::Exists(tmp_file))
			NSFile::CFileBinary::Remove(tmp_file);

		return status;
	}

	bool CHeifFile::Save(CBgraFrame *frame, const std::wstring& dstPath)
	{
		if (!frame)
			return false;

		heif_context* ctx = heif_context_alloc();
		heif_encoder* encoder;
		if (heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder).code != heif_error_Ok)
			return false;

		if (heif_encoder_set_lossy_quality(encoder, 50).code != heif_error_Ok)
			return false;

		heif_image* img;
		if (heif_context_encode_image(ctx, img, encoder, nullptr, nullptr).code != heif_error_Ok)
			return false;

		heif_encoder_release(encoder);

		NSUnicodeConverter::CUnicodeConverter converter;

		if (heif_context_write_to_file(ctx, converter.fromUnicode(dstPath, "UTF-8").c_str()).code != heif_error_Ok)
			return false;

		heif_context_free(ctx);

		return true;
	}
}
