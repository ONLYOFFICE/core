#pragma once
#include "Paragraph.h"
#include "../../resources/ImageInfo.h"
#include "../../resources/LinesTable.h"
#include "../../resources/VectorGraphics.h"

namespace NSDocxRenderer
{
	enum class eGraphicsType
	{
		gtUnknown,
		gtRectangle,
		gtCurve,
		gtComplicatedFigure,
		gtNoGraphics,
	};

	class CShape : public CBaseItem
	{
	public:
		enum class eShapeType
		{
			stUnknown,
			stTextBox,
			stPicture,
			stVectorGraphics,
			stVectorTexture,
			stGroup,
			stCanvas,
		};

	public:
		eShapeType              m_eType {eShapeType::stUnknown};
		CVectorGraphics			m_oVector;
		std::wstring			m_strDstMedia;
		NSStructures::CBrush    m_oBrush;
		NSStructures::CPen      m_oPen;
		double                  m_dRotate {0.0};

		bool m_bIsNoFill {true};
		bool m_bIsNoStroke {true};
		bool m_bIsBehindDoc {true};

		bool m_bIsUseInTable {false};

		eGraphicsType   m_eGraphicsType {eGraphicsType::gtUnknown};
		eSimpleLineType m_eSimpleLineType {eSimpleLineType::sltUnknown};
		eLineType       m_eLineType {eLineType::ltUnknown};

		std::vector<CBaseItem*> m_arOutputObjects;

		std::shared_ptr<CImageInfo> m_pImageInfo {nullptr};

	private:
		UINT m_nShapeId {0};
		UINT m_nRelativeHeight {0};

		static UINT m_gRelativeHeight;

	public:
		CShape();
		CShape(std::shared_ptr<CImageInfo> pInfo, const std::wstring& strDstMedia);
		virtual ~CShape();
		virtual void Clear() override final;
		virtual void AddContent(CBaseItem* pObj) override final{};
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

		void SetVector(CVectorGraphics&& oVector);
		bool TryMergeShape(CShape* pShape);
		std::wstring PathToWString();
		void DetermineGraphicsType(double dWidth, double dHeight, size_t nPeacks, size_t nCurves);
		bool IsItFitLine();
		bool IsCorrelated(const CShape* pShape);
		void ChangeGeometryOfDesiredShape(CShape* pShape);

		bool IsPeak();
		bool IsSide();
		void DetermineLineType(CShape* pShape = nullptr, bool bIsLast = false);

		void BuildGeneralProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildSpecificProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildShapeProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildPictureProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildGroupProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildCanvasProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildGraphicProperties(NSStringUtils::CStringBuilder &oWriter);
		void BuildTextBox(NSStringUtils::CStringBuilder &oWriter);

		static void ResetRelativeHeight();

	private:
		UINT GenerateShapeId();
	};
}
