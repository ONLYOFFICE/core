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
#ifndef PICFILE_H
#define PICFILE_H

#include "../../graphics/pro/Graphics.h"

enum PixelTrait
{
	UndefinedPixelTrait = 0x000000,
	CopyPixelTrait = 0x000001,
	UpdatePixelTrait = 0x000002,
	BlendPixelTrait = 0x000004
};

enum ClassType
{
	UndefinedClass,
	DirectClass,
	PseudoClass
};

enum PixelChannel
{
	UndefinedPixelChannel = 0,
	RedPixelChannel = 0,
	CyanPixelChannel = 0,
	GrayPixelChannel = 0,
	LPixelChannel = 0,
	LabelPixelChannel = 0,
	YPixelChannel = 0,
	aPixelChannel = 1,
	GreenPixelChannel = 1,
	MagentaPixelChannel = 1,
	CbPixelChannel = 1,
	bPixelChannel = 2,
	BluePixelChannel = 2,
	YellowPixelChannel = 2,
	CrPixelChannel = 2,
	BlackPixelChannel = 3,
	AlphaPixelChannel = 4,
	IndexPixelChannel = 5,
	ReadMaskPixelChannel = 6,
	WriteMaskPixelChannel = 7,
	MetaPixelChannel = 8, /* deprecated */
	CompositeMaskPixelChannel = 9,
	MetaPixelChannels = 10,
	IntensityPixelChannel = 64,  /* ???? */
	CompositePixelChannel = 64,  /* ???? */
	SyncPixelChannel = 65      /* not a real channel */
};

struct PixelChannelMap
{
	PixelChannel channel{UndefinedPixelChannel};
	PixelTrait	 traits{UndefinedPixelTrait};
	size_t		 offset{0};
};

struct PixelInfo
{
	ClassType storage_class{DirectClass};

	PixelTrait alpha_trait{UndefinedPixelTrait};

	double red{0.0};
	double green{0.0};
	double blue{0.0};
	double alpha{0.0};
	double index{0.0};
};

struct Image
{
	ClassType m_eStorageClass{DirectClass};
	PixelTrait m_eAlphaTrait{UndefinedPixelTrait};

	PixelInfo* m_pColormap{nullptr};
	PixelChannelMap* m_pChannelMap{nullptr};

	size_t m_nColors{0};
	size_t m_nHeight{0};
	size_t m_nWidth{0};
	BYTE* m_pPixelData{nullptr};

	Image()
	{
		m_pChannelMap = new PixelChannelMap[65];
		for (size_t i=0; i <= 64; i++)
			m_pChannelMap[i].channel=(PixelChannel) i;

		m_pChannelMap[RedPixelChannel].offset = 0;
		m_pChannelMap[GreenPixelChannel].offset = 1;
		m_pChannelMap[BluePixelChannel].offset = 2;
		m_pChannelMap[AlphaPixelChannel].offset = 3;
		m_pChannelMap[IndexPixelChannel].offset = 5;
	}

	Image(const Image& other) : Image()
	{
		*this = other;
	}

	~Image()
	{
		if (m_pChannelMap)
			delete[] m_pChannelMap;

		if (m_pColormap)
			delete[] m_pColormap;

		if (m_pPixelData)
			free(m_pPixelData);
	}

	Image& operator=(const Image& other)
	{
		if (this == &other)
			return *this;

		m_eStorageClass	= other.m_eStorageClass;
		m_eAlphaTrait	= other.m_eAlphaTrait;

		m_nColors	= other.m_nColors;
		m_nWidth	= other.m_nWidth;
		m_nHeight	= other.m_nHeight;

		if (other.m_pColormap)
		{
			m_pColormap = new PixelInfo[m_nColors + 1];
			memcpy(m_pColormap, other.m_pColormap, m_nColors + 1);
		}

		memcpy(m_pChannelMap, other.m_pChannelMap, 65);
		return *this;
	}
};

class CPictFile
{
public:
	CPictFile();
	~CPictFile();

	bool Open(CBgraFrame* frame, const std::wstring& fileName, bool isRGB);
	bool Open(CBgraFrame* frame, BYTE* buffer, const size_t& size, bool isRGB);
private:
	bool Decode();
	bool DecodeHeader();
	bool DecodeData();

	size_t GetFileSize() const;

	size_t Read(const size_t& length, void* data);
	const void* ReadBlobStream(const size_t& length, void* data, size_t* count);
	unsigned short ReadShortValue();
	signed short ReadSignedShortValue();
	unsigned int ReadLongValue();
	bool ReadRectangle(Aggplus::Rect* rect);

	void SetImageAlpha(Image* img, const BYTE alpha);
	BYTE* DecodeImage(const Image& img, size_t bytesPerLine, size_t bitsPerPixel, size_t* extent);
	const BYTE* UnpackScanline(const BYTE* pixels, const size_t& bitsPerPixel, BYTE* scanline, size_t* bytesPerLine);
	BYTE* GetPixels(const Image& image, const long long& x, const long long& y, const size_t& width, const size_t& height) const;
	void CompositeImage(const Image& composite,const long long& xOffset, const long long& yOffset);

	void ReadPolygon();
	Aggplus::Rect ContractRect(const Aggplus::Rect& rect, bool isFrame);
	void DrawPolygon(bool isFrame);
	void DrawLine(const Aggplus::Point& p1, const Aggplus::Point& p2);
	void DrawRect(bool isFrame);
	void DrawRoundRect(bool isFrame);
	void DrawOval(bool isFrame);
	void DrawArc();
	void ReadAndDrawText(int x, int y);

	void InitializeRenderer();

private:
	int m_nVersion{0};

	FILE* m_pFile{nullptr};
	Image m_oImgData{};
	CBgraFrame m_oFrame{};
	BYTE* m_pFrameData{nullptr};

	size_t m_nPenHeight{0};
	size_t m_nPenWidth{0};

	int m_nFontStyle{0};
	int m_nFontSize{0};

	std::wstring m_wsFontName{};

	Aggplus::Point m_oPenPoint{};
	Aggplus::Point m_oTextPoint{};

	Aggplus::Rect m_oLastRect{};
	Aggplus::Rect m_oLastRoundRect{};
	Aggplus::Rect m_oLastOval{};
	Aggplus::Rect m_oLastArc{};
	std::vector<Aggplus::Point> m_arLastPolygon{};

	NSGraphics::IGraphicsRenderer* m_pRenderer{nullptr};
	NSFonts::IFontManager* m_pFontManager{nullptr};
};

#endif // PICFILE_H
