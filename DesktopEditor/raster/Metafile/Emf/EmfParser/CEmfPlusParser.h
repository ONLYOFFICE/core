#ifndef CEMFPLUSPARSER_H
#define CEMFPLUSPARSER_H

//#include "../../Common/MetaFileUtils.h"
//#include "../../Common/MetaFile.h"
#include "../EmfPlusObjects.h"
#include "CEmfParserBase.h"
#include "../EmfPlusTypes.h"
#include "../EmfPlayer.h"

namespace MetaFile
{
	class CEmfPlusParser : public CEmfParserBase
	{
	public:
		CEmfPlusParser(CEmfInterpretatorBase *pEmfInterpretator, const TEmfHeader& oHeader);
		virtual ~CEmfPlusParser();

		bool            ReadFromBuffer(BYTE* pBuffer, unsigned int unSize, const bool& bIsExternal = true) override;
		bool            OpenFromFile(const wchar_t* wsFilePath)     override;

		void            PlayFile()                                  override;
		void            Scan()                                      override;
		double          GetDpi()                                    override;

		EmfParserType   GetType()				    override;

		void   SetStream(BYTE *pBytes, unsigned int unSize);
		bool   GetBanEMFProcesses();

	private:
		void RegisterObject(CEmfPlusObject* pObject, unsigned int unIndex);

		bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight) override;

		void ReadImage(unsigned short shObjectIndex, bool bIsContinuedObject);

		void ReadImage(CEmfPlusImage& oImage, bool bReadData = true);
		void ReadMetaFile(CEmfPlusImage& oImage, bool bReadData);
		void ReadBitmap(CEmfPlusImage& oImage, bool bReadData);

		CEmfPlusImage* GetImage(unsigned int unImageIndex);

		CEmfPlusBrush*  ReadBrush();
		CEmfPlusBrush*  GetBrush(unsigned int unBrushIndex);

		CEmfPlusPen*    ReadPen();
		CEmfPlusPen*    GetPen(unsigned int unPenIndex);

		CEmfPlusFont*   ReadFont();
		CEmfPlusFont*   GetFont(unsigned int unPenIndex);

		CEmfPlusPath*   ReadPath();
		CEmfPlusPath*   GetPath(unsigned int unPathIndex);

		CEmfPlusRegion*          ReadRegion();
		CEmfPlusRegionNode*      ReadRegionNode(unsigned int& unIndex);
		CEmfPlusRegionNodePath*  ReadRegionNodePath(unsigned int& unIndex);
		CEmfPlusRegionNodeRectF* ReadRegionNodeRectF(unsigned int& unIndex);
		CEmfPlusRegionNodeChild* ReadRegionNodeChild(unsigned int& unIndex);
		CEmfPlusRegion*          GetRegion(unsigned int unRegionIndex);

		CEmfPlusStringFormat*    GetStringFormat(unsigned int unStringFormatIndex);

		CEmfPlusImageAttributes* GetImageAttributes(unsigned int unImageAttributesIndex);

		template<typename PointType>
		std::vector<PointType> ReadPoints(unsigned int unPointCount);

		std::vector<char> ReadPointTypes(unsigned int unPointCount);

		void DrawRectangle(const TEmfPlusRectF& oRectangle, bool bStroke, bool bFill);

		void DrawLines(std::vector<TEmfPlusPointF> arPoints, bool bCloseFigure);

		void DrawImagePoints(unsigned int unImageIndex, unsigned int unImageAttributeIndex, const TEmfPlusRectF& oSrcRect, const std::vector<TEmfPlusPointF>& arPoints);
		void DrawMetafile(BYTE* pBuffer, unsigned int unSize, const TEmfPlusRectF& oSrcRect, const std::vector<TEmfPlusPointF>& arPoints, EEmfPlusMetafileDataType eMetafileType, unsigned int unImageAttributeIndex);
		void DrawBitmap(BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight, const TEmfPlusRectF& oSrcRect, const std::vector<TEmfPlusPointF>& arPoints);

		TEmfPlusARGB ApplyImageAttributes(TEmfPlusRectF& oRectangle, const CEmfPlusImageAttributes& oImageAttributes);

		void CombineClip(TRectD oBox, int nMode);
		template<typename T> std::vector<TEmfPlusPointF> GetConvertedPoints(std::vector<T>arPoints);
		template<typename T> TEmfPlusRectF GetConvertedRectangle(T oRectangle);

		void UpdateMatrix(TEmfPlusXForm& oMatrix);

		bool SaveImage(const CEmfPlusImage& oImage, std::wstring& wsPathToImage);

		void Read_EMRPLUS_OFFSETCLIP();
		void Read_EMRPLUS_RESETCLIP();
		void Read_EMFPLUS_SETCLIPPATH(unsigned short unShFlags);
		void Read_EMFPLUS_SETCLIPRECT(unsigned short unShFlags);
		void Read_EMFPLUS_SETCLIPREGION(unsigned short unShFlags);

		void Read_EMFPLUS_COMMENT();

		void Read_EMFPLUS_ENDOFFILE();
		void Read_EMFPLUS_GETDC();
		void Read_EMFPLUS_HEADER(unsigned short unShFlags);

		void Read_EMFPLUS_CLEAR();
		void Read_EMFPLUS_DRAWARC(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWARC_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWBEZIERS(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWBEZIERS_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWCLOSEDCURVE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWCLOSEDCURVE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWCURVE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWCURVE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWDRIVERSTRING(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWELLIPSE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWELLIPSE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWIMAGE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWIMAGE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWIMAGEPOINTS(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWIMAGEPOINTS_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWLINES(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWLINES_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWPATH(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWPIE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWPIE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWRECTS(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_DRAWRECTS_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_DRAWSTRING(unsigned short unShFlags);
		void Read_EMFPLUS_FILLCLOSEDCURVE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_FILLCLOSEDCURVE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_FILLELLIPSE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_FILLELLIPSE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_FILLPATH(unsigned short unShFlags);
		void Read_EMFPLUS_FILLPIE(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_FILLPIE_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_FILLPOLYGON(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_FILLPOLYGON_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_FILLRECTS(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_FILLRECTS_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_FILLREGION(unsigned short unShFlags);

		void Read_EMFPLUS_OBJECT(unsigned short unShFlags);
		void Read_EMFPLUS_SERIALIZABLEOBJECT(unsigned short unShFlags);

		void Read_EMFPLUS_SETANTIALIASMODE(unsigned short unShFlags);
		void Read_EMFPLUS_SETCOMPOSITINGMODE(unsigned short unShFlags);
		void Read_EMFPLUS_SETCOMPOSITINGQUALITY(unsigned short unShFlags);
		void Read_EMFPLUS_SETINTERPOLATIONMODE(unsigned short unShFlags);
		void Read_EMFPLUS_SETPIXELOFFSETMODE(unsigned short unShFlags);
		void Read_EMFPLUS_SETRENDERINGORIGIN();
		void Read_EMFPLUS_SETTEXTCONTRAST(unsigned short unShFlags);
		void Read_EMRPLUS_SETTEXTRENDERINGHINT(unsigned short unShFlags);

		void Read_EMFPLUS_BEGINCONTAINER(unsigned short unShFlags);
		void Read_EMFPLUS_BEGINCONTAINERNOPARAMS();
		void Read_EMFPLUS_ENDCONTAINER();
		void Read_EMFPLUS_RESTORE();
		void Read_EMFPLUS_SAVE();

		void Read_EMFPLUS_SETTSCLIP(unsigned short unShFlags);
		template<typename T>void Read_EMFPLUS_SETTSCLIP_BASE(unsigned short unShFlags);
		void Read_EMFPLUS_SETTSGRAPHICS(unsigned short unShFlags);

		void Read_EMFPLUS_MULTIPLYWORLDTRANSFORM(unsigned short unShFlags);
		void Read_EMFPLUS_RESETWORLDTRANSFORM();
		void Read_EMFPLUS_ROTATEWORLDTRANSFORM(unsigned short unShFlags);
		void Read_EMFPLUS_SCALEWORLDTRANSFORM(unsigned short unShFlags);
		void Read_EMFPLUS_SETPAGETRANSFORM(unsigned short unShFlags);
		void Read_EMFPLUS_SETWORLDTRANSFORM();
		void Read_EMFPLUS_TRANSLATEWORLDTRANSFORM(unsigned short unShFlags);

		template<typename T> short ExpressValue(T Flags, unsigned int unStartIndex, unsigned int unEndIndex) const;

		bool            m_bBanEmfProcessing;
		unsigned int    m_unLogicalDpiX;
		unsigned int    m_unLogicalDpiY;

		double          m_dUnitKoef;

		typedef std::map<unsigned int, CEmfPlusObject*> EmfPlusObjects;

		EmfPlusObjects m_mObjects;
	};
}

#endif // CEMFPLUSPARSER_H
