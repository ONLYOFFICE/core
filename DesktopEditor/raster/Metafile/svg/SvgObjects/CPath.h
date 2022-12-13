#ifndef CPATH_H
#define CPATH_H

#include "CObjectBase.h"

namespace SVG
{
	//	M(m)	-	move to
	//	Z(z)	-	close path
	//	L(l)	-	line to
	//	V(v)	-	vertical line
	//	H(h)	-	horizontal line
	//	C(c)	-	curve to			(x1 y1 x2 y2 x y)
	//  S(x)    -   shortened bezier    (x2 y2 x y)
	//	Q(q)	-	quadratic bezier	(x1 y1 x y)

	enum EPathElement
	{
		Move,
		Line,
		VerticalL,
		HorizantalL,
		Curve,
		SBezier,
		QBezier,
		TBezier,
		Arc,
		Close,
	};

	struct PathPoint
	{
		double dX;
		double dY;

		PathPoint& operator+=(const PathPoint& oPoint)
		{
			dX += oPoint.dX;
			dY += oPoint.dY;

			return *this;
		}
	};

	class CPathElement
	{
	public:
		CPathElement(EPathElement eType, bool bRelativeCoordinate) : m_eType(eType), m_bRelativeCoordinate(bRelativeCoordinate) {}

		EPathElement GetType() const
		{
			return m_eType;
		}

		bool ReadFromString(const std::wstring& wsValue)
		{
			if (wsValue.empty())
				return false;

			m_arValues = ReadDoubleValues(wsValue);

			return !m_arValues.empty();
		}

		void Draw(IRenderer* pRenderer, PathPoint& oLastPoint) const
		{
			PathPoint oPoint;

			switch (m_eType)
			{
				case Move:
				{
					if (m_arValues.empty() || 0 != (m_arValues.size() % 2))
						return;

					for (unsigned int unIndex = 0; unIndex < m_arValues.size(); unIndex += 2)
					{
						oPoint = {m_arValues[unIndex], m_arValues[unIndex + 1]};

						if (m_bRelativeCoordinate)
							oPoint += oLastPoint;

						pRenderer->PathCommandMoveTo(oPoint.dX, oPoint.dY);

						oLastPoint = oPoint;
					}
				}
				case Line:
				{
					if (m_arValues.empty() || 0 != (m_arValues.size() % 2))
						return;

					for (unsigned int unIndex = 0; unIndex < m_arValues.size(); unIndex += 2)
					{
						oPoint = {m_arValues[unIndex], m_arValues[unIndex + 1]};

						if (m_bRelativeCoordinate)
							oPoint += oLastPoint;

						pRenderer->PathCommandLineTo(oPoint.dX, oPoint.dY);

						oLastPoint = oPoint;
					}
				}
				case Close:
				{
					if (m_arValues.empty() || 0 != (m_arValues.size() % 2))
						return;

					pRenderer->PathCommandClose();
				}
			}

			oLastPoint = oPoint;
		}

	private:

		std::vector<double> ReadDoubleValues(const std::wstring& wsValue)
		{
			std::vector<double> arValues;

			std::wstring::const_iterator oFirstPos = wsValue.begin();
			std::wstring::const_iterator oSecondPos = oFirstPos;

			while (true)
			{
				oFirstPos  = std::find_if(oSecondPos, wsValue.end(), [](const wchar_t& wChar){ return iswdigit(wChar) || L'-' == wChar; });

				if (wsValue.end() == oFirstPos)
					break;

				oSecondPos = std::find_if(oFirstPos + 1,  wsValue.end(), [](const wchar_t& wChar){ return iswspace(wChar) || L',' == wChar; });

				arValues.push_back(std::stod(std::wstring(oFirstPos, oSecondPos)));
			}

			return arValues;

		}

		EPathElement        m_eType;
		bool                m_bRelativeCoordinate;
		std::vector<double> m_arValues;
	};

	class CPath : public CObjectBase
	{
	public:
		CPath(CObjectBase* pParent = NULL);
		virtual ~CPath();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;
	private:
		void ApplyStyle(IRenderer* pRenderer) override;

		void ReadFromString(const std::wstring& wsValue);

		std::vector<CPathElement> m_arElements;
	};
}

#endif // CPATH_H
