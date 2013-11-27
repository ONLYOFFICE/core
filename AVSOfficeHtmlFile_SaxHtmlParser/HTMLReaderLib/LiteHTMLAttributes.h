#pragma once

//#include "LiteHTMLCommon.h"
#include "LiteHTMLEntityResolver.h"

#pragma warning(push, 4)
#pragma warning (disable : 4290)	// C++ Exception Specification ignored
#pragma warning (disable : 4127)	// Disable warning C4127: conditional expression is constant
#pragma warning (disable : 4239)	// Disable warning C4239: nonstandard extension used : 'initializing' : conversion from 'CLiteHTMLAttributes' to 'CLiteHTMLAttributes &'



class CLiteHTMLAttributes;	// forward declaration

/**
 * CLiteHTMLElemAttr
 */
class CLiteHTMLElemAttr
{
	// Friends
	friend class CLiteHTMLAttributes;

// Constructors
public:
	CLiteHTMLElemAttr(LPCTSTR lpszAttribName = NULL, 
					  LPCTSTR lpszAttribValue = NULL)
	{
		Init();
		m_strAttrName = lpszAttribName;
		m_strAttrValue = lpszAttribValue;
	}

	CLiteHTMLElemAttr(const CLiteHTMLElemAttr &rSource)
	{
		Init();
		m_strAttrName = rSource.m_strAttrName;
		m_strAttrValue = rSource.m_strAttrValue;
	}

// Initialization Helpers
private:
	static void Init(void)
	{
		if (_namedColors.GetCount())
			return;

		/** 28 system colors */
		_namedColors[_T("activeborder")]		= (COLORREF)0x8000000A;
		_namedColors[_T("activecaption")]		= (COLORREF)0x80000002;
		_namedColors[_T("appworkspace")]		= (COLORREF)0x8000000C;
		_namedColors[_T("background")]			= (COLORREF)0x80000001;
		_namedColors[_T("buttonface")]			= (COLORREF)0x8000000F;
		_namedColors[_T("buttonhighlight")]		= (COLORREF)0x80000014;
		_namedColors[_T("buttonshadow")]		= (COLORREF)0x80000010;
		_namedColors[_T("buttontext")]			= (COLORREF)0x80000012;
		_namedColors[_T("captiontext")]			= (COLORREF)0x80000009;
		_namedColors[_T("graytext")]			= (COLORREF)0x80000011;
		_namedColors[_T("highlight")]			= (COLORREF)0x8000000D;
		_namedColors[_T("highlighttext")]		= (COLORREF)0x8000000E;
		_namedColors[_T("inactiveborder")]		= (COLORREF)0x8000000B;
		_namedColors[_T("inactivecaption")]		= (COLORREF)0x80000003;
		_namedColors[_T("inactivecaptiontext")]	= (COLORREF)0x80000013;
		_namedColors[_T("infobackground")]		= (COLORREF)0x80000018;
		_namedColors[_T("infotext")]			= (COLORREF)0x80000017;
		_namedColors[_T("menu")]				= (COLORREF)0x80000004;
		_namedColors[_T("menutext")]			= (COLORREF)0x80000007;
		_namedColors[_T("scrollbar")]			= (COLORREF)0x80000000;
		_namedColors[_T("threeddarkshadow")]	= (COLORREF)0x80000015;
		_namedColors[_T("threedface")]			= (COLORREF)0x8000000F;
		_namedColors[_T("threedhighlight")]		= (COLORREF)0x80000014;
		_namedColors[_T("threedlightshadow")]	= (COLORREF)0x80000016;
		_namedColors[_T("threedshadow")]		= (COLORREF)0x80000010;
		_namedColors[_T("window")]				= (COLORREF)0x80000005;
		_namedColors[_T("windowframe")]			= (COLORREF)0x80000006;
		_namedColors[_T("windowtext")]			= (COLORREF)0x80000008;

		/** 16 basic colors */
		_namedColors[_T("black")]				= RGB(0x00, 0x00, 0x00);
		_namedColors[_T("gray")]				= RGB(0x80, 0x80, 0x80);
		_namedColors[_T("silver")]				= RGB(0xC0, 0xC0, 0xC0);
		_namedColors[_T("white")]				= RGB(0xFF, 0xFF, 0xFF);
		_namedColors[_T("yellow")]				= RGB(0xFF, 0xFF, 0x00);
		_namedColors[_T("olive")]				= RGB(0x80, 0x80, 0x00);
		_namedColors[_T("red")]					= RGB(0xFF, 0x00, 0x00);
		_namedColors[_T("maroon")]				= RGB(0x80, 0x00, 0x00);
		_namedColors[_T("fuchsia")]				= RGB(0xFF, 0x00, 0xFF);
		_namedColors[_T("purple")]				= RGB(0x80, 0x00, 0x80);
		_namedColors[_T("blue")]				= RGB(0x00, 0x00, 0xFF);
		_namedColors[_T("navy")]				= RGB(0x00, 0x00, 0x80);
		_namedColors[_T("aqua")]				= RGB(0x00, 0xFF, 0xFF);
		_namedColors[_T("teal")]				= RGB(0x00, 0x80, 0x80);
		_namedColors[_T("lime")]				= RGB(0x00, 0xFF, 0x00);
		_namedColors[_T("green")]				= RGB(0x00, 0x80, 0xFF);

		/** additional named colors */
		_namedColors[_T("darkolivegreen")]		= RGB(0x55, 0x6B, 0x2F);
		_namedColors[_T("olivedrab")]			= RGB(0x6B, 0x8E, 0x23);
		_namedColors[_T("yellowgreen")]			= RGB(0x9A, 0xCD, 0x32);
		_namedColors[_T("lawngreen")]			= RGB(0x7C, 0xFC, 0x00);
		_namedColors[_T("chartreuse")]			= RGB(0x7F, 0xFF, 0x00);
		_namedColors[_T("greenyellow")]			= RGB(0xAD, 0xFF, 0x2F);
		_namedColors[_T("palegreen")]			= RGB(0x98, 0xFB, 0x98);
		_namedColors[_T("lightgreen")]			= RGB(0x90, 0xEE, 0x90);
		_namedColors[_T("darkgreen")]			= RGB(0x00, 0x64, 0x00);
		_namedColors[_T("forestgreen")]			= RGB(0x22, 0x8B, 0x22);
		_namedColors[_T("seagreen")]			= RGB(0x2E, 0x8B, 0x57);
		_namedColors[_T("mediumseagreen")]		= RGB(0x3C, 0xB3, 0x71);
		_namedColors[_T("limegreen")]			= RGB(0x32, 0xCD, 0x32);
		_namedColors[_T("darkseagreen")]		= RGB(0x8F, 0xBC, 0x8B);
		_namedColors[_T("springgreen")]			= RGB(0x00, 0xFF, 0x7F);
		_namedColors[_T("mediumspringgreen")]	= RGB(0x00, 0xFA, 0x99);
		_namedColors[_T("darkslategray")]		= RGB(0x2F, 0x4F, 0x4F);
		_namedColors[_T("darkcyan")]			= RGB(0x00, 0x8B, 0x8B);
		_namedColors[_T("cadetblue")]			= RGB(0x5F, 0x9E, 0xA0);
		_namedColors[_T("lightseagreen")]		= RGB(0x20, 0xB2, 0xAA);
		_namedColors[_T("mediumaquamarine")]	= RGB(0x66, 0xCD, 0xAA);
		_namedColors[_T("turquoise")]			= RGB(0x40, 0xE0, 0xD0);
		_namedColors[_T("aquamarine")]			= RGB(0x7F, 0xFF, 0xD4);
		_namedColors[_T("paleturquoise")]		= RGB(0xAF, 0xEE, 0xEE);
		_namedColors[_T("slategray")]			= RGB(0x70, 0x80, 0x90);
		_namedColors[_T("lightslategray")]		= RGB(0x77, 0x88, 0x99);
		_namedColors[_T("steelblue")]			= RGB(0x46, 0x82, 0xB4);
		_namedColors[_T("deepskyblue")]			= RGB(0x00, 0xBF, 0xFF);
		_namedColors[_T("darkturquoise")]		= RGB(0x00, 0xCE, 0xD1);
		_namedColors[_T("mediumturquoise")]		= RGB(0x48, 0xD1, 0xCC);
		_namedColors[_T("powderblue")]			= RGB(0xB0, 0xE0, 0xE6);
		_namedColors[_T("lightcyan")]			= RGB(0xE0, 0xFF, 0xFF);
		_namedColors[_T("darkblue")]			= RGB(0x00, 0x00, 0x8B);
		_namedColors[_T("mediumblue")]			= RGB(0x00, 0x00, 0xCD);
		_namedColors[_T("royalblue")]			= RGB(0x41, 0x69, 0xe1);
		_namedColors[_T("dodgerblue")]			= RGB(0x1E, 0x90, 0xFF);
		_namedColors[_T("cornflowerblue")]		= RGB(0x64, 0x95, 0xED);
		_namedColors[_T("skyblue")]				= RGB(0x87, 0xCE, 0xEB);
		_namedColors[_T("lightskyblue")]		= RGB(0x87, 0xCE, 0xFA);
		_namedColors[_T("lightblue")]			= RGB(0xAD, 0xD8, 0xE6);
		_namedColors[_T("midnightblue")]		= RGB(0x19, 0x19, 0x70);
		_namedColors[_T("darkslateblue")]		= RGB(0x48, 0x3D, 0x8B);
		_namedColors[_T("blueviolet")]			= RGB(0x8A, 0x2B, 0xE2);
		_namedColors[_T("slateblue")]			= RGB(0x6A, 0x5A, 0xCD);
		_namedColors[_T("mediumslateblue")]		= RGB(0x7B, 0x68, 0xEE);
		_namedColors[_T("mediumpurple")]		= RGB(0x93, 0x70, 0xDB);
		_namedColors[_T("lightsteelblue")]		= RGB(0xB0, 0xC4, 0xDE);
		_namedColors[_T("lavender")]			= RGB(0xE6, 0xE6, 0xFA);
		_namedColors[_T("indigo")]				= RGB(0x4B, 0x00, 0x82);
		_namedColors[_T("darkviolet")]			= RGB(0x94, 0x00, 0xD3);
		_namedColors[_T("darkorchid")]			= RGB(0x99, 0x32, 0xCC);
		_namedColors[_T("mediumorchid")]		= RGB(0xBA, 0x55, 0xD3);
		_namedColors[_T("orchid")]				= RGB(0xDA, 0x70, 0xD6);
		_namedColors[_T("violet")]				= RGB(0xEE, 0x82, 0xEE);
		_namedColors[_T("plum")]				= RGB(0xDD, 0xA0, 0xDD);
		_namedColors[_T("thistle")]				= RGB(0xD8, 0xDF, 0xD8);
		_namedColors[_T("darkmagenta")]			= RGB(0x8B, 0x00, 0x8B);
		_namedColors[_T("mediumvioletred")]		= RGB(0xC7, 0x15, 0x85);
		_namedColors[_T("deeppink")]			= RGB(0xFF, 0x14, 0x93);
		_namedColors[_T("palmvioletred")]		= RGB(0xDB, 0x70, 0x93);
		_namedColors[_T("hotpink")]				= RGB(0xFF, 0x69, 0xB4);
		_namedColors[_T("lightpink")]			= RGB(0xFF, 0xB6, 0xC1);
		_namedColors[_T("pink")]				= RGB(0xFF, 0xC0, 0xCB);
		_namedColors[_T("mistyrose")]			= RGB(0xFF, 0xE4, 0xE1);
		_namedColors[_T("brown")]				= RGB(0xA5, 0x2A, 0x2A);
		_namedColors[_T("indianred")]			= RGB(0xCD, 0x5C, 0x5C);
		_namedColors[_T("rosybrown")]			= RGB(0xBC, 0x8F, 0x8F);
		_namedColors[_T("salmon")]				= RGB(0xFA, 0x80, 0x72);
		_namedColors[_T("lightcoral")]			= RGB(0xF0, 0x80, 0x80);
		_namedColors[_T("darksalmon")]			= RGB(0xE9, 0x96, 0x7A);
		_namedColors[_T("lightsalmon")]			= RGB(0xFF, 0xA0, 0x7A);
		_namedColors[_T("peachpuff")]			= RGB(0xFF, 0xDA, 0xB9);
		_namedColors[_T("darkred")]				= RGB(0x8B, 0x00, 0x00);
		_namedColors[_T("firebrick")]			= RGB(0xB2, 0x22, 0x22);
		_namedColors[_T("crimson")]				= RGB(0xDC, 0x14, 0x3C);
		_namedColors[_T("orangered")]			= RGB(0xFF, 0x45, 0x00);
		_namedColors[_T("tomato")]				= RGB(0xFF, 0x63, 0x47);
		_namedColors[_T("coral")]				= RGB(0xFF, 0x7F, 0x50);
		_namedColors[_T("wheat")]				= RGB(0xF5, 0xDE, 0xB3);
		_namedColors[_T("papayawhip")]			= RGB(0xFF, 0xEF, 0xD5);
		_namedColors[_T("sienna")]				= RGB(0xA0, 0x52, 0x2D);
		_namedColors[_T("chocolate")]			= RGB(0xD2, 0x69, 0x1E);
		_namedColors[_T("darkorange")]			= RGB(0xFF, 0x8C, 0x00);
		_namedColors[_T("sandybrown")]			= RGB(0xF4, 0xA4, 0x60);
		_namedColors[_T("orange")]				= RGB(0xFF, 0xA5, 0x00);
		_namedColors[_T("navajowhite")]			= RGB(0xFF, 0xDE, 0xAD);
		_namedColors[_T("moccasin")]			= RGB(0xFF, 0xE4, 0xB5);
		_namedColors[_T("saddlebrown")]			= RGB(0x8B, 0x45, 0x13);
		_namedColors[_T("peru")]				= RGB(0xCD, 0x85, 0x3F);
		_namedColors[_T("burlywood")]			= RGB(0xDE, 0xB8, 0x87);
		_namedColors[_T("tan")]					= RGB(0xD2, 0xB4, 0x8C);
		_namedColors[_T("bisque")]				= RGB(0xFF, 0xE4, 0xC4);
		_namedColors[_T("blanchedalmond")]		= RGB(0xFF, 0xEB, 0xCD);
		_namedColors[_T("antiquewhite")]		= RGB(0xFA, 0xEB, 0xD7);
		_namedColors[_T("darkgoldenrod")]		= RGB(0xB8, 0x86, 0x0B);
		_namedColors[_T("goldenrod")]			= RGB(0xDA, 0xA5, 0x20);
		_namedColors[_T("darkkhaki")]			= RGB(0xBD, 0xB7, 0x6B);
		_namedColors[_T("gold")]				= RGB(0xFF, 0xD7, 0x00);
		_namedColors[_T("khaki")]				= RGB(0xF0, 0xE6, 0x8C);
		_namedColors[_T("palegoldenrod")]		= RGB(0xEE, 0xE8, 0xAA);
		_namedColors[_T("lemonchiffon")]		= RGB(0xFF, 0xFA, 0xCD);
		_namedColors[_T("beige")]				= RGB(0xF5, 0xF5, 0xDC);
		_namedColors[_T("lightgoldenrodyellow")]= RGB(0xFA, 0xFA, 0xD2);
		_namedColors[_T("lightyellow")]			= RGB(0xFF, 0xFF, 0xE0);
		_namedColors[_T("ivory")]				= RGB(0xFF, 0xFF, 0x00);
		_namedColors[_T("cornsilk")]			= RGB(0xFF, 0xF8, 0xDC);
		_namedColors[_T("oldlace")]				= RGB(0xFD, 0xF5, 0xE6);
		_namedColors[_T("florawhite")]			= RGB(0xFF, 0xFA, 0xF0);
		_namedColors[_T("honeydew")]			= RGB(0xF0, 0xFF, 0xF0);
		_namedColors[_T("mintcream")]			= RGB(0xF5, 0xFF, 0xFA);
		_namedColors[_T("azure")]				= RGB(0xF0, 0xFF, 0xFF);
		_namedColors[_T("ghostwhite")]			= RGB(0xF8, 0xF8, 0xFF);
		_namedColors[_T("linen")]				= RGB(0xFA, 0xF0, 0xE6);
		_namedColors[_T("seashell")]			= RGB(0xFF, 0xF5, 0xEE);
		_namedColors[_T("snow")]				= RGB(0xFF, 0xFA, 0xFA);
		_namedColors[_T("dimgray")]				= RGB(0x69, 0x69, 0x69);
		_namedColors[_T("darkgray")]			= RGB(0xA9, 0xA9, 0xA9);
		_namedColors[_T("lightgray")]			= RGB(0xD3, 0xD3, 0xD3);
		_namedColors[_T("gainsboro")]			= RGB(0xDC, 0xDC, 0xDC);
		_namedColors[_T("whitesmoke")]			= RGB(0xF5, 0xF5, 0xF5);
		_namedColors[_T("ghostwhite")]			= RGB(0xF8, 0xF8, 0xFF);
		_namedColors[_T("aliceblue")]			= RGB(0xF0, 0xF8, 0xFF);
	}

// Attributes
public:
	/**
	 * @return name of this CLiteHTMLElemAttr
	 */
	CString getName(void) const
		{ return (m_strAttrName); }

	/**
	 * @return value of this CLiteHTMLElemAttr
	 */
	CString getValue(void) const
		{ return (m_strAttrValue); }

	/**
	 * Determines if the attribute value is a named color value
	 * @return true if attribute value is a named color, otherwise, false
	 */
	static bool isNamedColorValue (const CString &sValue)
	{
		if ( (sValue.GetLength()) && (::_istalpha(sValue[0])) )
		{
			COLORREF crTemp = _clrInvalid;
			CString		strKey(sValue);

			strKey.MakeLower();
			if (_namedColors.Lookup(strKey, crTemp))
				return (true);
		}
		return (false);

	}
	bool isNamedColorValue(void) const
	{
		return isNamedColorValue (m_strAttrValue);
	}

	/**
	 * Determines if the attribute value is a named system color value
	 * @return true if value is a named system color, false otherwise
	 */
	static bool isSysColorValue (const CString &sValue)
	{
		if ( (sValue.GetLength()) && (::_istalpha(sValue[0])) )
		{
			COLORREF	crTemp = _clrInvalid;
			CString		strKey (sValue);

			strKey.MakeLower();
			if (_namedColors.Lookup(strKey, crTemp))
				return (crTemp >= 0x80000000 && crTemp <= 0x80000018);
		}
		return (false);
	}
	bool isSysColorValue(void) const
	{
		return isSysColorValue (m_strAttrValue);
	}

	/**
	 * Determines if the attribute value is a color value in 
	 * hexadecimal format
	 * @return true if attribute value is a color value, otherwise, false
	 */
	static bool isHexColorValue (const CString &sValue)
	{
		// zero-length attribute value?
		if (sValue.IsEmpty())
			return (false);

		if (sValue[0] == _T('#'))
		{
			if (sValue.GetLength() > 1)
			{
				for (int i = 1; i < sValue.GetLength(); i++)
				{
					if (!::_istxdigit(sValue[i]))
						return (false);
				}
				return (true);
			}
		}

		return (false);
	}
	bool isHexColorValue(void) const
	{
		return isHexColorValue (m_strAttrValue);
	}

	/**
	 * Determines if the attribute value contains a color reference
	 * @return true, if attribute value is color value, false otherwise.
	 */
	bool isColorValue(void) const
		{ return (isNamedColorValue() || isHexColorValue()); }

	/**
	 * Returns the color value of the attribute
	 * @return a COLORREF representing the color
	 */
	static COLORREF getColorValue (const CString& aValue)
	{
		COLORREF crTemp = _clrInvalid;
		if (isNamedColorValue(aValue))
		{
			CString	strKey(aValue);
			strKey.MakeLower();
			if (_namedColors.Lookup(strKey, crTemp))
			{
				// is this a system named color value?
				if (crTemp >= 0x80000000 && crTemp <= 0x80000018)
					crTemp = ::GetSysColor(crTemp & 0x7FFFFFFF);
			}
		}
		else if (isHexColorValue(aValue))
			crTemp = ::_tcstoul(aValue.Mid(1), NULL, 16);
		return (crTemp);
	}
	COLORREF getColorValue(void) const
	{
		return getColorValue (m_strAttrValue);
	}
	
	/**
	 * Returns the RGB value of the attribute in hexadecimal format
	 * @return hexadecimal string representing the color value
	 */
	
	static CString getColorHexValue (const CString& aValue)
	{
		CString	strColorHex;
		if (isHexColorValue (aValue))
			strColorHex = aValue;//aValue.Mid(1);
		else
		{
			COLORREF crTemp = getColorValue(aValue);
			if (crTemp != _clrInvalid)
				strColorHex.Format(_T("#%02x%02x%02x"), crTemp & 0xff, (crTemp >> 8) & 0xff, (crTemp >> 16) & 0xff);
		}
		return (strColorHex);
	}

	CString getColorHexValue(void) const
	{
		return getColorHexValue (m_strAttrValue);
	}

	/**
	 * Checks to see if the attribute contains a percent value
	 * @return true if value is a percent value, otherwise, false
	 */
	bool isPercentValue(void) const
		{ return (m_strAttrValue.Right(1) == _T("%") ? true : false); }

	/**
	 * Returns a percent value of the attribute
	 * @return percentage value
	 */
	unsigned short getPercentValue(unsigned short max = _percentMax) const
	{
		ATLASSERT(max > 0);
		if (!isPercentValue())	return (0);
		unsigned short	percentVal = (unsigned short)((short)*this);
		return ((percentVal > max ? max : percentVal));
	}
	
	/**
	 * Parses a length value from the attribute/value 
	 * and identifies its length unit also
	 *
	 * @param rUnit - this will receive the type of the length unit
	 *
	 * @return an integer value of the attribute 
	 */
	enum LengthUnitsEnum { em, ex, px, per, in, cm, mm, pt, pc };
	short getLengthValue(LengthUnitsEnum &rUnit) const
	{
		static const TCHAR	_szUnits[][4] = 
		{
			/** relative length units */
			_T("em"), _T("ex"), _T("px"), _T("%"), 
			/** absolute length units */
			_T("in"), _T("cm"), _T("mm"), _T("pt"), _T("pc") 
		};

		if (m_strAttrValue.IsEmpty())
			return (0);

		int i;
		for (i = 0; i < sizeof(_szUnits)/sizeof(_szUnits[0]); i++)
		{
			if (m_strAttrValue.Right(::lstrlen(_szUnits[i])). \
				CompareNoCase(_szUnits[i]) == 0)
			{
				rUnit = (LengthUnitsEnum)i;
				break;
			}
		}
		if (i == sizeof(_szUnits)/sizeof(_szUnits[0]))
			return (0);
		return (*this);
	}

// Operators
public:
	/**
	 * Converts attribute value to bool
	 * @return true or false
	 */
	operator bool() const
	{
		if (!m_strAttrValue.CompareNoCase(_T("true")))
			return (true);
		if (!m_strAttrValue.CompareNoCase(_T("false")))
			return (false);
		return (((short)*this ? true : false));
	}

	/**
	 * Converts attribute value to BYTE (unsigned char)
	 * @return the left-most character of attribute value
	 */
	operator BYTE() const
		{ return ((BYTE)(m_strAttrValue.GetLength() ? m_strAttrValue[0] : 0)); }
	
	/**
	 * Converts attribute value to double
	 * @return 0.00 on failure, otherwise, a numeric value
	 */
	operator double() const
		{ return (::_tcstod(m_strAttrValue, NULL)); }
	
	/**
	 * Converts attribute value to signed short int
	 * @return 0 on failure, otherwise, an integer value
	 */
	operator short() const
		{ return ((short)::_ttoi(m_strAttrValue)); }
	
	/**
	 * @return attribute value
	 */
	operator LPCTSTR() const
		{ return (m_strAttrValue); }

// Private Operations
public:
	/**
	 * Sets the value of an attribute. Takes care of the following:
	 *  1. Ignores leading and trailing white-space characters
	 *  2. Replaces character entities with appropriate characters.
	 *  3. Ignores line feeds (LF).
	 *  4. Replaces each carriage-return (CR) or tab with a single space.
	 *
	 * @param lpszValue - new attribute value
	 * 
	 */
	void putValue(LPCTSTR lpszValue)
	{
		ATLASSERT(AtlIsValidString(lpszValue));

		m_strAttrValue = lpszValue;
		
		// ignore leading white-spaces
		m_strAttrValue.TrimLeft();

		// ignore trailing white-spaces
		m_strAttrValue.TrimRight();

		// ignore line feeds
		m_strAttrValue.Remove(_T('\n'));

		// replace tab and carriage-return with a single space
		m_strAttrValue.Replace(_T('\r'), _T(' '));
		m_strAttrValue.Replace(_T('\t'), _T(' '));

		/** resolve entity reference(s) */
		/*
		int		iCurPos = -1, iParseLen = 0;
		TCHAR	chSubst = 0;
		do
		{
			if ((iCurPos = m_strAttrValue.Find(_T('&'), ++iCurPos)) == -1)
				break;
			
			iParseLen = CLiteHTMLEntityResolver::resolveEntity(m_strAttrValue.Mid(iCurPos), chSubst);
			if (iParseLen)
			{
				m_strAttrValue.Replace
				(
					m_strAttrValue.Mid(iCurPos, iParseLen), 
					CString(chSubst)
				);
			}
		}
		while (true);
		*/
	}

// Parsing Helpers
public:
	// parses an attribute/value pair from the given string
	UINT parseFromStr(LPCTSTR lpszString);

// Data Members
public:
	static const COLORREF		_clrInvalid;	// an invalid color
	static const unsigned short	_percentMax;	// maximum allowable percentage value
private:
	typedef CAtlMap<CString, COLORREF>	CNamedColors;

	static CNamedColors		_namedColors;	// collection of named colors
	CString					m_strAttrName,  // attribute name
							m_strAttrValue; // attribute value
};

/**
 * CLiteHTMLElemAttr::parseFromStr
 *
 * @param lpszString - string to parse
 *
 * @return number of TCHARs successfully parsed
 */
inline UINT CLiteHTMLElemAttr::parseFromStr(LPCTSTR lpszString)
{
	ATLASSERT(AtlIsValidString(lpszString));

	LPCTSTR	lpszBegin = lpszString;
	LPCTSTR	lpszEnd;
	TCHAR	ch = 0;

	// skip leading white-space characters
	while (::_istspace(*lpszBegin))
		lpszBegin = ::_tcsinc(lpszBegin);

	// name doesn't begin with an alphabet?
	if (!::_istalpha(*lpszBegin))
		return (0U);

	lpszEnd = lpszBegin;
	do
	{
		// attribute name may contain letters (a-z, A-Z), digits (0-9), 
		// underscores '_', hyphen '-', colons ':', and periods '.'
		if ( (!::_istalnum(*lpszEnd)) && 
			 (*lpszEnd != _T('-')) && (*lpszEnd != _T(':')) && 
			 (*lpszEnd != _T('_')) && (*lpszEnd != _T('.')) )
		{
			ATLASSERT(lpszEnd != lpszBegin);

			// only white-space characters, a null-character, an 
			// equal-sign, a greater-than symbol, or a forward-slash 
			// can act as the separator between an attribute and its 
			// value
			if (*lpszEnd == NULL || 
				*lpszEnd == _T('=') || 
				::_istspace(*lpszEnd) ||
				*lpszEnd == _T('>') || *lpszEnd == _T('/'))
			{
				break;
			}

			return (0U);	// any other character will fail parsing process
		}

		lpszEnd = ::_tcsinc(lpszEnd);
	} while (true);

	// extract attribute name
	CString	strAttrName(lpszBegin, int (lpszEnd - lpszBegin));
	
	// skip spaces after attribute name
	if (::_istspace(*lpszEnd))
	{
		lpszEnd = ::_tcsinc(lpszEnd);
	}

	if (*lpszEnd != _T('='))
	{
		m_strAttrName = strAttrName;
		m_strAttrValue.Empty();
		return (UINT) (lpszEnd - lpszString);
	}
	else
	{
		// skip white-space characters after equal-sign 
		// and the equal-sign itself
		do {
			lpszEnd = ::_tcsinc(lpszEnd);
		} while (::_istspace(*lpszEnd));

		lpszBegin = lpszEnd;
		ch = *lpszEnd;
		
		// is attribute value wrapped in quotes?
		if (ch == _T('\'') || ch == _T('\"'))
		{
			lpszBegin = ::_tcsinc(lpszBegin);	// skip quote symbol
			do
			{
				lpszEnd = ::_tcsinc(lpszEnd);
			}
			// Loop until we find the same quote character that 
			// was used at the starting of the attribute value.
			// Anything within these quotes is considered valid!
			// NOTE that the entity references are resolved later.
			while (*lpszEnd != NULL && *lpszEnd != ch);
		}

		// open attribute value i.e. not wrapped in quotes?
		else
		{
			do 
			{ 
				lpszEnd = ::_tcsinc(lpszEnd);
			}
			// loop until we find a tag ending delimeter or any 
			// white-space character, or until we reach at the 
			// end of the string buffer
			while (*lpszEnd != NULL && !::_istspace(*lpszEnd) && 
				   !(*lpszEnd == _T('/') && *(lpszEnd + 1) != NULL && *(lpszEnd + 1) == _T('>'))	// open atrtibute as ' type=text/javascript  ' - '/' is not end of tag
				   && *lpszEnd != _T('>'));
		}

		m_strAttrName = strAttrName;
		if (lpszEnd == lpszBegin)	// empty attribute value?
			m_strAttrValue.Empty();
		else
			// use putValue() instead of direct assignment; 
			// this will automatically normalize data before 
			// assigning according to the specs and will 
			// also resolve entity references!!!
			putValue(CString(lpszBegin, int (lpszEnd - lpszBegin)));

		// calculate and return the count of characters successfully parsed
		return (UINT (lpszEnd - lpszString) + 
				(ch == _T('\'') || ch == _T('\"') ? 1 : 0) );
	}

	return (0U);
}

/**
 * CLiteHTMLAttributes
 */
class CLiteHTMLAttributes
{
// Construction/Destruction
public:
	CLiteHTMLAttributes() 
		: m_parrAttrib(NULL)
	{ }

	/**
	 * @param bCopy - if true, this CLiteHTMLAttributes makes a copy 
	 * of the encapsulated pointer. if false, this constructor takes 
	 * ownership of the encapsulated pointer.
	 *
	 */
	CLiteHTMLAttributes(CLiteHTMLAttributes &rSource, bool bCopy = false)
		: m_parrAttrib(NULL)
	{
		if (!bCopy)
		{
			m_parrAttrib = rSource.m_parrAttrib;
			rSource.m_parrAttrib = NULL;
		}
		else
		{
			const int	nElemCount = rSource.getCount();
			if (nElemCount)
			{
				if ((m_parrAttrib = new CElemAttrArray) == NULL)
					ATLTRACE2 ("Out of memory\n");
				
				CLiteHTMLElemAttr	*pItem = NULL;
				m_parrAttrib->SetCount(nElemCount);
				
				/** DEEP COPY BEGIN */
				for (int iElem = 0; iElem < nElemCount; iElem++)
				{
					if ((pItem = new CLiteHTMLElemAttr(rSource[iElem])) == NULL)
					{
						removeAll();
						ATLTRACE2 ("Out of memory\n");
						return;
					}
					
					(*m_parrAttrib)[iElem] = pItem;
					pItem = NULL;
				}
				/** DEEP COPY END */
			}
		}
	}

	virtual ~CLiteHTMLAttributes()
		{ removeAll(); }

// Initialization
public:
	// parses attribute/value pairs from the given string
	UINT parseFromStr(LPCTSTR lpszString, long aLength);

// Attributes
public:
	/**
	 * Returns the count of CLiteHTMLElemAttr items in this collection
	 * @return number of items
	 */
	int getCount(void) const
	{
		if (m_parrAttrib != NULL)
			return (int) (m_parrAttrib->GetCount());
		return (0);
	}

	/**
	 * Look up the index of an attribute given its name.
	 * If more than one attribute with the same name exist, 
	 * this will return the index of the first match.
	 *
	 * @param lpszAttributeName - name of the attribute
	 *
	 * @return zero-based index of an attribute, or -1 if not found
	 */
	int getIndexFromName(LPCTSTR lpszAttributeName) const
	{
		ATLASSERT(AtlIsValidString(lpszAttributeName));
		CLiteHTMLElemAttr	*pItem = NULL;
		for (int iElem = 0; iElem < getCount(); iElem++)
		{
			if ((pItem = (*m_parrAttrib)[iElem]) == NULL)	// just in case
				continue;
			
			// perform a CASE-INSENSITIVE search
			if (pItem->m_strAttrName.CompareNoCase(lpszAttributeName) == 0)
				return (iElem);
		}
		return (-1);
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute's index
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 */
	CLiteHTMLElemAttr operator[](int nIndex) const
	{
		if (!(nIndex >= 0 && nIndex < getCount()))
		{
			//ATLASSERT(FALSE);
			return (CLiteHTMLElemAttr());
		}
		return ( *((*m_parrAttrib)[nIndex]) );
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute name
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 */
	CLiteHTMLElemAttr operator[](LPCTSTR lpszIndex) const
	{
		ATLASSERT(AtlIsValidString(lpszIndex));
		return ((*this)[getIndexFromName(lpszIndex)]);
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute's index
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 *  r
	 */
	CLiteHTMLElemAttr getAttribute(int nIndex) const
		{ return ((*this)[nIndex]); }

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute name
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 */
	CLiteHTMLElemAttr getAttribute(LPCTSTR lpszIndex) const
	{
		ATLASSERT(AtlIsValidString(lpszIndex));
		return ((*this)[getIndexFromName(lpszIndex)]);
	}

	BOOL getRawAttribute(int nIndex, CLiteHTMLElemAttr** pAttr)
	{
		if (NULL == pAttr)
			return FALSE;
		
		if (nIndex < 0 || nIndex >= getCount())
		{
			*pAttr = NULL;
			return FALSE;
		}

		*pAttr = m_parrAttrib->GetAt (nIndex);

		return TRUE; 
	}
	BOOL getRawAttribute(LPCTSTR sIndex, CLiteHTMLElemAttr** pAttr)
	{
		if (NULL == pAttr)
			return FALSE;
		
		int nIndex = getIndexFromName(sIndex);
		if (nIndex < 0 || nIndex >= getCount())
		{
			*pAttr = NULL;
			return FALSE;
		}

		*pAttr = m_parrAttrib->GetAt (nIndex);

		return TRUE; 
	}

	/**
	 * Returns the name of an attribute given its index
	 *
	 * @return name of an attribute
	 * @since 1.0
	 */
	CString getName(int nIndex) const
		{ return ((*this)[nIndex].m_strAttrName); }

	/**
	 * Returns the value of an attribute given its index
	 *
	 * @return value of an attribute
	 * @since 1.0
	 */
	CString getValue(int nIndex) const
		{ return ((*this)[nIndex].m_strAttrValue); }

	/**
	 * Returns the value of an attribute given its name
	 *
	 * @return value of an attribute
	 */
	CString getValueFromName(LPCTSTR lpszAttributeName) const
		{ return ((*this)[lpszAttributeName].m_strAttrValue); }

// Operations
public:
	/**
	 * Adds a new CLiteHTMLElemAttr item to the collection
	 *
	 * @param lpszName - attribute name (serves as the key to the item)
	 * @param lpszValue - attribute value
	 * @param bReplaceOld - If an item with the same name as specified 
	 *        by lpszName already exists in the collection, this 
	 *        parameter is used to determine whether to replace the 
	 *        existing item or add a new one
	 *
	 * @return pointer to a CLiteHTMLElemAttr
	 * @since 1.0
	 */
	CLiteHTMLElemAttr* addAttribute(LPCTSTR lpszName, LPCTSTR lpszValue)
	{
		ATLASSERT(AtlIsValidString(lpszName));
		ATLASSERT(AtlIsValidString(lpszValue));

		CLiteHTMLElemAttr	*pItem = new CLiteHTMLElemAttr(lpszName, lpszValue);
		if (pItem != NULL)
		{
			if (m_parrAttrib == NULL)
			{
				if ((m_parrAttrib = new CElemAttrArray) == NULL)
				{
					SAFE_DELETE_POINTER(pItem);
					ATLTRACE2("(Error) CLiteHTMLAttributes::addAttribute: Out of memory.\n");
					return (NULL);
				}
			}
			
			ATLVERIFY(m_parrAttrib->Add(pItem) >= 0);
		}
		return (pItem);
	}
	
	/**
	 * Removes an CLiteHTMLElemAttr item from the collection
	 *
	 * @param lpszName - attribute to remove
	 *
	 * @return true if successful, false otherwise
	 * @since 1.0
	 */
	bool removeAttribute(int nIndex)
	{
		if (!(nIndex >= 0 && nIndex < getCount()))
			return (false);
		CLiteHTMLElemAttr	*pItem = NULL;
		ATLVERIFY(((*m_parrAttrib)[nIndex]) != NULL);
		SAFE_DELETE_POINTER(pItem);
		return (true);
	}
	
	/**
	 * Removes all CLiteHTMLElemAttr items from the collection
	 * @return true if successful, false otherwise
	 * @since 1.0
	 */
	bool removeAll(void)
	{
		CLiteHTMLElemAttr	*pItem = NULL;
		for (int iElem = 0; iElem < getCount(); iElem++)
		{
			ATLVERIFY((pItem = (*m_parrAttrib)[iElem]) != NULL);
			SAFE_DELETE_POINTER(pItem);
		}
		SAFE_DELETE_POINTER(m_parrAttrib);
		return (true);
	}

// Data Members
private:
	typedef ATL::CAtlArray <CLiteHTMLElemAttr*>	CElemAttrArray;
	CElemAttrArray	*m_parrAttrib;	// array of attributes/value pairs
};

/**
 * CLiteHTMLAttributes::parseFromStr
 *
 * @param lpszString - string to parse. It can contain pairs such as: 
 *
 *          1. NAME
 *          2. NAME=VALUE
 *          3. NAME='VALUE'
 *          4. NAME="VALUE"
 *
 *        NAME consist of letters, digits, underscores, 
 *        colons, hyphens, and periods
 *
 *        NOTE that white-spaces between NAME and equal-sign AND 
 *        equal-sign and VALUE is allowed.
 *
 * @return number of TCHARs successfully parsed
 * @since 1.0
 */
inline UINT CLiteHTMLAttributes::parseFromStr(LPCTSTR lpszString, long aLength)
{
	ATLASSERT(AtlIsValidString(lpszString));

	CElemAttrArray		*pcoll = NULL;
	CLiteHTMLElemAttr	oElemAttr;
	const UINT			nStrLen = (UINT) (-1 == aLength) ? ::_tcslen(lpszString) : aLength;
	UINT				nRetVal = 0U, 
						nTemp = 0U;

	do
	{
		// try to parse an attribute/value 
		// pair from the rest of the string
		if (!(nTemp = oElemAttr.parseFromStr(&lpszString[nRetVal])))
		{
			if (!nRetVal)
				goto LError;
			break;
		}

		// collection has not been instantiated until now?
		if (pcoll == NULL)
		{
			// instantiate now
			if ((pcoll = new CElemAttrArray) == NULL)
				// out of memory?
			{
				ATLTRACE2("(Error) CLiteHTMLAttributes::parseFromStr: Out of memory.\n");
				goto LError;
			}
		}

		// add attribute/value pair to collection
		if (pcoll->Add(new CLiteHTMLElemAttr(oElemAttr)) < 0)
			goto LError;

		// advance seek pointer
		nRetVal += nTemp;
	}
	
	// do we still have something in the buffer to parse?
	while (nRetVal < nStrLen);

	// collection was never instantiated?
	if (pcoll == NULL)
		goto LError;
	// collection is empty?
	if (0 == pcoll->GetCount())
		goto LError;
	// current collection could not be emptied?
	if (!removeAll())
		goto LError;

	m_parrAttrib = pcoll;
	pcoll = NULL;
	goto LCleanExit;	// success!

LError:
	SAFE_DELETE_POINTER(pcoll);
	nRetVal = 0U;

LCleanExit:
	return (nRetVal);
}


/**
* CLiteHTMLElemStyleAttr
  style string has form: 'name1=value1 value2 value3; name2 = value4;'
*/

class CLiteHTMLElemStyleAttr
{
private:
	typedef CAtlMap<CString, CString>	CStyleMap;
	CStyleMap							m_aStyleAttrs;
	double								m_dParentFontSize;

protected:
	BOOL ParseMarginTag (CString &aValues)
	{
		CAtlArray<CString> arrValues;
		SpaceSeparatedValuesToArray (aValues, arrValues);
		const int nArrayLength = arrValues.GetCount ();
		if (nArrayLength < 1 || nArrayLength > 4)
			return FALSE;

		switch (nArrayLength)
		{
		case 1:
			{
				CString sValue = arrValues[0];
				m_aStyleAttrs.SetAt (_T("margin-top"), sValue);
				m_aStyleAttrs.SetAt (_T("margin-left"), sValue);
				m_aStyleAttrs.SetAt (_T("margin-right"), sValue);
				m_aStyleAttrs.SetAt (_T("margin-bottom"), sValue);
			}
			break;
		case 2:
			{
				CString sValueTop = arrValues[0];
				CString sValueLeft = arrValues[1];
				m_aStyleAttrs.SetAt (_T("margin-top"), sValueTop);
				m_aStyleAttrs.SetAt (_T("margin-left"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("margin-right"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("margin-bottom"), sValueTop);
			}
			break;
		case 3:
			{
				CString sValueTop = arrValues[0];
				CString sValueLeft = arrValues[1];
				CString sValueBottom = arrValues[2];
				m_aStyleAttrs.SetAt (_T("margin-top"), sValueTop);
				m_aStyleAttrs.SetAt (_T("margin-left"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("margin-right"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("margin-bottom"), sValueBottom);
			}
			break;
		case 4:
			{
				CString sValueTop = arrValues[0];
				CString sValueRight = arrValues[1];
				CString sValueBottom = arrValues[2];
				CString sValueLeft = arrValues[3];
				m_aStyleAttrs.SetAt (_T("margin-top"), sValueTop);
				m_aStyleAttrs.SetAt (_T("margin-left"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("margin-right"), sValueRight);
				m_aStyleAttrs.SetAt (_T("margin-bottom"), sValueBottom);
			}
			break;
		}
		return TRUE;
	}
	BOOL ParsePaddingTag (CString &aValues)
	{
		CAtlArray<CString> arrValues;
		SpaceSeparatedValuesToArray (aValues, arrValues);
		const int nArrayLength = arrValues.GetCount ();
		if (nArrayLength < 1 || nArrayLength > 4)
			return FALSE;

		switch (nArrayLength)
		{
		case 1:
			{
				CString sValue = arrValues[0];
				m_aStyleAttrs.SetAt (_T("padding-top"), sValue);
				m_aStyleAttrs.SetAt (_T("padding-left"), sValue);
				m_aStyleAttrs.SetAt (_T("padding-right"), sValue);
				m_aStyleAttrs.SetAt (_T("padding-bottom"), sValue);
			}
			break;
		case 2:
			{
				CString sValueTop = arrValues[0];
				CString sValueLeft = arrValues[1];
				m_aStyleAttrs.SetAt (_T("padding-top"), sValueTop);
				m_aStyleAttrs.SetAt (_T("padding-left"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("padding-right"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("padding-bottom"), sValueTop);
			}
			break;
		case 3:
			{
				CString sValueTop = arrValues[0];
				CString sValueLeft = arrValues[1];
				CString sValueBottom = arrValues[2];
				m_aStyleAttrs.SetAt (_T("padding-top"), sValueTop);
				m_aStyleAttrs.SetAt (_T("padding-left"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("padding-right"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("padding-bottom"), sValueBottom);
			}
			break;
		case 4:
			{
				CString sValueTop = arrValues[0];
				CString sValueRight = arrValues[1];
				CString sValueBottom = arrValues[2];
				CString sValueLeft = arrValues[3];
				m_aStyleAttrs.SetAt (_T("padding-top"), sValueTop);
				m_aStyleAttrs.SetAt (_T("padding-left"), sValueLeft);
				m_aStyleAttrs.SetAt (_T("padding-right"), sValueRight);
				m_aStyleAttrs.SetAt (_T("padding-bottom"), sValueBottom);
			}
			break;
		}
		return TRUE;
	}
	BOOL ParseBorderColorTag (CString &aValues)
	{
		CAtlArray<CString> arrValues;
		SpaceSeparatedValuesToArray (aValues, arrValues);
		const int nArrayLength = arrValues.GetCount ();
		if (nArrayLength < 1 || nArrayLength > 4)
			return FALSE;

		switch (nArrayLength)
		{
		case 1:
			{
				CString sValue = arrValues[0];
				CString sNewValue = CLiteHTMLElemAttr::getColorHexValue (sValue);
				if (sNewValue.IsEmpty())
					sNewValue = sValue;

				m_aStyleAttrs.SetAt (_T("border-left-color"), sNewValue);
				m_aStyleAttrs.SetAt (_T("border-top-color"), sNewValue);
				m_aStyleAttrs.SetAt (_T("border-right-color"), sNewValue);
				m_aStyleAttrs.SetAt (_T("border-bottom-color"), sNewValue);
			}
			break;
		case 2:
			{
				CString sValueTop = arrValues[0];
				CString sNewValueTop = CLiteHTMLElemAttr::getColorHexValue (sValueTop);
				if (sNewValueTop.IsEmpty())
					sNewValueTop = sValueTop;

				CString sValueLeft = arrValues[1];
				CString sNewValueLeft = CLiteHTMLElemAttr::getColorHexValue (sValueLeft);
				if (sNewValueLeft.IsEmpty())
					sNewValueLeft = sValueLeft;

				m_aStyleAttrs.SetAt (_T("border-left-color"), sNewValueLeft);
				m_aStyleAttrs.SetAt (_T("border-top-color"), sNewValueTop);
				m_aStyleAttrs.SetAt (_T("border-right-color"), sNewValueLeft);
				m_aStyleAttrs.SetAt (_T("border-bottom-color"), sNewValueTop);
			}
			break;
		case 3:
			{
				CString sValueTop = arrValues[0];
				CString sNewValueTop = CLiteHTMLElemAttr::getColorHexValue (sValueTop);
				if (sNewValueTop.IsEmpty())
					sNewValueTop = sValueTop;

				CString sValueLeft = arrValues[1];
				CString sNewValueLeft = CLiteHTMLElemAttr::getColorHexValue (sValueLeft);
				if (sNewValueLeft.IsEmpty())
					sNewValueLeft = sValueLeft;

				CString sValueBottom = arrValues[2];
				CString sNewValueBottom = CLiteHTMLElemAttr::getColorHexValue (sValueBottom);
				if (sNewValueBottom.IsEmpty())
					sNewValueBottom = sValueBottom;

				m_aStyleAttrs.SetAt (_T("border-left-color"), sNewValueLeft);
				m_aStyleAttrs.SetAt (_T("border-top-color"), sNewValueTop);
				m_aStyleAttrs.SetAt (_T("border-right-color"), sNewValueLeft);
				m_aStyleAttrs.SetAt (_T("border-bottom-color"), sNewValueBottom);
			}
			break;
		case 4:
			{
				CString sValueTop = arrValues[0];
				CString sNewValueTop = CLiteHTMLElemAttr::getColorHexValue (sValueTop);
				if (sNewValueTop.IsEmpty())
					sNewValueTop = sValueTop;

				CString sValueRight = arrValues[1];
				CString sNewValueRight = CLiteHTMLElemAttr::getColorHexValue (sValueRight);
				if (sNewValueRight.IsEmpty())
					sNewValueRight = sValueRight;

				CString sValueBottom = arrValues[2];
				CString sNewValueBottom = CLiteHTMLElemAttr::getColorHexValue (sValueBottom);
				if (sNewValueBottom.IsEmpty())
					sNewValueBottom = sValueBottom;

				CString sValueLeft = arrValues[3];
				CString sNewValueLeft = CLiteHTMLElemAttr::getColorHexValue (sValueLeft);
				if (sNewValueLeft.IsEmpty())
					sNewValueLeft = sValueLeft;

				m_aStyleAttrs.SetAt (_T("border-left-color"), sNewValueLeft);
				m_aStyleAttrs.SetAt (_T("border-top-color"), sNewValueTop);
				m_aStyleAttrs.SetAt (_T("border-right-color"), sNewValueRight);
				m_aStyleAttrs.SetAt (_T("border-bottom-color"), sNewValueBottom);
			}
			break;
		}
		return TRUE;
	}
	BOOL ParseBackgroundTag (CString &aValues)
	{
		// example: body {background:#ffffff url('img_tree.png') no-repeat right top;}
		// background-color
		// background-image
		// background-repeat
		// background-attachment
		// background-position
		
		CAtlArray<CString> arrValues;
		SpaceSeparatedValuesToArray (aValues, arrValues);
		const int nArrayLength = arrValues.GetCount ();
		int nCurrentElement = 0;

		if (nCurrentElement >= nArrayLength)
			return FALSE;

		CString sCurrentElement = arrValues[nCurrentElement].MakeLower();

		if (0 != sCurrentElement.Find (_T("url")) && (sCurrentElement != _T("none")))	// if first element is not color, but image
		{
			m_aStyleAttrs.SetAt (_T("background-color"), arrValues[nCurrentElement]);
			++nCurrentElement;
			
		}

		if (nCurrentElement >= nArrayLength)
			return TRUE;
		sCurrentElement = arrValues[nCurrentElement].MakeLower();


		if (0 == sCurrentElement.Find (_T("url")))
		{
			m_aStyleAttrs.SetAt (_T("background-image"), arrValues[nCurrentElement]);
			++nCurrentElement;
		}

		if (nCurrentElement >= nArrayLength)
			return TRUE;
		sCurrentElement = arrValues[nCurrentElement].MakeLower();

		if (sCurrentElement == _T("repeat-x") || sCurrentElement == _T("repeat-y") || sCurrentElement == _T("no-repeat"))
		{
			m_aStyleAttrs.SetAt (_T("background-repeat"), arrValues[nCurrentElement]);
			++nCurrentElement;
		}

		if (nCurrentElement >= nArrayLength)
			return TRUE;
		sCurrentElement = arrValues[nCurrentElement].MakeLower();

		if (sCurrentElement == _T("fixed") || sCurrentElement == _T("scroll") || sCurrentElement == _T("inherit") || sCurrentElement == _T("local"))
		{
			m_aStyleAttrs.SetAt (_T("background-attachment"), arrValues[nCurrentElement]);
			++nCurrentElement;
		}

		if (nCurrentElement >= nArrayLength)
			return TRUE;

		sCurrentElement = arrValues[nCurrentElement].MakeLower();
		m_aStyleAttrs.SetAt (_T("background-position"), arrValues[nCurrentElement]);
		
		return TRUE;
	}
	BOOL ParseFontTag (CString &aValues)
	{
		// example: font:italic bold 12px/30px Georgia, serif;
		// example: font:15px arial,sans-serif;
		// font-style 
		// font-variant 
		// font-weight 
		// font-size/line-height 
		// font-family
		
		CAtlArray<CString> arrValues;
		SpaceSeparatedValuesToArray (aValues, arrValues);
		const int nArrayLength = arrValues.GetCount ();
		int nCurrentElement = 0;
		// font-style
		if ((nCurrentElement < nArrayLength)
			&& (
				0 == arrValues[nCurrentElement].CompareNoCase (_T("normal"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("italic"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("oblique"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("inherit"))
				)
			)
		{
			m_aStyleAttrs.SetAt (_T("font-style"), arrValues[nCurrentElement]);
			++nCurrentElement;
		}
		// font-variant
		if ((nCurrentElement < nArrayLength)
			&& (
				0 == arrValues[nCurrentElement].CompareNoCase (_T("normal"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("small-caps"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("inherit"))
				)
			)
		{
			m_aStyleAttrs.SetAt (_T("font-variant"), arrValues[nCurrentElement]);
			++nCurrentElement;
		}
		// font-weight
		if ((nCurrentElement < nArrayLength)
			&& (
				0 == arrValues[nCurrentElement].CompareNoCase (_T("normal"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("bold"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("bolder"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("lighter"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("100"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("200"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("300"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("400"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("500"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("600"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("700"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("800"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("900"))
				|| 0 == arrValues[nCurrentElement].CompareNoCase (_T("inherit"))
				)
			)
		{
			m_aStyleAttrs.SetAt (_T("font-weight"), arrValues[nCurrentElement]);
			++nCurrentElement;
		}
		// font-size/line-height (required)
		if (nCurrentElement < nArrayLength)
		{
			// find 'line-height'
			const CString sFontSizeLineHeight = arrValues[nCurrentElement];
			int nLineHeightPos = sFontSizeLineHeight.Find('/');
			if (-1 != nLineHeightPos)
			{
				const CString sLineHeight = sFontSizeLineHeight.Mid (nLineHeightPos + 1);
				const CString sFontSize = sFontSizeLineHeight.Mid (0, nLineHeightPos);

				if (!sLineHeight.IsEmpty())
					m_aStyleAttrs.SetAt (_T("line-height"), sLineHeight);

				if (!sFontSize.IsEmpty())
					m_aStyleAttrs.SetAt (_T("font-size"), sLineHeight);
			}
			else
			{
				m_aStyleAttrs.SetAt (_T("font-size"), sFontSizeLineHeight);
			}

			++nCurrentElement;
		}
		// font-family (required)
		if (nCurrentElement < nArrayLength)
		{
			//example: font-family:"Times New Roman",Georgia,Serif;
			CString sFontFamily = arrValues[nCurrentElement];
			++nCurrentElement;

			for (; nCurrentElement < nArrayLength; ++nCurrentElement)
			{
				sFontFamily += _T(" ");
				sFontFamily += arrValues[nCurrentElement];
			}
			if (!sFontFamily.IsEmpty())
				m_aStyleAttrs.SetAt (_T("font-family"), sFontFamily);
		}

		
		
		return TRUE;
	}
	BOOL ParsePair (CString &sPair)
	{
		// an example of pair: "color:blue"
		BOOL bRes = TRUE;
		
		const int nValuePos = sPair.Find(':');
		if (-1 == nValuePos)
			return FALSE;

		CString sName = sPair.Mid (0, nValuePos);
		sName.Remove (' ');

		CString sValue = sPair.Mid (nValuePos + 1);
		sValue = sValue.Trim (' ');
		
		// check some attributes and generate multiple attributes
		if (0 == sName.CompareNoCase(_T("background")))
		{
			// parse background
			ParseBackgroundTag (sValue);
		}
		if (0 == sName.CompareNoCase(_T("border-color")))
		{
			// parse border color
			ParseBorderColorTag (sValue);
		}
		else if (0 == sName.CompareNoCase(_T("font")))
		{
			// parse font
			ParseFontTag (sValue);
		}
		else if (0 == sName.CompareNoCase(_T("font-size")))
		{
			m_aStyleAttrs.SetAt(sName, sValue);
		}
		else if (0 == sName.CompareNoCase(_T("margin")))
		{
			ParseMarginTag (sValue);
		}
		else if (0 == sName.CompareNoCase(_T("padding")))
		{
			ParsePaddingTag (sValue);
		}
		else if (0 == sName.CompareNoCase(_T("line-height")))
		{
			ConvertValueToPx (sValue);
			m_aStyleAttrs.SetAt(sName, sValue);
		}
		else if (
			0 == sName.CompareNoCase(_T("color")) 
			|| 0 == sName.CompareNoCase (_T("border-left-color"))
			|| 0 == sName.CompareNoCase (_T("border-right-color"))
			|| 0 == sName.CompareNoCase (_T("border-top-color"))
			|| 0 == sName.CompareNoCase (_T("border-bottom-color"))
			|| 0 == sName.CompareNoCase (_T("background-color"))
			)
		{
			CString sNewValue = CLiteHTMLElemAttr::getColorHexValue (sValue);
			if (sNewValue.IsEmpty())
				m_aStyleAttrs.SetAt(sName, sValue);
			else
				m_aStyleAttrs.SetAt(sName, sNewValue);
		}
		else
		{
			m_aStyleAttrs.SetAt(sName, sValue);

			

			/*
			// we can't convert all params to pixels - it may be in (%), but not depends from font-size of parent
			CAtlArray<CString> aArray;
			SpaceSeparatedValuesToArray (sValue, aArray);
			CString sResultValue, sFormattedValue;

			for (int nValue = 0; nValue < aArray.GetCount() - 1; nValue++)
			{
				sFormattedValue = aArray[nValue];
				ConvertValueToPx (sFormattedValue);
				sResultValue += sFormattedValue;
				sResultValue += ' ';
			}
			sFormattedValue = aArray[aArray.GetCount() - 1];
			ConvertValueToPx (sFormattedValue);
			sResultValue += sFormattedValue;

			m_aStyleAttrs.SetAt(sName, sResultValue);
			*/
		}

		return bRes;
	}
	BOOL ParseStyleAttributes (const CString &aStyleString)
	{
		BOOL bRes = TRUE;
		
		// an example of style string: "color:blue;text-align: center"

		// find pair
		int nPairPos = 0;
		while (true)
		{
			const int nPos = aStyleString.Find(';', nPairPos);
			if (-1 == nPos)
			{
				// last attribute
				CString sPair = aStyleString.Mid (nPairPos);
				bRes &= ParsePair (sPair);
				break;
			}
			else
			{
				CString sPair = aStyleString.Mid (nPairPos, nPos - nPairPos);
				nPairPos = nPos + 1;	// points to start of pair (skip ';' character)
				bRes &= ParsePair (sPair);
			}
		}

		return bRes;
	}
	BOOL ParseAttributes (CLiteHTMLAttributes* pParent)
	{
		const int nAttrsCount = pParent->getCount();
		for (int nAttr = 0; nAttr < nAttrsCount; nAttr++)
		{
			CString sName = pParent->getName(nAttr);

			if (0 == sName.CompareNoCase(_T("style")))		// parse 'style'
			{
				// found style attribute
				CString sValue = pParent->getValue (nAttr);
				sValue.Remove('\n');
				sValue.Remove('\r');
				sValue.Remove('!');
				sValue.Replace (_T("important"), _T(""));
				return ParseStyleAttributes (sValue);
			}
		}
		return FALSE;
	}
	BOOL SetDefaultValues ()
	{
		// set default values here
		m_dParentFontSize = 16.0;
		return TRUE;
	}
	BOOL AddFrom (const CStyleMap &aStyleAttrs)
	{
		POSITION pPos = aStyleAttrs.GetStartPosition ();
		while (NULL != pPos)
		{
			const CStyleMap::CPair *pPair = aStyleAttrs.GetAt (pPos);

			m_aStyleAttrs.SetAt (pPair->m_key, pPair->m_value);
			aStyleAttrs.GetNext (pPos);
		}
		return TRUE;
	}
	BOOL CopyFrom (const CStyleMap &aStyleAttrs)
	{
		m_aStyleAttrs.RemoveAll ();
		return AddFrom (aStyleAttrs);	
	}
public:
	static CString GetStyleString (const CLiteHTMLAttributes &oAttrs)
	{
		const int nAttrsCount = oAttrs.getCount();
		CString sValue;

		for (int nAttr = 0; nAttr < nAttrsCount; nAttr++)
		{
			CString sName = oAttrs.getName(nAttr);

			//CStringA sAttr; sAttr = sName; ATLTRACE2 ("Attr: %s\n", sAttr);

			if (0 == sName.CompareNoCase(_T("style")))		// parse 'style'
			{
				// found style attribute
				CString sTempValue = oAttrs.getValue (nAttr);
				sTempValue.Remove('\n');
				sTempValue.Remove('\r');

				//sTempValue.Remove('!');
				//sTempValue.Replace (_T("important"), _T(""));
				
				sValue += sTempValue;
			}
		}
		return sValue;
	}
	static CString GetStyleString (const CLiteHTMLAttributes *pAttrs)
	{
		if (NULL == pAttrs)
			return _T("");

		return GetStyleString (*pAttrs);
	}
	// convert size(width, height, max-width) 'auto' and '%' values to 'pt'-value (returns TRUE if converted)
	static BOOL SizeValueToPt (CString& sValue, const int nDefaultValue)
	{
		BOOL bRes = FALSE;
		if (0 == sValue.CompareNoCase(_T("auto")))
		{
			const double dValue = 72.0 / 96.0 * nDefaultValue;

			sValue.Format( _T("%.2fpt"), dValue);
			bRes = TRUE;
		}
		if (!bRes && (-1 != sValue.Find('%')))
		{
			sValue.Remove ('%');

			const int nPerc = _ttoi (sValue.GetBuffer());
			sValue.ReleaseBuffer();

			sValue.Format( _T("%.2fpt"), double (nDefaultValue * 72.0 / 96.0 * (nPerc / 100.0)) );
			bRes = TRUE;
		}
		return bRes;
	}
	static double ConvertFontSizeToPx (CString &sValue, const double dEmValue = 16.0)
	{
		double dValue (0);
		// absolute
		if (0 == sValue.CompareNoCase(_T("xx-small")))
		{
			dValue = 9.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("x-small")))
		{
			dValue = 10.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("small")))
		{
			dValue = 13.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("medium")))	// default
		{
			dValue = 16.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("large")))
		{
			dValue = 18.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("x-large")))
		{
			dValue = 24.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("xx-large")))
		{
			dValue = 32.0;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("larger")))
		{
			dValue = dEmValue * 1.2;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("smaller")))
		{
			dValue = dEmValue * 0.8;
			sValue.Format( _T("%.3fpx"), dValue);
			return dValue;
		}
		else if (0 == sValue.CompareNoCase(_T("inherit")))
		{
			sValue.Format( _T("%.3fpx"), dEmValue);
			return dEmValue;
		}
		
		return ConvertValueToPx (sValue, dEmValue);
	}
	static double ConvertValueToPx (CString &sValue, const double dEmValue = 16.0)
	{
			// dEmValue - размер шрифта родительского тэга, или, что тоже самое, 1em
			int nPos = -1;
			double dValue = dEmValue;
			if ( -1 != ( nPos = sValue.Find( _T("in") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue *= (2.54 / 0.0264);
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("cm") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue /= 0.0264;
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("mm") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue /= 0.00264;
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("em") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue *= dEmValue;
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("ex") ) ) )
			{
				// TO DO: надо протащить данный параметр по нормальному (аналогично 'em')
			}
			else if ( -1 != ( nPos = sValue.Find( _T("pt") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue *= (2.54 / 72 / 0.0264);
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("pc") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue *= (2.54 / 6 / 0.0264);
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("%") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				dValue *= (dEmValue / 100.0);
				sValue.Format( _T("%.3fpx"), dValue );
			}
			else if ( -1 != ( nPos = sValue.Find( _T("px") ) ) )
			{
				dValue = _tstof( sValue.Mid( 0, nPos ) );
				sValue.Format( _T("%.3fpx"), dValue );
			}

			return dValue;
	}


	// parse symbol-separated values to array
	static void SymbolSeparatedValuesToArray (TCHAR aSymbol, const CString& sValue, CAtlArray<CString>& aValues, int nStartPos = 0)
	{
		int nPosStart = nStartPos;	// start of value

		while (true)
		{
			const int nPosEnd = sValue.Find(aSymbol, nPosStart);	// symbol
			if (-1 == nPosEnd)
			{
				// end of string
				aValues.Add (sValue.Mid (nPosStart));
				break;
			}
			else
			{
				// space found
				aValues.Add(sValue.Mid (nPosStart, nPosEnd - nPosStart));
				nPosStart = nPosEnd + 1;	// skip symbol and continue search
			}
		}
	}

	// parse space-separated values to array
	static void SpaceSeparatedValuesToArray (const CString& sValue, CAtlArray<CString>& aValues)
	{
		int nPosStart = 0;	// start of value
		while (nPosStart < sValue.GetLength() && sValue[nPosStart] == ' ')
		{
			++nPosStart;	// spit spaces
		}

		SymbolSeparatedValuesToArray (' ', sValue, aValues, nPosStart);
	}

public:
	CLiteHTMLElemStyleAttr (CLiteHTMLAttributes &pParent)
	{
		SetDefaultValues ();
		ParseAttributes (&pParent);
	}
	CLiteHTMLElemStyleAttr (const CString &sStyle)
	{
		SetDefaultValues ();
		ParseStyleAttributes (sStyle);
	}
	// copy ctor
	CLiteHTMLElemStyleAttr (CLiteHTMLElemStyleAttr &aObj)
	{
		CopyFrom (aObj.m_aStyleAttrs);
	}
	CLiteHTMLElemStyleAttr& operator = (const CLiteHTMLElemStyleAttr& aObj)
	{
		if (&aObj != this)
		{
			CopyFrom (aObj.m_aStyleAttrs);
			m_dParentFontSize = aObj.m_dParentFontSize;
		}
		return *this;
	}
	CLiteHTMLElemStyleAttr& operator + (const CLiteHTMLElemStyleAttr& aObj)
	{
		if (&aObj != this)
		{
			AddFrom (aObj.m_aStyleAttrs);
		}
		return *this;
	}

	size_t getCount () const
	{
		return m_aStyleAttrs.GetCount();
	}
	CString getAttribute (CString sName) const
	{
		const CAtlMap<CString, CString>::CPair *pPair = m_aStyleAttrs.Lookup (sName);
		return (NULL == pPair) ? _T("") : pPair->m_value;
	}
	void setAttribute (CString sName, CString sValue)
	{
		m_aStyleAttrs.SetAt (sName, sValue);
	}
	CString ToString () const
	{
		CString sRes = _T("");
		POSITION pPos = m_aStyleAttrs.GetStartPosition ();
		while (NULL != pPos)
		{
			const CAtlMap<CString, CString>::CPair *pPair = m_aStyleAttrs.GetAt (pPos);
			sRes += pPair->m_key + _T(":") + pPair->m_value + _T(";");
			m_aStyleAttrs.GetNext (pPos);
		}
		return sRes;
	}
	BOOL AddStyleString (const CString aStyle)
	{
		return ParseStyleAttributes (aStyle);
	}
	void SetParentFontSize (double aParentFontSize)
	{
		m_dParentFontSize = aParentFontSize;
	}
	BOOL ConvertAttributesToPx (const double dEmValue = 16.0)
	{
		// rework font attributes
		CString sFontFamily = getAttribute(_T("font-family"));
		CString sFontSize = getAttribute(_T("font-size"));
		CString sFontStyle = getAttribute(_T("font-style"));
		CString sFontVariant = getAttribute(_T("font-variant"));
		CString sFontWeight = getAttribute(_T("font-weight"));
		if (sFontFamily.IsEmpty())
		{
			//setAttribute (_T("font-family"), _T("inherit"));
		}
		if (sFontSize.IsEmpty())
		{
			sFontSize.Format (_T("%.3fpx"), dEmValue);
			setAttribute (_T("font-size"), sFontSize);
		}
		if (sFontStyle.IsEmpty())
		{
			//setAttribute (_T("font-style"), _T("inherit"));
		}
		if (sFontVariant.IsEmpty())
		{
			//setAttribute (_T("font-variant"), _T("inherit"));
		}
		if (sFontWeight.IsEmpty())
		{
			//setAttribute (_T("font-weight"), _T("inherit"));
		}

		// translate to px
		POSITION pPos = m_aStyleAttrs.GetStartPosition ();
		while (NULL != pPos)
		{
			CAtlMap<CString, CString>::CPair *pPair = m_aStyleAttrs.GetAt (pPos);

			if (_T("font-size") == pPair->m_key 
				/*
				|| _T("margin-top") == pPair->m_key 
				|| _T("margin-bottom") == pPair->m_key  
				|| _T("margin-left") == pPair->m_key  
				|| _T("margin-right") == pPair->m_key*/  
				|| _T("text-indent") == pPair->m_key)
			{
				ConvertValueToPx (pPair->m_value, dEmValue);
			}
			m_aStyleAttrs.GetNext (pPos);
		}
		return TRUE;
	}
	// helpers
	// margins
	BOOL FillMargins ()
	{
		CString sMarginLeft = getAttribute(_T("margin-left"));
		CString sMarginRight = getAttribute(_T("margin-right"));
		CString sMarginTop = getAttribute(_T("margin-top"));
		CString sMarginBottom = getAttribute(_T("margin-bottom"));

		CString sMargin = getAttribute(_T("margin"));
		CAtlArray<CString> aMargins;

		if (!sMargin.IsEmpty())
		{
			// parse spece-splitted values
			SpaceSeparatedValuesToArray (sMargin, aMargins);
		}
		else
			return FALSE;

		switch (aMargins.GetCount())
		{
		case 0:
			{
				ATLTRACE2 ("margin string is empty\n");
			}
			break;
		
		
			break;
		case 1:
			{
				if (sMarginTop.IsEmpty())
					sMarginTop = aMargins[0];

				if (sMarginRight.IsEmpty())
					sMarginRight = sMarginTop;

				if (sMarginBottom.IsEmpty())
					sMarginBottom = sMarginTop;

				if (sMarginLeft.IsEmpty())
					sMarginLeft = sMarginTop;
			}
			break;
		case 2:
			{
				if (sMarginTop.IsEmpty())
					sMarginTop = aMargins[0];

				if (sMarginRight.IsEmpty())
					sMarginRight = sMarginTop;

				if (sMarginBottom.IsEmpty())
					sMarginBottom = aMargins[1];

				if (sMarginLeft.IsEmpty())
					sMarginLeft = sMarginRight;
			}
		case 3:
			{
				if (sMarginTop.IsEmpty())
					sMarginTop = aMargins[0];
				if (sMarginRight.IsEmpty())
					sMarginRight = aMargins[1];
				if (sMarginBottom.IsEmpty())
					sMarginBottom = aMargins[2];

				if (sMarginLeft.IsEmpty())
					sMarginLeft = sMarginRight;
			}
			break;
		case 4:		// 4 and more?
		default:
			{
				if (sMarginTop.IsEmpty())
					sMarginTop = aMargins[0];
				if (sMarginRight.IsEmpty())
					sMarginRight = aMargins[1];
				if (sMarginBottom.IsEmpty())
					sMarginBottom = aMargins[2];
				if (sMarginLeft.IsEmpty())
					sMarginLeft = aMargins[3];
			}
			break;
		}

		if (0 != aMargins.GetCount())
		{
			SizeValueToPt (sMarginTop, 0);
			SizeValueToPt (sMarginRight, 0);
			SizeValueToPt (sMarginBottom, 0);
			SizeValueToPt (sMarginLeft, 0);

			setAttribute (_T("margin-top"), sMarginTop);
			setAttribute (_T("margin-right"), sMarginRight);
			setAttribute (_T("margin-bottom"), sMarginBottom);
			setAttribute (_T("margin-left"), sMarginLeft);
		}
		return TRUE;
	}
	
};

#pragma warning (default : 4290)
#pragma warning (default : 4127)
#pragma warning (default : 4239)	// Disable warning C4239: nonstandard extension used : 'initializing' : conversion from 'CLiteHTMLAttributes' to 'CLiteHTMLAttributes &'
#pragma warning(pop)

