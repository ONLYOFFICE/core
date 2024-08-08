#ifndef STYLEPROPERTIES_H
#define STYLEPROPERTIES_H

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "../../../../DesktopEditor/graphics/Matrix.h"
#include "CUnitMeasureConverter.h"

namespace NSCSS
{
	namespace NSProperties
	{
	#define NEXT_LEVEL UINT_MAX, true

	template<typename T>
	class CValue
	{
		friend class CString;
		friend class CMatrix;
		friend class CDigit;
		friend class CColor;
		friend class CEnum;
		friend class CURL;

		T            m_oValue;
		unsigned int m_unLevel;
		bool         m_bImportant;
	public:
		CValue(const T& oValue, unsigned int unLevel, bool bImportant) :
		    m_oValue(oValue), m_unLevel(unLevel), m_bImportant(bImportant)
		{
		}

		virtual bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) = 0;

		virtual bool Empty() const = 0;
		virtual void Clear() = 0;
		virtual int ToInt() const = 0;
		virtual double ToDouble() const = 0;
		virtual std::wstring ToWString() const = 0;

		static void Equation(CValue &oFirstValue, CValue &oSecondValue)
		{
			if (oFirstValue.m_bImportant && !oSecondValue.m_bImportant && oFirstValue.Empty())
				oSecondValue.Clear();
			else if (oSecondValue.m_bImportant && !oFirstValue.m_bImportant && !oSecondValue.Empty())
				oFirstValue.Clear();
			else if (!oSecondValue.Empty())
			{
				if (oFirstValue.m_unLevel < oSecondValue.m_unLevel)
					oFirstValue.Clear();
				else
					oSecondValue.Clear();
			}
		}

		static bool LevelIsSame(const CValue& oFirstValue, const CValue& oSecondValue)
		{
			return oFirstValue.m_unLevel == oSecondValue.m_unLevel;
		}

		bool operator==(const T& oValue) const { return m_oValue == oValue; }
		bool operator>=(const T& oValue) const { return m_oValue >= oValue; }
		bool operator<=(const T& oValue) const { return m_oValue <= oValue; }
		bool operator> (const T& oValue) const { return m_oValue > oValue; }
		bool operator< (const T& oValue) const { return m_oValue < oValue; }

		virtual CValue& operator =(const CValue& oValue)
		{
			m_oValue     = oValue.m_oValue;
			m_unLevel    = oValue.m_unLevel;
			m_bImportant = oValue.m_bImportant;

			return *this;
		}

		virtual CValue& operator =(const T& oValue)
		{
			//m_oValue = oValue.m_oValue;
			return *this;
		}

		virtual CValue& operator+=(const CValue& oValue)
		{
			if (m_unLevel > oValue.m_unLevel || (m_bImportant && !oValue.m_bImportant) || oValue.Empty())
				return *this;

			m_oValue     = oValue.m_oValue;
			m_unLevel    = oValue.m_unLevel;
			m_bImportant = oValue.m_bImportant;

			return *this;
		}

		virtual bool operator==(const CValue& oValue) const
		{
			return m_oValue == oValue.m_oValue;
		}

		virtual bool operator!=(const CValue& oValue) const
		{
			return m_oValue != oValue.m_oValue;
		}
	};

	class CString : public CValue<std::wstring>
	{
	public:
		CString();
		CString(const std::wstring& wsValue, unsigned int unLevel, bool bImportant = false);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;
		bool SetValue(const std::wstring& wsValue, const std::vector<std::wstring>& arValiableValues, unsigned int unLevel, bool bHardMode);
		bool SetValue(const std::wstring& wsValue, const std::map<std::wstring, std::wstring>& arValiableValues, unsigned int unLevel, bool bHardMode);

		bool Empty() const override;
		void Clear() override;

		int          ToInt()     const override;
		double       ToDouble()  const override;
		std::wstring ToWString() const override;

		CString& operator+=(const CString& oString);
	};

	class CDigit : public CValue<double>
	{
		UnitMeasure m_enUnitMeasure;

		double ConvertValue(double dPrevValue, UnitMeasure enUnitMeasure) const;
	public:
		CDigit();
		CDigit(double dValue);
		CDigit(double dValue, unsigned int unLevel, bool bImportant = false);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel = 0, bool bHardMode = true) override;
		bool SetValue(const CDigit& oValue);
		bool SetValue(const double& dValue, unsigned int unLevel, bool bHardMode);

		bool Empty() const override;
		bool Zero() const;
		void Clear() override;

		void ConvertTo(UnitMeasure enUnitMeasure, double dPrevValue = 0.);

		int          ToInt()     const override;
		double       ToDouble()  const override;
		std::wstring ToWString() const override;

		int          ToInt(UnitMeasure enUnitMeasure, double dPrevValue = 0.)     const;
		double       ToDouble(UnitMeasure enUnitMeasure, double dPrevValue = 0.)  const;
		std::wstring ToWString(UnitMeasure enUnitMeasure, double dPrevValue = 0.) const;

		UnitMeasure GetUnitMeasure() const;

		bool operator==(const double& oValue) const;
		bool operator==(const CDigit& oDigit) const;

		bool operator!=(const double& oValue) const;
		bool operator!=(const CDigit& oDigit) const;

		CDigit operator+(const CDigit& oDigit) const;
		CDigit operator-(const CDigit& oDigit) const;
		CDigit operator*(const CDigit& oDigit) const;
		CDigit operator/(const CDigit& oDigit) const;

		CDigit operator*(double dValue) const;

		CDigit& operator+=(const CDigit& oDigit);
		CDigit& operator-=(const CDigit& oDigit);
		CDigit& operator+=(double dValue);
		CDigit& operator-=(double dValue);
		CDigit& operator*=(double dValue);
		CDigit& operator/=(double dValue);
		CDigit& operator =(double dValue);
	};

	struct TRGB
	{
		unsigned char uchRed;
		unsigned char uchGreen;
		unsigned char uchBlue;

		bool Empty() const;

		bool operator==(const TRGB& oRGB) const;
		bool operator!=(const TRGB& oRGB) const;
	};

	class CURL
	{
	public:
		CURL();

		bool Empty() const;
		bool LinkToId() const;

		void Clear();

		bool SetValue(const std::wstring& wsValue);
		std::wstring GetValue() const;

		bool operator==(const CURL& oValue) const;
		bool operator!=(const CURL& oValue) const;
	private:
		std::wstring m_wsValue;
	};
	
	typedef enum
	{
		ColorEmpty,
		ColorNone,
		ColorRGB,
		ColorHEX,
		ColorUrl
	} ColorType;

	class CColor : public CValue<void*>
	{
	public:
		CColor();

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel = 0, bool bHardMode = true) override;
		bool SetOpacity(const std::wstring& wsValue, unsigned int unLevel = 0, bool bHardMode = true);

		bool Empty() const override;
		bool None() const;
		bool Url() const;
		void Clear() override;

		ColorType GetType() const;

		double GetOpacity() const;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
		std::wstring EquateToColor(const std::vector<std::pair<TRGB, std::wstring>>& arColors) const;
		TRGB ToRGB() const;

		static TRGB ConvertHEXtoRGB(const std::wstring& wsValue);
		static std::wstring ConvertRGBtoHEX(const TRGB& oValue);

		bool operator==(const CColor& oColor) const;
		bool operator!=(const CColor& oColor) const;

		CColor& operator =(const CColor& oColor);
		CColor& operator+=(const CColor& oColor);
	private:
		CDigit    m_oOpacity;
		ColorType m_enType;

		void SetEmpty(unsigned int unLevel = 0);
		void SetRGB(unsigned char uchR, unsigned char uchG, unsigned char uchB);
		void SetRGB(const TRGB& oRGB);
		void SetHEX(const std::wstring& wsValue);
		void SetUrl(const std::wstring& wsValue);
		void SetNone();
	};

	typedef enum
	{
		TransformMatrix,
		TransformTranslate,
		TransformScale,
		TransformRotate,
		TransformSkewX,
		TransformSkewY,
		TransformNone
	} TransformType;

	typedef std::vector<std::pair<std::vector<double>, TransformType>> MatrixValues;

	class CMatrix : public CValue<MatrixValues>
	{
		std::vector<std::wstring> CutTransforms(const std::wstring& wsValue) const;
	public:
		CMatrix();
		CMatrix(const MatrixValues& oValue, unsigned int unLevel, bool bImportant = false);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;
		bool SetMatrix(const Aggplus::CMatrix& oValue);

		bool AddValue(const std::vector<double>& arValues, TransformType enType);

		bool Empty() const override;
		void Clear() override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;

		Aggplus::CMatrix GetFinalValue(TransformType oWithoutType = TransformNone) const;
		void ApplyTranform(Aggplus::CMatrix& oMatrix, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend) const;

		bool operator==(const CMatrix& oMatrix) const;
	};

	class CEnum : public CValue<int>
	{
		std::map<std::wstring, int> m_mMap;
	public:
		CEnum();

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;
		void SetMapping(const std::map<std::wstring, int>& mMap, int nDefaulvalue = -1);

		bool Empty() const override;
		void Clear() override;

		CEnum &operator =(int nValue);

		bool operator==(int nValue) const;
		bool operator!=(int nValue) const;

		int ToInt() const override;
	private:
		double ToDouble() const override;
		std::wstring ToWString() const override;
	};

	// PROPERTIES
	class CDisplay
	{
	public:
		CDisplay();

		static void Equation(CDisplay &oFirstDisplay, CDisplay &oSecondDisplay);

		bool SetX      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetY      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddX      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddY      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetHeight (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		bool SetHAlign (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetVAlign (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		bool SetDisplay(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		const CDigit& GetX()      const;
		const CDigit& GetY()      const;
		const CDigit& GetWidth()  const;
		const CDigit& GetHeight() const;

		const CString& GetHAlign() const;
		const CString& GetVAlign() const;

		const CString& GetDisplay() const;

		bool Empty() const;

		CDisplay& operator+=(const CDisplay& oDisplay);
		bool      operator==(const CDisplay& oDisplay) const;
	private:
		CDigit m_oX;
		CDigit m_oY;
		CDigit m_oWidth;
		CDigit m_oHeight;

		CString m_oHAlign;
		CString m_oVAlign;

		CString m_oDisplay;
	};

	class CStroke
	{
	public:
		CStroke();

		static void Equation(CStroke &oFirstStroke, CStroke &oSecondStroke);

		bool SetColor (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		const CColor& GetColor() const;
		const CDigit& GetWidth() const;

		bool Empty() const;

		CStroke& operator+=(const CStroke& oStroke);
		bool     operator==(const CStroke& oStroke) const;
	private:
		CColor m_oColor;
		CDigit m_oWidth;
	};

	class CBackground
	{
	public:
		CBackground();

		static void Equation(CBackground &oFirstBackground, CBackground &oSecondBackground);

		bool SetColor     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetBackground(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		const CColor& GetColor() const;

		void Clear();
		bool Empty()  const;
		bool IsNone() const;

		CBackground& operator =(const CBackground& oBackground);
		CBackground& operator+=(const CBackground& oBackground);
		bool         operator==(const CBackground& oBackground) const;
	private:
		CColor m_oColor;
	};

	class CTransform
	{
	public:
		CTransform();

		void Clear();

		static void Equation(CTransform &oFirstTransform, CTransform &oSecondTransform);

		bool SetMatrix(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetMatrix(const Aggplus::CMatrix &oMatrix);

		void Translate(double dOffsetX, double dOffsetY);
		void Scale(double dScaleX, double dScaleY);
		void Rotate(double dValue);
		void RotateAt(double dValue, double dX, double dY);

		const CMatrix& GetMatrix() const;

		bool Empty() const;

		CTransform& operator+=(const CTransform& oTransform);
		bool        operator==(const CTransform& oTransform) const;
	private:
		CMatrix m_oMatrix;
	};

	class CBorderSide
	{
	public:
		CBorderSide();
		CBorderSide(const CBorderSide& oBorderSide);

		void Clear();

		static void Equation(CBorderSide &oFirstBorderSide, CBorderSide &oSecondBorderSide);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth(const double& dValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyle(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColor(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void SetNone(unsigned int unLevel, bool bHardMode);

		void Block();
		void Unblock();

		bool IsBlock()            const;

		const CDigit&  GetWidth() const;
		const CString& GetStyle() const;
		const CColor&  GetColor() const;

		bool Empty() const;
		bool Zero()  const;
		bool Valid() const;

		CBorderSide& operator+=(const CBorderSide& oBorderSide);
		bool         operator==(const CBorderSide& oBorderSide) const;
		bool         operator!=(const CBorderSide& oBorderSide) const;
		CBorderSide& operator =(const CBorderSide& oBorderSide);
	private:
		CDigit  m_oWidth;
		CString m_oStyle;
		CColor  m_oColor;

		bool    m_bBlock;
	};

	typedef enum
	{
		Collapse,
		Separate
	} BorderCollapse;
	
	class CBorder
	{
	public:
		CBorder();

		void Clear();
		void ClearLeftSide();
		void ClearTopSide();
		void ClearRightSide();
		void ClearBottomSide();

		static void Equation(CBorder &oFirstBorder, CBorder &oSecondBorder);

		bool SetSides(const std::wstring& wsValue,    unsigned int unLevel, bool bHardMode = false);
		bool SetWidth(const std::wstring& wsValue,    unsigned int unLevel, bool bHardMode = false);
		bool SetWidth(const double& dValue,           unsigned int unLevel, bool bHardMode = false);
		bool SetStyle(const std::wstring& wsValue,    unsigned int unLevel, bool bHardMode = false);
		bool SetColor(const std::wstring& wsValue,    unsigned int unLevel, bool bHardMode = false);
		bool SetCollapse(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Left Side
		bool SetLeftSide       (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthLeftSide  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthLeftSide  (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetStyleLeftSide  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorLeftSide  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Top Side
		bool SetTopSide        (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthTopSide   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthTopSide   (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetStyleTopSide   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorTopSide   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Right Side
		bool SetRightSide      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthRightSide (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthRightSide (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetStyleRightSide (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorRightSide (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Bottom Side
		bool SetBottomSide     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthBottomSide(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthBottomSide(const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetStyleBottomSide(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorBottomSide(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void SetNone(unsigned int unLevel, bool bHardMode = false);

		void Block();
		void Unblock();

		bool Empty() const;
		bool Zero() const;
		bool EqualSides() const;

		const CEnum& GetCollapse() const;

		const CBorderSide& GetLeftBorder()   const;
		const CBorderSide& GetTopBorder()    const;
		const CBorderSide& GetRightBorder()  const;
		const CBorderSide& GetBottomBorder() const;

		CBorder& operator+=(const CBorder& oBorder);
		bool     operator==(const CBorder& oBorder) const;
		bool     operator!=(const CBorder& oBorder) const;
		CBorder& operator =(const CBorder& oBorder);
	private:
		CBorderSide m_oLeft;
		CBorderSide m_oTop;
		CBorderSide m_oRight;
		CBorderSide m_oBottom;
		
		CEnum m_enCollapse;
	};

	class CTextDecorationLine
	{
		bool m_bUnderline;
		bool m_bOverline;
		bool m_bLineThrough;
	public:
		CTextDecorationLine();

		bool Empty() const;

		bool SetValue(const std::wstring& wsValue);

		bool Underline()   const;
		bool Overline()    const;
		bool LineThrough() const;

		CTextDecorationLine &operator+=(const CTextDecorationLine& oTextDecoration);
		bool operator==(const CTextDecorationLine& oTextDecorationLine) const;
	};

	struct TTextDecoration
	{
		CTextDecorationLine m_oLine;
		CString             m_oStyle;
		CColor              m_oColor;

		TTextDecoration& operator+=(const TTextDecoration& oTextDecoration);
		bool operator==(const TTextDecoration& oTextDecoration) const;
	};

	class CText
	{
	public:
		CText();

		static void Equation(CText &oFirstText, CText &oSecondText);

		bool SetIndent    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetAlign     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetDecoration(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColor     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetHighlight (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		const CDigit&          GetIndent()     const;
		const CString&         GetAlign()      const;
		const TTextDecoration& GetDecoration() const;
		const CColor&          GetColor()      const;
		const CColor&          GetHighlight()  const;

		bool Empty() const;

		bool Underline()   const;
		bool Overline()    const;
		bool LineThrough() const;

		CText& operator+=(const CText& oText);
		bool   operator==(const CText& oText) const;
	private:
		TTextDecoration m_oDecoration;
		CDigit          m_oIndent;
		CString         m_oAlign;
		CColor          m_oColor;
		CColor          m_oHighlight;
	};

	class CIndent
	{
	public:
		CIndent();

		void Clear();

		static void Equation(CIndent &oFirstMargin, CIndent &oSecondMargin);

		bool Equals() const;

		void SetPermisson(bool bPermission);

		bool SetValues (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetTop    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetTop    (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetRight  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetRight  (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetBottom (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetBottom (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetLeft   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetLeft   (const double& dValue,        unsigned int unLevel, bool bHardMode = false);

		void UpdateAll   (double dFontSize);
		void UpdateTop   (double dFontSize);
		void UpdateRight (double dFontSize);
		void UpdateBottom(double dFontSize);
		void UpdateLeft  (double dFontSize);

		const CDigit& GetTop   () const;
		const CDigit& GetRight () const;
		const CDigit& GetBottom() const;
		const CDigit& GetLeft  () const;

		bool Empty() const;
		bool Zero() const;

		CIndent& operator+=(const CIndent& oIndent);
		bool     operator==(const CIndent& oIndent) const;
		bool     operator!=(const CIndent& oIndent) const;
	private:
		bool SetValues(const std::wstring& wsTopValue, const std::wstring& wsRightValue, const std::wstring& wsBottomValue, const std::wstring& wsLeftValue, unsigned int unLevel, bool bHardMode = false);
		void UpdateSide(CDigit& oSide, double dFontSize);

		CDigit m_oLeft;
		CDigit m_oTop;
		CDigit m_oRight;
		CDigit m_oBottom;

		bool   m_bPermission;
	};

	class CFont
	{
	public:
		CFont();

		static void Equation(CFont &oFirstFont, CFont &oSecondFont);

		bool SetValue         (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetSize          (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetSize          (const double& dValue,        unsigned int unLevel, bool bHardMode = false);
		bool SetLineHeight    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetFamily        (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStretch       (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyle         (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetVariant       (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWeight        (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void UpdateSize(double dFontSize);
		void UpdateLineHeight(double dFontSize);

		void Clear();

		bool Bold() const;
		bool Italic() const;

		const CDigit&  GetSize()       const;
		const CDigit&  GetLineHeight() const;
		CDigit&  GetLineHeight();
		const CString& GetFamily()     const;
		const CString& GetStretch()    const;
		const CString& GetStyle()      const;
		const CString& GetVariant()    const;
		const CString& GetWeight()     const;

		bool Empty() const;

		CFont& operator+=(const CFont& oFont);
		bool   operator==(const CFont& oFont) const;
	private:
		CDigit  m_oSize;
		CDigit  m_oLineHeight;
		CString m_oFamily;
		//TODO:: возможно стоит перейти в слудующих переменных на enum
		CString m_oStretch;
		CString m_oStyle;
		CString m_oVariant;
		CString m_oWeight;
	};

	class CPage
	{
	public:
		CPage();

		bool SetMargin (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetSize   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetFooter (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetHeader (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		const CDigit&  GetWidth()  const;
		const CDigit&  GetHeight() const;
		const CIndent& GetMargin() const;
		const CDigit&  GetFooter() const;
		const CDigit&  GetHeader() const;
	private:
		CDigit  m_oWidth;
		CDigit  m_oHeight;
		CIndent m_oMargin;
		CDigit  m_oFooter;
		CDigit  m_oHeader;
	};
	}
}

#endif // STYLEPROPERTIES_H
