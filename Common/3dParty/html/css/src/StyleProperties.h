#ifndef STYLEPROPERTIES_H
#define STYLEPROPERTIES_H

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "../../graphics/Matrix.h"

namespace NSCSS
{
	namespace NSProperty
	{
	template<typename T>
	class CValue
	{
		friend class CString;
		friend class CMatrix;
		friend class CDigit;
		friend class CColor;

		T            m_oValue;
		unsigned int m_unLevel;
		bool         m_bImportant;
	public:
		CValue(T oValue, unsigned int unLevel, bool bImportant);

		virtual bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) = 0;

		virtual bool Empty() const = 0;
		virtual void Clear(const T* pDefaultValue = NULL) = 0;
		virtual int ToInt() const = 0;
		virtual double ToDouble() const = 0;
		virtual std::wstring ToWString() const = 0;

		static void Equation(CValue &oFirstValue, CValue &oSecondValue, const T* pDefaultValue = NULL);

		bool operator==(const T& oValue) const;
		bool operator>=(const T& oValue) const;
		bool operator<=(const T& oValue) const;
		bool operator> (const T& oValue) const;
		bool operator< (const T& oValue) const;

		CValue& operator=(const CValue& oValue);
		CValue& operator+=(const T& oValue);
		CValue& operator+=(const CValue& oValue);
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
		void Clear(const  std::wstring* pDefaultValue = NULL) override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
	};

	class CDigit : public CValue<double>
	{
	public:
		CDigit();
		CDigit(double dValue, unsigned int unLevel, bool bImportant = false);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;

		bool Empty() const override;
		void Clear(const  double* pDefaultValue = NULL) override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;

		CDigit& operator*=(double dValue);
	};

	struct TRGB
	{
		unsigned char chRed;
		unsigned char chGreen;
		unsigned char chBlue;
	};

	class CColor : public CValue<TRGB>
	{
		std::wstring m_wsURL;

		static TRGB ConvertHEXtoRGB(const std::wstring& wsValue);
		static std::wstring ConvertRGBtoHEX(const TRGB& oValue);
	public:
		CColor();
		CColor(const TRGB& oValue, unsigned int unLevel, bool bImportant = false);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;

		bool Empty() const override;
		void Clear(const  TRGB* pDefaultValue = NULL) override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
		TRGB ToRGB() const;

		std::wstring GetURL() const;
	};

	typedef enum
	{
		TransformNone,
		TransformMatrix,
		TransformTranslate,
		TransformScale,
		TransformRotate
	} TransformType;

	class CMatrix : public CValue<Aggplus::CMatrix>
	{
		TransformType m_enType;
	public:
		CMatrix();
		CMatrix(const Aggplus::CMatrix& oValue, unsigned int unLevel, bool bImportant = false);

		bool SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;

		bool Empty() const override;
		void Clear(const  Aggplus::CMatrix* pDefaultValue = NULL) override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;

		Aggplus::CMatrix GetValue() const;
		TransformType GetType() const;
	};

//	class CEnum : public CValue<enum>

	// PROPERTIES
	class CDisplay
	{
	public:
		CDisplay();

		static void Equation(CDisplay &oFirstDisplay, CDisplay &oSecondDisplay);

		bool SetX      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetY      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetWidth  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetHeight (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		bool SetHAlign (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetVAlign (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		bool SetDisplay(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		double GetX()      const;
		double GetY()      const;
		double GetWidth()  const;
		double GetHeight() const;

		std::wstring GetHAlign() const;
		std::wstring GetVAlign() const;

		std::wstring GetDisplay() const;
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

		std::wstring GetColor() const;
		int          GetColorN()const;
		double       GetWidth() const;
	private:
		CColor m_oColor;
		CDigit m_oWidth;
	};

	class CBackground
	{
	public:
		CBackground();

		static void Equation(CBackground &oFirstBackground, CBackground &oSecondBackground);

		bool SetColor (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		void InBorder();

		std::wstring GetColor()    const;
		int          GetColorN()   const;
		std::wstring GetColorURL() const;
		bool IsInBorder()          const;
	private:
		CColor m_oColor;
		bool   m_bInBorder;
	};

	class CTransform
	{
	public:
		CTransform();

		static void Equation(CTransform &oFirstTransform, CTransform &oSecondTransform);

		bool SetMatrix(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		Aggplus::CMatrix GetMatrix() const;
		TransformType GetType() const;
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

		double       GetWidth()  const;
		std::wstring GetStyle()  const;
		std::wstring GetColor()  const;
		int          GetColorN() const;
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

		bool SetAllSides  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

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

		CBorderSide GetLeftBorder()   const;
		CBorderSide GetTopBorder()    const;
		CBorderSide GetRightBorder()  const;
		CBorderSide GetBottomBorder() const;
	private:
		CBorderSide m_oLeft;
		CBorderSide m_oTop;
		CBorderSide m_oRight;
		CBorderSide m_oBottom;
	};


//	float fIndent;
//                Align enAlign;
//                TextDecoration enDecoration;
//                std::wstring sColor; //HEX color

	class CText
	{
	public:
		CText();

		static void Equation(CText &oFirstText, CText &oSecondText);

		bool SetIndent    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetAlign     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetDecoration(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool SetColor     (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		double       GetIndent()     const;
		std::wstring GetAlign()      const;
		std::wstring GetDecoration() const;
		std::wstring GetColor()      const;
		int          GetColorN()     const;
	private:
		CDigit  m_oIndent;
		CString m_oAlign;
		CString m_oDecoration;
		CColor  m_oColor;
	};

	class CMargin
	{
	public:
		CMargin();

		static void Equation(CMargin &oFirstMargin, CMargin &oSecondMargin);

		void SetPermisson(bool bPermission);

		bool AddMargin (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddLeft   (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddTop    (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddRight  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		bool AddBottom (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		double GetLeft  () const;
		double GetTop   () const;
		double GetRight () const;
		double GetBottom() const;
	private:
		bool AddValue(CDigit& oValue, const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		CDigit m_oLeft;
		CDigit m_oTop;
		CDigit m_oRight;
		CDigit m_oBottom;

		bool   m_bPermission;
	};
	}
}

#endif // STYLEPROPERTIES_H
