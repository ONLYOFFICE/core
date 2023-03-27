#pragma once

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

            size_t GetCurSize() const;

            void Clear();
            void ClearNoAttack();

            void End();

            void CheckPoint(const double& x, const double& y);
    };
}
