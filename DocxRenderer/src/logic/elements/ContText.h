#pragma once
#include "BaseItem.h"
#include "../../../../DesktopEditor/common/StringBuilder.h"
#include "../managers/FontManager.h"
#include "../managers/FontStyleManager.h"
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
		std::shared_ptr<const CFontStyle> m_pFontStyle{nullptr};
		CFontManager* m_pManager                      {nullptr};

		// background graphics
		std::shared_ptr<CShape> m_pShape              {nullptr};

		// super/sub script
		std::weak_ptr<CContText> m_pCont              {};
		eVertAlignType m_eVertAlignType               {eVertAlignType::vatUnknown};

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

		// font to calc selected sizes
		NSStructures::CFont m_oSelectedFont{};

		// sizes
		double m_dSpaceWidthMM{0};
		CSelectedSizes m_oSelectedSizes{};

		double m_dTopWithAscent{0};
		double m_dBotWithDescent{0};

		NSStringUtils::CStringUTF32 m_oText{};
		UINT m_iNumDuplicates{0};

		bool m_bIsAddBrEnd{false};
		bool m_bWriteStyleRaw{false};

		double m_dFirstWordWidth{0};

		CContText() = default;
		CContText(CFontManager* pManager) : m_pManager(pManager) {}
		CContText(const CContText& rCont);
		virtual ~CContText();

		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual eVerticalCrossingType GetVerticalCrossingType(const CContText* pItem) const noexcept;

		// calc sizes in selected font (uses m_pFontStyle & m_pManager)
		void CalcSelected();

		CContText& operator=(const CContText& rCont);
		bool IsEqual(const CContText* pCont) const noexcept;

		UINT GetNumberOfFeatures() const noexcept;
		bool IsDuplicate(CContText *pCont, eVerticalCrossingType eVType) const noexcept;

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

		double CalculateWideSpace() const noexcept;
		double CalculateThinSpace() const noexcept;
	};
}
