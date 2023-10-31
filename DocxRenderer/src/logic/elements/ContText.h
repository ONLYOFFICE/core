#pragma once
#include "BaseItem.h"
#include "../DesktopEditor/common/StringBuilder.h"
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

	class CContText : public CBaseItem
	{
	public:
		std::shared_ptr<const CFontStyle> m_pFontStyle {nullptr};

		bool   m_bIsStrikeoutPresent{false};
		bool   m_bIsDoubleStrikeout{false};

		bool   m_bIsHighlightPresent{false};
		LONG   m_lHighlightColor{c_iBlackColor};

		bool   m_bIsUnderlinePresent{false};
		eLineType m_eUnderlineType{eLineType::ltUnknown};
		LONG   m_lUnderlineColor{c_iBlackColor};

		bool   m_bIsShadowPresent{false};
		bool   m_bIsOutlinePresent{false};
		bool   m_bIsEmbossPresent{false};
		bool   m_bIsEngravePresent{false};

		NSStringUtils::CStringUTF32 m_oText;

		double m_dSpaceWidthMM{0};
		bool   m_bSpaceIsNotNeeded{false};

		double m_dWidthSelected{0};
		double m_dSpaceWidthSelected{0};

		eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

		CFontManager* m_pManager{nullptr};

		CShape* m_pShape{nullptr}; //Если не nullptr, то есть фоновая графика - можно анализировать.
		const CContText* m_pCont{nullptr}; //Если не nullptr, то есть привязка к vatSubscript или vatSuperscript;

		UINT m_iNumDuplicates{0};
		double m_dTrueHeight{0};

	public:
		CContText(CFontManager* pManager) : m_pManager(pManager) {}
		CContText(const CContText& rCont);
		virtual ~CContText();
		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;

		virtual eVerticalCrossingType GetVerticalCrossingType(const CBaseItem* pItem) const noexcept  override  final;

		CContText& operator= (const CContText& rCont);

		void AddWideSpaceToXml(double dSpacingMM,
							   NSStringUtils::CStringBuilder& oWriter,
							   bool bIsNeedSaveFormat = false);
		bool IsEqual(const CContText* pCont);
		UINT GetNumberOfFeatures();
		bool IsDuplicate(CContText *pCont, eVerticalCrossingType eVType);
		bool IsThereAreFontEffects(CContText *pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType);
		bool IsVertAlignTypeBetweenConts(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType);
		void CalcSelectedWidth();

		double CalculateWideSpace();
		double CalculateThinSpace();
	};
}
