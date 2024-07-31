#pragma once
#include "Paragraph.h"
#include "TextLine.h"
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
			stVectorGraphics,
			stVectorTexture,
			stGroup,
			stCanvas,
		};

	public:
		eShapeType m_eType           {eShapeType::stUnknown};

		NSStructures::CBrush m_oBrush{};
		NSStructures::CPen m_oPen    {};

		CVectorGraphics m_oVector     {};
		std::wstring m_strDstMedia    {};

		double m_dRotation {0.0};
		size_t m_nOrder  {0};

		bool m_bIsNoFill    {true};
		bool m_bIsNoStroke  {true};
		bool m_bIsBehindDoc {true};
		bool m_bIsUseInTable{false};

		eGraphicsType m_eGraphicsType    {eGraphicsType::gtUnknown};
		eSimpleLineType m_eSimpleLineType{eSimpleLineType::sltUnknown};
		eLineType m_eLineType            {eLineType::ltUnknown};

		std::vector<std::shared_ptr<CBaseItem>> m_arOutputObjects;
		std::shared_ptr<CImageInfo> m_pImageInfo{nullptr};

	public:
		CShape();
		CShape(std::shared_ptr<CImageInfo> pInfo, const std::wstring& strDstMedia);
		virtual ~CShape();
		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter)const override final;

		void SetVector(CVectorGraphics&& oVector);
		void CalcNoRotVector();

		// tries merge shape, return true if ok and pShape was deleted
		bool TryMergeShape(std::shared_ptr<CShape>& pShape);

		std::wstring PathToWString() const;
		void DetermineGraphicsType(double dWidth, double dHeight, size_t nPeacks, size_t nCurves) noexcept;

		bool IsItFitLine() const noexcept;
		bool IsCorrelated(std::shared_ptr<const CShape> pShape) const noexcept;

		bool IsPeak() const noexcept;
		bool IsSide() const noexcept;

		void BuildGeneralProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildSpecificProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildShapeProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildPictureProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildGroupProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildCanvasProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildGraphicProperties(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildTextBox(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildTextBoxParams(NSStringUtils::CStringBuilder &oWriter) const;
		void BuildForm(NSStringUtils::CStringBuilder &oWriter, const bool& bIsLT = false) const;

		static void ResetRelativeHeight();

		// check type of line and delete not needed shape
		// one shape in line
		static void CheckLineType(std::shared_ptr<CShape>& pShape);

		// many shapes in line
		static void CheckLineType(std::shared_ptr<CShape>& pFirstShape, std::shared_ptr<CShape>& pSecondShape, bool bIsLast = false);

	private:
		CVectorGraphics m_oNoRotVector{};

		UINT m_nShapeId{0};
		UINT m_nRelativeHeight{0};

		static UINT m_gRelativeHeight;
		static UINT GenerateShapeId();
	};
}
