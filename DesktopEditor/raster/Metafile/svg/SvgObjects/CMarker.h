#ifndef CMARKER_H
#define CMARKER_H

#include "CContainer.h"

namespace SVG
{
	typedef enum
	{
		Marker_StrokeWidth,
		Marker_UserSpaceOnUse
	} MarkerUnits;

	class CMarker : public CObject, public CContainer<CRenderedObject>
	{
	public:
		CMarker(XmlUtils::CXmlNode& oNode);
		virtual ~CMarker();

		ObjectType GetType() const override;

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		void Update(const CSvgFile *pFile);

		void Draw(IRenderer* pRenderer, const std::vector<Point>& arPoints, double dStrokeWidth) const;
	private:
		MarkerUnits m_enUnits;

		TRect m_oWindow;
		TRect m_oViewBox;

		Aggplus::CImage *m_pImage;
	};
}

#endif // CMARKER_H
