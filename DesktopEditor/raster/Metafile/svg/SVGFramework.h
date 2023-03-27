#pragma once

#include "../../../common/Directory.h"
#include "../../../common/Array.h"
#include "../../../xml/include/xmlutils.h"
#include "../../../graphics/pro/Graphics.h"
#include "../../../graphics/Matrix.h"


#define GET_ATTRIBUTEN(NAME)	if(style == NAME) return m_n##NAME;
#define GET_ATTRIBUTED(NAME)	if(style == NAME) return m_n##NAME;
#define GET_ATTRIBUTE(NAME)		if(style == NAME) return m_##NAME;

#define VALIDATE_ATTRIBUTE(NAME) if(nStyle == NAME) return m_bHave##NAME;

#ifndef	M_PI
#define	M_PI		3.14159265358979323846
#endif

#include <math.h>
#include <algorithm>

//------------------------------------------------------------------------------------------------------

namespace SVG
{
	const long c_ag_LineCapFlat				= 0;
	const long c_ag_LineCapSquare			= 1;
	const long c_ag_LineCapTriangle			= 3;
	const long c_ag_LineCapNoAnchor			= 16;
	const long c_ag_LineCapSquareAnchor		= 17;
	const long c_ag_LineCapRoundAnchor		= 18;
	const long c_ag_LineCapDiamondAnchor	= 19;
	const long c_ag_LineCapArrowAnchor		= 20;
	const long c_ag_LineCapAnchorMask		= 240;
	const long c_ag_LineCapCustom			= 255;

	const long c_ag_DashCapFlat				= 0;
	const long c_ag_DashCapRound			= 2;
	const long c_ag_DashCapTriangle			= 3;

	const long c_ag_LineJoinMiter			= 0;
	const long c_ag_LineJoinBevel			= 1;
	const long c_ag_LineJoinRound			= 2;
	const long c_ag_LineJoinMiterClipped	= 3;

	const long c_ag_PenAlignmentCenter		= 0;
	const long c_ag_PenAlignmentInset		= 1;
	const long c_ag_PenAlignmentOutset		= 2;
	const long c_ag_PenAlignmentLeft		= 3;
	const long c_ag_PenAlignmentRight		= 4;

	const long c_dirHorizontal				= 1;
	const long c_dirVertical				= -1;

	const long PNG_ENCODER					= 0;
	const long JPG_ENCODER					= 1;
	const long INVALID_ENCODER				= -1;

	const long c_BrushTypeSolid				= 1000;
	const long c_BrushTypeHorizontal		= 2001;
	const long c_BrushTypeVertical			= 2002;
	const long c_BrushTypeDiagonal1			= 2003;
	const long c_BrushTypeDiagonal2			= 2004;
	const long c_BrushTypeCenter			= 2005;
	const long c_BrushTypePathGradient1		= 2006;
	const long c_BrushTypePathGradient2		= 2007;
	const long c_BrushTypeCylinderHor		= 2008;
	const long c_BrushTypeCylinderVer		= 2009;
	const long c_BrushTypeTexture			= 3008;
	const long c_BrushTypePattern			= 3009;
	const long c_BrushTypeHatch1			= 4009;
	const long c_BrushTypeHatch53			= 4061;
}

namespace StringHelpers
{
	static bool RemoveTagHTMLA(std::string& strXml, const std::string& strBeginTrag, const std::string& strEndTrag)
	{
		std::string::size_type from	= 0;
		std::string::size_type to =	0;

		from = strXml.find(strBeginTrag, from);
		if (std::string::npos == from)
			return false;

		to = strXml.find(strEndTrag, from);
		if (std::string::npos == to)
			return false;

		if ((to != from) && to > from)
		{
			std::string strRes;
			if ( from - 1 > 0 )
				strRes = strXml.substr(0, from);

			strRes += strXml.substr(to + 1, strXml.length() - to - 1);
			strXml = strRes;

			return RemoveTagHTMLA(strXml, strBeginTrag, strEndTrag);
		}

		return false;
	}
	static bool RemoveCommentsA(std::string& strXml)
	{
		std::string::size_type from	= 0;
		std::string::size_type to =	0;

		from = strXml.find("<!--", from);
		if (std::string::npos == from)
			return false;

		to = strXml.find("-->", from);
		if (std::string::npos == to)
			return false;

		if ((to != from) && to > from)
		{
			std::string result;
			if (from - 1 > 0)
				result = strXml.substr(0, from - 1);

			result += strXml.substr(to + 3, strXml.length() - to - 3);
			strXml = result;

			return RemoveCommentsA(strXml);
		}

		return false;
	}
	static bool RemoveTagHTML(std::wstring& strXml, const std::wstring& strBeginTrag, const std::wstring& strEndTrag)
	{
		std::wstring::size_type from	= 0;
		std::wstring::size_type to =	0;

		from = strXml.find(strBeginTrag, from);
		if (std::wstring::npos == from)
			return false;

		to = strXml.find(strEndTrag, from);
		if (std::wstring::npos == to)
			return false;

		if ((to != from) && to > from)
		{
			std::wstring strRes;
			if (from - 1 > 0)
				strRes = strXml.substr(0, from);

			strRes += strXml.substr(to + 1, strXml.length() - to - 1);
			strXml = strRes;

			return RemoveTagHTML(strXml, strBeginTrag, strEndTrag);
		}

		return false;
	}
	static bool RemoveComments(std::wstring& strXml)
	{
		std::wstring::size_type from	= 0;
		std::wstring::size_type to =	0;

		from = strXml.find(L"<!--", from);
		if (std::wstring::npos == from)
			return false;

		to = strXml.find(L"-->", from);
		if (std::wstring::npos == to)
			return false;

		if ((to != from) && to > from)
		{
			std::wstring result;
			if (from - 1 > 0)
				result = strXml.substr(0, from - 1);

			result += strXml.substr(to + 3, strXml.length() - to - 3);
			strXml = result;

			return RemoveComments(strXml);
		}

		return false;
	}
	static bool LoadXmlFile(const std::wstring& File, std::string& strXmlA)
	{
		return NSFile::CFileBinary::ReadAllTextUtf8A(File, strXmlA);
	}

	static std::wstring Tokenize(const std::wstring& value, const std::wstring& find, std::wstring::size_type& off)
	{
		const wchar_t* tokens = find.c_str();
		std::wstring::size_type tokens_size = find.length();

		const wchar_t* values = value.c_str();
		std::wstring::size_type values_size = value.length();

		if (off >= values_size)
		{
			off = std::wstring::npos;
			return L"";
		}

		std::wstring::size_type index = off;
		while (index < values_size)
		{
			std::wstring::size_type i = 0;
			for (; i < tokens_size; i++)
				if (values[index] == tokens[i])
					break;

			if (i != tokens_size)
			{
				if (index == off)
				{
					off = index + 1;
					if (off >= values_size)
						off = std::wstring::npos;
					return L"";
				}

				std::wstring::size_type oldOff = off;
				off = index + 1;
				return value.substr(oldOff, index - oldOff);
			}

			++index;
		}

		std::wstring::size_type oldOff = off;
		std::wstring::size_type len = index - off;

		if (len == 0)
		{
			off = std::wstring::npos;
			return L"";
		}
		else
			off = values_size;

		return value.substr(oldOff, len);
	}

	static void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
	{
		size_t posn = 0;
		while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
		{
			text.replace(posn, replaceFrom.length(), replaceTo);
			posn += replaceTo.length();
		}
	}
	static void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
	{
		size_t posn = 0;
		while (std::string::npos != (posn = text.find(replaceFrom, posn)))
		{
			text.replace(posn, replaceFrom.length(), replaceTo);
			posn += replaceTo.length();
		}
	}
};

namespace SVG
{
	static double DoubleFromString(const std::wstring& strValue)
	{
		bool bNeedZero = false;
		bool bNeedMZero = false;
		if(!strValue.empty())
		{
			if(strValue.front() == L'-')
			{
				if(strValue.length() > 1)
					if(strValue[1] == L'.')
						bNeedMZero = true;
			}
			else if(strValue.front() == L'.')
				bNeedZero = true;
		}

		const wchar_t* buffer = strValue.c_str();
		size_t nLen = strValue.length();

		for (size_t i = 0; i < nLen; ++i)
		{
			if (((buffer[i] >= wchar_t('0')) && (buffer[i] <= wchar_t('9'))) || (buffer[i] == wchar_t('.')))
				continue;
			return 1.0;
		}
		return std::stod(bNeedZero ? L'0' + strValue : (bNeedMZero ? L"-0" + strValue.substr(1) : strValue));
	}
}

namespace SVG
{
	enum ENodeType
	{
		ECircle,
		EClipPath,
		EDefs,
		EEllipse,
		EG,
		ELine,
		ELinearGradient,
		EPath,
		EPolygon,
		EPolyline,
		ERadialGradient,
		ERectangle,
		EStop,
		ESvg,
		ETitle,
		EText,
		EImage,
		EUse,
		ESymbol,
		EPattern,

		EGraphicsContainer,

		ENodesCount
	};
	enum Attribute
	{
		Opacity,

		FillColor,
		FillOpacity,
		FillRule,

		StrokeColor,
		StrokeWidth,
		StrokeOpacity,
		StrokeLineCap,
		StrokeLineJoin,
		StrokeMiterLimit,
		StrokeDashArray,
		StrokeDashOffset,

		FontSize,
		FontFamily,
		FontMetrics,
		FontWeight,
		FontStyleAttr,

		FontTextAnchor,
		FontTextAnchorStart,
		FontTextAnchorMiddle,
		FontTextAnchorEnd,

		FontWeightNormal,
		FontWeightBold,

		FontStyleNormal,
		FontStyleItalic,
		FontStyleOblique,
		FontStyleInherit,

		TLinearGradient,

		///

		None
	};
	enum Metrics
	{
		EM,
		EX,
		PX,
		PT,
		PC,
		CM,
		MM,
		INCH,

		PCT,	//	percent

		UNDEFINED
	};

	struct Point
	{
	public:
		Point ()
		{
			X	=	0.0;
			Y	=	0.0;
		}
		Point (double dX, double dY) : X(dX), Y(dY) {}

		inline void operator+= (const Point& point)
		{
			X	+=	point.X;
			Y	+=	point.Y;
		}
		inline Point operator+ (const Point& point)
		{
			return Point ( X + point.X, Y + point.Y );
		}
		inline void operator*= (const Point& point)
		{
			X	*=	point.X;
			Y	*=	point.Y;
		}

	public:
		double X;
		double Y;
	};
	struct Rect
	{
	public:
		Rect ()
		{
			X		=	0.0;
			Y		=	0.0;
			Width	=	0.0;
			Height	=	0.0;
		}
		Rect (double dX, double dY, double dWidth, double dHeight) : X(dX), Y(dY), Width(dWidth), Height(dHeight) {}

		inline double CenterX ()
		{
			return X + Width * 0.5;
		}

		inline double CenterY ()
		{
			return Y + Height * 0.5;
		}
		inline double GetX() const
		{
			return X;
		}
		inline double GetY() const
		{
			return Y;
		}

		inline double GetWidth() const
		{
			return Width;
		}
		inline double GetHeight() const
		{
			return Height;
		}

	public:

		double X;
		double Y;
		double Width;
		double Height;
	};
	struct PointPath
	{
		PointPath (): oPoint()
		{
			code	=	-1;
			subCode =	-1;
		}

		Point	oPoint;
		int		code;
		int		subCode;

		//	M(m)	-	move to
		//	Z(z)	-	close path
		//	L(l)	-	line to
		//	V(v)	-	vertical line
		//	H(h)	-	horizontal line
		//	C(c)	-	curve to			(x1 y1 x2 y2 x y)
		//	Q(q)	-	quadratic bezier	(x1 y1 x y)

	};
	struct StopColor
	{
	public:
		StopColor()
		{
			m_fOffset	=	0.0;
			m_nColor	=	(unsigned long)-1;
			m_fOpacity	=	0.0;
		}

		inline bool FromStyle(const std::wstring& oStyle)
		{
			return true;
		}
		inline bool FromXml(XmlUtils::CXmlNode& oXml)
		{
			return true;
		}

	public:
		double			m_fOffset;
		unsigned long	m_nColor;
		double			m_fOpacity;
	};
}

namespace SVG
{
	class StrUtils
	{
	public:
		static inline long LongValue(const std::wstring& value)
		{
			size_t len = value.length(); const wchar_t* buf = value.c_str();
			if (0 == len) return 0;
			for (size_t i = 0; i < len; ++i)
			{
				if (isdigit(buf[i]) || (buf[i] == L'-') /* || (value[i] == L'.') || (value[i] == L',') */)
					continue;

				return std::stol(value.substr(0, i));
			}

			return std::stol(value);
		}
		static inline double DoubleValue(const std::wstring& value)
		{
			size_t len = value.length(); const wchar_t* buf = value.c_str();
			if (0 == len) return 0;
			for (size_t i = 0; i < len; ++i)
			{
				if (isdigit(buf[i]) || (buf[i] == L'.') || (buf[i] == L',') || (buf[i] == L'-') || (buf[i] == 'e'))
					continue;

				return std::stol(value.substr(0, i));
			}

			bool bNeedZero = false;
			bool bNeedMZero = false;
			if(value.front() == L'-')
			{
				if(value.length() > 1)
					if(value[1] == L'.')
						bNeedMZero = true;
			}
			else if(value.front() == L'.')
				bNeedZero = true;
			return std::stod(bNeedZero ? L'0' + value : (bNeedMZero ? L"-0" + value.substr(1) : value));
		}
		static inline double DoubleValuePct(const std::wstring& sVal)
		{
			bool bNeedZero = false;
			bool bNeedMZero = false;
			if(!sVal.empty())
			{
				if(sVal.front() == L'-')
				{
					if(sVal.length() > 1)
						if(sVal[1] == L'.')
							bNeedMZero = true;
				}
				else if(sVal.front() == L'.')
					bNeedZero = true;
				if ('%' == sVal.back())
					return std::stod(sVal.substr(0, sVal.length() - 1)) * 0.01;
			}
			else
				return 0;
			return std::stod(bNeedZero ? L'0' + sVal : (bNeedMZero ? L"-0" + sVal.substr(1) : sVal));
		}
		static bool DoubleValues(const std::wstring& SourceW, std::vector<double>& Values)
		{
			std::wstring number	= L"";
			size_t length = SourceW.length();
			const wchar_t* Source = SourceW.c_str();

			for (size_t i = 0; i < length; ++i)
			{
				if ('-' == Source[i])
				{
					if (i > 1)
					{
						if ('e' == Source[i - 1])
						{
							number += Source[i];
							continue;
						}
					}
					if (!number.empty())
						Values.push_back(std::stod(number.front() == L'.' ? L'0' + number : number));
					number = L"";
				}

				if (isdigit(Source[i]) || (Source[i] == '.') || (Source[i] == '-') || (Source[i] == 'e'))
				{
					number += Source[i];
					continue;
				}

				if (!number.empty())
				{
					bool bNeedZero = false;
					bool bNeedMZero = false;
					if(number.front() == L'-')
					{
						if(number.length() > 1)
							if(number[1] == L'.')
								bNeedMZero = true;
					}
					else if(number.front() == L'.')
						bNeedZero = true;
					Values.push_back(std::stod(bNeedZero ? L'0' + number : (bNeedMZero ? L"-0" + number.substr(1) : number)));
				}
				number = L"";
			}

			if (!number.empty())
				Values.push_back(std::stod(number.front() == L'.' ? L'0' + number : number));

			return (0 != Values.size());
		}

		static inline std::wstring RemoveSymbol(std::wstring& src, wchar_t symbol)
		{
			std::wstring result;
			const wchar_t* buf = src.c_str();
			size_t len = src.length();
			for (size_t i = 0; i < len; i++)
			{
				wchar_t currentChar = buf[i];
				if (currentChar != symbol)
					result += currentChar;
			}
			src = result;
			return result;
		}

		static inline std::wstring RemoveSpaces(std::wstring& src)
		{
			return RemoveSymbol(src, ' ');
		}

		static inline Metrics GetMetrics(const std::wstring& value )		//	only for long numbers in value
		{
			const wchar_t* buf = value.c_str();
			size_t len = value.length();
			for (size_t i = 0; i < len; ++i)
			{
				if (isdigit(buf[i]) || (buf[i] == '.') || (buf[i] == ','))
					continue;

				std::wstring metrics = value.substr(i, len - i);

				if (metrics.empty())
					return PX;
				if (metrics == L"px")
					return PX;
				if (metrics == L"mm")
					return MM;
				if (metrics == L"cm")
					return CM;
				if (metrics == L"%")
					return PCT;
				if (metrics == L"in")
					return INCH;
				if (metrics == L"em")  //	default	-	16px
					return EM;
				if (metrics == L"pt")
					return PT;

				return UNDEFINED;
			}

			return UNDEFINED;
		}
		static inline std::wstring UrlRefValue(const std::wstring& sUrlRef)
		{
			if (sUrlRef.length() > 3)
			{
				std::wstring str = sUrlRef;
				str = StrUtils::RemoveSpaces(str);
				const wchar_t* buf = str.c_str();

				if ((buf[0] == 'u') || (buf[1] == 'r') || (buf[2] == 'l'))
				{
					std::wstring::size_type Oct = str.find(L"#");
					if (std::wstring::npos != Oct)
					{
						Oct += 1; // #
						if (')' == str[str.length() - 1])
							return str.substr(Oct, str.length() - Oct - 1);

						return str.substr(Oct, str.length() - Oct);
					}
				}
			}

			return L"";
		}
		static inline bool UrlRefValue2(std::wstring& sUrlRef)
		{
			std::wstring sRet = StrUtils::UrlRefValue(sUrlRef);

			if (!sRet.empty())
			{
				sUrlRef = sRet;
				return true;
			}

			return false;
		}

		static inline int Decode16(wchar_t c)
		{
			if (c >= '0' && c <= '9')
				return c - '0';
			if (c >= 'A' && c <= 'F')
				return 10 + c - 'A';
			if (c >= 'a' && c <= 'f')
				return 10 + c - 'a';
			return 0;
		}

		static std::wstring UrlDecode(const std::wstring& Source)
		{
			std::wstring strDecoded;

			const wchar_t* buf = Source.c_str();
			size_t len = Source.length();

			for ( size_t i = 0; i < len; ++i )
			{
				if ('%' == buf[i] && isxdigit(buf[i + 1]) && isxdigit(buf[i + 2]))
				{
					int byHex1 = Decode16(buf[i + 1]);
					int byHex2 = Decode16(buf[i + 2]);

					strDecoded += (wchar_t)(byHex1 * 16 + byHex2);

					i += 2;
				}
				else
					strDecoded += buf[i];
			}

			return strDecoded;
		}
	};
	struct CubicBezier
	{
	public:
		CubicBezier() {}
		CubicBezier(double X1, double Y1, double CX0, double CY0, double CX1, double CY1)
		{
			m_Points[0].X	=	X1;
			m_Points[0].Y	=	Y1;

			m_Points[1].X	=	CX0;
			m_Points[1].Y	=	CY0;

			m_Points[2].X	=	CX1;
			m_Points[2].Y	=	CY1;
		}
	public:

		Point	m_Points[3];
	};

	class CArcToCubicBezier
	{
	public:
		CArcToCubicBezier(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle, bool bClockDirection = false)
		{
			AddArc ( fX, fY, fWidth, fHeight, -fStartAngle, -fSweepAngle );
		}
		inline const std::vector<CubicBezier>& GetCurves() const
		{
			return m_arrCurves;
		}

	private:

		inline int EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, bool bClockDirection)
		{
			int nRet = 0;

			while ( fAngle1 < 0 )
				fAngle1 += 360;

			while ( fAngle1 > 360 )
				fAngle1 -= 360;

			while ( fAngle2 < 0 )
				fAngle2 += 360;

			while ( fAngle2 >= 360 )
				fAngle2 -= 360;

			if ( !bClockDirection )
			{
				if ( fAngle1 <= fAngle2 )
					nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, false );
				else
				{
					nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 360, false );
					nRet += EllipseArc2( fX, fY, fXRad, fYRad, 0, fAngle2, false );
				}
			}
			else
			{
				if ( fAngle1 >= fAngle2 )
					nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, true );
				else
				{
					nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 0, true );
					nRet += EllipseArc2( fX, fY, fXRad, fYRad, 360, fAngle2, true );
				}
			}

			return nRet;
		}
		inline double AngToEllPrm(double fAngle, double fXRad, double fYRad)
		{
			// Функция для перевода реального угла в параметрическое задание эллписа
			// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
			// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
			return atan2( sin( fAngle ) / fYRad,  cos( fAngle ) / fXRad );
		}
		inline int EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, bool bClockDirection)
		{
			// переведем углы в радианы
			int nRet = 0;

			double dAngle1 = fAngle1 * 3.141592 / 180;
			double dAngle2 = fAngle2 * 3.141592 / 180;

			// Выясним в каких четвертях находятся начальная и конечная точки
			unsigned int nFirstPointQuard  = int(fAngle1) / 90 + 1;
			unsigned int nSecondPointQuard = int(fAngle2) / 90 + 1;
			nSecondPointQuard = std::min( (unsigned int)4, std::max( (unsigned int)1, nSecondPointQuard ) );
			nFirstPointQuard  = std::min( (unsigned int)4, std::max( (unsigned int)1, nFirstPointQuard ) );
			// Проведем линию в начальную точку дуги
			double fStartX = 0.0, fStartY = 0.0, fEndX = 0.0, fEndY = 0.0;

			fStartX = fX + fXRad * cos( AngToEllPrm( dAngle1, fXRad, fYRad ) );
			fStartY = fY + fYRad * sin( AngToEllPrm( dAngle1, fXRad, fYRad ) );

			// _LineTo(fStartX, fStartY);

			// Дальше рисуем по четверям

			double fCurX = fStartX, fCurY = fStartY;
			double dStartAngle = dAngle1;
			double dEndAngle = 0;

			if ( !bClockDirection )
			{
				for( unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++ )
				{
					if ( nIndex == nSecondPointQuard )
						dEndAngle = dAngle2;
					else
						dEndAngle = (90 * (nIndex ) ) * 3.141592f / 180;
					if ( !( nIndex == nFirstPointQuard ) )
						dStartAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;

					EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, false);
				}
			}
			else
			{
				for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- )
				{
					if ( nIndex == nFirstPointQuard )
						dStartAngle = dAngle1;
					else
						dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
					if ( !( nIndex == nSecondPointQuard ) )
						dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
					else
						dEndAngle = dAngle2;

					EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, false);
				}
			}

			return nRet;
		}
		inline int EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, bool bClockDirection = false)
		{
			// Рассчитаем начальную, конечную и контрольные точки
			double fX1  = 0.0, fX2  = 0.0, fY1  = 0.0, fY2  = 0.0;
			double fCX1 = 0.0, fCX2 = 0.0, fCY1 = 0.0, fCY2 = 0.0;

			double fAlpha = sin( dAngle2 - dAngle1 ) * ( sqrt( 4.0 + 3.0 * tan( (dAngle2 - dAngle1) / 2.0 ) * tan( (dAngle2 - dAngle1) / 2.0 ) ) - 1.0 ) / 3.0;

			double fKoef = 1;

			fX1 = fX + fXRad * cos( dAngle1 );
			fY1 = fY + fYRad * sin( dAngle1 );

			fX2 = fX + fXRad * cos( dAngle2 );
			fY2 = fY + fYRad * sin( dAngle2 );

			fCX1 = fX1 - fAlpha * fXRad * sin ( dAngle1 );
			fCY1 = fY1 + fAlpha * fYRad * cos ( dAngle1 );

			fCX2 = fX2 + fAlpha * fXRad * sin ( dAngle2 );
			fCY2 = fY2 - fAlpha * fYRad * cos ( dAngle2 );

			if ( !bClockDirection )
			{
				CubicBezier	oBezier ( fCX1, fCY1, fCX2, fCY2, fX2, fY2 );
				m_arrCurves.push_back ( oBezier );

				*pfXCur = fX2;
				*pfYCur = fY2;
			}
			else
			{
				CubicBezier	oBezier ( fCX1, fCY1, fCX2, fCY2, fX2, fY2 );
				m_arrCurves.push_back ( oBezier );

				*pfXCur = fX1;
				*pfYCur = fY1;
			}

			return 0;
		}
		inline bool AddArc(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
			if (0.0 >= fWidth || 0.0 >= fHeight)
				return false;

			bool bClockDirection = false;
			double fEndAngle = 360 - ( fSweepAngle + fStartAngle );
			double fSrtAngle = 360 - fStartAngle;
			if( fSweepAngle > 0 )
				bClockDirection = true;

			return (0 == EllipseArc(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2, fSrtAngle, fEndAngle, bClockDirection)) ? true : false;
		}

	private:

		std::vector<CubicBezier> m_arrCurves;
	};
}

namespace SVG
{
	struct IRefStorage;
	struct IRef
	{
		virtual ~IRef () {}

		virtual bool RefreshXLinkSource(IRefStorage* pStorage) = 0;
		virtual bool Normalize(const double& dAfX, const double& dAfY) = 0;
	};
	struct ISvgRef: public IRef
	{
		virtual ~ISvgRef () {}

		// base
		inline const ENodeType nodeType() const
		{
			return m_nodeType;
		}
		inline const std::wstring& nodeId() const
		{
			return m_nodeId;
		}

		// clip-path
		inline const std::wstring& GetUrlClip() const
		{
			return m_urlClipPath;
		}
		inline void AttachClip(ISvgRef* pClip)
		{
			if (m_pClip)
				return;

			m_pClip = pClip;
		}
		inline ISvgRef* GetClip()
		{
			return m_pClip;
		}

	public:

		// base

		ENodeType    m_nodeType;
		std::wstring m_nodeId;

		// clip-path

		std::wstring m_urlClipPath;			//	id -> url(#AABB)
		ISvgRef*     m_pClip;
	};
	struct IRefStorage : public IRef
	{
		virtual ~IRefStorage () {}

		virtual bool HashRef(ISvgRef* pRef, bool bDef)	=	0;
		virtual bool GetRef(const std::wstring& ID, ISvgRef*& pRef)	=	0;

		virtual ISvgRef* Get(long index) = 0;
		virtual long GetSize()	=	0;
		virtual bool Push(ISvgRef* pRef) = 0;
	};
}

namespace SVG
{
	class ColorTable
	{
	public:
		ColorTable ()
		{
			InitClrTable ();
		}

		inline int Find(const std::wstring& sKey)
		{
			std::map<std::wstring, unsigned int>::iterator iter = m_Table.find(sKey);
			if (iter == m_Table.end())
				return -1;

			unsigned int clr = iter->second;
			return (int)clr;
			//return ((clr & 0xFF) << 16) | ((clr & 0xFF00)) | ((clr & 0xFF0000) >> 16);
		}

	private:
		void InitClrTable();
	private:

		std::map<std::wstring, unsigned int> m_Table;
	};
	class ColorParser
	{
	public:
		static long ColorFromString(const std::wstring& sColor);
		static long ColorFromRgbString(const std::wstring& Rgb);
		static long ColorFromHexString(const std::wstring& Hex);

	private:
		static ColorTable	m_oTable;
	};
	class GradientColor
	{
	public:
		GradientColor() {}

		bool Add(XmlUtils::CXmlNode& oXml)
		{
			StopColor color;

			color.m_nColor		=	(unsigned long)ColorParser::ColorFromString(oXml.GetAttributeOrValue(L"stop-color", L"#000000"));
			color.m_fOffset		=	StrUtils::DoubleValuePct(oXml.GetAttributeOrValue(L"offset", L"0.0"));
			color.m_fOpacity	=	StrUtils::DoubleValuePct(oXml.GetAttributeOrValue(L"stop-opacity", L"1.0"));

			std::wstring Styles		=	oXml.GetAttributeOrValue(L"style", L"");

			if (!Styles.empty())
			{
				std::wstring::size_type sTok = 0;
				while (std::wstring::npos != sTok)
				{
					std::wstring sSrc =	StringHelpers::Tokenize(Styles, L";", sTok);
					if (!sSrc.empty())
					{
						std::wstring::size_type index = 0;

						StrUtils::RemoveSpaces(sSrc);
						std::wstring sAttr = StringHelpers::Tokenize(sSrc, L":", index);
						if (std::wstring::npos == index)
							continue;

						std::wstring sVal =	sSrc.substr(index, sSrc.length() - index);
						if (L"stop-color" == sAttr)
						{
							color.m_nColor = (unsigned long)ColorParser::ColorFromString(sVal);
							continue;
						}
						else if (L"offset" == sAttr)
						{
							color.m_fOffset = StrUtils::DoubleValuePct(sVal);
							continue;
						}
						else if (L"stop-opacity" == sAttr)
						{
							color.m_fOpacity = StrUtils::DoubleValuePct(sVal);
							continue;
						}
					}
				}
			}

			m_colors.push_back(color);
			return true;
		}
		inline bool Add(const StopColor& color)
		{
			m_colors.push_back(color);
			return true;
		}

		//
		inline const StopColor& Get(int index) const
		{
			return m_colors[index];
		}
		inline long Count()	const
		{
			return (long)m_colors.size();
		}

	private:
		std::vector<StopColor> m_colors;
	};
}

namespace SVG
{
	class ViewBox
	{
	public:
		ViewBox ()
		{
			m_nWidth	=	0;
			m_nHeight	=	0;
			m_nX		=	0;
			m_nY		=	0;

			m_bInUse	=	false;
		}

		bool FromXml(XmlUtils::CXmlNode& oXmlNode)
		{
			return FromString(oXmlNode.GetAttribute(L"viewBox"));
		}
		bool FromString(const std::wstring& Str)
		{
			std::vector<double> Values;
			if (StrUtils::DoubleValues(Str, Values))
			{
				if (4 == Values.size())
				{
					m_nX		=	static_cast<int>(Values[0]);
					m_nY		=	static_cast<int>(Values[1]);
					m_nWidth	=	static_cast<int>(Values[2]);
					m_nHeight	=	static_cast<int>(Values[3]);

					m_bInUse	=	true;

					return true;
				}
			}

			return false;
		}

		inline int GetX() const
		{
			return m_nX;
		}
		inline int GetY() const
		{
			return m_nY;
		}

		inline int GetWidth() const
		{
			return m_nWidth;
		}
		inline int GetHeight() const
		{
			return m_nHeight;
		}

	public:

		int		m_nWidth;
		int		m_nHeight;
		int		m_nX;
		int		m_nY;

		bool	m_bInUse;
	};
	class UnitSystem
	{
	public:
		UnitSystem ()
		{
			m_bIsValid	=	false;

			m_nFontSize	=	0.0;
			m_dOffSetX	=	0.0;
			m_dOffSetY	=	0.0;
		}

		inline void SetViewBox(int SvgWidth, int SvgHeight, const ViewBox& oBox, Metrics oMetrics)
		{
			m_bIsValid		=	true;

			m_nSvgWidth		=	SvgWidth;
			m_nSvgHeight	=	SvgHeight;

			m_oViewBox		=	oBox;

			if (oBox.m_bInUse)
			{
				if ( 0 != (int)oBox.m_nWidth && 0 != (int)oBox.m_nHeight )
				{
					m_dFactorX	=	(double) (m_nSvgWidth)	/ abs ( (double) (oBox.m_nWidth) );
					m_dFactorY	=	m_dFactorX;//(double) (m_nSvgHeight) / (double) (oBox.m_nHeight);

					m_dOffSetX	=	-oBox.m_nX;// * m_dFactorX;
					m_dOffSetY	=	-oBox.m_nY;// * m_dFactorY;

					m_dFactorQ	=	( sqrt ( (double) ( m_nSvgWidth * m_nSvgWidth + m_nSvgHeight * m_nSvgHeight ) ) ) /
							( sqrt ( (double) ( oBox.m_nWidth * oBox.m_nWidth + oBox.m_nHeight * oBox.m_nHeight ) ) );

					return;
				}
			}

			if ( MM == oMetrics )
			{
				//m_dFactorX	=	3.543307;
				//m_dFactorY	=	3.543307;
				//m_dFactorQ	=	3.543307;

				//return;
			}

			m_dFactorX	=	1.0;
			m_dFactorY	=	1.0;
			m_dFactorQ	=	1.0;
		}

		inline void SetFontSizeForEM(double Size)
		{
			m_nFontSize	= Size;
		}
		inline double GetFactorFontSize()
		{
			return m_dFactorQ;
		}

		template <typename T>
		inline T Convert(T value, Metrics m, int direction)		//	'1' - x	; '-1' - y
		{
			if (!m_bIsValid)
				return value;

			if (m == UNDEFINED)	//	in VB CO
			{
				if (1 == direction)			value *=	m_dFactorX;
				else if (-1 == direction)	value	*=	m_dFactorY;
			}
			else if (m == INCH)
			{
				if (1 == direction)			value	*=	90 * m_dFactorX;
				else if (-1 == direction)	value	*=	90 * m_dFactorY;
			}
			else if (m == PCT)
			{
				if (1 == direction)			value	=	m_nSvgWidth * 0.01 * value;
				else if (-1 == direction)	value	=	m_nSvgHeight * 0.01 * value;
			}
			else if (m == EM)
			{
				if (1 == direction)			value	*=	m_nFontSize * m_dFactorX;
				else if (-1 == direction)	value	*=	m_nFontSize * m_dFactorY;
			}
			else if (m == CM)
			{
				if (1 == direction)			value	*=	90 * m_dFactorX / 2.54;
				else if (-1 == direction)	value	*=	90 * m_dFactorY / 2.54;
			}
			else if (m == MM)
			{
				if (1 == direction)			value	*=	90 * m_dFactorX / 25.4;
				else if (-1 == direction)	value	*=	90 * m_dFactorY / 25.4;
			}

			return value;
		}

		inline double GetPX(double value, Metrics m)
		{
			/*

			"1pt" equals "1.25px" (and therefore 1.25 user units)
			"1pc" equals "15px" (and therefore 15 user units)
			"1mm" would be "3.543307px" (3.543307 user units)
			"1cm" equals "35.43307px" (and therefore 35.43307 user units)
			"1in" equals "90px" (and therefore 90 user units)

			*/

			if (m == PC)		return value * 15.0;
			else if (m == PT)	return value * 1.25;
			else if (m == MM)	return value * 3.543307;
			else if (m == CM)	return value * 35.43307;
			else if (m == INCH)	return value * 90.0;

			return value;
		}
		inline double StrokeWidth(double value, Metrics m)
		{
			if (m == UNDEFINED)	value *= m_dFactorQ;
			else if (m == INCH)	value *= 90 * m_dFactorQ;
			else if (m == EM)	value *= m_nFontSize * m_dFactorQ;
			else if (m == PCT)	value  = sqrt ((double)(m_nSvgWidth * m_nSvgWidth + m_nSvgHeight * m_nSvgHeight)) / sqrt (2.0) * 0.01 * value;
			else if (m == CM)	value *= 90 * m_dFactorQ / 2.54;
			else if (m == MM)	value *= 90 * m_dFactorQ / 25.4;

			return value;
		}

		//
		inline const double& OffSetX () const
		{
			return m_dOffSetX;
		}
		inline const double& OffSetY () const
		{
			return m_dOffSetY;
		}
		inline const int& GetWidth () const
		{
			return m_nSvgWidth;
		}
		inline const int& GetHeight () const
		{
			return m_nSvgHeight;
		}

	private:

		bool	m_bIsValid;

		int		m_nSvgWidth;
		int		m_nSvgHeight;

		ViewBox	m_oViewBox;

		double	m_dFactorX;
		double	m_dFactorY;

		double	m_dFactorQ;
		double	m_nFontSize;

		double	m_dOffSetX;
		double	m_dOffSetY;
	};
	class Style	//	map добавим по мере надобности
	{
	public:
		Style()
		{
			SetDefault ();
		}

		inline void SetDefault ()
		{
			m_nOpacity				=	1.0;

			m_nFillColor			=	0;//-1;
			m_nFillOpacity			=	1.0;

			m_nStrokeColor			=	-1;
			m_nStrokeWidth			=	1.0;
			m_nStrokeOpacity		=	1.0;
			m_nStrokeLineCap		=	-1;
			m_nStrokeLineJoin		=	c_ag_LineJoinMiter;

			m_pRefFill				=	NULL;
			m_pRefStroke			=	NULL;

			m_bHaveOpacity			=	false;
			m_bHaveFillColor		=	false;
			m_bHaveFillOpacity		=	false;
			m_bHaveStrokeColor		=	false;
			m_bHaveStrokeWidth		=	false;
			m_bHaveStrokeOpacity	=	false;
			m_bHaveStrokeLineCap	=	false;
			m_bHaveStrokeLineJoin	=	false;

			m_IsCSS					=	false;
		}
		inline void ClearOpacity ()
		{
			m_nOpacity				=	1.0;
		}

		//
		inline bool SetStyle (const std::wstring& Styles, bool Clear, UnitSystem& oUnitSystem, IRefStorage* pStorage, const ColorTable& Table = ColorTable()) //	from string
		{
			if ( Clear )
			{
				SetDefault ();
			}

			std::wstring::size_type To = 0;
			while ( std::wstring::npos != To )
			{
				std::wstring Source = StringHelpers::Tokenize(Styles, L";", To);
				if (!Source.empty())
				{
					std::wstring::size_type Ind = 0;

					StrUtils::RemoveSpaces(Source);

					std::wstring attr = StringHelpers::Tokenize(Source, L":", Ind);
					if (std::wstring::npos == Ind)
						continue;

					std::wstring Value = Source.substr(Ind, Source.length() - Ind);

					if (L"opacity" == attr)
					{
						m_nOpacity		=	DoubleFromString(Value);
						m_bHaveOpacity	=	true;
					}
					else if (L"fill" == attr)
					{
						m_fillUrlRef = StrUtils::UrlRefValue(Value);
						if (!m_fillUrlRef.empty())
						{
							if (pStorage)
							{
								ISvgRef* pDef	=	NULL;
								if (pStorage->GetRef(m_fillUrlRef, pDef))
								{
									m_pRefFill	=	pDef;
									continue;
								}
							}
						}

						m_nFillColor		=	ColorParser::ColorFromString(Value);
						m_bHaveFillColor	=	true;
					}
					else if (L"fill-opacity" == attr)
					{
						m_nFillOpacity		=	DoubleFromString(Value);
						m_bHaveFillOpacity	=	true;
					}
					else if (L"stroke" == attr)
					{
						std::wstring sUrlRef		=	StrUtils::UrlRefValue(Value);
						if (!sUrlRef.empty())
						{
							if (pStorage)
							{
								ISvgRef* pDef	=	NULL;
								if (pStorage->GetRef(sUrlRef, pDef))
								{
									m_pRefStroke	=	pDef;
									continue;
								}
							}
						}

						m_nStrokeColor			=	ColorParser::ColorFromString ( Value );
						m_bHaveStrokeColor		=	true;
					}
					else if (L"stroke-width" == attr)
					{
						m_nStrokeWidth			=	DoubleValue (Value, oUnitSystem);
						m_bHaveStrokeWidth		=	true;
					}
					else if (L"stroke-opacity" == attr)
					{
						m_nStrokeOpacity		=	DoubleFromString(Value);
						m_bHaveStrokeOpacity	=	true;
					}
					else if (L"stroke-linecap" == attr)
					{
						m_nStrokeLineCap	=	c_ag_LineCapFlat;

						if (L"round" == Value)
						{
							m_nStrokeLineCap	=	c_ag_LineCapRoundAnchor;
						}

						if (L"square" == Value)
						{
							m_nStrokeLineCap	=	c_ag_LineCapSquareAnchor;
						}

						m_bHaveStrokeLineCap	=	true;
					}
					else if (L"stroke-linejoin"	==	attr)
					{
						m_nStrokeLineJoin	=	c_ag_LineJoinMiter;

						if (L"round" == Value)
						{
							m_nStrokeLineJoin	=	c_ag_LineJoinRound;
						}

						if (L"bevel" == Value)
						{
							m_nStrokeLineJoin	=	c_ag_LineJoinBevel;
						}

						m_bHaveStrokeLineJoin	=	true;
					}
				}
			}

			return true;
		}
		inline bool SetStyle (XmlUtils::CXmlNode& oXmlNode, bool bResetStyle, UnitSystem& oUnitSystem, IRefStorage* pStorage, const ColorTable& Table = ColorTable())
		{
			if (bResetStyle)
				SetDefault ();

			std::wstring FillColor = oXmlNode.GetAttributeOrValue(L"fill");

			std::wstring sUrlRef = StrUtils::UrlRefValue(FillColor);
			if (!sUrlRef.empty())
			{
				ISvgRef* pDef = NULL;
				if (pStorage->GetRef(sUrlRef, pDef))
				{
					m_pRefFill = pDef;
				}
			}
			else
			{
				m_nFillColor = ColorParser::ColorFromString(oXmlNode.GetAttributeOrValue(L"fill", L"#000000"));
			}

			std::wstring strOpacity	= oXmlNode.GetAttributeOrValue(L"opacity", L"");
			if (!strOpacity.empty())
			{
				m_nOpacity			=	DoubleFromString (strOpacity);
				m_bHaveOpacity		=	true;
			}

			std::wstring strFillOpacity = oXmlNode.GetAttributeOrValue(L"fill-opacity", L"1.0");
			if (!strOpacity.empty())
			{
				m_nFillOpacity		=	DoubleFromString (strFillOpacity);
				m_bHaveFillOpacity	=	true;
			}

			// stroke settings
			std::wstring StrokeColor = oXmlNode.GetAttributeOrValue(L"stroke");

			std::wstring DefSID = StrUtils::UrlRefValue(StrokeColor);
			if (DefSID.length() > 1)
			{
				ISvgRef* pDef	=	NULL;
				if ( pStorage->GetRef ( DefSID, pDef ) )
				{
					m_pRefStroke	=	pDef;
				}
			}
			else
			{
				m_nStrokeColor = ColorParser::ColorFromString(oXmlNode.GetAttributeOrValue(L"stroke", L"#000000"));
			}

			m_nStrokeOpacity	=	DoubleFromString(oXmlNode.GetAttributeOrValue(L"stroke-opacity", L"1.0"));
			m_nStrokeWidth		=	DoubleValue(oXmlNode, L"stroke-width", oUnitSystem); // StrUtils::DoubleValue ( oXmlNode.GetAttributeOrValue ( _T("stroke-width"), _T("1.0") ) );

			std::wstring LineCap = oXmlNode.GetAttributeOrValue(L"stroke-linecap", L"");
			if (!LineCap.empty())
			{
				m_nStrokeLineCap = c_ag_LineCapFlat;

				if (L"round" == LineCap)
				{
					m_nStrokeLineCap	=	c_ag_LineCapRoundAnchor;
				}

				if (L"square" == LineCap)
				{
					m_nStrokeLineCap	=	c_ag_LineCapSquare;
				}
			}

			std::wstring LineJoin = oXmlNode.GetAttributeOrValue(L"stroke-linejoin", L"miter");
			if (!LineJoin.empty())
			{
				m_nStrokeLineJoin = c_ag_LineJoinMiter;

				if (L"round" == LineJoin)
				{
					m_nStrokeLineJoin	=	c_ag_LineJoinRound;
				}

				if (L"bevel" == LineJoin)
				{
					m_nStrokeLineJoin	=	c_ag_LineJoinBevel;
				}
			}

			return true;
		}
		inline bool UpdateStyle (XmlUtils::CXmlNode& oXmlNode, UnitSystem& oUnitSystem, IRefStorage* pStorage, const ColorTable& Table = ColorTable())
		{
			// FILL

			std::wstring strFillColor = oXmlNode.GetAttributeOrValue(L"fill");

			if (!strFillColor.empty())
			{
				std::wstring sUrlRef = StrUtils::UrlRefValue(strFillColor);
				if (!sUrlRef.empty())
				{
					ISvgRef* pDef	=	NULL;
					if(pStorage != NULL)
						if (pStorage->GetRef(sUrlRef, pDef))
							m_pRefFill	=	pDef;
				}
				else
				{
					m_nFillColor		=	ColorParser::ColorFromString(oXmlNode.GetAttributeOrValue(L"fill", L"#000000"));
					m_bHaveFillColor	=	true;
				}
			}

			std::wstring strOpacity	= oXmlNode.GetAttributeOrValue(L"opacity", L"");
			if (!strOpacity.empty())
			{
				m_nOpacity			=	DoubleFromString(strOpacity);
				m_bHaveOpacity		=	true;
			}

			std::wstring strFillOpacity = oXmlNode.GetAttributeOrValue(L"fill-opacity", L"1.0");
			if (!strOpacity.empty())
			{
				m_nFillOpacity		=	DoubleFromString (strFillOpacity);
				m_bHaveFillOpacity	=	true;
			}

			// STROKE
			std::wstring StrokeColor = oXmlNode.GetAttributeOrValue(L"stroke");
			if (StrokeColor.length() > 1)
			{
				std::wstring sUrlRef = StrUtils::UrlRefValue(StrokeColor);
				if (!sUrlRef.empty())
				{
					ISvgRef* pDef	=	NULL;
					if (pStorage->GetRef(sUrlRef, pDef))
					{
						m_pRefStroke	=	pDef;
					}
				}
				else
				{
					m_nStrokeColor		=	ColorParser::ColorFromString(oXmlNode.GetAttributeOrValue(L"stroke", L"#000000"));
					m_bHaveStrokeColor	=	true;
				}
			}

			if (!oXmlNode.GetAttributeOrValue(L"stroke-opacity").empty())
			{
				m_nStrokeOpacity		=	DoubleFromString(oXmlNode.GetAttributeOrValue(L"stroke-opacity", L"1.0"));
				m_bHaveStrokeOpacity	=	true;
			}

			if (!oXmlNode.GetAttributeOrValue(L"stroke-width").empty())
			{
				m_nStrokeWidth			=	DoubleValue(oXmlNode, L"stroke-width", oUnitSystem); // StrUtils::DoubleValue ( oXmlNode.GetAttributeOrValue ( _T("stroke-width"), _T("1.0") ) );
				m_bHaveStrokeWidth		=	true;
			}

			std::wstring LineCap = oXmlNode.GetAttributeOrValue(L"stroke-linecap", L"");
			if (!LineCap.empty())
			{
				if (L"round" == LineCap)
				{
					m_nStrokeLineCap		=	c_ag_LineCapRoundAnchor;
					m_bHaveStrokeLineCap	=	true;
				}

				if (L"square" == LineCap)
				{
					m_nStrokeLineCap		=	c_ag_LineCapSquare;
					m_bHaveStrokeLineCap	=	true;
				}
			}

			std::wstring strLineJoin = oXmlNode.GetAttributeOrValue(L"stroke-linejoin", L"miter");
			if (!strLineJoin.empty())
			{
				m_nStrokeLineJoin			=	c_ag_LineJoinMiter;

				if (L"round" == strLineJoin)
				{
					m_nStrokeLineJoin		=	c_ag_LineJoinRound;
					m_bHaveStrokeLineJoin	=	true;
				}

				if (L"bevel" == strLineJoin)
				{
					m_nStrokeLineJoin		=	c_ag_LineJoinBevel;
					m_bHaveStrokeLineJoin	=	true;
				}
			}

			return true;
		}

		// EX
		inline Style(const std::wstring& Styles, bool Clear, UnitSystem& oUnitSystem, IRefStorage* pStorage, const ColorTable& Table = ColorTable())
		{
			SetStyle(Styles, Clear, oUnitSystem, pStorage, Table);
		}
		inline bool UpdateStyle (const std::wstring& Styles, UnitSystem& oUnitSystem, IRefStorage* pStorage, const ColorTable& Table = ColorTable())
		{
			return SetStyle(Styles, false, oUnitSystem, pStorage, Table);
		}
		inline void Combine(const Style& oSrc)
		{
			// opacity
			if(oSrc.ValidAttribute(Opacity))
			{
				m_nOpacity *= oSrc.m_nOpacity;
			}
		}

		//
		inline long LongAttribute (SVG::Attribute style) const
		{
			GET_ATTRIBUTEN(FillColor);
			GET_ATTRIBUTEN(StrokeColor);
			GET_ATTRIBUTEN(StrokeLineCap);
			GET_ATTRIBUTEN(StrokeLineJoin);

			return 0;
		}
		inline double DoubleAttribute (SVG::Attribute style) const
		{
			GET_ATTRIBUTED(Opacity);
			GET_ATTRIBUTED(FillOpacity);
			GET_ATTRIBUTED(StrokeWidth);
			GET_ATTRIBUTED(StrokeOpacity);

			return 0.0;
		}
		inline void ClearFillColor()
		{
			m_nFillColor = -3;
		}

		//
		inline bool ValidAttribute (Attribute nStyle) const
		{
			VALIDATE_ATTRIBUTE (Opacity);
			VALIDATE_ATTRIBUTE (FillColor);
			VALIDATE_ATTRIBUTE (FillOpacity);
			VALIDATE_ATTRIBUTE (StrokeColor);
			VALIDATE_ATTRIBUTE (StrokeWidth);
			VALIDATE_ATTRIBUTE (StrokeOpacity);
			VALIDATE_ATTRIBUTE (StrokeLineCap);
			VALIDATE_ATTRIBUTE (StrokeLineJoin);

			return false;
		}
		inline static void UpdateValidateAttributes (const Style& oSrc, Style& oTo)
		{
			if (oSrc.ValidAttribute(Opacity) && (false == oTo.ValidAttribute(Opacity)))
				oTo.m_nOpacity			=	oSrc.m_nOpacity;
			else if(oSrc.ValidAttribute(Opacity) && oTo.ValidAttribute(Opacity))
				oTo.m_nOpacity			*=	oSrc.m_nOpacity;

			if (oSrc.ValidAttribute(FillColor))
				oTo.m_nFillColor		=	oSrc.m_nFillColor;
			if (oSrc.ValidAttribute(FillOpacity))
				oTo.m_nFillOpacity		=	oSrc.m_nFillOpacity;
			if (oSrc.ValidAttribute(StrokeColor))
				oTo.m_nStrokeColor		=	oSrc.m_nStrokeColor;
			if (oSrc.ValidAttribute(StrokeWidth))
				oTo.m_nStrokeWidth		=	oSrc.m_nStrokeWidth;
			if (oSrc.ValidAttribute(StrokeOpacity))
				oTo.m_nStrokeOpacity	=	oSrc.m_nStrokeOpacity;
			if (oSrc.ValidAttribute(StrokeLineCap))
				oTo.m_nStrokeLineCap	=	oSrc.m_nStrokeLineCap;
			if (oSrc.ValidAttribute(StrokeOpacity))
				oTo.m_nStrokeLineJoin	=	oSrc.m_nStrokeLineJoin;
		}

		//
		inline ISvgRef* GetFill() const
		{
			return	m_pRefFill;
		}
		inline ISvgRef* GetStroke() const
		{
			return	m_pRefStroke;
		}

		inline const std::wstring& GetFillUrlRef() const
		{
			return m_fillUrlRef;
		}

		inline void SetCSS(bool isCSS)
		{
			m_IsCSS	=	isCSS;
		}
		inline bool IsCSS() const
		{
			return m_IsCSS;
		}

	private:

		inline static double DoubleValue (XmlUtils::CXmlNode& oXmlNode, std::wstring strAttr, UnitSystem& oUnitSystem)
		{
			return DoubleValue (oXmlNode.GetAttribute (strAttr), oUnitSystem);
		}
		inline static double DoubleValue (const std::wstring& sVal, UnitSystem& us)
		{
			if (sVal.empty())
				return 0.0;

			double number = StrUtils::DoubleValue(sVal);
			SVG::Metrics ms = StrUtils::GetMetrics(sVal);

			return us.StrokeWidth (number, ms);
		}

	private:

		bool		m_bHaveOpacity;
		bool		m_bHaveFillColor;
		bool		m_bHaveFillOpacity;
		bool		m_bHaveStrokeColor;
		bool		m_bHaveStrokeWidth;
		bool		m_bHaveStrokeOpacity;
		bool		m_bHaveStrokeLineCap;
		bool		m_bHaveStrokeLineJoin;

		double		m_nOpacity;

		long		m_nFillColor;
		double		m_nFillOpacity;

		// stroke
		long		m_nStrokeColor;
		double		m_nStrokeWidth;
		double		m_nStrokeOpacity;
		long		m_nStrokeLineCap;
		long		m_nStrokeLineJoin;

		ISvgRef*	m_pRefFill;
		ISvgRef*	m_pRefStroke;
		std::wstring		m_fillUrlRef;

		bool		m_IsCSS;
	};
	class StyleStack
	{
	public:
		StyleStack()
		{

		}

		inline bool Push(const Style& oStyle, bool combine = true)
		{
			if ((0 == m_styles.size()) || (false == combine))
			{
				m_styles.push_back(oStyle);
				return true;
			}

			if (combine)
			{
				Style oStyleTo = oStyle;
				oStyleTo.Combine(GetTop());
				m_styles.push_back(oStyleTo);

				return true;
			}

			m_styles.push_back(oStyle);
			return true;
		}
		inline bool Pop()
		{
			if (m_styles.size())
			{
				m_styles.pop_back();
				return true;
			}

			return false;
		}
		inline Style GetTop()
		{
			Style oStyle;

			if (m_styles.size())
				oStyle = m_styles[m_styles.size() - 1];

			return oStyle;
		}

	private:

		std::vector<Style> m_styles;
	};
	class FontStyle
	{
	public:
		FontStyle ()
		{
			SetDefault ();
		}

		bool SetStyle(const std::wstring& src)
		{
			/*
			<text id="svg_7" text-decoration="underline"
			font-style="italic"
			fill="#cc0000"
			dominant-baseline="text-before-edge"
			space="preserve"
			text-anchor="start"
			font-weight="normal"
			font-family="Arial"
			font-size="40pt"
			y="100"
			x="100">Simple Text</text>
			*/

			return true;
		}
		bool SetStyle(XmlUtils::CXmlNode& oXml)
		{
			m_nFontSize			=	StrUtils::DoubleValue(oXml.GetAttribute(L"font-size"));
			m_nFontMetrics		=	StrUtils::GetMetrics(oXml.GetAttribute(L"font-size"));
			m_FontFamily		=	oXml.GetAttributeOrValue(L"font-family", L"Arial");

			m_nFontTextAnchor	=	FontTextAnchorStart;

			std::wstring anchor = oXml.GetAttributeOrValue(L"text-anchor", L"start");
			if (L"middle" == anchor)
				m_nFontTextAnchor	=	FontTextAnchorMiddle;
			if (L"end" == anchor)
				m_nFontTextAnchor	=	FontTextAnchorEnd;

			m_nFontWeight			=	FontWeightNormal;

			std::wstring FontWeight	= oXml.GetAttributeOrValue(L"font-weight", L"");
			if (L"bold" == FontWeight)
				m_nFontWeight		=	FontWeightBold;

			m_nFontStyleAttr		=	FontStyleNormal;

			std::wstring StyleAttr = oXml.GetAttributeOrValue(L"font-style", L"");
			if (L"italic" == StyleAttr)
				m_nFontStyleAttr	=	FontStyleItalic;
			if (L"oblique" == StyleAttr)
				m_nFontStyleAttr	=	FontStyleItalic;	//	FontStyleOblique;
			//if (L"inherit" == FontWeight)
			//	m_nFontStyleAttr	=	FontStyleInherit;

			return true;
		}
		bool SetStyle(const std::wstring& Styles, bool Clear)
		{
			if (Clear)
			{
				SetDefault();
			}

			std::wstring::size_type To = 0;
			while (std::wstring::npos != To)
			{
				std::wstring Source	= StringHelpers::Tokenize(Styles, L";", To);
				if (!Source.empty())
				{
					std::wstring::size_type Ind = 0;

					StrUtils::RemoveSpaces(Source);

					std::wstring Attribute = StringHelpers::Tokenize(Source, L":", Ind);
					if (std::wstring::npos == Ind)
						continue;

					std::wstring Value = Source.substr(Ind, Source.length() - Ind);
					if (L"font-size" == Attribute)
					{
						m_nFontSize		=	StrUtils::DoubleValue ( Value );
						m_nFontMetrics	=	StrUtils::GetMetrics ( Value );
						continue;
					}

					if (L"font-family" == Attribute)
					{
						m_FontFamily	=	Value;
						continue;
					}

					if (L"text-anchor" == Attribute)
					{
						m_nFontTextAnchor	=	FontTextAnchorStart;
						if (L"middle" == Value)
							m_nFontTextAnchor	=	FontTextAnchorMiddle;
						if (L"end" == Value)
							m_nFontTextAnchor	=	FontTextAnchorEnd;

						continue;
					}

					if (L"font-weight" == Attribute)
					{
						m_nFontWeight		=	FontWeightNormal;
						if (L"bold" == Value)
							m_nFontWeight	=	FontWeightBold;
					}

					if (L"font-style" == Attribute)
					{
						m_nFontStyleAttr		=	FontStyleNormal;

						if (L"italic" == Value)
							m_nFontStyleAttr	=	FontStyleItalic;
						if (L"oblique" == Value)
							m_nFontStyleAttr	=	FontStyleItalic;	//	FontStyleOblique;
						//if (L"inherit" == FontWeight)
						//	m_nFontWeight		=	FontStyleInherit;
					}
				}
			}

			return true;
		}
		bool UpdateStyle(XmlUtils::CXmlNode& oXmlNode)
		{
			if (!oXmlNode.GetAttribute ( L"font-size" ).empty())
				m_nFontSize			=	StrUtils::DoubleValue ( oXmlNode.GetAttribute ( L"font-size") );

			if (!oXmlNode.GetAttribute ( L"font-size" ).empty())
				m_nFontMetrics		=	StrUtils::GetMetrics ( oXmlNode.GetAttribute ( L"font-size" ) );

			if (!oXmlNode.GetAttributeOrValue ( L"font-family", L"Arial" ).empty())
				m_FontFamily		=	oXmlNode.GetAttributeOrValue ( L"font-family", L"Arial");

			if (!oXmlNode.GetAttributeOrValue ( L"text-anchor", L"" ).empty())
			{
				m_nFontTextAnchor	=	FontTextAnchorStart;

				std::wstring anchor		=	oXmlNode.GetAttributeOrValue ( L"text-anchor", L"start" );
				if ( L"middle" == anchor )
					m_nFontTextAnchor	=	FontTextAnchorMiddle;
				if ( L"end" == anchor )
					m_nFontTextAnchor	=	FontTextAnchorEnd;
			}

			if (!oXmlNode.GetAttributeOrValue ( L"font-weight", L"" ).empty())
			{
				m_nFontWeight			=	FontWeightNormal;

				std::wstring FontWeight	= oXmlNode.GetAttributeOrValue(L"font-weight", L"");
				if (L"bold" == FontWeight)
					m_nFontWeight		=	FontWeightBold;
			}

			if (!oXmlNode.GetAttributeOrValue(L"font-style", L"").empty())
			{
				m_nFontStyleAttr		=	FontStyleNormal;

				std::wstring StyleAttr = oXmlNode.GetAttributeOrValue(L"font-style", L"");
				if ( L"italic" == StyleAttr )
					m_nFontStyleAttr	=	FontStyleItalic;
				if ( L"oblique" == StyleAttr )
					m_nFontStyleAttr	=	FontStyleItalic;	//	FontStyleOblique;
				//if ( L"inherit" == FontWeight )
				//	m_nFontStyleAttr	=	FontStyleInherit;
			}

			return true;
		}

		inline double DoubleAttribute(SVG::Attribute style) const
		{
			GET_ATTRIBUTED(FontSize);

			return 0.0;
		}
		inline long LongAttribute(SVG::Attribute style) const
		{
			GET_ATTRIBUTEN(FontMetrics);
			GET_ATTRIBUTEN(FontTextAnchor);
			GET_ATTRIBUTEN(FontWeight);
			GET_ATTRIBUTEN(FontStyleAttr);

			return 0;
		}
		inline std::wstring GetAttribute(SVG::Attribute style) const
		{
			GET_ATTRIBUTE(FontFamily);

			return L"";
		}
		inline void SetDefault ()
		{
			m_FontFamily		=	L"";
			m_nFontTextAnchor	=	FontTextAnchorStart;
			m_nFontSize			=	0.0;
			m_nFontWeight		=	FontWeightNormal;
			m_nFontStyleAttr	=	FontStyleNormal;
		}

	private:

		std::wstring m_FontFamily;
		int		m_nFontTextAnchor;
		int		m_nFontWeight;
		int		m_nFontStyleAttr;
		std::wstring m_Preserve;
		std::wstring m_DominantBaseline;
		std::wstring m_TextDecoration;

		double	m_nFontSize;
		Metrics	m_nFontMetrics;
	};

	class CStyleCSS
	{
	public:
		CStyleCSS() : m_IsValid(false)
		{

		}

		inline bool Read (XmlUtils::CXmlNode& oXml, UnitSystem& us, IRefStorage* model, const ColorTable& colors = ColorTable())
		{
			// TODO: универсальный селектор													* {margin:0; padding:0;}
			// TODO: селекторами элементов												    p {font-family: Garamond, serif;}
			// TODO: селекторами классов											    .note {color: red; background: yellow; font-weight: bold;}
			// TODO: селекторами идентификаторов								  #paragraph1 {margin: 0;}
			// TODO: селекторами атрибутов					 a[href="http://www.somesite.com"]{font-weight:bold;}
			// TODO: селекторами потомков (контекстными селекторами)	div#paragraph1 p.note {color: red;}
			// TODO: селекторами дочерних элементов								   p.note > b {color: green;}
			// TODO: селекторами сестринских элементов								   h1 + p {font-size: 24pt;}
			// TODO: селекторами псевдоклассов										 a:active {color:yellow;}
			// TODO: селекторами псевдоэлементов							  p::first-letter {font-size: 32px;}

			std::wstring source = oXml.GetText();

			StrUtils::RemoveSymbol(source, ' ');
			StrUtils::RemoveSymbol(source, '\n');
			StrUtils::RemoveSymbol(source, '\t');

			if (source.empty())
				return false;

			std::wstring::size_type begin	=	0;
			std::wstring::size_type right	=	0;

			while (std::wstring::npos != right)
			{
				right = source.find(L"}", right);
				if (std::wstring::npos == right)
					break;

				std::wstring src = source.substr(begin, right);
				std::wstring::size_type left = src.find(L"{", left);
				if (std::wstring::npos == left)
					break;

				std::wstring selector = src.substr(0, left);
				if (!selector.empty())
				{
					Style style;
					style.SetStyle(src.substr(left + 1, src.length() - left), true, us, model, colors);

					ReadSelList(selector, ',');
					for (size_t i = 0; i < m_selectors.size(); ++i)
					{
						selector = m_selectors[i];
						if ('.' == selector.c_str()[0])
						{
							StrUtils::RemoveSymbol(selector, '.');
							m_classStyles.insert(std::pair<std::wstring, Style>(selector, style));
						}
						else
						{
							AddTagStyle(m_selectors[i], style);
						}

						m_IsValid	=	true;
					}
				}

				right++;
				begin = right;
			}

			return true;
		}

		inline bool GetStyleForType(int tag, Style& style)
		{
			if (m_typeStyles[tag].IsCSS())
			{
				style = m_typeStyles[tag];
				return true;
			}

			return false;
		}
		inline bool GetClassStyle(const std::wstring& name, Style& style)
		{
			std::map<std::wstring, Style>::iterator iter = m_classStyles.find(name);
			if (iter == m_classStyles.end())
				return false;

			style = iter->second;
			return true;
		}

		inline bool IsValid()
		{
			return m_IsValid;
		}

	private:

		inline bool AddTagStyle(const std::wstring& selector, Style& style)
		{
			if (L"rect"				== selector)	{ m_typeStyles[ERectangle]	=	style;	m_typeStyles[ERectangle].SetCSS(true);	return true; }
			else if (L"ellipse"		== selector)	{ m_typeStyles[EEllipse]	=	style;	m_typeStyles[EEllipse].SetCSS(true);	return true; }
			else if (L"circle"		== selector)	{ m_typeStyles[ECircle]		=	style;	m_typeStyles[ECircle].SetCSS(true);		return true; }
			else if (L"path"		== selector)	{ m_typeStyles[EPath]		=	style;	m_typeStyles[EPath].SetCSS(true);		return true; }
			else if (L"polygon"		== selector)	{ m_typeStyles[EPolygon]	=	style;	m_typeStyles[EPolygon].SetCSS(true);	return true; }
			else if (L"polyline"	== selector)	{ m_typeStyles[EPolyline]	=	style;	m_typeStyles[EPolyline].SetCSS(true);	return true; }
			else if (L"text"		== selector)	{ m_typeStyles[EText]		=	style;	m_typeStyles[EText].SetCSS(true);		return true; }
			else if (L"image"		== selector)	{ m_typeStyles[EImage]		=	style;	m_typeStyles[EImage].SetCSS(true);		return true; }
			else if (L"line"		== selector)	{ m_typeStyles[ELine]		=	style;	m_typeStyles[ELine].SetCSS(true);		return true; }

			//else if (L"symbol"	== selector)	{ m_typeStyles[ESymbol]				=	style;	m_typeStyles[ESymbol].SetCSS(true);					return true; }
			//else if (L"g")		== selector)	{ m_typeStyles[EGraphicsContainer]	=	style;	m_typeStyles[EGraphicsContainer].SetCSS(true);		return true; }

			return false;
		}
		inline void ReadSelList(const std::wstring& src, wchar_t splitter)
		{
			m_selectors.clear();

			std::wstring selector;

			if (!src.empty())
			{
				std::wstring::size_type symbol = 0, begin = 0;
				while (std::wstring::npos != symbol)
				{
					symbol = src.find(splitter, symbol);
					if (std::wstring::npos == symbol)
					{
						if (src.length() - begin >= 1)
						{
							selector = src.substr(begin, src.length() - begin);
							if (!selector.empty())
								m_selectors.push_back(selector);
						}

						break;
					}

					selector = src.substr(begin, symbol);
					if (selector.length())
						m_selectors.push_back(selector);

					symbol++;
					begin = symbol;
				}
			}
		}

	private:
		bool					m_IsValid;

		std::map<std::wstring, Style>	m_classStyles;					//	селекторы классов
		Style							m_typeStyles[ENodesCount];		//	селекторы элементов
		std::vector<std::wstring>		m_selectors;

		// TODO: остальные типа селекторов
	};
}

namespace SVG
{
	class Matrix
	{
	public:
		Matrix()
		{
			Identity();
		}
		Matrix(double mat[9])
		{
			memcpy (m_fMat, mat, 9 * sizeof(double));
		}
		Matrix(double m1, double m2, double m3, double m4, double m5, double m6, double m7, double m8, double m9)
		{
			m_fMat[0] = m1;
			m_fMat[1] = m2;
			m_fMat[2] = m3;

			m_fMat[3] = m4;
			m_fMat[4] = m5;
			m_fMat[5] = m6;

			m_fMat[6] = m7;
			m_fMat[7] = m8;
			m_fMat[8] = m9;
		}
		Matrix(const std::wstring& transforms, UnitSystem& us)
		{
			FromString (transforms, us);
		}

		inline Matrix Copy() const
		{
			return (*this);
		}
		inline const double& operator [] (int i) const
		{
			return m_fMat[i];
		}
		inline Matrix& operator *= (const Matrix& transform)
		{
			double mat[9];

			for (int i = 0 ; i < 3; ++i)
			{
				for( int k = 0; k < 3; ++k)
				{
					mat [i*3 + k] = 0;

					for (int j = 0; j < 3; ++j)
					{
						mat[i*3 + k] += m_fMat[i * 3 + j] * transform.m_fMat [j*3 + k];
					}
				}
			}

			*this = Matrix(mat);

			return (*this);
		}
		inline Matrix& Move(double fX, double fY)
		{
			m_fMat[2] += fX;
			m_fMat[5] += fY;

			return (*this);
		}
		inline Matrix& Translate(double dX, double dY)
		{
			m_fMat[2] = dX;
			m_fMat[5] = dY;

			return (*this);
		}

		//
		inline static Matrix ScaleTransform(double dX, double dY)
		{
			Matrix mat;

			mat.m_fMat[0] = dX;
			mat.m_fMat[4] = dY;

			return mat;
		}
		inline static Matrix RotateTransform(double angle)
		{
			Matrix mat;

			mat.m_fMat[0] =	cos(angle);
			mat.m_fMat[1] =	-sin(angle);

			mat.m_fMat[3] = sin(angle);
			mat.m_fMat[4] = cos(angle);

			return mat;
		}
		inline static Matrix TranslateTransform(double dX, double dY)
		{
			Matrix mat;

			mat.m_fMat[2] = dX;
			mat.m_fMat[5] = dY;

			return mat;
		}
		inline static Matrix ApplyAt(double dX, double dY, const Matrix& transform)
		{
			Matrix mat;

			mat.m_fMat[2] = -dX;
			mat.m_fMat[5] = -dY;

			mat	*=	transform;

			mat.m_fMat[2] += dX;
			mat.m_fMat[5] += dY;

			return mat;
		}
		inline static Matrix RotateAtPoint(const Point& point, const double& angle)
		{
			Matrix mat;

			mat.m_fMat[6] = -point.X;
			mat.m_fMat[7] = -point.Y;

			mat *= Matrix::RotateTransform (angle);

			mat.m_fMat[6] += point.X;
			mat.m_fMat[7] += point.Y;

			return mat;
		}

		//
		inline Point TranformPoint (const Point& point)
		{
			return Point(point.X * m_fMat[0] + point.Y * m_fMat [3] + m_fMat[6], point.X * m_fMat[1] + point.Y * m_fMat [4] + m_fMat[7]);
		}

	private:

		inline void Identity()
		{
			m_fMat[0] = 1;
			m_fMat[1] = 0;
			m_fMat[2] = 0;

			m_fMat[3] = 0;
			m_fMat[4] = 1;
			m_fMat[5] = 0;

			m_fMat[6] = 0;
			m_fMat[7] = 0;
			m_fMat[8] = 1;
		}
		inline bool FromString(const std::wstring& sTransforms, UnitSystem& us)
		{
			std::vector<std::wstring> arrMatrix;
			if (GetTranforms(sTransforms, arrMatrix))
			{
				Matrix matrix;

				for (size_t i = 0; i < arrMatrix.size(); ++i)
					matrix *= Matrix::ReadMatrix(arrMatrix[i], us);

				*this = matrix;

				return true;
			}

			return false;
		}
		inline static std::wstring GetTransform(const std::wstring& strMat, std::wstring::size_type& index)
		{
			std::wstring strText = StringHelpers::Tokenize(strMat, L"(", index);
			if (std::wstring::npos == index)
				return L"";

			StrUtils::RemoveSpaces(strText);
			return strText;
		}
		inline static Matrix ReadMatrix(const std::wstring& sSrc, UnitSystem& us)
		{
			std::wstring::size_type index = 0;
			std::wstring matType = GetTransform(sSrc, index);
			if (matType.empty() || std::wstring::npos == index)
				return Matrix();

			std::wstring sMat = sSrc.substr(index, sSrc.length() - index);
			if (sMat.empty())
				return Matrix();

			std::vector<double> mat;
			if (StrUtils::DoubleValues(sMat, mat))
			{
				size_t count = mat.size();
				if (L"matrix" == matType)
				{
					Matrix oTransform;

					/*

					oTransform.m_fMat [ 0 ]	=	mat [ 0 ];														//	a
					oTransform.m_fMat [ 1 ]	=	mat [ 2 ];														//	c
					oTransform.m_fMat [ 2 ]	=	mat [ 4 ];		//	e	//	tx
					oTransform.m_fMat [ 3 ]	=	mat [ 1 ];														//	b
					oTransform.m_fMat [ 4 ]	=	mat [ 3 ];														//	d
					oTransform.m_fMat [ 5 ]	=	mat [ 5 ];		//	f	//	ty
					oTransform.m_fMat [ 6 ]	=	0.0;
					oTransform.m_fMat [ 7 ]	=	0.0;
					oTransform.m_fMat [ 8 ]	=	1.0;

					*/

					//matrix(0<a> 1<b> 2<c> 3<d> 4<e> 5<f>)
					oTransform.m_fMat[0] = mat[0];														//	a
					oTransform.m_fMat[1] = mat[2];														//	c
					oTransform.m_fMat[2] = us.Convert(mat[4], UNDEFINED, c_dirHorizontal);		//	e	//	tx
					oTransform.m_fMat[3] = mat[1];														//	b
					oTransform.m_fMat[4] = mat[3];														//	d
					oTransform.m_fMat[5] = us.Convert(mat[5], UNDEFINED, c_dirVertical);		//	f	//	ty
					oTransform.m_fMat[6] = 0.0;
					oTransform.m_fMat[7] = 0.0;
					oTransform.m_fMat[8] = 1.0;

					return oTransform;
				}
				else if (L"translate" == matType)
				{
					if (1 == count)	return	Matrix::TranslateTransform(us.Convert(mat[0], UNDEFINED, c_dirHorizontal), 0.0);
					if (2 == count)	return	Matrix::TranslateTransform(us.Convert(mat[0], UNDEFINED, c_dirHorizontal), us.Convert(mat[1], UNDEFINED, c_dirVertical));
				}
				else if (L"scale" == matType)
				{
					if (1 == count)	return Matrix::ScaleTransform(mat[0], mat[0]);
					if (2 == count)	return Matrix::ScaleTransform(mat[0], mat[1]);
				}
				else if (L"rotate" == matType)
				{
					if (1 == count)	return	Matrix::RotateTransform(mat[0] * M_PI / 180.0);
					if (3 == count)
					{
						Matrix oTransform = Matrix::TranslateTransform(mat[1], mat[2]);

						oTransform	*=	Matrix::RotateTransform(mat[0] * M_PI / 180);
						oTransform	*=	Matrix::TranslateTransform(-mat[1], -mat[2]);

						return	oTransform;
					}
				}
				else if (L"skewX" == matType)
				{
					Matrix oTransform;
					oTransform.m_fMat[1] = tan(mat[0] * M_PI / 180.0);		//	c

					return oTransform;
				}
				else if (L"skewY" == matType)
				{
					Matrix oTransform;
					oTransform.m_fMat[3] = tan(mat[0] * M_PI / 180.0);		//	b

					return oTransform;
				}
			}

			return Matrix();
		}
		inline static bool GetTranforms(const std::wstring& strMat, std::vector<std::wstring>& refTransforms)
		{
			std::wstring::size_type index = 0;
			while (std::wstring::npos != index)
			{
				std::wstring sToken = StringHelpers::Tokenize(strMat, L")", index);
				if (!sToken.empty())
					refTransforms.push_back(sToken);
			}

			return (0 != refTransforms.size());
		}

	private:

		double	m_fMat[9];
	};
	class MatrixStack
	{
	public:
		MatrixStack()
		{

		}

		inline bool Push(const Matrix& transform, bool combine = true)	//	каждая последующая трансформация применяется к вершине стэка
		{
			if ((0 == m_transforms.size()) || (false == combine))
			{
				m_transforms.push_back(transform);
				return true;
			}

			if (combine)
			{
				Matrix top = GetTop ();
				top *= transform;

				m_transforms.push_back(top);
				return true;
			}

			m_transforms.push_back(transform);
			return true;
		}
		inline bool Pop()
		{
			if (m_transforms.size() > 0)
			{
				m_transforms.pop_back();
				return true;
			}

			return false;
		}

		inline const Matrix GetTop() const
		{
			Matrix transform;

			if (m_transforms.size() > 0)
				transform = m_transforms[m_transforms.size() - 1];

			return transform;
		}
		inline const Matrix GetFinal()
		{
			Matrix transform;

			for (size_t i = 0; i < m_transforms.size(); ++i)
				transform *= m_transforms[i];

			return transform;
		}

	private:
		std::vector<Matrix> m_transforms;
	};
	class PathParser
	{
	public:
		PathParser()
		{
			m_bIsClose	=	false;
		}
		PathParser(UnitSystem& oUs, const std::wstring& path)
		{
			FromString (oUs, path);
		}

		inline bool FromString(UnitSystem& oUnitSystem, const std::wstring& path)
		{
			m_Points.clear ();

			std::wstring::size_type To = 0;
			bool	EndSegment	=	false;
			std::wstring Tokens = L"mMLlCcVvHhCcQqTtSsaAhHvV";

			std::wstring::size_type count = path.length();
			const wchar_t* path_str = path.c_str();
			for (std::wstring::size_type i = 0; i < count; ++i)
			{
				wchar_t symbol = path_str[i];

				//	move to
				if ('m' == symbol || 'M' == symbol || 'l' == symbol || 'L' == symbol || 't' == symbol || 'T' == symbol)
				{
					To = i + 1;

					std::wstring Source	= StringHelpers::Tokenize(path, Tokens, To);

					// ATLTRACE ( _T("%c : - %s\n"), symbol, Source );

					std::vector<double> Values;
					if (StrUtils::DoubleValues(Source, Values))
					{
						if ((symbol == 'M') || (symbol == 'm'))
						{
							int Segments	=	(int)Values.size () / 2;
							if (Segments > 0)
							{
								PointPath oPointM;
								oPointM.code	=	path[i];
								oPointM.oPoint	=	Point ( Values [ 0 ], Values [ 1 ] );
								oPointM.subCode	=	path[i];
								m_Points.push_back ( oPointM );
							}

							int iNextCode = (symbol == 'M') ? 'L' : 'l';
							for ( int j = 1; j <  Segments; ++j )
							{
								PointPath oPoint;
								oPoint.code		=	iNextCode;
								oPoint.oPoint	=	Point ( Values [ 0 + 2 * j ], Values [ 1 + 2 * j ] );
								oPoint.subCode	=	path[i];

								m_Points.push_back ( oPoint );
								EndSegment = false;
								//	ATLTRACE ( _T("LineTo : %c, -  %f, %f\n"), oPoint.code, oPoint.oPoint.X, oPoint.oPoint.Y );
							}
						}
						else
						{
							if (m_Points.size() > 0 && EndSegment == false)
							{
								if ( symbol == 'M' )
								{
									PointPath oEndPoint;
									oEndPoint.code = 'Z';
									m_Points.push_back ( oEndPoint );
								}

								if (symbol == 'm')
								{
									PointPath oEndPoint;
									oEndPoint.code = 'z';
									m_Points.push_back ( oEndPoint );
								}
							}

							int Segments	=	(int)Values.size () / 2;
							for ( int j = 0; j <  Segments; ++j )
							{
								PointPath oPoint;
								oPoint.code	=	symbol;

								if ( m_Points.size() > 0 && EndSegment == false )
								{
									if ( oPoint.code == 'M' )
										oPoint.code = 'L';

									if ( oPoint.code == 'm' )
										oPoint.code = 'l';
								}

								oPoint.oPoint	=	Point ( Values [ 0 + 2 * j ], Values [ 1 + 2 * j ] );

								m_Points.push_back ( oPoint );

								EndSegment	=	false;

								//	ATLTRACE ( _T("LineTo : %c, -  %f, %f\n"), oPoint.code, oPoint.oPoint.X, oPoint.oPoint.Y );
							}
						}
					}

					if (To == std::wstring::npos)
					{
						m_Points.clear ();
						return false;
					}
				}
				else if ( 'h' == symbol || 'H' == symbol || 'v' == symbol || 'V' == symbol )
				{
					To = i + 1;

					std::wstring Source	= StringHelpers::Tokenize(path, Tokens, To);

					std::vector<double> Values;
					if (StrUtils::DoubleValues(Source, Values))
					{
						for ( size_t j = 0; j < Values.size(); ++j )
						{
							PointPath oPoint;
							oPoint.code	= symbol;

							if ( 'h' == symbol || 'H' == symbol )
								oPoint.oPoint.X	=	Values [ j ];

							if ( 'v' == symbol || 'V' == symbol )
								oPoint.oPoint.Y	=	Values [ j ];

							m_Points.push_back(oPoint);
						}
					}

					if ( To == std::wstring::npos )
					{
						m_Points.clear ();
						return false;
					}
				}
				else if ( 'a' == symbol || 'A' == symbol )	// Elliptical arc curve
				{
					To = i + 1;

					std::wstring Source	= StringHelpers::Tokenize(path, Tokens, To);

					std::vector<double> Values;
					if (StrUtils::DoubleValues(Source, Values))
					{
						//rx ry x-axis-rotation large-arc-flag sweep-flag x y

						int Size	=	( Values.size () / 7 ) * 7;
						for ( int j = 0; j < Size; j += 7 )
						{
							PointPath oPoint;
							oPoint.code	=	symbol;

							oPoint.oPoint	=	Point ( Values [ j ], Values [ j + 1 ] );		//	rx ry
							m_Points.push_back ( oPoint );

							oPoint.oPoint	=	Point ( Values [ j + 2 ], Values [ j + 3 ] );	//	x-axis-rotation large-arc-flag
							m_Points.push_back ( oPoint );

							oPoint.oPoint	=	Point ( Values [ j + 4 ], Values [ j + 4 ] );	//	sweep-flag
							m_Points.push_back ( oPoint );

							oPoint.oPoint	=	Point ( Values [ j + 5 ], Values [ j + 6 ] );	//	x y
							m_Points.push_back ( oPoint );
						}
					}

					if ( To == std::wstring::npos )
					{
						m_Points.clear();
						return false;
					}
				}
				else if ( 'q' == symbol || 'Q' == symbol || 's' == symbol || 'S' == symbol )	// Quadratic Bezier
				{
					To = i + 1;

					std::wstring Source	= StringHelpers::Tokenize(path, Tokens, To);

					std::vector<double> Values;
					if (StrUtils::DoubleValues(Source, Values))
					{
						int Size	=	( Values.size () / 4 ) * 4;
						for ( int j = 0; j < Size; j += 4 )
						{
							PointPath oPoint;
							oPoint.code	=	symbol;

							oPoint.oPoint	=	Point ( Values [ j ], Values [ j + 1 ] );
							m_Points.push_back ( oPoint );

							oPoint.oPoint	=	Point ( Values [ j + 2 ], Values [ j + 3 ] );
							m_Points.push_back ( oPoint );

							// ATLTRACE ( _T("LineTo : %f, %f "), oPoint.oPoint.X, oPoint.oPoint.Y );
						}
					}

					if ( To == std::wstring::npos )
					{
						m_Points.clear ();
						return false;
					}
				}
				else if ( 'c' == symbol || 'C' == symbol )	// Cubic  Bezier
				{
					To = i + 1;

					std::wstring Source	= StringHelpers::Tokenize(path, Tokens, To);

					// ATLTRACE ( _T("%c : - %s\n"), symbol, Source );

					std::vector<double> Values;
					if (StrUtils::DoubleValues(Source, Values))
					{
						int Size	=	( Values.size () / 6 ) * 6;
						for ( int j = 0; j < Size; j += 6 )
						{
							PointPath oPoint;
							oPoint.code	=	symbol;

							oPoint.oPoint	=	Point ( Values [ j ], Values [ j + 1 ] );
							m_Points.push_back ( oPoint );

							oPoint.oPoint	=	Point ( Values [ j + 2 ], Values [ j + 3 ] );
							m_Points.push_back ( oPoint );

							oPoint.oPoint	=	Point ( Values [ j + 4 ], Values [ j + 5 ] );
							m_Points.push_back ( oPoint );

							// ATLTRACE ( _T("add curve : [ %f, %f ] \n"), oPoint.oPoint.X, oPoint.oPoint.Y );
						}
					}

					if (To == std::wstring::npos)
					{
						m_Points.clear();
						return false;
					}
				}
				else if ('z' == symbol || 'Z' == symbol)	//	close path
				{
					PointPath point;
					point.code		=	symbol;

					m_Points.push_back(point);

					m_bIsClose		=	true;
					EndSegment		=	true;
				}
			}

			count = m_Points.size();
			for (size_t i = 0; i < count; ++i)
			{
				if ('a' == m_Points[i].code || 'A' == m_Points[i].code)
				{
					m_Points [ i ].oPoint.X			=	oUnitSystem.Convert ( m_Points [ i ].oPoint.X, UNDEFINED, c_dirHorizontal );
					m_Points [ i ].oPoint.Y			=	oUnitSystem.Convert ( m_Points [ i ].oPoint.Y, UNDEFINED, c_dirVertical );

					m_Points [ i + 3 ].oPoint.X		=	oUnitSystem.Convert ( m_Points [ i + 3 ].oPoint.X, UNDEFINED, c_dirHorizontal );
					m_Points [ i + 3 ].oPoint.Y		=	oUnitSystem.Convert ( m_Points [ i + 3 ].oPoint.Y, UNDEFINED, c_dirVertical );

					i += 3;

					continue;
				}

				m_Points [ i ].oPoint.X		=	oUnitSystem.Convert ( m_Points [ i ].oPoint.X, UNDEFINED, c_dirHorizontal );
				m_Points [ i ].oPoint.Y		=	oUnitSystem.Convert ( m_Points [ i ].oPoint.Y, UNDEFINED, c_dirVertical );
			}

			return true;
		}
		inline std::vector<PointPath>& GetPoints()
		{
			return m_Points;
		}
		inline bool IsClose()
		{
			return m_bIsClose;
		}
		inline void SetPoint(long index, const Point& val)
		{
			if ((long)m_Points.size() > index)
				m_Points[index].oPoint = val;
		}
		inline Point GetPoint(long index)
		{
			if ((long)m_Points.size() > index)
				return m_Points[index].oPoint;

			return Point();
		}

	private:

		bool				m_bIsClose;
		std::vector <PointPath>	m_Points;
	};
	class ImageBase64		//	for inner data image's
	{
	public:
		ImageBase64 ()
		{
			m_pBuffer		=	NULL;
			m_nBufferSize	=	0L;
		}
		~ImageBase64 ()
		{
			if ( m_pBuffer )
			{
				delete [] m_pBuffer;
				m_pBuffer		=	NULL;

				m_nBufferSize	=	0L;
			}
		}

		inline bool FromString (const std::wstring& Src)
		{
			/*
			if ( m_pBuffer )
			{
				RELEASEOBJECT(m_pBuffer);

				m_nBufferSize	=	0L;
			}

			CString BASE64	=	CString ( _T("base64") );
			int to			=	Src.Find ( BASE64, 0 );
			if ( -1 != to )
			{
				to				=	Src.Find ( _T(","), to );
				CString Options	=	Src.Mid ( 0, to + 1 );

				int EncoderType	=	INVALID_ENCODER;
				if ( -1 != Options.Find ( _T("jpeg"), 0 ) )
					EncoderType	=	JPG_ENCODER;

				if ( -1 != Options.Find ( _T("png"), 0 ) )
					EncoderType	=	PNG_ENCODER;

				if ( INVALID_ENCODER != EncoderType )
				{
					CString buffer	=	Src.Mid ( Options.GetLength (), Src.GetLength () - Options.GetLength () );

					int Ind = -1;
					while ( 0 != Ind )
						Ind = buffer.Remove ( _T(' ') );		//	в некоторых SVG были пробелы

					return CreateImage ( (BYTE*) ( buffer.GetBuffer () ), buffer.GetLength (), EncoderType );
				}
			}
			*/

			return false;
		}

	private:

		bool CreateImage (BYTE* pBuffer, unsigned long Length, int ImageEncoder = INVALID_ENCODER)
		{
			return false;
		}

	private:

		BYTE*			m_pBuffer;
		int				m_nBufferSize;
	};
}

namespace SVG
{
	class RefElement : public ISvgRef
	{
	public:
		virtual ~RefElement()
		{

		}

		virtual bool FromXml (XmlUtils::CXmlNode& oXml, IRefStorage* model, UnitSystem& oUs)
		{
			m_XLinkRef				=	oXml.GetAttribute(L"xlink:href");
			m_nodeId				=	oXml.GetAttribute(L"id");
			m_model					=	model;

			m_gradientTransform		=	Matrix(oXml.GetAttribute(L"gradientTransform"), oUs);

			if (!m_XLinkRef.empty())
			{
				StrUtils::RemoveSpaces(m_XLinkRef);

				if ('#' == m_XLinkRef.c_str()[0])
				{
					m_XLinkRef = m_XLinkRef.substr(1);
				}
			}

			return false;
		}
		virtual bool FromString (std::wstring Str, IRefStorage* pStorage)
		{
			return false;
		}

		virtual bool Normalize (const double& dAddFMX, const double& dAddFMY) = 0;
	protected:
		inline static double DoubleValue (XmlUtils::CXmlNode& oXml, std::wstring oAttr, UnitSystem& oUs, long direction)
		{
			std::wstring sVal = oXml.GetAttribute(oAttr);
			if (!sVal.empty())
			{
				if ('%' == sVal.c_str()[sVal.length() - 1])
				{
					return 0.01 * std::stod(sVal.substr(0, sVal.length() - 1));
				}
			}

			double dVal				=	StrUtils::DoubleValue(sVal);
			SVG::Metrics oValMet	=	StrUtils::GetMetrics(sVal);

			return oUs.Convert(dVal, oValMet, direction);
		}

	protected:

		IRefStorage*	m_model;
		std::wstring	m_XLinkRef;

		Matrix			m_gradientTransform;
	};
	class LinearGradient : public RefElement
	{
	public:
		LinearGradient ()
		{
			m_nodeType	=	ELinearGradient;
			m_pClip		=	NULL;

			m_begin		=	Point(0, 0);
			m_end		=	Point(1, 0);
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, IRefStorage* model, UnitSystem& oUs)
		{
			RefElement::FromXml(oXml, model, oUs);

			if (!oXml.GetAttributeOrValue(L"x1").empty())
				m_begin.X	=	DoubleValue(oXml, L"x1", oUs, c_dirHorizontal);
			if (!oXml.GetAttributeOrValue(L"y1").empty())
				m_begin.Y	=	DoubleValue(oXml, L"y1", oUs, c_dirVertical);

			if (!oXml.GetAttributeOrValue(L"x2").empty())
				m_end.X		=	DoubleValue(oXml, L"x2", oUs, c_dirHorizontal);
			if (!oXml.GetAttributeOrValue(L"y2").empty())
				m_end.Y		=	DoubleValue(oXml, L"y2", oUs, c_dirVertical);

			m_gradientUnits	=	oXml.GetAttributeOrValue(L"gradientUnits", L"");

			XmlUtils::CXmlNodes comList;
			if (oXml.GetNodes(L"*", comList))
			{
				for (int i = 0; i < comList.GetCount(); ++i)
				{
					XmlUtils::CXmlNode oXml2;
					if (comList.GetAt(i, oXml2))
					{
						if (L"stop" == oXml2.GetName())
						{
							m_Color.Add(oXml2);
						}
					}
				}
			}

			return false;
		}
		virtual bool FromString(const std::wstring& Str, IRefStorage* model, UnitSystem& oUs)
		{
			return false;
		}
		virtual bool RefreshXLinkSource(IRefStorage* model)
		{
			if (!m_XLinkRef.empty())
			{
				ISvgRef* pDef	=	NULL;
				if (model->GetRef(m_XLinkRef, pDef))
				{
					if (ELinearGradient ==  pDef->nodeType())
					{
						LinearGradient* gradient = static_cast<LinearGradient*>(pDef);
						if (gradient)
						{
							for (int i = 0; i < gradient->GetColors().Count(); ++i)
								m_Color.Add(gradient->GetColors().Get(i));

							return true;
						}
					}
				}
			}

			return false;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			return false;
		}

		//
		inline const GradientColor& GetColors()
		{
			return m_Color;
		}
		inline const std::wstring& GetXml()
		{
			if (m_sXml.empty())
			{
				NSStringUtils::CStringBuilder builder;
				builder.WriteString(L"<linearGradient x1='");
				builder.AddDouble(m_begin.X, 5);
				builder.WriteString(L"' y1='");
				builder.AddDouble(m_begin.Y, 5);
				builder.WriteString(L"' x2='");
				builder.AddDouble(m_end.X, 5);
				builder.WriteString(L"' y2='");
				builder.AddDouble(m_end.Y, 5);
				builder.WriteString(L"' gradientUnits ='");
				builder.WriteString(m_gradientUnits);
				builder.WriteString(L"' gradientTransform='matrix(");
				builder.AddDouble(m_gradientTransform[0], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[1], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[2], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[3], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[4], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[5], 5);
				builder.WriteString(L")' sourceType='svg' >");

				for (int i = 0; i < m_Color.Count(); ++i)
				{
					builder.WriteString(L"<stop stop-color='");
					builder.AddInt(m_Color.Get(i).m_nColor);
					builder.WriteString(L"' offset='");
					builder.AddDouble(m_Color.Get(i).m_fOffset, 5);
					builder.WriteString(L"' stop-opacity='");
					builder.AddDouble(m_Color.Get(i).m_fOpacity, 5);
					builder.WriteString(L"'/>");
				}

				builder.WriteString(L"</linearGradient>");

				m_sXml = builder.GetBuffer();
			}

			return m_sXml;
		}

		inline void ToRenderer(IRenderer* renderer)
		{
			double dAngle = 0;
			if (fabs(m_end.X - m_begin.X) >= 0.00001 || fabs(m_end.Y - m_begin.Y) >= 0.00001)
				dAngle = 180 * atan2(m_end.Y - m_begin.Y, m_end.X - m_begin.X) / M_PI;

			renderer->put_BrushType(/*c_BrushTypePathGradient1*/2006);
			renderer->put_BrushLinearAngle(dAngle);

			int nCount = m_Color.Count();
			LONG* pColors = new LONG[nCount];
			double* pPositions = new double[nCount];

			for (int i = 0; i < nCount; ++i)
			{
				LONG c = m_Color.Get(i).m_nColor;
				LONG a = (LONG)(m_Color.Get(i).m_fOpacity * 255);
				if (a < 0) a = 0;
				if (a > 255) a = 255;
				pColors[i] = c | (a << 24);
				pPositions[i] = m_Color.Get(i).m_fOffset;
			}

			renderer->put_BrushGradientColors(pColors, pPositions, nCount);

			delete[] pColors;
			delete[] pPositions;
		}

	private:

		GradientColor	m_Color;
		std::wstring	m_sXml;
		std::wstring	m_gradientUnits;

		Point			m_begin;
		Point			m_end;
	};
	class RadialGradient : public RefElement
	{
	public:
		RadialGradient ()
		{
			m_nodeType	=	ERadialGradient;
			m_pClip		=	NULL;
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, IRefStorage* pStorage, UnitSystem& oUs)
		{
			RefElement::FromXml(oXml, pStorage, oUs);

			m_c.X	=	DoubleValue(oXml, L"cx", oUs, c_dirHorizontal);
			m_c.Y	=	DoubleValue(oXml, L"cy", oUs, c_dirVertical);

			m_f.X	=	DoubleValue(oXml, L"fx", oUs, c_dirHorizontal);
			m_f.Y	=	DoubleValue(oXml, L"fy", oUs, c_dirVertical);

			m_R		=	DoubleValue(oXml, L"r", oUs, c_dirHorizontal);

			m_gradientUnits	= oXml.GetAttributeOrValue(L"gradientUnits", L"");

			XmlUtils::CXmlNodes comList;
			if (oXml.GetNodes(L"*", comList))
			{
				for (int i = 0; i < comList.GetCount(); ++i)
				{
					XmlUtils::CXmlNode oXml2;
					if (comList.GetAt(i, oXml2))
					{
						if (L"stop" == oXml2.GetName())
						{
							m_Color.Add(oXml2);
						}
					}
				}
			}

			return false;
		}
		virtual bool FromString(const std::wstring& Str, IRefStorage* pStorage, UnitSystem& oUs)
		{
			return false;
		}
		virtual bool RefreshXLinkSource(IRefStorage* pStorage)
		{
			if (!m_XLinkRef.empty())
			{
				ISvgRef* pDef = NULL;
				if (pStorage->GetRef (m_XLinkRef, pDef))
				{
					if (ELinearGradient == pDef->nodeType())
					{
						LinearGradient* gradient = static_cast<LinearGradient*>(pDef);
						if (gradient)
						{
							for (int i = 0; i < gradient->GetColors().Count(); ++i)
								m_Color.Add(gradient->GetColors().Get(i));

							return true;
						}
					}
				}
			}

			return false;
		}
		virtual bool Normalize(const double& dAddFMX, const double& dAddFMY)
		{
			return false;
		}

		//

		inline const GradientColor& GetColors() const
		{
			return m_Color;
		}
		inline const std::wstring& GetXml()
		{
			if (m_sXml.empty())
			{
				NSStringUtils::CStringBuilder builder;
				builder.WriteString(L"<radialGradient cx='");
				builder.AddDouble(m_c.X, 5);
				builder.WriteString(L"' cy='");
				builder.AddDouble(m_c.Y, 5);
				builder.WriteString(L"' fx='");
				builder.AddDouble(m_f.X, 5);
				builder.WriteString(L"' fy='");
				builder.AddDouble(m_f.Y, 5);
				builder.WriteString(L"' r='");
				builder.AddDouble(m_R, 5);
				builder.WriteString(L"' gradientUnits ='");
				builder.WriteString(m_gradientUnits);
				builder.WriteString(L"' gradientTransform='matrix(");
				builder.AddDouble(m_gradientTransform[0], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[1], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[2], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[3], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[4], 5);
				builder.WriteString(L", ");
				builder.AddDouble(m_gradientTransform[5], 5);
				builder.WriteString(L")' sourceType='svg' >");

				for (int i = 0; i < m_Color.Count(); ++i)
				{
					builder.WriteString(L"<stop stop-color='");
					builder.AddInt(m_Color.Get(i).m_nColor);
					builder.WriteString(L"' offset='");
					builder.AddDouble(m_Color.Get(i).m_fOffset, 5);
					builder.WriteString(L"' stop-opacity='");
					builder.AddDouble(m_Color.Get(i).m_fOpacity, 5);
					builder.WriteString(L"'/>");
				}

				builder.WriteString(L"</radialGradient>");

				m_sXml = builder.GetBuffer();
			}

			return m_sXml;
		}

		inline void ToRenderer(IRenderer* renderer)
		{
			renderer->put_BrushType(/*c_BrushTypePathGradient2*/2007);

			int nCount = m_Color.Count();
			LONG* pColors = new LONG[nCount];
			double* pPositions = new double[nCount];

			for (int i = 0; i < nCount; ++i)
			{
				LONG c = m_Color.Get(i).m_nColor;
				LONG a = (LONG)(m_Color.Get(i).m_fOpacity * 255);
				if (a < 0) a = 0;
				if (a > 255) a = 255;
				pColors[i] = c | (a << 24);
				pPositions[i] = m_Color.Get(i).m_fOffset;
			}

			renderer->put_BrushGradientColors(pColors, pPositions, nCount);

			delete[] pColors;
			delete[] pPositions;
		}

	private:

		std::wstring	m_gradientUnits;
		GradientColor	m_Color;
		Point			m_c;
		Point			m_f;
		double			m_R;
		std::wstring	m_sXml;
	};
}

namespace SVG
{
	class DrawElement : public ISvgRef
	{
	public:
		virtual ~DrawElement()
		{

		}

		virtual bool FromXml (XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			m_nodeId		=	oXml.GetAttribute(L"id");
			m_className		=	oXml.GetAttribute(L"class");

			m_pClip			=	NULL;
			m_urlClipPath	=	oXml.GetAttribute(L"clip-path");
			StrUtils::UrlRefValue2(m_urlClipPath);

			return true;
		}
		virtual bool FromXml (std::wstring sXml, UnitSystem& oUs)
		{
			return false;
		}

		//
		virtual void SetStyle(const Style& oStyle)
		{
			m_oStyle = oStyle;
		}
		virtual const Style& GetStyle()
		{
			return m_oStyle;
		}
		virtual void SetTransform (const Matrix& oTransform)
		{
			m_oTransform	=	oTransform;
		}
		virtual const Matrix& GetTransform () const
		{
			return m_oTransform;
		}
		virtual const std::wstring& ClassName ()	const
		{
			return m_className;
		}

		//
		virtual bool Normalize (const double& dAddFMX, const double& dAddFMY)
		{

			return false;
		}

		// link
		virtual bool RefreshXLinkSource (IRefStorage* pStorage)
		{
#ifdef _DEBUG
			// ATLTRACE (L"NOT IMPLEMENTED - RefreshXLinkSource: id - %s\n", nodeId());
#endif
			return false;
		}
	protected:

		inline static double DoubleValue (XmlUtils::CXmlNode& oXml, std::wstring oAttr, UnitSystem& oUs, long direction)
		{
			double dVal				=	StrUtils::DoubleValue(oXml.GetAttribute(oAttr));
			SVG::Metrics oValMet	=	StrUtils::GetMetrics(oXml.GetAttribute(oAttr));

			return oUs.Convert(dVal, oValMet, direction);
		}

	protected:
		Style	m_oStyle;
		Matrix	m_oTransform;
		std::wstring m_className;
	};
	class Line : public DrawElement
	{
	public:
		Line ()
		{
			m_nodeType	=	ELine;
			m_pClip		=	NULL;
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml (oXml, oUs);

			m_From.X	=	DoubleValue(oXml, L"x1", oUs, c_dirHorizontal);
			m_From.Y	=	DoubleValue(oXml, L"y1", oUs, c_dirVertical);
			m_To.X		=	DoubleValue(oXml, L"x2", oUs, c_dirHorizontal);
			m_To.Y		=	DoubleValue(oXml, L"y2", oUs, c_dirVertical);

			return false;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			m_From	*=	Point(dAfX, dAfY);
			m_To	*=	Point(dAfX, dAfY);

			return true;
		}

		inline const Point& GetFrom() const
		{
			return	m_From;
		}
		inline const Point& GetTo() const
		{
			return	m_To;
		}

	private:

		Point m_From;
		Point m_To;
	};
	class Rectangle : public DrawElement
	{
	public:
		Rectangle ()
		{
			m_nodeType	=	ERectangle;
			m_pClip		=	NULL;

			m_Arc.X		=	0.0;
			m_Arc.Y		=	0.0;
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml(oXml, oUs);

			m_From.X	=	DoubleValue(oXml, L"x", oUs, c_dirHorizontal);
			m_From.Y	=	DoubleValue(oXml, L"y", oUs, c_dirVertical);
			m_To.X		=	m_From.X + DoubleValue(oXml, L"width",  oUs, c_dirHorizontal);
			m_To.Y		=	m_From.Y + DoubleValue(oXml, L"height", oUs, c_dirVertical);

			m_Arc.X		=	DoubleValue(oXml, L"rx", oUs, c_dirHorizontal);
			m_Arc.Y		=	DoubleValue(oXml, L"ry", oUs, c_dirVertical);

			return true;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			m_From	*=	Point(dAfX, dAfY);
			m_To	*=	Point(dAfX, dAfY);
			m_Arc	*=	Point(dAfX, dAfY);

			return false;
		}

		inline const Point& GetFrom() const
		{
			return	m_From;
		}
		inline const Point& GetTo() const
		{
			return	m_To;
		}
		inline const Point& GetArc() const
		{
			return	m_Arc;
		}

	private:

		Point m_From;
		Point m_To;

		Point m_Arc;
	};
	class Ellipse : public DrawElement
	{
	public:
		Ellipse ()
		{
			m_nodeType	=	EEllipse;
			m_pClip		=	NULL;
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml(oXml, oUs);

			m_C.X	=	DoubleValue(oXml, L"cx", oUs, c_dirHorizontal);
			m_C.Y	=	DoubleValue(oXml, L"cy", oUs, c_dirVertical);
			m_R.X	=	DoubleValue(oXml, L"rx", oUs, c_dirHorizontal);
			m_R.Y	=	DoubleValue(oXml, L"ry", oUs, c_dirVertical);

			return true;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			m_C	*=	Point(dAfX, dAfY);
			m_R	*=	Point(dAfX, dAfY);

			return true;
		}

	public:
		Point	m_C;
		Point	m_R;
	};
	class Circle : public DrawElement
	{
	public:
		Circle ()
		{
			m_nodeType	=	ECircle;
			m_pClip		=	NULL;
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml (oXml, oUs);

			m_C.X	=	DoubleValue(oXml, L"cx", oUs, c_dirHorizontal);
			m_C.Y	=	DoubleValue(oXml, L"cy", oUs, c_dirVertical);
			m_R.X	=	DoubleValue(oXml, L"r",  oUs, c_dirHorizontal);
			m_R.Y	=	m_R.X;

			return true;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			m_C	*=	Point(dAfX, dAfY);
			m_R	*=	Point(dAfX, dAfY);

			return true;
		}

	public:

		Point	m_C;
		Point	m_R;
	};
	class Path : public DrawElement
	{
	public:
		Path ()
		{
			m_nodeType	=	EPath;
			m_pClip		=	NULL;
		}

		virtual bool FromXml (XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml(oXml, oUs);

			m_oUs = oUs;

			m_oParser.FromString(oUs, oXml.GetAttribute(L"d"));

			return false;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			long count = (long)m_oParser.GetPoints().size();
			for (long i = 0; i < count; ++i)
			{
				Point oPoint	=	m_oParser.GetPoint(i);
				oPoint			*=	Point (dAfX, dAfY);
				m_oParser.SetPoint(i, oPoint);
			}

			return true;
		}

		inline PathParser& GetPath ()
		{
			return m_oParser;
		}
		inline double GetX (long index)
		{
			return m_oParser.GetPoints()[index].oPoint.X;
		}
		inline double GetY (long index)
		{
			return m_oParser.GetPoints()[index].oPoint.Y;
		}
		inline const Point& Get (long index)
		{
			return m_oParser.GetPoints()[index].oPoint;
		}

		inline int GetCode (long index)
		{
			return m_oParser.GetPoints()[index].code;
		}
		inline int GetSubCode (long index)
		{
			return m_oParser.GetPoints()[index].subCode;
		}
		inline long GetSize ()
		{
			return (long)m_oParser.GetPoints().size();
		}

	private:

		inline static double DoubleValue(UnitSystem& oUnitSystem, double Value, long Direction)
		{
			return	oUnitSystem.Convert(Value, UNDEFINED, Direction);
		}

	private:

		PathParser	m_oParser;
		UnitSystem	m_oUs;
		std::wstring m_ClipRule;
	};
	class Polyline : public DrawElement
	{
	public:
		Polyline ()
		{
			m_nodeType	=	EPolyline;
			m_pClip		=	NULL;
		}

		virtual bool FromXml (XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml (oXml, oUs);
			m_points.clear ();
			StrUtils::DoubleValues(oXml.GetAttribute(L"points"), m_points);

			for (size_t i = 0; i < m_points.size(); i += 2)
			{
				m_points[i]		=	DoubleValue(oUs, m_points[i], c_dirHorizontal);
				m_points[i+1]	=	DoubleValue(oUs, m_points[i+1], c_dirVertical);
			}

			return (m_points.size() > 0);
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			for (size_t i = 0; i < m_points.size(); i += 2)
			{
				m_points[i]		*=	dAfX;
				m_points[i + 1]	*=	dAfY;
			}

			return true;
		}

		inline double GetX (long index) const
		{
			return m_points[index * 2];
		}
		inline double GetY (long index) const
		{
			return m_points[index * 2 + 1];
		}
		inline long GetSize () const
		{
			return (long)m_points.size() / 2;
		}

	private:

		inline static double DoubleValue(UnitSystem& oUnitSystem, double Value, long Direction)
		{
			return oUnitSystem.Convert(Value, UNDEFINED, Direction);
		}

	protected:

		std::vector <double> m_points;
	};
	class Polygon : public Polyline
	{
	public:
		Polygon ()
		{
			m_nodeType	=	EPolygon;
			m_pClip		=	NULL;
		}

		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			for (size_t i = 0; i < m_points.size(); i += 2)
			{
				m_points[i]		*=	dAfX;
				m_points[i + 1]	*=	dAfY;
			}

			return true;
		}
	};
	class Text : public DrawElement
	{
	public:
		Text ()
		{
			m_nodeType	=	EText;
			m_pClip		=	NULL;
		}

		virtual bool FromXml ( XmlUtils::CXmlNode& oXmlNode, UnitSystem& oUnitSystem )
		{
			DrawElement::FromXml ( oXmlNode, oUnitSystem );

			m_oUs	=	oUnitSystem;

			m_Pos.X			=	DoubleValue ( oXmlNode, L"x", oUnitSystem, c_dirHorizontal );
			m_Pos.Y			=	DoubleValue ( oXmlNode, L"y", oUnitSystem, c_dirVertical );
			m_Shift.X		=	DoubleValue ( oXmlNode, L"dx", oUnitSystem, c_dirHorizontal );
			m_Shift.Y		=	DoubleValue ( oXmlNode, L"dy", oUnitSystem, c_dirVertical );

			m_Source		=	oXmlNode.GetText ();

			// стиль может задаваться вообще где то вверху по дереву
			m_oFontStyle.UpdateStyle ( oXmlNode );

			return true;
		}
		virtual bool Normalize ( const double& dAddFMX, const double& dAddFMY )
		{
			m_Pos	*=	Point ( dAddFMX, dAddFMY );
			m_Shift	*=	Point ( dAddFMX, dAddFMY );

			return true;
		}

		inline const FontStyle& GetFontStyle ()
		{
			return m_oFontStyle;
		}
		inline const std::wstring& GetText ()
		{
			return m_Source;
		}

	public:

		Point		m_Pos;
		Point		m_Shift;

		std::wstring m_Source;
		FontStyle	m_oFontStyle;

		UnitSystem	m_oUs;
	};
	class Image : public DrawElement
	{
	protected:
		class CImageManager
		{
		public:
			CImageManager()
			{
			}
		public:

			static bool IsNeedDownload(const std::wstring& FilePath)
			{
				std::wstring::size_type n1 = FilePath.find(L"www.");
				std::wstring::size_type n2 = FilePath.find(L"http://");
				std::wstring::size_type n3 = FilePath.find(L"ftp://");
				std::wstring::size_type n4 = FilePath.find(L"https://");

				if (n1 != std::wstring::npos && n1 < 10)
					return true;
				if (n2 != std::wstring::npos && n2 < 10)
					return true;
				if (n3 != std::wstring::npos && n3 < 10)
					return true;
				if (n4 != std::wstring::npos && n4 < 10)
					return true;

				return false;
			}

			inline std::wstring GenerateImage(const std::wstring& strInput)
			{
				/*
				if (IsNeedDownload(strInput))
					return DownloadImage(strInput);

				BOOL doCopyFile	=	FALSE;

				CString strExts = _T(".jpg");
				int nIndexExt = strInput.ReverseFind(TCHAR('.'));
				if (-1 != nIndexExt)
					strExts = strInput.Mid(nIndexExt);

				if (strExts == _T(".tmp"))
				{
					strExts		=	_T(".png");
					doCopyFile	=	TRUE;
				}

				if ( FALSE == doCopyFile )
					return strInput;

				TCHAR sTempPath[MAX_PATH];
				if ( 0 == GetTempPath ( MAX_PATH, sTempPath ) )
					return strInput;

				SYSTEMTIME oTime;
				GetSystemTime ( &oTime );

				CString fileName;
				fileName.Format (_T("%s\\%d%d%d%d%d%d%s"), sTempPath, oTime.wYear, oTime.wDay, oTime.wHour, oTime.wMinute, oTime.wSecond, oTime.wMilliseconds, strExts );

				CImageManager::CopyFile(strInput, fileName, NULL, NULL);

				return fileName;
				*/
				// TODO
				return L"";
			}
		};

	public:
		Image ()
		{
			m_nodeType	=	EImage;
			m_pClip		=	NULL;
		}

		virtual bool FromXml (XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml (oXml, oUs);

			m_Or.X					=	DoubleValue(oXml, L"x",		oUs, c_dirHorizontal);
			m_Or.Y					=	DoubleValue(oXml, L"y",		oUs, c_dirVertical);
			m_Size.X				=	DoubleValue(oXml, L"width",	oUs, c_dirHorizontal);
			m_Size.Y				=	DoubleValue(oXml, L"height", oUs, c_dirVertical);

			m_sPreserveAspectRatio	=	oXml.GetAttributeOrValue(L"preserveAspectRatio", L"");

			m_XLinkRef	=	oXml.GetAttribute(L"xlink:href");
			if (m_XLinkRef.empty())
				m_XLinkRef	=	oXml.GetAttribute(L"href");
			if (!m_XLinkRef.empty())
			{
				StrUtils::RemoveSpaces (m_XLinkRef);

				/*

				if ( _T('#') == m_XLinkRef [0] )
				{

				}

				*/

				if (m_XLinkRef.length() > 4)
				{
					CImageManager onlineImage;
					if (onlineImage.IsNeedDownload(m_XLinkRef))
					{
						m_ImagePath = onlineImage.GenerateImage(m_XLinkRef);
						if ( m_ImagePath.length() > 1 )
							return true;
					}

					const wchar_t* buf = m_XLinkRef.c_str();
					if ('d' == buf[0] && 'a' == buf[1] && 't' == buf[2] && 'a' == buf[3])
					{
						m_bin64Image.FromString(m_XLinkRef);
					}
					else
					{
						/*
						m_ImagePath		=	StrUtils::UrlDecode (m_XLinkRef).MakeLower();

						CString URLFile	=	CString(_T("file:///"));
						int to			=	m_ImagePath.Find(URLFile, 0);
						if (-1 != to)
						{
							m_ImagePath	=	m_ImagePath.Mid(URLFile.GetLength(), m_ImagePath.GetLength() - to - URLFile.GetLength());
						}
						*/
						// TODO
					}
				}
			}

			return true;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			m_Or	*=	Point(dAfX, dAfY);
			m_Size	*=	Point(dAfX, dAfY);

			return true;
		}

		inline const Point& GetFrom() const
		{
			return	m_Or;
		}
		inline const Point& GetSize() const
		{
			return	m_Size;
		}
		inline const std::wstring& GetXLink() const
		{
			return m_XLinkRef;
		}

		inline std::wstring LivePath(const std::wstring& sWorkingDirectory)
		{
			std::wstring sFile = GetXLink();

			if (!sFile.empty())
			{
				if (!NSFile::CFileBinary::Exists(sFile) && NSFile::CFileBinary::Exists(sWorkingDirectory + L"/" + sFile))
				{
					return sWorkingDirectory + L"/" + sFile;
				}
				return sFile;
			}

			return m_ImagePath;
		}

		inline const std::wstring& PreserveAspectRatio() const
		{
			return m_sPreserveAspectRatio;
		}

	public:

		Point			m_Or;
		Point			m_Size;

		std::wstring	m_sPreserveAspectRatio;

		std::wstring	m_XLinkRef;
		std::wstring	m_ImagePath;

		ImageBase64		m_bin64Image;
	};

	class Use : public DrawElement
	{
	public:
		Use()
		{
			m_nodeType		=	EUse;
			m_pClip			=	NULL;

			m_bBrokeRefLink	=	false;
			m_pRefLink		=	NULL;
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, UnitSystem& oUs)
		{
			DrawElement::FromXml (oXml, oUs);

			m_bBrokeRefLink	=	false;

			m_From.X		=	DoubleValue(oXml, L"x", oUs, c_dirHorizontal);
			m_From.Y		=	DoubleValue(oXml, L"y", oUs, c_dirVertical);
			m_To.X			=	DoubleValue(oXml, L"width", oUs, c_dirHorizontal) + m_From.X;
			m_To.Y			=	DoubleValue(oXml, L"height", oUs, c_dirVertical) + m_From.Y;

			m_XLinkRef		=	oXml.GetAttribute(L"xlink:href");

			if (!m_XLinkRef.empty())
			{
				StrUtils::RemoveSpaces (m_XLinkRef);
				if (!m_XLinkRef.empty())
				{
					if ('#' == m_XLinkRef.c_str()[0])
					{
						m_XLinkRef = m_XLinkRef.substr(1);
					}
				}
			}

			return true;
		}
		virtual bool RefreshXLinkSource (IRefStorage* pStorage)
		{
			if (NULL != m_pRefLink)		//	связь уже есть, нет смысла опять искать соотвествие
				return true;

			if (m_bBrokeRefLink)		//	нету связи
				return false;

			if (!m_XLinkRef.empty())
			{
				ISvgRef* pRefLink	=	NULL;
				if (pStorage->GetRef(m_XLinkRef, pRefLink))
				{
					m_pRefLink = pRefLink;

					// ATLTRACE(_T("RefLink for object : %s - %s\n"), nodeId(), m_pRefLink->nodeId());

					return true;
				}
			}

			m_bBrokeRefLink	= true;

			return false;
		}
		virtual bool Normalize(const double& dAddFMX, const double& dAddFMY)
		{

			return false;
		}

		inline ISvgRef* GetRefLink ()
		{
			return m_pRefLink;
		}

		inline const Point& GetFrom() const
		{
			return	m_From;
		}
		inline const Point& GetTo() const
		{
			return	m_To;
		}

		inline const std::wstring& GetXLink() const
		{
			return m_XLinkRef;
		}

	private:

		Point		m_From;
		Point		m_To;

		std::wstring m_XLinkRef;
		bool		m_bBrokeRefLink;
		ISvgRef*	m_pRefLink;
	};
}

namespace SVG
{
	class Painter;
	//
	class DrawBuilder
	{
	public:
		inline DrawElement* Build (const std::wstring& sName)
		{
			if (L"rect" == sName)			return new Rectangle();
			else if (L"ellipse" == sName)	return new Ellipse();
			else if (L"line" == sName)		return new Line();
			else if (L"circle" == sName)	return new Circle();
			else if (L"path" == sName)		return new Path();
			else if (L"polyline" == sName)	return new Polyline();
			else if (L"polygon" == sName)	return new Polygon();
			else if (L"text" == sName)		return new Text();
			else if (L"image" == sName)		return new Image();
			else if (L"use" == sName)		return new Use();

			return NULL;
		}
	};
	class Symbol : public RefElement	//	content element
	{
	public:
		Symbol()
		{
			m_nodeType	=	ESymbol;
			m_pClip		=	NULL;
		}
		virtual ~Symbol()
		{
			for (size_t i = 0; i < m_arrRefElems.size(); ++i)
				RELEASEOBJECT (m_arrRefElems[i]);

			m_arrRefElems.clear();
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXmlNode, IRefStorage* pStorage, UnitSystem& oUs)
		{
			RefElement::FromXml (oXmlNode, pStorage, oUs);

			m_oViewBox.FromXml (oXmlNode);

			return true;
		}

		inline void AddContent(DrawElement* pReference)	//	храним данные, поэтому сами и очищаем данные
		{
			if (pReference)
			{
				m_arrRefElems.push_back(pReference);
			}
		}
		inline long GetCount() const
		{
			return (long)m_arrRefElems.size();
		}
		inline DrawElement* GetAt(long nInd)
		{
			return m_arrRefElems[nInd];
		}

		// RefElement

		virtual bool RefreshXLinkSource(IRefStorage* pStorage)						//	NOT IMPLEMENTED
		{
			return 0;
		}
		virtual bool Normalize(const double& dAddFMX, const double& dAddFMY)		//	NOT IMPLEMENTED
		{
			return 0;
		}

	public:

		ViewBox					m_oViewBox;
		std::vector<DrawElement*>m_arrRefElems;
	};
	class ClipPath : public RefElement	//	content element
	{
	public:
		ClipPath()
		{
			m_nodeType	=	EClipPath;
			m_pClip		=	NULL;
		}
		virtual ~ClipPath()
		{
			for (size_t i = 0; i < m_clips.size(); ++i)
				RELEASEOBJECT (m_clips[i]);

			m_clips.clear();
		}

		virtual bool FromXml (XmlUtils::CXmlNode& oXml, IRefStorage* pStorage, UnitSystem& oUs)
		{
			RefElement::FromXml (oXml, pStorage, oUs);
			Explore(oXml);

			return true;
		}
		inline long GetCount ()
		{
			return (long)m_clips.size();
		}
		inline ISvgRef* GetAt (long nInd)
		{
			return m_clips[nInd];
		}


		// RefElement
		virtual bool RefreshXLinkSource (IRefStorage* pStorage)						//	NOT IMPLEMENTED
		{
			return 0;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			for (size_t i = 0; i < m_clips.size(); ++i)
				m_clips[i]->Normalize(dAfX, dAfY);

			return true;
		}

	protected:

		inline bool Explore (XmlUtils::CXmlNode& oXml)
		{
			Load (oXml);

			XmlUtils::CXmlNodes oXmlNodes;
			if (oXml.GetNodes(L"*", oXmlNodes))
			{
				for (int i = 0; i < oXmlNodes.GetCount(); ++i)
				{
					XmlUtils::CXmlNode oChild;
					if (oXmlNodes.GetAt(i, oChild))
					{
						if (!Explore(oChild))
						{
							return false;
						}
					}
				}
			}

			return true;
		}
		inline bool Load (XmlUtils::CXmlNode& oXml)
		{
			if (L"clipPath" == oXml.GetName())
				return false;
#ifdef _DEBUG
			//ATLTRACE (L"[svg] ClipPath-NodeName : %s\n", oXml.GetName());
#endif

			DrawElement* pClip = m_oBuilder.Build(oXml.GetName());
			if (pClip)
			{
				pClip->FromXml(oXml,m_oUs);
				m_clips.push_back(pClip);
				return true;
			}

			return false;
		}

	protected:

		std::vector <DrawElement*>	m_clips;
		DrawBuilder					m_oBuilder;
		UnitSystem					m_oUs;
	};
	class Pattern : public RefElement, public IRefStorage	//	content element
	{
	public:
		Pattern()
		{
			m_nodeType	=	EPattern;
			m_pClip		=	NULL;
		}
		virtual ~Pattern()
		{
			for (size_t i = 0; i < m_elements.size(); ++i)
				RELEASEOBJECT (m_elements[i]);

			m_elements.clear();
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXml, IRefStorage* model, UnitSystem& oUs)
		{
			m_rect.X				=	DoubleValue(oXml, L"x", oUs, c_dirHorizontal);
			m_rect.Y				=	DoubleValue(oXml, L"y", oUs, c_dirVertical);
			m_rect.Width			=	DoubleValue(oXml, L"width",  oUs, c_dirHorizontal);
			m_rect.Height			=	DoubleValue(oXml, L"height", oUs, c_dirVertical);

			m_patternUnits			=	oXml.GetAttributeOrValue(L"patternUnits", L"");
			m_preserveAspectRatio	=	oXml.GetAttributeOrValue(L"preserveAspectRatio", L"");

			m_model					=	model;

			RefElement::FromXml (oXml, model, oUs);
			m_oViewBox.FromXml (oXml);
			Explore(oXml);

			//Pattern::FromXml (oXml, m_model, oUs);

			return true;
		}

		// RefElement
		virtual bool RefreshXLinkSource(IRefStorage* pStorage)						//	NOT IMPLEMENTED
		{
			return 0;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			//for (size_t i = 0; i < m_elements.GetCount(); ++i)
			//	m_elements[i]->Normalize(dAfX, dAfY);

			return true;
		}

		//
		virtual ISvgRef* Get(long index)
		{
			return m_elements[index];
		}

		virtual long GetSize()
		{
			return (long)m_elements.size();
		}
		// NOT IMPLEMENTED
		virtual bool HashRef(ISvgRef* pRef, bool bDef)
		{
			return false;
		}
		// NOT IMPLEMENTED
		virtual bool GetRef(const std::wstring& ID, ISvgRef*& pRef)
		{
			return false;
		}
		// NOT IMPLEMENTED
		virtual bool Push(ISvgRef* pRef)
		{
			return false;
		}

		//
		inline const ViewBox& GetBox() const
		{
			return m_oViewBox;
		}
		inline const Rect& GetBound() const
		{
			return m_rect;
		}
		inline const std::wstring& PreserveAspectRatio() const
		{
			return m_preserveAspectRatio;
		}

		inline bool NormalizeEx(const double& dAfX, const double& dAfY)
		{
			for (size_t i = 0; i < m_elements.size(); ++i)
				m_elements[i]->Normalize(dAfX, dAfY);

			return true;
		}

	protected:

		inline bool Explore(XmlUtils::CXmlNode& oXml)
		{
			Load (oXml);

			XmlUtils::CXmlNodes oXmlNodes;
			if (oXml.GetNodes(L"*", oXmlNodes))
			{
				for (int i = 0; i < oXmlNodes.GetCount(); ++i)
				{
					XmlUtils::CXmlNode oChild;
					if (oXmlNodes.GetAt(i, oChild))
					{
						if (!Explore(oChild))
						{
							return false;
						}
					}
				}
			}

			return true;
		}
		inline bool Load(XmlUtils::CXmlNode& oXml)
		{
#ifdef _DEBUG
			//ATLTRACE (L"[svg] Pattern-NodeName : %s\n", oXml.GetName());
#endif

			DrawElement* element = m_oBuilder.Build (oXml.GetName());
			if (element)
			{
				Style oStyle = m_oStyle;
				FontStyle oFontStyle = m_oFontStyle;

				std::wstring css = oXml.GetAttribute(L"style");
				if (!css.empty())
				{
					oStyle.SetStyle(css, false, m_oUs, NULL, m_oColTable);
					oFontStyle.SetStyle(css, false);
				}
				else
				{
					oStyle.UpdateStyle(oXml, m_oUs, NULL, m_oColTable);
					oFontStyle.UpdateStyle(oXml);
				}

				m_oUs.SetFontSizeForEM(oFontStyle.DoubleAttribute(FontSize));

				element->SetStyle(oStyle);

				if (EText == element->nodeType())
				{
					Text* pText = static_cast<Text*>(element);
					if (pText)
					{
						pText->m_oFontStyle = oFontStyle;
					}
				}

				std::wstring transform = oXml.GetAttribute(L"transform");
				if (!transform.empty())
				{
					m_transforms.Push(Matrix(transform, m_oUs));

					element->SetTransform(m_transforms.GetTop());

					m_transforms.Pop();
				}
				else
				{
					element->SetTransform(m_transforms.GetTop());
				}

				element->FromXml(oXml,m_oUs);
				m_elements.push_back(element);
				return true;
			}

			return false;
		}

	protected:

		std::vector <DrawElement*>	m_elements;
		DrawBuilder					m_oBuilder;
		UnitSystem					m_oUs;
		ColorTable					m_oColTable;

		Style						m_oStyle;
		FontStyle					m_oFontStyle;
		MatrixStack					m_transforms;

		ViewBox						m_oViewBox;
		Rect						m_rect;
		std::wstring				m_patternUnits;
		std::wstring				m_preserveAspectRatio;
	};
	class PatternImage
	{
	public:
		PatternImage() : m_pFrame(NULL), m_pattern(NULL), m_baseRender(NULL), m_render(NULL), m_basePainter(NULL)
		{
			m_pPatternFrame	=	NULL;
		}
		PatternImage(Pattern* pattern, IRenderer* baseRender, Painter* basePainter) : m_pFrame(NULL), m_baseRender(baseRender), m_pattern(pattern), m_render(NULL), m_basePainter(basePainter)
		{
			m_pPatternFrame	=	NULL;
		}
		~PatternImage()
		{
			RELEASEINTERFACE(m_render);
			RELEASEOBJECT(m_pFrame);
			RELEASEOBJECT(m_pPatternFrame);

			if (!m_sLivePath.empty())
			{
				NSFile::CFileBinary::Remove(m_sLivePath);
			}
		}

		inline const std::wstring& LivePath()
		{
			Build();

			return m_sLivePath;
		}

	private:

		bool Build();
		bool InitFrame();
		bool InitPatternFrame();
		bool InitRender(Aggplus::CImage* frame, int frameWidth, int frameHeight);
		bool Render();
		bool SaveImage(Aggplus::CImage* pFrame, const std::wstring& file);
		Point GetNormalizeFactor();

	private:

		IRenderer* m_baseRender;
		NSGraphics::IGraphicsRenderer* m_render;

		Aggplus::CImage* m_pPatternFrame;
		Aggplus::CImage* m_pFrame;

		std::wstring m_sLivePath;
		Pattern*	m_pattern;
		Painter*	m_basePainter;
	};
	class DefsBuilder
	{
	public:
		RefElement* Build (const std::wstring& sName)
		{
			if (L"linearGradient" == sName)			return new LinearGradient ();
			else if (L"radialGradient" == sName)	return new RadialGradient ();
			else if (L"symbol"== sName)				return new Symbol();
			else if (L"clipPath" == sName)			return new ClipPath();
			else if (L"pattern" == sName)			return new Pattern();

			return NULL;
		}
	};
	class GraphicsContainer : public DrawElement, public IRefStorage
	{
	public:
		GraphicsContainer(bool bRefMode = false)		//	подчищает ссылки на объекты
		{
			m_nodeType		=	EGraphicsContainer;
			m_pClip			=	NULL;
			m_bRefMode		=	bRefMode;

			m_bAddNormMM	=	false;
		}
		virtual ~GraphicsContainer()
		{
			GraphicsContainer::Clear ();
		}

		virtual bool FromXml(XmlUtils::CXmlNode& oXmlNode, IRefStorage* model, const ViewBox& oViewBox, const UnitSystem& oUnitSystem, const Matrix& oTransform)
		{
			m_model			=	model;
			m_oViewBox		=	oViewBox;
			m_oUs			=	oUnitSystem;

			m_pClip			=	NULL;
			m_urlClipPath	=	oXmlNode.GetAttribute(L"clip-path");
			StrUtils::UrlRefValue2(m_urlClipPath);

			m_transforms.Push(oTransform);

			DrawElement::FromXml(oXmlNode, m_oUs);

			UpdateMainStyle(oXmlNode);
			UpdateMainTransform(oXmlNode);

			Explore(oXmlNode);

			m_model->HashRef(this, false);

			m_oTransform = m_transforms.GetFinal();

			return true;
		}

		virtual bool FromString(std::wstring Str, IRefStorage* model)
		{
			return false;
		}
		virtual bool RefreshXLinkSource(IRefStorage* model)
		{
			return false;
		}

		// storage
		inline ISvgRef* Get(long index)
		{
			if (index > (long)m_arrGroup.size())
				return NULL;

			return m_arrGroup[index];
		}
		inline long GetSize()
		{
			return (long)(m_arrGroup.size());
		}

		virtual bool Normalize(const double& dAddFMX, const double& dAddFMY)
		{
			if ( false == m_bAddNormMM )
			{
				for ( size_t i = 0; i < m_arrGroup.size (); ++i )
				{
					ISvgRef* pE	=	m_arrGroup [ i ];
					if ( pE )
					{
						pE->Normalize ( dAddFMX, dAddFMY );
					}
				}
			}

			m_bAddNormMM	=	true;

			return true;
		}

		inline void Add(ISvgRef* element)
		{
			if (m_bRefMode)
			{
				m_arrGroup.push_back(element);
			}
		}

	private:

		// NOT IMPLEMENTED
		virtual bool Push (ISvgRef* Elem)
		{
			return false;
		}
		virtual bool HashRef (ISvgRef* pElem, bool bDef)
		{
			return false;
		}
		virtual bool GetRef (const std::wstring& ID, ISvgRef*& pElem)
		{
			return false;
		}

	private:

		inline bool Explore (XmlUtils::CXmlNode& oXmlNode)
		{
			LoadElement ( oXmlNode );

			XmlUtils::CXmlNodes oXmlNodes;
			if ( oXmlNode.GetNodes ( L"*", oXmlNodes ) )
			{
				for ( int i = 0; i < oXmlNodes.GetCount(); ++i )
				{
					XmlUtils::CXmlNode oXmlNode2;
					if ( oXmlNodes.GetAt ( i, oXmlNode2 ) )
					{
						if ( L"g" == oXmlNode2.GetName() )
						{
							GraphicsContainer* pContainer	=	new GraphicsContainer();
							if ( pContainer )
							{
								pContainer->FromXml(oXmlNode2, m_model, m_oViewBox, m_oUs, m_transforms.GetFinal());
								m_model->HashRef(pContainer, false);

								m_arrGroup.push_back(pContainer);

								continue;
							}

							// return true;
						}

						if ( false == Explore ( oXmlNode2 ) )
						{
							return false;
						}
					}
				}
			}

			return true;
		}
		inline bool LoadElement (XmlUtils::CXmlNode& oXml, bool Defines = false)
		{
			std::wstring strXmlNode = oXml.GetName();

			RefElement* pReference = m_oDefsBuilder.Build(strXmlNode);
			if (pReference)
			{
				if (EPattern == pReference->nodeType())
				{
					((Pattern*)pReference)->FromXml(oXml, m_model, m_oUs);
				}
				else
				{
					pReference->FromXml(oXml, m_model, m_oUs);
				}

				if (m_model->HashRef(pReference, true))
					UpdateSymbol(pReference, oXml);
#ifdef _DEBUG
				//ATLTRACE(L"[svg] ref element (g) - : %s\n", strXmlNode);
#endif
				return true;
			}

			if (NULL == Create(oXml))
				return false;

			return true;
		}

		inline DrawElement* Create(XmlUtils::CXmlNode& oXml, bool bAddStorage = true)
		{
			std::wstring strXmlNode = oXml.GetName();
#ifdef _DEBUG
			//ATLTRACE(L"[svg] render element (g) - : %s\n", strXmlNode);
#endif
			DrawElement* element		=	m_oDrawBuilder.Build(strXmlNode);
			if (element)
			{
				Style oStyle			=	m_oStyle;
				FontStyle oFontStyle	=	m_oFontStyle;

				std::wstring css = oXml.GetAttribute(L"style");
				if (!css.empty())
				{
					oStyle.SetStyle(css, false, m_oUs, m_model, m_oColTable);
					oFontStyle.SetStyle(css, false);
				}
				else
				{
					oStyle.UpdateStyle(oXml, m_oUs, m_model, m_oColTable);
					oFontStyle.UpdateStyle(oXml);
				}

				m_oUs.SetFontSizeForEM(oFontStyle.DoubleAttribute(FontSize));

				element->SetStyle(oStyle);

				if (EText == element->nodeType())
				{
					Text* pText = static_cast<Text*>(element);
					if (pText)
					{
						pText->m_oFontStyle = oFontStyle;
					}
				}

				std::wstring transform = oXml.GetAttribute(L"transform");
				if (!transform.empty())
				{
					m_transforms.Push(Matrix(transform, m_oUs));

					element->SetTransform(m_transforms.GetTop());

					m_transforms.Pop();
				}
				else
				{
					element->SetTransform(m_transforms.GetTop());
				}

				element->FromXml(oXml, m_oUs);

				// ATLTRACE(L"GraphicsContainer : %s - push : %s\n", m_nodeId, oXmlNode.GetName());

				if (bAddStorage)
					m_arrGroup.push_back(element);

				return element;
			}

			return NULL;
		}

		inline void UpdateMainTransform (XmlUtils::CXmlNode& oXml)
		{
			std::wstring css = oXml.GetAttribute(L"transform");
			if (!css.empty())
			{
				Matrix oTransform(css, m_oUs);
				m_transforms.Push(oTransform);
			}
			else
			{
				m_transforms.Push(Matrix());
			}
		}
		inline void UpdateMainStyle (XmlUtils::CXmlNode& oXml)
		{
			std::wstring css = oXml.GetAttribute(L"style");
			if (!css.empty())
			{
				m_oFontStyle.SetStyle(css, true);
				m_oStyle.SetStyle(css, true, m_oUs, m_model, m_oColTable);
			}
			else
			{
				m_oFontStyle.UpdateStyle(oXml);
				m_oStyle.UpdateStyle(oXml, m_oUs, m_model, m_oColTable);
			}
		}
		inline void Clear()
		{
			if (!m_bRefMode)
			{
				for (size_t i = 0; i <m_arrGroup.size (); ++i)
					RELEASEOBJECT(m_arrGroup[i]);
			}

			m_arrGroup.clear();
		}

		//
		inline void UpdateSymbol(RefElement* pReference, XmlUtils::CXmlNode& oXml)
		{
			if (ESymbol == pReference->nodeType())
			{
				XmlUtils::CXmlNodes oXmlNodes;
				if (oXml.GetNodes(L"*", oXmlNodes))
				{
					for (int i = 0; i < oXmlNodes.GetCount(); ++i)
					{
						XmlUtils::CXmlNode oXml2;
						if (oXmlNodes.GetAt(i,oXml2))
						{
							((Symbol*)pReference)->AddContent(Create(oXml2, false));
						}
					}
				}
			}
		}

	private:

		bool					m_bRefMode;

		bool					m_bAddNormMM;

		std::vector <ISvgRef*>	m_arrGroup;

		IRefStorage*			m_model;
		DrawBuilder				m_oDrawBuilder;
		DefsBuilder				m_oDefsBuilder;

		ViewBox					m_oViewBox;
		UnitSystem				m_oUs;
		ColorTable				m_oColTable;

		Style					m_oStyle;
		FontStyle				m_oFontStyle;
		MatrixStack				m_transforms;
	};
}

namespace SVG
{	
	class Storage : public IRefStorage
	{
	public:
		Storage()
		{
			m_bAddNormMM	=	false;
		}
		virtual ~Storage ()
		{
			Clear ();
		}

		inline void Clear ()
		{
			for (size_t i = 0; i < m_arrGroup.size(); ++i)
			{
				RELEASEOBJECT(m_arrGroup[i]);
			}

			m_arrGroup.clear();
			m_arrRef.clear();

			for (size_t i = 0; i < m_arFlush.size(); ++i)
			{
				RELEASEOBJECT (m_arFlush[i]);
			}

			m_arFlush.clear();
		}
		inline bool JoinXLinkReference()
		{
			for (std::map<std::wstring, ISvgRef*>::iterator i = m_arrRef.begin(); i != m_arrRef.end(); i++)
			{
				ISvgRef* pE = static_cast<ISvgRef*>(i->second);
				if (pE)
				{
					pE->RefreshXLinkSource (this);
				}
			}

			return true;
		}
		inline bool JoinClipPathLinks()
		{
			for (size_t i = 0; i < m_arrGroup.size(); ++i)
			{
				DrawElement* pRef = static_cast<DrawElement*>(m_arrGroup[i]);
				if (pRef)
				{
					std::wstring clipId = pRef->GetUrlClip();
					if (!clipId.empty())
					{
						ISvgRef* pClipRef = NULL;
						if (GetRef(clipId, pClipRef))
						{
							pRef->AttachClip (pClipRef);
						}
					}
				}
			}

			return true;
		}
		inline bool JoinStyleLinks()
		{
			for (size_t i = 0; i < m_arrGroup.size(); ++i)
			{
				DrawElement* pRef = static_cast<DrawElement*>(m_arrGroup[i]);
				if (pRef)
				{
					const Style& style = pRef->GetStyle();
					if (style.GetFillUrlRef().length() && (NULL == style.GetFill()))
					{
#ifdef _DEBUG
						//ATLTRACE (_T("[svg] NEED JOIN FILL STYLE : id - %s"), pRef->nodeId());
#endif
					}
				}
			}

			return true;
		}

		//	IRefStorage
		virtual ISvgRef* Get (long index)
		{
			if (index > (long)m_arrGroup.size())
				return NULL;

			// ATLTRACE ( _T("get : %s\n"), m_arrGroup [nIndex]->nodeId () );

			return m_arrGroup [index];
		}
		virtual long GetSize ()
		{
			return (long)m_arrGroup.size();
		}
		virtual bool Push (ISvgRef* Elem)
		{
			// ATLTRACE ( _T("push : %s\n"), Elem->nodeId () );
			m_arrGroup.push_back(Elem);
			return true;
		}

		virtual bool HashRef (ISvgRef* element, bool bDef)
		{
			if (element)
			{
				if (!element->nodeId().empty())
				{
					std::map<std::wstring, ISvgRef*>::iterator iter = m_arrRef.find(element->nodeId());

					if (iter == m_arrRef.end())
					{
						//ATLTRACE(_T("IRefStorage - Hash : %s, %d\n"), element->nodeId(), element->nodeType());

						m_arrRef.insert(std::pair<std::wstring, ISvgRef*>(element->nodeId(), element));

						if (bDef)
						{
							m_arFlush.push_back(element);
						}

						// объекты с id могут быть использованы вне описания другими элементами, поэтому их надо учитывать
						InternalHashElements(element);

						return true;
					}
				}
			}

			return false;
		}
		virtual bool GetRef (const std::wstring& sId, ISvgRef*& pRef)
		{
			std::map<std::wstring, ISvgRef*>::iterator iter = m_arrRef.find(sId);
			if (iter != m_arrRef.end())
			{
				pRef = iter->second;
				return true;
			}

			return false;
		}
		virtual bool Normalize(const double& dAfX, const double& dAfY)
		{
			if ( false == m_bAddNormMM )
			{
				for ( size_t i = 0; i < m_arrGroup.size (); ++i )
				{
					ISvgRef* pE	=	m_arrGroup [ i ];
					if ( pE )
					{
						pE->Normalize(dAfX, dAfY);
					}
				}

				for (std::map<std::wstring, ISvgRef*>::iterator i = m_arrRef.begin(); i != m_arrRef.end(); i++)
				{
					ISvgRef* pE = static_cast<ISvgRef*>(i->second);
					if (pE)
					{
						pE->Normalize(dAfX, dAfY);
					}
				}
			}

			m_bAddNormMM	=	true;

			return true;
		}
		virtual bool RefreshXLinkSource(IRefStorage* pStorage)
		{

			return false;
		}

		//
		inline void SetWorkingDirectory (const std::wstring& sWorkingDirectory)
		{
			m_sWorkingDirectory	= sWorkingDirectory;
		}

	private:

		inline bool InternalHashElements (ISvgRef* element)
		{
			bool ret = false;

			if (EClipPath == element->nodeType())
			{
				ClipPath* clip = static_cast<ClipPath*>(element);
				if (clip)
				{
					if (clip->GetCount())
					{
						for (long i = 0; i < clip->GetCount(); ++i)
						{
							ISvgRef* ref = clip->GetAt(i);
							if (ref)
							{
								if (!ref->nodeId().empty())
								{
									m_arrRef.insert(std::pair<std::wstring, ISvgRef*>(ref->nodeId(), ref));
									ret = true;
								}
							}
						}
					}
				}
			}

			return ret;
		}

	private:

		bool								m_bAddNormMM;

		std::vector <ISvgRef*>				m_arrGroup;
		std::map<std::wstring, ISvgRef*>	m_arrRef;					// индексация (только хранения ссылок)
		std::wstring						m_sWorkingDirectory;
		std::vector<ISvgRef*>				m_arFlush;					// объекты которые нужно удалять
	};
	class Painter
	{
	public:
		Painter()
		{
			m_pManager		=	NULL;
			m_model			=	NULL;
			m_render		=	NULL;

			m_dpiX			=	0.0;
			m_dpiY			=	0.0;

			m_widthMM		=	0.0;
			m_heightMM		=	0.0;

			m_bEnableFonts	=	true;
			m_CSS			=	NULL;

			m_bIsExternalManager = false;
		}
		~Painter()
		{
			if (!m_bIsExternalManager)
				RELEASEINTERFACE (m_pManager);

			for (std::map<std::wstring, PatternImage*>::iterator iter = m_patterns.begin(); iter != m_patterns.end(); iter++)
			{
				PatternImage* p = iter->second;
				RELEASEOBJECT(p);
			}
		}

		inline void SetWorkingDirectory(const std::wstring& sWorkingDirectory)
		{
			m_sWorkingDirectory	= sWorkingDirectory;
		}
		inline const std::wstring& GetWorkingDirectory() const
		{
			return m_sWorkingDirectory;
		}
		inline const UnitSystem& GetUs() const
		{
			return m_oUs;
		}
		inline void SetCSS(CStyleCSS* css)
		{
			m_CSS	=	css;
		}

		bool Draw(IRefStorage* model, IRenderer* render, const UnitSystem& oUs, double dX, double dY, double dW, double dH);

		void SetFontManager(NSFonts::IFontManager* pManager)
		{
			m_bIsExternalManager = true;
			m_pManager = pManager;
		}

	private:
		void SetTransform(const double& sx, const double& shy, const double& shx, const double& sy, const double& tx, const double& ty);
		bool DrawLine (Line* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawRectangle (Rectangle* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawCircle (Circle* element,const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawEllipse (Ellipse* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawPolyline (Polyline* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawPolygon (Polygon* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawPath (Path* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawText (Text* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawImage (Image* element, const Style& oStyle, const std::wstring& strClassName = L"");
		bool DrawUse (Use* element, const Style& oStyle, const std::wstring& strClassName = L"");

		bool DrawStorage (IRefStorage* pStorage, const Matrix& parentTransform = Matrix(), const Point& off = Point());
		bool DrawGraphicsContainer (GraphicsContainer* element, const Matrix& parentTransform = Matrix(), const Point& off = Point());
		bool DrawInternal (DrawElement* pE, const Matrix& parentTransform, const Point& off, const Style& oMainStyle);

		bool SetBrushStyle (const Style& style, const std::wstring& strClassName = L"");
		bool SetStrokeStyle (const Style& style, const std::wstring& strClassName = L"");

		// commands only

		void DoRectangle(Rectangle* element, long type, bool clipMode = false);
		void DoCircle(Circle* element, long type, bool clipMode = false);
		void DoEllipse(Ellipse* element, long type, bool clipMode = false);
		void DoPolyline(Polyline* element, long type, bool clipMode = false);
		void DoPolygon(Polygon* element, long type, bool clipMode = false);
		void DoPath(Path* element, long type, bool clipMode = false);

		// clip
		bool DoClip(ISvgRef* pRef, bool enable);

		inline ISvgRef* FindEntry(Use* element)
		{
			if (element)
			{
				element->RefreshXLinkSource (m_model);

				ISvgRef* refLink = element->GetRefLink();
				if (refLink)
				{
					ENodeType code = refLink->nodeType();
					if (EUse == refLink->nodeType())
						return FindEntry(static_cast<Use*>(refLink));

					return refLink;
				}
			}

			return NULL;
		}

		// image
		bool DrawImageFromFile(Image* element, std::wstring texturePath = L"");
		bool DoPattern(Pattern* element);

		// path
		bool PushPathCommands(Path* element, long PathType);
		bool ClipPathCommands(Path* element, long PathType);

	private:
		// for arc render
		inline bool RenderArcAppx(const double& dRotAngle, const Point& Center, Point Radi, const double& dStartAngle, const double& dSweep)
		{
			if (0.0 != dRotAngle)
			{
				Matrix rotate			=	Matrix::RotateAtPoint(Point(Center.X, Center.Y), -dRotAngle * M_PI / 180);

				CArcToCubicBezier oSplitter(Center.X - Radi.X, Center.Y - Radi.Y, Radi.X * 2.0, Radi.Y * 2.0, dStartAngle - dRotAngle, dSweep);
				for (size_t i = 0; i < oSplitter.GetCurves().size(); ++i)
				{
					CubicBezier oBezier	=	oSplitter.GetCurves()[i];

					Point oPointU		=	rotate.TranformPoint(oBezier.m_Points[0]);
					Point oPointV		=	rotate.TranformPoint(oBezier.m_Points[1]);
					Point oPointE		=	rotate.TranformPoint(oBezier.m_Points[2]);

					m_render->PathCommandCurveTo(oPointU.X, oPointU.Y,	oPointV.X, oPointV.Y, oPointE.X, oPointE.Y);
				}
			}
			else
			{
				m_render->PathCommandArcTo(Center.X - Radi.X, Center.Y - Radi.Y, Radi.X * 2.0, Radi.Y * 2.0, dStartAngle, dSweep);
			}

			return true;
		}
		inline bool ClipArcAppx(Aggplus::CGraphicsPathSimpleConverter& simplifier, const double& dRotAngle, const Point& Center, Point Radi, const double& dStartAngle, const double& dSweep)
		{
			if (0.0 != dRotAngle)
			{
				Matrix rotate			=	Matrix::RotateAtPoint(Point(Center.X, Center.Y), -dRotAngle * M_PI / 180);

				CArcToCubicBezier oSplitter(Center.X - Radi.X, Center.Y - Radi.Y, Radi.X * 2.0, Radi.Y * 2.0, dStartAngle - dRotAngle, dSweep);
				for (size_t i = 0; i < oSplitter.GetCurves().size(); ++i)
				{
					CubicBezier oBezier	=	oSplitter.GetCurves()[i];

					Point oPointU		=	rotate.TranformPoint(oBezier.m_Points[0]);
					Point oPointV		=	rotate.TranformPoint(oBezier.m_Points[1]);
					Point oPointE		=	rotate.TranformPoint(oBezier.m_Points[2]);

					simplifier.PathCommandCurveTo(oPointU.X, oPointU.Y, oPointV.X, oPointV.Y, oPointE.X, oPointE.Y);
				}
			}
			else
			{
				simplifier.PathCommandArcTo(Center.X - Radi.X, Center.Y - Radi.Y, Radi.X * 2.0, Radi.Y * 2.0, dStartAngle, dSweep);
			}

			return true;
		}

		static double GetAngle(const double& CX, const double& CY, const double& X, const double& Y);
		static Point GetCenter(int LargeFlag, int SweepFlag, Point Radi, Point P1, Point P2);
		static bool GetArcAngles(int LargeFlag, int SweepFlag, const double& dStartAngle, const double& dEndAngle, double& dSweep);

		inline NSFonts::IFontManager* GetFontManager()
		{
			if (NULL == m_pManager)
			{
				m_pApplicationFonts = NSFonts::NSApplication::Create();
				m_pApplicationFonts->Initialize();

				m_pManager = m_pApplicationFonts->GenerateFontManager();
			}

			return m_pManager;
		}

		inline void UpdateClass(DrawElement* element)
		{
			/*
			if (element)
			{
				if (element->ClassName().GetLength ())
				{
					VARIANT vt;
					vt.vt		=	VT_BSTR;
					vt.bstrVal	=	CComBSTR (element->ClassName());

					m_render->SetAdditionalParam(L"AttributeClassName", vt);
				}
			}
			*/
		}
		inline Style ComposeStyles(DrawElement* element, const Style& style)
		{
			Style composeStyle = style;
			if (m_CSS)
			{
				Style elementStyle;
				if (m_CSS->GetStyleForType(element->nodeType(), elementStyle))
				{
					if (elementStyle.IsCSS())
					{
						Style::UpdateValidateAttributes(elementStyle, composeStyle);
					}
				}

				Style classStyle;
				if (m_CSS->GetClassStyle(element->ClassName(), classStyle))
				{
					Style::UpdateValidateAttributes(classStyle, composeStyle);
				}
			}

			return composeStyle;
		}

	private:
		bool                            m_bIsExternalManager;
		NSFonts::IFontManager*          m_pManager;
		NSFonts::IApplicationFonts*     m_pApplicationFonts;

		IRenderer*                      m_render;
		IRefStorage*					m_model;
		UnitSystem						m_oUs;

		double							m_dAddMX;
		double							m_dAddMY;
		double							m_dpiX;
		double							m_dpiY;
		double							m_widthMM;
		double							m_heightMM;

		Aggplus::CMatrix                m_baseTransform;
		MatrixStack						m_transforms;
		bool							m_bEnableFonts;
		std::wstring					m_sWorkingDirectory;
		std::map<std::wstring, PatternImage*>	m_patterns;
		CStyleCSS*						m_CSS;
	};

}
namespace SVG
{
	class ENTITY
	{
	public:
		ENTITY()
		{
		}

		inline bool Read (const std::wstring& sTag)
		{
			std::wstring::size_type at = sTag.find(L"\"");
			std::wstring::size_type to = (std::wstring::npos != at) ? sTag.find(L"\"", at + 1) : std::wstring::npos;

			if (std::wstring::npos != at && std::wstring::npos != to)
			{
				m_sValue	=	sTag.substr(at + 1, to - at - 1);
				m_sName		=	sTag.substr(0, at - 1);

				StrUtils::RemoveSpaces(m_sValue);
				StrUtils::RemoveSpaces(m_sName);
				return true;
			}

			return false;
		}
		inline const std::wstring& GetName() const
		{
			return m_sName;
		}
		inline const std::wstring& GetValue() const
		{
			return m_sValue;
		}

	private:

		std::wstring m_sName;
		std::wstring m_sValue;
	};
	class DOCTYPE
	{
	public:
		DOCTYPE()
		{

		}

		inline bool Read (const std::wstring& sTag)
		{
			if (!sTag.empty())
			{
				// READ TAG - <!ENTITY

				std::wstring::size_type at = sTag.find(L"<!ENTITY");
				if (std::wstring::npos == at)	return false;
				std::wstring::size_type to = sTag.find(L">", at);
				if (std::wstring::npos == to)	return false;

				size_t length = std::wstring(L"<!ENTITY").length();

				if (true)
				{
					ENTITY entity;
					if (entity.Read(sTag.substr(at + length, to - at - length)))
						m_ENTITY.push_back(entity);

					while (std::wstring::npos != at)
					{
						at = sTag.find(L"<!ENTITY", to);
						if (std::wstring::npos == at)
							break;

						to = sTag.find(L">", at);
						if (std::wstring::npos == to)
							break;

						ENTITY entity;
						if (entity.Read(sTag.substr(at + length, to - at - length)))
							m_ENTITY.push_back(entity);
					}
				}

				if (m_ENTITY.empty() == false)
					return true;
			}

			return false;
		}

		bool ConnectEntities(std::wstring& sXml)
		{
			if (m_ENTITY.empty() == false)
			{
				std::vector<std::wstring> subStrings;

				std::wstring _and	=	L"&";
				std::wstring _sem	=	L";";

				std::wstring::size_type at = sXml.find(_and);
				if (std::wstring::npos == at)
					return false;
				std::wstring::size_type to = sXml.find(_sem, at);
				if (std::wstring::npos == to)
					return false;

				while (std::wstring::npos != at && std::wstring::npos != to)
				{
					std::wstring replace = sXml.substr(at + 1, to - at - 1);
					subStrings.push_back(replace);

					at = sXml.find(_and, to);
					if (std::wstring::npos == at) break;
					to = sXml.find(_sem, at);
					if (std::wstring::npos == to) break;
				}

				if (subStrings.empty() == false)
				{
					for (long i = (long)subStrings.size() - 1; i >= 0; --i)
					{
						std::wstring str = _and + subStrings[i] + _sem;
						StringHelpers::string_replace(sXml, str, GetVal(subStrings[i]));
					}

					return true;
				}
			}

			return false;
		}

		inline std::wstring GetVal(const std::wstring& sName) const
		{
			for (size_t i = 0; i < m_ENTITY.size(); ++i)
			{
				if (sName == m_ENTITY[i].GetName())
				{
					return m_ENTITY[i].GetValue();
				}
			}

			return L"";
		}
		inline std::wstring GetAt(long index, long type = 0) const
		{
			if (index >= (long)m_ENTITY.size())
				return L"";

			return m_ENTITY[index].GetValue();
		}

		inline void Clear()
		{
			m_ENTITY.clear();
		}

	private:

		std::vector<ENTITY> m_ENTITY;	//	<!ENTITY
	};

	class Parser
	{
	public:
		Parser ()
		{
			m_model		=	NULL;

			m_nWidth		=	0;
			m_nHeight		=	0;

			m_nLayerLevel	=	0;

			m_bDefinesLayer	=	false;
			m_nDefsLayer	=	0;

			m_nDefWidth		=	800;
			m_nDefHeight	=	600;
		}
		~Parser ()
		{

		}

		inline bool	LoadFromFile (const std::wstring& strFile, Storage* model)
		{
#if 0
			std::wstring sExt = NSFile::GetFileExtention(strFile);
			if (sExt != L"svg" && sExt != L"xml")
				return false;
#endif

			bool bRet = false;

			if (model)
			{
				m_model = model;

				std::wstring sXml;
				NSFile::CFileBinary::ReadAllTextUtf8(strFile, sXml);

				XmlUtils::IXmlDOMDocument::DisableOutput();
				bRet = LoadFromString(sXml, model);
				XmlUtils::IXmlDOMDocument::EnableOutput();
			}

			return bRet;
		}
		inline bool	LoadFromString (const std::wstring& strXml, Storage* model)
		{
			if (model)
			{
				m_model = model;

				std::wstring sXml = strXml;
				if (m_DOCTYPE.Read(sXml))
				{
					m_DOCTYPE.ConnectEntities(sXml);
					m_DOCTYPE.Clear();
				}

				if (!sXml.empty())
				{
					StringHelpers::RemoveTagHTML (sXml, L"<!ENTITY", L">");
					StringHelpers::RemoveTagHTML (sXml, L"<!DOCTYPE", L">");
					StringHelpers::RemoveComments (sXml);
				}

				XmlUtils::CXmlNode oXml;
				if (!oXml.FromXmlString(sXml))
					return false;

				std::wstring sNodeName = oXml.GetName();
				if (L"svg" != sNodeName &&
						L"g" != sNodeName &&
						L"xml" != sNodeName)
					return false;


				m_nLayerLevel = 0;

				if (Explore(oXml))
				{
					m_model->JoinXLinkReference();
					m_model->JoinClipPathLinks();
					m_model->JoinStyleLinks();

					return true;
				}
			}

			return false;
		}
		inline bool Explore(XmlUtils::CXmlNode& oXml)
		{
			std::wstring strXmlNode = oXml.GetName();			// ATLTRACE ( L"%s\n", strXmlNode);

			bool ExploreLayer = false;
			bool readInnerNodes = true;

			if (m_bDefinesLayer)
				++m_nDefsLayer;

			if (L"svg" == strXmlNode)
			{
				m_oViewBox.FromXml (oXml);

				m_Metrics			=	StrUtils::GetMetrics(oXml.GetAttribute(L"width"));

				if (PCT == m_Metrics && m_oViewBox.m_bInUse)
				{
					if (0 == (int)m_oViewBox.m_nWidth && 0 == (int)m_oViewBox.m_nHeight)
					{
						return false;
					}

					m_nWidth		=	m_oViewBox.m_nWidth;
					m_nHeight		=	m_oViewBox.m_nHeight;

					if (0 == (int)m_oViewBox.m_nWidth)
						m_nWidth	=	std::max (m_oViewBox.m_nWidth, m_oViewBox.m_nHeight);
					if (0 == (int)m_oViewBox.m_nHeight)
						m_nHeight	=	std::max (m_oViewBox.m_nWidth, m_oViewBox.m_nHeight);
				}
				else
				{
					m_nWidth		=	static_cast<long>(m_oUs.GetPX(StrUtils::DoubleValue(oXml.GetAttribute(L"width")), m_Metrics));
					m_nHeight		=	static_cast<long>(m_oUs.GetPX(StrUtils::DoubleValue(oXml.GetAttribute(L"height")), m_Metrics));
				}

				m_oUs.SetViewBox(m_nWidth, m_nHeight, m_oViewBox, m_Metrics);

				SetDefaultSizes ();

				m_oStyle.SetDefault();
				m_oStyle.ClearFillColor();
				std::wstring css = oXml.GetAttribute(L"style");
				if (!css.empty())
				{
					m_oFontStyle.SetStyle (css, true);
					m_oStyle.SetStyle (css, true, m_oUs, m_model, m_oColTable);
				}
				else
				{
					m_oFontStyle.UpdateStyle (oXml);
					m_oStyle.UpdateStyle (oXml, m_oUs, m_model, m_oColTable);
				}

				m_oStyles.Push(m_oStyle);
			}
			else if (L"g" == strXmlNode)
			{
				if (m_bDefinesLayer)
				{
					GraphicsContainer* pContainer = new GraphicsContainer ();
					if (pContainer)
					{
						pContainer->FromXml(oXml, m_model, m_oViewBox, m_oUs, Matrix());
						m_model->HashRef(pContainer, false);
					}

					--m_nDefsLayer;
					if (0 == m_nDefsLayer)
						m_bDefinesLayer	= false;

					return true;
				}


				// ATLTRACE ( L"Node : %s\n", oXml.GetName() );

				ExploreLayer	=	true;

				++m_nLayerLevel;

				std::wstring ID	= oXml.GetAttribute(L"id");

				std::wstring XmlTransform = oXml.GetAttribute(L"transform");
				if (!XmlTransform.empty())
				{
					Matrix oTransform(XmlTransform, m_oUs);

					m_transforms.Push(oTransform);
				}
				else
				{
					m_transforms.Push(Matrix());
				}

				// m_oStyle.SetDefault ();

				m_oStyle.ClearFillColor();

				std::wstring css = oXml.GetAttribute(L"style");
				if (!css.empty())
				{
					m_oFontStyle.SetStyle (css, true);
					m_oStyle.SetStyle (css, true, m_oUs, m_model, m_oColTable);
				}
				else
				{
					m_oFontStyle.UpdateStyle (oXml);
					m_oStyle.UpdateStyle (oXml, m_oUs, m_model, m_oColTable);
				}

				m_oStyles.Push(m_oStyle);

				m_transforms.Pop();

				GraphicsContainer* pContainer = new GraphicsContainer();
				if (pContainer)
				{
					pContainer->FromXml (oXml, m_model, m_oViewBox, m_oUs, m_transforms.GetFinal());
					m_model->Push(pContainer);

					return true;
				}
			}
			else if (L"xml" == strXmlNode)
			{
				XmlUtils::CXmlNodes oNodes;

				if (oXml.GetNodes(L"*", oNodes))
				{
					if (oNodes.GetCount() > 1)
					{
						XmlUtils::CXmlNode oXmlSub;
						oNodes.GetAt(0, oXmlSub);

						if (false == Explore(oXmlSub))
							return false;
					}
				}
			}
			else if (L"defs" == strXmlNode)
			{
				m_bDefinesLayer	=	true;
				m_nDefsLayer++;
			}
			else if (L"style" == strXmlNode)
			{
				if (L"text/css" == oXml.GetAttribute(L"type"))
				{
					m_CSS.Read(oXml, m_oUs, m_model, m_oColTable);
				}
			}
			else
			{
				LoadElement(oXml);

				if (L"clipPath" == strXmlNode && !m_bDefinesLayer)
					return true;

				if (L"pattern" == strXmlNode && m_bDefinesLayer)
					readInnerNodes = false;

				if (L"symbol" == strXmlNode)	//	контент хранится в соответствующем классе
					return true;
			}

			if (readInnerNodes)
			{
				XmlUtils::CXmlNodes oXmlNodes;
				if (oXml.GetNodes(L"*", oXmlNodes))
				{
					for (long i = 0; i < oXmlNodes.GetCount(); ++i)
					{
						XmlUtils::CXmlNode oXmlNode2;
						if (oXmlNodes.GetAt(i, oXmlNode2))
						{
							if (false == Explore(oXmlNode2))
							{
								if (m_bDefinesLayer)
								{
									--m_nDefsLayer;
									if (0 == m_nDefsLayer)
										m_bDefinesLayer	= false;
								}

								return false;
							}
						}
					}
				}
			}

			if (ExploreLayer)
			{
				m_transforms.Pop ();
				m_oStyles.Pop();

				--m_nLayerLevel;
			}

			if (m_bDefinesLayer)
			{
				--m_nDefsLayer;
				if ( 0 == m_nDefsLayer )
					m_bDefinesLayer	=	false;
			}

			return true;
		}

	public:

		inline long	GetWidth() const
		{
			return m_nWidth;
		}
		inline long	GetHeight() const
		{
			return m_nHeight;
		}
		inline Metrics GetMetrics() const
		{
			return m_Metrics;
		}
		inline const UnitSystem& GetUnitSystem ()
		{
			return m_oUs;
		}
		inline CStyleCSS* GetCSS()
		{
			if (m_CSS.IsValid())
				return &m_CSS;

			return NULL;
		}
		//
		inline void SetDefaultWidth(const long& Width)
		{
			m_nDefWidth = Width;
		}

		inline void SetDefaultHeight(const long& Height)
		{
			m_nDefHeight = Height;
		}
		inline void SetWorkingDirectory(const std::wstring& sWorkingDirectory)
		{
			m_sWorkingDirectory	= sWorkingDirectory;
		}

	private:

		inline bool LoadElement(XmlUtils::CXmlNode& oXml, bool Defines = false)
		{
			std::wstring strXmlNode = oXml.GetName();
			if (!strXmlNode.empty())
			{
				if ('#' == strXmlNode.c_str()[0])
					return false;
			}

			// ATLTRACE ( L"LoadElement : %s\n", strXmlNode );

			if (m_model)
			{
				RefElement* pReference = m_oDefsBuilder.Build (strXmlNode);
				if (pReference)
				{
					if (EPattern == pReference->nodeType())
					{
						((Pattern*)pReference)->FromXml (oXml, m_model, m_oUs);
					}
					else
					{
						pReference->FromXml (oXml, m_model, m_oUs);
					}

					if (m_model->HashRef (pReference, true))
						UpdateSymbol (pReference, oXml);

					return true;
				}

				if (m_bDefinesLayer)
				{
					DrawElement* paintElem = Create (oXml, false);
					if (paintElem)
					{
						if (m_model->HashRef (paintElem, false))
						{

						}
					}

					return true;
				}

				Create (oXml);
			}

			return false;
		}
		inline DrawElement* Create(XmlUtils::CXmlNode& oXmlNode, bool bAddStorage = true)
		{
			std::wstring strXmlNode = oXmlNode.GetName();
			if (!strXmlNode.empty())
			{
				if ('#' == strXmlNode.c_str()[0])
					return NULL;
			}

#ifdef _DEBUG
			//ATLTRACE (L"[svg] render element : %s\n", strXmlNode);
#endif

			DrawElement* element		=	m_oDrawBuilder.Build (strXmlNode);
			if (element)
			{
				Style oTopStyle			=	m_oStyles.GetTop();
				Style oStyle			=	m_oStyle;
				oStyle.ClearOpacity ();

				FontStyle oFontStyle	=	m_oFontStyle;

				std::wstring css = oXmlNode.GetAttribute(L"style");
				if (!css.empty())
				{
					oStyle.UpdateStyle(oXmlNode, m_oUs, m_model, m_oColTable);
					oStyle.SetStyle(css, false, m_oUs, m_model, m_oColTable);
					oFontStyle.SetStyle(css, false);
				}
				else
				{
					oStyle.UpdateStyle(oXmlNode, m_oUs, m_model, m_oColTable);
					oFontStyle.UpdateStyle(oXmlNode);
				}

				m_oUs.SetFontSizeForEM(oFontStyle.DoubleAttribute(FontSize));

				oStyle.Combine(oTopStyle);
				element->SetStyle(oStyle);

				if (EText == element->nodeType())
				{
					Text* pText = static_cast<Text*>(element);
					if (pText)
						pText->m_oFontStyle	= oFontStyle;
				}

				std::wstring transforms = oXmlNode.GetAttribute(L"transform");
				if (!transforms.empty())
				{
					Matrix transform(transforms, m_oUs);
					m_transforms.Push(transform);
					element->SetTransform(m_transforms.GetTop());
					m_transforms.Pop ();
				}
				else
				{
					element->SetTransform(m_transforms.GetTop());
				}

				element->FromXml(oXmlNode, m_oUs);

				if (bAddStorage)
					m_model->Push(element);

				return element;
			}

			return NULL;
		}
		inline void UpdateSymbol(RefElement* pReference, XmlUtils::CXmlNode& oXml)
		{
			if (ESymbol == pReference->nodeType())
			{
				XmlUtils::CXmlNodes oXmlNodes;
				if (oXml.GetNodes(L"*", oXmlNodes))
				{
					for (long i = 0; i < oXmlNodes.GetCount(); ++i)
					{
						XmlUtils::CXmlNode oXml2;
						if (oXmlNodes.GetAt(i,oXml2))
						{
							((Symbol*)pReference)->AddContent(Create(oXml2, false));
						}
					}
				}
			}
		}
		inline bool SetDefaultSizes()
		{
			if ((0 == m_nWidth)||(0 == m_nHeight))
			{
				if ((0 == (int)m_oViewBox.m_nWidth) || (0 == (int)m_oViewBox.m_nHeight))
				{
					m_oViewBox.m_nWidth		=	m_nDefWidth;
					m_oViewBox.m_nHeight	=	m_nDefHeight;
				}

				m_Metrics					=	PX;

				m_nWidth					=	m_oViewBox.m_nWidth;
				m_nHeight					=	m_oViewBox.m_nHeight;

				if (0 == m_oViewBox.m_nWidth)
					m_nWidth				=	std::max(m_oViewBox.m_nWidth, m_oViewBox.m_nHeight);
				if (0 == m_oViewBox.m_nHeight)
					m_nHeight				=	std::max(m_oViewBox.m_nWidth, m_oViewBox.m_nHeight);

				m_oUs.SetViewBox (m_nWidth, m_nHeight, m_oViewBox, m_Metrics);

				return true;
			}

			return false;
		}

	private:

		DOCTYPE			m_DOCTYPE;

		Metrics			m_Metrics;

		LONG			m_nWidth;
		LONG			m_nHeight;

		LONG			m_nDefsLayer;
		bool			m_bDefinesLayer;

		Storage*		m_model;
		DrawBuilder		m_oDrawBuilder;
		DefsBuilder		m_oDefsBuilder;

		ColorTable		m_oColTable;
		Style			m_oStyle;
		FontStyle		m_oFontStyle;
		StyleStack		m_oStyles;

		MatrixStack		m_transforms;

		long			m_nLayerLevel;	//	for transforms

		ViewBox			m_oViewBox;
		UnitSystem		m_oUs;

		long			m_nDefWidth;
		long			m_nDefHeight;

		std::wstring	m_sWorkingDirectory;
		CStyleCSS		m_CSS;
	};
}
