#ifndef DOCX_RENDERER_ELEMENT_SHAPE_H
#define DOCX_RENDERER_ELEMENT_SHAPE_H

#include "Common.h"
#include "ElementParagraph.h"

namespace NSDocxRenderer
{
    class CVectorGraphics
    {
        public:
            enum VectorGraphicsType
            {
                vgtMove = 0,
                vgtLine = 1,
                vgtCurve = 2,
                vgtClose = 3
            };

        public:
            double*	m_pData;
            size_t	m_lSize;

            double*	m_pDataCur;
            size_t	m_lSizeCur;

        public:
            double m_dLeft;
            double m_dTop;
            double m_dRight;
            double m_dBottom;

        public:
            CVectorGraphics();
            ~CVectorGraphics();

            inline void AddSize(size_t nSize);

        public:
            void MoveTo(const double& x1, const double& y1);
            void LineTo(const double& x1, const double& y1);
            void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
            void Close();

            size_t GetCurSize();

            void Clear();
            void ClearNoAttack();

            void End();

            void CheckPoint(const double& x, const double& y);
    };

    class CShape : public CBaseItem
    {
        public:
            //Подобранные константы
            static const double POSITION_CORRECTION_FOR_X_MM;
            static const double POSITION_CORRECTION_FOR_Y_MM;
            static const double SIZE_CORRECTION_FOR_X_MM;
            static const double SIZE_CORRECTION_FOR_Y_MM;

        public:
            std::wstring m_strPath;
            NSStructures::CBrush	m_oBrush;
            NSStructures::CPen		m_oPen;

            double m_dLeft;
            double m_dTop;
            double m_dWidth;
            double m_dHeight;

            bool m_bIsNoFill;
            bool m_bIsNoStroke;

            LONG m_lCoordSizeX;
            LONG m_lCoordSizeY;

            LONG m_lTxId;

            std::vector<CParagraph*> m_arParagraphs;

            CFontManagerLight* m_pManagerLight;

        public:
            CShape();
            ~CShape();
            void Clear();

            CShape(const CShape& oSrc);

            CShape& operator=(const CShape& oSrc);

            void CreateFromVectorData(CVectorGraphics* pVector, NSStringUtils::CStringBuilder& oWriter, const LONG& lCoordSize, LONG lType);

            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter);
    };
};

#endif // DOCX_RENDERER_ELEMENT_SHAPE_H
