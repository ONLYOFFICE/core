#ifndef CPATH_H
#define CPATH_H

#include "CObjectBase.h"
#include "../SvgTypes.h"

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
		CBezier,
		SBezier,
		QBezier,
		TBezier,
		Arc,
		Close,
	};

	class IPathElement
	{
	public:
		virtual ~IPathElement() = default;
		virtual EPathElement GetType() const = 0;
		virtual void Draw(IRenderer* pRenderer) const = 0;
		virtual TBounds GetBounds() const = 0;

		Point GetFirstPoint() const;
		Point GetLastPoint() const;
	};

	class CMoveElement : public IPathElement
	{
	public:
		CMoveElement(const Point& oPoint);
		virtual ~CMoveElement() = default;
		EPathElement GetType() const override;
		static CMoveElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		void Draw(IRenderer* pRenderer) const override;
		TBounds GetBounds() const override;
	private:
		Point m_oPoint;

		friend class IPathElement;
		friend class CLine;
	};

	class CLineElement : public IPathElement
	{
	public:
		CLineElement(const Point& oPoint);
		virtual ~CLineElement() = default;
		EPathElement GetType() const override;
		static CLineElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		static CLineElement* CreateFromVArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		static CLineElement* CreateFromHArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		void Draw(IRenderer* pRenderer) const override;
		TBounds GetBounds() const override;
		Point GetPoint() const;
	private:
		Point m_oPoint;

		friend class IPathElement;
		friend class CLine;
	};

	class CCBezierElement : public IPathElement
	{
	public:
		CCBezierElement(const Point& oPoint1, const Point& oPoint2, const Point& oPointE, EPathElement enType = CBezier);
		virtual ~CCBezierElement() = default;
		EPathElement GetType() const override;
		static IPathElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		static IPathElement* CreateFromSArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		static IPathElement* CreateFromQArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		static IPathElement* CreateFromTArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		static std::vector<IPathElement*> CreateFromArc(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL);
		void Draw(IRenderer* pRenderer) const override;
		TBounds GetBounds() const override;
	private:
		static void  CalculateArcData(const Point& oFirst, const Point& oSecond, Point& oRadius, Point& oCenter, double dAngle, bool bLargeArc, bool bSweep, double& dStartAngle, double& dSweep);

		Point m_oPoint1;
		Point m_oPoint2;
		Point m_oPointE;
		EPathElement m_enType;

		friend class IPathElement;
		friend class CMovingPath;
		friend class CPath;
	};

	class CCloseElement : public IPathElement
	{
	public:
		CCloseElement() = default;
		virtual ~CCloseElement() = default;
		EPathElement GetType() const override;
		void Draw(IRenderer* pRenderer) const override;
		TBounds GetBounds() const override;
	};

	class CPath : public CRenderedObject
	{
		friend class CObject;
	protected:
		CPath(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		virtual ~CPath();

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		IPathElement* operator[](int nIndex) const;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;
		bool DrawMarkers(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;

		const int FindIndexFirstNotEmpty(bool bReverseSearch = false) const;

		void ReadFromString(const std::wstring& wsValue);

		std::vector<IPathElement*> m_arElements;

		TMarkers m_oMarkers;
		bool m_bEvenOddRule;

		friend class CFont;
	protected:
		bool AddElement(IPathElement* pElement);

		void SetMarker(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode);
	};

	class CMovingPath
	{
	public:
		CMovingPath(const CPath* pPath);

		bool Move(double dX);
		void ToStart();
		Point GetPosition() const;
		double GetAngle() const;
	private:
		void UpdatePosition(const Point& oPoint, double& dX);
		bool NextMove(double dX, const Point& oPoint);

		const CPath  *m_pPath;
		Point         m_oPosition;
		Point         m_oLastPoint;
		double        m_dAngle;
		IPathElement *m_pCurrentElement;
		unsigned int  m_unIndexElement;

		// Необходимо при рабое с кривыми Безье
		double        m_dCurveIndex;
		double        m_dCurveStep;
	};
}

#endif // CPATH_H
