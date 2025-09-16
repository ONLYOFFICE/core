/*
* (c) Copyright Ascensio System SIA 2010-2023
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
#pragma once

#define MAYBE_UNUSED(x) (void)(x)

const double c_dDpiX = 72.0;
const double c_dDpiY = 72.0;

constexpr double c_dMMToPix = 72.0 / 25.4;
constexpr double c_dPixToMM = 25.4 / 72.0;
constexpr double c_dMMToPt  = 72.0 / 25.4;
constexpr double c_dPtToMM  = 25.4 / 72.0;
constexpr double c_dY_PRECISION_MM = 1.0;
constexpr double c_dX_PRECISION_MM = 0.05;
constexpr double c_dSPACE_WIDTH_COEF = 0.8;
constexpr double c_dSPLIT_WIDTH_COEF = 2.0;
constexpr double c_dFONT_SIZE_DECREASE_COEF = 0.7;

const unsigned int c_SPACE_SYM = 0x20;

#include <memory>

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../../../DesktopEditor/graphics/pro/officedrawingfile.h"

class CTxtRenderer : public IRenderer
{
public:
	CTxtRenderer();
	CTxtRenderer(const CTxtRenderer& other) = delete;
	CTxtRenderer(CTxtRenderer&& other) = delete;
	virtual ~CTxtRenderer();

	// base settings
	virtual HRESULT get_Type  (LONG* lType)           override;
	virtual HRESULT NewPage   ()                      override;
	virtual HRESULT get_Height(double* dHeight)       override;
	virtual HRESULT put_Height(const double& dHeight) override;
	virtual HRESULT get_Width (double* dWidth)        override;
	virtual HRESULT put_Width (const double& dWidth)  override;
	virtual HRESULT get_DpiX  (double* dDpiX)         override;
	virtual HRESULT get_DpiY  (double* dDpiY)         override;

	// pen settings
	virtual HRESULT get_PenColor(LONG* lColor)       override;
	virtual HRESULT put_PenColor(const LONG& lColor) override;

	virtual HRESULT get_PenAlpha(LONG* lAlpha)       override;
	virtual HRESULT put_PenAlpha(const LONG& lAlpha) override;

	virtual HRESULT get_PenSize(double* dSize)       override;
	virtual HRESULT put_PenSize(const double& dSize) override;

	virtual HRESULT get_PenDashStyle(BYTE* nDashStyle)       override;
	virtual HRESULT put_PenDashStyle(const BYTE& nDashStyle) override;

	virtual HRESULT get_PenLineStartCap(BYTE* nCapStyle)       override;
	virtual HRESULT put_PenLineStartCap(const BYTE& nCapStyle) override;

	virtual HRESULT get_PenLineEndCap(BYTE* nCapStyle)       override;
	virtual HRESULT put_PenLineEndCap(const BYTE& nCapStyle) override;

	virtual HRESULT get_PenLineJoin(BYTE* nJoinStyle)       override;
	virtual HRESULT put_PenLineJoin(const BYTE& nJoinStyle) override;

	virtual HRESULT get_PenDashOffset(double* dOffset)       override;
	virtual HRESULT put_PenDashOffset(const double& dOffset) override;

	virtual HRESULT get_PenAlign(LONG* lAlign)       override;
	virtual HRESULT put_PenAlign(const LONG& lAlign) override;

	virtual HRESULT get_PenMiterLimit(double* dMiter)       override;
	virtual HRESULT put_PenMiterLimit(const double& dMiter) override;

	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount) override;

	// brush settings
	virtual HRESULT get_BrushType(LONG* lType)      override;
	virtual HRESULT put_BrushType(const LONG& lType)override;

	virtual HRESULT get_BrushColor1(LONG* lColor)       override;
	virtual HRESULT put_BrushColor1(const LONG& lColor) override;

	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)       override;
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha) override;

	virtual HRESULT get_BrushColor2(LONG* lColor)       override;
	virtual HRESULT put_BrushColor2(const LONG& lColor) override;

	virtual HRESULT get_BrushAlpha2(LONG* lAlpha)       override;
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha) override;

	virtual HRESULT get_BrushTexturePath(std::wstring* wsPath)       override;
	virtual HRESULT put_BrushTexturePath(const std::wstring& wsPath) override;

	virtual HRESULT get_BrushTextureMode(LONG* lMode)       override;
	virtual HRESULT put_BrushTextureMode(const LONG& lMode) override;

	virtual HRESULT get_BrushTextureAlpha(LONG* lAlpha)       override;
	virtual HRESULT put_BrushTextureAlpha(const LONG& lAlpha) override;

	virtual HRESULT get_BrushLinearAngle(double* dAngle)       override;
	virtual HRESULT put_BrushLinearAngle(const double& dAngle) override;

	virtual HRESULT BrushRect  (const INT& nVal,
	                            const double& dLeft,
	                            const double& dTop,
	                            const double& dWidth,
	                            const double& dHeight) override;

	virtual HRESULT BrushBounds(const double& dLeft,
	                            const double& dTop,
	                            const double& dWidth,
	                            const double& dHeight) override;

	virtual HRESULT put_BrushGradientColors(LONG* pColors,
	                                        double* pPositions,
	                                        LONG lCount) override;

	virtual HRESULT get_BrushTextureImage(Aggplus::CImage** pImage) override;
	virtual HRESULT put_BrushTextureImage(Aggplus::CImage* pImage)  override;

	virtual HRESULT get_BrushTransform(Aggplus::CMatrix& oMatrix)       override;
	virtual HRESULT put_BrushTransform(const Aggplus::CMatrix& oMatrix) override;

	virtual void put_BrushGradInfo(void* pGradInfo) override;

	// font settings
	virtual HRESULT get_FontName(std::wstring* wsName)       override;
	virtual HRESULT put_FontName(const std::wstring& wsName) override;

	virtual HRESULT get_FontPath(std::wstring* wsPath)       override;
	virtual HRESULT put_FontPath(const std::wstring& wsPath) override;

	virtual HRESULT get_FontSize(double* dSize)       override;
	virtual HRESULT put_FontSize(const double& dSize) override;

	virtual HRESULT get_FontStyle(LONG* lStyle)       override;
	virtual HRESULT put_FontStyle(const LONG& lStyle) override;

	virtual HRESULT get_FontStringGID(INT* bGid)       override;
	virtual HRESULT put_FontStringGID(const INT& bGid) override;

	virtual HRESULT get_FontCharSpace(double* dSpace)       override;
	virtual HRESULT put_FontCharSpace(const double& dSpace) override;

	virtual HRESULT get_FontFaceIndex(int* lFaceIndex)       override;
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex) override;

	// text commands
	virtual HRESULT CommandDrawTextCHAR(const LONG& lUnicode,
	                                    const double& dX,
	                                    const double& dY,
	                                    const double& dW,
	                                    const double& dH) override;

	virtual HRESULT CommandDrawTextExCHAR(const LONG& lUnicode,
	                                      const LONG& lGid,
	                                      const double& dX,
	                                      const double& dY,
	                                      const double& dW,
	                                      const double& dH) override;

	virtual HRESULT CommandDrawText(const std::wstring& wsUnicodeText,
	                                const double& dX,
	                                const double& dY,
	                                const double& dW,
	                                const double& dH) override;

	virtual HRESULT CommandDrawTextEx(const std::wstring& wsUnicodeText,
	                                  const unsigned int* pGids,
	                                  const unsigned int nGidsCount,
	                                  const double& dX,
	                                  const double& dY,
	                                  const double& dW,
	                                  const double& dH) override;

	// command type
	virtual HRESULT BeginCommand(const DWORD& lType) override;
	virtual HRESULT EndCommand(const DWORD& lType)   override;

	// graphic commands
	virtual HRESULT PathCommandMoveTo  (const double& dX, const double& dY) override;
	virtual HRESULT PathCommandLineTo  (const double& dX, const double& dY) override;
	virtual HRESULT PathCommandLinesTo (double* pPoints, const int& nCount) override;
	virtual HRESULT PathCommandCurveTo (const double& dX1,
	                                    const double& dY1,
	                                    const double& dX2,
	                                    const double& dY2,
	                                    const double& dXe,
	                                    const double& dYe) override;

	virtual HRESULT PathCommandCurvesTo(double* pPoints, const int& nCount) override;
	virtual HRESULT PathCommandArcTo  (const double& dX,
	                                   const double& dY,
	                                   const double& dW,
	                                   const double& dH,
	                                   const double& dStartAngle,
	                                   const double& dSweepAngle) override;

	virtual HRESULT PathCommandClose  ()                  override;
	virtual HRESULT PathCommandEnd    ()                  override;
	virtual HRESULT DrawPath          (const LONG& lType) override;
	virtual HRESULT PathCommandStart  ()                  override;

	virtual HRESULT PathCommandGetCurrentPoint(double* dX, double* dY) override;

	virtual HRESULT PathCommandTextCHAR(const LONG& lUnicode,
	                                    const double& dX,
	                                    const double& dY,
	                                    const double& dW,
	                                    const double& dH) override;

	virtual HRESULT PathCommandTextExCHAR(const LONG& lUnicode,
	                                      const LONG& lGid,
	                                      const double& dX,
	                                      const double& dY,
	                                      const double& dW,
	                                      const double& dH) override;

	virtual HRESULT PathCommandText(const std::wstring& wsUnicodeText,
	                                const double& dX,
	                                const double& dY,
	                                const double& dW,
	                                const double& dH) override;

	virtual HRESULT PathCommandTextEx (const std::wstring& wsUnicodeText,
	                                   const unsigned int* pGids,
	                                   const unsigned int nGidsCount,
	                                   const double& dX,
	                                   const double& dY,
	                                   const double& dW,
	                                   const double& dH) override;

	// image commands
	virtual HRESULT DrawImage(IGrObject* pImage,
	                          const double& dX,
	                          const double& dY,
	                          const double& dW,
	                          const double& dH) override;

	virtual HRESULT DrawImageFromFile(const std::wstring& wsImagePath,
	                                  const double& dX,
	                                  const double& dY,
	                                  const double& dW,
	                                  const double& dH,
	                                  const BYTE& nAlpha = 255) override;

	// transform commands
	virtual HRESULT SetTransform(const double& dM11,
	                             const double& dM12,
	                             const double& dM21,
	                             const double& dM22,
	                             const double& dX,
	                             const double& dY) override;

	virtual HRESULT GetTransform(double* dM11,
	                             double* dM12,
	                             double* dM21,
	                             double* dM22,
	                             double* dX,
	                             double* dY) override;

	virtual HRESULT ResetTransform() override;

	virtual HRESULT get_ClipMode(LONG* lMode)       override;
	virtual HRESULT put_ClipMode(const LONG& lMode) override;

	// advanced
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand)           override;
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)       override;
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand) override;

	virtual HRESULT IsSupportAdvancedCommand(const IAdvancedCommand::AdvancedCommandType& type) override;
	virtual HRESULT AdvancedCommand(IAdvancedCommand* command) override;

	// main method
	void Convert(IOfficeDrawingFile* pFile, const std::wstring& wsDstTxtFile = L"./output.txt");

private:
	class CTxtRendererImpl;
	std::unique_ptr<CTxtRendererImpl> m_pImpl;

	void DrawPage(IOfficeDrawingFile* pFile, int nPage);
};
