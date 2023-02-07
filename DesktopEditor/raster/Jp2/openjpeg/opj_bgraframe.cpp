#define OPJ_SKIP_POISON
#include "./opj_bgraframe.h"

#include "../../../common/File.h"
#include "opj_includes.h"
#include "opj_malloc.h"

#include "../../bin/common/color.c"

#define USE_SEPARATE_COMPONENTS
#ifdef USE_SEPARATE_COMPONENTS
// 20 Mb (4 channels)
#define MEMORY_LIMIT_FOR_TILE 5000000
#endif

// memory reader
typedef struct
{
    OPJ_UINT8* pData;
    OPJ_SIZE_T dataSize;
    OPJ_SIZE_T offset;
} opj_input_memory_stream;

static OPJ_SIZE_T opj_input_memory_stream_read(void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * p_user_data)
{
    opj_input_memory_stream* l_stream = (opj_input_memory_stream*)p_user_data;
    OPJ_SIZE_T l_nb_bytes_read = p_nb_bytes;

    if (l_stream->offset >= l_stream->dataSize) {
        return (OPJ_SIZE_T)-1;
    }

    if (p_nb_bytes > (l_stream->dataSize - l_stream->offset)) {
        l_nb_bytes_read = l_stream->dataSize - l_stream->offset;
    }

    memcpy(p_buffer, &(l_stream->pData[l_stream->offset]), l_nb_bytes_read);
    l_stream->offset += l_nb_bytes_read;
    return l_nb_bytes_read;
}

static OPJ_SIZE_T opj_input_memory_stream_write(void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * p_user_data)
{
    opj_input_memory_stream* l_stream = (opj_input_memory_stream*)p_user_data;
    OPJ_SIZE_T l_nb_bytes_write = p_nb_bytes;

    if (l_stream->offset >= l_stream->dataSize) {
        return (OPJ_SIZE_T)-1;
    }

    if (p_nb_bytes > (l_stream->dataSize - l_stream->offset)) {
        l_nb_bytes_write = l_stream->dataSize - l_stream->offset;
    }

    memcpy(&(l_stream->pData[l_stream->offset]), p_buffer, l_nb_bytes_write);
    l_stream->offset += l_nb_bytes_write;
    return l_nb_bytes_write;
}

static OPJ_OFF_T opj_input_memory_stream_skip(OPJ_OFF_T p_nb_bytes, void * p_user_data)
{
    opj_input_memory_stream* l_stream = (opj_input_memory_stream*)p_user_data;

    if (p_nb_bytes < 0) {
        return -1;
    }
    l_stream->offset += (OPJ_SIZE_T)p_nb_bytes;
    return p_nb_bytes;
}

static OPJ_BOOL opj_input_memory_stream_seek(OPJ_OFF_T p_nb_bytes, void * p_user_data)
{
    opj_input_memory_stream* l_stream = (opj_input_memory_stream*)p_user_data;
    if (p_nb_bytes < 0) {
        return OPJ_FALSE;
    }
    l_stream->offset = (OPJ_SIZE_T)p_nb_bytes;
    return OPJ_TRUE;
}

static void opj_input_memory_stream_free(void * p_user_data)
{
    opj_input_memory_stream* p = (opj_input_memory_stream*)p_user_data;
    free(p);
    // external free data!
}

// file reader
static OPJ_SIZE_T opj_read_from_file(void * p_buffer, OPJ_SIZE_T p_nb_bytes,
                                     FILE * p_file)
{
    OPJ_SIZE_T l_nb_read = fread(p_buffer, 1, p_nb_bytes, p_file);
    return l_nb_read ? l_nb_read : (OPJ_SIZE_T) - 1;
}

static OPJ_UINT64 opj_get_data_length_from_file(FILE * p_file)
{
    OPJ_OFF_T file_length = 0;

    OPJ_FSEEK(p_file, 0, SEEK_END);
    file_length = (OPJ_OFF_T)OPJ_FTELL(p_file);
    OPJ_FSEEK(p_file, 0, SEEK_SET);

    return (OPJ_UINT64)file_length;
}

static OPJ_SIZE_T opj_write_from_file(void * p_buffer, OPJ_SIZE_T p_nb_bytes,
                                      FILE * p_file)
{
    return fwrite(p_buffer, 1, p_nb_bytes, p_file);
}

static OPJ_OFF_T opj_skip_from_file(OPJ_OFF_T p_nb_bytes, FILE * p_user_data)
{
    if (OPJ_FSEEK(p_user_data, p_nb_bytes, SEEK_CUR)) {
        return -1;
    }

    return p_nb_bytes;
}

static OPJ_BOOL opj_seek_from_file(OPJ_OFF_T p_nb_bytes, FILE * p_user_data)
{
    if (OPJ_FSEEK(p_user_data, p_nb_bytes, SEEK_SET)) {
        return OPJ_FALSE;
    }

    return OPJ_TRUE;
}

OPJ_CODEC_FORMAT check_j2000_type(BYTE* pBuffer, unsigned int nSize)
{
    if (!pBuffer)
        return OPJ_CODEC_UNKNOWN;

    if ((32 <= nSize) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1] && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
        && 0x6a == pBuffer[4] && 0x50 == pBuffer[5] && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]

        && 0x0d == pBuffer[8] && 0x0a == pBuffer[9] && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
        && 0x00 == pBuffer[12] && 0x00 == pBuffer[13] && 0x00 == pBuffer[14] /*&&  (0x14 == pBuffer[15] || 0x18 == pBuffer[15] )*/

        && 0x66 == pBuffer[16] && 0x74 == pBuffer[17] && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
        && 0x6a == pBuffer[20] && 0x70 == pBuffer[21] && 0x32 == pBuffer[22] && 0x20 == pBuffer[23]

        && 0x00 == pBuffer[24] && 0x00 == pBuffer[25] && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
        /*&& 0x6a == pBuffer[28] && 0x70 == pBuffer[29]  && 0x32 == pBuffer[30] && 0x20 == pBuffer[31]*/))
    {
        return OPJ_CODEC_JP2;
    }
    if ((4 <= nSize) && (0xff == pBuffer[0] && 0x4f == pBuffer[1] && 0xff == pBuffer[2] && 0x51 == pBuffer[3]))
    {
        return OPJ_CODEC_J2K;
    }
    if ((32 <= nSize) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1] && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
        && 0x6a == pBuffer[4] && 0x50 == pBuffer[5] && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]

        && 0x0d == pBuffer[8] && 0x0a == pBuffer[9] && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
        && 0x00 == pBuffer[12] && 0x00 == pBuffer[13] && 0x00 == pBuffer[14] && 0x18 == pBuffer[15]

        && 0x66 == pBuffer[16] && 0x74 == pBuffer[17] && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
        && 0x6d == pBuffer[20] && 0x6a == pBuffer[21] && 0x70 == pBuffer[22] && 0x32 == pBuffer[23]

        && 0x00 == pBuffer[24] && 0x00 == pBuffer[25] && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
        && 0x6d == pBuffer[28] && 0x6a == pBuffer[29] && 0x70 == pBuffer[30] && 0x32 == pBuffer[31]))
    {
        return OPJ_CODEC_JPT;
    }
    return OPJ_CODEC_UNKNOWN;
}
OPJ_CODEC_FORMAT check_j2000_type(FILE* pFile)
{
    if (!pFile)
        return OPJ_CODEC_UNKNOWN;

    BYTE *pBuffer = new BYTE[32];

    if (!pBuffer)
        return OPJ_CODEC_UNKNOWN;

    size_t nBytesRead = fread(pBuffer, 1, 32, pFile);
    OPJ_CODEC_FORMAT type = check_j2000_type(pBuffer, nBytesRead);
    delete[] pBuffer;
    OPJ_FSEEK(pFile, 0, SEEK_SET);
    return type;
}

int check_j2000_type(const std::wstring& sFile)
{
    NSFile::CFileBinary oFile;
    if (oFile.OpenFile(sFile))
    {
        return check_j2000_type(oFile.GetFileNative());
    }
    return 0;
}

opj_stream_t* get_file_stream(const std::wstring& sFile, OPJ_CODEC_FORMAT& codec)
{
    FILE* p_file = NSFile::CFileBinary::OpenFileNative(sFile, L"rb");
    if (!p_file)
        return NULL;

    codec = check_j2000_type(p_file);
    if (OPJ_CODEC_UNKNOWN == codec)
    {
        fclose(p_file);
        return NULL;
    }

    opj_stream_t* l_stream = opj_stream_create(OPJ_J2K_STREAM_CHUNK_SIZE, OPJ_TRUE);
    if (!l_stream)
    {
        fclose(p_file);
        return NULL;
    }

    opj_stream_set_user_data(l_stream, p_file, (opj_stream_free_user_data_fn) fclose);
    opj_stream_set_user_data_length(l_stream, opj_get_data_length_from_file(p_file));
    opj_stream_set_read_function(l_stream, (opj_stream_read_fn) opj_read_from_file);
    opj_stream_set_write_function(l_stream, (opj_stream_write_fn) opj_write_from_file);
    opj_stream_set_skip_function(l_stream, (opj_stream_skip_fn) opj_skip_from_file);
    opj_stream_set_seek_function(l_stream, (opj_stream_seek_fn) opj_seek_from_file);

    return l_stream;
}

opj_stream_t* get_file_stream(BYTE* data, unsigned int size, OPJ_CODEC_FORMAT& codec)
{
    if (!data)
        return NULL;

    codec = check_j2000_type(data, size);
    if (OPJ_CODEC_UNKNOWN == codec)
        return NULL;

    opj_stream_t* l_stream = opj_stream_create(OPJ_J2K_STREAM_CHUNK_SIZE, OPJ_TRUE);
    if (!l_stream)
        return NULL;

    opj_input_memory_stream* p_file = (opj_input_memory_stream*)malloc(sizeof(opj_input_memory_stream));
    p_file->pData = data;
    p_file->dataSize = size;
    p_file->offset = 0;

    opj_stream_set_user_data(l_stream, p_file, (opj_stream_free_user_data_fn) opj_input_memory_stream_free);
    opj_stream_set_user_data_length(l_stream, (OPJ_UINT64)size);
    opj_stream_set_read_function(l_stream, (opj_stream_read_fn) opj_input_memory_stream_read);
    opj_stream_set_write_function(l_stream, (opj_stream_write_fn) opj_input_memory_stream_write);
    opj_stream_set_skip_function(l_stream, (opj_stream_skip_fn) opj_input_memory_stream_skip);
    opj_stream_set_seek_function(l_stream, (opj_stream_seek_fn) opj_input_memory_stream_seek);

    return l_stream;
}

static int int_ceildivpow2(int a, int b)
{
    return (a + (1 << b) - 1) >> b;
}

namespace openjpeg
{
	bool ImageToData(const opj_image_t* pImage, BYTE* pData, const int& nComponentsCount, const int& nWidth, const int& nHeight, const int& nYOffset, const bool isFlip)
	{
		if (!pData)
			return false;

		BYTE* pPtr = pData;

		int nResW = int_ceildivpow2(pImage->comps[0].w, pImage->comps[0].factor);
		int nResH = int_ceildivpow2(pImage->comps[0].h, pImage->comps[0].factor);

		for (int nY = 0, nIndex = 0; nY < nResH; ++nY)
		{
			if (isFlip)
				pPtr = pData + (nHeight - 1 - nYOffset - nY) * nWidth * nComponentsCount;

			for (int nX = 0; nX < nResW; ++nX, ++nIndex)
			{
				for (int nComponent = 0; nComponent < nComponentsCount; nComponent++)
				{
					pPtr[nComponent] = pImage->comps[nComponent].data[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				}
				pPtr += nComponentsCount;
			}
		}

		return true;
	}
	BYTE* GetBuffer(opj_image_t* pImage, BYTE*& pData, int& nComponentsCount,  int& nWidth, int& nHeight)
	{
		nWidth  = pImage->comps[0].w;
		nHeight = pImage->comps[0].h;
		nComponentsCount = pImage->numcomps;

		for (int nComponent = 1; nComponent < nComponentsCount; nComponent++)
		{
			if (pImage->comps[0].w != pImage->comps[nComponent].w
					|| pImage->comps[0].h != pImage->comps[nComponent].h
					|| pImage->comps[0].prec != pImage->comps[nComponent].prec)

			{
				return NULL;
			}
		}

		int nBufferSize = nComponentsCount * nWidth * nHeight;
		if (nBufferSize < 1 || nComponentsCount <= 0)
			return NULL;

		pData = new BYTE[nBufferSize];

		return pData;
	}
	bool GetData(BYTE* pFileData, const unsigned int& nFileSize, BYTE*& pData, int& nComponents, int& nWidth, int& nHeight, const bool& isFlip)
	{
		OPJ_CODEC_FORMAT codec = OPJ_CODEC_UNKNOWN;
		opj_stream_t* l_stream = get_file_stream(pFileData, nFileSize, codec);

		if (!l_stream)
			return false;

		opj_dparameters_t parameters;
		opj_set_default_decoder_parameters(&parameters);

		opj_codec_t* l_codec = opj_create_decompress(codec);

		bool bResult = false;

		opj_image_t* image = NULL;
		unsigned int nTileHeight = 0;

		if (!opj_setup_decoder(l_codec, &parameters))
			goto end;

		if (!opj_read_header(l_stream, l_codec, &image))
			goto end;

#ifdef USE_SEPARATE_COMPONENTS
		if (((image->x1 - image->x0) * (image->y1 - image->y0)) > MEMORY_LIMIT_FOR_TILE)
			nTileHeight = MEMORY_LIMIT_FOR_TILE / (image->x1 - image->x0);
#endif
		pData       = NULL;
		nComponents = 0;
		nWidth      = 0;
		nHeight     = 0;

		if (!GetBuffer(image, pData, nComponents, nWidth, nHeight))
			goto end;

		if (0 == nTileHeight)
		{
			if (!(opj_decode(l_codec, l_stream, image) && opj_end_decompress(l_codec, l_stream)))
				goto end;
		}
		else
		{
			unsigned int nLimitY = image->y1;
			unsigned int nOffsetY = image->y0;
			while (nOffsetY < nLimitY)
			{
				if ((nLimitY - nOffsetY) < nTileHeight)
					nTileHeight = nLimitY - nOffsetY;

				if (!opj_set_decode_area(l_codec, image, image->x0, nOffsetY, image->x1, nOffsetY + nTileHeight))
				{
					opj_stream_destroy(l_stream); l_stream = NULL;
					opj_destroy_codec(l_codec); l_codec = NULL;
					opj_image_destroy(image); image = NULL;

					l_stream = get_file_stream(pFileData, nFileSize, codec);

					if (!l_stream)
						goto end;

					l_codec = opj_create_decompress(codec);

					if (!opj_setup_decoder(l_codec, &parameters))
						goto end;

					if (!opj_read_header(l_stream, l_codec, &image))
						goto end;

					if (!opj_set_decode_area(l_codec, image, image->x0, nOffsetY, image->x1, nOffsetY + nTileHeight))
						goto end;
				}

				if (!(opj_decode(l_codec, l_stream, image)))
					goto end;

				if (!opj_end_decompress(l_codec, l_stream))
					goto end;

				if (!ImageToData(image, isFlip ? pData : pData + (nComponents * nWidth) * (nLimitY - nOffsetY - nTileHeight), nComponents, nWidth, nHeight, (nLimitY - nOffsetY - nTileHeight), isFlip))
					goto end;

				nOffsetY += nTileHeight;
			}
		}

		bResult = true;

end:
		opj_stream_destroy(l_stream);
		opj_destroy_codec(l_codec);

		if (bResult && image && 0 == nTileHeight)
			bResult = ImageToData(image, pData, nComponents, nWidth, nHeight, 0, isFlip);

		opj_image_destroy(image);

		return bResult;
	}
	void DestroyData(BYTE*& pData)
	{
		if (pData)
		{
			delete[] pData;
			pData = NULL;
		}
	}
}
