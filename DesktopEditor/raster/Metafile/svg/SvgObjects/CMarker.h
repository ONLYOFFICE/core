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

	class CMarker : public CGraphicsContainer
	{
	public:
		CMarker(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);
		virtual ~CMarker();

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		void Update(const CSvgFile *pFile);

		void Draw(IRenderer* pRenderer, const std::vector<Point>& arPoints, double dStrokeWidth) const;
	private:
		TBounds GetBounds() const override;

		MarkerUnits m_enUnits;

		Aggplus::CImage *m_pImage;
	};
}

#endif // CMARKER_H
