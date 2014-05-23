#ifndef _FONT_DICTIONARY_H

#define FONTS_DICT_ASCII_NAMES_COUNT 2
static const char* FD_Ascii_Names[FONTS_DICT_ASCII_NAMES_COUNT] = 
{
	"Arial",
	"Times New Roman"
};

static const int FD_Ascii_Names_Offsets[256] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define FONTS_DICT_UNICODE_NAMES_COUNT 1
static const char* FD_Unicode_Names[FONTS_DICT_UNICODE_NAMES_COUNT] = 
{
	"Arial"
};

typedef struct FD_FontInfo_Rec
{
	const char*		m_name;
	long			m_lIndex;

	unsigned char	m_bBold;
	unsigned char	m_bItalic;
	unsigned char	m_bIsFixed;

	unsigned char	m_aPanose[10];
	unsigned long	m_ulUnicodeRange1;
	unsigned long	m_ulUnicodeRange2;
	unsigned long	m_ulUnicodeRange3;
	unsigned long	m_ulUnicodeRange4;

	unsigned long	m_ulCodePageRange1;
	unsigned long	m_ulCodePageRange2;

	unsigned short	m_usWeigth;
	unsigned short	m_usWidth;

	short			m_sFamilyClass;
	unsigned char	m_eFontFormat;

	short			m_shAvgCharWidth;
	short			m_shAscent;
	short			m_shDescent;
	short			m_shLineGap;
	short			m_shXHeight;
	short			m_shCapHeight;
} FD_FontInfo;

static const FD_FontInfo FD_Ascii_Infos[FONTS_DICT_ASCII_NAMES_COUNT] = 
{
	{"Arial", 0, 0, 0, 0, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 }, 1, 2, 3, 4, 1, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
	{"Times New Roman", 0, 0, 0, 0, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 }, 1, 2, 3, 4, 1, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }
};

#endif /* _FONT_DICTIONARY_H */