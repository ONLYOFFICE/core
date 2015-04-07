
#pragma once

class TextFormat
{
public:
	// format
	long	m_nAlignVertical;
	long	m_nAlignHorizontal;

	// rotate
	long	m_nRotateAngle;

	// preset name
	CString	m_sCurrentPresetName;

	// shadow
	BOOL	m_bShadowVisible;
	long	m_nShadowDistanceX;  
	long	m_nShadowDistanceY; 
	Color	m_cShadowColor;
	long	m_nShadowBlur;

	// edge
	long	m_nEdgeVisible;
	long	m_nEdgeDist;
	Color	m_cEdgeColor;

	// brush
	long	m_nBrushFaceColorType;
	long	m_nBrushFaceColorMode;
	// old value (do not used)
	Color	m_cFontColor;
	Color	m_cFontGradColor1;
	Color	m_cFontGradColor2;
	long	m_nHatchType;
	Color	m_cFontHatchColor1;
	Color	m_cFontHatchColor2;

	// texture
	CString	m_sTextureFilePath;
	BYTE	m_nTextureAlpha;

	// font
	CString	m_sFontName;
	long	m_nFontSize;
	long	m_nFontStyle;

public:

	CString ToXmlString()
	{
		XmlUtils::CXmlWriter oXmlWriter;

		// begin node preset
		oXmlWriter.WriteNodeBegin("preset", TRUE);
		oXmlWriter.WriteAttribute("name", m_sCurrentPresetName);
		oXmlWriter.WriteNodeEnd("preset", TRUE, FALSE);

		// save font
		oXmlWriter.WriteNode("fontName", m_sFontName);
		oXmlWriter.WriteNode("fontSize", m_nFontSize);
		oXmlWriter.WriteNode("fontStyle", m_nFontStyle);
		ARGB nFontColor = m_cFontColor.GetValue();
		CString sFontColor; sFontColor.Format("%u", nFontColor);
		oXmlWriter.WriteNode("fontColor", sFontColor);
		oXmlWriter.WriteNode("alignVertical", m_nAlignVertical);
		oXmlWriter.WriteNode("alignHorizontal", m_nAlignHorizontal);

		// save rotate
		oXmlWriter.WriteNode("rotateAngle", m_nRotateAngle);

		// save shadow
		oXmlWriter.WriteNode("shadowVisible", m_bShadowVisible);
		oXmlWriter.WriteNode("shadowDistanceX", m_nShadowDistanceX);
		oXmlWriter.WriteNode("shadowDistanceY", m_nShadowDistanceY);
		ARGB nShadowColor = m_cShadowColor.GetValue();
		CString sShadowColor; sShadowColor.Format("%u", nShadowColor);
		oXmlWriter.WriteNode("shadowColor", sShadowColor);
		oXmlWriter.WriteNode("shadowBlur", m_nShadowBlur);

		// save edge
		oXmlWriter.WriteNode("edgeVisible", m_nEdgeVisible);
		oXmlWriter.WriteNode("edgeDist", m_nEdgeDist);
		ARGB nEdgeColor = m_cEdgeColor.GetValue();
		CString sEdgeColor; sEdgeColor.Format("%u", nEdgeColor);
		oXmlWriter.WriteNode("edgeColor", sEdgeColor);

		// save colors
		oXmlWriter.WriteNode("brushFaceColorType", m_nBrushFaceColorType);
		oXmlWriter.WriteNode("brushFaceColorMode", m_nBrushFaceColorMode);
		ARGB nFontGradColor1 = m_cFontGradColor1.GetValue();
		CString sFontGradColor1; sFontGradColor1.Format("%u", nFontGradColor1);
		oXmlWriter.WriteNode("fontGradColor1", sFontGradColor1);
		ARGB nFontGradColor2 = m_cFontGradColor2.GetValue();
		CString sFontGradColor2; sFontGradColor2.Format("%u", nFontGradColor2);
		oXmlWriter.WriteNode("fontGradColor2", sFontGradColor2);

		// save texture
		oXmlWriter.WriteNode("textureFilePath", m_sTextureFilePath);
		oXmlWriter.WriteNode("textureAlpha", m_nTextureAlpha);

		// save hatch
		oXmlWriter.WriteNode("hatchType", m_nHatchType);
		ARGB nFontHatchColor1 = m_cFontHatchColor1.GetValue();
		CString sFontHatchColor1; sFontHatchColor1.Format("%u", nFontHatchColor1);
		oXmlWriter.WriteNode("fontHatchColor1", sFontHatchColor1);
		ARGB nFontHatchColor2 = m_cFontHatchColor2.GetValue();
		CString sFontHatchColor2; sFontHatchColor2.Format("%u", nFontHatchColor2);
		oXmlWriter.WriteNode("fontHatchColor2", sFontHatchColor2);

		// end node preset
		oXmlWriter.WriteNodeEnd("preset");
		
		return oXmlWriter.GetXmlString();
	}
	void FromXmlString(CString sXml)
	{
		XmlUtils::CXmlReader oXmlReader;
		if (!oXmlReader.SetXmlString(sXml))
			return;

		// get preset name
		oXmlReader.ReadRootNode("preset");
		m_sCurrentPresetName = oXmlReader.ReadNodeAttribute("name");

		// get font
		m_sFontName = oXmlReader.ReadNodeValue("fontName");
		m_nFontSize = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("fontSize"));
		m_nFontStyle = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("fontStyle"));
		CString sFontColor = oXmlReader.ReadNodeValue("fontColor");
		m_cFontColor.SetValue((ARGB)_atoi64(sFontColor));
		m_nAlignVertical = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("alignVertical"));
		m_nAlignHorizontal = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("alignHorizontal"));

		// get rotate
		m_nRotateAngle = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("rotateAngle"));

		// get shadow
		m_bShadowVisible = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("shadowVisible"));
		m_nShadowDistanceX = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("shadowDistanceX"));
		m_nShadowDistanceY = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("shadowDistanceY"));
		CString sShadowColor =  oXmlReader.ReadNodeValue("shadowColor");
		m_cShadowColor.SetValue((ARGB)_atoi64(sShadowColor));
		m_nShadowBlur = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("shadowBlur"));

		// get edge
		m_nEdgeVisible = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("edgeVisible"));
		m_nEdgeDist = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("edgeDist"));
		CString sEdgeColor = oXmlReader.ReadNodeValue("edgeColor");
		m_cEdgeColor.SetValue((ARGB)_atoi64(sEdgeColor));

		// get colors
		m_nBrushFaceColorType = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("brushFaceColorType"));
		m_nBrushFaceColorMode = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("brushFaceColorMode"));
		CString sFontGradColor1 = oXmlReader.ReadNodeValue("fontGradColor1");
		m_cFontGradColor1.SetValue((ARGB)_atoi64(sFontGradColor1));
		CString sFontGradColor2 = oXmlReader.ReadNodeValue("fontGradColor2");
		m_cFontGradColor2.SetValue((ARGB)_atoi64(sFontGradColor2));

		// get texture
		m_sTextureFilePath = oXmlReader.ReadNodeValue("textureFilePath");
		m_nTextureAlpha = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("textureAlpha"));

		// get hatch
		m_nHatchType = XmlUtils::GetInteger(oXmlReader.ReadNodeValue("hatchType"));
		CString sFontHatchColor1 = oXmlReader.ReadNodeValue("fontHatchColor1");
		m_cFontHatchColor1.SetValue((ARGB)_atoi64(sFontHatchColor1));
		CString sFontHatchColor2 = oXmlReader.ReadNodeValue("fontHatchColor2");
		m_cFontHatchColor2.SetValue((ARGB)_atoi64(sFontHatchColor2));
	}
	TextFormat()
	{
		// format
		m_nAlignVertical = 0;
		m_nAlignHorizontal = 0;

		// rotate
		m_nRotateAngle = 0;

		// preset name
		m_sCurrentPresetName = "";

		// shadow
		m_bShadowVisible = FALSE;
		m_nShadowDistanceX = 0;  
		m_nShadowDistanceY = 0; 
		m_cShadowColor = RGB(0, 0, 0);
		m_nShadowBlur = 0;

		// edge
		m_nEdgeVisible = 0;
		m_nEdgeDist = 0;
		m_cEdgeColor = RGB(0, 0, 0);;

		// brush
		m_nBrushFaceColorType = 0;
		m_nBrushFaceColorMode = 0;
		// old value (do not used)
		m_cFontColor  = RGB(0, 0, 0);
		m_cFontGradColor1  = RGB(0, 0, 0);
		m_cFontGradColor2 = RGB(0, 0, 0);
		m_nHatchType = 0;
		m_cFontHatchColor1 = RGB(0, 0, 0);
		m_cFontHatchColor2 = RGB(0, 0, 0);

		// texture
		m_sTextureFilePath = "";
		m_nTextureAlpha = 0;

		// font
		m_sFontName = "";
		m_nFontSize = 0;
		m_nFontStyle = 0;
	}
};
