#pragma once
#include "../../../Common/ASCUtils.h"
#include "../Interfaces/ASCRenderer.h"
#include "../../../Common/XmlUtils.h"
#include "../agg/ap_aggplusenums.h"

#include <gdiplus.h>

// pen -----------------------------------------------------------
const long c_ag_LineCapFlat				= 0;
const long c_ag_LineCapSquare			= 1;
const long c_ag_LineCapTriangle			= 3;
const long c_ag_LineCapNoAnchor			= 16;
const long c_ag_LineCapSquareAnchor		= 17;
const long c_ag_LineCapRoundAnchor		= 18;
const long c_ag_LineCapDiamondAnchor	= 19;
const long c_ag_LineCapArrowAnchor		= 20;
const long c_ag_LineCapAnchorMask		= 240;
const long c_ag_LineCapCustom			= 255;

const long c_ag_DashCapFlat				= 0;
const long c_ag_DashCapRound			= 2;
const long c_ag_DashCapTriangle			= 3;

const long c_ag_LineJoinMiter			= 0;
const long c_ag_LineJoinBevel			= 1;
const long c_ag_LineJoinRound			= 2;
const long c_ag_LineJoinMiterClipped	= 3;

const long c_ag_PenAlignmentCenter		= 0;
const long c_ag_PenAlignmentInset		= 1;
const long c_ag_PenAlignmentOutset		= 2;
const long c_ag_PenAlignmentLeft		= 3;
const long c_ag_PenAlignmentRight		= 4;
// --------------------------------------------------------------
// brush --------------------------------------------------------
// old constants for brush type
const long c_BrushTypeSolid_				= 0;
const long c_BrushTypeHorizontal_			= 1;
const long c_BrushTypeVertical_				= 2;
const long c_BrushTypeDiagonal1_			= 3;
const long c_BrushTypeDiagonal2_			= 4;
const long c_BrushTypeCenter_				= 5;
const long c_BrushTypePathGradient1_		= 6;	
const long c_BrushTypePathGradient2_		= 7;
const long c_BrushTypeTexture_				= 8;
const long c_BrushTypeHatch1_				= 9;
const long c_BrushTypeHatch53_				= 61;
const long c_BrushTypeGradient1_			= 62;
const long c_BrushTypeGradient6_			= 70;

const long c_BrushTypeSolid					= 1000;
const long c_BrushTypeHorizontal			= 2001;
const long c_BrushTypeVertical				= 2002;
const long c_BrushTypeDiagonal1				= 2003;
const long c_BrushTypeDiagonal2				= 2004;
const long c_BrushTypeCenter				= 2005;
const long c_BrushTypePathGradient1			= 2006;	
const long c_BrushTypePathGradient2			= 2007;
const long c_BrushTypeCylinderHor			= 2008;
const long c_BrushTypeCylinderVer			= 2009;
const long c_BrushTypeTexture				= 3008;
const long c_BrushTypePattern				= 3009;
const long c_BrushTypeHatch1				= 4009;
const long c_BrushTypeHatch53				= 4061;

const long c_BrushTextureModeStretch		= 0;
const long c_BrushTextureModeTile			= 1;
const long c_BrushTextureModeTileCenter		= 2;
// --------------------------------------------------------------

//using namespace Gdiplus;

namespace NSStructures
{
	class CPen
	{
	public:
		long Color;
		long Alpha;
		double Size;

		byte DashStyle;
		byte LineStartCap;
		byte LineEndCap;
		byte LineJoin;

		double* DashPattern;
		long Count;

		double DashOffset;
		
		LONG Align;
		double MiterLimit;
		
	public:
	
		CString ToXmlString(CString strRootNodeName = _T("pen"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);
				oXmlWriter.WriteNode(_T("color"), Color);
				oXmlWriter.WriteNode(_T("alpha"), Alpha);
				oXmlWriter.WriteNode(_T("size"), Size);
				oXmlWriter.WriteNode(_T("style"), DashStyle);
				oXmlWriter.WriteNode(_T("line-start-cap"), LineStartCap);
				oXmlWriter.WriteNode(_T("line-end-cap"), LineEndCap);
				oXmlWriter.WriteNode(_T("line-join"), LineJoin);
				oXmlWriter.WriteNode(_T("dash-pattern-count"), Count);
				oXmlWriter.WriteNodeBegin(_T("dash-pattern"));
				if (DashPattern != NULL)
				{
					for (int i = 0; i < Count; ++i)
					{
						oXmlWriter.WriteNode(_T("dash"), DashPattern[i]);
					}
				}
				oXmlWriter.WriteNodeEnd(_T("dash-pattern"));
				oXmlWriter.WriteNode(_T("dash-offset"), DashOffset);
				oXmlWriter.WriteNode(_T("alignment"), Align);
				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString& strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}

		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("color"), Color);
			oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
			oXmlWriter.WriteAttribute(_T("size"), Size);

			oXmlWriter.WriteAttribute(_T("style"), DashStyle);
			oXmlWriter.WriteAttribute(_T("line-start-cap"), LineStartCap);
			oXmlWriter.WriteAttribute(_T("line-end-cap"), LineEndCap);
			oXmlWriter.WriteAttribute(_T("line-join"), LineJoin);

			oXmlWriter.WriteAttribute(_T("dash-offset"), DashOffset);
			oXmlWriter.WriteAttribute(_T("alignment"), Align);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			Color = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("alpha"), _T("0")));
			Size = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("size"), _T("0")));
			
			DashStyle = (byte)XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("style"), _T("0")));
			LineStartCap = (byte)XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("line-start-cap"), _T("0")));
			LineEndCap = (byte)XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("line-end-cap"), _T("0")));
			LineJoin = (byte)XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("line-join"), _T("0")));

			RELEASEARRAYOBJECTS(DashPattern);
			Count = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("dash-pattern-count"), _T("0")));
			if (Count != 0)
			{
				XmlUtils::CXmlNode oPatternNode;
				oXmlReader.ReadNodeList(_T("dash-pattern"));
				if (oPatternNode.FromXmlString(oXmlReader.ReadNodeXml(0)))
				{
					XmlUtils::CXmlNodes oDashNodes;
					if (oPatternNode.GetNodes(_T("dash"), oDashNodes))
					{
						Count = oDashNodes.GetCount();
						if (Count != 0)
						{
							DashPattern = new double[Count];
							XmlUtils::CXmlNode oDashNode;
							for (int i = 0; i < Count; ++i)
							{
								oDashNodes.GetAt(i, oDashNode);
								DashPattern[i] = XmlUtils::GetDouble(oDashNode.GetText());
							}
						}
					}
				}
			}

			DashOffset = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("dash-offset"), _T("0")));

			Align =  XmlUtils::GetInteger(oXmlReader.ReadNodeAttribute(_T("alignment"), _T("0")));
		}
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("0")));
			Size = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));

			DashStyle = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("style"), _T("0")));
			LineStartCap = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("line-start-cap"), _T("0")));
			LineEndCap = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("line-end-cap"), _T("0")));
			LineJoin = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("line-join"), _T("0")));

			RELEASEARRAYOBJECTS(DashPattern);
			Count = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("dash-pattern-count"), _T("0")));
			if (Count != 0)
			{
				XmlUtils::CXmlNode oPatternNode;
				if (oXmlNode.GetNode(_T("dash-pattern"), oPatternNode))
				{
					XmlUtils::CXmlNodes oDashNodes;
					if (oPatternNode.GetNodes(_T("dash"), oDashNodes))
					{
						Count = oDashNodes.GetCount();
						if (Count != 0)
						{
							DashPattern = new double[Count];
							XmlUtils::CXmlNode oDashNode;
							for (int i = 0; i < Count; ++i)
							{
								oDashNodes.GetAt(i, oDashNode);
								DashPattern[i] = XmlUtils::GetDouble(oDashNode.GetText());
							}
						}
					}
				}
			}

			DashOffset = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("dash-offset"), _T("0")));

			Align = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("alignment"), _T("0")));
		}

		void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
		{
			Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-alpha"), _T("0")));
			Size = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("pen-size"), _T("0")));
			DashOffset = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("dash-offset"), _T("0")));

			DashStyle = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-style"), _T("0")));
			LineStartCap = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-line-start-cap"), _T("0")));
			LineEndCap = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-line-end-cap"), _T("0")));
			LineJoin = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-line-join"), _T("0")));

			RELEASEARRAYOBJECTS(DashPattern);
			Count = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-dash-pattern-count"), _T("0")));
			if (Count != 0)
			{
				XmlUtils::CXmlNode oPatternNode;
				if (oXmlNode.GetNode(_T("dash-pattern"), oPatternNode))
				{
					XmlUtils::CXmlNodes oDashNodes;
					if (oPatternNode.GetNodes(_T("dash"), oDashNodes))
					{
						Count = oDashNodes.GetCount();
						if (Count != 0)
						{
							DashPattern = new double[Count];
							XmlUtils::CXmlNode oDashNode;
							for (int i = 0; i < Count; ++i)
							{
								oDashNodes.GetAt(i, oDashNode);
								DashPattern[i] = XmlUtils::GetDouble(oDashNode.GetText());
							}
						}
					}
				}
			}

			DashOffset = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("pen-dash-offset"), _T("0")));

			Align = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-alignment"), _T("0")));
		}
		
		void GetDashPattern(double* arrDashPattern, long& nCount) const
		{
			if (nCount == Count)
			{
				for (int i = 0; i < Count; ++i)
				{
					arrDashPattern[i] = DashPattern[i];
				}
			}
		}
		void SetDashPattern(double* arrDashPattern, long nCount)
		{
			if ((arrDashPattern == NULL) || (nCount == 0))
			{
				Count = 0;
				RELEASEARRAYOBJECTS(DashPattern);
			}
			else
			{
				if (Count != nCount)
				{
					Count = nCount;
					RELEASEARRAYOBJECTS(DashPattern);
					DashPattern = new double[Count];
				}

				for (int i = 0; i < Count; ++i)
				{
					DashPattern[i] = arrDashPattern[i];
				}
			}
		}
		
		void ScaleAlpha( double dScale )
		{
			long dNewAlpha = long(Alpha * dScale + 0.5);
			
			if( dNewAlpha > 255 ) dNewAlpha = 255;
			else if( dNewAlpha < 0 ) dNewAlpha = 0;
			
			Alpha = dNewAlpha;
		}

		BOOL IsEqual(CPen* pPen)
		{
			if (NULL == pPen)
				return FALSE;

			return ((Color == pPen->Color) && (Alpha == pPen->Alpha) && (Size == pPen->Size) &&
				(DashStyle == pPen->DashStyle) && (LineStartCap == pPen->LineStartCap) &&
				(LineEndCap == pPen->LineEndCap) && (LineJoin == pPen->LineJoin));
		}
		void SetToRenderer(IASCRenderer *pRenderer)
		{
			pRenderer->put_PenColor(Color);
			pRenderer->put_PenAlpha(Alpha);
			pRenderer->put_PenSize(Size);
			pRenderer->put_PenDashStyle(DashStyle);
			pRenderer->put_PenLineStartCap(LineStartCap);
			pRenderer->put_PenLineEndCap(LineEndCap);
			pRenderer->put_PenLineJoin(LineJoin);
			pRenderer->put_PenAlign(Align);

			if (DashStyle != Aggplus::DashStyleSolid)
			{
				SAFEARRAYBOUND rgsab;
				rgsab.lLbound	= 0;
				rgsab.cElements	= Count;

				SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
				memcpy(pArray->pvData, DashPattern, Count * sizeof(double));

				pRenderer->PenDashPattern(pArray);

				RELEASEARRAY(pArray);
				pRenderer->put_PenDashOffset(DashOffset);
			}
		}
		void SetDefaultParams()
		{
			Color = 0;
			Alpha = 255;
			Size  = 1;

			DashStyle    = 0;
			LineStartCap = 0;
			LineEndCap   = 0;
			LineJoin     = 0;

			DashPattern = NULL;
			Count       = 0;

			DashOffset = 0;
			Align = Gdiplus::PenAlignmentCenter;
			MiterLimit = 0.5;
		}

		
	public:

		CPen()
		{
			SetDefaultParams();
		}
		CPen( const CPen& other )
		{
			SetDefaultParams();
			*this = other;
		}
		CPen& operator=(const CPen& other)
		{
			Color = other.Color;
			Alpha = other.Alpha;
			Size  = other.Size;

			DashStyle = other.DashStyle;
			LineStartCap = other.LineStartCap;
			LineEndCap = other.LineEndCap;
			LineJoin = other.LineJoin;

			RELEASEARRAYOBJECTS(DashPattern);
			Count = other.Count;
			if (Count != 0)
			{
				DashPattern = new double[Count];
				for (int i = 0; i < Count; ++i)
				{
					DashPattern[i] = other.DashPattern[i];
				}
			}

			DashOffset = other.DashOffset;

			Align = other.Align;
			MiterLimit = other.MiterLimit;

			return *this;
		}
		virtual ~CPen()
		{
			RELEASEARRAYOBJECTS(DashPattern);
		}
	};

	class CBrush
	{
	public:
		struct TSubColor
		{
			long color;
			long position; // [0..65536]
		};

	public:
		long Type;
		
		long Color1;
		long Color2;
		long Alpha1;
		long Alpha2;
		
		CString TexturePath;
		long TextureAlpha;
		long TextureMode;
		
		BOOL Rectable;
		Gdiplus::RectF Rect;

		double LinearAngle;

		ATL::CSimpleArray<TSubColor> m_arrSubColors;

	public:
		void LoadSubColors( const CString& str )
		{
			m_arrSubColors.RemoveAll();

			if( str.IsEmpty() )
				return;

			TSubColor subcolor;
			int start = 0;			
			
			for(;;)
			{
				int pos = str.Find( _T(','), start );
				if( pos < 0 )
					break;

				subcolor.color = ::_ttoi( str.Mid( start, pos - start ) );
				start = pos + 1;

				pos = str.Find( _T(';'),  start );
				if( pos < 0 )
					break;

				subcolor.position = ::_ttoi( str.Mid( start, pos - start ) );
				start = pos + 1;

				m_arrSubColors.Add( subcolor );
			}
		}

		CString ToXmlString(CString strRootNodeName = _T("brush"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);

				oXmlWriter.WriteNode(_T("type"), Type);
				oXmlWriter.WriteNode(_T("color1"), Color1);
				oXmlWriter.WriteNode(_T("color2"), Color2);
				oXmlWriter.WriteNode(_T("alpha1"), Alpha1);
				oXmlWriter.WriteNode(_T("alpha2"), Alpha2);
				oXmlWriter.WriteNode(_T("texturepath"), TexturePath);
				oXmlWriter.WriteNode(_T("texturealpha"), TextureAlpha);
				oXmlWriter.WriteNode(_T("texturemode"), TextureMode);
				oXmlWriter.WriteNode(_T("rectable"), Rectable);
				oXmlWriter.WriteNode(_T("linearangle"), LinearAngle);

				oXmlWriter.WriteNodeBegin(_T("rectangle"), TRUE);
				oXmlWriter.WriteAttribute(_T("left"), Rect.X);
				oXmlWriter.WriteAttribute(_T("top"), Rect.Y);
				oXmlWriter.WriteAttribute(_T("width"), Rect.Width);
				oXmlWriter.WriteAttribute(_T("height"), Rect.Height);
				oXmlWriter.WriteNodeEnd(_T("rectangle"), TRUE);

				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString& strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}
	
		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("type"), Type);
			oXmlWriter.WriteAttribute(_T("color1"), Color1);
			oXmlWriter.WriteAttribute(_T("color2"), Color2);
			oXmlWriter.WriteAttribute(_T("alpha1"), Alpha1);
			oXmlWriter.WriteAttribute(_T("alpha2"), Alpha2);
			oXmlWriter.WriteAttribute(_T("texturepath"), TexturePath);
			oXmlWriter.WriteAttribute(_T("texturealpha"), TextureAlpha);
			oXmlWriter.WriteAttribute(_T("texturemode"), TextureMode);
			oXmlWriter.WriteAttribute(_T("rectable"), Rectable);
			oXmlWriter.WriteAttribute(_T("linearangle"), LinearAngle);

			oXmlWriter.WriteAttribute(_T("rect-left"), Rect.X);
			oXmlWriter.WriteAttribute(_T("rect-top"), Rect.Y);
			oXmlWriter.WriteAttribute(_T("rect-width"), Rect.Width);
			oXmlWriter.WriteAttribute(_T("rect-height"), Rect.Height);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			Type = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("type"), _T("0")));
			Type = ConstantCompatible(Type);

			Color1 = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("color1"), _T("0")));
			Color2 = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("color2"), _T("0")));
			Alpha1 = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("alpha1"), _T("0")));
			Alpha2 = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("alpha2"), _T("0")));

			TexturePath = oXmlReader.ReadNodeAttributeOrValue(_T("texturepath"), _T(""));
			TextureAlpha = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("texturealpha"), _T("0")));
			TextureMode = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("texturemode"), _T("0")));

			Rectable = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("rectable"), _T("0")));
			LinearAngle = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("linearangle"), _T("0")));

			Rect.X = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("rect-left")));
			Rect.Y = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("rect-top")));
			Rect.Width = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("rect-width")));
			Rect.Height = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("rect-height")));

			LoadSubColors( oXmlReader.ReadNodeAttribute( _T("subcolors") ) );

			if (oXmlReader.ReadNode(_T("rectangle")))
			{
				Rect.X = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttribute(_T("left")));
				Rect.Y = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttribute(_T("top")));
				Rect.Width = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttribute(_T("width")));
				Rect.Height = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlReader.ReadNodeAttribute(_T("height")));
			}
		}		
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			Type = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
			Type = ConstantCompatible(Type);

			Color1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("color1"), _T("0")));
			Color2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("color2"), _T("0")));
			Alpha1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("alpha1"), _T("0")));
			Alpha2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("alpha2"), _T("0")));

			TexturePath = oXmlNode.GetAttributeOrValue(_T("texturepath"), _T(""));
			TextureAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("texturealpha"), _T("0")));
			TextureMode = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("texturemode"), _T("0")));

			Rectable = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("rectable"), _T("0")));
			LinearAngle = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("linearangle"), _T("0")));

			Rect.X = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("rect-left")));
			Rect.Y = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("rect-top")));
			Rect.Width = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("rect-width")));
			Rect.Height = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("rect-height")));

			XmlUtils::CXmlNode oNodeRect;
			if (oXmlNode.GetNode(_T("rectangle"), oNodeRect))
			{
				Rect.X = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("left")));
				Rect.Y = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("top")));
				Rect.Width = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("width")));
				Rect.Height = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("height")));
			}
		}	

		void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
		{
			Type = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-type"), _T("0")));
			Type = ConstantCompatible(Type);

			Color1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-color1"), _T("0")));
			Color2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-color2"), _T("0")));
			Alpha1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-alpha1"), _T("0")));
			Alpha2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-alpha2"), _T("0")));

			TexturePath = oXmlNode.GetAttributeOrValue(_T("brush-texturepath"), _T(""));
			TextureAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-texturealpha"), _T("0")));
			TextureMode = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-texturemode"), _T("0")));

			Rectable = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-rectable"), _T("0")));
			LinearAngle = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-linearangle"), _T("0")));

			Rect.X = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-left")));
			Rect.Y = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-top")));
			Rect.Width = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-width")));
			Rect.Height = (Gdiplus::REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-height")));

			XmlUtils::CXmlNode oNodeRect;
			if (oXmlNode.GetNode(_T("brush-rectangle"), oNodeRect))
			{
				Rect.X = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("left")));
				Rect.Y = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("top")));
				Rect.Width = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("width")));
				Rect.Height = (Gdiplus::REAL)XmlUtils::GetDouble(oNodeRect.GetAttributeOrValue(_T("height")));
			}
		}	


		inline LONG ConstantCompatible(LONG nConstant)
		{
			if( c_BrushTypeDiagonal1_ == nConstant )
				nConstant = c_BrushTypeDiagonal2_;
			else if( c_BrushTypeDiagonal2_ == nConstant )
				nConstant = c_BrushTypeDiagonal1_;

			if (1000 <= nConstant)
				return nConstant;
			if (c_BrushTypeSolid_ == nConstant)
				return nConstant + 1000;
			if (c_BrushTypeHorizontal_ <= nConstant && c_BrushTypePathGradient2_ >= nConstant)
				return nConstant + 2000;
			if (c_BrushTypeTexture_ == nConstant)
				return nConstant + 3000;
			if (c_BrushTypeHatch1_ <= nConstant && c_BrushTypeHatch53_ >= nConstant)
				return nConstant + 4000;
			if (c_BrushTypeGradient1_ <= nConstant && c_BrushTypeGradient6_ >= nConstant)
				return nConstant + 2000 - 61;

			return 1000;
		}
		
		void ScaleAlpha1( double dScale )
		{
			long dNewAlpha = long(Alpha1 * dScale + 0.5);
			
			if( dNewAlpha > 255 ) dNewAlpha = 255;
			else if( dNewAlpha < 0 ) dNewAlpha = 0;
			
			Alpha1 = dNewAlpha;
		}
		void ScaleAlpha2( double dScale )
		{
			long dNewAlpha = long(Alpha2 * dScale + 0.5);
			
			if( dNewAlpha > 255 ) dNewAlpha = 255;
			else if( dNewAlpha < 0 ) dNewAlpha = 0;
			
			Alpha2 = dNewAlpha;
		}

		void ScaleTextureAlpha( double dScale )
		{
			long dNewAlpha = long(TextureAlpha * dScale + 0.5);
			
			if( dNewAlpha > 255 ) dNewAlpha = 255;
			else if( dNewAlpha < 0 ) dNewAlpha = 0;
			
			TextureAlpha = dNewAlpha;
		}

		
		BOOL IsEqual(CBrush* pBrush)
		{
			if (NULL == pBrush)
				return FALSE;

			/*return ((Type == pBrush->Type) && 
				(Color1 == pBrush->Color1) && (Color2 == pBrush->Color2) &&
				(Alpha1 == pBrush->Alpha1) && (Alpha2 == pBrush->Alpha2));*/

			return ((Type == pBrush->Type) && 
				(Color1 == pBrush->Color1) && (Color2 == pBrush->Color2) &&
				(Alpha1 == pBrush->Alpha1) && (Alpha2 == pBrush->Alpha2) && (LinearAngle == pBrush->LinearAngle) && 
				(TexturePath == pBrush->TexturePath) && (TextureAlpha == pBrush->TextureAlpha) && (TextureMode == pBrush->TextureMode) &&
				(Rectable == pBrush->Rectable) && (Rect.Equals(pBrush->Rect)));
		}

		void SetDefaultParams()
		{
			Type = c_BrushTypeSolid;

			Color1 = 0;
			Alpha1 = 255;
			Color2 = 0;
			Alpha2 = 255;

			TextureAlpha = 255;
			TextureMode  = c_BrushTextureModeStretch;

			LinearAngle = 0;

			TexturePath = _T("");

			Rectable = FALSE;

			Rect.X      = 0.0F;
			Rect.Y      = 0.0F;
			Rect.Width  = 0.0F;
			Rect.Height = 0.0F;

			m_arrSubColors.RemoveAll();
		}
		
	public:
		
		CBrush()
		{
			SetDefaultParams();
		}
		CBrush( const CBrush& other )
		{
			Type    = other.Type;
			
			Color1  = other.Color1;
			Alpha1  = other.Alpha1;
			Color2  = other.Color2;
			Alpha2  = other.Alpha2;

			TexturePath  = other.TexturePath;
			TextureAlpha = other.TextureAlpha;
			TextureMode  = other.TextureMode;

			Rectable = other.Rectable;
			Rect     = other.Rect;

			LinearAngle = other.LinearAngle;
			m_arrSubColors = other.m_arrSubColors;
		}
		CBrush& operator=(const CBrush& other)
		{
			Type    = other.Type;
			
			Color1  = other.Color1;
			Alpha1  = other.Alpha1;
			Color2  = other.Color2;
			Alpha2  = other.Alpha2;

			TexturePath  = other.TexturePath;
			TextureAlpha = other.TextureAlpha;
			TextureMode  = other.TextureMode;

			Rectable = other.Rectable;
			Rect     = other.Rect;

			LinearAngle = other.LinearAngle;
			m_arrSubColors = other.m_arrSubColors;

			return *this;
		}
		virtual ~CBrush()
		{
		}

		BOOL IsTexture()
		{
			return (c_BrushTypeTexture == Type || c_BrushTypePattern == Type);
		}
		BOOL IsOneColor()
		{
			return (c_BrushTypeSolid == Type);
		}
		BOOL IsTwoColor()
		{
			return ((c_BrushTypeHorizontal <= Type && c_BrushTypeCylinderVer >= Type) ||
					(c_BrushTypeHatch1 <= Type && c_BrushTypeHatch53 >= Type));
		}

		void	SetToRenderer(IASCRenderer *pRenderer)
		{
			Type = ConstantCompatible(Type);
			pRenderer->put_BrushType(Type);
			if (IsOneColor())
			{
				pRenderer->put_BrushColor1(Color1);
				pRenderer->put_BrushAlpha1(Alpha1);
			}
			else if (IsTexture())
			{
				BSTR bstrTexturePath = TexturePath.AllocSysString();
				pRenderer->put_BrushTexturePath(bstrTexturePath);
				SysFreeString(bstrTexturePath);
				pRenderer->put_BrushTextureMode(TextureMode);
				pRenderer->put_BrushTextureAlpha(TextureAlpha);
				pRenderer->BrushRect(Rectable, Rect.X, Rect.Y, Rect.Width, Rect.Height);
			}
			else if (IsTwoColor())
			{
				pRenderer->put_BrushColor1(Color1);
				pRenderer->put_BrushAlpha1(Alpha1);
				pRenderer->put_BrushColor2(Color2);
				pRenderer->put_BrushAlpha2(Alpha2);
			}
		}

		void	SetToRenderer(IASCRenderer *pRenderer, const float &fTransparency)
		{
			Type = ConstantCompatible(Type);
			pRenderer->put_BrushType(Type);
			if (IsOneColor())
			{
				pRenderer->put_BrushColor1(Color1);
				pRenderer->put_BrushAlpha1((LONG)(Alpha1 * fTransparency));
			}
			else if (IsTexture())
			{
				BSTR bstrTexturePath = TexturePath.AllocSysString();
				pRenderer->put_BrushTexturePath(bstrTexturePath);
				SysFreeString(bstrTexturePath);
				pRenderer->put_BrushTextureMode(TextureMode);
				pRenderer->put_BrushTextureAlpha((LONG)(TextureAlpha * fTransparency));
				pRenderer->BrushRect(Rectable, Rect.X, Rect.Y, Rect.Width, Rect.Height);
			}
			else if (IsTwoColor())
			{
				pRenderer->put_BrushColor1(Color1);
				pRenderer->put_BrushAlpha1((LONG)(Alpha1 * fTransparency));
				pRenderer->put_BrushColor2(Color2);
				pRenderer->put_BrushAlpha2((LONG)(Alpha2 * fTransparency));
			}
		}
	};
	class CFont
	{
	public:

		CString Path;
		CString Name;
		double Size;
		BOOL Bold;
		BOOL Italic;
		byte Underline;
		byte Strikeout;

		BOOL StringGID;
		double CharSpace;
	
	public:

		CString ToXmlString(CString strRootNodeName = _T("font"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);
				oXmlWriter.WriteNode(_T("name"), Name);
				oXmlWriter.WriteNode(_T("size"), Size);
				oXmlWriter.WriteNode(_T("bold"), Bold);
				oXmlWriter.WriteNode(_T("italic"), Italic);
				oXmlWriter.WriteNode(_T("underline"), Underline);
				oXmlWriter.WriteNode(_T("strikeout"), Strikeout);
				oXmlWriter.WriteNode(_T("path"), Path);
				oXmlWriter.WriteNode(_T("gid"), StringGID);
				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString& strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}
	
		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("name"), Name);
			oXmlWriter.WriteAttribute(_T("size"), Size);
			oXmlWriter.WriteAttribute(_T("bold"), Bold);
			oXmlWriter.WriteAttribute(_T("italic"), Italic);
			oXmlWriter.WriteAttribute(_T("underline"), Underline);
			oXmlWriter.WriteAttribute(_T("strikeout"), Strikeout);
			oXmlWriter.WriteAttribute(_T("path"), Path);
			oXmlWriter.WriteAttribute(_T("gid"), StringGID);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			Name = oXmlReader.ReadNodeAttributeOrValue(_T("name"), _T("Arial"));
			Size = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("size"), _T("0")));
			Bold = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("bold"), _T("0")));
			Italic = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("italic"), _T("0")));
			Underline = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("underline"), _T("0")));
			Strikeout = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("strikeout"), _T("0")));
			Path = oXmlReader.ReadNodeAttributeOrValue(_T("path"), _T(""));
			StringGID = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("gid"), _T("0")));
		}		
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			Name = oXmlNode.GetAttributeOrValue(_T("name"), _T("Arial"));
			Size = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
			Bold = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("bold"), _T("0")));
			Italic = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("italic"), _T("0")));
			Underline = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("underline"), _T("0")));
			Strikeout = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("strikeout"), _T("0")));
			Path = oXmlNode.GetAttributeOrValue(_T("path"), _T(""));
			StringGID = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("gid"), _T("0")));
		}

		void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
		{
			Name = oXmlNode.GetAttributeOrValue(_T("font-name"), _T("Arial"));
			Size = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("font-size"), _T("0")));
			Bold = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-bold"), _T("0")));
			Italic = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-italic"), _T("0")));
			Underline = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-underline"), _T("0")));
			Strikeout = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-strikeout"), _T("0")));
			Path = oXmlNode.GetAttributeOrValue(_T("font-path"), _T(""));
			StringGID = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-gid"), _T("0")));
		}
		
		BOOL IsEqual(CFont* pFont)
		{
			if (NULL == pFont)
				return FALSE;

			return ((Name == pFont->Name) && (Path == pFont->Path)  && (StringGID == pFont->StringGID) && (Size == pFont->Size) &&
				(Bold == pFont->Bold) && (Italic == pFont->Italic) &&
				(Underline == pFont->Underline) && (Strikeout == pFont->Strikeout));
		}

		LONG GetStyle() const 
		{
			LONG lStyle = 0;
			if (Bold)
				lStyle |= 0x01;
			if (Italic)
				lStyle |= 0x02;
			lStyle |= Underline << 2;
			lStyle |= Strikeout << 7;
			return lStyle;
		}
		void SetStyle(LONG const& lStyle)
		{
			Bold	= (0x01 == (0x01 & lStyle));
			Italic	= (0x02 == (0x02 & lStyle));
			Underline = (byte)(0x7C & lStyle) >> 2;
			Strikeout = (byte)(0x0180 & lStyle) >> 7;
		}
		void	SetToRenderer(IASCRenderer *pRenderer)
		{
			BSTR bstrName = Name.AllocSysString();			
			pRenderer->put_FontName(bstrName);			
			SysFreeString(bstrName);

			BSTR bstrPath = Path.AllocSysString();
			pRenderer->put_FontPath(bstrPath);
			SysFreeString(bstrPath);

			pRenderer->put_FontSize(Size);
			pRenderer->put_FontStyle(GetStyle());
			pRenderer->put_FontStringGID(StringGID);
			pRenderer->put_FontCharSpace(CharSpace);
		}
		void SetDefaultParams()
		{
			Name = _T("Arial");
			Path = _T("");
			
			Size      = 0;
			Bold      = FALSE;
			Italic    = FALSE;
			Underline = 0;
			Strikeout = 0;

			StringGID = FALSE;
			CharSpace = 0.0;
		}

		
	public:

		CFont()
		{
			SetDefaultParams();
		}
		CFont( const CFont& other )
		{
			*this = other;
		}
		CFont& operator=(const CFont& other)
		{
			Name      = other.Name;
			Path	  = other.Path;
			Size      = other.Size;
			Bold      = other.Bold;
			Italic    = other.Italic;
			Underline = other.Underline;
			Strikeout = other.Strikeout;

			StringGID = other.StringGID;
			CharSpace = other.CharSpace;

			return *this;
		}
		virtual ~CFont()
		{
		}
	};
	class CFormat
	{
	public:
	
		BOOL AntiAliasPen;
		BOOL AntiAliasBrush;
		BOOL AntiAliasText;

		long StringAlignmentVertical;
		long StringAlignmentHorizontal;

		BOOL WordWrapBySymbol;

		long ImageAlphMode;

	public:

		CString ToXmlString(CString strRootNodeName = _T("format"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);
				oXmlWriter.WriteNode(_T("antialiaspen"), AntiAliasPen);
				oXmlWriter.WriteNode(_T("antialiasbrush"), AntiAliasBrush);
				oXmlWriter.WriteNode(_T("antialiastext"), AntiAliasText);
				oXmlWriter.WriteNode(_T("stringalignmentvertical"), StringAlignmentVertical);
				oXmlWriter.WriteNode(_T("stringalignmenthorizontal"), StringAlignmentHorizontal);
				oXmlWriter.WriteNode(_T("wordwrapbysymbol"), WordWrapBySymbol);
				oXmlWriter.WriteNode(_T("imagealphmode"), ImageAlphMode);
				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}
	
		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("antialiaspen"), AntiAliasPen);
			oXmlWriter.WriteAttribute(_T("antialiasbrush"), AntiAliasBrush);
			oXmlWriter.WriteAttribute(_T("antialiastext"), AntiAliasText);
			oXmlWriter.WriteAttribute(_T("stringalignmentvertical"), StringAlignmentVertical);
			oXmlWriter.WriteAttribute(_T("stringalignmenthorizontal"), StringAlignmentHorizontal);
			oXmlWriter.WriteAttribute(_T("wordwrapbysymbol"), WordWrapBySymbol);
			oXmlWriter.WriteAttribute(_T("imagealphmode"), ImageAlphMode);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			AntiAliasPen = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("antialiaspen"), _T("1")));
			AntiAliasBrush = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("antialiasbrush"), _T("1")));
			AntiAliasText = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("antialiastext"), _T("0")));
			StringAlignmentVertical = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("stringalignmentvertical"), _T("0")));
			StringAlignmentHorizontal = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("stringalignmenthorizontal"), _T("0")));
			WordWrapBySymbol = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("wordwrapbysymbol"), _T("0")));
			ImageAlphMode = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("imagealphmode"), _T("0")));
		}		
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			AntiAliasPen = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("antialiaspen"), _T("1")));
			AntiAliasBrush = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("antialiasbrush"), _T("1")));
			AntiAliasText = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("antialiastext"), _T("0")));
			StringAlignmentVertical = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-stringalignmentvertical"), _T("1")));
			StringAlignmentHorizontal = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-stringalignmenthorizontal"), _T("1")));
			WordWrapBySymbol = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("wordwrapbysymbol"), _T("0")));
			ImageAlphMode = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("imagealphmode"), _T("0")));
		}		
			
	public:
		
		CFormat()
		{
			AntiAliasPen = TRUE;
			AntiAliasBrush = TRUE;
			AntiAliasText = TRUE;
			StringAlignmentVertical = 0;
			StringAlignmentHorizontal = 0;
			WordWrapBySymbol = FALSE;
			ImageAlphMode = 0;
		}
		CFormat( const CFormat& other )
		{
			AntiAliasPen   = other.AntiAliasPen;
			AntiAliasBrush = other.AntiAliasBrush;
			AntiAliasText  = other.AntiAliasText;
			StringAlignmentVertical   = other.StringAlignmentVertical;
			StringAlignmentHorizontal = other.StringAlignmentHorizontal;
			WordWrapBySymbol = other.WordWrapBySymbol;
			ImageAlphMode  = other.ImageAlphMode;
		}
		CFormat& operator=(const CFormat& other)
		{
			AntiAliasPen   = other.AntiAliasPen;
			AntiAliasBrush = other.AntiAliasBrush;
			AntiAliasText  = other.AntiAliasText;
			StringAlignmentVertical   = other.StringAlignmentVertical;
			StringAlignmentHorizontal = other.StringAlignmentHorizontal;
			WordWrapBySymbol = other.WordWrapBySymbol;
			ImageAlphMode  = other.ImageAlphMode;
			
			return *this;
		}
		~CFormat()
		{
		}
	};

	class CShadow
	{
	public:
	
		BOOL Visible;
		double DistanceX;
		double DistanceY;
		double BlurSize;
		long Color;
		long Alpha;
	
	public:

		CString ToXmlString(CString strRootNodeName = _T("shadow"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);
				oXmlWriter.WriteNode(_T("visible"), Visible);
				oXmlWriter.WriteNode(_T("distancex"), DistanceX);
				oXmlWriter.WriteNode(_T("distancey"), DistanceY);
				oXmlWriter.WriteNode(_T("blursize"), BlurSize);
				oXmlWriter.WriteNode(_T("color"), Color);
				oXmlWriter.WriteNode(_T("alpha"), Alpha);
				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString& strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}
	
		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("visible"), Visible);
			oXmlWriter.WriteAttribute(_T("distancex"), DistanceX);
			oXmlWriter.WriteAttribute(_T("distancey"), DistanceY);
			oXmlWriter.WriteAttribute(_T("blursize"), BlurSize);
			oXmlWriter.WriteAttribute(_T("color"), Color);
			oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			Visible = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("visible"), _T("0")));
			DistanceX = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("distancex"), _T("0")));
			DistanceY = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("distancey"), _T("0")));
			BlurSize = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("blursize"), _T("0")));
			Color = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("alpha"), _T("0")));
		}		
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			Visible = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("visible"), _T("0")));
			DistanceX = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("distancex"), _T("0")));
			DistanceY = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("distancey"), _T("0")));
			BlurSize = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("blursize"), _T("0")));
			Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("0")));
		}		
		void SetDefaultParams()
		{
			Visible   = FALSE;
			DistanceX = 15;
			DistanceY = 15;
			BlurSize  = 0;
			Color     = 0;
			Alpha     = 120;
		}
		
	public:
	
		CShadow()
		{
			SetDefaultParams();
		}
		CShadow( const CShadow& other )
		{
			Visible   = other.Visible;
			DistanceX = other.DistanceX;
			DistanceY = other.DistanceY;
			BlurSize  = other.BlurSize;
			Color     = other.Color;
			Alpha     = other.Alpha;
						
		}
		CShadow& operator=(const CShadow& other)
		{
			Visible   = other.Visible;
			DistanceX = other.DistanceX;
			DistanceY = other.DistanceY;
			BlurSize  = other.BlurSize;
			Color     = other.Color;
			Alpha     = other.Alpha;

			return *this;
		}
		virtual ~CShadow()
		{
		}
	};
	class CBackground
	{
	public:	
		long Color;
		long Alpha;
	
	public:

		CString ToXmlString(CString strRootNodeName = _T("background"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);
				oXmlWriter.WriteNode(_T("color"), Color);
				oXmlWriter.WriteNode(_T("alpha"), Alpha);
				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString& strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}
	
		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("color"), Color);
			oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			Color = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("alpha"), _T("0")));
		}		
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("0")));
		}		
		
	public:
	
		CBackground()
		{
			Color = 0;
			Alpha = 0;
		}
		CBackground( const CBackground& other )
		{
			Color     = other.Color;
			Alpha     = other.Alpha;						
		}
		CBackground& operator=(const CBackground& other)
		{
			Color     = other.Color;
			Alpha     = other.Alpha;

			return *this;
		}
		virtual ~CBackground()
		{
		}
	};
	class CEdgeText
	{
	public:
	
		long Visible;
		double Dist;
		long Color;
		long Alpha;
	
	public:
		
		CString ToXmlString(CString strRootNodeName = _T("edgetext"), BOOL bAttributed = FALSE)
		{
			XmlUtils::CXmlWriter oXmlWriter;

			if (!bAttributed)
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName);
				oXmlWriter.WriteNode(_T("visible"), Visible);
				oXmlWriter.WriteNode(_T("dist"), Dist);
				oXmlWriter.WriteNode(_T("color"), Color);
				oXmlWriter.WriteNode(_T("alpha"), Alpha);
				oXmlWriter.WriteNodeEnd(strRootNodeName);
			}
			else
			{
				oXmlWriter.WriteNodeBegin(strRootNodeName, TRUE);

				ToXmlAttributes(oXmlWriter);

				oXmlWriter.WriteNodeEnd(strRootNodeName, TRUE);
			}

			return oXmlWriter.GetXmlString();
		}
		void FromXmlString(CString& strXml)
		{
			XmlUtils::CXmlReader oXmlReader;

			oXmlReader.SetXmlString(strXml);
			oXmlReader.ReadRootNode();

			FromXmlAttributes(oXmlReader);
		}
	
		void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
		{
			oXmlWriter.WriteAttribute(_T("visible"), Visible);
			oXmlWriter.WriteAttribute(_T("dist"), Dist);
			oXmlWriter.WriteAttribute(_T("color"), Color);
			oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
		}
		void FromXmlAttributes(XmlUtils::CXmlReader& oXmlReader)
		{
			Visible = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("edge-visible"), _T("0")));
			Dist = XmlUtils::GetDouble(oXmlReader.ReadNodeAttributeOrValue(_T("edge-dist"), _T("0")));
			Color = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("edge-color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlReader.ReadNodeAttributeOrValue(_T("edge-alpha"), _T("0")));
		}		
		void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		{
			Visible = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-visible"), _T("0")));
			Dist = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("edge-dist"), _T("0")));
			Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-color"), _T("0")));
			Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-alpha"), _T("0")));
		}		
		void SetDefaultParams()
		{
			Visible = 0;
			Dist    = 5;
			Color   = 0;
			Alpha   = 255;
		}
		
	public:
	
		CEdgeText()
		{
			SetDefaultParams();
		}
		CEdgeText( const CEdgeText& other )
		{
			Visible = other.Visible;
			Dist    = other.Dist;
			Color   = other.Color;
			Alpha   = other.Alpha;
		}
		CEdgeText& operator=(const CEdgeText& other)
		{
			Visible = other.Visible;
			Dist    = other.Dist;
			Color   = other.Color;
			Alpha   = other.Alpha;

			return *this;
		}
		virtual ~CEdgeText()
		{
		}
	};

	static void SetMatrix(Gdiplus::Matrix& oDst, const Gdiplus::Matrix& oSrc)
	{
		Gdiplus::REAL mass[6];
		oSrc.GetElements(mass);

		oDst.SetElements(mass[0], mass[1], mass[2], mass[3], mass[4], mass[5]);
	}
}