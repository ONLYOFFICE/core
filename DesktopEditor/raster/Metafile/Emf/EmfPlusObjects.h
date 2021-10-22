#ifndef _METAFILE_EMF_EMFPLUSOBJECTS_H
#define _METAFILE_EMF_EMFPLUSOBJECTS_H

#include "EmfPlusTypes.h"
#include "../../../common/File.h"
#include "../Common/MetaFileObjects.h"

namespace MetaFile
{
        typedef enum
        {
                ObjectTypeInvalid           = 0x00,
                ObjectTypeBrush             = 0x01,
                ObjectTypePen               = 0x02,
                ObjectTypePath              = 0x03,
                ObjectTypeRegion            = 0x04,
                ObjectTypeImage             = 0x05,
                ObjectTypeFont              = 0x06,
                ObjectTypeStringFormat      = 0x07,
                ObjectTypeImageAttributes   = 0x08,
                ObjectTypeCustomLineCap     = 0x09
        } EEmfPlusObjectType;

	class CEmfPlusObjectBase
	{
	public:
		CEmfPlusObjectBase(){}
		virtual ~CEmfPlusObjectBase(){}
		virtual EEmfPlusObjectType GetType()
		{
			return ObjectTypeInvalid;
		}
	};

        typedef  enum
         {
                BrushTypeSolidColor     = 0x00,
                BrushTypeHatchFill      = 0x01,
                BrushTypeTextureFill    = 0x02,
                BrushTypePathGradient   = 0x03,
                BrushTypeLinearGradient = 0x04
         } EEmfPlusBrushType;

	class CEmfPlusBrush : public CEmfPlusObjectBase, public IBrush
	{
	    public:
		virtual EEmfPlusObjectType GetType() override
		{
			return ObjectTypeBrush;
		}

		int          GetColor() override
		{
			return METAFILE_RGBA(m_oARGB.chRed, m_oARGB.chGreen, m_oARGB.chBlue);
		}

		int          GetColor2() override
		{
			return METAFILE_RGBA(m_oBackARGB.chRed, m_oBackARGB.chGreen, m_oBackARGB.chBlue);
		}

		unsigned int GetStyle() override
		{
			return m_unBrushStyle;
		}

		unsigned int GetStyleEx() override
		{
			return 0;
		}

		unsigned int GetHatch() override
		{
			return 0;
		}

		unsigned int GetAlpha() override
		{
			return 0;
		}

		unsigned int GetAlpha2() override
		{
			return 0xff;
		}

		std::wstring GetDibPatterPath() override
		{
			return std::wstring();
		}

		void GetBounds(double& left, double& top, double& width, double& height) override {}

		TEmfPlusARGB		m_oARGB;
		TEmfPlusARGB		m_oBackARGB;
		unsigned int		m_unBrushStyle;

		EEmfPlusBrushType	m_enType;
	};

        typedef  enum
        {
                UnitTypeWorld       = 0x00,
                UnitTypeDisplay     = 0x01,
                UnitTypePixel       = 0x02,
                UnitTypePoint       = 0x03,
                UnitTypeInch        = 0x04,
                UnitTypeDocument    = 0x05,
                UnitTypeMillimeter  = 0x06
        } EEmfPlusPenUnitType;

        typedef enum
        {
                PenDataTransform        = 0x0001,
                PenDataStartCap         = 0x0002,
                PenDataEndCap           = 0x0004,
                PenDataJoin             = 0x0008,
                PenDataMiterLimit       = 0x0010,
                PenDataLineStyle        = 0x0020,
                PenDataDashedLineCap    = 0x0040,
                PenDataDashedLineOffset = 0x0080,
                PenDataDashedLine       = 0x0100,
                PenDataNonCenter        = 0x0200,
                PenDataCompoundLine     = 0x0400,
                PenDataCustomStartCap   = 0x0800,
                PenDataCustomEndCap     = 0x1000
        } EEmfPlusPenDataFlags;

	class CEmfPlusPen: public CEmfPlusObjectBase, public IPen
	{
	    public:
		virtual EEmfPlusObjectType GetType()
		{
			return ObjectTypePen;
		}

		int          GetColor()
		{
			return 0;
		}

		unsigned int GetStyle()
		{
			return (unsigned int)EEmfPlusPenUnitType;
		}

		unsigned int GetWidth()
		{
			return (unsigned int)m_dWidth;
		}

		EEmfPlusPenUnitType	m_enUnit;
		double			m_dWidth;
	};
}

#endif // _METAFILE_EMF_EMFPLUSOBJECTS_H
