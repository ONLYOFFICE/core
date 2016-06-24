/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#include <atlcoll.h>
	#include <gdiplus.h>
#else
    #include "../../PPTXLib/Linux/PPTXFormatLib/linux_gdiplus.h"
#endif

#include "Metric.h"
#include "Effects.h"

#include "../../../Common/ASCUtils.h"
#include "./XmlStringWriter.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../../../DesktopEditor/graphics/structures.h"


namespace NSPresentationEditor
{
	static void ReplaceAll(std::wstring & str, const std::wstring& from, const std::wstring& to) 
	{
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::wstring::npos) 
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); 
		}
	}

	class CExFilesInfo
	{
	public:
		enum ExFilesType
		{
			eftNone		= 0,
			eftVideo	= 1,
			eftAudio	= 2,
			eftHyperlink= 3,
			eftObject	= 4
		};
	public:
		DWORD			m_dwID;		
		std::wstring	m_strFilePath;

		// clip
		double			m_dStartTime;
		double			m_dEndTime;

		// loop
		bool			m_bLoop;

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
		std::wstring			m_strPresentationDirectory;
		std::wstring			m_strSourceDirectory;

		std::vector<CExFilesInfo> m_arVideos;
		std::vector<CExFilesInfo> m_arImages;
		std::vector<CExFilesInfo> m_arAudios;
		std::vector<CExFilesInfo> m_arHyperlinks;

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
		CExFilesInfo* LockHyperlink(DWORD dwID)
		{
			size_t nCount = m_arHyperlinks.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (dwID == m_arHyperlinks[i].m_dwID)
				{
					return &m_arHyperlinks[i];
				}
			}

			return NULL;
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

			pInfo = LockHyperlink(dwID);
			if (NULL != pInfo)
			{
				eType = CExFilesInfo::eftHyperlink;
				return pInfo;
			}
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

#if defined(_WIN32) || defined(_WIN64)
	static void CorrectXmlString2(CString & strText)
	{
		strText.Replace(L"&apos;",	L"'");
		strText.Replace(L"&lt;",	L"<");
		strText.Replace(L"&gt;",	L">");
		strText.Replace(L"&quot;",	L"\"");
		strText.Replace(L"&amp;",	L"&");
	}
	static void CorrectXmlString(CString & strText)
	{
		strText.Replace(L"&",	L"&amp;");
		strText.Replace(L"'",	L"&apos;");
		strText.Replace(L"<",	L"&lt;");
		strText.Replace(L">",	L"&gt;");
		strText.Replace(L"\"",	L"&quot;");
	}
#endif
	static void CorrectXmlString2(std::wstring & strText)
	{
		ReplaceAll(strText, L"&apos;",	L"'");
		ReplaceAll(strText, L"&lt;",	L"<");
		ReplaceAll(strText, L"&gt;",	L">");
		ReplaceAll(strText, L"&quot;",	L"\"");
		ReplaceAll(strText, L"&amp;",	L"&");
	}					
	static void CorrectXmlString(std::wstring & strText)
	{
		ReplaceAll(strText, L"&",	L"&amp;");
		ReplaceAll(strText, L"'",	L"&apos;");
		ReplaceAll(strText, L"<",	L"&lt;");
		ReplaceAll(strText, L">",	L"&gt;");
		ReplaceAll(strText, L"\"",	L"&quot;");
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
		CColor(DWORD rgb)
		{	
			CreateColor(rgb);
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
		AVSINLINE bool IsEqual(const CColor& oSrc)
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
		void SetRGB(BYTE r, BYTE g, BYTE b)
		{
			R = r;
			G = g;
			B = b;

			m_lSchemeIndex = -1;
		}
		void SetR(BYTE r){	R = r;	}
		void SetG(BYTE g){	G = g;	}
		void SetB(BYTE b){	B = b;	}

		BYTE GetR(){ return R;	}
		BYTE GetG(){ return G;	}
		BYTE GetB(){ return B;	}

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
		
		CColor Color2;	//backLine	
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

		bool IsEqual(CPen* pPen)
		{
			if (NULL == pPen)
                                return false;

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


				pRenderer->PenDashPattern(DashPattern, Count);


				pRenderer->put_PenDashOffset(DashOffset);
			}
		}
		void SetDefaultParams()
		{
			Alpha = 255;
			Size  = 0.26458;

			DashStyle    = 0;
			LineStartCap = 0;
			LineEndCap   = 0;
			LineJoin     = 0;

			DashPattern = NULL;
			Count       = 0;

			Color.SetRGB	(0x00, 0x00, 0x00);
			Color2.SetRGB	(0xff, 0xff, 0xff);

			DashOffset	= 0;
            Align		= Gdiplus::PenAlignmentCenter;
			MiterLimit	= 0.5;
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
			Color	= other.Color;
			Color2	= other.Color2;
			Alpha	= other.Alpha;
			Size	= other.Size;

			DashStyle		= other.DashStyle;
			LineStartCap	= other.LineStartCap;
			LineEndCap		= other.LineEndCap;
			LineJoin		= other.LineJoin;

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
		long			Type;
		
		CColor			Color1;
		CColor			Color2;

		long			Alpha1;
		long			Alpha2;
		
		std::wstring	TexturePath;
		long			TextureAlpha;
		long			TextureMode;
		
		bool			Rectable;
		Gdiplus::RectF	Rect;

		double			LinearAngle;

		std::vector<std::pair<CColor, double>> ColorsPosition;

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

		
		bool IsEqual(CBrush* pBrush)
		{
			if (NULL == pBrush)
                                return false;

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
			Type = c_BrushTypeNoFill;

			Color1 = 0xFFFFFFFF;
			Alpha1 = 255;
			Color2 = 0xFFFFFFFF;
			Alpha2 = 255;

			TextureAlpha = 255;
			TextureMode  = c_BrushTextureModeStretch;

			LinearAngle = 0;

			TexturePath = _T("");

			Rectable = false;

			Rect.X      = 0.0F;
			Rect.Y      = 0.0F;
			Rect.Width  = 0.0F;
			Rect.Height = 0.0F;

			ColorsPosition.clear();
		}
		
	public:
		
		CBrush()
		{
			SetDefaultParams();
		}
		CBrush( const CBrush& other )
		{
			Type			= other.Type;
			
			Color1			= other.Color1;
			Alpha1			= other.Alpha1;
			Color2			= other.Color2;
			Alpha2			= other.Alpha2;

			TexturePath		= other.TexturePath;
			TextureAlpha	= other.TextureAlpha;
			TextureMode		= other.TextureMode;

			Rectable		= other.Rectable;
			Rect			= other.Rect;

			LinearAngle		= other.LinearAngle;

			ColorsPosition	= other.ColorsPosition;
		}
		CBrush& operator=(const CBrush& other)
		{
			Type			= other.Type;
			
			Color1			= other.Color1;
			Alpha1			= other.Alpha1;
			Color2			= other.Color2;
			Alpha2			= other.Alpha2;

			TexturePath		= other.TexturePath;
			TextureAlpha	= other.TextureAlpha;
			TextureMode		= other.TextureMode;

			Rectable		= other.Rectable;
			Rect			= other.Rect;

			LinearAngle		= other.LinearAngle;

			ColorsPosition	= other.ColorsPosition;
			return *this;
		}
		virtual ~CBrush()
		{
		}

		bool IsTexture()
		{
			return (c_BrushTypeTexture == Type);
		}
		bool IsOneColor()
		{
			return (c_BrushTypeSolid == Type);
		}
		bool IsTwoColor()
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
                //BSTR bstrTexturePath = TexturePath.AllocSysString();
                pRenderer->put_BrushTexturePath(TexturePath);
                //SysFreeString(bstrTexturePath);
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
	};

	class CFont
	{
	public:
		std::wstring	Path;
		std::wstring	Name;
		double			Size;
		bool			Bold;
		bool			Italic;
        BYTE			Underline;
        BYTE			Strikeout;

		bool			StringGID;
		double			CharSpace;

		std::wstring	PitchFamily;
		BYTE			Charset;
		std::wstring	Panose;
		bool			Monospace;
	
	public:

		bool IsEqual(CFont* pFont)
		{
			if (NULL == pFont)
                                return false;

			return ((Name == pFont->Name) && (Path == pFont->Path)  && (StringGID == pFont->StringGID) && (Size == pFont->Size) &&
				(Bold == pFont->Bold) && (Italic == pFont->Italic) &&
				(Underline == pFont->Underline) && (Strikeout == pFont->Strikeout));
		}
		bool IsEqual2(CFont* pFont)
		{
			if (NULL == pFont)
                                return false;

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
			Bold		= (0x01 == (0x01 & lStyle));
			Italic		= (0x02 == (0x02 & lStyle));
            Underline	= (BYTE)(0x7C & lStyle) >> 2;
            Strikeout	= (BYTE)(0x0180 & lStyle) >> 7;
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
			Bold      = false;
			Italic    = false;
			Underline = 0;
			Strikeout = 0;

			StringGID = false;
			CharSpace = 0.0;

			PitchFamily	= _T("");
			Charset		= 0;
			Panose		= _T("");
			Monospace	= false;
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
	};

	class CShadow
	{
	public:
	
		bool			Visible;

		double			DistanceX;
		double			DistanceY;
		
		double			OriginX;
		double			OriginY;

		double			BlurSize;
		CColor			Color;
		long			Alpha;

		int				Type;

		double			ScaleXToX;
		double			ScaleXToY;
		double			ScaleYToX;
		double			ScaleYToY;

		int				PerspectiveX;
		int				PerspectiveY;
	
	public:

		void SetDefaultParams()
		{
			Visible		= false;
			DistanceX	= 0.5;
			DistanceY	= 0.5;
			BlurSize	= 0;
			Alpha		= 255;

			OriginX		= 0;
			OriginY		= 0;

			Type		= -1;

			ScaleXToX	 = 1.;
			ScaleXToY	 = 1.;
			ScaleYToX	 = 1.;
			ScaleYToY	 = 1.;

			PerspectiveX = 0;
			PerspectiveY = 0;
			
			Color.SetRGB(0x80, 0x80, 0x80);
		}
		
	public:
	
		CShadow()
		{
			SetDefaultParams();
		}
		CShadow( const CShadow& other )
		{
			Visible		= other.Visible;
			DistanceX	= other.DistanceX;
			DistanceY	= other.DistanceY;
			OriginX		= other.OriginX;
			OriginY		= other.OriginY;
			BlurSize	= other.BlurSize;
			Color		= other.Color;
			Alpha		= other.Alpha;

			Type		= other.Type;

			ScaleXToX	= other.ScaleXToX;
			ScaleXToY	= other.ScaleXToY;
			ScaleYToX	= other.ScaleYToX;
			ScaleYToY	= other.ScaleYToY;

			PerspectiveX= other.PerspectiveX;
			PerspectiveY= other.PerspectiveY;
		}
		CShadow& operator=(const CShadow& other)
		{
			Visible		= other.Visible;
			DistanceX	= other.DistanceX;
			DistanceY	= other.DistanceY;
			OriginX		= other.OriginX;
			OriginY		= other.OriginY;
			BlurSize	= other.BlurSize;
			Color		= other.Color;
			Alpha		= other.Alpha;
			Type		= other.Type;
		
			PerspectiveX= other.PerspectiveX;
			PerspectiveY= other.PerspectiveY;
			
			ScaleXToX	= other.ScaleXToX;
			ScaleXToY	= other.ScaleXToY;
			ScaleYToX	= other.ScaleYToX;
			ScaleYToY	= other.ScaleYToY;	
			
			return *this;
		}
		virtual ~CShadow()
		{
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
			m_nTextAlignVertical	= -1; //not set
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
