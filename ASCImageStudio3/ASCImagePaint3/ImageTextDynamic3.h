#pragma once
		
#include "resource.h"
#include <registration.h>
#include <PaintStruct.h>
#include <ImageColors.h>
#include <ImageCore.h>
#include "Effects.h"
	
[object, uuid("543D4C9D-4EE1-4f88-93BC-8445B788ECCB"), dual, helpstring("IImageTextDynamic3 Interface"), pointer_default(unique)]
__interface IImageTextDynamic3 : IDispatch
{
	// effects
	[id(10)] HRESULT GetCategoriesCount([out, retval] long* Categories);
	[id(11)] HRESULT GetCategoryName([in] long Category, [out, retval] BSTR* Name);
	[id(12)] HRESULT GetEffectsCount([in] long Category, [out, retval] long* Count);
	[id(13)] HRESULT GetEffectName([in] long Category, [in] long Effect, [out, retval] BSTR* Name);
	[id(14)] HRESULT GetEffectAt([in] long Category, [in] long Effect, [out, retval] long* ID);
	[id(15)] HRESULT GetCategotyIndex([in] long Effect, [out, retval] long* Index);
	
	// painting
	[id(20)] HRESULT Start([in] BSTR String, [in] long Category, [in] long Effect, [in] long EffectID, [in] long CoorX, [in] long CoorY, [in] long Step);
	[id(21)] HRESULT Stop();
	[id(22)] HRESULT Render([in, out, satype("BYTE")] SAFEARRAY** pSafeArray, [in] double Frame);
	[id(23)] HRESULT RenderMedia([in] IUnknown* pInterfaceInput, [out] IUnknown** pInterfaceOutput, [in] double Frame);

	// font
	[propget, id(301)] HRESULT fontName([out, retval] BSTR* pVal);
	[propput, id(301)] HRESULT fontName([in] BSTR newVal);
	[propget, id(302)] HRESULT fontSize([out, retval] long* pVal);
	[propput, id(302)] HRESULT fontSize([in] long newVal);
	[propget, id(303)] HRESULT fontBold([out, retval] VARIANT_BOOL* pVal);
	[propput, id(303)] HRESULT fontBold([in] VARIANT_BOOL newVal);
	[propget, id(304)] HRESULT fontItalic([out, retval] VARIANT_BOOL* pVal);
	[propput, id(304)] HRESULT fontItalic([in] VARIANT_BOOL newVal);
	[propget, id(305)] HRESULT fontUnderline([out, retval] VARIANT_BOOL* pVal);
	[propput, id(305)] HRESULT fontUnderline([in] VARIANT_BOOL newVal);
	[propget, id(306)] HRESULT fontStrikeout([out, retval] VARIANT_BOOL* pVal);
	[propput, id(306)] HRESULT fontStrikeout([in] VARIANT_BOOL newVal);

	// formatting
	[propget, id(351)] HRESULT formatRotate([out, retval] long* pVal);
	[propput, id(351)] HRESULT formatRotate([in] long newVal);

	// brush
	[propget, id(403)] HRESULT brushType([out, retval] long* pVal);
	[propput, id(403)] HRESULT brushType([in] long newVal);
	[propget, id(404)] HRESULT brushColor1([out, retval] long* pVal);
	[propput, id(404)] HRESULT brushColor1([in] long newVal);
	[propget, id(405)] HRESULT brushColor2([out, retval] long* pVal);
	[propput, id(405)] HRESULT brushColor2([in] long newVal);
	[propget, id(406)] HRESULT brushAlpha1([out, retval] BYTE* pVal);
	[propput, id(406)] HRESULT brushAlpha1([in] BYTE newVal);
	[propget, id(407)] HRESULT brushAlpha2([out, retval] BYTE* pVal);
	[propput, id(407)] HRESULT brushAlpha2([in] BYTE newVal);
	[propget, id(412)] HRESULT brushTextureLoad([out, retval] BSTR* pVal);
	[propput, id(412)] HRESULT brushTextureLoad([in] BSTR newVal);
	[propget, id(413)] HRESULT brushTextureAlpha([out, retval] BYTE* pVal);
	[propput, id(413)] HRESULT brushTextureAlpha([in] BYTE newVal);
	[propget, id(414)] HRESULT brushTextureMode([out, retval] long* val);
	[propput, id(414)] HRESULT brushTextureMode([in] long val);

	// edge
	[propget, id(501)] HRESULT edgeVisible([out, retval] long* pVal);
	[propput, id(501)] HRESULT edgeVisible([in] long newVal);
	[propget, id(502)] HRESULT edgeDistance([out, retval] long* pVal);
	[propput, id(502)] HRESULT edgeDistance([in] long newVal);
	[propget, id(503)] HRESULT edgeColor([out, retval] long* pVal);
	[propput, id(503)] HRESULT edgeColor([in] long newVal);
	[propget, id(504)] HRESULT edgeAlpha([out, retval] BYTE* pVal);
	[propput, id(504)] HRESULT edgeAlpha([in] BYTE newVal);
	
	// shadow
	[propget, id(601)] HRESULT shadowVisible([out, retval] VARIANT_BOOL* pVal);
	[propput, id(601)] HRESULT shadowVisible([in] VARIANT_BOOL newVal);
	[propget, id(602)] HRESULT shadowDistanceX([out, retval] long* pVal);
	[propput, id(602)] HRESULT shadowDistanceX([in] long newVal);
	[propget, id(603)] HRESULT shadowDistanceY([out, retval] long* pVal);
	[propput, id(603)] HRESULT shadowDistanceY([in] long newVal);
	[propget, id(604)] HRESULT shadowColor([out, retval] long* pVal);
	[propput, id(604)] HRESULT shadowColor([in] long newVal);
	[propget, id(605)] HRESULT shadowAlpha([out, retval] BYTE* pVal);
	[propput, id(605)] HRESULT shadowAlpha([in] BYTE newVal);
	[propget, id(606)] HRESULT shadowBlur([out, retval] long* pVal);
	[propput, id(606)] HRESULT shadowBlur([in] long newVal);

	[id(701)] HRESULT SetFormat([in, satype("BYTE")] SAFEARRAY** Array);
	[id(702)] HRESULT GetFormat([out, retval, satype("BYTE")] SAFEARRAY** Array);
};
	
[coclass, uuid("E832BC32-2A62-464e-A833-4DE451D29274"), threading(apartment), vi_progid("AVSImagePaint3.ImageTextDynamic3"), progid("AVSImagePaint3.ImageTextDynamic3.1"), version(1.0), helpstring("ImageTextDynamic3 Class")]
class ATL_NO_VTABLE ImageTextDynamic3 : public IImageTextDynamic3
{
// Interface
public:
		
	// effects
	STDMETHOD(GetCategoriesCount)(long* Categories)
	{
		*Categories = m_categories.GetSize();
		
		return S_OK;
	}
	STDMETHOD(GetCategoryName)(long Category, BSTR* Name)
	{ 
		CString _name = "";

		if (Category >= 0 && Category < m_categories.GetSize()) 
			_name = m_categories[Category].Name;

		*Name = _name.AllocSysString();

		return S_OK; 
	}
	STDMETHOD(GetEffectsCount)(long Category, long* Count)
	{ 
		if (Category < 0)
			*Count = m_effects.GetSize();
		else if (Category >=0 && Category < m_categories.GetSize())
			*Count = m_categories[Category].Effects.GetSize();
		else
			*Count = 0;

		return S_OK; 
	}
	STDMETHOD(GetEffectName)(long Category, long Effect, BSTR* Name)
	{ 
		CString _name = "";

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_name = m_effects[Effect].Name2;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_name = m_effects[m_categories[Category].Effects[Effect]].Name2;
		}

		*Name = _name.AllocSysString();

		return S_OK; 
	}
	STDMETHOD(GetEffectAt)(long Category, long Effect, long* ID)
	{ 
		int _id = -1;

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_id = m_effects[Effect].ID;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_id = m_effects[m_categories[Category].Effects[Effect]].ID;
		}

		*ID = _id;

		return S_OK; 
	}
	STDMETHOD(GetCategotyIndex)(long Effect, long* Index)
	{
		long nEffectsInCategory = 0;
		long nEffectsFound = 0;
		long nEffectsTotal = 0;

		for(int i = 0; i < m_categories.GetSize(); i++)			
			nEffectsTotal += m_categories[i].Effects.GetSize();

		if (Effect >= 0 && Effect < nEffectsTotal)
		{
			for(int i = 0; i < m_categories.GetSize(); i++)
			{
				GetEffectsCount(i, &nEffectsInCategory);

				nEffectsFound += nEffectsInCategory;

				if (Effect < nEffectsFound)
				{
					*Index = i;
					break;
				}
			}
		}
		else
			*Index = -1;

		return S_OK; 
	}
	
	// painting
	STDMETHOD(Start)(BSTR String, long Category, long Effect, long EffectID, long CoorX, long CoorY, long Step)
	{
		EffectStart(String, Category, Effect, EffectID, CoorX, CoorY, Step);
		
		return S_OK;
	}
	STDMETHOD(Stop)()
	{
		EffectStop();
		
		return S_OK;
	}
	STDMETHOD(Render)(SAFEARRAY** pSafeArray, double Frame)
	{
		if (!pSafeArray || !*pSafeArray)
			return S_FALSE;

		if (!CRegistratorClient::IsRegistered())
			return S_FALSE;

		int nChannels = 0;

		m_pixels = ImageStudioUtils::ExtractImage(pSafeArray, m_width, m_height, nChannels);

		if (!m_pixels || nChannels != 4)
			return S_FALSE;

		EffectRenderEnd();
		EffectRender(Frame);
		EffectRenderEnd();

		return S_OK;
	}
	STDMETHOD(RenderMedia)(IUnknown* pInterfaceInput, IUnknown** pInterfaceOutput, double Frame)
	{
		if (!pInterfaceInput || !pInterfaceOutput)
			return S_FALSE;

		*pInterfaceOutput = NULL;

		if (!CRegistratorClient::IsRegistered())
			return S_FALSE;

		if (!m_oMediaData.Create(pInterfaceInput, FALSE) || !m_oMediaData.IsValidBGRA())
			return S_FALSE;

		m_pixels = m_oMediaData.GetBuffer();
		m_width = m_oMediaData.GetWidth();
		m_height = m_oMediaData.GetHeight();
		
		EffectRenderEnd();
		EffectRender(Frame);
		EffectRenderEnd();

		// save pointer to new interface (if it was transformed to BGRA32 format)
		*pInterfaceOutput = m_oMediaData.GetMediaData(TRUE);

		return S_OK;
	}
	
	// font property
	STDMETHOD(get_fontName)(BSTR* pVal)
	{
		*pVal = m_font.Name.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_fontName)(BSTR newVal)
	{
		m_font.Name = CString(newVal);
		m_font.Update();
		return S_OK;
	} 	
	STDMETHOD(get_fontSize)(long* pVal)
	{
		*pVal = m_font.Size;
		return S_OK;
	}
	STDMETHOD(put_fontSize)(long newVal)
	{
		m_font.Size = newVal;
		m_font.Update();

		letter_rect.X = letter_rect.Y = -newVal*0.8;
		letter_rect.Width = letter_rect.Height = newVal*1.6;

		return S_OK;
	}

	STDMETHOD(get_fontBold)(VARIANT_BOOL* pVal)
	{
		*pVal = m_font.Bold ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontBold)(VARIANT_BOOL newVal)
	{
		m_font.Bold = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}

	STDMETHOD(get_fontItalic)(VARIANT_BOOL* pVal)
	{
		*pVal = m_font.Italic ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontItalic)(VARIANT_BOOL newVal) 
	{
		m_font.Italic = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}

 	STDMETHOD(get_fontUnderline)(VARIANT_BOOL* pVal) 
	{
		*pVal = m_font.Underline ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontUnderline)(VARIANT_BOOL newVal) 
	{
		m_font.Underline = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}

	STDMETHOD(get_fontStrikeout)(VARIANT_BOOL* pVal) 
	{
		*pVal = m_font.Strikeout ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontStrikeout)(VARIANT_BOOL newVal) 
	{
		m_font.Strikeout = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}
	
	// formatting
	STDMETHOD(get_formatRotate)(long* pVal) 
	{
		*pVal = m_nAngle;
		return S_OK;
	}
	STDMETHOD(put_formatRotate)(long newVal) 
	{
		m_nAngle = newVal;
		return S_OK;
	}
	
	// brush property
	STDMETHOD(get_brushType)(long* pVal) 
	{
		*pVal = m_brush.Type;
		return S_OK;
	}
	STDMETHOD(put_brushType)(long newVal) 
	{
		m_brush.Type = Painter::ConstantCompatible(newVal);
		m_brush.Update();
		return S_OK;
	}
	
	STDMETHOD(get_brushColor1)(long* pVal)		
	{ 
		*pVal = m_brush.Color1; 
		return S_OK; 
	}
	STDMETHOD(put_brushColor1)(long newVal)		
	{ 
		m_brush.Color1 = newVal;
		m_brush.Update();
		return S_OK; 
	}

	STDMETHOD(get_brushColor2)(long* pVal)		
	{ 
		*pVal = m_brush.Color2; 
		return S_OK; 
	}
	STDMETHOD(put_brushColor2)(long newVal)		
	{ 
		m_brush.Color2 = newVal;
		m_brush.Update();
		return S_OK; 
	}
	
	STDMETHOD(get_brushAlpha1)(BYTE* pVal)		
	{ 
		*pVal = m_brush.Alpha1; 
		return S_OK; 
	}
	STDMETHOD(put_brushAlpha1)(BYTE newVal)		
	{ 
		m_brush.Alpha1 = newVal;
		m_brush.Update();
		return S_OK; 
	}

	STDMETHOD(get_brushAlpha2)(BYTE* pVal)		
	{ 
		*pVal = m_brush.Alpha2; 
		return S_OK; 
	}
	STDMETHOD(put_brushAlpha2)(BYTE newVal)		
	{ 
		m_brush.Alpha2 = newVal;
		m_brush.Update();
		return S_OK; 
	}

	STDMETHOD(get_brushTextureLoad)(BSTR* pVal)
	{
		*pVal = m_brush.TexturePath.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_brushTextureLoad)(BSTR newVal)
	{
		CString path(newVal);
		path.MakeLower();
		int index = m_textures.Add(path);
		if (index < 0)
			return S_OK;

		m_brush.TexturePath = path;
		m_brush.Update();
		return S_OK;
	}

	STDMETHOD(get_brushTextureAlpha)(BYTE* pVal)
	{
		*pVal = m_brush.TextureAlpha;
		return S_OK;
	}
	STDMETHOD(put_brushTextureAlpha)(BYTE newVal)
	{
		m_brush.TextureAlpha = newVal;
		m_brush.Update();
		return S_OK;
	}

	STDMETHOD(get_brushTextureMode)(long* pVal)
	{
		*pVal = m_brush.TextureMode;
		return S_OK;
	}
	STDMETHOD(put_brushTextureMode)(long newVal)
	{
		m_brush.TextureMode = newVal;
		m_brush.Update();
		return S_OK;
	}
	
    // edge property
	STDMETHOD(get_edgeVisible)(long* pVal) 
	{
		*pVal = m_edge.Visible;
		return S_OK; 
	}
	STDMETHOD(put_edgeVisible)(long newVal)
	{
		m_edge.Visible = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_edgeDistance)(long* pVal)
	{
		*pVal = m_edge.Dist; 
		return S_OK; 
	}
	STDMETHOD(put_edgeDistance)(long newVal)
	{
		m_edge.Dist = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_edgeColor)(long* pVal)
	{
		*pVal = m_edge.Color; 
		return S_OK; 
	}
	STDMETHOD(put_edgeColor)(long newVal)
	{
		m_edge.Color = newVal;
		m_edge.Update();
		return S_OK; 
	}
	STDMETHOD(get_edgeAlpha)(BYTE* pVal)
	{
		*pVal = m_edge.Alpha;
		return S_OK;
	}
	STDMETHOD(put_edgeAlpha)(BYTE newVal)
	{
		m_edge.Alpha = newVal;
		m_edge.Update();
		return S_OK;
	}
	
	// shadow property
	STDMETHOD(get_shadowVisible)(VARIANT_BOOL* pVal)
	{
		*pVal = m_shadow.Visible ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK; 
	}
	STDMETHOD(put_shadowVisible)(VARIANT_BOOL newVal)
	{
		m_shadow.Visible = (newVal == VARIANT_TRUE); 
		return S_OK; 
	}
	STDMETHOD(get_shadowDistanceX)(long* pVal) 
	{
		*pVal = m_shadow.DistanceX; 
		return S_OK; 
	}
	STDMETHOD(put_shadowDistanceX)(long newVal) 
	{
		m_shadow.DistanceX = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_shadowDistanceY)(long* pVal) 
	{
		*pVal = m_shadow.DistanceY; 
		return S_OK; 
	}
	STDMETHOD(put_shadowDistanceY)(long newVal) 
	{					 
		m_shadow.DistanceY = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_shadowColor)(long* pVal)
	{
		*pVal = m_shadow.Color; 
		return S_OK; 
	}
	STDMETHOD(put_shadowColor)(long newVal) 
	{
		m_shadow.Color = newVal;
		m_shadow.Update();
		return S_OK; 
	}
	STDMETHOD(get_shadowAlpha)(BYTE* pVal) 
	{
		*pVal = m_shadow.Alpha;
		return S_OK;
	}
	STDMETHOD(put_shadowAlpha)(BYTE newVal) 
	{
		m_shadow.Alpha = newVal;
		m_shadow.Update();
		return S_OK;
	}
	STDMETHOD(get_shadowBlur)(long* pVal) 
	{
		*pVal = m_shadow.BlurSize; 
		return S_OK; 
	}
	STDMETHOD(put_shadowBlur)(long newVal) 
	{
		m_shadow.BlurSize = newVal; 
		return S_OK; 
	}
	STDMETHOD(SetFormat)(SAFEARRAY** Array)
	{
		if (!Array || !*Array)
			return S_OK;

		BYTE* buffer = (BYTE*)((*Array)->pvData);

		if (!buffer)
			return S_OK;

		Painter::CStream stream;

		stream.SetBuffer(buffer);

		m_brush.Read(stream);
		m_font.Read(stream);
		m_shadow.Read(stream);
		m_edge.Read(stream);

		return S_OK;
	}
	STDMETHOD(GetFormat)(SAFEARRAY** Array)
	{
		long size = m_brush.GetSize() + m_font.GetSize() + m_shadow.GetSize() + m_edge.GetSize();

		SAFEARRAYBOUND bound;

		bound.lLbound = 0;
		bound.cElements = size;

		*Array = SafeArrayCreate(VT_UI1, 1, &bound);

		if (!*Array)
			return S_OK;

		BYTE* buffer = (BYTE*)((*Array)->pvData);

		if (!buffer)
			return S_OK;

		Painter::CStream stream;

		stream.SetBuffer(buffer);

		m_brush.Write(stream);
		m_font.Write(stream);
		m_shadow.Write(stream);
		m_edge.Write(stream);

		return S_OK;
	}

		
// Members
protected:
	
	// painting formats
	Painter::CBrush m_brush;
	Painter::CFont m_font;
	Painter::CShadow m_shadow;
	Painter::CEdgeText m_edge;
	Painter::CTextureManager m_textures;

	// painting
	int m_width;
	int m_height;
	RectF letter_rect;
	BYTE* m_pixels;
	Bitmap* m_bitmap;
	CString m_string;
	int m_effect;
	long m_nAngle;
	long m_nStartX;
	long m_nStartY;
	
	ImageStudio::Core::MediaData m_oMediaData;

	// effects
	CSimpleArray<Category> m_categories;
	CSimpleArray<Effect> m_effects;
    CSimpleArray<float> m_finish;

	// painting
	CSimpleArray<Letter> m_letters;
	
	// gdi+
	CGdiPlusInit initGdiPlus;
	
// Methods
protected:
	
	// construction
	ImageTextDynamic3()
	{
		initGdiPlus.Init();
		m_width = 0;
		m_height =  0;				 
		m_pixels = 0;
		m_bitmap = 0;
		m_effect = -1;
		letter_rect.X = -12.8f;
		letter_rect.Y = -12.8f;
		letter_rect.Width = 25.6f;
		letter_rect.Height = 25.6f;
		
		m_nAngle = 0;
		m_nStartX = 0; 
		m_nStartY = 0;

		// create effects list
		AddCategory("Appear Displace");
			AddEffect("Displace Random Movements", "Random\nMovements", 1);
			AddEffect("Displace Fade","Displace\nFade", 2);
			AddEffect("Displace Shade","Displace\nShade", 3);
			AddEffect("Displace Rotate","Displace\nRotate", 4);
			AddEffect("Displace Random","Displace\nRandom", 5);
			AddEffect("Displace Airport","Displace\nAirport", 6);
			AddEffect("Displace Shake","Displace\nShake", 7);
  			AddEffect("Displace Wedge","Displace\nWedge", 8);
  			AddEffect("Displace Stream","Displace\nStream", 9);
		AddCategory("Appear Waves");
			AddEffect("Wave Simple","Wave\nSimple", 101);
			AddEffect("Wave Zoom","Wave\nZoom", 102);
			AddEffect("Wave Fall","Wave\nFall", 103);
			AddEffect("Wave Run Shift","Wave\nRun Shift", 104);
			AddEffect("Wave Swing","Wave\nSwing", 105);
			AddEffect("Wave Run","Wave\nRun", 106);
			AddEffect("Wave Zoom Vertical","Wave\nZoom Vertical", 107);
			AddEffect("Wave Shift","Wave\nShift", 108);
			AddEffect("Wave Pulse","Wave\nPulse", 109);
   		AddCategory("Appear Alternate");
			AddEffect("Alternate Ejection","Alternate\nEjection", 402);
			AddEffect("Alternate Ellipse","Alternate\nEllipse", 403);
			AddEffect("Alternate Wheel","Alternate\nWheel", 404);
			AddEffect("Alternate Miscarry","Alternate\nMiscarry", 405);
			AddEffect("Alternate Rotation","Alternate\nRotation", 406);
			AddEffect("Alternate Increase","Alternate\nIncrease", 407);
			AddEffect("Alternate TwoString","Alternate\nTwoString", 408);
			AddEffect("Alternate Tumbling","Alternate\nTumbling", 409);
			AddEffect("Alternate Leave","Alternate\nLeave", 410);
			AddEffect("Alternate Unfold","Alternate\nUnfold", 411);
			AddEffect("Alternate Centrifuge","Alternate\nCentrifuge", 412);
			AddEffect("Alternate HorizontalRotation","Alternate Horizontal\nRotation", 413);
			AddEffect("Alternate HorizontalRotation3D","Alternate Horizontal\nRotation3D", 414);
			AddEffect("Alternate VerticalRotation","Alternate Vertical\nRotation", 415);
			AddEffect("Alternate CheChe","Alternate\nCheChe", 416);		
		AddCategory("Appear Transform");
			AddEffect("Transform Boomerang","Transform\nBoomerang", 601);
			AddEffect("Transform Flash","Transform\nFlash", 602);
			AddEffect("Transform Flight","Transform\nFlight", 603);
			AddEffect("Transform Jump","Transform\nJump", 604);
			AddEffect("Transform Compress","Transform\nCompress", 605);
			AddEffect("Transform Motion","Transform\nMotion", 606);
			AddEffect("Transform Manifestation","Transform\nManifestation", 607);
			AddEffect("Transform Shot","Transform\nShot", 608);
			AddEffect("Transform Reduction","Transform\nReduction", 609);
			AddEffect("Transform VHRotation","Transform\nVHRotation", 610);
			AddEffect("Transform Rotate3D","Transform\nRotate3D", 611);
			AddEffect("Transform Spiral","Transform\nSpiral", 612);
			AddEffect("Transform Flag","Transform\nFlag", 613);
			AddEffect("Transform Perpendicular","Transform\nPerpendicular", 614);
			AddEffect("Transform Clock","Transform\nClock", 615);
		AddCategory("Appear Animation");
			AddEffect("Animation Galaxy","Animation\nGalaxy", 801);
			AddEffect("Animation WaveRotate3D","Animation Wave\nRotate3D", 802);
			AddEffect("Animation RandomJump","Animation\nRandom Jump", 803);
			AddEffect("Animation Galaxy2","Animation\nGalaxy2", 804);
			AddEffect("Animation Roll","Animation\nRoll", 805);
			AddEffect("Animation Conga","Animation\nConga", 806);
			AddEffect("Animation Matrix","Animation\nMatrix", 807);
			AddEffect("Animation HotPinkSpin","Animation Hot\nPink Spin", 808);
			AddEffect("Animation HopInJumpOut","Animation Hop\nIn Jump Out", 809);
			AddEffect("Animation ConJoin","Animation\nConJoin", 810);
			AddEffect("Animation HopsCotch","Animation\nHopsCotch", 811);
			AddEffect("Animation Twist","Animation\nTwist", 812);
			AddEffect("Animation StarWars","Animation\nStarWars", 813);
			AddEffect("Animation Squeeze","Animation\nSqueeze", 814);
			AddEffect("Animation Terminator","Animation\nTerminator", 815);
			
		AddCategory("Erase Displace");
			AddEffect("Displace Random Movements", "Displace Random\nMovements", 201);
			AddEffect("Displace Fade","Displace\nFade", 202);
			AddEffect("Displace Shade","Displace\nShade", 203);
			AddEffect("Displace Rotate","Displace\nRotate", 204);
			AddEffect("Displace Random","Displace\nRandom", 205);
			AddEffect("Displace Airport","Displace\nAirport", 206);
			AddEffect("Displace Shake","Displace\nShake", 207);
  			AddEffect("Displace Wedge","Displace\nWedge", 208);
  			AddEffect("Displace Stream","Displace\nStream", 209);
		AddCategory("Erase Waves");
			AddEffect("Wave Simple","Wave\nSimple", 301);
			AddEffect("Wave Zoom","Wave\nZoom", 302);
			AddEffect("Wave Fall","Wave\nFall", 303);
			AddEffect("Wave Run Shift","Wave\nRun Shift", 304);
			AddEffect("Wave Swing","Wave\nSwing", 305);
			AddEffect("Wave Run","Wave\nRun", 306);
			AddEffect("Wave Zoom Vertical","Wave\nZoom Vertical", 307);
			AddEffect("Wave Shift","Wave\nShift", 308);
			AddEffect("Wave Pulse","Wave\nPulse", 309);
   		AddCategory("Erase Alternate");
			AddEffect("Alternate Ejection","Alternate\nEjection", 502);
			AddEffect("Alternate Ellipse","Alternate\nEllipse", 503);
			AddEffect("Alternate Wheel","Alternate\nWheel", 504);
			AddEffect("Alternate Miscarry","Alternate\nMiscarry", 505);
			AddEffect("Alternate Rotation","Alternate\nRotation", 506);
			AddEffect("Alternate Increase","Alternate\nIncrease", 507);
			AddEffect("Alternate TwoString","Alternate\nTwoString", 508);
			AddEffect("Alternate Tumbling","Alternate\nTumbling", 509);
			AddEffect("Alternate Leave","Alternate\nLeave", 510);
			AddEffect("Alternate Unfold","Alternate\nUnfold", 511);
			AddEffect("Alternate Centrifuge","Alternate\nCentrifuge", 512);
			AddEffect("Alternate HorizontalRotation","Alternate Horizontal\nRotation", 513);
			AddEffect("Alternate HorizontalRotation3D","Alternate Horizontal\nRotation3D", 514);
			AddEffect("Alternate VerticalRotation","Alternate Vertical\nRotation", 515);
			AddEffect("Alternate CheChe","Alternate\nCheChe", 516);
   		AddCategory("Erase Transform");
			AddEffect("Transform Boomerang","Transform\nBoomerang", 701);
			AddEffect("Transform Flash","Transform\nFlash", 702);
			AddEffect("Transform Flight","Transform\nFlight", 703);
			AddEffect("Transform Jump","Transform\nJump", 704);
			AddEffect("Transform Compress","Transform\nCompress", 705);
			AddEffect("Transform Motion","Transform\nMotion", 706);
			AddEffect("Transform Manifestation","Transform\nManifestation", 707);
			AddEffect("Transform Shot","Transform\nShot", 708);
			AddEffect("Transform Reduction","Transform\nReduction", 709);
			AddEffect("Transform VHRotation","Transform\nVHRotation", 710);
			AddEffect("Transform Rotate3D","Transform\nRotate3D", 711);
			AddEffect("Transform Spiral","Transform\nSpiral", 712);
			AddEffect("Transform Flag","Transform\nFlag", 713);
			AddEffect("Transform Perpendicular","Transform\nPerpendicular", 714);
			AddEffect("Transform Clock","Transform\nClock", 715);
		AddCategory("Erase Animation");
			AddEffect("Animation Galaxy","Galaxy", 901);
			AddEffect("Animation WaveRotate3D","Animation Wave\nRotate3D", 902);
			AddEffect("Animation RandomJump","Animation\nRandom Jump", 903);
			AddEffect("Animation Galaxy2","Animation\nGalaxy2", 904);
			AddEffect("Animation Roll","Animation\nRoll", 905);
			AddEffect("Animation Conga","Animation\nConga", 906);
			AddEffect("Animation Matrix","Animation\nMatrix", 907);
			AddEffect("Animation HotPinkSpin","Animation Hot\nPink Spin", 908);
			AddEffect("Animation HopInJumpOut","Animation Hop\nIn Jump Out", 909);
			AddEffect("Animation ConJoin","Animation\nConJoin", 910);
			AddEffect("Animation HopsCotch","Animation\nHopsCotch", 911);
			AddEffect("Animation Twist","Animation\nTwist", 912);
			AddEffect("Animation StarWars","Animation\nStarWars", 913);
			AddEffect("Animation Squeeze","Animation\nSqueeze", 914);
			AddEffect("Animation Terminator","Animation\nTerminator", 915);
	}
	~ImageTextDynamic3()
	{
		Clear();
	}
	void Clear()
	{
		m_brush.Clear();
		m_font.Clear();
		m_shadow.Clear();
		m_edge.Clear();
		m_textures.Clear();
	}
	
	// helpers
	Brush* GetBrush(Painter::CTextureManager* pManager, RectF* pRect, BYTE Alpha)
	{	
		Brush* pBrush = NULL;

		// create texture brush
		if (Painter::c_BrushTypeTexture == m_brush.Type && pManager)
		{
			int index = pManager->Find(m_brush.TexturePath);
			if (index >= 0)
			{
				Gdiplus::TextureBrush* brush = GetTexureBrush(index, pManager, pRect, (BYTE)(m_brush.TextureAlpha*Alpha/255), m_brush.TextureMode);

				pBrush = brush;

				return pBrush;
			}
		}
		// create hatch brush
		else if (m_brush.Type >= Painter::c_BrushTypeHatch1 && m_brush.Type <= Painter::c_BrushTypeHatch53)
		{
			Gdiplus::Color color1((BYTE)(m_brush.Alpha1*Alpha/255), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Gdiplus::Color color2((BYTE)(m_brush.Alpha2*Alpha/255), GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));
			Gdiplus::HatchBrush* brush = new HatchBrush((HatchStyle)(m_brush.Type - Painter::c_BrushTypeHatch1), color1, color2);

			pBrush = brush;

			return pBrush;
		}
		// create gradient brush (Horizontal, Vertical, Diagonals)
		else if (m_brush.Type >= Painter::c_BrushTypeHorizontal && m_brush.Type <= Painter::c_BrushTypeDiagonal2 && pRect)
		{
			Gdiplus::Color color1((BYTE)(m_brush.Alpha1*Alpha/255), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Gdiplus::Color color2((BYTE)(m_brush.Alpha2*Alpha/255), GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));
			Gdiplus::LinearGradientBrush* brush = new Gdiplus::LinearGradientBrush(*pRect, color1, color2, (LinearGradientMode)(m_brush.Type - Painter::c_BrushTypeHorizontal));

			pBrush = brush;

			return pBrush;
		}
		// create gradient brush (rect with center)
		else if (Painter::c_BrushTypeCenter == m_brush.Type && pRect)
		{
			Gdiplus::Color color1((BYTE)(m_brush.Alpha1*Alpha/255), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Gdiplus::Color color2((BYTE)(m_brush.Alpha2*Alpha/255), GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));
			Gdiplus::GraphicsPath path; path.AddRectangle(*pRect);
			PathGradientBrush* brush = new PathGradientBrush(&path);
			Color colors[] = {color1};
			int count = 1;
			brush->SetSurroundColors(colors, &count);
			brush->SetCenterColor(color2);

			pBrush = brush;

			return pBrush;
		}
		// create gradient brush (Cylinder Horizontal, Cylinder Vertical)
		else if (m_brush.Type >= Painter::c_BrushTypeCylinderHor && m_brush.Type <= Painter::c_BrushTypeCylinderVer && pRect)
		{
			Gdiplus::Color color1((BYTE)(m_brush.Alpha1*Alpha/255), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Gdiplus::Color color2((BYTE)(m_brush.Alpha2*Alpha/255), GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));
			Gdiplus::LinearGradientBrush* brush = NULL;
			switch(m_brush.Type - Painter::c_BrushTypeCylinderHor)
			{
			case 0:
				brush = new Gdiplus::LinearGradientBrush(*pRect, color1, color2, -90.F);
				brush->SetBlendBellShape(0.70f);
				break;
			case 1:
				brush = new Gdiplus::LinearGradientBrush(*pRect, color1, color2, -180.F);
				brush->SetBlendBellShape(0.70f);
				break;
			}

			pBrush = brush;

			return pBrush;
		}
		// create solid brush
		// if (Painter::c_BrushTypeSolid == m_brush.Type)
		Gdiplus::Color color((BYTE)(m_brush.Alpha1*Alpha/255), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
		Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(color);

		pBrush = brush;

		return pBrush;
	}
	void DrawPathGradientText(Graphics* graph, StringFormat* format, int i, BSTR Text, BYTE Alpha)
	{	  
		Font* myFont = m_font.GetFont();
		Gdiplus::Color color1((BYTE)(m_brush.Alpha1*Alpha/255), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
		Gdiplus::Color color2((BYTE)(m_brush.Alpha2*Alpha/255), GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));
		GraphicsPath path;
		int colorsCount = 4;
		if (Painter::c_BrushTypePathGradient1 == m_brush.Type)
		{
			// get rect of current(0) character
			RectF rectCharacter;
			Painter::GetRectCurrentCharacter(graph, format, myFont, Text, 1, 0, letter_rect, rectCharacter);
			
			// create path and brush
			path.AddRectangle(rectCharacter);
			PathGradientBrush* pBrush = new PathGradientBrush(&path);
			Color colors[] = {color1, color1, color1, color1};
			pBrush->SetSurroundColors(colors, &colorsCount);
			pBrush->SetCenterColor(color2);
		
			// draw text
			graph->DrawString(Text, 1, myFont, letter_rect, format, pBrush);

			// delete brush and reset path
			if (pBrush)
				delete pBrush;
			path.Reset();
		}
			
		if (Painter::c_BrushTypePathGradient2 == m_brush.Type)
		{   
			// get rect of current(0) character
			RectF rectCharacter;
			Painter::GetRectCurrentCharacter(graph, format, myFont, Text, 1, 0, letter_rect, rectCharacter);

			// create path and brush
			path.AddRectangle(rectCharacter);
			PathGradientBrush* pBrush = new PathGradientBrush(&path);
			Color colors1[] = {color1, color2, color2, color1};
			Color colors2[] = {color2, color1, color1, color2};
			if (i%2 == 0)
			{
				pBrush->SetSurroundColors(colors1, &colorsCount);
				pBrush->SetCenterColor(color1);
			}
			if (i%2 == 1)
			{
				pBrush->SetSurroundColors(colors2, &colorsCount);
				pBrush->SetCenterColor(color2);
			}

			// draw text
			graph->DrawString(Text, 1, myFont, letter_rect, format, pBrush);

			// delete brush and reset path
			if (pBrush)
				delete pBrush;
			path.Reset();
		}  
	}
	
	// appendix
	Gdiplus::ImageAttributes* GetImageAttributes(BYTE Alpha)
	{
		// alh >=0 && alph <= 1
		REAL alph = (REAL)(Alpha)/255.0F;
		ColorMatrix colorMatrix = {	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, alph, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		ImageAttributes* pImageAtt = new ImageAttributes();
		pImageAtt->SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
		return pImageAtt;
	}
	Gdiplus::TextureBrush* GetTexureBrush(int index, Painter::CTextureManager* manager, RectF* pRect, BYTE Alpha, int TextureMode)
	{
		// get texture bounds
		RectF boundsRect;
		Unit unit;
		Gdiplus::Bitmap* pTextureBitmap = manager->GetAt(index)->Bitmap;
		pTextureBitmap->GetBounds(&boundsRect, &unit);

		// create texture brush with textureAlpha
		ImageAttributes* pImageAtt = GetImageAttributes(Alpha);
		Gdiplus::TextureBrush* brush = new Gdiplus::TextureBrush(pTextureBitmap, boundsRect, pImageAtt);
		if (pImageAtt)
			delete pImageAtt;
		
		// choice texture mode
		switch(TextureMode)
		{
		// tile texture
		case Painter::c_BrushTextureModeTile:
			brush->SetWrapMode(WrapModeTile);
			break;
		// stretch texture
		case Painter::c_BrushTextureModeStretch:
			if (pRect)
			{
				brush->TranslateTransform(pRect->X, pRect->Y);
				brush->ScaleTransform(pRect->Width/boundsRect.Width, pRect->Height/boundsRect.Height);
			}
			break;
		}

		return brush;
	}
		
	// categories
	void AddCategory(const CString& name)
	{ 
		int count = m_categories.GetSize(); 
		
		Category category(name); 
		
		m_categories.Add(category); 
	}
	void AddEffect(const CString& name, const CString& name2, int id)
	{ 
		int count = m_effects.GetSize(); 
		
		m_categories[m_categories.GetSize() - 1].Effects.Add(count); 
		
		Effect effect(name, name2, id); 
		
		m_effects.Add(effect); 
	}
	
	// painting
	void EffectStart(BSTR String, long Category, long Effect, long EffectID, long CoorX, long CoorY, long Step)
	{
		int effect = -1;

		if (EffectID >= 0)
			effect = EffectID;
		else
		{
			if (Category < 0)
			{
				if (Effect >= 0 && Effect < m_effects.GetSize())
					effect = m_effects[Effect].ID;
			}
			else if (Category >=0 && Category < m_categories.GetSize())
			{
				if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
					effect = m_effects[m_categories[Category].Effects[Effect]].ID;
			}
		}

		if (m_effect != effect)
			EffectStop();

		m_effect = effect;
		m_string = String;

		if (effect < 0)
			return;

		// create letters array
		m_letters.RemoveAll();
		Gdiplus::Color brushColor1((BYTE)m_brush.Alpha1, GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
		ApplyEffect(brushColor1, 255, CoorX, CoorY, Step);
	}
	void EffectStop()
	{
		// TODO: clear effect resources here if necessary
	}
	void EffectRender(double Frame)
	{
		m_bitmap = new Bitmap(m_width, m_height, m_width*4, PixelFormat32bppRGB, m_pixels);

		if (Frame < 0.001)
			return;

		if (!m_bitmap)
			return;

		Graphics* graphics = new Graphics(m_bitmap);

		if (!graphics)
			return;

		graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);

		// variables
		Waypoint point;
		
		// setup painting format
		StringFormat format; 
		format.SetFormatFlags(StringFormatFlagsNoWrap);
		format.SetAlignment(StringAlignmentCenter); 
		format.SetLineAlignment(StringAlignmentCenter);
		format.SetTrimming(StringTrimmingNone);

		Font* myFont = m_font.GetFont();
		Matrix oMatrix;
		oMatrix.Reset();
		oMatrix.RotateAt(m_nAngle, PointF(m_nStartX, m_nStartY + myFont->GetHeight(graphics)/2));
		graphics->SetTransform(&oMatrix);

		// draw shadow for all letters
		if (m_shadow.Visible) 
		{   		
			for (int index = 0; index < m_letters.GetSize(); ++index)
			{
				if (!m_letters[index].GetWaypoint(Frame, point))
					continue;
				if (!point.Visible)
					continue;

				// reset transformation sequence
				graphics->ResetTransform();	
								
				// apply transform to the graphics
				graphics->SetTransform(&oMatrix);
				point.Render(graphics);
				
				// draw shadow
				m_shadow.Draw(graphics, myFont, &format, &(point.Name), 1, letter_rect, point.Alpha);
			}
		}	

		// draw all letters
		for (int index = 0; index < m_letters.GetSize(); ++index)
		{
			if (!m_letters[index].GetWaypoint(Frame, point))
				continue;
			if (!point.Visible)
				continue;

			// reset transformation sequence
			graphics->ResetTransform();	
			
			// apply transform to the graphics
			graphics->SetTransform(&oMatrix);
			point.Render(graphics);

			// draw edge
			m_edge.Draw(graphics, myFont, &format, &(point.Name), 1, letter_rect, point.Alpha);

			// draw letter at (0,0) position
			if((Painter::c_BrushTypePathGradient1 != m_brush.Type) && (Painter::c_BrushTypePathGradient2 != m_brush.Type))
			{
				Brush* myBrush = GetBrush(&m_textures, &letter_rect, point.Alpha);
				graphics->DrawString(&(point.Name), 1, myFont, letter_rect, &format, myBrush);

				// delete brush
				if (myBrush)
					delete myBrush;
			}
			else
				DrawPathGradientText(graphics, &format, index, &(point.Name), point.Alpha);

			// stop painting
			graphics->Flush();
		}
		
		graphics->ResetTransform();

		// delete bitmap
		if (graphics)
			delete graphics;
		if (m_bitmap)
			delete m_bitmap;
	}
	BOOL EffectRenderEnd()
	{
		// variables
		int nIndex = 0;
		int nLineSize = 4*m_width;
		int nIndexFlip = nLineSize*(m_height - 1);
		BYTE* pLine = new BYTE[nLineSize];

		// flip image vertically
		for (int nY = 0; nY < m_height/2; ++nY, nIndex += nLineSize, nIndexFlip -= nLineSize)
		{
			memcpy(pLine, (BYTE*)(m_pixels + nIndex), nLineSize);
			memcpy((BYTE*)(m_pixels + nIndex), (BYTE*)(m_pixels + nIndexFlip), nLineSize);
			memcpy((BYTE*)(m_pixels + nIndexFlip), pLine, nLineSize);
		}

		// clear memory
		delete[] pLine;

		// all ok
		return TRUE;
	}
	
	// effect
	void ApplyEffect(Color Color, long Alpha, long CoorX, long CoorY, long Step)
	{
		int length = m_string.GetLength();

		if (length < 1)
			return;

		BSTR bstr = m_string.AllocSysString();

		Font* myFont = m_font.GetFont();
		
		// TODO: step == 0 situation currently is under investigation
		if (Step == 0)
		{
			long newCoorX = CoorX + myFont->GetSize()/2;
			long nDistance = 0;
			
			Gdiplus::Bitmap* pBitmap = NULL;
			
			if (m_pixels == NULL)
				pBitmap = new Bitmap(640, 480, 640*4, PixelFormat32bppRGB, m_pixels);
			else
				pBitmap = new Bitmap(m_width, m_height, m_width*4, PixelFormat32bppRGB, m_pixels);
			
			Gdiplus::Graphics* pGraphics = new Gdiplus::Graphics(pBitmap);
			
			if(!pGraphics || !pBitmap)
				return;
			
					
			StringFormat format; 
			format.SetFormatFlags(StringFormatFlagsNoWrap);
			format.SetAlignment(StringAlignmentCenter); 
			format.SetLineAlignment(StringAlignmentCenter);
			format.SetTrimming(StringTrimmingNone);
									
			for (int index = 0; index < length; ++index)
			{
				Letter letter;
				letter.Initialize(Color.GetValue(), (long)myFont->GetSize());
				letter.Name = bstr[index];        

				PointF pointOrigin(CoorX, CoorY);
				RectF StringBox;
				
				pGraphics->MeasureString(&bstr[index], 1, myFont, pointOrigin, &format, &StringBox);

				/*WCHAR* mas = new WCHAR[2];
				
				mas[0] = bstr[index];
				mas[1] = bstr[index];

				RectF rect;
				pGraphics->MeasureString(mas, 2, myFont, PointF(0, 0), &format, &rect);
				int len1 = rect.Width;
				pGraphics->MeasureString(mas, 1, myFont, PointF(0, 0), &format, &rect);
				int len2 = rect.Width;
				int nDist = len1 - len2;*/
				
				m_finish.Add(newCoorX / 2);
				m_finish.Add(CoorY + myFont->GetSize()/2);
				newCoorX += (StringBox.Width + myFont->GetSize()/4);
				//newCoorX += nDist;
				//newCoorX -= nDist;

				m_letters.Add(letter);
			}
			
			delete pBitmap;
			delete pGraphics;
		}
		
		else 
		{
			for (int index = 0; index < length; ++index)
			{
				// initialize letter
				Letter letter;
				letter.Initialize(Color.GetValue(), (long)myFont->GetSize());
				letter.Name = bstr[index];        

				// fill finish point X
				m_finish.Add(Step*index + CoorX + myFont->GetSize()/2);

				// fill finish point Y
				m_finish.Add(CoorY + myFont->GetSize()/2);
					
				m_letters.Add(letter);
			}
		}
		

		// draw letters with specified effecta 
		switch (m_effect)
		{
			// Appear Displace
			case 1: RandomMovements(m_letters, m_finish, Alpha); break;
			case 2: AppearFade(m_letters, m_finish, Alpha); break;
			case 3: AppearShade(m_letters, m_finish, Alpha); break;
			case 4: AppearRotate(m_letters, m_finish, Alpha); break;
			case 5: AppearRandom(m_letters, m_finish, Alpha); break;
			case 6: Airport(m_letters, m_finish, Alpha); break;
			case 7: AppearShake(m_letters, m_finish, Alpha); break;
			case 8: AppearWedge(m_letters, m_finish, Alpha); break;
			case 9: AppearStream(m_letters, m_finish, Alpha); break;

			// Appear Waves
			case 101: WaveSimple(m_letters, m_finish, Alpha); break;
			case 102: WaveZoom(m_letters, m_finish, Alpha); break;
			case 103: WaveFall(m_letters, m_finish, Alpha); break;
			case 104: WaveRunShift(m_letters, m_finish, Alpha); break;
			case 105: WaveSwing(m_letters, m_finish, Alpha); break;
			case 106: WaveRun(m_letters, m_finish, Alpha); break;
			case 107: WaveZoomVertical(m_letters, m_finish, Alpha); break;
			case 108: WaveShift(m_letters, m_finish, Alpha); break;
			case 109: WavePulse(m_letters, m_finish, Alpha); break;

			// Erase Displace
			case 201: RandomMovementsBack(m_letters, m_finish, Alpha); break;
			case 202: AppearFadeBack(m_letters, m_finish, Alpha); break;
			case 203: AppearShadeBack(m_letters, m_finish, Alpha); break;
			case 204: AppearRotateBack(m_letters, m_finish, Alpha); break;
			case 205: AppearRandomBack(m_letters, m_finish, Alpha); break;
			case 206: AirportBack(m_letters, m_finish, Alpha); break;
			case 207: AppearShakeBack(m_letters, m_finish, Alpha); break;
			case 208: AppearWedgeBack(m_letters, m_finish, Alpha); break;
			case 209: AppearStreamBack(m_letters, m_finish, Alpha); break;

			// Erase Waves
			case 301: WaveSimpleBack(m_letters, m_finish, Alpha);break;
			case 302: WaveZoomBack(m_letters, m_finish, Alpha); break;
			case 303: WaveFallBack(m_letters, m_finish, Alpha); break;
			case 304: WaveRunShiftBack(m_letters, m_finish, Alpha); break;
			case 305: WaveSwingBack(m_letters, m_finish, Alpha); break;
			case 306: WaveRunBack(m_letters, m_finish, Alpha); break;
			case 307: WaveZoomVerticalBack(m_letters, m_finish, Alpha); break;
			case 308: WaveShiftBack(m_letters, m_finish, Alpha); break;
			case 309: WavePulseBack(m_letters, m_finish, Alpha); break;

			// New
			case 402: Ejection(m_letters, m_finish, Alpha); break;
			case 403: Ellipse(m_letters, m_finish, Alpha); break;
			case 404: Wheel(m_letters, m_finish, Alpha); break;
			case 405: Miscarry(m_letters, m_finish, Alpha); break;	
			case 406: Rotation(m_letters, m_finish, Alpha); break;	
			case 407: Increase(m_letters, m_finish, Alpha); break;	
			case 408: TwoString(m_letters, m_finish, Alpha); break;	
			case 409: Tumbling(m_letters, m_finish, Alpha); break;	
			case 410: Leave(m_letters, m_finish, Alpha); break;	
			case 411: Unfold(m_letters, m_finish, Alpha); break;	
			case 412: Centrifuge(m_letters, m_finish, Alpha); break;
			case 413: HorizontalRotation(m_letters, m_finish, Alpha); break;
			case 414: HorizontalRotation3D(m_letters, m_finish, Alpha); break;
			case 415: VerticalRotation(m_letters, m_finish, Alpha); break;
			case 416: CheChe(m_letters, m_finish, Alpha); break;


	   		// Erase New
			case 502: EjectionBack(m_letters, m_finish, Alpha); break;
			case 503: EllipseBack(m_letters, m_finish, Alpha); break;
			case 504: WheelBack(m_letters, m_finish, Alpha); break;
			case 505: MiscarryBack(m_letters, m_finish, Alpha); break;	
			case 506: RotationBack(m_letters, m_finish, Alpha); break;	
			case 507: IncreaseBack(m_letters, m_finish, Alpha); break;	
			case 508: TwoStringBack(m_letters, m_finish, Alpha); break;	
			case 509: TumblingBack(m_letters, m_finish, Alpha); break;	
			case 510: LeaveBack(m_letters, m_finish, Alpha); break;	
			case 511: UnfoldBack(m_letters, m_finish, Alpha); break;	
			case 512: CentrifugeBack(m_letters, m_finish, Alpha); break;
			case 513: HorizontalRotationBack(m_letters, m_finish, Alpha); break;	
			case 514: HorizontalRotation3DBack(m_letters, m_finish, Alpha); break;	
			case 515: VerticalRotationBack(m_letters, m_finish, Alpha); break;			
			case 516: CheCheBack(m_letters, m_finish, Alpha); break;

			// New2
			case 601: Boomerang(m_letters, m_finish, Alpha); break;
			case 602: Flash(m_letters, m_finish, Alpha); break;
			case 603: Flight(m_letters, m_finish, Alpha); break;
			case 604: Jump(m_letters, m_finish, Alpha); break;
			case 605: Compress(m_letters, m_finish, Alpha); break;
			case 606: Motion(m_letters, m_finish, Alpha); break;
			case 607: Manifestation(m_letters, m_finish, Alpha); break;
			case 608: Shot(m_letters, m_finish, Alpha); break;
			case 609: Reduction(m_letters, m_finish, Alpha); break;
			case 610: VHRotation(m_letters, m_finish, Alpha); break;
			case 611: Rotate3D(m_letters, m_finish, Alpha); break;
			case 612: Spiral(m_letters, m_finish, Alpha); break;
			case 613: Flag(m_letters, m_finish, Alpha); break;
			case 614: Perpendicular(m_letters, m_finish, Alpha); break;
			case 615: Clock(m_letters, m_finish, Alpha); break;

	   		// Erase New2
			case 701: BoomerangBack(m_letters, m_finish, Alpha); break;
			case 702: FlashBack(m_letters, m_finish, Alpha); break;
			case 703: FlightBack(m_letters, m_finish, Alpha); break;
			case 704: JumpBack(m_letters, m_finish, Alpha); break;
			case 705: CompressBack(m_letters, m_finish, Alpha); break;
			case 706: MotionBack(m_letters, m_finish, Alpha); break;
			case 707: ManifestationBack(m_letters, m_finish, Alpha); break;
			case 708: ShotBack(m_letters, m_finish, Alpha); break;
			case 709: ReductionBack(m_letters, m_finish, Alpha); break;
			case 710: VHRotationBack(m_letters, m_finish, Alpha); break;
			case 711: Rotate3DBack(m_letters, m_finish, Alpha); break;
			case 712: SpiralBack(m_letters, m_finish, Alpha); break;
			case 713: FlagBack(m_letters, m_finish, Alpha); break;
			case 714: PerpendicularBack(m_letters, m_finish, Alpha); break;
			case 715: ClockBack(m_letters, m_finish, Alpha); break;

			// New3
			case 801: Galaxy(m_letters, m_finish, Alpha); break;
			case 802: WaveRotate3D(m_letters, m_finish, Alpha); break;
			case 803: RandomJump(m_letters, m_finish, Alpha); break;
			case 804: Galaxy2(m_letters, m_finish, Alpha); break;
			case 805: Roll(m_letters, m_finish, Alpha); break;
			case 806: Conga(m_letters, m_finish, Alpha); break;
			case 807: _Matrix(m_letters, m_finish, Alpha); break;
			case 808: HotPinkSpin(m_letters, m_finish, Alpha); break;
			case 809: HopInJumpOut(m_letters, m_finish, Alpha); break;
			case 810: ConJoin(m_letters, m_finish, Alpha); break;
			case 811: HopsCotch(m_letters, m_finish, Alpha); break;
			case 812: Twist(m_letters, m_finish, Alpha); break;
			case 813: StarWars(m_letters, m_finish, Alpha); break;
			case 814: Squeeze(m_letters, m_finish, Alpha); break;
			case 815: Terminator(m_letters, m_finish, Alpha); break;

			// Erase new3
			case 901: GalaxyBack(m_letters, m_finish, Alpha); break;
			case 902: WaveRotate3DBack(m_letters, m_finish, Alpha); break;
			case 903: RandomJumpBack(m_letters, m_finish, Alpha); break;
			case 904: Galaxy2Back(m_letters, m_finish, Alpha); break;
			case 905: RollBack(m_letters, m_finish, Alpha); break;
			case 906: CongaBack(m_letters, m_finish, Alpha); break;
			case 907: _MatrixBack(m_letters, m_finish, Alpha); break;
			case 908: HotPinkSpinBack(m_letters, m_finish, Alpha); break;
			case 909: HopInJumpOutBack(m_letters, m_finish, Alpha); break;
			case 910: ConJoinBack(m_letters, m_finish, Alpha); break;
			case 911: HopsCotchBack(m_letters, m_finish, Alpha); break;
			case 912: TwistBack(m_letters, m_finish, Alpha); break;
			case 913: StarWarsBack(m_letters, m_finish, Alpha); break;
			case 914: SqueezeBack(m_letters, m_finish, Alpha); break;
			case 915: TerminatorBack(m_letters, m_finish, Alpha); break;
			
		}																				
		
		if (m_finish.GetSize() >= 2)
		{
			m_nStartX = m_finish[0];
			m_nStartY = m_finish[1];
		}

		m_finish.RemoveAll();

		SysFreeString(bstr);
	}
};

