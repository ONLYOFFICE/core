#ifndef STYLEPROPERTIES_H
#define STYLEPROPERTIES_H

#include <map>
#include <string>
#include <vector>
#include <sstream>

namespace NSCSS
{
	template<typename T>
	class CValue
	{
		friend class CString;
		friend class CDigit;
		friend class CColor;

		T            m_oValue;
		unsigned int m_unLevel;
		bool         m_bImportant;
	public:
		CValue(T oValue, unsigned int unLevel, bool bImportant);

		virtual void SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) = 0;

		virtual bool Empty() const = 0;
		virtual void Clear() = 0;
		virtual int ToInt() const = 0;
		virtual double ToDouble() const = 0;
		virtual std::wstring ToWString() const = 0;

		static void Equation(CValue &oFirstValue, CValue &oSecondValue);
	};

	class CString : public CValue<std::wstring>
	{
	public:
		CString();
		CString(const std::wstring& wsValue, unsigned int unLevel, bool bImportant = false);

		void SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;
		void SetValue(const std::wstring& wsValue, const std::vector<std::wstring>& arValiableValues, unsigned int unLevel, bool bHardMode);
		void SetValue(const std::wstring& wsValue, const std::map<std::wstring, std::wstring>& arValiableValues, unsigned int unLevel, bool bHardMode);

		bool Empty() const override;
		void Clear() override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
	};

	class CDigit : public CValue<double>
	{
	public:
		CDigit();
		CDigit(double dValue, unsigned int unLevel, bool bImportant = false);

		void SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;

		bool Empty() const override;
		void Clear() override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
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

		void SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode) override;

		bool Empty() const override;
		void Clear() override;

		int ToInt() const override;
		double ToDouble() const override;
		std::wstring ToWString() const override;
		TRGB ToRGB() const;

		std::wstring GetURL() const;
	};

	class CDisplay
	{
	public:
		CDisplay();

		static void Equation(CDisplay &oFirstDisplay, CDisplay &oSecondDisplay);

		void SetX      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		void SetY      (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		void SetWidth  (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		void SetHeight (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void SetHAlign (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		void SetVAlign (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		void SetDisplay(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

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

		void SetColor (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);
		void SetWidth (const std::wstring& wsValue, unsigned int unLevel, bool bHardMode = false);

		std::wstring GetColor() const;
		double       GetWidth() const;
	private:
		CColor m_oColor;
		CDigit m_oWidth;
	};
}

#endif // STYLEPROPERTIES_H
