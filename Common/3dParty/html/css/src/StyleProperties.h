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
	template<typename T>
	class CValue
	{
		friend class CString;
		friend class CMatrix;
		friend class CDigit;
		friend class CColor;
		friend class CEnum;

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

		CValue& operator =(const T& oValue)
		{
			//m_oValue = oValue.m_oValue;
			return *this;
		}

		CValue& operator+=(const CValue& oValue)
		{
			if (m_unLevel > oValue.m_unLevel || (m_bImportant && !oValue.m_bImportant) || oValue.Empty())
				return *this;

			m_oValue     = oValue.m_oValue;
			m_unLevel    = std::max(m_unLevel, oValue.m_unLevel);
			m_bImportant = std::max(m_bImportant, oValue.m_bImportant);

			return *this;
		}

		bool    operator==(const CValue& oValue) const
		{
			return m_oValue == oValue.m_oValue;
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

		bool Empty() const override;
		bool Zero() const;
		void Clear() override;

		int          ToInt()     const override;
		double       ToDouble()  const override;
		std::wstring ToWString() const override;

		int          ToInt(UnitMeasure enUnitMeasure, double dPrevValue = 0.)     const;
		double       ToDouble(UnitMeasure enUnitMeasure, double dPrevValue = 0.)  const;
		std::wstring ToWString(UnitMeasure enUnitMeasure, double dPrevValue = 0.) const;

		UnitMeasure GetUnitMeasure() const;

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
	};

	typedef enum
	{
		ColorEmpty,
		ColorNone,
		ColorRGB,
		ColorHEX,
		ColorUrl
	} ColorType;

	class Q_DECL_EXPORT CColorValue
	{
	public:
		CColorValue();
		CColorValue(const CColorValue& oColorValue);
		~CColorValue();

		void SetRGB(unsigned char uchR, unsigned char uchG, unsigned char uchB);
		void SetRGB(const TRGB& oRGB);
		void SetHEX(const std::wstring& wsValue);
		void SetUrl(const std::wstring& wsValue);
		void SetNone();

		void Clear();

		bool Empty() const;

		ColorType    m_enType;
		void*        m_pColor = NULL;

		std::wstring GetColor() const;

		bool         operator==(const CColorValue& oColorValue) const;
		CColorValue& operator= (const CColorValue& oColorValue);
	};

	class CColor : public CValue<CColorValue>
	{
		CDigit m_oOpacity;
		static TRGB ConvertHEXtoRGB(const std::wstring& wsValue);
		static std::wstring ConvertRGBtoHEX(const TRGB& oValue);
		static std::wstring CutURL(const std::wstring& wsValue);
	public:
		CColor();

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel = 0, bool bHardMode = true) override;
		bool SetOpacity(const std::wstring& wsValue, unsigned int unLevel = 0, bool bHardMode = true);

		bool Empty() const override;
		void Clear() override;

		ColorType GetType() const;

		double GetOpacity() const;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
		TRGB ToRGB() const;
	};

	typedef enum
	{
		TransformMatrix,
		TransformTranslate,
		TransformScale,
		TransformRotate,
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
		void SetMapping(const std::map<std::wstring, int>& mMap);

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
		void InBorder();

		const CColor& GetColor() const;
		bool IsInBorder()        const;

		bool Empty() const;

		CBackground& operator =(const CBackground& oBackground);
		CBackground& operator+=(const CBackground& oBackground);
		bool         operator==(const CBackground& oBackground) const;
	private:
		CColor m_oColor;
		bool   m_bInBorder;
	};

	class CTransform
	{
	public:
		CTransform();

		void Clear();

		static void Equation(CTransform &oFirstTransform, CTransform &oSecondTransform);

		bool SetMatrix(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetMatrix(const Aggplus::CMatrix &oMatrix);

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

		static void Equation(CBorderSide &oFirstBorderSide, CBorderSide &oSecondBorderSide);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyle(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColor(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void Block();
		void Unblock();

		bool IsBlock()            const;

		const CDigit&  GetWidth() const;
		const CString& GetStyle() const;
		const CColor&  GetColor() const;

		bool Empty() const;

		CBorderSide& operator+=(const CBorderSide& oBorderSide);
		bool         operator==(const CBorderSide& oBorderSide) const;
	private:
		CDigit  m_oWidth;
		CString m_oStyle;
		CColor  m_oColor;

		bool    m_bBlock;
	};

	class CBorder
	{
	public:
		CBorder();

		static void Equation(CBorder &oFirstBorder, CBorder &oSecondBorder);

		bool SetSides(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyle(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColor(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Left Side
		bool SetLeftSide       (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthLeftSide  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyleLeftSide  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorLeftSide  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Top Side
		bool SetTopSide        (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthTopSide   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyleTopSide   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorTopSide   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Right Side
		bool SetRightSide      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthRightSide (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyleRightSide (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorRightSide (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		//Bottom Side
		bool SetBottomSide     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidthBottomSide(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyleBottomSide(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColorBottomSide(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void Block();
		void Unblock();

		bool Empty() const;
		bool EqualSides() const;

		const CBorderSide& GetLeftBorder()   const;
		const CBorderSide& GetTopBorder()    const;
		const CBorderSide& GetRightBorder()  const;
		const CBorderSide& GetBottomBorder() const;

		CBorder& operator+=(const CBorder& oBorder);
		bool     operator==(const CBorder& oBorder) const;
	private:
		CBorderSide m_oLeft;
		CBorderSide m_oTop;
		CBorderSide m_oRight;
		CBorderSide m_oBottom;
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
	};

	struct TTextDecoration
	{
		CTextDecorationLine m_oLine;
		CString             m_oStyle;
		CColor              m_oColor;

		TTextDecoration& operator+=(const TTextDecoration& oTextDecoration);
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

		const CDigit&          GetIndent()     const;
		const CString&         GetAlign()      const;
		const TTextDecoration& GetDecoration() const;
		const CColor&          GetColor()      const;

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
	};

	class CIndent
	{
	public:
		CIndent();

		static void Equation(CIndent &oFirstMargin, CIndent &oSecondMargin);

		void SetPermisson(bool bPermission);

		bool AddValue  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddLeft   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddTop    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddRight  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddBottom (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		const CDigit& GetLeft  () const;
		const CDigit& GetTop   () const;
		const CDigit& GetRight () const;
		const CDigit& GetBottom() const;

		bool Empty() const;

		CIndent& operator+=(const CIndent& oMargin);
		bool     operator==(const CIndent& oMargin) const;
	private:
		bool AddValue(CDigit& oValue, const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

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
		bool SetLineHeight    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetFamily        (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStretch       (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetStyle         (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetVariant       (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWeight        (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		bool UpdateSize(double dSize);

		void Clear();

		bool Bold() const;
		bool Italic() const;

		const CDigit&  GetSize()       const;
		const CDigit&  GetLineHeight() const;
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

		TTextDecoration m_oTextDecoration;
	};
	}
}

#endif // STYLEPROPERTIES_H
