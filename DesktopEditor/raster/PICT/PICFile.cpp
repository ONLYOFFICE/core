/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "PICFile.h"
#include "../../common/File.h"

#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif

CPictFile::CPictFile()
{
	m_pRenderer = NSGraphics::Create();
	m_pFontManager = NSFonts::NSFontManager::Create();
	m_pRenderer->SetFontManager(m_pFontManager);
}

CPictFile::~CPictFile()
{
	RELEASEINTERFACE(m_pFontManager);
	RELEASEINTERFACE(m_pRenderer);
}

bool CPictFile::Open(CBgraFrame* frame, const std::wstring& fileName, bool isRGB)
{
	NSFile::CFileBinary file;
	if (!file.OpenFile(fileName))
		return false;

	m_pFile = file.GetFileNative();
	auto status = Decode();
	file.CloseFile();

	if (!status)
		return false;

	BYTE* data = new BYTE[4 * m_oImgData.m_nWidth * m_oImgData.m_nHeight];
	frame->put_Data(data);
	frame->put_Height(m_oImgData.m_nHeight);
	frame->put_Width(m_oImgData.m_nWidth);
	frame->put_Stride(4 * m_oImgData.m_nWidth);

	BYTE* buffer = data;

	for (size_t i = 0; i < m_oImgData.m_nHeight; i++)
	{
		BYTE* q = m_oImgData.m_pPixelData + 4 * i * m_oImgData.m_nWidth;
		for (size_t j = 0; j < m_oImgData.m_nWidth; j++)
		{
			buffer[isRGB ? 0 : 2] = * q;
			buffer[1] = * (q + 1);
			buffer[isRGB ? 2 : 0] = * (q + 2);
			buffer[3] = *(q + 3);
			q += 4;
			buffer += 4;
		}
	}

	return true;
}

bool CPictFile::Open(CBgraFrame* frame, BYTE* buffer, const size_t& size, bool isRGB)
{
	NSFile::CFileBinary file;
	auto tmp_file = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"pct");

	if (tmp_file.empty())
		return false;

	if (!file.CreateFile(tmp_file))
		return false;

	file.WriteFile(buffer, size);
	file.CloseFile();

	return Open(frame, tmp_file, isRGB);
}

bool CPictFile::Decode()
{
	if (m_pFile == nullptr)
		return 0;

	if (!DecodeHeader())
		return false;

	if (feof(m_pFile) != 0)
		return false;

	return DecodeData();
}

bool CPictFile::DecodeHeader()
{
	unsigned char header[4];

	Aggplus::Rect rect;

	Read(4, header);

	if (!((header[0] == 0x50) && (header[1] == 0x49) && (header[2] == 0x43) && (header[3] == 0x54 )))
	{
		rect.Y		= (short) ((header[2] << 8) | header[3]);
		rect.X		= ReadShortValue();
		rect.Height	= ReadShortValue() - rect.Y;
		rect.Width	= ReadShortValue() - rect.X;

		if (!rect.IsPositive())
			return false;

		m_oImgData.m_nWidth  = rect.Width;
		m_oImgData.m_nHeight = rect.Height;

		fseek(m_pFile, 1, SEEK_CUR);
		if (fgetc(m_pFile) == 0x11)
		{
			if (fgetc(m_pFile) == 2)
			{
				if (fgetc(m_pFile) != 0xff)
					return false;
				m_nVersion = 2;
			}
			else
				m_nVersion = 1;

			return true;
		}

		fseek(m_pFile, 512, SEEK_SET);
		fseek(m_pFile, 2, SEEK_CUR);
		if (!ReadRectangle(&rect))
			return false;

		if (!rect.IsPositive())
			return false;

		m_oImgData.m_nWidth  = rect.Width;
		m_oImgData.m_nHeight = rect.Height;

		fseek(m_pFile, 1, SEEK_CUR);
		if (fgetc(m_pFile) == 0x11)
		{
			int version = fgetc(m_pFile);
			if (version == 2)
			{
				if (fgetc(m_pFile) != 0xff)
					return false;

				m_nVersion = 2;
			}
			else if (version == 1)
				m_nVersion = 1;
			else
				return false;

			return true;
		}
	}

	return false;
}

bool CPictFile::DecodeData()
{
	int flag = 0;

	bool  is_pix_data = false;
	if (!m_oImgData.m_pPixelData)
		m_oImgData.m_pPixelData = (BYTE*) malloc(m_oImgData.m_nWidth * m_oImgData.m_nHeight * 4);

	for (int code = 0; feof(m_pFile) == 0; )
	{
		if (m_nVersion == 1 || ftell(m_pFile) % 2 != 0)
			code = fgetc(m_pFile);
		if (m_nVersion == 2)
			code = ReadSignedShortValue();
		code &= 0xffff;
		if (code < 0)
			break;
		if (code == 0)
			continue;
		if (code <= 0xa1)
		{
			switch(code)
			{
			case 0x01:
			{
				size_t length = ReadShortValue();
				if (length > GetFileSize())
					return false;

				if (length != 0x000a)
				{
					for (size_t i = 0; i < length - 2; i++)
						if (fgetc(m_pFile) == EOF)
							break;
					break;
				}

				Aggplus::Rect frame;
				if (ReadRectangle(&frame) == 0)
					return false;
				if (((frame.GetLeft() & 0x8000) != 0) || ((frame.GetTop() & 0x8000) != 0))
					break;
				m_oImgData.m_nHeight = frame.Height;
				m_oImgData.m_nWidth  = frame.Width;
				m_oImgData.m_pPixelData = (BYTE*)realloc(m_oImgData.m_pPixelData, m_oImgData.m_nHeight * m_oImgData.m_nWidth * 4);
				break;
			}
			case 0x11:
			{
				fseek(m_pFile, 1, SEEK_CUR);
				break;
			}
			case 0x03:
			case 0x05:
			case 0x08:
			case 0x15:
			case 0x16:
			case 0xa0:
			{
				fseek(m_pFile, 2, SEEK_CUR);
				break;
			}
			case 0x1d:
			case 0x1f:
			{
				fseek(m_pFile, 3, SEEK_CUR);
				break;
			}
			case 0x06:
			case 0x0b:
			case 0x0c:
			case 0x0f:
			case 0x6d:
			case 0x6e:
			case 0x6f:
			{
				fseek(m_pFile, 4, SEEK_CUR);
				break;
			}
			case 0x02:
			case 0x09:
			case 0x0a:
			case 0x10:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x45:
			case 0x46:
			case 0x47:
			case 0x55:
			case 0x56:
			case 0x57:
			{
				fseek(m_pFile, 8, SEEK_CUR);
				break;
			}
			case 0x2d:
			{
				fseek(m_pFile, 10, SEEK_CUR);
				break;
			}
			case 0x65:
			case 0x66:
			case 0x67:
			{
				fseek(m_pFile, 12, SEEK_CUR);
				break;
			}
			case 0xa1:
			{
				fseek(m_pFile, 2, SEEK_CUR);
				size_t length = ReadShortValue();
				if (length > GetFileSize())
					return false;

				fseek(m_pFile, length, SEEK_CUR);
				break;
			}
			case 0x24:
			case 0x25:
			case 0x26:
			case 0x27:
			case 0x2e:
			case 0x2f:
			case 0x75:
			case 0x76:
			case 0x77:
			case 0x80:
			case 0x81:
			case 0x82:
			case 0x83:
			case 0x84:
			case 0x92:
			case 0x93:
			case 0x94:
			case 0x95:
			case 0x96:
			case 0x97:
			case 0x9c:
			case 0x9d:
			case 0x9e:
			case 0x9f:
			{
				size_t length = ReadShortValue();
				if (length > GetFileSize())
					return false;

				fseek(m_pFile, length - 2, SEEK_CUR);
				break;
			}
			case 0x85:
			case 0x86:
			case 0x87:
			{
				size_t length = ReadShortValue();
				if (length > GetFileSize())
					return false;

				fseek(m_pFile, length - 4, SEEK_CUR);
				break;
			}
			case 0x04:
			{
				char c = fgetc(m_pFile);
				if ((c & 0x01) != 0)
					m_nFontStyle |= 0x01;
				if ((c & 0x02) != 0)
					m_nFontStyle |= 0x02;
				if ((c & 0x04) != 0)
					m_nFontStyle |= 0x7c >> 2;
				if ((c & 0x08) != 0)
					m_nFontStyle |= 0x0180 >> 7;
				break;
			}
			case 0x07:
			{
				m_nPenHeight = ReadShortValue();
				m_nPenWidth = ReadShortValue();
				break;
			}
			case 0x0d:
			{
				m_nFontSize = ReadShortValue();
				break;
			}
			case 0x0e:
			{
				long color = ReadLongValue();

				switch(color)
				{
				case 33: m_pRenderer->put_BrushColor1(0x000000); break;
				case 30: m_pRenderer->put_BrushColor1(0xFFFFFF); break;
				case 205: m_pRenderer->put_BrushColor1(0xFF8080); break; //lightred
				case 341: m_pRenderer->put_BrushColor1(0x90EE90); break; //lightgreen
				case 409: m_pRenderer->put_BrushColor1(0xADD8E6); break; //lightblue
				case 273: m_pRenderer->put_BrushColor1(0xE0FFFF); break; //lightcyan
				case 137: m_pRenderer->put_BrushColor1(0xFF80FF); break; //lightmagenta
				case 69: m_pRenderer->put_BrushColor1(0xFFFF00); break; //yellow
				default: m_pRenderer->put_BrushColor1(0xD3D3D3); break; //lightgray
				}
				break;
			}
			case 0x12:
			case 0x13:
			case 0x14:
			{
				short pattern = ReadShortValue();
				for (size_t i = 0; i < 8; i++)
					if (fgetc(m_pFile) == EOF)
						break;
				if (pattern == 2)
				{
					for (size_t i = 0; i < 5; i++)
						if (fgetc(m_pFile) == EOF)
							break;
					break;
				}
				if (pattern != 1)
					return false;

				size_t length = ReadShortValue();
				if (length > GetFileSize())
					return false;

				Aggplus::Rect frame;
				if (ReadRectangle(&frame) == 0)
					return false;

				fseek(m_pFile, 18, SEEK_CUR);

				short bits_per_pixel	= ReadShortValue();
				short component_count	= ReadShortValue();
				short component_size	= ReadShortValue();

				fseek(m_pFile, 12, SEEK_CUR);

				if (feof(m_pFile) != 0 || bits_per_pixel <= 0 ||
					bits_per_pixel > 32 || component_count <= 0 ||
					component_count > 4 || component_size <= 0)
					return false;

				fseek(m_pFile, 4, SEEK_CUR);
				flag = ReadShortValue();
				length = ReadShortValue();

				if (length > GetFileSize())
					return false;

				for (size_t i = 0; i < length; i++)
					fseek(m_pFile, 4, SEEK_CUR);

				size_t width  = frame.Width;
				size_t height = frame.Height;

				if (bits_per_pixel <= 8)
					length &= 0x7fff;

				if (bits_per_pixel == 16)
					width <<= 1;

				if (length == 0)
					length = width;

				if (length < 8)
				{
					for (size_t i = 0; i < (length * height); i++)
						if (fgetc(m_pFile) == EOF)
							break;
				}
				else
				{
					for (size_t i = 0; i < height; i++)
					{
						size_t scanline_length;

						if (feof(m_pFile) != 0)
							break;

						if (length > 200)
							scanline_length = ReadShortValue();
						else
							scanline_length = fgetc(m_pFile);

						if (scanline_length > GetFileSize())
							return false;

						for (size_t j = 0; j < scanline_length; j++)
							if (fgetc(m_pFile) == EOF)
								break;
					}
				}
				break;
			}
			case 0x1a:
			{
				BYTE r = ReadShortValue() >> 8;
				BYTE g = ReadShortValue() >> 8;
				BYTE b = ReadShortValue() >> 8;
				long c = r << 16 | g << 8 | b;
				m_pRenderer->put_BrushColor1(c);
				break;
			}
			case 0x1b:
			{
				fseek(m_pFile, 6, SEEK_CUR);
				break;
			}
			case 0x20:
			{
				short y1 = ReadShortValue();
				short x1 = ReadShortValue();
				short y2 = ReadShortValue();
				short x2 = ReadShortValue();

				m_oPenPoint = Aggplus::Point(x2, y2);

				if (m_nPenHeight == 0 && m_nPenWidth == 0)
					break;

				DrawLine(Aggplus::Point(x1, y1), Aggplus::Point(x2, y2));
				break;
			}
			case 0x21:
			{
				short y1 = ReadShortValue();
				short x1 = ReadShortValue();

				if (feof(m_pFile))
					break;

				if (m_nPenHeight == 0 && m_nPenWidth == 0)
					break;

				DrawLine(Aggplus::Point(x1, y1), m_oPenPoint);
				break;
			}
			case 0x22:
			{
				short y1 = ReadShortValue();
				short x1 = ReadShortValue();
				short y2 = static_cast<short>(y1 + fgetc(m_pFile));
				short x2 = static_cast<short>(x1 + fgetc(m_pFile));

				if (m_nPenHeight == 0 && m_nPenWidth == 0)
					break;

				DrawLine(Aggplus::Point(x1, y1), Aggplus::Point(x2, y2));
				break;
			}
			case 0x23:
			{
				short y2 = static_cast<short>(m_oPenPoint.Y + fgetc(m_pFile));
				short x2 = static_cast<short>(m_oPenPoint.X + fgetc(m_pFile));

				if (m_nPenHeight == 0 && m_nPenWidth == 0)
					break;

				DrawLine(m_oPenPoint, Aggplus::Point(x2, y2));
				break;
			}
			case 0x28:
			{
				short y = ReadShortValue();
				short x = ReadShortValue();

				m_oTextPoint = Aggplus::Point(x, y);
				ReadAndDrawText(x, y);
				break;
			}
			case 0x29:
			{
				unsigned char h = static_cast<unsigned char>(fgetc(m_pFile));
				ReadAndDrawText(m_oTextPoint.X + h, m_oTextPoint.Y);
				break;
			}
			case 0x2a:
			{
				unsigned char v = static_cast<unsigned char>(fgetc(m_pFile));
				ReadAndDrawText(m_oTextPoint.X, m_oTextPoint.Y + v);
				break;
			}
			case 0x2b:
			{
				unsigned char h = static_cast<unsigned char>(fgetc(m_pFile));
				unsigned char v = static_cast<unsigned char>(fgetc(m_pFile));
				ReadAndDrawText(m_oTextPoint.X + h, m_oTextPoint.Y + v);
				break;
			}
			case 0x2c:
			{
				fseek(m_pFile, 4, SEEK_CUR);
				char byte_len = fgetc(m_pFile);
				unsigned short len = static_cast<unsigned short>(byte_len) & 0x00ff;
				char f_name[256];
				Read(len, f_name);
				f_name[len] = 0;
				m_wsFontName = std::wstring(len, L'\0');
				std::mbstowcs(&m_wsFontName[0], f_name, len);
				break;
			}
			case 0x30:
			case 0x31:
			case 0x32:
			case 0x33:
			case 0x34:
			{
				if (!ReadRectangle(&m_oLastRect))
					return false;

				DrawRect(code == 0x30);
				break;
			}
			case 0x38:
			case 0x39:
			case 0x3a:
			case 0x3b:
			case 0x3c:
			{
				DrawRect(code == 0x38);
				break;
			}
			case 0x40:
			case 0x41:
			case 0x42:
			case 0x43:
			case 0x44:
			{
				if (!ReadRectangle(&m_oLastRoundRect))
					return false;

				DrawRoundRect(code == 0x40);
				break;
			}
			case 0x48:
			case 0x49:
			case 0x4a:
			case 0x4b:
			case 0x4c:
			{
				DrawRoundRect(code == 0x48);
				break;
			}
			case 0x50:
			case 0x51:
			case 0x52:
			case 0x53:
			case 0x54:
			{
				if (!ReadRectangle(&m_oLastOval))
					return false;

				DrawOval(code == 0x50);
				break;
			}
			case 0x58:
			case 0x59:
			case 0x5a:
			case 0x5b:
			case 0x5c:
			{
				DrawOval(code == 0x58);
				break;
			}
			case 0x60:
			case 0x61:
			case 0x62:
			case 0x63:
			case 0x64:
			{
				if (!ReadRectangle(&m_oLastArc))
					return false;

				DrawArc();
				break;
			}
			case 0x68:
			case 0x69:
			case 0x6a:
			case 0x6b:
			case 0x6c:
			{
				DrawArc();
				break;
			}
			case 0x70:
			case 0x71:
			case 0x72:
			case 0x73:
			case 0x74:
			{
				ReadPolygon();
				DrawPolygon(code == 0x70);
				break;
			}
			case 0x78:
			case 0x79:
			case 0x7a:
			case 0x7b:
			case 0x7c:
			{
				DrawPolygon(code == 0x78);
				break;
			}
			case 0x90:
			case 0x91:
			case 0x98:
			case 0x99:
			case 0x9a:
			case 0x9b:
			{
				is_pix_data = true;
				short bits_per_pixel	= 0;
				short component_count	= 0;

				size_t bytes_per_line = 0;
				if ((code != 0x9a) && (code != 0x9b))
					bytes_per_line = ReadShortValue();
				else
					fseek(m_pFile, 6, SEEK_CUR);

				Aggplus::Rect frame;
				if (ReadRectangle(&frame) == 0)
					return false;

				Image tile_image(m_oImgData);
				if (frame.Width != 0)
					tile_image.m_nWidth = frame.Width;
				if (frame.Height != 0)
					tile_image.m_nHeight = frame.Height;

				tile_image.m_pPixelData = (BYTE*)malloc(4 * tile_image.m_nWidth * tile_image.m_nHeight);
				if ((code == 0x9a) || (code == 0x9b) || ((bytes_per_line & 0x8000) != 0))
				{
					fseek(m_pFile, 18, SEEK_CUR);

					bits_per_pixel	= ReadShortValue();
					component_count	= ReadShortValue();
					short component_size = ReadShortValue();

					fseek(m_pFile, 12, SEEK_CUR);

					if (feof(m_pFile) != 0 || bits_per_pixel <= 0 ||
						bits_per_pixel > 32 || component_count <= 0 ||
						component_count > 4 || component_size <= 0)
						return false;

					tile_image.m_eAlphaTrait = component_count == 4 ? BlendPixelTrait : UndefinedPixelTrait;
					if (component_count == 4)
						SetImageAlpha(&tile_image, 255);
				}

				if ((code != 0x9a) && (code != 0x9b))
				{
					tile_image.m_nColors = 2;

					if ((bytes_per_line & 0x8000) != 0)
					{
						fseek(m_pFile, 4, SEEK_CUR);
						flag = ReadShortValue();
						tile_image.m_nColors = 1UL * ReadShortValue() + 1;
					}

					tile_image.m_pColormap = new PixelInfo[tile_image.m_nColors + 1];

					for (size_t i = 0; i < tile_image.m_nColors; i++)
					{
						tile_image.m_pColormap[i].storage_class = tile_image.m_eStorageClass;
						tile_image.m_pColormap[i].alpha_trait = tile_image.m_eAlphaTrait;

						double pixel;
						if (tile_image.m_nColors - 1 > 1)
							pixel = ((double) i * (255 / (tile_image.m_nColors - 1)));
						else
							pixel = ((double) i * 255);

						tile_image.m_pColormap[i].red = pixel;
						tile_image.m_pColormap[i].green = pixel;
						tile_image.m_pColormap[i].blue = pixel;
						tile_image.m_pColormap[i].alpha = 255.0;
						tile_image.m_pColormap[i].alpha_trait = BlendPixelTrait;
					}
					tile_image.m_eStorageClass = PseudoClass;

					if ((bytes_per_line & 0x8000) != 0)
					{
						for (size_t i = 0; i < tile_image.m_nColors; i++)
						{
							int k = ReadShortValue() % tile_image.m_nColors;
							if ((flag & 0x8000) != 0)
								k = (size_t) i;

							tile_image.m_pColormap[k].red	= static_cast<double>(ReadShortValue());
							tile_image.m_pColormap[k].green	= static_cast<double>(ReadShortValue());
							tile_image.m_pColormap[k].blue	= static_cast<double>(ReadShortValue());
						}
					}
					else
					{
						for (size_t i = 0; i < tile_image.m_nColors; i++)
						{
							tile_image.m_pColormap[i].red	= 255.0 - tile_image.m_pColormap[i].blue;
							tile_image.m_pColormap[i].green	= 255.0 - tile_image.m_pColormap[i].green;
							tile_image.m_pColormap[i].blue	= 255.0 - tile_image.m_pColormap[i].red;
						}
					}
				}
				if (feof(m_pFile) != 0)
					return false;

				Aggplus::Rect source;
				if (ReadRectangle(&source) == 0)
					return false;

				Aggplus::Rect destination;
				if (ReadRectangle(&destination) == 0)
					return false;

				fseek(m_pFile, 2, SEEK_CUR);

				if ((code == 0x91) || (code == 0x99) || (code == 0x9b))
				{
					size_t length = ReadShortValue();
					if (length > GetFileSize())
						return false;

					for (size_t i = 0; i < length - 2; i++)
						if (fgetc(m_pFile) == EOF)
							break;
				}

				size_t extent;
				BYTE* pixels;
				if ((code != 0x9a) && (code != 0x9b) && (bytes_per_line & 0x8000) == 0)
					pixels = DecodeImage(tile_image, bytes_per_line, 1, &extent);
				else
					pixels = DecodeImage(tile_image, bytes_per_line, bits_per_pixel, &extent);

				if (pixels == (unsigned char*) nullptr)
				{
					delete[] pixels;
					return false;
				}

				BYTE* p = pixels;
				for	(size_t y = 0; y < tile_image.m_nHeight; y++)
				{
					if (p > (pixels + extent + m_oImgData.m_nWidth))
					{
						delete[] pixels;
						return 0;
					}

					unsigned char* q = tile_image.m_pPixelData + 4 * y * frame.Width;
					if (q == (unsigned char *) nullptr)
						break;

					for (size_t x = 0; x < tile_image.m_nWidth; x++)
					{
						if (tile_image.m_eStorageClass == PseudoClass)
						{
							unsigned char index;
							if (*p < 0 || *p >= tile_image.m_nColors)
								index = 0;
							else
								index = *p;

							if (tile_image.m_pChannelMap[IndexPixelChannel].traits != UndefinedPixelTrait)
								q[tile_image.m_pChannelMap[IndexPixelChannel].offset] = index;

							q[tile_image.m_pChannelMap[RedPixelChannel].offset]		= tile_image.m_pColormap[index].red;
							q[tile_image.m_pChannelMap[GreenPixelChannel].offset]	= tile_image.m_pColormap[index].green;
							q[tile_image.m_pChannelMap[BluePixelChannel].offset]	= tile_image.m_pColormap[index].blue;
						}
						else
						{
							if (bits_per_pixel == 16)
							{
								size_t i = (size_t) (*p++);
								int k = (*p);

								q[tile_image.m_pChannelMap[RedPixelChannel].offset]		= (BYTE) ((i & 0x7c) << 1);
								q[tile_image.m_pChannelMap[GreenPixelChannel].offset]	= (BYTE) ((i & 0x03) << 6) |((k & 0xe0) >> 2);
								q[tile_image.m_pChannelMap[BluePixelChannel].offset]	= (BYTE) ((k & 0x1f) << 3);
							}
							else if (tile_image.m_eAlphaTrait == UndefinedPixelTrait)
							{
								if (p > (pixels + extent + 2 * m_oImgData.m_nWidth))
								{
									delete[] pixels;
									return false;
								}

								q[tile_image.m_pChannelMap[RedPixelChannel].offset]		= *p;
								q[tile_image.m_pChannelMap[GreenPixelChannel].offset]	= *(p + tile_image.m_nWidth);
								q[tile_image.m_pChannelMap[BluePixelChannel].offset]	= *(p + 2 * tile_image.m_nWidth);
							}
							else
							{
								if (p > (pixels + extent + 3 * m_oImgData.m_nWidth))
								{
									delete[] pixels;
									return false;
								}

								if (tile_image.m_pChannelMap[AlphaPixelChannel].traits != UndefinedPixelTrait)
									q[tile_image.m_pChannelMap[AlphaPixelChannel].offset] = *p;

								q[tile_image.m_pChannelMap[RedPixelChannel].offset]		= *(p + 1 * tile_image.m_nWidth);
								q[tile_image.m_pChannelMap[GreenPixelChannel].offset]	= *(p + 2 * tile_image.m_nWidth);
								q[tile_image.m_pChannelMap[BluePixelChannel].offset]	= *(p + 3 * tile_image.m_nWidth);
							}
						}
						p++;
						q += 4;
					}
					if (tile_image.m_eStorageClass == DirectClass && bits_per_pixel != 16)
					{
						p += (component_count - 1) * tile_image.m_nWidth;
						if (p < pixels)
							break;
					}
				}

				if (tile_image.m_eStorageClass == PseudoClass ||
					bits_per_pixel == 16 ||
					tile_image.m_eAlphaTrait == UndefinedPixelTrait)
					SetImageAlpha(&tile_image, 255);

				delete[] pixels;
				if (feof(m_pFile) == 0)
					if ((code == 0x9a) || (code == 0x9b) || ((bytes_per_line & 0x8000) != 0))
					{
						if (m_oImgData.m_nHeight <= tile_image.m_nHeight && m_oImgData.m_nWidth <= tile_image.m_nWidth)
						{
							m_oImgData.m_nHeight = tile_image.m_nHeight;
							m_oImgData.m_nWidth = tile_image.m_nWidth;
							m_oImgData.m_pPixelData = (BYTE*)realloc(m_oImgData.m_pPixelData, m_oImgData.m_nHeight * m_oImgData.m_nWidth * 4);
							memcpy(m_oImgData.m_pPixelData, tile_image.m_pPixelData, m_oImgData.m_nHeight * m_oImgData.m_nWidth * 4);
						}
						else
							CompositeImage(tile_image, destination.X, destination.Y);
					}
				break;
			}
			default:
				break;
			}
		}

		if (code == 0xc00)
		{
			for (size_t i = 0; i < 24; i++)
				if (fgetc(m_pFile) == EOF)
					break;
			continue;
		}

		if (((code >= 0xb0) && (code <= 0xcf)) || ((code >= 0x8000) && (code <= 0x80ff)))
			continue;

		if ((code == 0xff) || (code == 0xffff))
			continue;

		if (((code >= 0xd0) && (code <= 0xfe)) || ((code >= 0x8100) && (code <= 0xffff)))
		{
			size_t length = ReadShortValue();
			if (length > GetFileSize())
				return false;

			for (size_t i = 0; i < length; i++)
				if (fgetc(m_pFile) == EOF)
					break;
			continue;
		}

		if ((code >= 0x100) && (code <= 0x7fff))
		{
			size_t length = (size_t) ((code >> 7) & 0xff);
			if (length > GetFileSize())
				return false;

			for (size_t i = 0; i < length; i++)
				if (fgetc(m_pFile) == EOF)
					break;
			continue;
		}
	}

	if (!is_pix_data)
	{
		if (!m_oImgData.m_pPixelData)
			m_oImgData.m_pPixelData	= (BYTE*)malloc(4 * m_oImgData.m_nHeight * m_oImgData.m_nWidth);
			memcpy(m_oImgData.m_pPixelData, m_oFrame.get_Data(), 4 * m_oImgData.m_nHeight * m_oImgData.m_nWidth);
	}

	return true;
}

size_t CPictFile::GetFileSize() const
{
	struct stat st;

	long long file_discription =
#ifdef _WIN32
	_fileno(m_pFile);
#else
	fileno(m_pFile);
#endif
	if (fstat(file_discription, &st) == 0)
		return st.st_size;

	return 0;
}

size_t CPictFile::Read(const size_t& length, void* data)
{
	if (!m_pFile)
		return 0;
	if (data == nullptr)
		return 0;

	unsigned char* q = (unsigned char*) data;
	return fread(q, 1, length, m_pFile);
}

const void* CPictFile::ReadBlobStream(const size_t& length, void* data, size_t* count)
{
	*count = Read(length, (unsigned char*) data);
	return data;
}

unsigned short CPictFile::ReadShortValue()
{
	unsigned char buffer[2];
	size_t count;
	*buffer='\0';

	const unsigned char* p = (const unsigned char*) ReadBlobStream(2, buffer, &count);
	if (count != 2)
		return EOF;

	unsigned short value = (unsigned short) ((*p++) << 8);
	value |= (unsigned short) (*p++);
	return (unsigned short) (value & 0xffff);
}

signed short CPictFile::ReadSignedShortValue()
{
	union
	{
		unsigned short unsigned_value;
		signed short signed_value;
	} Quantum;

	Quantum.unsigned_value = ReadShortValue();
	return (Quantum.signed_value);
}

unsigned int CPictFile::ReadLongValue()
{
	unsigned char buffer[4];
	*buffer='\0';
	int count = Read(4, buffer);

	if (count != 4)
		return EOF;

	const unsigned char* p = (const unsigned char*) buffer;

	unsigned int value = (unsigned int) (*p++) << 24;
	value |= (unsigned int) (*p++) << 16;
	value |= (unsigned int) (*p++) << 8;
	value |= (unsigned int) (*p++);

	return value;
}

bool CPictFile::ReadRectangle(Aggplus::Rect* rect)
{
	rect->Y = ReadShortValue();
	rect->X = ReadShortValue();
	rect->Height = ReadShortValue() - rect->Y;
	rect->Width  = ReadShortValue() - rect->X;

	if (feof(m_pFile) != 0)
		return false;

	if (!rect->IsPositive())
		return false;

	return true;
}

void CPictFile::SetImageAlpha(Image* img, const BYTE alpha)
{
	bool status = true;

	if (!img->m_pPixelData)
		img->m_pPixelData = (BYTE*)malloc(4 * img->m_nHeight * img->m_nWidth);

	img->m_eAlphaTrait = BlendPixelTrait;
	img->m_pChannelMap[AlphaPixelChannel].traits = UpdatePixelTrait;
	for (int y = 0; y < img->m_nHeight; y++)
	{
		if (!status)
			continue;

		unsigned char* q = img->m_pPixelData + 4 * (y * img->m_nWidth);

		if (q == nullptr)
		{
			status = false;
			continue;
		}
		for (int x = 0; x < img->m_nWidth; x++)
		{
			int write_mask;
			if (img->m_pChannelMap[WriteMaskPixelChannel].traits == UndefinedPixelTrait)
				write_mask = 255;
			else
				write_mask = q[img->m_pChannelMap[WriteMaskPixelChannel].offset];

			if (write_mask > (255 / 2))
				if (img->m_pChannelMap[AlphaPixelChannel].traits != UndefinedPixelTrait)
					q[img->m_pChannelMap[AlphaPixelChannel].offset] = alpha;

			q += 4;
		}
	}
}

BYTE* CPictFile::DecodeImage(const Image& img, size_t bytesPerLine, size_t bitsPerPixel, size_t* extent)
{
	bool status = true;

	if (bitsPerPixel <= 8)
		bytesPerLine &= 0x7fff;

	size_t width = img.m_nWidth;
	size_t bytes_per_pixel = 1;

	if (bitsPerPixel == 16)
	{
		bytes_per_pixel = 2;
		width *= 2;
	}
	else if (bitsPerPixel == 32)
		width *= img.m_eAlphaTrait ? 4 : 3;

	if (bytesPerLine == 0)
		bytesPerLine = width;

	size_t row_bytes = (size_t) (img.m_nWidth | 0x8000);

	if (img.m_eStorageClass == DirectClass)
		row_bytes = (size_t) ((4 * img.m_nWidth) | 0x8000);

	BYTE* pixels = new BYTE[img.m_nHeight * row_bytes];

	if (pixels == nullptr)
		return nullptr;

	*extent = row_bytes * img.m_nHeight;
	memset(pixels, 0, *extent);
	BYTE* scanline = new BYTE[row_bytes * 2];

	if (scanline == nullptr)
	{
		delete[] pixels;
		return nullptr;
	}

	memset(scanline, 0, 2 * row_bytes * sizeof(*scanline));

	BYTE unpack_buffer[8 * 256];
	memset(unpack_buffer, 0, sizeof(unpack_buffer));

	if (bytesPerLine < 8)
	{
		for (size_t y = 0; y < img.m_nHeight; y++)
		{
			BYTE*  q = pixels + y * (long long) width * 4;
			int count = Read(bytesPerLine, scanline);
			if (count != bytesPerLine)
			{
				status = false;
				break;
			}

			size_t number_pixels;
			const unsigned char* p = UnpackScanline(scanline, bitsPerPixel, unpack_buffer, &number_pixels);
			if (q + number_pixels > pixels + (*extent))
			{
				status = false;
				break;
			}
			memcpy(q, p, (size_t) number_pixels);
		}
		delete[] scanline;
		if (!status)
			delete[] pixels;
		return pixels;
	}

	for (size_t y = 0; y < img.m_nHeight; y++)
	{
		BYTE* q = pixels + y * width;

		size_t scanline_length;
		if (bytesPerLine > 250)
			scanline_length = ReadShortValue();
		else
			scanline_length = fgetc(m_pFile);

		if ((scanline_length >= row_bytes) || (scanline_length == 0))
		{
			status = false;
			break;
		}

		int count = Read(scanline_length,scanline);
		if (count != scanline_length)
		{
			status = false;
			break;
		}

		for (size_t j = 0; j <  scanline_length; )
			if ((scanline[j] & 0x80) == 0)
			{
				size_t length = (size_t) ((scanline[j] & 0xff) + 1);
				size_t number_pixels = length * bytes_per_pixel;
				const unsigned char* p = UnpackScanline(scanline + j + 1, bitsPerPixel, unpack_buffer, &number_pixels);
				if ((q - pixels + number_pixels) <= *extent)
					memcpy(q, p, number_pixels);
				q += number_pixels;
				j += length * bytes_per_pixel + 1;
			}
			else
			{
				size_t length = ((scanline[j] ^ 0xff) & 0xff) + 2;
				size_t number_pixels = bytes_per_pixel;
				const unsigned char* p = UnpackScanline(scanline + j + 1, bitsPerPixel, unpack_buffer, &number_pixels);
				for (size_t i = 0; i < length; i++)
				{
					if ((q - pixels + number_pixels) <= *extent)
						memcpy(q, p, (size_t) number_pixels);
					q += number_pixels;
				}
				j += bytes_per_pixel + 1;
			}
	}
	delete[] scanline;

	if (!status)
	{
		delete[] pixels;
		pixels = nullptr;
	}

	return pixels;
}

const BYTE* CPictFile::UnpackScanline(const BYTE* pixels, const size_t& bitsPerPixel, BYTE* scanline, size_t* bytesPerLine)
{
	const unsigned char* p = pixels;
	unsigned char* q = scanline;
	switch (bitsPerPixel)
	{
	case 8:
	case 16:
	case 32:
		return pixels;
	case 4:
	{
		for (long long i = 0; i < (long long) *bytesPerLine; i++)
		{
			*q++ = (*p >> 4) & 0xff;
			*q++ = (*p & 15);
			p++;
		}
		*bytesPerLine *= 2;
		break;
	}
	case 2:
	{
		for (long long i = 0; i < (long long) *bytesPerLine; i++)
		{
			*q++ = (*p >> 6) & 0x03;
			*q++ = (*p >> 4) & 0x03;
			*q++ = (*p >> 2) & 0x03;
			*q++ = (*p & 3);
			p++;
		}
		*bytesPerLine *= 4;
		break;
	}
	case 1:
	{
		for (long long i = 0; i < (long long) *bytesPerLine; i++)
		{
			*q++ = (*p >> 7) & 0x01;
			*q++ = (*p >> 6) & 0x01;
			*q++ = (*p >> 5) & 0x01;
			*q++ = (*p >> 4) & 0x01;
			*q++ = (*p >> 3) & 0x01;
			*q++ = (*p >> 2) & 0x01;
			*q++ = (*p >> 1) & 0x01;
			*q++ = (*p & 0x01);
			p++;
		}
		*bytesPerLine *= 8;
		break;
	}
	default:
		break;
	}
	return scanline;
}

void CPictFile::ReadPolygon()
{
	size_t size = ReadShortValue();
	fseek(m_pFile, 8, SEEK_CUR);

	size = (size - 10) / 4;
	const size_t max_possible_points = GetFileSize() - ftell(m_pFile);
	if (size > max_possible_points)
		size = max_possible_points;

	m_arLastPolygon.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		int y = ReadShortValue();
		int x = ReadShortValue();
		m_arLastPolygon[i] = Aggplus::Point(x, y);
		if (feof(m_pFile))
		{
			m_arLastPolygon.resize(i);
			break;
		}
	}
}

Aggplus::Rect CPictFile::ContractRect(const Aggplus::Rect& rect, bool isFrame)
{
	if (!isFrame)
		return rect;

	int pen_size = (m_nPenWidth + m_nPenHeight) / 2;

	if (pen_size * 2 > rect.Width)
		pen_size = (rect.Width + 1) / 2;
	if (pen_size * 2 > rect.Height)
		pen_size = (rect.Height + 1) / 2;

	const long X[2] = {rect.GetLeft() + pen_size / 2, rect.GetRight() - (pen_size + 1) / 2};
	const long Y[2] = {rect.GetTop() + pen_size / 2, rect.GetBottom() - (pen_size + 1) / 2};

	return Aggplus::Rect(X[0], Y[0], X[1] - X[0], Y[1] - Y[0]);
}

void CPictFile::DrawPolygon(bool isFrame)
{
	InitializeRenderer();

	size_t pen_size = (m_nPenWidth + m_nPenHeight) / 2;
	long deca1TL[2] = {0, 0}, deca1BR[2] = {static_cast<long>(m_nPenWidth), static_cast<long>(m_nPenWidth)};

	if (isFrame)
	{
		deca1TL[0] += pen_size / 2;
		deca1TL[1] += pen_size / 2;
		deca1BR[0] -= (pen_size + 1) / 2;
		deca1BR[1] -= (pen_size + 1) / 2;
	}
	else
		deca1BR[0] = deca1BR[1] = 0;

	size_t points_count = m_arLastPolygon.size();
	if (points_count < 1)
		return;

	double bary[2] = {0.0, 0.0};
	for (int i = 0; i < points_count; i++)
	{
		const Aggplus::Point& pt = m_arLastPolygon[i];
		bary[0] += double(pt.X);
		bary[1] += double(pt.Y);
	}
	bary[0] /= double(points_count);
	bary[1] /= double(points_count);


	std::vector<Aggplus::PointF> B2Dpoly;
	B2Dpoly.resize(points_count);
	for (int i = 0; i < points_count; i++)
	{
		const Aggplus::Point& pt = m_arLastPolygon[i];
		double x = (double(pt.X) < bary[0]) ? pt.X + deca1TL[0] : pt.X + deca1BR[0];
		double y = (double(pt.Y) < bary[1]) ? pt.Y + deca1TL[1] : pt.Y + deca1BR[1];
		B2Dpoly[i] = Aggplus::PointF(x, y);
	}

	m_pRenderer->PathCommandStart();
	m_pRenderer->BeginCommand(c_nPathType);
	m_pRenderer->PathCommandMoveTo(B2Dpoly[0].X, B2Dpoly[0].Y);

	for (int i = 1; i < points_count; i++)
		m_pRenderer->PathCommandLineTo(B2Dpoly[i].X, B2Dpoly[i].Y);


	if (!isFrame)
		m_pRenderer->Fill();

	m_pRenderer->DrawPath(c_nStroke);
	m_pRenderer->EndCommand(c_nPathType);
	m_pRenderer->PathCommandEnd();
}

void CPictFile::DrawLine(const Aggplus::Point& p1, const Aggplus::Point& p2)
{
	long dir[2] = {p2.X - p1.X, p2.Y - p1.Y};
	long X[2] = {p1.X, p2.X};
	long Y[2] = {p1.Y, p2.Y};

	if (dir[0] == 0)
	{
		if (X[0] < X[1])
			X[1] += m_nPenWidth;
		else
			X[0] += m_nPenWidth;
		Y[1] += m_nPenHeight;

		std::vector<Aggplus::Point> poly;
		poly.resize(5);
		poly[0] = Aggplus::Point(X[0], Y[0]);
		poly[1] = Aggplus::Point(X[1], Y[0]);
		poly[2] = Aggplus::Point(X[1], Y[1]);
		poly[3] = Aggplus::Point(X[0], Y[1]);
		poly[4] = Aggplus::Point(X[0], Y[0]);
		std::vector<Aggplus::Point> tmp = m_arLastPolygon;
		m_arLastPolygon = poly;
		DrawPolygon(false);
		m_arLastPolygon = tmp;
	}
	else if (dir[1] == 0)
	{
		if (Y[0] < Y[1])
			Y[1] += m_nPenHeight;
		else
			Y[0] += m_nPenHeight;
		X[1] += m_nPenWidth;

		std::vector<Aggplus::Point> poly;
		poly.resize(5);
		poly[0] = Aggplus::Point(X[0], Y[0]);
		poly[1] = Aggplus::Point(X[1], Y[0]);
		poly[2] = Aggplus::Point(X[1], Y[1]);
		poly[3] = Aggplus::Point(X[0], Y[1]);
		poly[4] = Aggplus::Point(X[0], Y[0]);
		std::vector<Aggplus::Point> tmp = m_arLastPolygon;
		m_arLastPolygon = poly;
		DrawPolygon(false);
		m_arLastPolygon = tmp;
	}
	else if (dir[0] * dir[0] + dir[1] * dir[1] < 25)
	{
		long orig_pt[4][2] = {{p1.X, p1.Y},
							  {p1.X + static_cast<long>(m_nPenWidth), p1.Y},
							  {p1.X + static_cast<long>(m_nPenWidth), p1.Y + static_cast<long>(m_nPenHeight)},
							  {p1.X, p1.Y + static_cast<long>(m_nPenHeight)}};
		int orig_avoid = dir[0] > 0 ? (dir[1] > 0 ? 2 : 1) : (dir[1] > 0 ? 3 : 0);
		long dest_pt[4][2] = {{p2.X, p2.Y},
							  {p2.X + static_cast<long>(m_nPenWidth), p2.Y},
							  {p2.X + static_cast<long>(m_nPenWidth), p2.Y + static_cast<long>(m_nPenHeight)},
							  {p2.X, p2.Y + static_cast<long>(m_nPenHeight)}};
		std::vector<Aggplus::Point> poly;
		poly.resize(7);
		for (int w = 1; w <	4; w++)
		{
			int wh = (w + orig_avoid) % 4;
			poly[w - 1] = Aggplus::Point(orig_pt[wh][0], orig_pt[wh][1]);
		}

		for (int w = 3; w < 6; w++)
		{
			int wh = (w + orig_avoid) % 4;
			poly[w] = Aggplus::Point(dest_pt[wh][0], dest_pt[wh][1]);
		}

		int wh = (orig_avoid + 1) % 4;
		poly[6] = Aggplus::Point(orig_pt[wh][0], orig_pt[wh][1]);
		std::vector<Aggplus::Point> tmp = m_arLastPolygon;
		m_arLastPolygon = poly;
		DrawPolygon(false);
		m_arLastPolygon = tmp;
	}
	else
	{
		long decal[2] = {static_cast<long>(m_nPenWidth / 2), static_cast<long>(m_nPenHeight / 2)};
		std::vector<Aggplus::Point> poly;
		poly.resize(2);
		poly[0] = Aggplus::Point(p1.X + decal[0], p1.Y + decal[1]);
		poly[1] = Aggplus::Point(p2.X + decal[0], p2.Y + decal[1]);
		std::vector<Aggplus::Point> tmp = m_arLastPolygon;
		m_arLastPolygon = poly;
		DrawPolygon(false);
		m_arLastPolygon = tmp;
	}
}

void CPictFile::DrawRect(bool isFrame)
{
	InitializeRenderer();

	Aggplus::Rect new_rect = ContractRect(m_oLastRect, isFrame);
	const long X[2] = {new_rect.GetLeft(), new_rect.GetRight()};
	const long Y[2] = {new_rect.GetTop(), new_rect.GetBottom()};

	m_pRenderer->PathCommandStart();
	m_pRenderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddRectangle(X[0], Y[0], X[1] - X[0], Y[1] - Y[0]);
	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	m_pRenderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i++)
		m_pRenderer->PathCommandLineTo(points[i].X, points[i].Y);

	if (!isFrame)
		m_pRenderer->Fill();

	m_pRenderer->DrawPath(c_nStroke);
	m_pRenderer->EndCommand(c_nPathType);
	m_pRenderer->PathCommandEnd();

	delete[] points;
}

void CPictFile::DrawRoundRect(bool isFrame)
{
	InitializeRenderer();

	Aggplus::Rect oval = ContractRect(m_oLastRoundRect, isFrame);
	int ovalW = m_nPenWidth;
	int ovalH = m_nPenHeight;
	const long X[2] = {oval.GetLeft(), oval.GetRight()};
	const long Y[2] = {oval.GetTop(), oval.GetBottom()};
	long width = X[1] - X[0];
	long height = Y[1] - Y[0];

	if (ovalW > width)
		ovalW = static_cast<int>(width);
	if (ovalH > height)
		ovalH = static_cast<int>(height);

	m_pRenderer->PathCommandStart();
	m_pRenderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddRoundRectangle(X[0], Y[0], X[1] - X[0], Y[1] - Y[0], width == 0.0 ? 0.0 : ovalW / width, height == 0.0 ? 0.0 : ovalH / height);
	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	m_pRenderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i++)
	{
		if (path.IsCurvePoint(i))
		{
			m_pRenderer->PathCommandCurveTo(points[i].X, points[i].Y,
											points[i + 1].X, points[i + 1].Y,
											points[i + 2].X, points[i + 2].Y);
			i += 2;
		}
		m_pRenderer->PathCommandLineTo(points[i].X, points[i].Y);
	}


	if (!isFrame)
		m_pRenderer->Fill();

	m_pRenderer->DrawPath(c_nStroke);
	m_pRenderer->EndCommand(c_nPathType);
	m_pRenderer->PathCommandEnd();

	delete[] points;
}

void CPictFile::DrawOval(bool isFrame)
{
	InitializeRenderer();

	Aggplus::Rect rect = ContractRect(m_oLastOval, isFrame);
	const long X[2] = {rect.GetLeft(), rect.GetRight()};
	const long Y[2] = {rect.GetTop(), rect.GetBottom()};

	m_pRenderer->PathCommandStart();
	m_pRenderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddEllipse(X[0], Y[0], X[1] - X[0], Y[1] - Y[0]);
	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	m_pRenderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i += 3)
		m_pRenderer->PathCommandCurveTo(points[i].X, points[i].Y,
										points[i + 1].X, points[i + 1].Y,
										points[i + 2].X, points[i + 2].Y);

	if (!isFrame)
		m_pRenderer->Fill();

	m_pRenderer->DrawPath(c_nStroke);
	m_pRenderer->EndCommand(c_nPathType);
	m_pRenderer->PathCommandEnd();

	delete[] points;
}

void CPictFile::DrawArc()
{
	InitializeRenderer();

	short start_angle = ReadShortValue();
	short angle = ReadShortValue();
	if (feof(m_pFile) || (m_nPenWidth == 0 && m_nPenHeight == 0))
		return;

	if (angle < 0)
	{
		start_angle = start_angle + angle;
		angle = -angle;
	}

	double ang1 = agg::deg2rad(start_angle);
	double ang2 = agg::deg2rad(angle);

	m_pRenderer->PathCommandStart();
	m_pRenderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddArc(m_oLastArc.GetLeft(), m_oLastArc.GetTop(), m_oLastArc.Width, m_oLastArc.Height, ang1, ang2);

	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	m_pRenderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i += 3)
		m_pRenderer->PathCommandCurveTo(points[i].X, points[i].Y,
										points[i + 1].X, points[i + 1].Y,
										points[i + 2].X, points[i + 2].Y);

	m_pRenderer->DrawPath(c_nStroke);
	m_pRenderer->EndCommand(c_nPathType);
	m_pRenderer->PathCommandEnd();

	delete[] points;
}

void CPictFile::ReadAndDrawText(int x, int y)
{
	InitializeRenderer();

	char text[256];
	char byte_len = fgetc(m_pFile);
	unsigned long len = static_cast<unsigned long>(byte_len) & 0x000000ff;
	len = Read(len, &text);

	if (m_nPenWidth == 0 && m_nPenHeight == 0)
		return;

	while (len > 0 && static_cast<unsigned char>(text[len - 1]) < 32)
		len--;
	text[len] = 0;
	m_pRenderer->put_FontName(m_wsFontName);
	m_pRenderer->put_FontSize(m_nFontSize);
	m_pRenderer->put_FontStyle(m_nFontStyle);

	std::wstring ws_text(len, L'\0');
	std::mbstowcs(&ws_text[0], text, len);

	m_pRenderer->BeginCommand(c_nTextGraphicType);
	m_pRenderer->CommandDrawText(ws_text, x, y, 0.0, 0.0);
	m_pRenderer->EndCommand(c_nTextGraphicType);
}

void CPictFile::InitializeRenderer()
{
	if (m_pFrameData)
		return;

	m_pFrameData = new BYTE[4 * m_oImgData.m_nWidth * m_oImgData.m_nHeight];
	m_oFrame.put_Data(m_pFrameData);
	m_oFrame.put_Width(m_oImgData.m_nWidth);
	m_oFrame.put_Height(m_oImgData.m_nHeight);
	m_oFrame.put_Stride(4 * m_oImgData.m_nWidth);

	m_pRenderer->CreateFromBgraFrame(&m_oFrame);
	m_pRenderer->put_Width(m_oImgData.m_nWidth);
	m_pRenderer->put_Height(m_oImgData.m_nHeight);
	m_pRenderer->SetSwapRGB(false);
	m_pRenderer->put_PenColor(0x000000);
}


BYTE* CPictFile::GetPixels(const Image& image, const long long& x, const long long& y, const size_t& width, const size_t& height) const
{
	BYTE* pixels = image.m_pPixelData + 4 * (y * image.m_nWidth + x);

	if (!pixels)
		return nullptr;

	if (image.m_nWidth == 0)
		return nullptr;

	long long offset = y * image.m_nWidth;

	offset += x;
	size_t length =  4 * width * sizeof(unsigned char);

	size_t rows = height;
	size_t extent = length * rows;

	BYTE* q = pixels;

	if (image.m_nWidth == width)
	{
		length = extent;
		rows = 1UL;
	}

	BYTE* p = image.m_pPixelData + 4 * offset;
	for (size_t i = 0; i < rows; i++)
	{
		memcpy(q, p, length);
		p += 4 * image.m_nWidth;
		q += 4 * width;
	}

	return pixels;
}

void CPictFile::CompositeImage(const Image& composite,const long long& xOffset, const long long& yOffset)
{
	m_oImgData.m_eStorageClass = DirectClass;
	Image source_image(composite);
	source_image.m_pPixelData = (BYTE*)malloc(source_image.m_nWidth * source_image.m_nHeight * 4);
	memcpy(source_image.m_pPixelData, composite.m_pPixelData, source_image.m_nWidth * source_image.m_nHeight * 4);

	m_oImgData.m_pChannelMap[RedPixelChannel].traits = UpdatePixelTrait;
	m_oImgData.m_pChannelMap[GreenPixelChannel].traits = UpdatePixelTrait;
	m_oImgData.m_pChannelMap[BluePixelChannel].traits = UpdatePixelTrait;
	m_oImgData.m_pChannelMap[AlphaPixelChannel].traits = UpdatePixelTrait;

	source_image.m_pChannelMap[RedPixelChannel].traits = CopyPixelTrait;
	source_image.m_pChannelMap[GreenPixelChannel].traits = CopyPixelTrait;
	source_image.m_pChannelMap[BluePixelChannel].traits = CopyPixelTrait;
	source_image.m_pChannelMap[AlphaPixelChannel].traits = CopyPixelTrait;

	bool status = false;
	if (!((xOffset < 0) || (yOffset < 0)) && !((xOffset + source_image.m_nWidth) > m_oImgData.m_nWidth) && !((yOffset + source_image.m_nHeight) > m_oImgData.m_nHeight))
	{
		if ((source_image.m_eAlphaTrait == UndefinedPixelTrait) &&
			(m_oImgData.m_eAlphaTrait != UndefinedPixelTrait))
			SetImageAlpha(&source_image, 255);

		status = true;
		for (size_t y = 0; y < source_image.m_nHeight; y++)
		{
			if (!status)
				continue;

			const BYTE* p = GetPixels(source_image,0,y,source_image.m_nWidth, 1);
			BYTE* q = GetPixels(m_oImgData, xOffset, y + yOffset, source_image.m_nWidth, 1);

			if (p == nullptr || q == nullptr)
			{
				status = false;
				continue;
			}

			for (size_t x = 0; x < source_image.m_nWidth; x++)
			{
				BYTE read_mask;
				if (source_image.m_pChannelMap[ReadMaskPixelChannel].traits == UndefinedPixelTrait)
					read_mask = 255;
				else
					read_mask = p[source_image.m_pChannelMap[ReadMaskPixelChannel].offset];

				if (read_mask <= (255.0 / 2.0))
				{
					p += 4;
					q += 4;
					continue;
				}

				for (int i = 0; i < 4; i++)
				{
					PixelChannel channel = source_image.m_pChannelMap[i].channel;
					PixelTrait source_traits = source_image.m_pChannelMap[channel].traits;
					PixelTrait traits = m_oImgData.m_pChannelMap[channel].traits;

					if ((source_traits == UndefinedPixelTrait) || (traits == UndefinedPixelTrait))
						continue;
					q[m_oImgData.m_pChannelMap[channel].offset] = p[i];
				}
				p += 4;
				q += 4;
			}
		}

		return;
	}

	status = true;
	for (size_t y = 0; y < m_oImgData.m_nHeight; y++)
	{
		if (!status)
			continue;

		if (y < yOffset)
			continue;
		if (y - yOffset >= source_image.m_nHeight)
			continue;

		const BYTE* pixels = nullptr;
		const BYTE* p = nullptr;
		if (y >= yOffset && y - yOffset < source_image.m_nHeight)
		{
			p = GetPixels(source_image, 0, y - yOffset, source_image.m_nWidth,1);
			if (!p)
			{
				status = false;
				continue;
			}
			pixels = p;
			if (xOffset < 0)
				p -= xOffset * 4;
		}

		BYTE* q = GetPixels(m_oImgData, 0, y, m_oImgData.m_nWidth,1);
		if (!q)
		{
			status = false;
			continue;
		}

		for (size_t x = 0; x < m_oImgData.m_nWidth; x++)
		{
			double
				Dc = 0.0,
				Sa = 0.0,
				Sc = 0.0;

			if (x < xOffset)
			{
				q += 4;
				continue;
			}
			if (x - xOffset >= source_image.m_nWidth)
				break;

			if (!pixels || x < xOffset || x - xOffset >= source_image.m_nWidth)
			{
				for (int i = 0; i < 4; i++)
				{
					PixelChannel channel = m_oImgData.m_pChannelMap[i].channel;
					PixelTrait traits = m_oImgData.m_pChannelMap[channel].traits;
					PixelTrait source_traits = source_image.m_pChannelMap[channel].traits;
					if (traits == UndefinedPixelTrait || source_traits == UndefinedPixelTrait)
						continue;

					q[i] = 0;
				}
				q += 4;
				continue;
			}

			BYTE pixel_alpha = 0;
			if (source_image.m_pChannelMap[AlphaPixelChannel].traits != UndefinedPixelTrait)
				pixel_alpha = p[source_image.m_pChannelMap[AlphaPixelChannel].offset];
			Sa = (1.0 / 255.0) *  pixel_alpha;
			Sa = Sa == 0 ? 1 : Sa;
			pixel_alpha = 0;
			if (m_oImgData.m_pChannelMap[AlphaPixelChannel].traits != UndefinedPixelTrait)
				pixel_alpha = q[m_oImgData.m_pChannelMap[AlphaPixelChannel].offset];

			for (int i = 0; i < 4; i++)
			{
				double pixel = 0.0;
				PixelChannel channel = m_oImgData.m_pChannelMap[i].channel;
				PixelTrait traits = m_oImgData.m_pChannelMap[channel].traits;
				PixelTrait source_traits = source_image.m_pChannelMap[channel].traits;
				if (traits == UndefinedPixelTrait)
					continue;
				if ((channel == AlphaPixelChannel) &&
					((traits & UpdatePixelTrait) != 0))
				{
					pixel = 255.0 * Sa;
					q[i] = pixel > 255 ? 255 : pixel < 0 ? 0 : pixel;
					continue;
				}
				if (source_traits == UndefinedPixelTrait)
					continue;

				Sc = p[source_image.m_pChannelMap[channel].offset];
				Dc=q[i];

				if ((traits & CopyPixelTrait) != 0)
				{
					q[i] = Dc > 255 ? 255 : Dc < 0 ? 0 : Dc;
					continue;
				}

				pixel = 255.0 * (1.0 / 255.0) * Sa * Sc;
				q[i] = pixel > 255 ? 255 : pixel < 0 ? 0 : pixel;
			}

			p += 4;
			if (p >= (pixels + 4 * source_image.m_nWidth))
				p = pixels;
			q += 4;
		}
	}

	return;
}
