#include "heif.h"
#include "../../common/File.h"

namespace NSHeif {
	bool CHeifFile::isHeif(const std::wstring& fileName)
	{
		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););
		bool status = heif_context_read_from_file(ctx, m_oConverter.fromUnicode(fileName, "UTF-8").c_str(), nullptr).code == heif_error_Ok;

		return status;
	}

	bool CHeifFile::isHeif(BYTE* buffer, DWORD size)
	{
		NSFile::CFileBinary file;
		std::wstring tmp_file = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"heiс");

		if (!file.CreateFile(tmp_file))
			return false;

		file.WriteFile(buffer, size);
		file.CloseFile();

		return isHeif(tmp_file);
	}

	bool CHeifFile::Open(CBgraFrame *frame, const std::wstring& fileName)
	{
		heif_context* ctx = heif_context_alloc();
		defer(heif_context_free(ctx););

		if (IsError(heif_context_read_from_file(ctx, m_oConverter.fromUnicode(fileName, "UTF-8").c_str(), nullptr)))
			return false;

		heif_image_handle* handle;
		defer(heif_image_handle_release(handle););

		if (IsError(heif_context_get_primary_image_handle(ctx, &handle)))
			return false;

		heif_image* img;
		defer(heif_image_release(img););

		if (IsError(heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr)))
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


		for (size_t i = 0; i < height; ++i)
		{
			const BYTE* row = source + i * stride;
			for (size_t j = 0; j < width; ++j)
			{
				data[(i * width + j) * 3 + 0] = row[j * 3 + 2];
				data[(i * width + j) * 3 + 1] = row[j * 3 + 1];
				data[(i * width + j) * 3 + 2] = row[j * 3 + 0];
			}
		}

		return true;
	}

	bool CHeifFile::Open(CBgraFrame *frame, BYTE* buffer, DWORD size)
	{
		NSFile::CFileBinary file;
		std::wstring tmp_file = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"heif");

		if (!file.CreateFile(tmp_file))
			return false;

		file.WriteFile(buffer, size);
		file.CloseFile();

		bool status = false;
		if (Open(frame, tmp_file))
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
		defer(heif_context_free(ctx););

		heif_encoder* encoder;
		defer(heif_encoder_release(encoder););

		if (IsError(heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder)))
			return false;

		if (IsError(heif_encoder_set_lossy_quality(encoder, 50)))
			return false;

		heif_image* img;
		defer(heif_image_release(img););

		int width = frame->get_Width();
		int height = frame->get_Height();

		if (IsError(heif_image_create(width, height, heif_colorspace_RGB, heif_chroma_interleaved_RGB, &img)))
			return false;

		BYTE* data = heif_image_get_plane(img, heif_channel_interleaved, nullptr);
		const BYTE* source = frame->get_Data();

		for (size_t i = 0; i <height; ++i)
		{
			const BYTE* row = source + i * frame->get_Stride();
			for (size_t j = 0; j < width; ++j)
			{
				data[(i * width + j) * 3 + 0] = row[j * 3 + 2];
				data[(i * width + j) * 3 + 1] = row[j * 3 + 1];
				data[(i * width + j) * 3 + 2] = row[j * 3 + 0];
			}
		}

		if (!data)
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
}
