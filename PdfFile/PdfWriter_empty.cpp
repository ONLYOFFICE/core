#include "PdfWriter.h"

#ifdef BUILDING_WASM_MODULE
CPdfWriter::CPdfWriter(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA) : m_oCommandManager(this) {}
CPdfWriter::~CPdfWriter() {}
int CPdfWriter::SaveToFile(const std::wstring& wsPath) { return 0; }
void CPdfWriter::SetPassword(const std::wstring& wsPassword) {}
void CPdfWriter::SetDocumentID(const std::wstring& wsDocumentID) {}
void CPdfWriter::SetDocumentInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords) {}
HRESULT CPdfWriter::NewPage() { return 0; }
HRESULT CPdfWriter::get_Height(double* dHeight) { return 0; }
HRESULT CPdfWriter::put_Height(const double& dHeight) { return 0; }
HRESULT CPdfWriter::get_Width(double* dWidth) { return 0; }
HRESULT CPdfWriter::put_Width(const double& dWidth) { return 0; }
HRESULT CPdfWriter::get_PenColor(LONG* lColor) { return 0; }
HRESULT CPdfWriter::put_PenColor(const LONG& lColor) { return 0; }
HRESULT CPdfWriter::get_PenAlpha(LONG* lAlpha) { return 0; }
HRESULT CPdfWriter::put_PenAlpha(const LONG& lAlpha) { return 0; }
HRESULT CPdfWriter::get_PenSize(double* dSize) { return 0; }
HRESULT CPdfWriter::put_PenSize(const double& dSize) { return 0; }
HRESULT CPdfWriter::get_PenDashStyle(BYTE* nDashStyle) { return 0; }
HRESULT CPdfWriter::put_PenDashStyle(const BYTE& nDashStyle) { return 0; }
HRESULT CPdfWriter::get_PenLineStartCap(BYTE* nCapStyle) { return 0; }
HRESULT CPdfWriter::put_PenLineStartCap(const BYTE& nCapStyle) { return 0; }
HRESULT CPdfWriter::get_PenLineEndCap(BYTE* nCapStyle) { return 0; }
HRESULT CPdfWriter::put_PenLineEndCap(const BYTE& nCapStyle) { return 0; }
HRESULT CPdfWriter::get_PenLineJoin(BYTE* nJoinStyle) { return 0; }
HRESULT CPdfWriter::put_PenLineJoin(const BYTE& nJoinStyle) { return 0; }
HRESULT CPdfWriter::get_PenDashOffset(double* dOffset) { return 0; }
HRESULT CPdfWriter::put_PenDashOffset(const double& dOffset) { return 0; }
HRESULT CPdfWriter::get_PenAlign(LONG* lAlign) { return 0; }
HRESULT CPdfWriter::put_PenAlign(const LONG& lAlign) { return 0; }
HRESULT CPdfWriter::get_PenMiterLimit(double* dMiter) { return 0; }
HRESULT CPdfWriter::put_PenMiterLimit(const double& dMiter) { return 0; }
HRESULT CPdfWriter::PenDashPattern(double* pPattern, LONG lCount) { return 0; }
HRESULT CPdfWriter::get_BrushType(LONG* lType) { return 0; }
HRESULT CPdfWriter::put_BrushType(const LONG& lType) { return 0; }
HRESULT CPdfWriter::get_BrushColor1(LONG* lColor) { return 0; }
HRESULT CPdfWriter::put_BrushColor1(const LONG& lColor) { return 0; }
HRESULT CPdfWriter::get_BrushAlpha1(LONG* lAlpha) { return 0; }
HRESULT CPdfWriter::put_BrushAlpha1(const LONG& lAlpha) { return 0; }
HRESULT CPdfWriter::get_BrushColor2(LONG* lColor) { return 0; }
HRESULT CPdfWriter::put_BrushColor2(const LONG& lColor) { return 0; }
HRESULT CPdfWriter::get_BrushAlpha2(LONG* lAlpha) { return 0; }
HRESULT CPdfWriter::put_BrushAlpha2(const LONG& lAlpha) { return 0; }
HRESULT CPdfWriter::get_BrushTexturePath(std::wstring* wsPath) { return 0; }
HRESULT CPdfWriter::put_BrushTexturePath(const std::wstring& wsPath) { return 0; }
HRESULT CPdfWriter::get_BrushTextureMode(LONG* lMode) { return 0; }
HRESULT CPdfWriter::put_BrushTextureMode(const LONG& lMode) { return 0; }
HRESULT CPdfWriter::get_BrushTextureAlpha(LONG* lAlpha) { return 0; }
HRESULT CPdfWriter::put_BrushTextureAlpha(const LONG& lAlpha) { return 0; }
HRESULT CPdfWriter::get_BrushLinearAngle(double* dAngle) { return 0; }
HRESULT CPdfWriter::put_BrushLinearAngle(const double& dAngle) { return 0; }
HRESULT CPdfWriter::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight) { return 0; }
HRESULT CPdfWriter::put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount) { return 0; }
HRESULT CPdfWriter::get_FontName(std::wstring* wsName) { return 0; }
HRESULT CPdfWriter::put_FontName(const std::wstring& wsName) { return 0; }
HRESULT CPdfWriter::get_FontPath(std::wstring* wsPath) { return 0; }
HRESULT CPdfWriter::put_FontPath(const std::wstring& wsPath) { return 0; }
HRESULT CPdfWriter::get_FontSize(double* dSize) { return 0; }
HRESULT CPdfWriter::put_FontSize(const double& dSize) { return 0; }
HRESULT CPdfWriter::get_FontStyle(LONG* lStyle) { return 0; }
HRESULT CPdfWriter::put_FontStyle(const LONG& lStyle) { return 0; }
HRESULT CPdfWriter::get_FontStringGID(INT* bGid) { return 0; }
HRESULT CPdfWriter::put_FontStringGID(const INT& bGid) { return 0; }
HRESULT CPdfWriter::get_FontCharSpace(double* dSpace) { return 0; }
HRESULT CPdfWriter::put_FontCharSpace(const double& dSpace) { return 0; }
HRESULT CPdfWriter::get_FontFaceIndex(int* lFaceIndex) { return 0; }
HRESULT CPdfWriter::put_FontFaceIndex(const int& lFaceIndex) { return 0; }
HRESULT CPdfWriter::CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::CommandDrawTextCHAR2 (unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::EndCommand(const DWORD& lType, const LONG& lClipMode) { return 0; }
HRESULT CPdfWriter::PathCommandMoveTo(const double& dX, const double& dY) { return 0; }
HRESULT CPdfWriter::PathCommandLineTo(const double& dX, const double& dY) { return 0; }
HRESULT CPdfWriter::PathCommandLinesTo(double* pPoints, const int& nCount) { return 0; }
HRESULT CPdfWriter::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe) { return 0; }
HRESULT CPdfWriter::PathCommandCurvesTo(double* pPoints, const int& nCount) { return 0; }
HRESULT CPdfWriter::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle) { return 0; }
HRESULT CPdfWriter::PathCommandClose() { return 0; }
HRESULT CPdfWriter::PathCommandEnd() { return 0; }
HRESULT CPdfWriter::DrawPath(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const LONG& lType) { return 0; }
HRESULT CPdfWriter::PathCommandStart() { return 0; }
HRESULT CPdfWriter::PathCommandGetCurrentPoint(double* dX, double* dY) { return 0; }
HRESULT CPdfWriter::PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::DrawImageFromFile(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha) { return 0; }
HRESULT CPdfWriter::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY) { return 0; }
HRESULT CPdfWriter::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY) { return 0; }
HRESULT CPdfWriter::ResetTransform() { return 0; }
HRESULT CPdfWriter::AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip) { return 0; }
HRESULT CPdfWriter::AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage) { return 0; }
HRESULT CPdfWriter::AddFormField(NSFonts::IApplicationFonts* pAppFonts, IFormField* pInfo) { return 0; }
HRESULT CPdfWriter::DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
HRESULT CPdfWriter::EnableBrushRect(const LONG& lEnable) { return 0; }
HRESULT CPdfWriter::SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2) { return 0; }
HRESULT CPdfWriter::SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2) { return 0; }
HRESULT CPdfWriter::DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH) { return 0; }
bool CPdfWriter::EditPage(PdfWriter::CPage* pNewPage) { return false; }
bool CPdfWriter::AddPage(int nPageIndex) { return false; }
bool CPdfWriter::EditClose() { return false; }
void CPdfWriter::PageRotate(int nRotate) {}
void CPdfWriter::Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate) {}
//PdfWriter::CImageDict* CPdfWriter::LoadImage(Aggplus::CImage* pImage, const BYTE& nAlpha) { return NULL; }
bool CPdfWriter::DrawImage(Aggplus::CImage* pImage, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha) { return false; }
//bool CPdfWriter::DrawText(unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY) { return false; }
bool CPdfWriter::PathCommandDrawText(unsigned int* pUnicodes, unsigned int unLen, const double& dX, const double& dY, const unsigned int* pGids) { return false; }
void CPdfWriter::UpdateFont() {}
void CPdfWriter::GetFontPath(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic, std::wstring& wsFontPath, LONG& lFaceIndex) {}
PdfWriter::CFontCidTrueType* CPdfWriter::GetFont(const std::wstring& wsFontPath, const LONG& lFontIndex) { return NULL; }
PdfWriter::CFontCidTrueType* CPdfWriter::GetFont(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic) { return NULL; }
void CPdfWriter::UpdateTransform() {}
void CPdfWriter::UpdatePen() {}
void CPdfWriter::UpdateBrush(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory) {}
void CPdfWriter::Reset() {}
bool CPdfWriter::IsValid() { return false; }
bool CPdfWriter::IsPageValid() { return false; }
void CPdfWriter::SetError() {}
void CPdfWriter::AddLink(PdfWriter::CPage* pPage, const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const unsigned int& unDestPage) {}
unsigned char* CPdfWriter::EncodeString(const unsigned int* pUnicodes, const unsigned int& unUnicodesCount, const unsigned int* pGIDs) { return NULL; }
unsigned char* CPdfWriter::EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unUnicodesCount) { return NULL; }
std::wstring CPdfWriter::GetDownloadFile(const std::wstring& sUrl, const std::wstring& wsTempDirectory) { return std::wstring(); }

CCommandManager::CCommandManager(CPdfWriter* pRenderer) {}
CCommandManager::~CCommandManager() {}
CRendererTextCommand* CCommandManager::AddText(unsigned char* pCodes, unsigned int nLen, const double& dX, const double& dY) { return NULL; }
void CCommandManager::Flush() {}
void CCommandManager::Add(CRendererCommandBase* pCommand) {}
void CCommandManager::Clear() {}
void CBrushState::Reset() {}
#endif // BUILDING_WASM_MODULE
