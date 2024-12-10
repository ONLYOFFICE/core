#pragma once
#include "../../../../DesktopEditor/common/StringBuilder.h"

#include "BaseItem.h"
#include "../managers/FontManager.h"
#include "../managers//FontStyleManager.h"
#include "../styles/FontStyle.h"
#include "../../resources/Constants.h"
#include "../../resources/LinesTable.h"

namespace NSDocxRenderer
{
	class CShape;

	enum class eVertAlignType
	{
		vatUnknown,
		vatBase,
		vatSubscript,
		vatSuperscript
	};

	// sizes in selected font
	struct CSelectedSizes
	{
		double dWidth{0};
		double dHeight{0};

		CSelectedSizes() = default;
		~CSelectedSizes() = default;
		CSelectedSizes(const CSelectedSizes& oSelectedSizes);
		CSelectedSizes& operator=(const CSelectedSizes& oSelectedSizes);
	};

	class CContText : public CBaseItem
	{
	public:
		// utils
		std::shared_ptr<CFontStyle> m_pFontStyle{nullptr};

		CFontManager* m_pManager        {nullptr};
		std::shared_ptr<CShape> m_pShape{nullptr};

		// super/sub script
		std::weak_ptr<CContText> m_pCont {};
		eVertAlignType m_eVertAlignType  {eVertAlignType::vatUnknown};

		// highlights
		bool m_bIsStrikeoutPresent{false};
		bool m_bIsDoubleStrikeout {false};
		bool m_bIsHighlightPresent{false};
		LONG m_lHighlightColor    {c_iBlackColor};
		bool m_bIsUnderlinePresent{false};
		eLineType m_eUnderlineType{eLineType::ltUnknown};
		LONG m_lUnderlineColor    {c_iBlackColor};
		bool m_bIsShadowPresent   {false};
		bool m_bIsOutlinePresent  {false};
		bool m_bIsEmbossPresent   {false};
		bool m_bIsEngravePresent  {false};
		bool m_bIsRtl             {false};

		// font to calc selected sizes
		NSStructures::CFont m_oSelectedFont{};

		// origin font
		std::wstring m_wsOriginFontName{};

		// sizes
		double m_dSpaceWidthMM{0};
		CSelectedSizes m_oSelectedSizes{};

		double m_dTopWithAscent{0};
		double m_dBotWithDescent{0};

		UINT m_iNumDuplicates{0};

		bool m_bIsAddBrEnd{false};
		bool m_bWriteStyleRaw{false};
		bool m_bPossibleSplit{false};

		CContText() = default;
		CContText(CFontManager* pManager) : m_pManager(pManager) {}
		CContText(const CContText& rCont);
		virtual ~CContText();

		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual eVerticalCrossingType GetVerticalCrossingType(const CContText* pItem) const noexcept;

		// calc sizes in selected font (uses m_oSelectedFont & m_pManager)
		void CalcSelected();

		size_t GetLength() const noexcept;
		void AddTextBack(const NSStringUtils::CStringUTF32& oText, const std::vector<double>& arSymWidths);
		void AddTextFront(const NSStringUtils::CStringUTF32& oText, const std::vector<double>& arSymWidths);
		void SetText(const NSStringUtils::CStringUTF32& oText, const std::vector<double>& arSymWidths);

		void AddSymBack(uint32_t cSym, double dWidth);
		void AddSymFront(uint32_t cSym, double dWidth);
		void SetSym(uint32_t cSym, double dWidth);
		void RemoveLastSym();

		uint32_t GetLastSym() const;

		const NSStringUtils::CStringUTF32& GetText() const noexcept;
		const std::vector<double>& GetSymWidths() const noexcept;
		const std::vector<double> GetSymLefts() const noexcept;

		std::shared_ptr<CContText> Split(size_t index);
		std::shared_ptr<CContText> Split(double dLeft);

		CContText& operator=(const CContText& rCont);
		bool IsEqual(const CContText* pCont) const noexcept;

		UINT GetNumberOfFeatures() const noexcept;
		bool IsDuplicate(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType) const noexcept;

		bool IsOnlySpaces() const;
		double CalculateSpace() const noexcept;

		// check font effect and delete not needed cont
		// return true if was deleted
		static bool CheckFontEffects
			(std::shared_ptr<CContText>& pFirstCont,
			 std::shared_ptr<CContText>& pSecondCont,
			 eVerticalCrossingType eVType,
			 eHorizontalCrossingType eHType);

		static bool CheckVertAlignTypeBetweenConts
			(std::shared_ptr<CContText> pFirstCont,
			 std::shared_ptr<CContText> pSecondCont,
			 eVerticalCrossingType eVType,
			 eHorizontalCrossingType eHType);

		static bool IsUnicodeRtl(uint32_t cSym);
		static bool IsUnicodeBullet(uint32_t cSym);
		static bool IsUnicodeSpace(uint32_t c);
		static bool IsUnicodeSymbol(uint32_t symbol);
		static bool IsUnicodeDiacriticalMark(uint32_t symbol);

	private:
		NSStringUtils::CStringUTF32 m_oText{};
		std::vector<double> m_arSymWidths{};
	};

	class CContTextBuilder
	{
	protected:
		using cont_ptr_t = std::shared_ptr<CContText>;

	public:
		CContTextBuilder() = delete;
		CContTextBuilder(CFontStyleManager* pFontStyleManager, CFontSelector* pFontSelector);
		~CContTextBuilder() = default;

		// after call CContTextBuilder is empty.
		std::vector<cont_ptr_t> GetConts();
		void AddUnicode(
			double dTop,
			double dBot,
			double dLeft,
			double dRight,
			const NSStructures::CFont& oFont,
			const NSStructures::CBrush& oBrush,
			CFontManager* pFontManager,
			const NSStringUtils::CStringUTF32& oText,
			bool bForcedBold = false,
			bool bUseDefaultFont = false,
			bool bWriteStyleRaw = false);

	private:
		std::vector<cont_ptr_t> m_arConts;
		cont_ptr_t m_pCurrCont {nullptr};
		NSStructures::CFont m_oPrevFont;
		NSStructures::CBrush m_oPrevBrush;

		CFontStyleManager* m_pFontStyleManager {nullptr};
		CFontSelector* m_pFontSelector {nullptr};
	};
}
