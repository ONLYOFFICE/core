// Font.h: interface for the Font class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_FONT_H__A1C73CFC_A9F8_4B48_AEC3_2E061766C78C__INCLUDED_)
#define Aggplus_FONT_H__A1C73CFC_A9F8_4B48_AEC3_2E061766C78C__INCLUDED_

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"

#include "agg_font_win32_tt.h"

typedef agg::font_engine_win32_tt_int32 font_engine_type;
typedef agg::font_cache_manager<font_engine_type> font_manager_type;


namespace Aggplus
{
		class Graphics;
		class RectF;

		class MeasureCharsRectF
		{
			public:
				MeasureCharsRectF() : X(0.0f), Y(0.0f), Width(0.0f), Height(0.0f), StartX(0.0f), Advance(0.0f) { }
			public:
				REAL X, Y, Width, Height, StartX, Advance;
		};

		class Font  
		{
				friend class GraphicsPath;
			protected:
				void Construct(const WCHAR* familyName, float emSize, int style, Unit unit);//
				Font();
			public:
				Font(const WCHAR *familyName, REAL emSize, INT style = FontStyleRegular, Unit unit = UnitPoint);//const FontCollection *  fontCollection = NULL
				virtual ~Font();

				BOOL IsAvailable() const;
				Unit GetUnit() const { return m_Unit; }

				inline REAL GetSize() const { return(m_femSize); }
				REAL        GetHeight(REAL dpi) const;

				       double get_1_6_em() const;
				inline UINT16 GetEmHeight() const { return m_nEmHeight; } //from FontFamily
				inline UINT16 GetCellAscent() const { return m_nCellAscent; } //from FontFamily
				inline UINT16 GetCellDescent() const { return m_nCellDescent; } //from FontFamily
				inline UINT16 GetLineSpacing() const { return m_nLineSpacing; } //from FontFamily

				//ALTER Begin
				void  MeasureChars2(const WCHAR* string, int length, const PointF &origin, MeasureCharsRectF *rcArray) const;
				float get_res_unit_to_pixel() const;
				//ALTER End

				font_engine_type *get_agg_font_engine() const { return(m_agg_pFeng); }
			protected:
				font_engine_type *m_agg_pFeng;

				Unit m_Unit;
				//FontFamily m_fFamily;
				float m_femSize;


				unsigned short m_nEmHeight;
				unsigned short m_nCellAscent;
				unsigned short m_nCellDescent;
				unsigned short m_nLineSpacing;

				HDC m_hDC;
				//static int m_hdcUsage;
		};

		void aggplus_flat_measure_fit_chars(const Font *font, const WCHAR* string, int length, float fMaxWidth, float *pfLinStart, float *pfLinWidth, int *pnLinChars, int *pnAdvanceChars, bool *break_by_newline);


}
#endif // !defined(Aggplus_FONT_H__A1C73CFC_A9F8_4B48_AEC3_2E061766C78C__INCLUDED_)
