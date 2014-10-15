#pragma once

#ifdef _WIN32
	#include <atlcoll.h>
	#include <gdiplus.h>
#endif

#include "Metric.h"
#include "Effects.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
#include "../../Common/ASCUtils.h"
#include "XmlWriter.h"



#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/structures.h"

namespace NSPresentationEditor
{
	class CExFilesInfo
	{
	public:
		enum ExFilesType
		{
			eftNone		= 0,
			eftVideo	= 1,
			eftAudio	= 2
		};
	public:
		DWORD		m_dwID;		
		CString		m_strFilePath;

		// clip
		double		m_dStartTime;
		double		m_dEndTime;

		// loop
		bool		m_bLoop;

	public:
		CExFilesInfo()
		{
			m_dwID			= 0;
			m_strFilePath	= _T("");

			m_dStartTime	= 0.0;
			m_dEndTime		= -1.0;

			m_bLoop			= false;
		}
		CExFilesInfo(const CExFilesInfo& oSrc)
		{
			*this = oSrc;
		}
		CExFilesInfo& operator=(const CExFilesInfo& oSrc)
		{
			m_dwID			= oSrc.m_dwID;
			m_strFilePath	= oSrc.m_strFilePath;

			m_dStartTime	= oSrc.m_dStartTime;
			m_dEndTime		= oSrc.m_dEndTime;

			m_bLoop			= oSrc.m_bLoop;

			return *this;
		}
	};

	class CExMedia
	{
	public:
		CString m_strPresentationDirectory;
		CString m_strSourceDirectory;

		std::vector<CExFilesInfo> m_arVideos;
		std::vector<CExFilesInfo> m_arImages;
		std::vector<CExFilesInfo> m_arAudios;

		std::vector<CExFilesInfo> m_arAudioCollection;

	public:
		void Clear()
		{
			m_arVideos.clear();
			m_arImages.clear();
			m_arAudios.clear();
			m_arAudioCollection.clear();
		}

	public:
		CExMedia() : m_arVideos(), m_arImages(), m_arAudios()
		{
			m_strPresentationDirectory	= _T("");
			m_strSourceDirectory		= _T("");
		}

		CExMedia(const CExMedia& oSrc)
		{
			*this = oSrc;
		}

		CExMedia& operator=(const CExMedia& oSrc)
		{
			m_strPresentationDirectory	= oSrc.m_strPresentationDirectory;
			m_strSourceDirectory		= oSrc.m_strSourceDirectory;

			for (int i=0; i < oSrc.m_arVideos.size(); i++)
				m_arVideos.push_back(oSrc.m_arVideos[i]);
			for (int i=0; i < oSrc.m_arVideos.size(); i++)
				m_arImages.push_back(oSrc.m_arImages[i]);
			for (int i=0; i < oSrc.m_arVideos.size(); i++)
				m_arAudios.push_back(oSrc.m_arAudios[i]);

			return *this;
		}

		CExFilesInfo* LockVideo(DWORD dwID)
		{
			size_t nCount = m_arVideos.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (dwID == m_arVideos[i].m_dwID)
				{
					return &m_arVideos[i];
				}
			}

			return NULL;
		}
		CExFilesInfo* LockImage(DWORD dwID)
		{
			size_t nCount = m_arImages.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (dwID == m_arImages[i].m_dwID)
				{
					return &m_arImages[i];
				}
			}

			return NULL;
		}
		CExFilesInfo* LockAudio(DWORD dwID)
		{
			size_t nCount = m_arAudios.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (dwID == m_arAudios[i].m_dwID)
				{
					return &m_arAudios[i];
				}
			}

			return NULL;
		}
		CExFilesInfo* LockAudioFromCollection(DWORD dwID)
		{
			size_t nCount = m_arAudioCollection.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (dwID == m_arAudioCollection[i].m_dwID)
				{
					return &m_arAudioCollection[i];
				}
			}

			return NULL;
		}

		CExFilesInfo* Lock(DWORD dwID, CExFilesInfo::ExFilesType& eType)
		{
			CExFilesInfo* pInfo = NULL;

			pInfo = LockVideo(dwID);
			if (NULL != pInfo)
			{
				eType = CExFilesInfo::eftVideo;
				return pInfo;
			}
			pInfo = LockAudio(dwID);
			if (NULL != pInfo)
			{
				eType = CExFilesInfo::eftAudio;
				return pInfo;
			}
			eType = CExFilesInfo::eftNone;
			return LockImage(dwID);
		}
	};

	static void CorrectXmlString(CString& strText)
	{
		strText.Replace(L"&",	L"&amp;");
		strText.Replace(L"'",	L"&apos;");
		strText.Replace(L"<",	L"&lt;");
		strText.Replace(L">",	L"&gt;");
		strText.Replace(L"\"",	L"&quot;");
	}

	static void CorrectXmlString2(CString& strText)
	{
		strText.Replace(L"&apos;",	L"'");
		strText.Replace(L"&lt;",	L"<");
		strText.Replace(L"&gt;",	L">");
		strText.Replace(L"&quot;",	L"\"");
		strText.Replace(L"&amp;",	L"&");
	}
	static inline CString BoolToString(bool bValue)
	{
		if (bValue)
			return _T("1");
		return _T("0");
	}

	static inline CString ToString(int val)
	{
		CString str = _T("");
		str.Format(_T("%d"), val);
		return str;
	}
	static inline CString ToString(DWORD val)
	{
		CString str = _T("");
		str.Format(_T("%d"), val);
		return str;
	}
    static inline CString ToString(long val)
    {
        CString str = _T("");
        str.Format(_T("%d"), val);
        return str;
    }
	static inline CString ToString(double val)
	{
		CString str = _T("");
		str.Format(_T("%lf"), val);
		return str;
	}

	class CColor
	{
	public:
		BYTE R;
		BYTE G;
		BYTE B;
		BYTE A;

		LONG m_lSchemeIndex;

	public:
		CColor()
		{
			R = 0;
			G = 0;
			B = 0;
			A = 255;

			m_lSchemeIndex = -1;
		}
		CColor& operator =(const CColor& oSrc)
		{
			R = oSrc.R;
			G = oSrc.G;
			B = oSrc.B;
			A = oSrc.A;

			m_lSchemeIndex = oSrc.m_lSchemeIndex;
			return (*this);
		}
		AVSINLINE BOOL IsEqual(const CColor& oSrc)
		{
			return ((R == oSrc.R) && (G == oSrc.G) && (B == oSrc.B) && (m_lSchemeIndex == oSrc.m_lSchemeIndex));
		}
		static CColor CreateColor(DWORD dwColor)
		{
			CColor oColor;
			oColor.R = (BYTE)(dwColor >> 16);
			oColor.G = (BYTE)(dwColor >> 8);
			oColor.B = (BYTE)(dwColor);
			oColor.A = 0xFF;

			oColor.m_lSchemeIndex = -1;
			return oColor;
		}

		CColor& operator =(const DWORD& oSrc)
		{
			R = (BYTE)(oSrc >> 8);
			G = (BYTE)(oSrc >> 16);
			B = (BYTE)(oSrc >> 24);
			A = (BYTE)oSrc;

			m_lSchemeIndex = -1;
			return (*this);
		}
		void SetSBGR(const DWORD& lBGR)
		{
			R = (BYTE)(lBGR);
			G = (BYTE)(lBGR >> 8);
			B = (BYTE)(lBGR >> 16);

			if (lBGR & 0xFF000000)
				m_lSchemeIndex = R;
		}
		void SetBGR(const LONG& lBGR)
		{
			R = (BYTE)(lBGR);
			G = (BYTE)(lBGR >> 8);
			B = (BYTE)(lBGR >> 16);

			m_lSchemeIndex = -1;
		}

		friend bool operator==(const CColor& color1, const CColor& color2)
		{
			return ((color1.R == color2.R) && (color1.G == color2.G) && (color1.B == color2.B));
		}
	
		LONG GetLONG() const
		{
			LONG dwColor = 0;
			dwColor |= R;
			dwColor |= (G << 8);
			dwColor |= (B << 16);

			return dwColor;
		}
		LONG GetLONG_RGB() const
		{
			LONG dwColor = 0;
			dwColor |= B;
			dwColor |= (G << 8);
			dwColor |= (R << 16);

			return dwColor;
		}

		CString ToString()
		{
			DWORD dwColor = 0;
			dwColor |= R;
			dwColor |= (G << 8);
			dwColor |= (B << 16);
			return NSPresentationEditor::ToString((int)dwColor);
		}

		void FromString(CString str)
		{
			int lColor;
			if (str.Find(_T("#")) == 0)
			{
				lColor = XmlUtils::GetColor(str.Mid(1, 6));
				R = (BYTE)(lColor);
				G = (BYTE)(lColor >> 8);
				B = (BYTE)(lColor >> 16);
				A = 255;				
			}
			else
			{
				int nLen = str.GetLength();
				TCHAR* pBuffer		= str.GetBuffer();
				TCHAR* pBuffer1		= pBuffer;
				TCHAR* pBuffer2		= pBuffer;
				TCHAR* pBufferEnd	= pBuffer + nLen;
				
				while ((pBuffer1 < pBufferEnd) && !XmlUtils::IsDigit(*pBuffer1))
					++pBuffer1;
				pBuffer2 = pBuffer1;
				while ((pBuffer2 < pBufferEnd) && XmlUtils::IsDigit(*pBuffer2))
					++pBuffer2;
				R = _GetColor(pBuffer1, pBuffer2);

				pBuffer1 = pBuffer2;
				while ((pBuffer1 < pBufferEnd) && !XmlUtils::IsDigit(*pBuffer1))
					++pBuffer1;
				pBuffer2 = pBuffer1;
				while ((pBuffer2 < pBufferEnd) && XmlUtils::IsDigit(*pBuffer2))
					++pBuffer2;
				G = _GetColor(pBuffer1, pBuffer2);

				pBuffer1 = pBuffer2;
				while ((pBuffer1 < pBufferEnd) && !XmlUtils::IsDigit(*pBuffer1))
					++pBuffer1;
				pBuffer2 = pBuffer1;
				while ((pBuffer2 < pBufferEnd) && XmlUtils::IsDigit(*pBuffer2))
					++pBuffer2;
				B = _GetColor(pBuffer1, pBuffer2);

				A = 0xFF;
				m_lSchemeIndex = -1;
			}
		}
		void FromDWORD(DWORD dwVal)
		{

		}

	private:
		BYTE _GetColor(TCHAR* pChar1, TCHAR* pChar2)
		{
			if (pChar1 == pChar2)
				return 0;
			CString s(pChar1, (int)(pChar2 - pChar1));
			return (BYTE)XmlUtils::GetInteger(s);
		}
	};

	class CPen
	{
	public:
		CColor Color;
		long Alpha;
		double Size;

        BYTE DashStyle;
        BYTE LineStartCap;
        BYTE LineEndCap;
        BYTE LineJoin;

		double* DashPattern;
		long Count;

		double DashOffset;
		
		LONG Align;
		double MiterLimit;
		
	public:
	
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
		void SetToRenderer(IRenderer *pRenderer)
		{
			if (-1 == Color.m_lSchemeIndex)
				pRenderer->put_PenColor(Color.GetLONG());
			else
			{
				LONG lColor = Color.GetLONG();
				lColor |= (0xFF000000 & ((Color.m_lSchemeIndex + 1 + 100) << 24));
				pRenderer->put_PenColor(lColor);
			}
			pRenderer->put_PenAlpha(Alpha);
			pRenderer->put_PenSize(Size);
			pRenderer->put_PenDashStyle(DashStyle);
			pRenderer->put_PenLineStartCap(LineStartCap);
			pRenderer->put_PenLineEndCap(LineEndCap);
			pRenderer->put_PenLineJoin(LineJoin);
			pRenderer->put_PenAlign(Align);

			if (DashStyle != Gdiplus::DashStyleSolid)
			{
				SAFEARRAYBOUND rgsab;
				rgsab.lLbound	= 0;
				rgsab.cElements	= Count;

				SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
				memcpy(pArray->pvData, DashPattern, Count * sizeof(double));

				pRenderer->PenDashPattern(DashPattern, Count);

				RELEASEARRAY(pArray);
				pRenderer->put_PenDashOffset(DashOffset);
			}
		}
		void SetDefaultParams()
		{
			Color = 0;
			Alpha = 255;
			Size  = 0.26458;

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

	public:

		inline CString ToString()
        {
			if (0 == Count)
			{
				CString str = _T("");
				str.Format(_T("<pen pen-color='%s' pen-alpha='%d' pen-size='%.6f' pen-style='%d' pen-line-start-cap='%d' pen-line-end-cap='%d' pen-line-join='%d'/>"),
					Color.ToString(), Alpha, Size, DashStyle, LineStartCap, LineEndCap, LineJoin);
				return str;
			}
			CString str = _T("");
			str.Format(_T("<pen pen-color='%s' pen-alpha='%d' pen-size='%.6f' pen-style='%d' pen-line-start-cap='%d' pen-line-end-cap='%d' pen-line-join='%d' pen-dash-pattern-count='%d' dash-offset='%.2lf'>"),
				Color.ToString(), Alpha, Size, DashStyle, LineStartCap, LineEndCap, LineJoin, Count, DashOffset);

			str += _T("<dash-pattern>");

			for (long i = 0; i < Count; ++i)
			{
				CString strMem = _T("");
				strMem.Format(_T("<dash>%.2lf</dash>"), DashPattern[i]);
				str += strMem;
			}

			str += _T("</dash-pattern></pen>");

			return str;
        }
		inline CString ToXmlWriter(NSPresentationEditor::CXmlWriter* pWriter)
		{
			pWriter->WriteNodeBegin(_T("pen"), TRUE);
			pWriter->WriteAttributeString(_T("color"), Color.ToString());
			pWriter->WriteAttributeLONG(_T("alpha"), Alpha);
			pWriter->WriteAttributeDouble(_T("size"), Size);

			pWriter->WriteAttributeDouble(_T("style"), (LONG)DashStyle);
			pWriter->WriteAttributeDouble(_T("line-start-cap"), (LONG)LineStartCap);
			pWriter->WriteAttributeDouble(_T("line-end-cap"), (LONG)LineEndCap);
			pWriter->WriteAttributeDouble(_T("line-join"), (LONG)LineJoin);
			pWriter->WriteNodeEnd(_T("pen"), TRUE);
		}
	};

	class CBrush
	{
	public:

		long Type;
		
		CColor Color1;
		CColor Color2;
		long Alpha1;
		long Alpha2;
		
		CString TexturePath;
		long TextureAlpha;
		long TextureMode;
		
		BOOL	Rectable;
		Gdiplus::RectF	Rect;

		double LinearAngle;

	public:

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

			Color1 = 0xFFFFFFFF;
			Alpha1 = 255;
			Color2 = 0xFFFFFFFF;
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

			return *this;
		}
		virtual ~CBrush()
		{
		}

		BOOL IsTexture()
		{
			return (c_BrushTypeTexture == Type);
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

		void SetToRenderer(IRenderer *pRenderer)
		{
			Type = ConstantCompatible(Type);
			pRenderer->put_BrushType(Type);
			if (IsOneColor())
			{
				//pRenderer->put_BrushColor1(Color1.GetLONG());

				if (-1 == Color1.m_lSchemeIndex)
					pRenderer->put_BrushColor1(Color1.GetLONG());
				else
				{
					LONG lColor = Color1.GetLONG();
					lColor |= (0xFF000000 & ((Color1.m_lSchemeIndex + 1 + 100) << 24));
					pRenderer->put_BrushColor1(lColor);
				}

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
				//pRenderer->put_BrushColor1(Color1.GetLONG());
				
				if (-1 == Color1.m_lSchemeIndex)
					pRenderer->put_BrushColor1(Color1.GetLONG());
				else
				{
					LONG lColor = Color1.GetLONG();
					lColor |= (0xFF000000 & ((Color1.m_lSchemeIndex + 1 + 100) << 24));
					pRenderer->put_BrushColor1(lColor);
				}

				pRenderer->put_BrushAlpha1(Alpha1);
				pRenderer->put_BrushColor2(Color2.GetLONG());
				pRenderer->put_BrushAlpha2(Alpha2);
			}
		}

	public:
		inline CString ToString()
		{
			return _T("<brush brush-type='") + NSPresentationEditor::ToString(Type)
				+ _T("' brush-color1='") + Color1.ToString() + _T("' brush-color2='") + Color2.ToString()
				+ _T("' brush-alpha1='") + NSPresentationEditor::ToString(Alpha1) + _T("' brush-alpha2='") + NSPresentationEditor::ToString(Alpha2) 
				+ _T("' brush-texturepath='") + TexturePath + _T("' brush-texturealpha='") + NSPresentationEditor::ToString(TextureAlpha)  
				+ _T("' brush-texturemode='") + NSPresentationEditor::ToString(TextureMode)  + _T("' />");
		}
		inline CString ToXmlWriter(NSPresentationEditor::CXmlWriter* pWriter)
		{
			pWriter->WriteNodeBegin(_T("brush"), TRUE);
			pWriter->WriteAttributeLONG(_T("brush-type"), Type);
			pWriter->WriteAttributeString(_T("brush-color1"), Color1.ToString());
			pWriter->WriteAttributeString(_T("brush-color2"), Color2.ToString());
			pWriter->WriteAttributeLONG(_T("brush-alpha1"), Alpha1);
			pWriter->WriteAttributeLONG(_T("brush-alpha2"), Alpha2);

			pWriter->WriteAttributeString(_T("brush-texturepath"), TexturePath);

			pWriter->WriteAttributeDouble(_T("brush-texturealpha"), TextureAlpha);
			pWriter->WriteAttributeDouble(_T("brush-texturemode"), TextureMode);

			pWriter->WriteNodeEnd(_T("brush"), TRUE);
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
        BYTE Underline;
        BYTE Strikeout;

		BOOL StringGID;
		double CharSpace;

		CString PitchFamily;
		BYTE	Charset;
		CString Panose;
		BOOL	Monospace;
	
	public:

		BOOL IsEqual(CFont* pFont)
		{
			if (NULL == pFont)
				return FALSE;

			return ((Name == pFont->Name) && (Path == pFont->Path)  && (StringGID == pFont->StringGID) && (Size == pFont->Size) &&
				(Bold == pFont->Bold) && (Italic == pFont->Italic) &&
				(Underline == pFont->Underline) && (Strikeout == pFont->Strikeout));
		}
		BOOL IsEqual2(CFont* pFont)
		{
			if (NULL == pFont)
				return FALSE;

			return ((Name == pFont->Name) && (Path == pFont->Path)  && (StringGID == pFont->StringGID) && (Size == pFont->Size) &&
				(Bold == pFont->Bold) && (Italic == pFont->Italic));
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
            Underline = (BYTE)(0x7C & lStyle) >> 2;
            Strikeout = (BYTE)(0x0180 & lStyle) >> 7;
		}
		void SetToRenderer(IRenderer *pRenderer)
		{		
            pRenderer->put_FontName(Name);
            pRenderer->put_FontPath(Path);


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

			PitchFamily	= _T("");
			Charset		= 0;
			Panose		= _T("");
			Monospace	= FALSE;
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

			PitchFamily	= other.PitchFamily;
			Charset		= other.Charset;
			Panose		= other.Panose;
			Monospace	= other.Monospace;

			return *this;
		}
		virtual ~CFont()
		{
		}

	public:
		
		virtual CString ToString()
		{
			CString strFont = _T("");

			strFont.Format(_T("font-name='%s' font-size='%d' font-bold='%s' font-italic='%s' font-underline='%s' font-strikeout='%s' "),
				Name, (int)Size, NSPresentationEditor::ToString(Bold), NSPresentationEditor::ToString(Italic),
				NSPresentationEditor::ToString(Underline), NSPresentationEditor::ToString(Strikeout));
			
			return strFont;
		}
	};

	class CShadow
	{
	public:
	
		BOOL Visible;
		double DistanceX;
		double DistanceY;
		double BlurSize;
		CColor Color;
		long Alpha;
	
	public:

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

	public:
		virtual CString ToString()
		{
			CString strShadow = _T("");
			strShadow += (_T("shadow-visible=\"") + NSPresentationEditor::ToString(Visible) + _T("\" "));
			strShadow += (_T("shadow-distancex=\"") + NSPresentationEditor::ToString(DistanceX) + _T("\" "));
			strShadow += (_T("shadow-distancey=\"") + NSPresentationEditor::ToString(DistanceY) + _T("\" "));
			strShadow += (_T("shadow-blursize=\"") + NSPresentationEditor::ToString(BlurSize) + _T("\" "));
			strShadow += (_T("shadow-color=\"") + Color.ToString() + _T("\" "));
			strShadow += (_T("shadow-alpha=\"") + NSPresentationEditor::ToString((int)Alpha) + _T("\" "));
			return strShadow;
		}
	};

	class CEdgeText
	{
	public:
	
		long Visible;
		double Dist;
		CColor Color;
		long Alpha;
	
	public:

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

	public:
		virtual CString ToString()
		{
			CString strEdge = _T("");
            strEdge += _T("edge-visible='");
            strEdge += NSPresentationEditor::ToString(Visible);
            strEdge += _T("' ");
            strEdge += _T("edge-dist='");
            strEdge += NSPresentationEditor::ToString(Dist);
            strEdge += _T("' ");
            strEdge += _T("edge-color='");
            strEdge += Color.ToString();
            strEdge += _T("' ");
            strEdge += _T("edge-alpha='");
            strEdge += NSPresentationEditor::ToString((int)Alpha);
            strEdge += _T("' ");
			return strEdge;
		}
	};

	class CTextAttributes
	{
	public:
		CFont		m_oFont;
		CBrush		m_oTextBrush;
		CShadow		m_oTextShadow;
		CEdgeText	m_oTextEdge;

		int			m_nTextAlignHorizontal;
		int			m_nTextAlignVertical;
		double		m_dTextRotate;

	public:
		CTextAttributes() : m_oFont(), m_oTextBrush(), m_oTextShadow(), m_oTextEdge()
		{
			m_oFont.Size = 36;
			m_oTextBrush.Color1 = 0xFF;

			m_nTextAlignHorizontal	= 0;
			m_nTextAlignVertical	= 0;
			m_dTextRotate			= 0;
		}
		CTextAttributes& operator =(const CTextAttributes& oSrc)
		{
			m_oFont			= oSrc.m_oFont;
			m_oTextBrush	= oSrc.m_oTextBrush;
			m_oTextShadow	= oSrc.m_oTextShadow;
			m_oTextEdge		= oSrc.m_oTextEdge;

			m_nTextAlignHorizontal	= oSrc.m_nTextAlignHorizontal;
			m_nTextAlignVertical	= oSrc.m_nTextAlignVertical;
			m_dTextRotate			= oSrc.m_dTextRotate;

			return (*this);
		}
		CString ToString()
		{
			CString strText = _T("<Attributes ");

			// add Brush Param
			strText += (_T("brush-type='") + NSPresentationEditor::ToString(m_oTextBrush.Type) + _T("' "));
			strText += (_T("brush-color1='") + m_oTextBrush.Color1.ToString() + _T("' "));
			strText += (_T("brush-color2='") + m_oTextBrush.Color2.ToString() + _T("' "));
			strText += (_T("brush-alpha1='") + NSPresentationEditor::ToString(m_oTextBrush.Alpha1) + _T("' "));
			strText += (_T("brush-alpha2='") + NSPresentationEditor::ToString(m_oTextBrush.Alpha2) + _T("' "));
			strText += (_T("brush-texturepath='") + m_oTextBrush.TexturePath + _T("' "));
			strText += (_T("brush-texturealpha='") + NSPresentationEditor::ToString(m_oTextBrush.TextureAlpha) + _T("' "));
			strText += (_T("brush-texturemode='") + NSPresentationEditor::ToString(m_oTextBrush.TextureMode) + _T("' "));
			strText += (_T("brush-rectable='0' "));

			// add Font Param
			strText += m_oFont.ToString();

			// add Format Param
			strText += _T("font-antialiastext='1' ");
			strText += (_T("font-stringalignmentvertical='") + NSPresentationEditor::ToString(m_nTextAlignVertical) + _T("' "));
			strText += (_T("font-stringalignmenthorizontal='") + NSPresentationEditor::ToString(m_nTextAlignHorizontal) + _T("' "));
			strText += (_T("font-angle='") + NSPresentationEditor::ToString((int)m_dTextRotate) + _T("' "));

			// add Shadow Param
			if (m_oTextShadow.Visible)
				strText += m_oTextShadow.ToString();

			// add Edge Param
			if (m_oTextEdge.Visible > 0)
				strText += m_oTextEdge.ToString();
			
			strText += _T("/>");

			return strText;
		}

		void ToXmlWriter(NSPresentationEditor::CXmlWriter* pWriter)
		{
			pWriter->WriteNodeBegin(_T("Attributes"), TRUE);

			// add Brush Param
			pWriter->WriteAttributeLONG(_T("brush-type"), m_oTextBrush.Type);
			pWriter->WriteAttributeString(_T("brush-color1"), m_oTextBrush.Color1.ToString());
			pWriter->WriteAttributeString(_T("brush-color2"), m_oTextBrush.Color2.ToString());
			pWriter->WriteAttributeLONG(_T("brush-alpha1"), m_oTextBrush.Alpha1);
			pWriter->WriteAttributeLONG(_T("brush-alpha2"), m_oTextBrush.Alpha2);
			pWriter->WriteAttributeString(_T("brush-texturepath"), m_oTextBrush.TexturePath);
			pWriter->WriteAttributeLONG(_T("brush-texturealpha"), m_oTextBrush.TextureAlpha);
			pWriter->WriteAttributeLONG(_T("brush-texturemode"), m_oTextBrush.TextureMode);
			pWriter->WriteAttributeLONG(_T("brush-rectable"), 0);

			// add Font Param
			pWriter->WriteString(m_oFont.ToString());

			// add Format Param
			pWriter->WriteAttributeLONG(_T("font-antialiastext"), 1);
			pWriter->WriteAttributeLONG(_T("font-stringalignmentvertical"), m_nTextAlignVertical);
			pWriter->WriteAttributeLONG(_T("font-stringalignmenthorizontal"), m_nTextAlignHorizontal);
			pWriter->WriteAttributeLONG(_T("font-angle"), (int)m_dTextRotate);

			// add Shadow Param
			if (m_oTextShadow.Visible)
			{
				pWriter->WriteString(m_oTextShadow.ToString());
			}

			// add Edge Param
			if (m_oTextEdge.Visible > 0)
			{
				pWriter->WriteString(m_oTextEdge.ToString());
			}
			
			pWriter->WriteNodeEnd(_T("Attributes"), TRUE);
		}
	};

	static void ParseString(CString strDelimeters, CString strSource, 
			std::vector<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;

		if (bIsCleared)
			pArrayResults->clear();

		CString resToken;
		int curPos= 0;

		resToken = strSource.Tokenize(strDelimeters, curPos);
		while (resToken != _T(""))
		{
			pArrayResults->push_back(resToken);
			resToken = strSource.Tokenize(strDelimeters, curPos);
		};
	}
}
