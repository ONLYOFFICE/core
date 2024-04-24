/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
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
		USHORT          GetDpi()                                    override;

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

		void ReadBufferPath(CEmfPlusBuffer* pDataPath);
		CEmfPlusBuffer*  GetBuffer(unsigned int unPathIndex);

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
		template <typename MetafileType>
		void DrawMetafile(BYTE* pBuffer, unsigned int unSize, const TEmfPlusRectF& oSrcRect, const std::vector<TEmfPlusPointF>& arPoints);
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
