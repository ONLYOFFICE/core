#ifndef CMARKER_H
#define CMARKER_H

#include "CContainer.h"

namespace SVG
{
	enum class EMarkerUnits
	{
		StrokeWidth,
		UserSpaceOnUse
	};

	enum class EMarkerOrient
	{
		Auto,
		Auto_start_reverse,
		Angle
	};

	struct TPointData
	{
		Point  m_oPoint;
		double m_dAngle;

		TPointData()
		    : m_dAngle(0.)
		{}
	};

	struct TMarkerExternData
	{
		std::vector<TPointData>* m_pPoints;
		double m_dStroke;

		TMarkerExternData()
		    : m_pPoints(NULL), m_dStroke(0.)
		{}

		~TMarkerExternData()
		{
			RELEASEOBJECT(m_pPoints);
		}
	};

	class CMarker : public CObject, public CContainer<CRenderedObject>
	{
	public:
		CMarker(CSvgReader& oReader);
		virtual ~CMarker();

		ObjectType GetType() const override;

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		void Draw(IRenderer* pRenderer, const CSvgFile *pFile, const TMarkerExternData& oExternalData, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		bool NeedExternalAngle() const;
		EMarkerOrient GetOrientType() const;
	private:
		EMarkerUnits  m_enUnits;
		EMarkerOrient m_enOrient;

		double       m_dAngle;

		TRect        m_oWindow;
		TRect        m_oViewBox;

		TBounds      m_oBounds;

	};
}

#endif // CMARKER_H
