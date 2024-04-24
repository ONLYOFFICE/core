/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
            std::wstring            m_strPath {L""};
            NSStructures::CBrush	m_oBrush;
            NSStructures::CPen		m_oPen;
            double                  m_dRotate {0.0};

            bool m_bIsNoFill {true};
            bool m_bIsNoStroke {true};
            bool m_bIsBehindDoc {true};

            eGraphicsType   m_eGraphicsType {eGraphicsType::gtUnknown};
            eSimpleLineType m_eSimpleLineType {eSimpleLineType::sltUnknown};
            eLineType       m_eLineType {eLineType::ltUnknown};

            std::vector<CParagraph*> m_arParagraphs;

            std::shared_ptr<CImageInfo> m_pImageInfo {nullptr};

        private:
            UINT m_nShapeId {0};
            UINT m_nRelativeHeight {0};

            static UINT m_gRelativeHeight;

        public:
            CShape();
            virtual ~CShape();
            virtual void Clear() override final;

            CShape(std::shared_ptr<CImageInfo> pInfo, const std::wstring& strDstMedia);

            void GetDataFromVector(const CVectorGraphics& oVector);

            void WritePath(const CVectorGraphics& oVector);

            void DetermineGraphicsType(double dWidth, double dHeight, size_t nPeacks, size_t nCurves);

            bool IsItFitLine();
            bool IsCorrelated(const CShape* pShape);
            void ChangeGeometryOfDesiredShape(CShape* pShape);

            void DetermineLineType(CShape* pShape = nullptr, bool bIsLast = false);

            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;
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
