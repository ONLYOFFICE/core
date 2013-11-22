#pragma once

#ifdef _DEBUG
//#define TME_DEBUG
//#define TME_DEBUG_VSXML
//#define TME_LOG(msg, offset) MeasurerLogger::log3(msg, offset)
//#define TME_SHOW_TABS
//#define TME_EMPTY_DRAWING
//#define TME_DONT_OUTPUT_HEADER_FOOTER
//#define TME_NODE_XML
//#define TME_DRAW_SYMBOL_BORDER
//#define TME_DRAW_LINE_BORDER
//#define TME_DRAW_LETTER_BORDER
//#define TME_TEXT_BLACK
//#define TME_RENDERER_COMMAND_DEBUG
#endif

#include <map>
#include "Structures.h"
#include "Wrap.h"

namespace ImageStudio {
	namespace Serialize {
		namespace Paint {
			namespace Common {
#include "Numeration.h"
#include "RendererCommands.h"
#include "Settings.h"

				class MeasurerLogger {
				public:
					static void log3(CString text, const int& offset) {
						log2(text, offset);
					}
					static void log3(float value, const int& offset) {
						CString text;
						text.Format(_T("%f\n"), value);
						log2(text, offset, false);
					}
					static void log3(int value, const int& offset) {
						CString text;
						text.Format(_T("%d\n"), value);
						log2(text, offset, false);
					}
				private:
					static void log2(CString text, const int& offset, const bool& applyLevel = true) {
						static int level = 0;
						if (text != _T("")) {
							if (applyLevel)
								for (int i = 0; i < level; ++i)
									text = _T("\t") + text;
							log(text);
						}
						level += offset;
					}

					static void log(const CString& text) {
						static const char logFilePath[] = "c:\\temp\\measurerLog.xml";
						FILE* logFile;
						fopen_s(&logFile, logFilePath, "a+");
						fwprintf_s(logFile, L"%s", text);
						fclose(logFile);
					}
				};
				class CDrawGraphicPath;
				class ITextPainter
				{
				public:
					virtual void	CommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, IAVSRenderer *pRenderer) = 0;
				};

				class IWorker
				{
					// Methods
				public:
					virtual void	IWorker_Resume()	= 0;
					virtual void	IWorker_Pause()	= 0;
					virtual void	IWorker_Stop()		= 0;
					// Events
				public:
					virtual void	IWorker_OnNewPage(double dWidthMm, double dHeightMm)	= 0;
					virtual void	IWorker_OnCompletePage()								= 0;

				public:
					virtual void	IWorker_OnProgress(double dCompleteness)	= 0;
				};

				class CTextMeasurer : public IWorker {
				protected:
					// ### ### ###
					static enum CAlignHorizontal {
						NoneAlign = 0, LeftAlign, CenterAlign, RightAlign, WideAlign
					};
					static enum CAlignVertical {
						NoneAlignVert = 0, FarAlignVert, CenterAlignVert, NearAlignVert
					};
					static enum CRelativeFrom {
						FromCharacter = 0, FromLine, FromColumn, FromParagraph,
						FromPage, FromMargin, FromInsideMargin, FromOutsideMargin,
						FromLeftMargin, FromRightMargin, 
						FromBottomMargin,  FromTopMargin
					};
					static enum CItemType {
						Symbols = 0, Space, LineEnd, TabStop, Bullet, Word, Alternate, Footnote, None
					};
					// ### ### ###
					class CDocumentManager;
					class CGraphicLine
					{
					public:
						REAL	m_fStartX;
						REAL	m_fStartY;

						REAL	m_fFinishX;
						REAL	m_fFinishY;

						CPen	m_oPen;

					public:
						// ---
						CGraphicLine()
							: m_fStartX(0.0f), m_fStartY(0.0f), 
							m_fFinishX(0.0f), m_fFinishY(0.0f)
						{
						}
						// ---
						void	Clear()
						{
							m_fStartX	= 0.0f;
							m_fStartY	= 0.0f;

							m_fFinishX	= 0.0f;
							m_fFinishY	= 0.0f;
						}
						// ---
						void	SetStartPoint(const REAL &fStartX, const REAL &fStartY)
						{
							m_fStartX	= fStartX;
							m_fStartY	= fStartY;
						}
						void	SetFinishPoint(const REAL &fFinishX, const REAL &fFinishY)
						{
							m_fFinishX	= fFinishX;
							m_fFinishY	= fFinishY;
						}
						void	SetPen(const CPen &oPen)
						{
							m_oPen	= oPen;
						}
						void	MoveOn(const REAL &fDeltaX, const REAL &fDeltaY)
						{
							m_fStartX	+= fDeltaX;
							m_fStartY	+= fDeltaY;

							m_fFinishX	+= fDeltaX;
							m_fFinishY	+= fDeltaY;
						}
						// ---
						void	Draw(IAVSRenderer *pRenderer, const REAL &fCompleteness)
						{							
							m_oPen.SetToRenderer(pRenderer);
							pRenderer->BeginCommand(c_nPathType);
							//pRenderer->BeginCommand(c_nTextGraphicType);
							pRenderer->PathCommandStart();
							pRenderer->PathCommandMoveTo(m_fStartX, m_fStartY);
							pRenderer->PathCommandLineTo(m_fFinishX, m_fFinishY);
							pRenderer->PathCommandClose();
							pRenderer->DrawPath(0x01);
							pRenderer->PathCommandEnd();
							//pRenderer->EndCommand(c_nTextGraphicType);
							pRenderer->EndCommand(c_nPathType);


							/*pRenderer->PathCommandStart();

							SAFEARRAYBOUND rgsab;
							rgsab.lLbound	= 0;
							rgsab.cElements	= 4;

							SAFEARRAY* pArray = SafeArrayCreate(VT_R4, 1, &rgsab);
							FLOAT* pBuffer = (FLOAT*)(pArray->pvData);

							memset(pBuffer, 0, rgsab.cElements * sizeof(float));

							const float fPtToMm	= 

							*pBuffer = m_fStartX;	++pBuffer;
							*pBuffer = m_fStartY;	++pBuffer;
							*pBuffer = m_fFinishX;	++pBuffer;
							*pBuffer = m_fFinishY;	++pBuffer;

							pRenderer->PathCommandLinesTo(pArray);

							RELEASEARRAY(pArray);

							pRenderer->PathCommandClose();
							pRenderer->DrawPath(0x01);
							pRenderer->PathCommandEnd();*/
						}
					};

					class CBackgroundRect
					{
					public:
						REAL	m_fX;
						REAL	m_fY;

						REAL	m_fWidth;
						REAL	m_fHeight;

						CBrush	m_oBrush;

					public:
						// ---
						CBackgroundRect()
							: m_fX(0.0f), m_fY(0.0f), 
							m_fWidth(0.0f), m_fHeight(0.0f)
						{
						}
						// ---
						void	Clear()
						{
							m_fX	= 0.0f;
							m_fY	= 0.0f;

							m_fWidth	= 0.0f;
							m_fHeight	= 0.0f;
						}
						// ---
						void	SetStartPoint(const REAL &fStartX, const REAL &fStartY)
						{
							m_fX	= fStartX;
							m_fY	= fStartY;
						}
						void	SetSize(const REAL &fWidth, const REAL &fHeight)
						{
							m_fWidth	= fWidth;
							m_fHeight	= fHeight;
						}
						void	SetBrush(const CBrush &oBrush)
						{
							m_oBrush	= oBrush;
						}						
						void	MoveOn(const REAL &fDeltaX, const REAL &fDeltaY)
						{
							m_fX	+= fDeltaX;
							m_fY	+= fDeltaY;
						}
						// ---
						void	Draw(IAVSRenderer *pRenderer, const REAL &fCompleteness)
						{
							if (m_oBrush.m_oBrush.Alpha1 != 0)
							{
								pRenderer->put_PenColor(m_oBrush.m_oBrush.Color1);
								pRenderer->put_PenSize(0);

								/*pRenderer->PathCommandStart();
								pRenderer->PathCommandMoveTo(m_fX + m_fWidth, m_fY);
								pRenderer->PathCommandLineTo(m_fX + m_fWidth, m_fY + m_fHeight);
								pRenderer->PathCommandLineTo(m_fX, m_fY + m_fHeight);
								pRenderer->DrawPath(c_nStroke);
								pRenderer->PathCommandEnd();*/

								m_oBrush.SetToRenderer(pRenderer);

								pRenderer->BeginCommand(c_nPathType);
								//pRenderer->BeginCommand(c_nTextGraphicType);
								pRenderer->PathCommandStart();
								pRenderer->PathCommandMoveTo(m_fX, m_fY);
								pRenderer->PathCommandLineTo(m_fX + m_fWidth, m_fY);
								pRenderer->PathCommandLineTo(m_fX + m_fWidth, m_fY + m_fHeight);
								pRenderer->PathCommandLineTo(m_fX, m_fY + m_fHeight);
								pRenderer->PathCommandClose();
								pRenderer->DrawPath(c_nWindingFillMode);
								pRenderer->PathCommandEnd();
								//pRenderer->EndCommand(c_nTextGraphicType);
								pRenderer->EndCommand(c_nPathType);
							}
						}
					};


					class CBulletProperties
					{
					public:
						CString	m_strChar;

						REAL	m_fSize;

						LONG	m_iColor;

					public:
						// ---
						CBulletProperties()
							: m_strChar(_T("")), 
							m_fSize(1.0f), 
							m_iColor(0)
						{
						}
						// ---
						~CBulletProperties()
						{
							Clear();
						}
						// ---
						void	Clear()
						{
							m_strChar	= _T("");

							m_fSize		= 1.0f;

							m_iColor	= 0;
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							m_strChar	= oXmlNode.GetAttributeOrValue(_T("bulletchar"), _T(""));

							m_fSize		= oXmlNode.ReadAttributeOrValueFloat(_T("bulletsize"), 100) / 100.0f;
							if (m_fSize <= 0)
							{
								m_fSize	= 1.0f;
							}

							m_iColor	= oXmlNode.ReadAttributeOrValueInt(_T("bulletcolor"), 0);
						}
					};

					class CLinkProperties
					{
					public:
						BYTE	nMode;
						CString	strTarget;

					public:
						// ---
						CLinkProperties()
							: nMode(0), strTarget(_T(""))
						{
						}
						// ---
						void	Clear()
						{
							nMode		= 0;
							strTarget	= _T("");
						}
						// ---
						void	FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
						{
							strTarget	= oXmlNode.GetAttribute(_T("external-destination"), _T(""));
							if (strTarget != _T(""))
							{
								nMode	= 2;
							}
							else
							{
								strTarget	= oXmlNode.GetAttribute(_T("internal-destination"), _T(""));
								if (strTarget != _T(""))
								{
									nMode	= 1;
								}
							}
						}
					};

					class CBreakProperties : protected CXmlAttributeReader
					{
					public:
						int	m_nBreakBeforeType;

					public:
						CBreakProperties() 
							: m_nBreakBeforeType(0)
						{
						}
						void	Clear()
						{
							m_nBreakBeforeType	= 0;
						}
					public:
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{
							CString strDefaultValue;

							strDefaultValue.Format(_T("%s"), GetBreakType(m_nBreakBeforeType));
							SetBreakType(m_nBreakBeforeType, oXmlNode.GetAttributeOrValue(_T("break-before"), strDefaultValue));
						}
					};

					class CBorderProperties : private CXmlAttributeReader
					{
					public:
						class CSide
						{
						public:
							CPen	m_oPen;

						public:
							CSide()
								: m_oPen()
							{
							}
						public:
							CGraphicLine*	GetGraphicLine(const REAL &fStartX, const REAL &fStartY, const REAL &fFinishX, const REAL &fFinishY)
							{
								CGraphicLine *pGraphicLine = new CGraphicLine();
								pGraphicLine->SetPen(m_oPen);
								pGraphicLine->SetStartPoint(fStartX * (25.4f / 72.0f), fStartY * (25.4f / 72.0f));
								pGraphicLine->SetFinishPoint(fFinishX * (25.4f / 72.0f), fFinishY * (25.4f / 72.0f));

								return pGraphicLine;
							}
						};
					public:
						CSide	*m_pLeftSide;
						CSide	*m_pRightSide;
						CSide	*m_pTopSide;
						CSide	*m_pBottomSide;

					public:
						CBorderProperties()
							: m_pLeftSide(NULL), m_pRightSide(NULL), 
							m_pTopSide(NULL), m_pBottomSide(NULL)
						{
						}
						CBorderProperties& operator=(const CBorderProperties& oSrc)
						{
							if (oSrc.m_pLeftSide != NULL)
							{
								m_pLeftSide		= new CSide();
								*m_pLeftSide	= *oSrc.m_pLeftSide;
							}
							if (oSrc.m_pRightSide != NULL)
							{
								m_pRightSide	= new CSide();
								*m_pRightSide	= *oSrc.m_pRightSide;
							}
							if (oSrc.m_pTopSide != NULL)
							{
								m_pTopSide	= new CSide();
								*m_pTopSide	= *oSrc.m_pTopSide;
							}
							if (oSrc.m_pBottomSide != NULL)
							{
								m_pBottomSide	= new CSide();
								*m_pBottomSide	= *oSrc.m_pBottomSide;
							}

							return *this;
						}
						int GetBorderType()
						{
							int res = 0;
							if (m_pLeftSide != NULL)
								res += 1;
							if (m_pRightSide != NULL)
								res += 2;
							if (m_pTopSide != NULL)
								res += 4;
							if (m_pBottomSide != NULL)
								res += 8;
							return res;
						}
					public:
						void	Clear()
						{
							RELEASEOBJECT(m_pLeftSide);
							RELEASEOBJECT(m_pRightSide);
							RELEASEOBJECT(m_pTopSide);
							RELEASEOBJECT(m_pBottomSide);
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{
							LoadSide(oXmlNode, _T("left"), &m_pLeftSide);
							LoadSide(oXmlNode, _T("right"), &m_pRightSide);
							LoadSide(oXmlNode, _T("top"), &m_pTopSide);
							LoadSide(oXmlNode, _T("bottom"), &m_pBottomSide);
						}
					private:
						void	LoadSide(XmlUtils::CXmlNode &oXmlNode, const CString &strSideName, CSide** ppSide)
						{
							CSide* pSide	= *ppSide;
							CString strStyle	= oXmlNode.GetAttributeOrValue(_T("border-") + strSideName + _T("-style"), _T(""));

							if (strStyle != _T(""))
							{
								if (strStyle == _T("none"))
								{
									RELEASEOBJECT(pSide);
									*ppSide	= NULL;
								}
								else
								{
									if (pSide == NULL)
									{
										*ppSide	= new CSide();
										pSide	= *ppSide;
									}

									CXmlAttributeReader::SetPenStyle(pSide->m_oPen.m_oPen, strStyle);

									REAL fWidth	= oXmlNode.ReadAttributeOrValueFloat(_T("border-") + strSideName + _T("-width"), -1);
									if (fWidth > 0)
									{
										if (fWidth > 6)
										{
											fWidth	= 1;
										}
										pSide->m_oPen.m_oPen.Size = fWidth * 0.3528f;
									}
									else
									{
										RELEASEOBJECT(pSide);
										*ppSide	= NULL;
										return;
									}

									LONG iColor	= CXmlAttributeReader::LongAttributeHexFromXml(oXmlNode, _T("border-") + strSideName + _T("-color"), -1);
									if (iColor >= 0)
									{
										if (iColor != 0xFFFFFF)
											pSide->m_oPen.m_oPen.Color = BGRToRGB(iColor);
										else 
											pSide->m_oPen.m_oPen.Color = 0xFFFFFF;
									}
								}
							}
						}
					};
					class CBackgroundProperties : private CXmlAttributeReader
					{
					public:
						CBackgroundRect*	m_pRect;
						CBrush				m_oBrush;	 

					public:
						CBackgroundProperties()
							: m_pRect(NULL)
						{
						}
						CBackgroundProperties& operator=(const CBackgroundProperties& oSrc)
						{
							if (oSrc.m_pRect != NULL)
							{
								m_pRect		= new CBackgroundRect();
								*m_pRect	= *oSrc.m_pRect;
							}

							return *this;
						}
						CBackgroundRect*	GetBackgroundRect(const REAL &fStartX, const REAL &fStartY, const REAL &fWidth, const REAL &fHeight)
						{
							CBackgroundRect *pBackgroundRect	= new CBackgroundRect();
							pBackgroundRect->SetBrush(m_oBrush);
							pBackgroundRect->SetStartPoint(fStartX * (25.4f / 72.0f), fStartY * (25.4f / 72.0f));
							pBackgroundRect->SetSize(fWidth * (25.4f / 72.0f), fHeight * (25.4f / 72.0f));

							return pBackgroundRect;
						}
						CBackgroundRect*	GetBackgroundRect2(const REAL &fStartX, const REAL &fStartY, const REAL &fWidth, const REAL &fHeight)
						{
							CBackgroundRect *pBackgroundRect	= new CBackgroundRect();
							pBackgroundRect->SetBrush(m_oBrush);
							pBackgroundRect->SetStartPoint(fStartX , fStartY);
							pBackgroundRect->SetSize(fWidth, fHeight);

							return pBackgroundRect;
						}
					public:
						void	Clear()
						{
							RELEASEOBJECT(m_pRect);
						}
						void	PageBGFromXml(XmlUtils::CXmlNode & oXmlNode)
						{	
							LONG iColor	= CXmlAttributeReader::LongAttributeHexFromXml(oXmlNode, _T("w:color"), -1);
							if (iColor >= 0 && iColor != 0x00FFFFFF)
							{		
								if (m_pRect == NULL)
								{
									CBackgroundRect* ppRect	= new CBackgroundRect();
									m_pRect	= ppRect;
								}	
								m_oBrush.m_oBrush.Color1 = BGRToRGB(iColor);
								m_oBrush.m_oBrush.Alpha1 = 255;
							}
							else
							{
								m_oBrush.m_oBrush.Alpha1 = 0;
							}
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{							
							LONG iColor	= CXmlAttributeReader::LongAttributeHexFromXml(oXmlNode, _T("background-color"), -1);
							if (iColor >= 0)
							{
								if (m_pRect == NULL)
								{
									CBackgroundRect* ppRect	= new CBackgroundRect();
									m_pRect	= ppRect;
								}			
								if (iColor != 0x00FFFFFF)
									m_oBrush.m_oBrush.Color1 = BGRToRGB(iColor);
								else
									m_oBrush.m_oBrush.Color1 = 0xFFFFFF;
								m_oBrush.m_oBrush.Alpha1 = 255;
							}
							else
							{
								m_oBrush.m_oBrush.Alpha1 = 0;
							}
						}
						bool HasColor()
						{
							//if ()
							return !(m_pRect->m_oBrush.m_oBrush.Alpha1 == 0);
						}
					};
					class CPaddingProperties : private CXmlAttributeReader
					{
					public:
						REAL	m_fPaddingLeft;
						REAL	m_fPaddingRight;
						REAL	m_fPaddingTop;
						REAL	m_fPaddingBottom;

					public:
						// ---
						CPaddingProperties()
							: m_fPaddingLeft(1.8f), m_fPaddingRight(1.8f), 
							m_fPaddingTop(1.8f), m_fPaddingBottom(1.8f)
						{
						}
						// ---
						void	Clear()
						{
							m_fPaddingLeft		= 18.0f;
							m_fPaddingRight		= 18.0f;
							m_fPaddingTop		= 18.0f;
							m_fPaddingBottom	= 18.0f;
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							m_fPaddingLeft		= FloatPtAttributeFromXml(oXmlNode, _T("padding-left"), m_fPaddingLeft);
							if (m_fPaddingLeft < 0)
								m_fPaddingLeft	= 1.8f;
							m_fPaddingRight		= FloatPtAttributeFromXml(oXmlNode, _T("padding-right"), m_fPaddingRight);
							if (m_fPaddingLeft < 0)
								m_fPaddingLeft	= 1.8f;
							m_fPaddingTop		= FloatPtAttributeFromXml(oXmlNode, _T("padding-top"), m_fPaddingTop);
							if (m_fPaddingLeft < 0)
								m_fPaddingLeft	= 1.8f;
							m_fPaddingBottom	= FloatPtAttributeFromXml(oXmlNode, _T("padding-bottom"), m_fPaddingBottom);
							if (m_fPaddingLeft < 0)
								m_fPaddingLeft	= 1.8f;
						}

					};
					class CMarginProperties : protected CXmlAttributeReader
					{
					public:
						REAL	m_nSpaceBefore;
						REAL	m_nSpaceAfter;
						REAL	m_nLineSpacing; //  xsl-fo     line-height.minimum
						REAL	m_fLineHeight;

						REAL	m_fStartIndent;
						REAL	m_fEndIndent;
						REAL	m_fTextIndent;

					public:
						CMarginProperties() 
							: m_nSpaceBefore(0.0f), m_nSpaceAfter(0.0f), m_nLineSpacing(0.0f), m_fLineHeight(0.0f), 
							m_fStartIndent(0.0f), m_fEndIndent(0.0f), m_fTextIndent(0.0f)
						{
						}
					public:
						void	Clear()
						{
							m_nSpaceBefore		= 0;
							m_nSpaceAfter		= 0;
							m_nLineSpacing		= 0;
							m_fLineHeight		= 0;

							m_fStartIndent		= 0;
							m_fEndIndent		= 0;
							m_fTextIndent		= 0;
						}
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode, CDocumentManager& documentManager)
						{
							m_nLineSpacing	= -oXmlNode.ReadAttributeOrValueFloat(_T("linespacing"), 0);
							m_nSpaceBefore	= -oXmlNode.ReadAttributeOrValueFloat(_T("spacebefore"), 0);
							m_nSpaceAfter	= -oXmlNode.ReadAttributeOrValueFloat(_T("spaceafter"), 0);

							m_fStartIndent	= oXmlNode.ReadAttributeOrValueFloat(_T("leftmargin"), 0)	* documentManager.m_ItemManager.GetMultiplierX();
							m_fEndIndent	= oXmlNode.ReadAttributeOrValueFloat(_T("rightmargin"), 0)	* documentManager.m_ItemManager.GetMultiplierX();
							m_fTextIndent	= oXmlNode.ReadAttributeOrValueFloat(_T("indent"), 0)		* documentManager.m_ItemManager.GetMultiplierX();
							if (m_fStartIndent + m_fTextIndent < 0)
							{
								m_fTextIndent	= -m_fStartIndent;
							}
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode, const bool& bAndVerctical = true)
						{
							m_nSpaceBefore	= FloatPtAttributeFromXml(oXmlNode, _T("space-before"), m_nSpaceBefore);
							m_nSpaceAfter	= FloatPtAttributeFromXml(oXmlNode, _T("space-after"), m_nSpaceAfter);
							m_fLineHeight	= FloatPtAttributeFromXml(oXmlNode, _T("line-height"), m_fLineHeight);
							if (m_fLineHeight < -4.0f)
							{
								m_fLineHeight	= -1.0f;
							}

							m_nLineSpacing	= FloatPtAttributeFromXml(oXmlNode, _T("line-height.minimum"), 0);
							if (m_nLineSpacing < -4.0f)
							{
								m_nLineSpacing	= -1.0f;
							}

							if (bAndVerctical)
							{
								m_fStartIndent	= FloatPtAttributeFromXml(oXmlNode, _T("start-indent"), m_fStartIndent);
								m_fEndIndent	= FloatPtAttributeFromXml(oXmlNode, _T("end-indent"), m_fEndIndent);
								m_fTextIndent	= FloatPtAttributeFromXml(oXmlNode, _T("text-indent"), m_fTextIndent);
							}
						}
					};
					class CTabParams: CXmlAttributeReader
					{
					private:
						float	m_fTabPosition;
						byte	m_bLeader;
						byte	m_bType;
					public:
						CTabParams()
							: m_fTabPosition(0),
							m_bLeader(0),
							m_bType(0)
						{}
						CTabParams(float fPos, byte bLeader, byte bType)
							: m_fTabPosition(fPos),
							m_bLeader(bLeader),
							m_bType(bType)
						{}
						CTabParams(float fPos, CString strLeader, CString strType)
							: m_fTabPosition(fPos)
						{
							m_bLeader	= SetLeaderTab(strLeader);
							m_bType		= SetTypeTab(strType);
						}
						~CTabParams()
						{}
						friend bool operator>(const CTabParams& a, const CTabParams& b)
						{
							return (a.m_fTabPosition > b.m_fTabPosition) ? true : false;
						}
						friend bool operator<(const CTabParams& a, const CTabParams& b)
						{
							return (a.m_fTabPosition < b.m_fTabPosition) ? true : false;
						}
						friend bool operator>=(const CTabParams& a, const CTabParams& b)
						{
							return (a.m_fTabPosition >= b.m_fTabPosition) ? true : false;
						}
						friend bool operator<=(const CTabParams& a, const CTabParams& b)
						{
							return (a.m_fTabPosition <= b.m_fTabPosition) ? true : false;
						}
					public:
						float GetPosition()
						{
							return m_fTabPosition;
						}
						void SetPosition(float fPos)
						{
							m_fTabPosition = fPos;
						}
						byte GetLeader()
						{
							return m_bLeader;
						}
						void SetLeader(byte bLeader)
						{
							m_bLeader = bLeader;
						}
						byte GetType()
						{
							return m_bType;
						}
						void SetType(byte bType)
						{
							m_bType = bType;
						}

					};
					class CTabProperties : protected CXmlAttributeReader
					{
					public:
						CArray<CTabParams> m_arrTabs;
						float			m_fDefaultTabSize;

					public:
						CTabProperties()
							: m_arrTabs(), m_fDefaultTabSize(0.0f) 
						{
						}
					public:
						void	Clear()
						{
							m_arrTabs.RemoveAll();
							m_fDefaultTabSize	= 0.0f;
						}
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							m_fDefaultTabSize	= (REAL)oXmlNode.ReadAttributeOrValueInt(_T("defaulttabsize"), 0) * 72.0f / (36000.0f*25.4f);

							if (m_fDefaultTabSize < 0)
								m_fDefaultTabSize = -m_fDefaultTabSize;
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{
							m_fDefaultTabSize = FloatPtAttributeFromXml(oXmlNode, _T("default-tab-size"), 0);
							XmlUtils::CXmlNodes oTabsNodes;
							XmlUtils::CXmlNode oTabsNode;
							if (oXmlNode.GetNodes(_T("fo:tabs"), oTabsNodes))
							{
								for(int i = 0; i < oTabsNodes.GetCount(); i++ )
									if (oTabsNodes.GetAt(i, oTabsNode))
									{
										XmlUtils::CXmlNodes oTabNodes;
										oTabsNode.GetNodes(_T("fo:tab"), oTabNodes);
										int iCount = oTabNodes.GetCount();
										for (int iIndex = 0; iIndex < iCount; ++iIndex)
										{
											XmlUtils::CXmlNode oNode;
											oTabNodes.GetAt(iIndex, oNode);
											float fTabPosition = oNode.ReadAttributeOrValueFloat(_T("position"), -1);
											if (fTabPosition >= 0.0f)
											{												
												CString tabLeader = oNode.GetAttributeOrValue(_T("leader"));
												CString tabType   = oNode.GetAttributeOrValue(_T("val"), _T("left"));
												m_arrTabs.Add(CTabParams(fTabPosition, tabLeader, tabType));
											}
										}
									}
									m_arrTabs.Sort();
									Replace();
							}
						}
						//   left,    num,    
						void Replace()
						{
							size_t nSize = m_arrTabs.GetCount();
							bool hasLeftInThisPosition = false;
							for(size_t i = 0; i < nSize; i++)
							{
								if (m_arrTabs[i].GetType() == 0)
									hasLeftInThisPosition = true;
								if (m_arrTabs[i].GetType() == 2)
								{
									if (i>0 && m_arrTabs[i].GetPosition() == m_arrTabs[i - 1].GetPosition() && hasLeftInThisPosition == true)
									{
										m_arrTabs[i].SetType(0);
									}
									else
									{
										size_t j = i;
										while (j < nSize - 1 && m_arrTabs[j].GetPosition() == m_arrTabs[j + 1].GetPosition() )
										{											
											j++;
											if (m_arrTabs[j].GetType() == 0)
											{
												m_arrTabs[i].SetType(0);
												break;
											}
										}
									}
								}
							}
						}						
					};
					class CBlockAttributes : public CMarginProperties, public CBreakProperties, public CTabProperties, public CBorderProperties, public CBackgroundProperties
					{
					public:
						BOOL	m_bIsFirst;

						LONG	m_lCount;

						WORD	m_nTextAlignment;

						WORD	m_nWrapFlags;
						BYTE	m_nKeepTogether;
						BYTE	m_nKeepNext;
						bool _oneLine;

						CString				m_pStyle;
						CString				m_contextualSpacing;

						CBulletProperties	*m_pBulletProperties;

					public:
						CBlockAttributes() 
							: CMarginProperties(), 
							CBreakProperties(), 
							m_bIsFirst(FALSE), 
							m_lCount(0), 
							m_nTextAlignment(0), 
							m_nWrapFlags(0), m_nKeepTogether(0), m_nKeepNext(0), 
							m_pBulletProperties(NULL), m_pStyle(), m_contextualSpacing(), 
							_oneLine(false)
						{
						}						
						~CBlockAttributes()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							CMarginProperties::Clear();
							CBreakProperties::Clear();
							CTabProperties::Clear();

							m_lCount			= 0;

							m_nTextAlignment	= 0;

							m_nWrapFlags		= 1;
							m_nKeepTogether		= 0;
							m_nKeepNext			= 0;

							RELEASEOBJECT(m_pBulletProperties);
							_oneLine = false;
						}
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode, CDocumentManager& documentManager)
						{
							m_lCount	= oXmlNode.ReadAttributeOrValueInt(_T("count"), 0);

							int nTextAlignment	= (WORD)oXmlNode.ReadAttributeOrValueInt(_T("textalignment"), -1);
							if (nTextAlignment >= 0)
							{
								m_nTextAlignment	= nTextAlignment;
							}

							m_nWrapFlags	= (WORD)oXmlNode.ReadAttributeOrValueInt(_T("wrapflags"), 0);

							WORD nBulletFlag	= (WORD)oXmlNode.ReadAttributeOrValueInt(_T("bulletflag"), 0);
							if (nBulletFlag & 1)
							{
								RELEASEOBJECT(m_pBulletProperties);
								m_pBulletProperties	= new CBulletProperties();
								m_pBulletProperties->FromXmlNode(oXmlNode);
							}

							CMarginProperties::FromXmlNode(oXmlNode, documentManager);
							CTabProperties::FromXmlNode(oXmlNode);

							if (oXmlNode.GetAttribute(_T("oneline")) == _T("1"))
								_oneLine = true;
							else
								_oneLine = false;
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode, const bool &bMarginSet = true)
						{
							CMarginProperties::FromXmlNode2(oXmlNode, bMarginSet);
							m_arrTabs.Add(CTabParams((float)m_fStartIndent - 0.001, _T("leader"), _T("num")));

							CBreakProperties::FromXmlNode2(oXmlNode);
							CTabProperties::FromXmlNode2(oXmlNode);
							CBorderProperties::FromXmlNode2(oXmlNode);
							CBackgroundProperties::FromXmlNode2(oXmlNode);

							m_pStyle			= oXmlNode.GetAttributeOrValue(_T("fo:pStyle"), _T("usualStyle"));
							m_contextualSpacing = oXmlNode.GetAttributeOrValue(_T("fo:contextualSpacing"), _T("false"));

							CString strDefaultValue;
							strDefaultValue.Format(_T("%s"), GetTextAlignment(m_nTextAlignment));
							SetTextAlignment(m_nTextAlignment, oXmlNode.GetAttributeOrValue(_T("text-align"), strDefaultValue));
							SetKeepTogether(m_nKeepTogether, oXmlNode.GetAttributeOrValue(_T("keep-together.within-page")));
							SetKeepNext(m_nKeepNext, oXmlNode.GetAttributeOrValue(_T("keep-with-next")));
						}
					};

					class CBlockFrameAttributes : private CXmlAttributeReader
					{
					public:
						CString m_dropCap;
						int		m_lines;

					public:
						double		m_vSpace;
						double		m_hSpace;

					public:
						CString		m_vAnchor;
						CString		m_hAnchor;
						CString		m_xAlign;
						CString		m_yAlign;

					public:
						double		m_x;
						double		m_y;
						double		m_width;
						double		m_height;

					public:
						CString m_wrap;
					public:
						CBlockFrameAttributes() 
							: m_dropCap(_T("")), m_lines(0), m_vSpace(0), m_hSpace(0), m_vAnchor(_T("")),
							m_hAnchor(_T("")), m_wrap(_T("")), m_x(-1), m_y(-1), m_width(0), m_height(0)
						{
						}
					public:
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
#ifdef AVS_LOG_DEBUG
							CString p = oXmlNode.GetXml();
#endif
							m_dropCap	= oXmlNode.GetAttributeOrValue(_T("w:dropCap"), _T(""));
							m_lines		= oXmlNode.ReadAttributeOrValueInt(_T("w:lines"), 0);

							m_vSpace	= oXmlNode.ReadAttributeOrValueDouble(_T("w:vSpace"), 0) / 20;
							m_hSpace	= oXmlNode.ReadAttributeOrValueDouble(_T("w:hSpace"), 0) / 20;

							m_vAnchor	= oXmlNode.GetAttributeOrValue(_T("w:vAnchor"), _T(""));
							m_hAnchor	= oXmlNode.GetAttributeOrValue(_T("w:hAnchor"), _T(""));

							m_xAlign	= oXmlNode.GetAttributeOrValue(_T("w:xAlign"), _T(""));
							m_yAlign	= oXmlNode.GetAttributeOrValue(_T("w:yAlign"), _T(""));

							m_x			= oXmlNode.ReadAttributeOrValueDouble(_T("w:x"), -1)/20;
							m_y			= oXmlNode.ReadAttributeOrValueDouble(_T("w:y"), -1)/20;

							m_width		= oXmlNode.ReadAttributeOrValueDouble(_T("w:w"), 0)/20;
							m_height	= oXmlNode.ReadAttributeOrValueDouble(_T("w:h"), 0)/20;

							m_wrap		= oXmlNode.GetAttributeOrValue(_T("w:wrap"), _T(""));
						}						
					};
					class CListBlockAttributes : public CBreakProperties
					{
					public:
						CListBlockAttributes() 
							: CBreakProperties()
						{
						}						
						~CListBlockAttributes()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							CBreakProperties::Clear();
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{
							XmlUtils::CXmlNode oItemNode;
							oXmlNode.GetNode(_T("fo:list-item"), oItemNode);
							CBreakProperties::FromXmlNode2(oItemNode);
						}
					};

					class CTextAttributes : private CXmlAttributeReader
					{
					public:
						enum ETextTransform {
							NoneTransform = 0, CapsTransform, SmallCapsTransform
						};

					public:
						CFont		m_oFont;
						CString		m_FontFamilyEA;
						CString		_fontFamilyCS;

						CBrush		m_oBrush;
						CShadow		m_oShadow;
						CBackground m_oBackground;
						CEdge		m_oEdge;

						WORD	m_nTextAlignmentVertical;
						WORD	m_nTextAlignmentHorizontal;
						BOOL	m_bAntiAliasText;

						WORD	m_nBaselineShift;
						REAL	m_fBaselineShift;
					public:
						CTextAttributes() : m_oFont(), m_FontFamilyEA(_T("")), _fontFamilyCS(_T("")),
							m_oBrush(), m_oShadow(), m_oBackground(), m_oEdge(),
							m_nTextAlignmentVertical(0), m_nTextAlignmentHorizontal(0),
							m_bAntiAliasText(TRUE), 
							m_nBaselineShift(0), m_fBaselineShift(0.0f), 
							m_TextTransform(NoneTransform)
						{
						}
						CTextAttributes&	operator=(const CTextAttributes& oSrc)
						{
							m_oFont			= oSrc.m_oFont;
							m_FontFamilyEA	= oSrc.m_FontFamilyEA;
							_fontFamilyCS	= oSrc._fontFamilyCS;

							m_oBrush		= oSrc.m_oBrush;
							m_oShadow		= oSrc.m_oShadow;
							m_oBackground	= oSrc.m_oBackground;
							m_oEdge			= oSrc.m_oEdge;

							m_nTextAlignmentVertical	= oSrc.m_nTextAlignmentVertical;
							m_nTextAlignmentHorizontal	= oSrc.m_nTextAlignmentHorizontal;
							m_bAntiAliasText			= oSrc.m_bAntiAliasText;

							m_nBaselineShift	= oSrc.m_nBaselineShift;
							m_fBaselineShift	= oSrc.m_fBaselineShift;

							m_TextTransform = oSrc.m_TextTransform;

							return *this;
						}
					public:
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							m_oFont.FromXmlNode(oXmlNode);
							m_oBrush.FromXmlNode(oXmlNode);
							m_oShadow.FromXmlNode(oXmlNode);
							m_oBackground.FromXmlNode(oXmlNode);
							m_oEdge.FromXmlNode(oXmlNode);

							m_nTextAlignmentVertical	= 1 + (WORD)oXmlNode.ReadAttributeOrValueInt(_T("font-stringalignmentvertical"), 0);
							m_nTextAlignmentHorizontal	= (WORD)oXmlNode.ReadAttributeOrValueInt(_T("font-stringalignmenthorizontal"), 0);
							m_bAntiAliasText			= (BOOL)oXmlNode.ReadAttributeOrValueInt(_T("font-antialiastext"), -1);

							m_fBaselineShift			= oXmlNode.ReadAttributeOrValueFloat(_T("baseline-shift"), 0) / 100.0f;
							if (m_fBaselineShift != 0)
							{
								m_fBaselineShift	*= (REAL)m_oFont.m_oFont.Size;
								m_oFont.m_oFont.Size = m_oFont.m_oFont.Size * 0.667f;
							}
						}						
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{
							FromXmlAttributes(oXmlNode);
						}
						void	FromXmlNode3(XmlUtils::CXmlNode &oXmlNode, bool IsInTOC)
						{
							if (IsInTOC == false)
							{
								XmlUtils::CXmlNode oXmlComplete;
								oXmlNode.GetNode(_T("style-complete"), oXmlComplete);
								FromXmlAttributes(oXmlComplete);
							}
							else
							{
								XmlUtils::CXmlNode oXmlReduce;
								oXmlNode.GetNode(_T("style-reduce"), oXmlReduce);
								FromXmlAttributes(oXmlReduce);
							}
						}
					public:
						const ETextTransform&	TextTransform() const
						{
							return m_TextTransform;
						}
					public:
						void	ProcessFontProperties(XmlUtils::CXmlNode& character, CDocumentManager& documentManager)
						{
							XmlUtils::CXmlNode fontProperties;
							if (character.GetNode(_T("FontProperties"), fontProperties))
								documentManager.AddFont(m_oFont, fontProperties.GetXml());
						}



					private:
						ETextTransform	m_TextTransform;
					private:
						void	FromXmlAttributes(XmlUtils::CXmlNode &oXmlNode)
						{
							XML::IXMLDOMNamedNodeMapPtr pAttributes = oXmlNode.GetAttributes();
							if (NULL == pAttributes)
								return;

							m_oFont.FromXmlAttributes(pAttributes);
							BSTR value = GetAttribute(pAttributes, g_cpszXML_Font_Family_EA);
							if (value != NULL)
							{
								m_FontFamilyEA = value;
								SysFreeString(value);
							}

							value = GetAttribute(pAttributes, g_cpszXML_Font_Family_CS);
							if (value != NULL)
							{
								_fontFamilyCS = value;
								SysFreeString(value);
							}

							m_oBrush.FromXmlAttributes(pAttributes);
							m_oShadow.FromXmlAttributes(pAttributes);
							m_oBackground.FromXmlAttributes(pAttributes);

							BSTR bsValue = GetAttribute(pAttributes, g_cpszXML_Text_Align);
							if (NULL != bsValue)
							{
								m_nTextAlignmentHorizontal = (WORD)XmlUtils::GetInteger(bsValue);
								SysFreeString(bsValue);
							}
							else
								m_nTextAlignmentHorizontal = 0;

							bsValue = GetAttribute(pAttributes, g_cpszXML_Text_BaselineShift);
							if (NULL != bsValue)
							{
								CString strValue = (CString)bsValue;
								SetBaselineShift(m_nBaselineShift, m_fBaselineShift, strValue);

								SysFreeString(bsValue);
							}

							if (m_nBaselineShift != 0)
								m_oFont.m_oFont.Size = m_oFont.m_oFont.Size * 0.667f;

							value = GetAttribute(pAttributes, g_cpszXML_Text_Transform);
							if (value != NULL)
							{
								CString transform = value;
								SysFreeString(value);
								if (transform == _T("caps"))
									m_TextTransform = CapsTransform;
							}
							else
								m_TextTransform = NoneTransform;
						}
						void	LoadBaseline(XmlUtils::CXmlNode &oXmlNode)
						{
							CString strDefaultValue = _T("");

							strDefaultValue.Format(_T("%s"), GetBaselineShift(m_nBaselineShift));
							SetBaselineShift(m_nBaselineShift, m_fBaselineShift, oXmlNode.GetAttributeOrValue(_T("baseline-shift"), strDefaultValue));

							if (m_nBaselineShift != 0)
								m_oFont.m_oFont.Size = m_oFont.m_oFont.Size * 0.667f;
						}

					public:
						void checkFontRange(CString& text) {
							if (text.GetLength() < 1)
								return;
							WCHAR symbol = text[0];
							if ((0x2e80 <= symbol) && (symbol <= 0x9fff) && (m_FontFamilyEA != _T("")))
								m_oFont.SetName(m_FontFamilyEA);
							else if ((0x0590 <= symbol) && (symbol <= 0x05ff) && (_fontFamilyCS != _T("")))
								m_oFont.SetName(_fontFamilyCS);
						}
					};

					class CCharacter
					{
					private:
						bool _isLabel;

					public:
						LONG			m_lCount;
						CTextAttributes	m_oAttributes;

						CLinkProperties	m_oLink;
						BYTE			m_nAdditionalParam;

					public:
						CCharacter() 
							: m_oAttributes(), m_lCount(0), 
							m_oLink(), m_nAdditionalParam(0),
							_isLabel(false)
						{
						}
						CCharacter(const CCharacter& src) {
							m_lCount = src.m_lCount;
							m_oAttributes = src.m_oAttributes;

							m_oLink = src.m_oLink;
							m_nAdditionalParam = src.m_nAdditionalParam;

							_isLabel = src._isLabel;
						}
						~CCharacter()
						{
						}
					public:
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
						{
							m_lCount = oXmlNode.ReadAttributeOrValueInt(_T("count"), 0);

							XmlUtils::CXmlNode oAttributeNode;
							if (oXmlNode.GetNode(_T("Attributes"), oAttributeNode))
								m_oAttributes.FromXmlNode(oAttributeNode);
						}
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode, CDocumentManager& documentManager)
						{
							FromXmlNode(oXmlNode);
							m_oAttributes.ProcessFontProperties(oXmlNode, documentManager);
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode, CLinkProperties& oLink)
						{
							if (oXmlNode.GetName() == _T("fo:inline"))
							{
								CString strText	= oXmlNode.GetTextExt();
								m_lCount		= strText.GetLength();
							}

							m_oLink	= oLink;

							m_oAttributes.FromXmlNode2(oXmlNode);
						}
						void	FromXmlNode3(XmlUtils::CXmlNode &oXmlNode, CLinkProperties& oLink, bool IsInTOC)
						{
							if (oXmlNode.GetName() == _T("fo:inline"))
							{
								CString strText	= oXmlNode.GetTextExt();
								m_lCount		= strText.GetLength();
							}

							m_oLink	= oLink;

							m_oAttributes.FromXmlNode3(oXmlNode, IsInTOC);
						}
						void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
						{
							if (oXmlNode.GetName() != _T("text"))
							{
								m_oAttributes.FromXmlNode2(oXmlNode);
								m_lCount = 0;
							}
							else
							{
								CString strText	= oXmlNode.GetTextExt();
								m_lCount		= strText.GetLength();
							}
						}
						void	FromXmlNode3(XmlUtils::CXmlNode &oXmlNode, bool IsInTOC)
						{
							if (oXmlNode.GetName() != _T("text"))
							{
								m_oAttributes.FromXmlNode3(oXmlNode, IsInTOC);
								m_lCount = 0;
							}
							else
							{
								CString strText	= oXmlNode.GetTextExt();
								m_lCount		= strText.GetLength();
							}
						}
					public:
						inline bool getIsLabel() const {
							return _isLabel;
						}
						inline void setIsLabel(const bool& value) {
							_isLabel = value;
						}
					public:
						void	Transform(CString& text)
						{
							try
							{
								if (m_oAttributes.TextTransform() == CTextAttributes::CapsTransform)
									CharUpper(text.GetBuffer());
							}
							catch (...)
							{
							}
						}
						void CheckFontRange(CString& text) {
							m_oAttributes.checkFontRange(text);
						}
					};
					// ### ### ###
					class CPage;
					class CRegion;
					class CAlternate2;
					class CItem;
					class CSymbols;
					class CBullet;
					class CTabStop;
					class CFootnote;
					class CGrapher;
					class CFontTable
					{
					public:
						class CFontProperties
						{
						public:
							CFontProperties() : 
							  m_Name(_T("")), m_AltName(_T("")), 
								  m_Charset(0), m_Family(_T("")), m_Panose(_T("")), m_Fixed(_T("1")), 
								  m_SigUsb0(_T("00000000")), m_SigUsb1(_T("00000000")), m_SigUsb2(_T("00000000")), m_SigUsb3(_T("00000000")), 
								  m_SigCsb0(_T("00000000")), m_SigCsb1(_T("00000000"))
							  {
							  }
						public:
							byte	FromXml(XmlUtils::CXmlNode& font, const CString& name, IAVSFontManager* fontManager)
							{
								m_Name = name;
								FromXml(font);

								CString params = _T("<FontProperties>");
								CString charset;
								charset.Format(_T("%x"), m_Charset);
								params += _T("<Charset value='") + charset + _T("'/>");
								params += _T("<Name value='") + m_Name + _T("'/>");
								params += _T("<AltName value='") + m_AltName + _T("'/>");
								params += _T("<FamilyClass name='") + m_Family + _T("'/>");
								params += _T("<Panose value='") + m_Panose + _T("'/>");
								params += _T("<FixedWidth value='") + m_Fixed + _T("'/>");
								params += _T("<UnicodeRange range1='") + m_SigUsb0 + _T("' ");
								params += _T("range2='") + m_SigUsb1 + _T("' ");
								params += _T("range3='") + m_SigUsb2 + _T("' ");
								params += _T("range4='") + m_SigUsb3 + _T("' ");
								params += _T("coderange1='") + m_SigCsb0 + _T("' ");
								params += _T("coderange2='") + m_SigCsb1 + _T("'/>");
								params += _T("</FontProperties>");

								BSTR bstrFontPath;
								long index = 0;
								BSTR bstrParams = params.AllocSysString();
								fontManager->GetWinFontByParams(bstrParams, &bstrFontPath, &index);
								SysFreeString(bstrParams);
								int status = fontManager->LoadFontFromFile(bstrFontPath, 12, 72, 72, index);
								SysFreeString(bstrFontPath);
								if (!CheckRange(fontManager))
								{
									params = _T("<FontProperties>");
									CString charset;
									charset.Format(_T("%x"), m_Charset);
									params += _T("<Charset value='") + charset + _T("'/>");
									params += _T("<Name value='") + m_Name + _T("'/>");
									params += _T("<AltName value='") + m_AltName + _T("'/>");
									params += _T("</FontProperties>");

									index = 0;
									bstrParams = params.AllocSysString();
									fontManager->GetWinFontByParams(bstrParams, &bstrFontPath, &index);
									SysFreeString(bstrParams);
									status = fontManager->LoadFontFromFile(bstrFontPath, 12, 72, 72, index);
									SysFreeString(bstrFontPath);
								}
								BSTR bstrFamilyName;
								fontManager->GetFamilyName(&bstrFamilyName);
								m_Name = CString(bstrFamilyName);
								SysFreeString(bstrFamilyName);

								return 0;
							}
							byte	FromXml(const CFont& font, const CString& properties, IAVSFontManager* fontManager)
							{
								BSTR bstrFontPath;
								long index = 0;
								BSTR bstrParams = properties.AllocSysString();
								fontManager->GetWinFontByParams(bstrParams, &bstrFontPath, &index);
								SysFreeString(bstrParams);
								int status = fontManager->LoadFontFromFile(bstrFontPath, 12, 72, 72, index);
								BSTR bstrFamilyName;
								fontManager->GetFamilyName(&bstrFamilyName);
								m_Name = CString(bstrFamilyName);
								SysFreeString(bstrFamilyName);
								return 0;
							}
						public:
							CString	Name() const 
							{
								return m_Name;
							}
							void	Name(const CString& name)
							{
								m_Name = name;
							}
							CString	AltName() const 
							{
								return m_AltName;
							}
							void	AltName(const CString& altName)
							{
								m_AltName = altName;
							}
						public:
							bool	CheckRange(IAVSFontManager* fontManager) const 
							{
								unsigned char charset = (unsigned char)m_Charset;
								unsigned long bit;
								unsigned int index;
								GetCodePageByCharset(charset, &bit, &index);
								VARIANT_BOOL rangeSuits = VARIANT_FALSE;
								fontManager->IsUnicodeRangeAvailable(bit, index, &rangeSuits);
								return (rangeSuits == VARIANT_TRUE);
							}

						private:
							CString	m_Name;
							CString m_AltName;

							int		m_Charset;
							CString	m_Family;
							CString	m_Panose;
							CString m_Fixed;

							CString	m_SigUsb0;
							CString	m_SigUsb1;
							CString	m_SigUsb2;
							CString	m_SigUsb3;
							CString	m_SigCsb0;
							CString	m_SigCsb1;

						private:
							byte	FromXml(XmlUtils::CXmlNode& font)
							{
								XmlUtils::CXmlNode altName;
								if (font.GetNode(_T("altName"), altName))
									m_AltName = altName.GetText();
								XmlUtils::CXmlNode charset;
								if (font.GetNode(_T("charset"), charset))
									m_Charset = HexToInt(charset.GetText());
								XmlUtils::CXmlNode family;
								if (font.GetNode(_T("family"), family))
									m_Family = family.GetText();
								XmlUtils::CXmlNode panose;
								if (font.GetNode(_T("panose"), panose))
									m_Panose = panose.GetText();
								XmlUtils::CXmlNode fixed;
								if (font.GetNode(_T("fixed"), fixed))
									m_Fixed = fixed.GetText();

								XmlUtils::CXmlNode sig;
								if (font.GetNode(_T("sig"), sig))
								{
									m_SigUsb0 = sig.GetAttribute(_T("usb0"), _T("00000000"));
									m_SigUsb1 = sig.GetAttribute(_T("usb1"), _T("00000000"));
									m_SigUsb2 = sig.GetAttribute(_T("usb2"), _T("00000000"));
									m_SigUsb3 = sig.GetAttribute(_T("usb3"), _T("00000000"));
									m_SigCsb0 = sig.GetAttribute(_T("csb0"), _T("00000000"));
									m_SigCsb1 = sig.GetAttribute(_T("csb1"), _T("00000000"));
								}

								return 0;
							}
						private:
							static int	HexToInt(int hex)
							{
								if ( hex >= '0' && hex <= '9' ) return (hex - '0');
								if ( hex >= 'a' && hex <= 'f' ) return (hex - 'a' + 10);
								if ( hex >= 'A' && hex <= 'F' ) return (hex - 'A' + 10);

								return 0;
							}
							static int	HexToInt(CString hex)
							{
								int result = 0;
								int mult = 1;
								for (int i = hex.GetLength() - 1; i >= 0; --i )
								{
									result += HexToInt(hex.GetAt(i)) * mult;
									mult <<= 4;
								}

								return result;
							}
						};

					private:
						std::map<CString, CFontProperties> m_Fonts;

					public:
						CFontTable() :
						  m_Fonts()
						  {
						  }
					public:
						byte	FromXml(XmlUtils::CXmlNode& fontTable, IAVSFontManager* fontManager)
						{
							XmlUtils::CXmlNodes fonts;
							if (!fontTable.GetNodes(_T("font"), fonts))
								return 1;
							int fontCount = fonts.GetCount();
							for (int index = 0; index < fontCount; ++index)
							{
								XmlUtils::CXmlNode font;
								fonts.GetAt(index, font);
								std::pair<CString, CFontProperties> fontPair;
								fontPair.first = font.GetAttribute(_T("name"));
								if (fontPair.second.FromXml(font, fontPair.first, fontManager))
									continue;
								m_Fonts.insert(fontPair);
							}

							return 0;
						}
					public:
						CString	SetFont(const CFont& font, IAVSFontManager* fontManager)
						{
							bool contains = Contains(font.Name());
							if (!contains)
								AddFont(font, fontManager);

							CFontProperties& properties = m_Fonts[font.Name()];

							if  (LoadFont(font, properties.Name(), fontManager) != S_OK) {
								BSTR fontName;
								fontManager->GetFamilyName(&fontName);
								properties.Name(fontName);
								SysFreeString(fontName);
							}

							return properties.Name();
						}
						void	AddFont(const CFont& font, const CString& properties, IAVSFontManager* fontManager)
						{
							std::pair<CString, CFontProperties> fontPair;
							fontPair.first = font.Name();
							fontPair.second.FromXml(font, properties, fontManager);
							m_Fonts.insert(fontPair);							
						}
					public:
						CString	GetName(const CFont& font)
						{
							return m_Fonts[font.Name()].Name();
						}
					private:
						int		LoadFont(const CFont& font, IAVSFontManager* fontManager)
						{
							return LoadFont(font, font.Name(), fontManager);
						}
						int		LoadFont(const CFont& font, const CString& name, IAVSFontManager* fontManager)
						{
							BSTR bstrName = name.AllocSysString();
							float size = (float)font.Size();
							LONG style = font.Style();
							int status = fontManager->LoadFontByName(bstrName, size, style, 72, 72);
							SysFreeString(bstrName);
							return status;
						}
					private:
						bool	Contains(const CString& name)
						{
							return (m_Fonts.find(name) != m_Fonts.end());
						}
						void	AddFont(const CFont& font, IAVSFontManager* fontManager)
						{
							std::pair<CString, CFontProperties> fontPair;
							fontPair.first = font.Name();
							fontPair.second.Name(fontPair.first);
							m_Fonts.insert(fontPair);
						}
					};
					class CLinker
					{
					private:
						struct CChain
						{
							CString	m_strID;
							INT		m_nID;

							RectF	m_oRect;
							WORD	m_nPageNumber;

							CChain(const CString& strID, const RectF& oRect, const WORD nPageNumber, const INT& nID = -1)
								: m_strID(strID), m_nID(nID), 
								m_oRect(oRect), m_nPageNumber(nPageNumber)
							{
							}
							CChain()
								: m_strID(_T("")), m_nID(-1), 
								m_oRect(), m_nPageNumber(0)
							{
							}
						};

						CArray<CChain>	m_arrSources;
						CArray<CChain>	m_arrTargets;

					public:
						CLinker() : 
						  m_arrSources(), m_arrTargets()
						  {
						  }
						  void	Clear()
						  {
							  m_arrSources.RemoveAll();
							  m_arrTargets.RemoveAll();
						  }
					public:
						void	AddTarget(const CString& strID, const RectF& oRect, const CDocumentManager& documentManager)
						{
							WORD pageIndex = documentManager.getPageIndex();
							CChain oChain(strID, oRect, pageIndex);

							m_arrTargets.Add(oChain);
						}
						void	AddSource(const CString& strID, const RectF& oRect, const CDocumentManager& documentManager, const bool& bIsExtern = true)
						{
							WORD nPageNumber = documentManager.getPageIndex() - 1;
							CChain oChain(strID, oRect, nPageNumber);

							m_arrSources.Add(oChain);
						}
						BSTR	GenerateXml()
						{
							CString strXml;
							strXml += _T("<linker>");

							size_t nCount = m_arrSources.GetCount();
							for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
							{
								strXml	+= _T("<link>");

								CChain* pChain	= &m_arrSources[nIndex];
								CString strSource;
								strSource.Format(_T("<source x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" page=\"%i\"/>"), pChain->m_oRect.X, pChain->m_oRect.Y, pChain->m_oRect.Width, pChain->m_oRect.Height, pChain->m_nPageNumber);
								strXml	+= strSource;

								CChain* pChain2	= FindTarget(pChain->m_strID);
								if (pChain2 != 0)
								{
									strSource.Format(_T("<target x=\"%f\" y=\"%f\" width=\"0\" height=\"0\" page=\"%i\"/>"), pChain2->m_oRect.X, pChain2->m_oRect.Y, pChain2->m_nPageNumber);
								}
								else
								{
									strSource.Format(_T("<target url=\"%s\"/>"), pChain->m_strID);
								}
								strXml	+= strSource;

								strXml	+= _T("</link>");
							}

							strXml	+= _T("</linker>");
							strXml.Replace(_T("&"), _T("&amp;"));

							return strXml.AllocSysString();
						}

					private:
						bool	IsContains()
						{
							return NULL;
						}
						CChain*	FindTarget(const CString& strID)
						{
							CChain* pTarget	= NULL;
							size_t nCount	= m_arrTargets.GetCount();
							for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
							{
								CString strTarget	= m_arrTargets[nIndex].m_strID;
								INT iPos			= 0;
								while (iPos != -1)
								{
									CString strToken	= strTarget.Tokenize(_T(";"), iPos);
									if (strToken == strID)
									{
										iPos	= 0;
										break;
									}
								}

								if (iPos == 0)
								{
									pTarget	= &m_arrTargets[nIndex];
								}
							}

							return pTarget;
						}
					};

					class CItemManager
					{
					private:
						REAL	m_fPrevLineHeight;

						REAL	m_fMultiplierX;
						REAL	m_fMultiplierY;

						REAL	m_fWidthL;
						REAL	m_fHeightL;

						bool	m_bWrapFlags;
						bool	m_bIsFo;

					public:
						CItemManager() : 
						  m_fPrevLineHeight(0), 
							  m_fMultiplierX(0), m_fMultiplierY(0), 
							  m_fWidthL(6000), m_fHeightL(6000), 
							  m_bWrapFlags(true), m_bIsFo(false)
						  {
						  }
					public:
						void	SetMultiplierY(const REAL &fMultiplierY)
						{
							m_fMultiplierY = fMultiplierY / m_fHeightL;
						}
						REAL	GetMultiplierY()
						{
							return m_fMultiplierY;
						}
						void	SetMultiplierX(const REAL &fMultiplierX)
						{
							m_fMultiplierX = fMultiplierX / m_fWidthL;
						}
						REAL	GetMultiplierX()
						{
							return m_fMultiplierX;
						}
						void	SetWidthL(const REAL &fWidthL)
						{
							m_fWidthL = fWidthL;
						}
						void	SetHeightL(const REAL &fHeightL)
						{
							m_fHeightL = fHeightL;
						}
						bool	WrapBySymbol()
						{
							return m_bWrapFlags;
						}
						BYTE	GetWrapFlags()
						{
							return m_bWrapFlags;
						}
						void	SetWrapFlags(const WORD &nWrapFlags)
						{
							if (nWrapFlags & 1)
							{
								m_bWrapFlags	= TRUE;
							}
							else
							{
								m_bWrapFlags	= FALSE;
							}
						}

						void	SetIsFo(const bool &bIsFo)
						{
							m_bIsFo	= bIsFo;
						}
						bool	IsFo()
						{
							return m_bIsFo;
						}
					public:
						void	PrepareItems(const CString &strText, CBlockAttributes *pParagraph, 
							CAtlArray<CCharacter*> &arrCharacters, CAtlArray<CAlternate2*> &arrAlternates, 
							CAtlArray<CAlternate2*> &anchors, CAtlArray<CFootnote*>& arrFootnotes, 
							CAtlArray<CItem*>& arrItems, CGrapher* pGrapher)
						{
							size_t nCharacterIndex	= 0;
							size_t nCharacterCount	= arrCharacters.GetCount();
							if (nCharacterCount == 0)
							{
								INT	iIndexContent	= GetNextInline(arrAlternates, -1);
								INT iIndexFootnote	= GetNextFootnote(arrFootnotes, -1);
								int indexAnchor = GetNextAnchor(anchors, -1);
								while (iIndexContent != -1)
								{
									arrItems.Add(arrAlternates[iIndexContent]);
									iIndexContent	= GetNextInline(arrAlternates, iIndexContent);
								}
								while (indexAnchor != -1)
								{
									anchors[indexAnchor]->InlinePosition((int) arrItems.GetCount());
									indexAnchor = GetNextAnchor(anchors, indexAnchor);
								}
								while (iIndexFootnote != -1)
								{
									arrItems.Add(arrFootnotes[iIndexFootnote]);
									iIndexFootnote	= GetNextFootnote(arrFootnotes, iIndexFootnote);
								}
								return;
							}

							// add bullet symbol
							if (pParagraph->m_pBulletProperties != NULL)
							{
								CBullet *pBullet	= CreateBullet(pParagraph->m_pBulletProperties, arrCharacters[nCharacterIndex], pGrapher);
								if (fabs(pParagraph->m_fTextIndent) > pBullet->GetRect().Width) {
									pBullet->GetRect().Width	= fabs(pParagraph->m_fTextIndent);
									if (pParagraph->m_fTextIndent < -pParagraph->m_fStartIndent)
										pParagraph->m_fTextIndent	= -pParagraph->m_fStartIndent;
								}
								arrItems.Add(pBullet);
								arrCharacters.InsertAt(nCharacterIndex, pBullet->GetCharacter());

								++nCharacterIndex;
								++nCharacterCount;
							}

							CCharacter *pCurrentCharacter	= arrCharacters[nCharacterIndex];
							size_t nMaxLength				= pCurrentCharacter->m_lCount;

							INT	iIndexInStr		= 0;
							INT	iIndexContent	= GetNextInline(arrAlternates, -1);
							int indexAnchor = GetNextAnchor(anchors, -1);
							INT iIndexFootnote	= GetNextFootnote(arrFootnotes, -1);

							// process text string
							CString strTextCopy	= strText;
							while (strTextCopy.GetLength() != 0)
							{
								// add inline alternate content to item array
								if (iIndexContent != -1)
								{
									while (iIndexInStr == arrAlternates[iIndexContent]->InlinePosition())
									{
										arrItems.Add(arrAlternates[iIndexContent]);
										iIndexContent	= GetNextInline(arrAlternates, iIndexContent);
										if (iIndexContent == -1)
											break;
									}
								}
								if (indexAnchor != -1)
								{
									while (iIndexInStr == anchors[indexAnchor]->InlinePosition())
									{
										anchors[indexAnchor]->InlinePosition((int) arrItems.GetCount());
										indexAnchor = GetNextAnchor(anchors, indexAnchor);
										if (indexAnchor == -1)
											break;
									}
								}
								if (iIndexFootnote != -1)
								{
									if (iIndexInStr == arrFootnotes[iIndexFootnote]->GetInilinePosition())
									{
										arrItems.Add(arrFootnotes[iIndexFootnote]);
										iIndexFootnote	= GetNextFootnote(arrFootnotes, iIndexFootnote);
									}
								}

								// get next syllable
								CSymbols *pSyllable	= GetNextSyllable(strTextCopy, nMaxLength, pParagraph, pCurrentCharacter, pGrapher);
								if (pSyllable != 0)
								{
									iIndexInStr	+= pSyllable->GetLength();

									arrItems.Add(pSyllable);
								}
								else
								{
									break;
								}

								// check is current character end
								if (nMaxLength == 0)
								{
									if (nCharacterIndex != nCharacterCount - 1)
									{
										++nCharacterIndex;

										pCurrentCharacter	= arrCharacters[nCharacterIndex];
										nMaxLength			= pCurrentCharacter->m_lCount;
									}
									else
									{
										break;
									}
								}
							}

							// add remaining alternate content
							while (iIndexContent != -1)
							{
								arrItems.Add(arrAlternates[iIndexContent]);
								iIndexContent	= GetNextInline(arrAlternates, iIndexContent);
							}
							while (indexAnchor != -1)
							{
								anchors[indexAnchor]->InlinePosition((int) arrItems.GetCount());
								indexAnchor = GetNextAnchor(anchors, indexAnchor);
							}
							while (iIndexFootnote != -1)
							{
								arrItems.Add(arrFootnotes[iIndexFootnote]);
								iIndexFootnote	= GetNextFootnote(arrFootnotes, iIndexFootnote);
							}
						}
						// ---
						INT		GetNextInline(CAtlArray<CAlternate2*> &arrAlternateContent, INT iPreviousIndex)
						{
							if (iPreviousIndex < (INT)arrAlternateContent.GetCount() - 1)
								return ++iPreviousIndex;
							else
								return	-1;
						}
						int		GetNextAnchor(CAtlArray<CAlternate2*>& anchors, const int& prevIndex)
						{
							if (prevIndex < (int)anchors.GetCount() - 1)
								return prevIndex + 1;
							else
								return	-1;
						}
						INT		GetNextFootnote(CAtlArray<CFootnote*> &arrFootnotes, INT iPreviousIndex)
						{
							if (iPreviousIndex < (INT)arrFootnotes.GetCount() - 1)
							{
								return ++iPreviousIndex;
							}
							else
							{
								return	-1;
							}
						}
						// ---
						REAL	GetPlcOrPxl(const REAL &iValue, const REAL &fHeight, const byte& nType = 0)
						{
							if (m_bIsFo == TRUE)
							{
								REAL fResult	= 0.0f;
								if (iValue >= 0)
								{
									fResult	= iValue;
								}
								else
								{
									fResult	= iValue * (-fHeight);
								}

								//*/
								if (nType == 1)
								{
									if (fResult < fHeight)
									{
										fResult	= fHeight;
									}
								}
								//*/

								return fResult;
							}
							else if (iValue < 0)
							{
								return ((-iValue)/100.0f)*fHeight;
							}
							else
							{
								return iValue*m_fMultiplierY;
							}
						}
					protected:
						CSymbols*	GetNextSyllable(CString &strText, size_t &nMaxCount, 
							CBlockAttributes *pParagraph, CCharacter *pCharacter, CGrapher* pGrapher)
						{
							int iMaxLength = (int)nMaxCount;
							if (iMaxLength <= 0)
								return NULL;
							//if (iMaxLength > 20)
							//	iMaxLength = 20;
							if (iMaxLength > strText.GetLength())
								iMaxLength = strText.GetLength();
							CSymbols *pSyllable = NULL;
							if (iMaxLength == 0)
								return pSyllable;
							int iIndex = 0;
							if (strText[(int)iIndex] == _T('\t'))
							{
								++iIndex;
								pSyllable = CreateTabStop(pCharacter, pGrapher);
							}
							else if (strText[(int)iIndex] == _T(' '))
							{
								for (iIndex = 1; iIndex < iMaxLength; ++iIndex)
									if (strText[(int)iIndex] != _T(' '))
										break;
								pSyllable = new CSpace(iIndex, pCharacter, pGrapher);
								((CSpace*)pSyllable)->SetOriginWidth();
							}
							else if (strText[(int)iIndex] == _T('\n'))
							{
								iIndex = 1;
								pSyllable = new CLineEnd(iIndex, pCharacter, pGrapher);
							}
							else
							{
								for (iIndex = 1; iIndex < iMaxLength; ++iIndex)
									if ((strText[(int)iIndex] == _T(' ')) || (strText[(int)iIndex] == _T('\n')) || (strText[(int)iIndex] == _T('\t')))
										break;
								CString strSyllableText = strText.Mid(0, (int)iIndex);
								pSyllable = new CSymbols(strSyllableText, pCharacter, pGrapher);
							}
							strText.Delete(0, (int)iIndex);
							nMaxCount -= iIndex;
							return pSyllable;
						}
						CBullet*	CreateBullet(CBulletProperties *pBulletProperties, CCharacter *pCharacter, CGrapher* pGrapher)
						{
							CCharacter *pBulletCharacter	= new CCharacter();						
							pBulletCharacter->m_lCount		= pBulletProperties->m_strChar.GetLength();
							pBulletCharacter->m_oAttributes = pCharacter->m_oAttributes;

							double dFontSize	= pBulletCharacter->m_oAttributes.m_oFont.m_oFont.Size;
							pBulletCharacter->m_oAttributes.m_oFont.m_oFont.Size = dFontSize * pBulletProperties->m_fSize;

							XmlUtils::CXmlNode oXmlBulletFontSettings;
							oXmlBulletFontSettings.FromXmlString(CString(_T("<Attributes font-underline=\"0\" font-strikeout=\"0\"/>")));

							pBulletCharacter->m_oAttributes.m_oFont.FromXmlNode(oXmlBulletFontSettings);

							CBullet *pBullet	= new CBullet(pBulletProperties->m_strChar, pBulletCharacter, pGrapher);

							return pBullet;
						}
						CTabStop*	CreateTabStop(CCharacter *pCharacter, CGrapher* pGrapher)
						{
							CCharacter *pTabStopCharacter		= new CCharacter();
							pTabStopCharacter->m_lCount			= 1;
							pTabStopCharacter->m_oAttributes	= pCharacter->m_oAttributes;

							XmlUtils::CXmlNode oXmlTabFontSettings;

							pTabStopCharacter->m_oAttributes.m_oFont.FromXmlNode(oXmlTabFontSettings);
							CTabStop *pTabStop	= new CTabStop(pTabStopCharacter, pGrapher);

							return pTabStop;
						}						
					};
					class CDocumentManager
					{
					private:
						int m_iPageCount;
						int	m_iCurrentPageNumber;
						int m_iBlocksCount;
						int m_iBlockIndex;

						bool	m_bStaticProcessing;

						CArray<CTabParams> m_arrTabs;
						float			m_fDefaultTabSize;
						float _lastTab;
						float			m_fLeftBorder;
						float			m_fRightBorder;
						bool			m_bTabReady;

						ListNumeration _listNum;
						PageNumeration _pageNum;

						float	m_fWidthMm;
						float	m_fHeightMm;

						CFontTable	m_FontTable;
						bool	m_UseHtmlSpacing;

						bool m_RendererHtml;

					public:
						CItemManager	m_ItemManager;
						CLinker			m_Linker;
						IAVSFontManager*	m_FontManager;
						ITextPainter*	m_TextPainter;

					public:
						CDocumentManager() 
							: m_iCurrentPageNumber(0), 
							m_iBlocksCount(0), m_iBlockIndex(0), 
							m_bStaticProcessing(false), 
							m_arrTabs(), m_fDefaultTabSize(0.0f), _lastTab(0),
							m_fLeftBorder(0.0f), m_fRightBorder(0.0f), m_bTabReady(false), 
							_listNum(), 
							m_fWidthMm(0.0f), m_fHeightMm(0.0f), 
							m_FontTable(), 
							m_UseHtmlSpacing(true), 
							m_ItemManager(), m_Linker(), 
							m_FontManager(NULL), m_TextPainter(NULL), 
							m_RendererHtml(false)
						{
						}
						
					public:
						void loadFromXml(XmlUtils::CXmlNode& docInfoXml) {
							XmlUtils::CXmlNode listNumSettingsXml;
							if (docInfoXml.GetNode(_T("listSettings"), listNumSettingsXml))
								_listNum.loadFromXml(listNumSettingsXml);

							XmlUtils::CXmlNode pageListNumSettingsXml;
							if (docInfoXml.GetNode(_T("pageNumSettings"), pageListNumSettingsXml))
								_pageNum.loadFromXml(pageListNumSettingsXml);
						}
						void	Clear()
						{
							m_iCurrentPageNumber	= 0;
							m_iBlocksCount			= 0;
							m_iBlockIndex			= 0;

							m_bStaticProcessing	= false;

							m_arrTabs.RemoveAll();
							m_fDefaultTabSize	= 0.0f;
							_lastTab = 0;
							m_fLeftBorder		= 0.0f;
							m_fRightBorder		= 0.0f;
							m_bTabReady			= false;

							_listNum.clear();
							_pageNum.clear();

							m_UseHtmlSpacing = true;
							m_Linker.Clear();

							m_TextPainter = NULL;

							m_RendererHtml = false;
						}

					public:
						void	BlockCreated()
						{
							if (m_bStaticProcessing)
								return;
							++m_iBlocksCount;
						}
						void	BlockProcessed()
						{
							if (m_bStaticProcessing)
								return;
							if (m_iBlocksCount > 0)
							{
								++m_iBlockIndex;
								if (m_iBlockIndex >= m_iBlocksCount - 2)
									m_iBlockIndex = m_iBlocksCount;
							}
						}
						void	SetStaticProcessing(const bool bSet)
						{
							m_bStaticProcessing	= bSet;
						}
						bool	GetStaticProcessing() const 
						{
							return m_bStaticProcessing;
						}
						float	GetCompleteness()
						{
							if (m_iBlocksCount != 0)
							{
								float fProgress = (float)m_iBlockIndex / m_iBlocksCount;
								if (fProgress >= 0.99f)
									fProgress = 0.99f;
								return fProgress;
							}
							else
								return 0.99f;
						}
					public:
						void	SetDefaultTab(const float& fDefaultTabSize, const float& fLeftBorder, const float& fRightBorder)
						{
							m_arrTabs.RemoveAll();

							if (fDefaultTabSize != 0)
								m_fDefaultTabSize = fDefaultTabSize;
							else
								m_fDefaultTabSize = 35.433f;
							
							_lastTab = fLeftBorder;
							m_fLeftBorder		= fLeftBorder;
							m_fRightBorder		= fRightBorder;
							m_bTabReady			= false;
						}
						void	AddTab(CTabParams& Tab)
						{
							m_arrTabs.Add(CTabParams(m_fLeftBorder + Tab.GetPosition(), Tab.GetLeader(), Tab.GetType()));
						}
						CTabParams	GetTabSize(const float& fCurrentPosition)
						{
							if (!m_bTabReady)
							{
								m_bTabReady = true;
								if (m_fDefaultTabSize != 0)
								{
									float fTabPosition = m_fLeftBorder;	

									int iCount = (int)m_arrTabs.GetCount();
									if (iCount)
									{
										_lastTab = m_arrTabs[iCount - 1].GetPosition();
										while (fTabPosition < _lastTab)
											fTabPosition += m_fDefaultTabSize;
									}
									
									while (fTabPosition < m_fRightBorder) {
										m_arrTabs.Add(CTabParams(fTabPosition, 0, 0));
										fTabPosition += m_fDefaultTabSize;
									}

									if (_lastTab < fTabPosition)
										_lastTab = fTabPosition;
									//m_arrTabs.Add(CTabParams(m_fRightBorder, 0, 0));
								}
							}
							/*/ feature : подсчЄт размера табул€ции за пределами разметки страницы
							if (fCurrentPosition > _lastTab) {
								do {
									_lastTab += m_fDefaultTabSize;
									m_arrTabs.Add(CTabParams(_lastTab, 0, 0));
								} while (_lastTab < fCurrentPosition);
							}
							//*/
							for (size_t nIndex = 0; nIndex < m_arrTabs.GetCount(); ++nIndex) {
								if (fCurrentPosition < m_arrTabs[nIndex].GetPosition() 
									    || (fCurrentPosition == m_arrTabs[nIndex].GetPosition() && m_arrTabs[nIndex].GetType() == 2)) {
									return CTabParams(m_arrTabs[nIndex].GetPosition() - fCurrentPosition + 0.001, m_arrTabs[nIndex].GetLeader(), m_arrTabs[nIndex].GetType());
								}
							}

							if (fCurrentPosition <= m_fRightBorder) {
								return CTabParams(m_fRightBorder - fCurrentPosition - 0.001, 0, 0);
							}
							else
								return CTabParams();
						}
					public:
						CString	getListElement(const CString& id, const CString& numId, const int& level) {
							return _listNum.getElement(id, numId, level);
						}

					public:
						void setPageNumbering(const PageNumbering& numbering) {
							_pageNum.setNumbering(numbering);
						}
						void nextPage() {
							_pageNum.nextPage();
						}

						int getPageIndex() const {
							return _pageNum.getPageIndex();
						}
						CString getPageIndexString() const {
							return _pageNum.getPageIndexElement();
						}
						int getPageCount() const {
							return _pageNum.getPageCount();
						}
						CString getPageCountString() const {
							return _pageNum.getPageCountElement();
						}

					public:
						void	SetPage(const CPage& oPage)
						{
							m_fWidthMm = oPage.GetWidthMm();
							m_fHeightMm = oPage.GetHeightMm();
						}
						void	SetBorders(const CRegion& oRegion)
						{
							m_fLeftBorder  = oRegion.GetRectPt().GetLeft();
							m_fRightBorder = oRegion.GetRectPt().GetRight();
						}
						float	GetPageWidthMm()
						{
							return m_fWidthMm;
						}
						float	GetPageHeightMm()
						{
							return m_fHeightMm;
						}
						float	GetPageLeftBorderMm()
						{
							return m_fLeftBorder;
						}
						float	GetPageRightBorderMm()
						{
							return m_fRightBorder;
						}
					public:
						void	FontTableFromXml(XmlUtils::CXmlNode& fontTable)
						{
							m_FontTable.FromXml(fontTable, m_FontManager);
						}
						void	AddFont(const CFont& font, const CString& properties)
						{
							m_FontTable.AddFont(font, properties, m_FontManager);
						}
						CString	SetFont(const CFont& font)
						{
							return m_FontTable.SetFont(font, m_FontManager);
						}
						CString	GetFontName(const CFont& font)
						{
							return m_FontTable.GetName(font);
						}

					public:
						bool	UseHtmlSpacing()
						{
							return m_UseHtmlSpacing;
						}
						void	UseHtmlSpacing(const bool& useHtmlSpacing)
						{
							m_UseHtmlSpacing = useHtmlSpacing;
						}
					public:
						bool RendererHtml() const {
							return m_RendererHtml;
						}
						void RendererType(IAVSRenderer* renderer) {
							LONG rendererType;
							renderer->get_Type(&rendererType);
							if (rendererType == c_nHtmlRendrerer)
								m_RendererHtml = true;
							else
								m_RendererHtml = false;
						}
					};
					class CGrapher
					{
					private:
						class CFontMetrics
						{
						private:
							float	m_fLineSpacing;
							float	m_fEmHeight;
							float	m_fAscent;
							float	m_fDescent;

							float	m_fFontSize;

						public:
							CFontMetrics() : 
							  m_fLineSpacing(0), m_fEmHeight(0), 
								  m_fAscent(0), m_fDescent(0), 
								  m_fFontSize(0)
							  {
							  }
						public:
							void	FromFontManager(IAVSFontManager* m_pFontManager)
							{
								unsigned short iTemp = 0;
								m_pFontManager->GetCellAscent(&iTemp);
								m_fAscent = iTemp;
								m_pFontManager->GetCellDescent(&iTemp);
								m_fDescent = iTemp;
								m_pFontManager->GetLineSpacing(&iTemp);
								m_fLineSpacing = iTemp;
								m_pFontManager->GetEmHeight(&iTemp);
								m_fEmHeight = iTemp;
							}
							void	FontSize(const float& fFontSize)
							{
								m_fFontSize = fFontSize;
							}
						public:
							void	ApplyToRect(RectF& oRect)
							{
								oRect.X = 0;
								oRect.Height = m_fLineSpacing * m_fFontSize / m_fEmHeight;
								oRect.Y = m_fDescent * m_fFontSize / m_fEmHeight - oRect.Height;
							}
							float	GetDescent()
							{
								return (m_fDescent * m_fFontSize / m_fEmHeight);
							}
						};

					private:
						IAVSFontManager*	m_pFontManager;
						CFontMetrics	m_oFontMetrics;

						NSStructuresGDI::CFont m_oldFont;
						bool	m_FontSubstitute;

					public:
						CDocumentManager*	m_DocumentManager;

					public:
						CGrapher() : 
						  m_pFontManager(NULL), m_oFontMetrics(), 
							  m_oldFont(), m_FontSubstitute(false), 
							  m_DocumentManager(NULL)
						  {
							  m_oldFont.Size = -1;
						  }
						  ~CGrapher()
						  {
							  Clear();
						  }
						  void	Clear()
						  {
							  RELEASEINTERFACE(m_pFontManager);

							  m_oldFont.Size = -1;
							  m_DocumentManager = NULL;
						  }
					public:
						void	FontManager(IAVSFontManager* pFontManager)
						{
							RELEASEINTERFACE(m_pFontManager);
							m_pFontManager = pFontManager;
							ADDREFINTERFACE(m_pFontManager);
						}
						void	DocumentManager(CDocumentManager* documentManager)
						{
							assert(documentManager !=NULL);
							m_DocumentManager = documentManager;
						}
					public:
						float	GetDescent(CCharacter* pCharacter)
						{
							return m_oFontMetrics.GetDescent();
						}
						RectF	GetRectForString(const CString& strText, CCharacter *pCharacter)
						{
							SetFont(pCharacter);

							BSTR bstrText = strText.AllocSysString();
							m_pFontManager->LoadString(bstrText, 0, 0);
							SysFreeString(bstrText);

							RectF oRect;
							m_pFontManager->MeasureString2(&oRect.X, &oRect.Y, &oRect.Width, &oRect.Height);
							m_oFontMetrics.ApplyToRect(oRect);

							return oRect;
						}
						float*	GetWidths(const CString& text, CCharacter* character)
						{
							SetFont(character);

							int length = text.GetLength();
							float* widths	= new float[length];

							float temp = 0;
							float width = 0;
							float sumWidth = 0;
							for (int index = 0; index < length; ++index)
							{
								BSTR sysText = CString(text[index]).AllocSysString();
								m_pFontManager->LoadString(sysText, 0, 0);
								SysFreeString(sysText);
								m_pFontManager->MeasureString2(&temp, &temp, &width, &temp);
								widths[index] = width + sumWidth;
								sumWidth += width;
							}

							return widths;
						}
						double	GetStrikePosition(CCharacter	*m_pCharacter)
						{
							BOOL bAvailible; 
							m_pFontManager->IsCharAvailable(121, &bAvailible);
							if (bAvailible)
							{
								CString strText = _T("x");
								BSTR bstrText = strText.AllocSysString();
								m_pFontManager->LoadString(bstrText, 0, 0);
								SysFreeString(bstrText);

								RectF oRect;
								m_pFontManager->MeasureString(&oRect.X, &oRect.Y, &oRect.Width, &oRect.Height);
								float fFontSize = m_pCharacter->m_oAttributes.m_oFont.m_oFont.Size;
								double dStrike = fabs((double)(oRect.Y + oRect.Height / 2));
								return dStrike;
							}
							else
							{
								return 0;
							}
						}
						double	GetUnderlinePosition(CCharacter	*m_pCharacter, double& dSize)
						{
							float fX1, fY1, fX2, fY2, fSize; 
							m_pFontManager->GetUnderline(&fX1, &fY1, &fX2, &fY2, &fSize);
							float fFontSize = m_pCharacter->m_oAttributes.m_oFont.m_oFont.Size;
							double dUnderline = (double) (fY1);
							dSize = (double) fSize * 25.4 / 72.0 / 2.0;
							return dUnderline;						
						}
					public:
						static IAVSFontManager*	GenerateFontManager()
						{
							IAVSFontManager* pFontManager = NULL;
							CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&pFontManager);
							pFontManager->Initialize(L"");

							CString strDefaultFontName = _T("Arial");
							BSTR bstrDefaultFontName = strDefaultFontName.AllocSysString();
							float fFontSize = 12;
							LONG iFontStyle = 0;
							pFontManager->SetDefaultFont(bstrDefaultFontName);
							SysFreeString(bstrDefaultFontName);

							return pFontManager;
						}
					private:
						void	SetFont(CCharacter *pCharacter)
						{
							assert(m_DocumentManager != NULL);
							NSStructuresGDI::CFont& oFont = pCharacter->m_oAttributes.m_oFont.m_oFont;

							if (!oFont.IsEqual(&m_oldFont))
							{
								m_FontSubstitute = false;
								m_oldFont = oFont;

								CString name = m_DocumentManager->SetFont(pCharacter->m_oAttributes.m_oFont);
								if (oFont.Name != name)
								{
									oFont.Name = m_DocumentManager->GetFontName(pCharacter->m_oAttributes.m_oFont);
									m_FontSubstitute = true;
								}

								m_oFontMetrics.FromFontManager(m_pFontManager);
								m_oFontMetrics.FontSize((float)oFont.Size);
							}
							else
							{
								if (m_FontSubstitute)
									oFont.Name = m_DocumentManager->GetFontName(pCharacter->m_oAttributes.m_oFont);
							}

							m_pFontManager->SetCharSpacing(oFont.CharSpace * (72.0f / 25.4f));
						}
					};

					class CAreaManager
					{
					private:
						CWrapRegion	m_oWrapRegion;
						bool		m_bHasWrap;

					public:
						CAreaManager()
							: m_oWrapRegion(), m_bHasWrap(false) 
						{
						}
						~CAreaManager()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							m_oWrapRegion.RemoveAll();
							m_bHasWrap	= false;
						}
					public:
						void	AddWrapPolygon(const CArray<PointF>& arrPoints, const REAL& fDistanceLeft, const REAL& fDistanceRight)
						{
							if (arrPoints.GetCount() == 0)
								return;
							m_bHasWrap	= true;

							CWrapPolygon oWrapPolygon;
							oWrapPolygon.SetPoints(arrPoints);
							m_oWrapRegion.Add(oWrapPolygon, fDistanceLeft, fDistanceRight);
						}
						REAL	GetArea(const RectF& oLineRect, CArray<RectF>& arrRects)
						{
							if (m_bHasWrap)
							{
								m_oWrapRegion.Calculate(oLineRect, arrRects);

								return 0;
							}
							else
							{
								arrRects.RemoveAll();
								arrRects.Add(oLineRect);

								return -1;
							}
						}					
					};

					// ### ### ###
					class CItem
					{
					protected:
						RectF	m_oRect;
						REAL	m_fDescent;

					public:
						CItem() : 
						  m_oRect(), m_fDescent(0.0f)
						  {
						  }
						  virtual ~CItem()
						  {
							  Clear();
						  }
						  void	Clear()
						  {
							  m_oRect	= RectF();
							  m_fDescent	= 0.0f;
						  }
					public:
						virtual RectF&	GetRect()
						{
							return m_oRect;
						}
						virtual REAL	GetHeightPt()
						{
							return m_oRect.Height;
						}
						virtual REAL	GetWidthPt()
						{
							return m_oRect.Width;
						}
						virtual REAL	GetDescent()
						{
							return m_fDescent;
						}
						virtual void	RestoreRect()
						{
							m_oRect.X	= 0.0f;
							m_oRect.Y	= -m_oRect.Height + m_fDescent;
						}
						virtual CItemType	GetType()
						{
							return None;
						}
					public:
						virtual bool	IsOneSymbol()
						{
							return false;
						}
						virtual bool	CanRupture()
						{
							return false;
						}
						virtual bool	CanRupture(CGrapher* grapher, const float& maxWidth)
						{
							return false;
						}
						virtual CItem*	Rupture(const float& maxWidth, CGrapher* grapher, const bool& bySymbol = false
							, const bool& firstSymbolAlways = false)
						{
							return NULL;
						}
						virtual void SetOnBaseline(const bool& inited = true) {
							if (inited)
								MoveTo(0, m_fDescent - m_oRect.Height);
							else
								MoveTo(m_oRect.X, m_fDescent - m_oRect.Height);
						}
						virtual void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float &fCompleteness) = 0;
						virtual void	MoveTo(const REAL &fNewX)
						{
							m_oRect.X	= fNewX;
						}
						virtual void	MoveTo(const REAL &fNewX, const REAL &fNewY)
						{
							m_oRect.X	= fNewX;
							m_oRect.Y	= fNewY;
						}
						virtual void	MoveOn(const REAL &fDeltaX, const REAL &fDeltaY)
						{
							m_oRect.X	+= fDeltaX;
							m_oRect.Y	+= fDeltaY;
						}
					public:
						static void drawRect(IAVSRenderer* renderer, RectF rect) {
							static const float ptToMm = 25.4f / 72.0f;
							rect.X *= ptToMm;
							rect.Y *= ptToMm;
							rect.Width *= ptToMm;
							rect.Height *= ptToMm;

							renderer->put_PenColor(0x0000ff);
							renderer->put_PenSize(0.1);
							renderer->put_PenAlpha(255);
							renderer->BeginCommand(c_nPathType);
							renderer->BeginCommand(c_nTextGraphicType);
							renderer->PathCommandStart();
							renderer->PathCommandMoveTo(rect.X, rect.Y);
							renderer->PathCommandLineTo(rect.X + rect.Width, rect.Y);
							renderer->PathCommandLineTo(rect.X + rect.Width, rect.Y + rect.Height);
							renderer->PathCommandLineTo(rect.X, rect.Y + rect.Height);
							renderer->PathCommandClose();
							renderer->DrawPath(0x01);
							renderer->PathCommandEnd();
							renderer->EndCommand(c_nPathType);
							renderer->EndCommand(c_nTextGraphicType);
						}
					};

					class CSymbols : public CItem
					{
					protected:
						CString		m_strText;

						CCharacter	*m_pCharacter;

					public:
						double		m_size;
						double		m_underlinePosition;	
						double		m_strikePosition;

					public:
						CSymbols() 
							: CItem(), 
							m_strText(_T("")), 
							m_pCharacter(NULL)
						{
						}
						CSymbols(CCharacter* pCharacter)
							: CItem(), 
							m_strText(_T("")), 
							m_pCharacter(pCharacter)
						{
						}
						CSymbols(const CString &strText, CCharacter* pCharacter, CGrapher* pGrapher)
							: CItem(), 
							m_strText(strText), 
							m_pCharacter(pCharacter)
						{
							m_oRect		= pGrapher->GetRectForString(m_strText, pCharacter);							
							m_fDescent	= pGrapher->GetDescent(pCharacter);
							SetOnBaseline();

							m_strikePosition	= GetStrikePosition(pGrapher);
							m_underlinePosition = GetUnderlinePosition(pGrapher, m_size);
						}
					public:
						CCharacter*	GetCharacter()
						{
							return m_pCharacter;
						}
						int			GetLength()
						{
							return m_strText.GetLength();
						}
						inline CItemType	GetType()
						{
							return Symbols;
						}						
						double	GetStrikePosition( CGrapher* pGrapher)
						{
							double dPos = pGrapher->GetStrikePosition(m_pCharacter);
							if (dPos != 0)
								m_strikePosition = dPos;
							else 
								m_strikePosition = GetDescent();
							return dPos;
						}
						double	GetUnderlinePosition( CGrapher* pGrapher, double& dSize)
						{
							double dPos = pGrapher->GetUnderlinePosition(m_pCharacter, dSize);
							return dPos;
						}
						bool	CanRupture()
						{
							return (m_strText.FindOneOf(_T("-")) != -1);
						}
						bool	CanRupture(CGrapher* grapher, const float& maxWidth)
						{
							if (m_strText.GetLength() == 0)
								return false;

							float* firstSymbolWidth = grapher->GetWidths((CString) m_strText[0], m_pCharacter);
							bool canRupture = false;
							if (firstSymbolWidth[0] <= maxWidth)
								canRupture = true;

							return canRupture;
						}
					public:
						bool	IsOneSymbol()
						{
							return (m_strText.GetLength() == 1);
						}
						CItem*	Rupture(const float& maxWidth, CGrapher* grapher, const bool& bySymbol = false
							, const bool& firstSymbolAlways = false)
						{
							float* widths = grapher->GetWidths(m_strText, m_pCharacter);

							int length = m_strText.GetLength();
							int lastSuitItem = -1;

							CArray<int> rupPositions;
							RupturePositions(rupPositions);
							int rupPositionsCount = (int)rupPositions.GetCount();
							if (rupPositionsCount > 0)
							{
								int rupPos = rupPositionsCount - 1;
								while (rupPos >= 0)
									if (widths[rupPositions[rupPos]] > maxWidth)
										--rupPos;
									else
										break;
								if (rupPos >= 0)
									lastSuitItem = rupPositions[rupPos];
							}

							if (lastSuitItem < 0 && bySymbol)
							{
								lastSuitItem = length - 1;
								while (lastSuitItem >= 0)
									if (widths[lastSuitItem] > maxWidth)
										--lastSuitItem;
									else
										break;
							}

							RELEASEARRAYOBJECTS(widths);

							if (lastSuitItem == -1 && firstSymbolAlways)
								++lastSuitItem;

							CSymbols* newItem = NULL;
							if (lastSuitItem >= 0)
							{
								CString newItemText = m_strText.Mid(0, lastSuitItem + 1);
								m_strText.Delete(0, lastSuitItem + 1);

								newItem = new CSymbols(newItemText, m_pCharacter, grapher);

								m_oRect = grapher->GetRectForString(m_strText, m_pCharacter);
								m_oRect.Y = newItem->m_oRect.Y;
							}
							return newItem;
						}
						void	SetOnBaseline(const bool& bIsInit = true)
						{
							if (bIsInit)
							{
								MoveTo(0, m_fDescent - m_oRect.Height);
							}
							else
							{
								MoveTo(m_oRect.X, m_fDescent - m_oRect.Height);
							}

							if (m_pCharacter->m_oAttributes.m_nBaselineShift == 1)
							{
								MoveOn(0, -2 * m_fDescent);
							}
							else if (m_pCharacter->m_oAttributes.m_nBaselineShift == 2)
							{
								MoveOn(0, m_fDescent);
							}
							else if (m_pCharacter->m_oAttributes.m_fBaselineShift != 0)
							{
								MoveOn(0, -m_pCharacter->m_oAttributes.m_fBaselineShift);
							}
						}
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float &fCompleteness)
						{
							if (m_oRect.Width == 0)
								return;

							if (m_pCharacter->m_oLink.strTarget != _T(""))
								documentManager.m_Linker.AddSource(m_pCharacter->m_oLink.strTarget, m_oRect, documentManager);

							BSTR bstrText	= m_strText.AllocSysString();

							m_pCharacter->m_oAttributes.m_oFont.SetToRenderer(pRenderer);
							m_pCharacter->m_oAttributes.m_oBrush.SetToRenderer(pRenderer, fCompleteness);
							m_pCharacter->m_oAttributes.m_oShadow.SetToRenderer(pRenderer);
							m_pCharacter->m_oAttributes.m_oEdge.SetToRenderer(pRenderer);

							sendBaselineShiftToRenderer(pRenderer);

							#ifdef TME_TEXT_BLACK
							pRenderer->put_BrushColor1(0xff000000);
							#endif

							const float fPtToMm	= 25.4f / 72.0f;

							if (m_pCharacter->getIsLabel())
								pRenderer->BeginCommand(c_nNumberingMarker);

							if (NULL == documentManager.m_TextPainter)
								pRenderer->CommandDrawText(bstrText, m_oRect.X * fPtToMm, m_oRect.Y * fPtToMm, m_oRect.Width * fPtToMm, m_oRect.Height * fPtToMm, (m_oRect.Height - m_fDescent) * fPtToMm);
							else
								documentManager.m_TextPainter->CommandText(bstrText, m_oRect.X * fPtToMm, m_oRect.Y * fPtToMm, m_oRect.Width * fPtToMm, m_oRect.Height * fPtToMm, (m_oRect.Height - m_fDescent) * fPtToMm, pRenderer);
							SysFreeString(bstrText);
							if (m_pCharacter->getIsLabel())
								pRenderer->EndCommand(c_nNumberingMarker);

							#ifdef TME_DRAW_SYMBOL_BORDER
							CItem::drawRect(pRenderer, m_oRect);
							#endif
						}						
					private:
						void	RupturePositions(CArray<int>& rupturePositions)
						{
							CString rupSymbols = _T("-");

							int index = 0;
							int length = m_strText.GetLength();
							while (index < length)
							{
								if (rupSymbols.Find(m_strText[index]) != -1)
									rupturePositions.Add(index);
								++index;
							}
						}
					private:
						void sendBaselineShiftToRenderer(IAVSRenderer* renderer) {
							IAVSRenderer2* renderer2 = NULL;
							if (SUCCEEDED(renderer->QueryInterface(__UUIDOF__IAVSRENDERER2, (void**)&renderer2)) && (NULL != renderer2)) {
								byte baselineShiftPos = m_pCharacter->m_oAttributes.m_nBaselineShift;
								float baselineShift = m_pCharacter->m_oAttributes.m_fBaselineShift;

								if (baselineShiftPos != 0)
									renderer2->CommandLong2(c_nBaselineShift, baselineShiftPos);
								else if (baselineShift != 0) {
									if (baselineShift > 0)
										renderer2->CommandLong2(c_nBaselineShift, 1);
									else
										renderer2->CommandLong2(c_nBaselineShift, 2);
								}
							}
						}
					};

					class CSpace : public CSymbols
					{
					private:
						double m_originWidth;

					public:
						CSpace() 
							: CSymbols()
						{
							m_originWidth = 0;
						}
						CSpace(const size_t &nCount, CCharacter* pCharacter, CGrapher* pGrapher)
							: CSymbols()
						{
							m_strText	= _T("");
							for (size_t i = 0; i < nCount; ++i)
							{
								m_strText	+= _T(" ");
							}

							m_pCharacter	= pCharacter;

							m_oRect		= pGrapher->GetRectForString(m_strText, pCharacter);
							m_fDescent	= pGrapher->GetDescent(pCharacter);

							m_strikePosition	= GetStrikePosition(pGrapher);
							m_underlinePosition = GetUnderlinePosition(pGrapher, m_size);

							SetOnBaseline();
						}
					public:
						inline CItemType	GetType()
						{
							return Space;
						}
						void	RestoreRect()
						{
							m_oRect.X	= 0.0f;
							m_oRect.Y	= -m_oRect.Height + m_fDescent;
							m_originWidth;

							m_oRect.Width = m_originWidth;
						}
						void   SetOriginWidth()
						{
							m_originWidth = m_oRect.Width;
						}
					public:
						CItem*	Rupture(const float& maxWidth, CGrapher* grapher, const bool& bySymbol = false
							, const bool& firstSymbolAlways = false)
						{
							CItem* newItem = CSymbols::Rupture(maxWidth, grapher, bySymbol, firstSymbolAlways);
							if (newItem != NULL)
							{
								CSymbols* symbolsItem = (CSymbols*) newItem;
								int length = (int) symbolsItem->GetLength();
								CCharacter* character = symbolsItem->GetCharacter();
								CSpace* spaceItem = new CSpace(length, character, grapher);
								RELEASEOBJECT(newItem);
								return spaceItem;
							}
							else
								return newItem;
						}
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float &fCompleteness)
						{
						}
					};

					class CLineEnd : public CSymbols
					{
					public:
						CLineEnd() 
							: CSymbols()
						{
						}
						CLineEnd(const size_t &nCount, CCharacter *pCharacter, CGrapher* pGrapher)
							: CSymbols()
						{
							m_strText	= _T("");
							for (size_t i = 0; i < nCount; ++i)
							{
								m_strText	+= _T("\n");
							}

							m_pCharacter	= pCharacter;

							m_oRect		= pGrapher->GetRectForString(m_strText, pCharacter);
							m_fDescent	= pGrapher->GetDescent(pCharacter);

							SetOnBaseline();

							m_oRect.Width	= 0.0f;
						}
					public:
						inline CItemType	GetType()
						{
							return LineEnd;
						}
					public:
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float &fCompleteness)
						{
						}
					};

					class CBullet : public CSymbols
					{
					public:
						CBullet() 
							: CSymbols()
						{
							m_pCharacter->setIsLabel(true);
						}
						CBullet(const CString &strBullet, CCharacter *pCharacter, CGrapher* pGrapher)
							: CSymbols(strBullet, pCharacter, pGrapher)
						{
							m_strikePosition	= GetStrikePosition(pGrapher);
							m_underlinePosition = GetUnderlinePosition(pGrapher, m_size);

							m_pCharacter->setIsLabel(true);
						}
						~CBullet()
						{
						}
					public:
						inline CItemType	GetType()
						{
							return Bullet;
						}
					};

					class CTabStop : public CSymbols
					{
					public:
						byte m_bType;
						byte m_bLeader;

					public:
						CTabStop() 
							: CSymbols()
						{
						}
						CTabStop(CCharacter *pCharacter, CGrapher* pGrapher)
						{
							m_strText = _T("x");
							m_pCharacter = pCharacter;

							m_oRect		= pGrapher->GetRectForString(m_strText, pCharacter);
							m_fDescent	= pGrapher->GetDescent(pCharacter);

							m_strikePosition	= GetStrikePosition(pGrapher);
							m_underlinePosition = GetUnderlinePosition(pGrapher, m_size);

							SetOnBaseline();
						}
					public:
						inline CItemType	GetType()
						{
							return TabStop;
						}
					public:
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float& completeness) {
							#ifdef TME_SHOW_TABS
							CSymbols::Draw(pRenderer, documentManager, completeness);
							#endif

							const float fPtToMm	= 25.4f / 72.0f;
							if (m_bLeader != 0 && (m_bLeader == 1 || m_bLeader == 2 || m_bLeader == 3))
							{
								double lineY;
								if (m_bLeader == 1)
								{
									lineY = GetRect().GetBottom() - GetDescent();
									pRenderer->put_PenSize(2 * m_size);
									pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);

									SAFEARRAYBOUND rgsab;
									rgsab.lLbound	= 0;
									rgsab.cElements	= 2;

									SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);

									double* pData = (double*)pArray->pvData;
									pData[0] = 1;
									pData[1] = 2;

									pRenderer->PenDashPattern(pArray);

									RELEASEARRAY(pArray);
								}
								else if (m_bLeader == 2)
								{
									lineY = GetRect().GetBottom() - GetDescent() - m_strikePosition;
									pRenderer->put_PenSize(m_size);
									pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);

									SAFEARRAYBOUND rgsab;
									rgsab.lLbound	= 0;
									rgsab.cElements	= 2;

									SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);

									double* pData = (double*)pArray->pvData;
									pData[0] = 2;
									pData[1] = 2;

									pRenderer->PenDashPattern(pArray);

									RELEASEARRAY(pArray);

								}
								else if (m_bLeader == 3)
								{
									lineY = GetRect().GetBottom() - GetDescent() + m_underlinePosition / 2;
									pRenderer->put_PenSize(m_size);
									pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);

									SAFEARRAYBOUND rgsab;
									rgsab.lLbound	= 0;
									rgsab.cElements	= 2;

									SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);

									double* pData = (double*)pArray->pvData;
									pData[0] = 5;
									pData[1] = 1;

									pRenderer->PenDashPattern(pArray);

									RELEASEARRAY(pArray);
								}
								if ((GetRect().GetRight() - GetRect().GetLeft()) * fPtToMm > 10)
								{
									pRenderer->put_PenColor(GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
									pRenderer->BeginCommand(c_nPathType);
									//pRenderer->BeginCommand(c_nTextGraphicType);
									pRenderer->PathCommandStart();
									pRenderer->PathCommandMoveTo(GetRect().GetLeft() * fPtToMm + 1, lineY * fPtToMm);
									pRenderer->PathCommandLineTo(GetRect().GetRight() * fPtToMm, lineY * fPtToMm);
									pRenderer->DrawPath(c_nStroke);
									pRenderer->PathCommandEnd();
									//pRenderer->EndCommand(c_nTextGraphicType);
									pRenderer->EndCommand(c_nPathType);
								}
							}
						}
					};

					class CRect
					{
					private:
						RectF  m_rect;
						long   m_color;
						byte   m_underline;
						byte   m_alpha;

					public:
						CRect()
							: m_rect(),
							m_color(0xFFFFFF),
							m_underline()
						{
						}
						CRect(RectF rect, long color, byte alpha,byte underline)
							: m_rect(rect),
							m_color(color),
							m_alpha(alpha),
							m_underline(underline)
						{
						}
						CRect(RectF rect, long color)
							: m_rect(rect),
							m_color(color),
							m_underline()
						{
						}

						double GetLeft()
						{
							return m_rect.GetLeft();
						}
						double GetRight()
						{
							return m_rect.GetRight();
						}
						double GetTop()
						{
							return m_rect.GetTop();
						}
						double GetBottom()
						{
							return m_rect.GetBottom();
						}
						long GetColor()
						{
							return m_color;
						}
						byte GetAlpha()
						{
							return m_alpha;
						}
						byte GetUnderline()
						{
							return m_underline;
						}
					};

					class CWord : public CItem {
					protected:
						CAtlArray<CItem*>	m_arrItems;
						size_t				m_nItemsCount;

						float _originDescent;

					public:
						CWord() 
							: CItem(), 
							m_nItemsCount(0), 
							_originDescent(0) {
						}
						~CWord() {
							Clear();
						}
						void Clear() {
							for (size_t nIndex = 0; nIndex < m_nItemsCount; ++nIndex)
							{
								RELEASEOBJECT(m_arrItems[nIndex]);
							}
							m_arrItems.RemoveAll();
							m_nItemsCount = 0;

							_originDescent = 0;
						}
					public:
						void RemoveAllItems()
						{
							m_arrItems.RemoveAll();
							m_nItemsCount	= 0;
						}
						inline CItemType	GetType()
						{
							return Word;
						}

						inline float getOriginDescent() const {
							return _originDescent;
						}
					public:
						void	AddItem(CItem *pItem)
						{
							pItem->MoveTo(m_oRect.X + m_oRect.Width, pItem->GetRect().Y);
							m_oRect.Width	+= pItem->GetRect().Width;

							updateAscent(pItem);
							updateDescent(pItem);

							m_fDescent	= m_oRect.GetBottom();

							m_arrItems.Add(pItem);
							++m_nItemsCount;
						}

					private:
						void updateAscent(CItem* item) {
							float itemAscent = item->GetHeightPt() - item->GetDescent();

							float itemTop = item->GetRect().GetTop();
							if (itemTop > -itemAscent)
								itemTop = -itemAscent;
							float lineTop = m_oRect.GetTop();

							if (lineTop > itemTop) {
								m_oRect.Height += lineTop - itemTop;
								m_oRect.Y = itemTop;
							}
						}
						void updateDescent(CItem* item) {
							float itemDescent = item->GetDescent();
							if (itemDescent > _originDescent)
								_originDescent = itemDescent;
							
							float itemBottom = item->GetRect().GetBottom();
							if (itemBottom < itemDescent)
								itemBottom = itemDescent;

							if (m_oRect.GetBottom() < itemBottom)
								m_oRect.Height	= itemBottom - m_oRect.GetTop();
						}

					public:
						size_t	GetItemsCount()
						{
							return m_nItemsCount;
						}
						void	RearrangeItems()
						{
							REAL fCurrentX	= m_oRect.X;
							for (size_t nIndex = 0; nIndex < m_nItemsCount; ++nIndex)
							{
								CItem *pItem	= m_arrItems[nIndex];
								pItem->MoveTo(fCurrentX);
								fCurrentX		+= pItem->GetRect().Width;
							}
						}
						CWord*	Rupture(REAL &fPersistWidth, CGrapher* pGrapher)
						{
							REAL fCurrentWidth	= 0;							
							CItem *pItem		= NULL;

							size_t nItemIndex	= 0;
							for (; nItemIndex < m_nItemsCount; ++nItemIndex)
							{
								pItem	= m_arrItems[nItemIndex];
								if (fCurrentWidth + pItem->GetRect().Width > fPersistWidth)
								{
									break;
								}
								fCurrentWidth	+= pItem->GetRect().Width;
							}

							CWord *pNewWord	= new CWord();
							for (size_t nIndex = 0; nIndex < nItemIndex; ++nIndex)
							{
								pNewWord->AddItem(m_arrItems[nIndex]);
							}

							m_arrItems.RemoveAt(0, nItemIndex);
							m_nItemsCount	-= nItemIndex;

							fPersistWidth	-= fCurrentWidth;

							CItem *pNewItem = pItem->Rupture(fPersistWidth, pGrapher);
							if (pNewItem != NULL)
							{
								pNewWord->AddItem(pNewItem);
							}

							if (pNewWord->m_nItemsCount != 0)
							{
								m_oRect.Width	-= pNewWord->m_oRect.Width;

								RearrangeItems();
							}
							else
							{
								RELEASEOBJECT(pNewWord);
							}

							return pNewWord;
						}
					public:
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float &fCompleteness)
						{
							RectF underlineRect;
							RectF shadingRect;
							RectF strikeRect;
							double descent = 0;							
							double dSizeUnderline = 0.25;
							double dSizeStrike = 0.25;
							const float fPtToMm	= 25.4f / 72.0f;
							CAtlArray<CRect> arrShadings;
							CAtlArray<CRect> arrUnderlines;


							for (size_t nIndex = 0; nIndex < m_nItemsCount; ++nIndex)
							{								
#ifdef _DEBUG
								CItemType p = m_arrItems[nIndex]->GetType();
#endif

								if (m_arrItems[nIndex]->GetType() != Word && m_arrItems[nIndex]->GetType() != Alternate &&
									m_arrItems[nIndex]->GetType() != Footnote)
								{
									CSymbols* item = (CSymbols*)m_arrItems[nIndex];

									//SHADING
									if (item->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Alpha)
									{
										if (IsLastInEquallyShading(nIndex - 1))
										{
											shadingRect.X = item->GetRect().GetLeft();
											shadingRect.Y = item->GetRect().GetTop();
										}

										shadingRect.Width += item->GetWidthPt();

										if (IsLastInEquallyShading(nIndex))
										{
											long color = item->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color;
											arrShadings.Add(CRect(RectF(shadingRect.X * fPtToMm, shadingRect.Y * fPtToMm, shadingRect.Width * fPtToMm, shadingRect.Height * fPtToMm), color));

											shadingRect.Width  = 0;
										}
									}
								}
							}
							//drawShading
							for(size_t nIndex = 0; nIndex < arrShadings.GetCount(); ++nIndex)
							{
								CRect rect = arrShadings[nIndex];
								DrawShading(pRenderer, rect.GetLeft(), GetRect().GetTop() * fPtToMm, rect.GetRight() - rect.GetLeft(), GetRect().Height * fPtToMm, rect.GetColor());
								//DrawShading(pRenderer, rect.GetLeft(), (shadingRect.Y + descent - shadingRect.Height) * fPtToMm, rect.GetRight() - rect.GetLeft(), shadingRect.Height * fPtToMm, rect.GetColor());
							}

							for(size_t nIndex = 0; nIndex < m_nItemsCount; ++nIndex)
							{
#ifdef AVS_LOG_DEBUG
								CItemType p = m_arrItems[nIndex]->GetType();
#endif
								if (m_arrItems[nIndex]->GetType() != Word && m_arrItems[nIndex]->GetType() != Alternate && m_arrItems[nIndex]->GetType() != Footnote)
								{
									CSymbols* item = (CSymbols*)m_arrItems[nIndex];									
									CTextAttributes textAttrs = item->GetCharacter()->m_oAttributes;

									//item->m_strikePosition	= item->GetStrikePosition(pGrapher);
									//item->m_underlinePosition = item->GetUnderlinePosition(pGrapher, item->m_size);

									double dHeightLine = item->GetHeightPt();
									//STRIKE
									if (textAttrs.m_oFont.m_oFont.Strikeout != 0)
									{

										strikeRect.Width = item->GetWidthPt();
										dSizeStrike = item->m_size;

										strikeRect.X = item->GetRect().GetLeft();
										if (item->m_strikePosition != 0)
											strikeRect.Y = item->GetRect().GetBottom() - item->GetDescent() - item->m_strikePosition;
										else
											strikeRect.Y = item->GetRect().GetBottom() - 2 * item->GetDescent();

										DrawStrike(pRenderer, strikeRect.X * fPtToMm,
											strikeRect.Y * fPtToMm,
											(strikeRect.X + strikeRect.Width)  * fPtToMm,
											strikeRect.Y  * fPtToMm,
											textAttrs.m_oBrush.m_oBrush.Color1,
											textAttrs.m_oBrush.m_oBrush.Alpha1,
											textAttrs.m_oFont.m_oFont.Strikeout, 
											dSizeStrike);
									}

									//UNDERLINE
									if (textAttrs.m_oFont.m_oFont.Underline != 0)
									{
										/*if (textAttrs.m_oFont.m_oFont.Strikeout == 0)
										;item->m_underlinePosition = item->GetUnderlinePosition(pGrapher, item->m_size);*/

										if (dHeightLine > underlineRect.Height)
											underlineRect.Height = dHeightLine;

										if (IsLastInEquallyUnder(nIndex - 1))
										{
											underlineRect.X = item->GetRect().GetLeft();
											underlineRect.Y = 0;
											underlineRect.Width  = 0;
											underlineRect.Height = 0;
											dSizeUnderline = 0;										
										}
										double size  = item->m_size;//dHeightLine / 50;
										double width = item->GetWidthPt();
										double dUnderlinePos = item->m_underlinePosition / 2 * fPtToMm  + item->GetRect().GetBottom() * fPtToMm - item->GetDescent() * fPtToMm;	

										if ((underlineRect.Width + width) != 0 )
										{
											dSizeUnderline  = (width * size + underlineRect.Width * dSizeUnderline) / (underlineRect.Width + width);
											underlineRect.Y = (width * dUnderlinePos + underlineRect.Width * underlineRect.Y) / (underlineRect.Width + width);
										}

										underlineRect.Width  += width;

										if (IsLastInEquallyUnder(nIndex))
										{										
											arrUnderlines.Add(CRect(RectF(underlineRect.X * fPtToMm, underlineRect.Y * fPtToMm, underlineRect.Width * fPtToMm, 0), textAttrs.m_oBrush.m_oBrush.Color1, textAttrs.m_oBrush.m_oBrush.Alpha1, textAttrs.m_oFont.m_oFont.Underline));

											underlineRect.Width  = 0;
											if (IsLastInEquallyTypeUnder(nIndex) && nIndex != m_nItemsCount - 1)
											{
												for(size_t nIndex = 0; nIndex < arrUnderlines.GetCount(); ++nIndex)
												{
													CRect rect = arrUnderlines[nIndex];
													DrawUnderline(pRenderer, rect.GetLeft(), 
														(underlineRect.Y),
														rect.GetRight(),
														(underlineRect.Y),												 
														rect.GetColor(),
														rect.GetAlpha(),
														rect.GetUnderline(), 
														dSizeUnderline);												
												}

												arrUnderlines.RemoveAll();
												underlineRect.Height = 0;
												dSizeUnderline = 0;
											}
										}
									}
								}

								m_arrItems[nIndex]->Draw(pRenderer, documentManager, fCompleteness);
							}

							//drawUnderline
							for(size_t nIndex = 0; nIndex < arrUnderlines.GetCount(); ++nIndex)
							{
								CRect rect = arrUnderlines[nIndex];
								DrawUnderline(pRenderer, rect.GetLeft(), 
									(underlineRect.Y), 
									rect.GetRight(),
									(underlineRect.Y), 														 
									rect.GetColor(), 
									rect.GetAlpha(),
									rect.GetUnderline(),
									dSizeUnderline);								
							}
						}
					public:
						bool	IsLastInEquallyUnder(size_t num)
						{
							if (num == m_nItemsCount - 1 || num == -1)
								return true;
							CItemType type1 = m_arrItems[num]->GetType();
							CItemType type2 = m_arrItems[num + 1]->GetType();
							if (type1 == Word || type2 == Word ||
								type1 == Alternate || type2 == Alternate ||
								type1 == Footnote || type2 == Footnote)
								return true;
							CTextAttributes cur  = ((CSymbols*)m_arrItems[num])->GetCharacter()->m_oAttributes;
							CTextAttributes next = ((CSymbols*)m_arrItems[num + 1])->GetCharacter()->m_oAttributes;
							byte under1 = cur.m_oFont.m_oFont.Underline;
							byte under2 = next.m_oFont.m_oFont.Underline;
							long color1 = cur.m_oBrush.m_oBrush.Color1;
							long color2 = next.m_oBrush.m_oBrush.Color1;

							if ((under1 == under2) != 0 && color1 == color2)
								return false;
							else
								return true;								
						}
						bool	IsLastInEquallyTypeUnder(size_t num)
						{
							if (num == m_nItemsCount - 1 || num == -1)
								return true;
							CItemType type1 = m_arrItems[num]->GetType();
							CItemType type2 = m_arrItems[num + 1]->GetType();
							if (type1 == Word || type2 == Word ||
								type1 == Alternate || type2 == Alternate ||
								type1 == Footnote || type2 == Footnote)
								return true;
							CTextAttributes cur  = ((CSymbols*)m_arrItems[num])->GetCharacter()->m_oAttributes;
							CTextAttributes next = ((CSymbols*)m_arrItems[num + 1])->GetCharacter()->m_oAttributes;
							byte under1 = cur.m_oFont.m_oFont.Underline;
							byte under2 = next.m_oFont.m_oFont.Underline;

							if ((under1 == under2) != 0)
								return false;
							else
								return true;								
						}
						bool	IsLastInEquallyShading(size_t num)
						{
							if (num == m_nItemsCount - 1 || num == -1)
								return true;
							CItemType type1 = m_arrItems[num]->GetType();
							CItemType type2 = m_arrItems[num + 1]->GetType();
							if (type1 == Word || type2 == Word ||
								type1 == Alternate || type2 == Alternate ||
								type1 == Footnote || type2 == Footnote)
								return true;
							CSymbols* cur  = (CSymbols*)m_arrItems[num];
							CSymbols* next = (CSymbols*)m_arrItems[num + 1];
							CCharacter* curCharacter  = cur->GetCharacter();
							CCharacter* nextCharacter = next->GetCharacter();
							if (curCharacter == NULL || nextCharacter == NULL)
								return false;
							long shadow1 = curCharacter->m_oAttributes.m_oBackground.m_oBackground.Color;
							long shadow2 = nextCharacter->m_oAttributes.m_oBackground.m_oBackground.Color;
							byte alpha1 = curCharacter->m_oAttributes.m_oBackground.m_oBackground.Alpha;
							byte alpha2 = nextCharacter->m_oAttributes.m_oBackground.m_oBackground.Alpha;

							if ((shadow1 == shadow2) && (alpha1 == alpha2) )
								return false;
							else
								return true;

						}
						void	DrawUnderline(IAVSRenderer *pRenderer, double dX1, double dY1, double dX2, double dY2, long lColor, byte bAlpha, byte bUnderline, double dSize)
						{							
							if (bUnderline != 0)
							{
								pRenderer->put_PenAlpha(bAlpha);
								pRenderer->put_PenSize(dSize);
								pRenderer->put_PenColor(lColor);
								pRenderer->BeginCommand(c_nTextGraphicType);
								if (bUnderline == 1  || bUnderline == 3  || bUnderline == 4  || bUnderline == 5 ||
									bUnderline == 6  || bUnderline == 7  || bUnderline == 8  || bUnderline == 9 ||
									bUnderline == 10 || bUnderline == 11 || bUnderline == 12 || bUnderline == 13)
								{
									//Type
									if (bUnderline == 1)
										pRenderer->put_PenDashStyle(DashStyleSolid);									
									else if (bUnderline == 6 || bUnderline == 7 || bUnderline == 8 || bUnderline == 9)
										pRenderer->put_PenDashStyle(DashStyleDash);
									else if (bUnderline == 4 || bUnderline == 5)
										pRenderer->put_PenDashStyle(DashStyleDot);
									else if (bUnderline == 10 || bUnderline == 11)
										pRenderer->put_PenDashStyle(DashStyleDashDot);
									else if (bUnderline == 12 || bUnderline == 13)
										pRenderer->put_PenDashStyle(DashStyleDashDotDot);

									//Size
									if (bUnderline == 3 || bUnderline == 5 || bUnderline == 7 ||
										bUnderline == 9 || bUnderline == 11 || bUnderline == 13)
										pRenderer->put_PenSize(2 * dSize);
									else
										pRenderer->put_PenSize(dSize);

									DrawLine(pRenderer, dX1, dY1  - dSize, dX2, dY2  - dSize);
								}
								else if (bUnderline == 2)
								{
									pRenderer->put_PenDashStyle(DashStyleSolid);

									DrawLine(pRenderer, dX1, dY1  - dSize, dX2, dY2  - dSize);
									DrawLine(pRenderer, dX1, dY1  + dSize, dX2, dY2  + dSize);									
								}
								else if (bUnderline == 14 || bUnderline == 15 || bUnderline == 16)
								{		
									pRenderer->put_PenDashStyle(DashStyleSolid);
									if (bUnderline == 14)
									{
										pRenderer->put_PenSize(dSize / 2);
										DrawWave(pRenderer, dX1, dY1, dX2, dY2, dSize);
									}
									else if (bUnderline == 15)
									{
										pRenderer->put_PenSize(dSize);
										DrawWave(pRenderer, dX1, dY1, dX2, dY2, 3 * dSize / 2);
									}
									else if (bUnderline == 16)
									{
										pRenderer->put_PenSize(dSize / 2);
										DrawWave(pRenderer, dX1, dY1 - dSize / 2, dX2, dY2 - dSize / 2, dSize / 2);
										DrawWave(pRenderer, dX1, dY1 + dSize / 2, dX2, dY2 + dSize / 2, dSize / 2);
									}
								}
								pRenderer->EndCommand(c_nTextGraphicType);
							}
						}
						void	DrawStrike(IAVSRenderer *pRenderer, double dX1, double dY1, double dX2, double dY2, long lColor, byte bAlpha, byte bStrike, double dSize)
						{
							if (bStrike != 0)
							{
								pRenderer->BeginCommand(c_nTextGraphicType);
								pRenderer->put_PenDashStyle(DashStyleSolid);
								pRenderer->put_PenSize(dSize);
								pRenderer->put_PenColor(lColor);
								pRenderer->put_PenAlpha(bAlpha);

								if (bStrike == 1)
								{
									DrawLine(pRenderer, dX1, dY1, dX2, dY2);
								}
								if (bStrike == 2)
								{
									DrawLine(pRenderer, dX1, dY1 - dSize, dX2, dY2 - dSize);
									DrawLine(pRenderer, dX1, dY1 + 3 * dSize / 2, dX2, dY2 + 3 * dSize / 2);
								}
								pRenderer->EndCommand(c_nTextGraphicType);
							}
						}
						void	DrawShading(IAVSRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight, long lColor)
						{	

							pRenderer->put_BrushColor1(lColor);
							pRenderer->BeginCommand(c_nPathType);
							//pRenderer->BeginCommand(c_nTextGraphicType);
							pRenderer->PathCommandStart();
							pRenderer->PathCommandMoveTo(dX, dY);
							pRenderer->PathCommandLineTo(dX + dWidth, dY);
							pRenderer->PathCommandLineTo(dX + dWidth, dY + dHeight);
							pRenderer->PathCommandLineTo(dX, dY + dHeight);
							pRenderer->PathCommandClose();
							pRenderer->DrawPath(c_nWindingFillMode);
							//pRenderer->DrawPath(1);
							pRenderer->PathCommandEnd();
							//pRenderer->EndCommand(c_nTextGraphicType);
							pRenderer->EndCommand(c_nPathType);
						}
						void	DrawWave(IAVSRenderer *pRenderer, double dX1, double dY1, double dX2, double dY2, double dSize)
						{
							pRenderer->BeginCommand(c_nPathType);
							pRenderer->BeginCommand(c_nTextGraphicType);
							pRenderer->PathCommandStart();
							pRenderer->PathCommandMoveTo(dX1, dY1);
							//2.29 -  
							double lenghtWave = 2.3;
							int dev = (int)((dX2 - dX1) / lenghtWave);
							if (dev == 0)
								dev = 1;
							double dStep = (dX2 - dX1) / dev;							
							for(double dX3 = dX1, dY3 = dY1; dX3 + 3 * dStep / 4 < dX2; dX3 += dStep )
							{								
								pRenderer->PathCommandCurveTo(dX3, dY3, dX3 + dStep / 4, dY3 - dSize, dX3 + dStep / 2, dY3 );
								pRenderer->PathCommandCurveTo(dX3 + dStep / 2, dY3, dX3 + 3 * dStep / 4, dY3 + dSize, dX3 + dStep, dY3 );
							}
							pRenderer->DrawPath(c_nStroke);
							pRenderer->PathCommandEnd();
							pRenderer->EndCommand(c_nTextGraphicType);
							pRenderer->EndCommand(c_nPathType);
						}
						void	DrawLine(IAVSRenderer *pRenderer, double dX1, double dY1, double dX2, double dY2)
						{
							pRenderer->BeginCommand(c_nPathType);
							pRenderer->BeginCommand(c_nTextGraphicType);
							pRenderer->PathCommandStart();
							pRenderer->PathCommandMoveTo(dX1, dY1);
							pRenderer->PathCommandLineTo(dX2, dY2);
							pRenderer->DrawPath(c_nStroke);
							pRenderer->PathCommandEnd();
							pRenderer->EndCommand(c_nTextGraphicType);
							pRenderer->EndCommand(c_nPathType);
						}

					public:
						void	MoveTo(const REAL &fNewX)
						{
							REAL fDeltaX = fNewX - m_oRect.X;							
							MoveOn(fDeltaX, 0);
						}						
						void	MoveTo(const REAL &fNewX, const REAL &fNewY)
						{
							REAL fDeltaX = fNewX - m_oRect.X;							
							REAL fDeltaY = fNewY - m_oRect.Y;
							MoveOn(fDeltaX, fDeltaY);
						}
						void	MoveOn(const REAL &fDeltaX, const REAL &fDeltaY)
						{
							m_oRect.X	+= fDeltaX;
							m_oRect.Y	+= fDeltaY;
							for (size_t i = 0; i < m_arrItems.GetCount(); ++i)
							{
								m_arrItems[i]->MoveOn(fDeltaX, fDeltaY);
							}
						}
					};
					class CLine : public CWord, public RendererCommandSender {
					protected:
						REAL	m_fPersistWidth;
						REAL	m_fTextHeight;

						size_t	m_nSpacesCount;
						bool	m_bIsContainsAlternate;
						INT		m_iFootnoteCount;

						bool _startCell;
						bool _finishCell;

						bool _dummy;

					public:
						CLine() 
							: CWord(), RendererCommandSender() 
							, m_fPersistWidth(0.0f), m_fTextHeight(11*1.147f)
							, m_nSpacesCount(0), m_bIsContainsAlternate(false)
							, m_iFootnoteCount(0)
							, _startCell(false), _finishCell(false)
							, _dummy(false) {
						}
						~CLine()
						{
							Clear();
						}
						void	Clear()
						{
							m_fPersistWidth	= 0.0f;
							m_fTextHeight	= 11 * 1.147f;

							m_arrItems.RemoveAll();
							m_nItemsCount	= 0;

							m_nSpacesCount			= 0;
							m_bIsContainsAlternate	= false;
							m_iFootnoteCount		= 0;
							
							_startCell = false;
							_finishCell = false;
						}
					public:
						virtual void	AddItem(CItem *pItem)
						{
							if (pItem->GetType() == Alternate)
							{
								m_bIsContainsAlternate	= true;
							}
							else
							{
								if (pItem->GetType() == Space)
								{
									++m_nSpacesCount;
								}
								else if (pItem->GetType() == Footnote)
								{
									++m_iFootnoteCount;
								}

								if (pItem->GetRect().Height > m_fTextHeight)
								{
									m_fTextHeight	= pItem->GetRect().Height;
								}
							}

							CWord::AddItem(pItem);
						}
						void			SetPersistWidth(const REAL &fPersistWidth)
						{
							m_fPersistWidth	= fPersistWidth;
						}
						REAL			GetPersistWidth()
						{
							return m_fPersistWidth;
						}
						virtual void SetAlign(const CAlignHorizontal& align, const bool& first = false, const bool& last = false)
						{
							if (m_nItemsCount != 0)
							{
								AbsentLastSpaces();
								if (!first)
									AbsentFormerSpaces();

								switch(align) {
								case LeftAlign :
									break;

								case CenterAlign :
									MoveOn(m_fPersistWidth/2, 0);
									break;

								case RightAlign :
									MoveOn(m_fPersistWidth, 0);
									break;

								case WideAlign :
									if (last )
										break;
									float deltaSpace = (m_nSpacesCount != 0) ? m_fPersistWidth / m_nSpacesCount : 0;
									if (deltaSpace != 0) {
										for (int i = 0; i < (int) m_nItemsCount; ++i) {
											CItem* item = m_arrItems[i];
											if (item->GetType() == Space)
												if (item->GetRect().Width != 0)
													item->GetRect().Width += deltaSpace;
										}
									}
									break;
								}

								RearrangeItems();
							}
						}
						REAL			GetTextHeight()
						{
							return m_fTextHeight;
						}

						void setStartCell(const bool& value = true) {
							_startCell = value;
						}
						void setFinishCell(const bool& value = true) {
							_finishCell = value;
						}
						void setDummy(const bool& dummy) {
							_dummy = dummy;
						}
						bool isDummy() const {
							return _dummy;
						}
					public:
						CAtlArray<CItem*>*	GetItems()
						{
							return &m_arrItems;
						}
						virtual void		RestoreItems()
						{
							size_t nItemsCount	= m_arrItems.GetCount();
							for (size_t nIndex = 0; nIndex < nItemsCount; ++nIndex)
							{
								m_arrItems[nIndex]->RestoreRect();
							}
						}
						virtual void		SetOnBaseline(const bool& bIsInit = true)
						{
							m_oRect.Y	= m_fDescent - m_oRect.Height;

							size_t nItemsCount	= m_arrItems.GetCount();
							for (size_t nIndex = 0; nIndex < nItemsCount; ++nIndex)
							{
								m_arrItems[nIndex]->SetOnBaseline(false);
							}
						}
						REAL				GetAscent()
						{
							return m_oRect.Height - m_fDescent;
						}
						inline BOOL			IsContainsAlternate()
						{
							return m_bIsContainsAlternate;
						}
						inline INT			GetFootnoteCount()
						{
							return m_iFootnoteCount;
						}
					public:
						void Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const float& completeness) {
							sendRendererCommands(renderer, RendererCommand::BEGIN);

							if (_startCell)
								renderer->BeginCommand(c_nTableCell);
							renderer->BeginCommand(c_nLine);

							CWord::Draw(renderer, documentManager, completeness);

							renderer->EndCommand(c_nLine);
							if (_finishCell)
								renderer->EndCommand(c_nTableCell);

							sendRendererCommands(renderer, RendererCommand::END);

							#ifdef TME_DRAW_LINE_BORDER
							CItem::drawRect(renderer, m_oRect);
							#endif
						}
					private:
						void	AbsentLastSpaces()
						{
							for (int nIndex = (int)m_nItemsCount - 1; nIndex >= 0; --nIndex)
							{
								CItem *pItem	= m_arrItems[nIndex];
								REAL fWidth		= pItem->GetRect().Width;

								switch (pItem->GetType())
								{
								case Space :
									((CSpace*)pItem)->SetOriginWidth();
									m_fPersistWidth		+= fWidth;
									m_oRect.Width		-= fWidth;
									pItem->GetRect().Width	= 0.0f;
									--m_nSpacesCount;
									break;

								case LineEnd :
									pItem->GetRect() = RectF();
									break;

								default :
									return;
								}
							}
						}
						void	AbsentFormerSpaces()
						{
							for (size_t nIndex = 0; nIndex < m_nItemsCount; ++nIndex)
							{
								CItem *pItem	= m_arrItems[nIndex];
								REAL fWidth		= pItem->GetRect().Width;

								switch (pItem->GetType())
								{
								case Space :
									((CSpace*)pItem)->SetOriginWidth();
									m_fPersistWidth		+= fWidth;

									m_oRect.Width		-= fWidth;

									pItem->GetRect().Width	= 0;
									--m_nSpacesCount;
									break;

								default :
									return;									
								}
							}
						}
					};
					class CCompositeLine : public CLine
					{
					public:
						CCompositeLine()
							: CLine()
						{
						}
						~CCompositeLine()
						{
							Clear();
						}
						void	Clear()
						{
							for (size_t nIndex = 0; nIndex < m_nItemsCount; ++nIndex)
							{
								RELEASEOBJECT(m_arrItems[nIndex]);
							}
							m_arrItems.RemoveAll();
							m_nItemsCount	= 0;
						}
					public:
						void	AddItem(CItem *pItem)
						{
							if (pItem->GetType() == Word)
							{
								m_iFootnoteCount += ((CLine*)pItem)->GetFootnoteCount();
							}
							if (m_oRect.GetTop() > pItem->GetRect().GetTop())
							{
								m_oRect.Height	+= m_oRect.GetTop() - pItem->GetRect().GetTop();
								m_oRect.Y		= pItem->GetRect().GetTop();
							}

							if (m_oRect.GetBottom() < pItem->GetRect().GetBottom())
							{
								m_oRect.Height	= pItem->GetRect().GetBottom() - m_oRect.GetTop();
							}

							m_fDescent	= m_oRect.GetBottom();

							m_arrItems.Add(pItem);
							++m_nItemsCount;
						}
						void	RestoreItems()
						{
							size_t nItemsCount	= m_arrItems.GetCount();
							for (size_t nIndex = 0; nIndex < nItemsCount; ++nIndex)
							{
								((CLine*)(m_arrItems[nIndex]))->RestoreItems();
							}
						}
						virtual void SetAlign(const CAlignHorizontal& eAlign, const bool& bIsFirstLine = false, const bool& last = false)
						{

							switch(eAlign)
							{
							case LeftAlign :
								break;

							case CenterAlign :
								for (size_t iIndex = 1; iIndex < m_arrItems.GetCount() - 2; ++iIndex)
								{
									((CLine*)(m_arrItems[iIndex]))->SetAlign(CenterAlign, false);
								}
								if (m_arrItems.GetCount() > 1)
								{
									((CLine*)(m_arrItems[0]))->SetAlign(RightAlign, bIsFirstLine);
									((CLine*)(m_arrItems[m_arrItems.GetCount() - 1]))->SetAlign(LeftAlign);
								}
								else
								{
									((CLine*)(m_arrItems[0]))->SetAlign(CenterAlign, bIsFirstLine);
								}
								break;

							case RightAlign :
								for (size_t iIndex = 0; iIndex < m_arrItems.GetCount() - 1; ++iIndex)
								{
									((CLine*)(m_arrItems[iIndex]))->SetAlign(RightAlign, bIsFirstLine && (iIndex == 0));
								}
								break;

							case WideAlign :
								if (last)
									break;
								for (size_t iIndex = 0; iIndex < m_arrItems.GetCount(); ++iIndex)
								{
									((CLine*)(m_arrItems[iIndex]))->SetAlign(WideAlign, bIsFirstLine && (iIndex == 0));
								}
								break;
							}
						}
					};
					// ### ### ###
					class CFlowMap;
					class CSimplePageMaster;
					class CPageSequenceMaster;
					class CLayoutMasterSet;
					class CFlow;
					class CBounds : private CXmlAttributeReader
					{
					private:
						bool	m_bIsMargin;

						RectF	m_oRectPt;

					public:
						CBounds()
							: m_bIsMargin(TRUE), 
							m_oRectPt()
						{
						}
						~CBounds()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							m_bIsMargin	= TRUE;

							m_oRectPt	= RectF();
						}
						void	FromXmlNodeFo(XmlUtils::CXmlNode& oXmlNode, const bool& withPaddings = false)
						{
							m_oRectPt.X		= FloatPtAttributeFromXml(oXmlNode, _T("margin-left"));
							m_oRectPt.Y		= FloatPtAttributeFromXml(oXmlNode, _T("margin-top"));
							m_oRectPt.Width		= FloatPtAttributeFromXml(oXmlNode, _T("margin-right"));
							m_oRectPt.Height	= FloatPtAttributeFromXml(oXmlNode, _T("margin-bottom"));

							if (withPaddings)
								ReadMarginsFromPaddings(oXmlNode);

							m_bIsMargin	= TRUE;
						}
						void	FromXmlNodeFo2(XmlUtils::CXmlNode& oXmlNode, const bool& withPaddings = false)
						{
							m_oRectPt.X		= FloatPtAttributeFromXml(oXmlNode, _T("margin-left"), 85.0f);
							m_oRectPt.Y		= FloatPtAttributeFromXml(oXmlNode, _T("margin-top"), 56.7f);
							m_oRectPt.Width		= FloatPtAttributeFromXml(oXmlNode, _T("margin-right"), 42.5f);
							m_oRectPt.Height	= FloatPtAttributeFromXml(oXmlNode, _T("margin-bottom"), 56.7f);

							if (withPaddings)
								ReadMarginsFromPaddings(oXmlNode);

							m_bIsMargin	= TRUE;
						}
					public:
						void	SetAsMarginPt(const REAL& fLeftMarginPt, const REAL& fTopMarginPt, 
							const REAL& fRightMarginPt, const REAL& fBottommarginPt)
						{
							m_oRectPt.X			= fLeftMarginPt;
							m_oRectPt.Y			= fTopMarginPt;
							m_oRectPt.Width		= fRightMarginPt;
							m_oRectPt.Height	= fBottommarginPt;

							m_bIsMargin	= TRUE;
						}
						void	SetAsMarginMm(const REAL& fLeftMarginMm, const REAL& fTopMarginMm, 
							const REAL& fRightMarginMm, const REAL& fBottommarginMm)
						{
							static const REAL fMmToPt	= 72.0f / 25.4f;

							m_oRectPt.X			= fLeftMarginMm * fMmToPt;
							m_oRectPt.Y			= fTopMarginMm * fMmToPt;
							m_oRectPt.Width		= fRightMarginMm * fMmToPt;
							m_oRectPt.Height	= fBottommarginMm * fMmToPt;

							m_bIsMargin	= TRUE;
						}
						void	AddMarginPt(const REAL& fLeftMarginPt, const REAL& fTopMarginPt, 
							const REAL& fRightMarginPt, const REAL& fBottomMarginPt)
						{
							assert(m_bIsMargin);
							m_oRectPt.X			+= fLeftMarginPt;
							m_oRectPt.Y			+= fTopMarginPt;
							m_oRectPt.Width		+= fRightMarginPt;
							m_oRectPt.Height	+= fBottomMarginPt;
						}
					public:
						void	SetAsRect(const CBounds& oBounds)
						{
							REAL fLeftMarginPt	= m_oRectPt.X;
							REAL fTopMarginPt	= m_oRectPt.Y;

							m_oRectPt.X			= oBounds.m_oRectPt.X + fLeftMarginPt;
							m_oRectPt.Y			= oBounds.m_oRectPt.Y + fTopMarginPt;
							m_oRectPt.Width		= oBounds.m_oRectPt.Width - fLeftMarginPt - m_oRectPt.Width;
							m_oRectPt.Height	= oBounds.m_oRectPt.Height - fTopMarginPt - m_oRectPt.Height;

							m_bIsMargin	= FALSE;
						}
						void	SetAsRectPt(const RectF& oRectPt)
						{
							if (m_bIsMargin)
							{
								REAL fLeftMarginPt	= m_oRectPt.X;
								REAL fTopMarginPt	= m_oRectPt.Y;

								m_oRectPt.X			= oRectPt.X + fLeftMarginPt;
								m_oRectPt.Y			= oRectPt.Y + fTopMarginPt;
								m_oRectPt.Width		= oRectPt.Width - fLeftMarginPt - m_oRectPt.Width;
								m_oRectPt.Height	= oRectPt.Height - fTopMarginPt - m_oRectPt.Height;

								m_bIsMargin	= FALSE;
							}
							else
							{
								m_oRectPt	= oRectPt;
							}
						}
						void	SetAsRectPt(const REAL& fWidthPt, const REAL& fHeightPt)
						{
							m_oRectPt.Width		= fWidthPt - m_oRectPt.X - m_oRectPt.Width;
							m_oRectPt.Height	= fHeightPt - m_oRectPt.Y - m_oRectPt.Height;

							m_bIsMargin	= FALSE;
						}
						void	SetAsRectMm(const RectF& oRectMm)
						{
							if (m_bIsMargin)
							{
								static const REAL fMmToPt	= 72.0f / 25.4f;

								REAL fLeftMarginPt	= m_oRectPt.X;
								REAL fTopMarginPt	= m_oRectPt.Y;

								m_oRectPt.X			= oRectMm.X * fMmToPt + fLeftMarginPt;
								m_oRectPt.Y			= oRectMm.Y * fMmToPt + fTopMarginPt;
								m_oRectPt.Width		= oRectMm.Width * fMmToPt - fLeftMarginPt - m_oRectPt.Width;
								m_oRectPt.Height	= oRectMm.Height * fMmToPt - fTopMarginPt - m_oRectPt.Height;

								m_bIsMargin	= FALSE;
							}
						}
						void	SetAsRectMm(const REAL& fWidthMm, const REAL& fHeightMm)
						{
							static const REAL fMmToPt	= 72.0f / 25.4f;

							m_oRectPt.Width		= fWidthMm * fMmToPt - m_oRectPt.X - m_oRectPt.Width;
							m_oRectPt.Height	= fHeightMm * fMmToPt - m_oRectPt.Y - m_oRectPt.Height;

							m_bIsMargin	= FALSE;
						}
						// ---
						void	SetAfter(const RectF& oRectPt)
						{
							m_oRectPt.Y	= oRectPt.GetBottom();
						}
						// ---
						RectF	GetRectPt() const
						{
							if (!m_bIsMargin)
								return m_oRectPt;
							else
								return m_oRectPt;
						}
					private:
						void	ReadMarginsFromPaddings(XmlUtils::CXmlNode& paddings)
						{
							m_oRectPt.X		+= FloatPtAttributeFromXml(paddings, _T("padding-left"));
							m_oRectPt.Y		+= FloatPtAttributeFromXml(paddings, _T("padding-top"));
							m_oRectPt.Width		+= FloatPtAttributeFromXml(paddings, _T("padding-right"));
							m_oRectPt.Height	+= FloatPtAttributeFromXml(paddings, _T("padding-bottom"));
						}
					};
					class CRegion : private CXmlAttributeReader
					{
					private:
						CString	m_strName;

						CBounds	m_oBounds;

						RectF	m_oRectPt;
						BYTE	m_nIsFilled;

						bool	m_bIsColumn;
						byte	m_BreakType;

					public:
						CRegion()
							: m_strName(_T("")), 
							m_oBounds(), 
							m_oRectPt(), m_nIsFilled(1), 
							m_bIsColumn(false), m_BreakType(0)
						{
						}
						~CRegion()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							m_strName	= _T("");

							m_oBounds.Clear();

							m_oRectPt	= RectF();
							m_nIsFilled	= 1;

							m_bIsColumn = false;
							m_BreakType = 0;
						}
						void	FromXmlNodeFo(XmlUtils::CXmlNode& oXmlNode, const bool& withPaddings = false)
						{
							m_strName	= oXmlNode.GetAttributeOrValue(_T("region-name"), _T(""));
							if (m_strName == _T(""))
							{
								LoadStandartRegionName(oXmlNode);
							}

							m_oBounds.FromXmlNodeFo(oXmlNode, withPaddings);

							ReadDisplayAlign(oXmlNode);
						}						
						void	FromRectMm(const RectF& rect, const bool& asMargin = false)
						{
							CString m_strName = _T("from-rect");

							m_oBounds.Clear();
							if (asMargin)
								m_oBounds.SetAsMarginMm(rect.X, rect.Y, rect.Width, rect.Height);
							else
								m_oBounds.SetAsRectMm(rect);

							SetComplete();
							CheckRect();
						}
					public:
						inline void	SetReady()
						{
							m_nIsFilled	= 0;
						}
						inline void	SetComplete()
						{
							m_nIsFilled	= 1;
						}
						inline void SetProcessing()
						{
							m_nIsFilled	= 2;
						}
						inline bool	IsReady()
						{
							return (m_nIsFilled == 0);
						}
						inline BOOL	IsComplete()
						{
							return (m_nIsFilled == 1);
						}
						inline BOOL	IsEqualName(const CString& strName)
						{
							return (m_strName == strName);
						}

						inline byte	BreakType()
						{
							return m_BreakType;
						}
						inline void BreakType(const byte& breakType)
						{
							m_BreakType = breakType;
						}
					public:
						inline const RectF&	GetRectPt()	const
						{
							return m_oRectPt;
						}
						inline CBounds		GetBounds()
						{
							return m_oBounds;
						}
						inline RectF		GetOriginalRectPt() const
						{
							return m_oBounds.GetRectPt();
						}
						RectF				GetFilledRectPt() const
						{
							RectF boundsRect = m_oBounds.GetRectPt();
							return RectF(boundsRect.GetLeft(), boundsRect.GetTop(), m_oRectPt.GetRight() - m_oRectPt.GetLeft(), m_oRectPt.GetTop() - boundsRect.GetTop());							
						}
						inline REAL			GetRegionHeightPt()	const
						{
							return m_oRectPt.Height;
						}
						inline void			SetIsColumn()
						{
							m_bIsColumn = true;
						}
						inline bool			IsColumn() {
							return m_bIsColumn;
						}
						double				GetFilledHeight()
						{
							RectF originRect = GetOriginalRectPt();
							RectF rect		 = GetRectPt();
							double originHeight = originRect.GetBottom() - originRect.GetTop();
							double height		= rect.GetBottom() - rect.GetTop();
							return (originHeight - height);
						}
						double				GetFilledWidth()
						{
							RectF originRect = GetOriginalRectPt();
							double originWidth = originRect.GetRight() - originRect.GetLeft();
							return originWidth;
						}
						void				CheckRect()
						{
							if (IsComplete())
							{
								m_oRectPt	= m_oBounds.GetRectPt();
								SetReady();
							}
						}
						inline void			TrancateRectPt(const REAL &fHeightPt)
						{
							m_oRectPt.Y			+= fHeightPt;
							m_oRectPt.Height	-= fHeightPt;
						}
						inline void			TruncateHeightPt(const REAL &fHeightPt)
						{
							m_oRectPt.Height	-= fHeightPt;
						}
						inline void			TruncateLeftPt(const float& delta)
						{
							m_oRectPt.X += delta;
							m_oRectPt.Width += delta;
						}
						void				ChangeRect(CBounds oBoundsPt)
						{
							oBoundsPt.SetAsRectPt(m_oRectPt);
							m_oBounds	= oBoundsPt;
							m_oRectPt	= m_oBounds.GetRectPt();
						}
						void				SetRect(RectF &oRectPt)
						{
							m_oBounds.SetAsRectPt(oRectPt);
							m_oRectPt	= m_oBounds.GetRectPt();
						}
					public:
						void	SetBounds(const CBounds& oBounds)
						{
							m_oBounds.SetAsRect(oBounds);
						}
						void	AddBounds(const REAL& fLeftMarginPt, const REAL& fTopMarginPt, 
							const REAL& fRightMarginPt, const REAL& fBottommarginPt)
						{
							m_oBounds.AddMarginPt(fLeftMarginPt, fTopMarginPt, fRightMarginPt, fBottommarginPt);
						}
					public:
						void	ApplyDisplayAlign(RectF& oRectPt)
						{
							if (m_strName == _T("xsl-region-body"))
							{
								oRectPt	= m_oBounds.GetRectPt();
							}
							if (m_nIsFilled == 4)
							{
								m_oBounds.SetAfter(oRectPt);
							}
						}
					private:
						void	LoadStandartRegionName(XmlUtils::CXmlNode& oXmlNode)
						{
							CString strNodeName	= oXmlNode.GetName();

							if (strNodeName == _T("fo:region-body"))
							{
								m_strName	= _T("xsl-region-body");
							}
						}
						void	ReadDisplayAlign(XmlUtils::CXmlNode& oXmlNode)
						{
							CString strDisplayAlign	= oXmlNode.GetAttributeOrValue(_T("display-align"), _T(""));

							if (strDisplayAlign == _T("after"))
							{
								m_nIsFilled	= 4;
							}
							else
							{
								m_nIsFilled	= 3;
							}
						}
					};

					// ### ### ###
					class CBlock;
					class CContent;
					class CPictureC;
					class CShapeC;
					class CBlockC;
					class CTextC;
					class CPageNumberC;
					class CAlternate2 : public CItem, private CXmlAttributeReader {
					public:
						class CAreaWrap;
						enum EWrapType {
							WrapRuntime = 0, WrapInline, WrapNone, 
							WrapTopAndBottom, WrapSquare, WrapThrough, WrapTight
						};
						struct SSidesF
						{
							float	m_fTop;
							float	m_fBottom;
							float	m_fLeft;
							float	m_fRight;

							SSidesF() :
							m_fTop(0), m_fBottom(0), 
								m_fLeft(0), m_fRight(0)
							{
							}
						};
						class CAreaProperties : private CXmlAttributeReader2 {
							enum EActual {
								ActualComplete = 0, ActualPartial, ActualNone
							};

						public:
							class GeometryProperties : private CXmlAttributeReader2 {
							private:
								bool _horizontalFlip;
								bool _verticalFlip;

								float _rotate;

								bool _anchored;

							public:
								GeometryProperties() {
									_horizontalFlip = false;
									_verticalFlip = false;

									_rotate = 0;

									_anchored = false;
								}

							public:
								void loadFromTransformXml(XmlUtils::CXmlNode& transformXml) {
									setRotate(transformXml.GetAttribute(_T("rotate")));
									setHFlip(transformXml.GetAttribute(_T("hFlip")));
									setVFlip(transformXml.GetAttribute(_T("vFlip")));
									if (_horizontalFlip != _verticalFlip)
										_rotate *= -1;
								}
								void loadFromBoundsXml(XmlUtils::CXmlNode& boundsXml) {
									setRotate(AttrToStr(boundsXml, _T("rotation"), _T("val")));
									setFlips(AttrToStr(boundsXml, _T("flip"), _T("val")));
									if (_horizontalFlip != _verticalFlip)
										_rotate *= -1;
								}

							private:
								void setFlips(const CString& flip) {
									if (flip.Find(_T('x')) != -1)
										_horizontalFlip = true;
									if (flip.Find(_T('y')) != -1)
										_verticalFlip = true;
								}
								void setHFlip(const CString& flip) {
									_horizontalFlip = XmlUtils::GetBoolean(flip) == TRUE;
								}
								void setVFlip(const CString& flip) {
									_verticalFlip = XmlUtils::GetBoolean(flip) == TRUE;
								}
								void setRotate(const CString& rotate) {
									if (rotate != _T(""))
										_rotate = XmlUtils::GetFloat(rotate) * M_PI / 180;
								}

							public:
								void setRotationAngle(const float& rotate) {
									_rotate = rotate;
								}

								void changeHFlip() {
									_horizontalFlip = !_horizontalFlip;
									_rotate *= -1;
								}
								void changeVFlip() {
									_verticalFlip = !_verticalFlip;
									_rotate *= -1;
								}
							public:
								const bool& getHorzFlip() const {
									return _horizontalFlip;
								}
								const bool& getVertFlip() const {
									return _verticalFlip;
								}
								const float& getRotationAngle() const {
									return _rotate;
								}
							public:
								void setAnchored(const bool& anchored) {
									_anchored = anchored;
								}
								bool isAnchored() const {
									return _anchored;
								}
							};
							
						private:
							CRelativeFrom	m_eRelHorz;
							CRelativeFrom	m_eRelVert;
							PointF	m_oRelPoint;
							RectF	m_oArea;
							SSidesF _stretch;
							PointF _contraint;

							RectF	m_oRect;
							EActual	m_eActualOfRect;

							CAlignHorizontal	m_eAlignHorz;
							CAlignVertical		m_eAlignVert;

							GeometryProperties _geometryProperties;

							float _zIndex;

						private:
							bool	ReadIsRuntime(XmlUtils::CXmlNode& oPropertyNode)
							{
								CString strType = oPropertyNode.GetAttribute(_T("type"));
								if (strType == _T("runtime"))
									return true;
								else
									return false;
							}
							void	ReadArea(RectF& area, XmlUtils::CXmlNode& propertyXml) {
								area.X = TextToPt(propertyXml, _T("fo:position-x"));
								area.Y = TextToPt(propertyXml, _T("fo:position-y"));
								area.Width = TextToPt(propertyXml, _T("fo:extent-x"));
								area.Height = TextToPt(propertyXml, _T("fo:extent-y"));

								XmlUtils::CXmlNode transformXml;
								if (propertyXml.GetNode(_T("transform"), transformXml))
									_geometryProperties.loadFromTransformXml(transformXml);
							}
							CRelativeFrom	ReadRelHorz(XmlUtils::CXmlNode& propertyXml) {
								CString relative = AttrToStr(propertyXml, _T("fo:position-x"), _T("relative-from"));
								return GetRelHorz(relative);
							}
							CRelativeFrom	ReadRelVert(XmlUtils::CXmlNode& propertyXml) {
								CString relative = AttrToStr(propertyXml, _T("fo:position-y"), _T("relative-from"));
								return GetRelVert(relative);
							}
							CRelativeFrom	GetRelHorz(const CString& relativeHorz) {
								if (relativeHorz == _T("page"))
									return FromPage;
								else if (relativeHorz == _T("margin"))
									return FromMargin;
								else
									return FromColumn;
							}
							CRelativeFrom	GetRelVert(const CString& relativeVert) {
								if (relativeVert == _T("page"))
									return FromPage;
								else if (relativeVert == _T("margin"))
									return FromMargin;
								else if (relativeVert == _T("line"))
									return FromLine;
								else
									return FromParagraph;
							}

							void readStretch(XmlUtils::CXmlNode& propertyXml) {
								XmlUtils::CXmlNode stretchXml;
								if (propertyXml.GetNode(_T("stretch"), stretchXml)) {
									_stretch.m_fTop = stretchXml.ReadAttributeFloat(_T("top"), 0) / 100;
									_stretch.m_fBottom = stretchXml.ReadAttributeFloat(_T("bottom"), 0) / 100;
									_stretch.m_fLeft = stretchXml.ReadAttributeFloat(_T("left"), 0) / 100;
									_stretch.m_fRight = stretchXml.ReadAttributeFloat(_T("right"), 0) / 100;
								}
							}
							void readAlign(XmlUtils::CXmlNode& propertyXml) {
								CString align = AttrToStr(propertyXml, _T("fo:position-x"), _T("align"));
								m_eAlignHorz = GetAlignHorz(align);

								align = AttrToStr(propertyXml, _T("fo:position-y"), _T("align"));
								m_eAlignVert = GetAlignVert(align);
							}
							void	FromContentXml(XmlUtils::CXmlNode& contentNode) {
								XmlUtils::CXmlNode intContentNode = contentNode;
								XmlUtils::CXmlNode alternateNode;
								if (contentNode.GetNode(_T("fo:alternate-content"), alternateNode))
									if (!alternateNode.GetNode(_T("fo:content"), intContentNode))
										intContentNode = contentNode;

								bool nodeRead = true;
								XmlUtils::CXmlNode oShapeNode;
								if (!intContentNode.GetNode(_T("shape"), oShapeNode))
									if (!intContentNode.GetNode(_T("group"), oShapeNode))
										nodeRead = false;
								
								if (nodeRead) {
									ReadAreaFromContentBounds(m_oArea, oShapeNode);
									ReadAlignFromContentBounds(m_eAlignHorz, m_eAlignVert, oShapeNode);
									ReadRelFromContentBounds(m_eRelHorz, m_eRelVert, oShapeNode);
									ReadZIndexFromContentBounds(_zIndex, oShapeNode);
								}
							}
							void	ReadAreaFromContentBounds(RectF& oArea, XmlUtils::CXmlNode& oShapeNode)
							{
								XmlUtils::CXmlNode oBoundsNode;
								if (oShapeNode.GetNode(_T("bounds"), oBoundsNode)) {
									float left = AttrToPt(oBoundsNode, _T("left"), _T("val"));
									oArea.X = AttrToPt(oBoundsNode, _T("marginLeft"), _T("val"), left);
									float top = AttrToPt(oBoundsNode, _T("top"), _T("val"));
									oArea.Y = AttrToPt(oBoundsNode, _T("marginTop"), _T("val"), top);
									oArea.Width = AttrToPt(oBoundsNode, _T("width"), _T("val"));
									oArea.Height = AttrToPt(oBoundsNode, _T("height"), _T("val"));

									_geometryProperties.loadFromBoundsXml(oBoundsNode);
								}
							}
							void	ReadAlignFromContentBounds(CAlignHorizontal& eAlignHorz, CAlignVertical& eAlignVert, XmlUtils::CXmlNode& oShapeNode)
							{
								XmlUtils::CXmlNode oBoundsNode;
								if (oShapeNode.GetNode(_T("bounds"), oBoundsNode))
								{
									CString strAlign = AttrToStr(oBoundsNode, _T("PosHor"), _T("val"));
									m_eAlignHorz = GetAlignHorz(strAlign);
									strAlign = AttrToStr(oBoundsNode, _T("PosVert"), _T("val"));
									m_eAlignVert = GetAlignVert(strAlign);
								}
							}
							void	ReadRelFromContentBounds(CRelativeFrom& relHorz, CRelativeFrom& relVert, XmlUtils::CXmlNode& shape)
							{
								XmlUtils::CXmlNode bounds;
								if (shape.GetNode(_T("bounds"), bounds))
								{
									relHorz = GetRelHorz(AttrToStr(bounds, _T("PosHorRel"), _T("val")));
									relVert = GetRelVert(AttrToStr(bounds, _T("PosVertRel"), _T("val")));
								}
							}
							void	ReadZIndexFromContentBounds(float& zIndex, XmlUtils::CXmlNode& shape)
							{
								XmlUtils::CXmlNode bounds;
								if (shape.GetNode(_T("bounds"), bounds))
									zIndex = XmlUtils::GetFloat(AttrToStr(bounds, _T("z-index"), _T("val")));
							}
							CAlignHorizontal GetAlignHorz(const CString& strAlignHorz)
							{
								CAlignHorizontal eAlignHorz = NoneAlign;
								if (strAlignHorz == _T("left"))
									eAlignHorz = LeftAlign;
								else if (strAlignHorz == _T("center"))
									eAlignHorz = CenterAlign;
								else if (strAlignHorz == _T("right"))
									eAlignHorz = RightAlign;
								return eAlignHorz;
							}
							CAlignVertical GetAlignVert(const CString& strAlignVert)
							{
								CAlignVertical eAlignVert = NoneAlignVert;
								if (strAlignVert == _T("top"))
									eAlignVert = FarAlignVert;
								else if (strAlignVert == _T("center"))
									eAlignVert = CenterAlignVert;
								else if (strAlignVert == _T("bottom"))
									eAlignVert = NearAlignVert;
								return eAlignVert;
							}
						private:
							void UpdatePosition(EActual actual) {
								if (m_eActualOfRect == ActualNone 
										&& (actual == ActualPartial || actual == ActualComplete)) {
									m_oRect = m_oArea;
									SetRectFromRelPoint(m_oRect);
									m_eActualOfRect = ActualPartial;
								}
								if (m_eActualOfRect == ActualPartial 
										&& actual == ActualComplete) {
									m_oRect.X += _contraint.X;
									m_oRect.Y += _contraint.Y;
									m_eActualOfRect = ActualComplete;
								}

							}
							void	SetRelPoint(const CPage& oPage, const CRegion& oRegion, const RectF& line) {
								SetRelPointHorz(oRegion);
								SetRelPointVert(oRegion, line);
							}
							void	SetRelPointHorz(const CRegion& oRegion)
							{
								if (m_eRelHorz == FromColumn)
									m_oRelPoint.X = oRegion.GetRectPt().X;
								else
									m_oRelPoint.X = 0;
							}
							void	SetRelPointVert(const CRegion& oRegion, const RectF& line)	{
								if (FromParagraph == m_eRelVert)
									m_oRelPoint.Y = oRegion.GetRectPt().Y;
								else if (FromLine == m_eRelVert)
									m_oRelPoint.Y = line.GetBottom();
								else
									m_oRelPoint.Y = 0;
							}
							void	Align(const CPage& page, const CRegion& region, const RectF& line) {
								AlignHorz(page, region);
								AlignVert(page, region, line);
							}
							void	AlignHorz(const CPage& page, const CRegion& oRegion)
							{
								if (m_eAlignHorz == NoneAlign)
									return;
								else if (m_eAlignHorz == LeftAlign) {
									if (m_eRelHorz == FromPage)
										m_oRelPoint.X = 0;
									else
										m_oRelPoint.X = oRegion.GetRectPt().X;
									m_oArea.X = 0;
								}
								else if (m_eAlignHorz == CenterAlign) {
									if (m_eRelHorz == FromPage)
										m_oRelPoint.X = page.GetWidthPt()/2;
									else
										m_oRelPoint.X = oRegion.GetRectPt().X + oRegion.GetRectPt().Width/2;
									m_oArea.X = -m_oArea.Width/2;
								}
								else if (m_eAlignHorz == RightAlign) {
									if (m_eRelHorz == FromPage)
										m_oRelPoint.X = page.GetWidthPt();
									else
										m_oRelPoint.X = oRegion.GetRectPt().GetRight();
									m_oArea.X = -m_oArea.Width;
								}
							}
							void	AlignVert(const CPage& page, const CRegion& oRegion, const RectF& line)	{
								if (NoneAlignVert == m_eAlignVert)
									return;
								if (FarAlignVert == m_eAlignVert) {
									if (FromPage == m_eRelVert)
										m_oRelPoint.Y = 0;
									else if (FromLine == m_eRelVert)
										m_oRelPoint.Y = line.GetBottom();
									else
										m_oRelPoint.Y = oRegion.GetOriginalRectPt().GetTop();
									m_oArea.Y = 0;
								}
								else if (m_eAlignVert == CenterAlignVert) {
									if (m_eRelVert == FromPage)
										m_oRelPoint.Y = page.GetHeightPt()/2;
									else
										m_oRelPoint.Y = oRegion.GetOriginalRectPt().Y + oRegion.GetOriginalRectPt().Height/2;
									m_oArea.Y = -m_oArea.Height/2;
								}
								else if (m_eAlignVert == NearAlignVert) {
									if (FromPage == m_eRelVert)
										m_oRelPoint.Y = page.GetHeightPt();
									else if (FromLine == m_eRelVert)
										m_oRelPoint.Y = line.GetBottom();
									else
										m_oRelPoint.Y = oRegion.GetOriginalRectPt().GetBottom();
									m_oArea.Y = -m_oArea.Height;
								}
							}

						private:
							void	SetRectFromRelPoint(RectF& oRect)
							{
								oRect.X += m_oRelPoint.X;
								oRect.Y += m_oRelPoint.Y;
							}
						public:
							CAreaProperties() 
									: m_eRelHorz(FromPage), m_eRelVert(FromPage)
									, m_eActualOfRect(ActualNone)
									, m_eAlignHorz(NoneAlign), m_eAlignVert(NoneAlignVert)
									, _zIndex(0) {}
						public:
							void	SetAreaRect(RectF& rect, const bool& onlySize = false) {
								if (onlySize) {
									m_oArea.Width = rect.Width;
									m_oArea.Height = rect.Height;
								}
								else 
									m_oArea = rect;
							}
							void	SetRect(RectF& oRect)
							{
								m_oRect = oRect;
							}
						public:
							byte	FromXml(XmlUtils::CXmlNode& alternateNode) {
								XmlUtils::CXmlNode propertyNode;
								if (!alternateNode.GetNode(_T("fo:property"), propertyNode))
									return 1;
								XmlUtils::CXmlNode contentNode;
								if (!alternateNode.GetNode(_T("fo:content"), contentNode))
									return 1;

								return FromXml(propertyNode, contentNode);
							}
							byte	FromXml(XmlUtils::CXmlNode& oPropertyNode, XmlUtils::CXmlNode& oContentNode) {
								ReadArea(m_oArea, oPropertyNode);

								m_eRelHorz = ReadRelHorz(oPropertyNode);
								m_eRelVert = ReadRelVert(oPropertyNode);
								readStretch(oPropertyNode);
								readAlign(oPropertyNode);

								//bool bIsRuntime = ReadIsRuntime(oPropertyNode);
								//if (bIsRuntime)
									FromContentXml(oContentNode);

								return 0;
							}
							void	Relate(const CPage& oPage, const CRegion& oRegion, const RectF& line) {
								SetRelPoint(oPage, oRegion, line);
								Align(oPage, oRegion, line);
								m_eActualOfRect = ActualNone;
							}
							bool checkContraints(const CPage& page ) {
								UpdatePosition(ActualPartial);

								if (m_oRect.X < 0)
									_contraint.X = -m_oRect.X;
								if (m_oRect.Y < 0)
									_contraint.Y = -m_oRect.Y;
								float deltaX = m_oRect.GetRight() - page.GetWidthPt();
								if (deltaX > 0)
									_contraint.X = -deltaX;
								if (m_oRect.GetBottom() > page.GetHeightPt())
									return false;

								return true;
							}
						public:
							const RectF&	RectPt() {
								UpdatePosition(ActualComplete);
								return m_oRect;
							}
							const RectF&	AreaRectPt() {
								return m_oArea;
							}
							const PointF&	RelPointPt()
							{
								return m_oRelPoint;
							}
							CRelativeFrom	RelHorz() const
							{
								return m_eRelHorz;
							}
							CRelativeFrom	RelVert() const
							{
								return m_eRelVert;
							}

							float getZIndex() const
							{
								return _zIndex;
							}
							RectF getDrawingRect(const RectF& rect) const {
								RectF drawingRect(rect);

								float offsetTop = rect.Height * _stretch.m_fTop;
								float offsetBottom = rect.Height * _stretch.m_fBottom;
								drawingRect.Y += offsetTop;
								drawingRect.Height -= offsetTop + offsetBottom;
								
								float offsetLeft = rect.Width * _stretch.m_fLeft;
								float offsetRight = rect.Width * _stretch.m_fRight;
								drawingRect.X += offsetLeft;
								drawingRect.Width -= offsetLeft + offsetRight;

								return drawingRect;
							}
							const GeometryProperties& getGeomPr() const {
								return _geometryProperties;
							}
							void setAnchored(const EWrapType& wrapType) {
								_geometryProperties.setAnchored(wrapType == WrapInline);
							}
						};
						class CAreaWrap : private CXmlAttributeReader2
						{
						private:
							EWrapType	m_eWrapType;
							int			m_iInlinePosition;

							CArray<PointF>	m_arrPointsOrigin;
							CArray<PointF>	m_arrPoints;
							SSidesF	m_oDistanceOrigin;
							SSidesF	m_oDistance;

						private:
							bool	ReadIsRuntime(XmlUtils::CXmlNode& oPropertyNode)
							{
								CString strType = oPropertyNode.GetAttribute(_T("type"));
								if (strType == _T("runtime"))
									return true;
								else
									return false;
							}
							byte	FromContentXml(XmlUtils::CXmlNode& oContentNode)
							{
								bool nodeRead = true;
								XmlUtils::CXmlNode oShapeNode;
								if (!oContentNode.GetNode(_T("shape"), oShapeNode))
									if (!oContentNode.GetNode(_T("group"), oShapeNode))
										nodeRead = false;
									
								if (nodeRead) {
									m_eWrapType = ReadContentWrapType(oShapeNode);
									ReadDistanceFromContentBounds(m_oDistance, oShapeNode);
									return 0;
								}

								return 1;
							}
							EWrapType	ReadWrapType(XmlUtils::CXmlNode& oPropertyNode)
							{
								CString strWrapType = oPropertyNode.GetAttribute(_T("type"));
								XmlUtils::CXmlNode oWrapNode;
								if (oPropertyNode.GetNode(_T("fo:wrap"), oWrapNode))
									strWrapType = oWrapNode.GetAttribute(_T("type"));
								
								return GetWrapType(strWrapType);
							}
							EWrapType	ReadContentWrapType(XmlUtils::CXmlNode& oShapeNode)
							{
								CString strWrapType = _T("none");
								XmlUtils::CXmlNode oWrapNode;
								if (oShapeNode.GetNode(_T("w10:wrap"), oWrapNode))
								{
									strWrapType = oWrapNode.GetAttribute(_T("type"));
									if (strWrapType == _T(""))
									{
										CString anchorX = oWrapNode.GetAttribute(_T("anchorx"));
										CString anchorY = oWrapNode.GetAttribute(_T("anchory"));
										bool wrapNoneX = false;
										if ((anchorX == _T("page")) || (anchorX == _T("margin")))
											wrapNoneX = true;
										bool wrapNoneY = false;
										if ((anchorY == _T("page")) || (anchorY == _T("margin")))
											wrapNoneY = true;
										if (wrapNoneX && wrapNoneY)
											return WrapNone;
									}
								}
								if (strWrapType == _T("none")) {
									XmlUtils::CXmlNode oBoundsNode;
									if (oShapeNode.GetNode(_T("bounds"), oBoundsNode))
									{
										CString pos = AttrToStr(oBoundsNode, _T("Pos"), _T("val"));
										if ((pos == _T("")) || (pos == _T("static")))
											strWrapType = _T("inline");
									}
								}
								
								return GetWrapType(strWrapType);
							}
							void	ReadDistance(SSidesF& oDistance, XmlUtils::CXmlNode& oPropertyNode)
							{
								XmlUtils::CXmlNode anchorNode;
								if (oPropertyNode.GetNode(_T("fo:anchor"), anchorNode))
								{
									oDistance.m_fLeft = AttrToPt(anchorNode, _T(""), _T("distance-left"), 0);
									oDistance.m_fRight = AttrToPt(anchorNode, _T(""), _T("distance-right"), 0);
									oDistance.m_fTop = AttrToPt(anchorNode, _T(""), _T("distance-top"), 0);
									oDistance.m_fBottom = AttrToPt(anchorNode, _T(""), _T("distance-bottom"), 0);
								}
							}
							void	ReadDistanceFromContentBounds(SSidesF& oDistance, XmlUtils::CXmlNode& oShapeNode)
							{
								XmlUtils::CXmlNode oBoundsNode;
								if (oShapeNode.GetNode(_T("bounds"), oBoundsNode))
								{
									oDistance.m_fLeft = AttrToPt(oBoundsNode, _T("wrapDistLeft"), _T("val"), 9);
									oDistance.m_fRight = AttrToPt(oBoundsNode, _T("wrapDistRight"), _T("val"), 9);
									oDistance.m_fTop = AttrToPt(oBoundsNode, _T("wrapDistTop"), _T("val"));
									oDistance.m_fBottom = AttrToPt(oBoundsNode, _T("wrapDistBottom"), _T("val"));
								}
							}
						private:
							void	Points(const RectF& oRect)
							{
								m_arrPoints.RemoveAll();
								m_arrPoints.Add(PointF(oRect.GetLeft(), oRect.GetTop()));
								m_arrPoints.Add(PointF(oRect.GetRight(), oRect.GetTop()));
								m_arrPoints.Add(PointF(oRect.GetRight(), oRect.GetBottom()));
								m_arrPoints.Add(PointF(oRect.GetLeft(), oRect.GetBottom()));
								m_arrPoints.Add(PointF(oRect.GetLeft(), oRect.GetTop()));
							}
							void	Points(const CArray<PointF>& arrPoints)
							{
								m_arrPoints.RemoveAll();
								m_arrPoints.Copy(arrPoints);
							}
							EWrapType	GetWrapType(const CString strWrapType)
							{
								EWrapType eReturnWrapType = WrapInline;

								if (strWrapType == _T("none"))
									eReturnWrapType = WrapNone;
								else if (strWrapType == _T("topAndBottom"))
									eReturnWrapType = WrapTopAndBottom;
								else if (strWrapType == _T("square"))
									eReturnWrapType = WrapSquare;
								else if (strWrapType == _T("through"))
									eReturnWrapType = WrapThrough;
								else if (strWrapType == _T("tight"))
									eReturnWrapType = WrapTight;
								else if (strWrapType == _T("inline"))
									eReturnWrapType = WrapInline;

								return eReturnWrapType;
							}
						private:
							void	ApplyTopAndBottom(CAreaProperties& oAreaProperties, const CRegion& oRegion)
							{
								RectF& oRegionRect = oRegion.GetOriginalRectPt();
								RectF oAreaRect = oAreaProperties.RectPt();

								float distLeft = oAreaRect.GetLeft() - oRegionRect.GetLeft();
								float distRight = oRegionRect.GetRight() - oAreaRect.GetRight();

								if ((distLeft >= 0) && (distRight >= 0))
								{
									m_oDistance.m_fLeft = distLeft;
									m_oDistance.m_fRight = distRight;
									Points(oAreaRect);
								}
								else
									ApplySquare(oAreaProperties, oRegion);
							}

							void	ApplySquare(CAreaProperties& oAreaProperties, const CRegion& oRegion)
							{
								RectF oAreaRect = oAreaProperties.RectPt();
								oAreaRect.Y -= m_oDistance.m_fTop;
								oAreaRect.Height += m_oDistance.m_fTop + m_oDistance.m_fBottom;

								Points(oAreaRect);
							}
							void	ApplyTight(CAreaProperties& oAreaProperties, const CRegion& oRegion)
							{
								RectF oAreaRect = oAreaProperties.RectPt();
								oAreaRect.Y -= m_oDistance.m_fTop;
								oAreaRect.Height += m_oDistance.m_fTop + m_oDistance.m_fBottom;

								Points(oAreaRect);
							}
						public:
							CAreaWrap() : 
							  m_eWrapType(WrapNone), m_iInlinePosition(0), 
								  m_arrPointsOrigin(), m_arrPoints(), 
								  m_oDistanceOrigin(), m_oDistance()
							  {
							  }
							  ~CAreaWrap()
							  {
							  }

						public:
							byte	FromXml(XmlUtils::CXmlNode& oPropertyNode, XmlUtils::CXmlNode& oContentNode)
							{
								m_eWrapType = ReadWrapType(oPropertyNode);
								ReadDistance(m_oDistance, oPropertyNode);

								bool bIsRuntime = ReadIsRuntime(oPropertyNode);
								if (bIsRuntime)
									FromContentXml(oContentNode);

								return 0;
							}

							int		InlinePosition()
							{
								return m_iInlinePosition;
							}
							void	InlinePosition(const int& iPosition)
							{
								m_iInlinePosition = iPosition;
							}
							EWrapType	WrapType()
							{
								return m_eWrapType;
							}
							const CArray<PointF>&	Points()
							{
								return m_arrPoints;
							}
							const SSidesF&	Distance() const
							{
								return m_oDistance;
							}
						public:
							bool	Inline()
							{
								return (m_eWrapType == WrapInline);
							}
							void	Inline(const bool& bIsInline)
							{
								if (bIsInline == true)
									m_eWrapType = WrapInline;
								else
									assert(false);
							}
						public:
							void Wrap(CAreaProperties& areaProperties, const CRegion& region) {
								areaProperties.setAnchored(m_eWrapType);

								if (m_eWrapType == WrapTopAndBottom)
									ApplyTopAndBottom(areaProperties, region);
								else if (m_eWrapType == WrapSquare)
									ApplySquare(areaProperties, region);
								else if (m_eWrapType == WrapTight)
									ApplyTight(areaProperties, region);
							}
						};

						class CAltContent {
						private:
							enum EContentType {
								ContentShape = 0, ContentPicture, ContentGroup, ContentLargeLetter, ContentNone
							};
						private:
							CContent*	m_pContent;
							CTextC* _Text;
							EContentType	m_eType;
							bool	m_bPlaced;

						private:
							byte	FromPropertyXml(XmlUtils::CXmlNode& oPropertyNode)
							{
								XmlUtils::CXmlNode graphicsNode;
								if (oPropertyNode.GetNode(_T("fo:graphic"), graphicsNode))
									m_eType = ReadContentType(graphicsNode);
								if (m_eType == ContentPicture)
									if (!PictureFromXml(graphicsNode))
										return 0;

								return 1;
							}
							EContentType	ReadContentType(XmlUtils::CXmlNode& graphicsNode)
							{
								CString strContentType = graphicsNode.GetAttribute(_T("type"));
								if (strContentType == _T("shape"))
									return ContentShape;
								else if (strContentType == _T("picture"))
									return ContentPicture;
								else if (strContentType == _T("LargeLetter"))
									return ContentLargeLetter;
								else if (strContentType == _T("group"))
									return ContentGroup;

								return ContentNone;
							}

							byte	FromContentXml(XmlUtils::CXmlNode& oContentNode, CDocumentManager& documentManager)
							{
								if (m_eType == ContentShape)
									return ShapeFromXml(oContentNode, documentManager);
								else if (m_eType == ContentLargeLetter)
									return LargeLetterFromXml(oContentNode, documentManager);
								else if (m_eType == ContentGroup)
									return GroupFromXml(oContentNode, documentManager);

								return 1;
							}
							byte	ReadContentTypeFromContent(XmlUtils::CXmlNode& alternateNode)
							{
								XmlUtils::CXmlNode contentNode;
								if (alternateNode.GetNode(_T("fo:content"), contentNode)) {
									XmlUtils::CXmlNode innerAlternateNode;
									if (contentNode.GetNode(_T("fo:alternate-content"), innerAlternateNode)) {
										XmlUtils::CXmlNode propertyNode;
										if (innerAlternateNode.GetNode(_T("fo:property"), propertyNode))
											return FromPropertyXml(propertyNode);
									}
								}

								return 1;
							}
							byte	PictureFromXml(XmlUtils::CXmlNode& oContentNode)
							{
								CPictureC* pPicture = new CPictureC();
								if (!pPicture->FromXml(oContentNode))
								{
									m_pContent = pPicture;
									return 0;
								}
								return 1;
							}
							byte	ShapeFromXml(XmlUtils::CXmlNode& oContentNode, CDocumentManager& documentManager)
							{
								XmlUtils::CXmlNode oIntContentNode = oContentNode;
								XmlUtils::CXmlNode oAlternateNode;
								if (oContentNode.GetNode(_T("fo:alternate-content"), oAlternateNode))
									if (!oAlternateNode.GetNode(_T("fo:content"), oIntContentNode))
										oIntContentNode = oContentNode;

								CShapeC* pShape = new CShapeC();
								if (!pShape->FromXml(oIntContentNode)) {
									m_pContent = pShape;
									TextFromXml(oIntContentNode, documentManager);
								}

								return (m_pContent != NULL) ? 0 : 1;
							}
							byte	LargeLetterFromXml(XmlUtils::CXmlNode& oContentNode, CDocumentManager& documentManager)
							{
								XmlUtils::CXmlNode oIntContentNode = oContentNode;
								XmlUtils::CXmlNode oAlternateNode;
								if (oContentNode.GetNode(_T("fo:alternate-content"), oAlternateNode))
									if (!oAlternateNode.GetNode(_T("fo:content"), oIntContentNode))
										oIntContentNode = oContentNode;

								CBlockC* pBlock = new CBlockC();
								if (!pBlock->FromXml(oIntContentNode, documentManager))
									m_pContent = pBlock;

								return (m_pContent != NULL) ? 0 : 1;
							}

							byte	GroupFromXml(XmlUtils::CXmlNode& contentNode, CDocumentManager& documentManager) {
								XmlUtils::CXmlNode intContentNode = contentNode;
								XmlUtils::CXmlNode alternateNode;
								if (contentNode.GetNode(_T("fo:alternate-content"), alternateNode))
									if (!alternateNode.GetNode(_T("fo:content"), intContentNode))
										intContentNode = contentNode;

								CGroupC* group = new CGroupC();
								if (!group->FromXml(intContentNode, documentManager))
									m_pContent = group;

								return (m_pContent != NULL) ? 0 : 1;
							}
							byte	TextFromXml(XmlUtils::CXmlNode& contentNode, CDocumentManager& documentManager) {
								XmlUtils::CXmlNode shapeNode;
								if (contentNode.GetNode(_T("shape"), shapeNode)) {
									XmlUtils::CXmlNode textBoxNode;
									if (shapeNode.GetNode(_T("textBox"), textBoxNode)) {
										_Text = new CTextC();
										if (_Text->FromXml(textBoxNode, documentManager))
											RELEASEOBJECT(_Text);
									}
								}

								return 0;
							}
						public:
							CAltContent()
								: m_pContent(NULL), _Text(NULL), 
								m_eType(ContentNone), 
								m_bPlaced(false)
							{
							}
							~CAltContent()
							{
								RELEASEOBJECT(m_pContent);
								RELEASEOBJECT(_Text);
							}
						public:
							byte	FromXml(XmlUtils::CXmlNode& oAlternateNode, CDocumentManager& documentManager)
							{
								RELEASEOBJECT(m_pContent);
								RELEASEOBJECT(_Text);
								bool bIsLoaded = false;

								XmlUtils::CXmlNode oPropertyNode;
								if (oAlternateNode.GetNode(_T("fo:property"), oPropertyNode))
									if (!FromPropertyXml(oPropertyNode))
										bIsLoaded = true;
								if (m_eType == ContentNone)
									ReadContentTypeFromContent(oAlternateNode);
								if (!bIsLoaded) {
									if (m_eType != ContentNone) {
										XmlUtils::CXmlNode oContentNode;
										if (oAlternateNode.GetNode(_T("fo:content"), oContentNode))
											if (!FromContentXml(oContentNode, documentManager))
												bIsLoaded = true;
									}
								}

								return (bIsLoaded) ? 0 : 1;
							}
							void	Update(CGrapher* pGrapher, RectF& oRect, float& fDescent, CAlternate2::CAreaProperties& m_oAreaProperties)
							{
								assert(m_pContent != NULL);
								m_pContent->Update(pGrapher, oRect, fDescent, m_oAreaProperties);
								if (_Text)
									_Text->Update(pGrapher, oRect, fDescent, m_oAreaProperties);
							}
							void	CalculateRect(CAlternate2::CAreaProperties& m_oAreaProperties) {
								m_pContent->CalculateRect(m_oAreaProperties);
								if (_Text)
									_Text->fitRegionIfNecessary(m_oAreaProperties);
							}

						public:
							void	Set(CContent* pContent)
							{
								RELEASEOBJECT(m_pContent);
								m_pContent = pContent;
							}
							bool	Placed()
							{
								return m_bPlaced;
							}
							void	Placed(const bool& bPlaced)
							{
								m_bPlaced = bPlaced;
							}

							bool isLetter() const {
								return dynamic_cast<CBlockC*>(m_pContent) != NULL;
							}
						public:
							void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const RectF& oRect, 
										const float& fDescent, const CAreaProperties::GeometryProperties& geomPr, 
										const float& fCompleteness)	{
								if (m_pContent != NULL) {
									IAVSRenderer2* renderer2 = NULL;
									if (!SUCCEEDED(pRenderer->QueryInterface(__UUIDOF__IAVSRENDERER2, (void**)&renderer2)))
										renderer2 = NULL;

									if (renderer2 != NULL)
										m_pContent->sendBeginCommand(renderer2, geomPr);

									m_pContent->transform(geomPr.getRotationAngle(), geomPr.getHorzFlip(), geomPr.getVertFlip());
									m_pContent->Draw(pRenderer, documentManager, oRect, fDescent, fCompleteness);
									m_pContent->clearTransform();

									if (renderer2 != NULL)
										m_pContent->sendEndCommand(renderer2, geomPr);

									RELEASEINTERFACE(renderer2);
								}
								if (_Text)
									_Text->Draw(pRenderer, documentManager, oRect, fDescent, fCompleteness);
							}
							void	MoveOn(const float& deltaX, const float& deltaY)
							{
								if (m_pContent)
									m_pContent->MoveOn(deltaX, deltaY);
								if (_Text)
									_Text->MoveOn(deltaX, deltaY);
							}
						};
						class CInlineProperties
						{
						public:
							CInlineProperties() : 
								m_VertPosition(0)
							{
							}
							~CInlineProperties()
							{
								Clear();
							}
							void	Clear()
							{
								m_VertPosition = 0;
							}
						public:
							void	FromXml(XmlUtils::CXmlNode& alternateProperty)
							{
								XmlUtils::CXmlNode inlineProperty;
								if (alternateProperty.GetNode(_T("inline"), inlineProperty))
									m_VertPosition = XmlUtils::GetFloat(inlineProperty.GetAttribute(_T("vert-pos"), _T("0")));

							}										
						public:
							float	VertPos() const 
							{
								return m_VertPosition;
							}

						private:
							float	m_VertPosition;
						};
					private:
						CAreaProperties	m_oAreaProperties;
						CAreaWrap		m_oAreaWrap;
						CAltContent		m_oAltContent;
						CInlineProperties m_oInlineProperties;

						bool m_Related;
						bool _inHeader;

					private:
						void	UpdateRect() {
							m_oRect = m_oAreaProperties.RectPt();
							if (m_oAreaWrap.Inline())
								SetOnBaseline();
						}

					public:
						CAlternate2() 
								: CItem()
								, m_oAreaProperties(), m_oAreaWrap()
								, m_oAltContent(), m_oInlineProperties()
								, m_Related(false), _inHeader(false) {}
						virtual ~CAlternate2() {}
					public:
						byte	FromXml(XmlUtils::CXmlNode& alternateNode, CDocumentManager& documentManager, const bool& bUpdate = false)	{
							#ifdef TME_NODE_XML
							CString xml = alternateNode.GetXml();
							#endif

							XmlUtils::CXmlNode oPropertyNode;
							if (!alternateNode.GetNode(_T("fo:property"), oPropertyNode))
								return 1;
							XmlUtils::CXmlNode oContentNode;
							if (!alternateNode.GetNode(_T("fo:content"), oContentNode))
								return 1;

							if (m_oAreaWrap.FromXml(oPropertyNode, oContentNode))
								return 2;
							if (m_oAreaProperties.FromXml(oPropertyNode, oContentNode))
								return 3;
							if (m_oAltContent.FromXml(alternateNode, documentManager))
								return 4;

							m_oInlineProperties.FromXml(oPropertyNode);
							m_fDescent = -m_oInlineProperties.VertPos();
							UpdateRect();

							return 0;
						}
						void	SetContent(CContent* pContent, const int& iInlinePosition = 0)
						{
							assert(pContent != NULL);
							m_oAltContent.Set(pContent);
							m_oAreaWrap.InlinePosition(iInlinePosition);
						}
						void	UpdateContent(CGrapher* pGrapher)
						{
							m_oAltContent.Update(pGrapher, m_oRect, m_fDescent, m_oAreaProperties);
							if (Inline())
								SetOnBaseline();
						}
						bool relate(CPage& page, CRegion& region, const RectF& line) {
							m_oAreaProperties.Relate(page, region, line);
							m_oAltContent.CalculateRect(m_oAreaProperties);
							/*if (!m_oAreaProperties.checkContraints(page))
								return false;*/

							UpdateRect();
							m_oAreaWrap.Wrap(m_oAreaProperties, region);

							return true;
						}
					public:
						int		InlinePosition()
						{
							return m_oAreaWrap.InlinePosition();
						}
						void	InlinePosition(const int& iInlinePosition)
						{
							assert(iInlinePosition >= 0);
							m_oAreaWrap.InlinePosition(iInlinePosition);
						}
						bool	Placed()
						{
							return m_oAltContent.Placed();
						}
						void	Placed(const bool& bPlaced)
						{
							m_oAltContent.Placed(bPlaced);
						}
						bool	Related() const 
						{
							return m_Related;
						}
						void	Related(const bool& related)
						{
							m_Related = related;
						}
						bool	AnchoredToRegion() const
						{
							if (m_oAreaProperties.RelVert() == FromParagraph)
								return true;
							else
								return false;
						}
						const CArray<PointF>&	Points()
						{
							return m_oAreaWrap.Points();
						}
						float	DistanceLeft() const
						{
							return m_oAreaWrap.Distance().m_fLeft;
						}
						float	DistanceRight() const
						{
							return m_oAreaWrap.Distance().m_fRight;
						}

						float getZIndex() const
						{
							return m_oAreaProperties.getZIndex();
						}
						void setInHeader(const bool& inHeader) {
							_inHeader = inHeader;
						}
					public:
						bool	Inline()
						{
							return m_oAreaWrap.Inline();
						}
						void	Inline(const bool& bIsInline)
						{
							m_oAreaWrap.Inline(bIsInline);
						}
					public:
						bool isLetter() const {
							return m_oAltContent.isLetter();
						}
						CItemType	GetType()
						{
							return Alternate;
						}
						void Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const float& completeness) {
							if (_inHeader)
								renderer->BeginCommand(c_nHeader);
							RectF drawingRect = m_oAreaProperties.getDrawingRect(m_oRect);

							m_oAltContent.Draw(renderer, documentManager, drawingRect, m_fDescent, 
								m_oAreaProperties.getGeomPr(), completeness);
							//CItem::drawRect(renderer, m_oRect);
							if (_inHeader)
								renderer->EndCommand(c_nHeader);
						}
						void MoveOn(const float& deltaX, const float& deltaY) {
							CItem::MoveOn(deltaX, deltaY);
							m_oAltContent.MoveOn(deltaX, deltaY);
						}
					};

					class CContent {
					protected:
						class ContentTransformer {
						private:
							bool _enabled;

							float _rotate;
							bool _horzFlip;
							bool _vertFlip;

						public:
							ContentTransformer() 
								: _enabled(false)
								, _rotate(0)
								, _horzFlip(false), _vertFlip(false) {
							}

						public:
							void setTransform(const float& rotate, const bool& horzFlip, const bool& vertFlip) {
								assert(!isEnabled());
								if (rotate != 0 || horzFlip || vertFlip) {
									_rotate = rotate;
									_horzFlip = horzFlip;
									_vertFlip = vertFlip;
									
									setEnabled(true);
								}
							}
							void clearTransform() {
								setEnabled(false);
							}

						private:
							void setEnabled(const bool& enabled) {
								_enabled = enabled;
							}

						protected:
							bool isEnabled() const {
								return _enabled;
							}
							
							float getRotate() const {
								return _rotate;
							}
							float getFlipXCoeff() const {
								return _horzFlip ? -1 : 1;
							}
							float getFlipYCoeff() const {
								return _vertFlip ? -1 : 1;
							}
						};
					public:
						CArray<PointF> m_arrPoints;

					public:
						virtual ~CContent()	{
							Clear();
						}
					public:
						void Clear() {
							m_arrPoints.RemoveAll();
						}
						virtual bool	FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
						{
							return false;
						}
						virtual void	Update(CGrapher* pGrapher, RectF& oRect, REAL& fDescent, CAlternate2::CAreaProperties& m_oAreaProperties)
						{
							oRect.Y	= -oRect.Height + fDescent;
						}
						virtual BYTE	GetType()
						{
							return 0;
						}
						virtual void	CalculateRect(CAlternate2::CAreaProperties& m_oAreaProperties) {}

					public:
						void			SetPoints(const CArray<PointF>& arrPoints)
						{
							m_arrPoints.RemoveAll();
							m_arrPoints.Copy(arrPoints);
						}
						void			SetPoints(const REAL& fPositionX, const REAL& fPositionY)
						{
							for (size_t nIndex = 0; nIndex < m_arrPoints.GetCount(); ++nIndex)
							{
								m_arrPoints[nIndex].X	+= fPositionX;
								m_arrPoints[nIndex].Y	+= fPositionY;
							}
						}
						CArray<PointF>&	GetPoints()
						{
							return m_arrPoints;
						}
					public:
						virtual void	Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const RectF& rect, const float& descent, const float& completeness) {
						}
					public:
						virtual void	MoveOn(const float& deltaX, const float& deltaY) {}
					public:
						virtual void transform(const float& angle, const bool& horzFlip, const bool& vertFlip) {}
						virtual void clearTransform() {}
					public:
						virtual void sendBeginCommand(IAVSRenderer2* renderer
								, const CAlternate2::CAreaProperties::GeometryProperties& geomPr) {}
						virtual void sendEndCommand(IAVSRenderer2* renderer
								, const CAlternate2::CAreaProperties::GeometryProperties& geomPr) {}
					
					protected:
						static inline RectF convertFromPtToMm(const RectF& rect) {
							static const float multPtToMm = 25.4f / 72.0f;;

							return RectF(rect.X * multPtToMm, rect.Y * multPtToMm
								, rect.Width * multPtToMm, rect.Height * multPtToMm);
						}
					};

					class CPictureC : public CContent {
					private:
						class PictureTransformer : public ContentTransformer {
						protected:
							IAVSRenderer* _renderer;

						public:
							PictureTransformer() 
								: ContentTransformer() {
							}

						public:
							void applyTransform(IAVSRenderer* renderer, RectF& rectMm) {
								if (isEnabled()) {
									_renderer = renderer;
									
									float rotate = -getRotate();
									float cosA = cos(rotate);
									float sinA = sin(rotate);

									_renderer->SetTransform(getFlipXCoeff() * cosA, -getFlipXCoeff() * sinA
											, getFlipYCoeff() * sinA, getFlipYCoeff() * cosA
											, rectMm.X + rectMm.Width / 2, rectMm.Y + rectMm.Height / 2);

									rectMm.X = -rectMm.Width / 2;
									rectMm.Y = -rectMm.Height / 2;
								}
							}
							void clearTransform() {
								if (isEnabled())
									_renderer->ResetTransform();

								ContentTransformer::clearTransform();
							}
						} _transformer;
					public:
						CString	m_strSourceFilePath;

					public:
						CPictureC()
							: m_strSourceFilePath(_T(""))
						{
						}
						~CPictureC()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							m_strSourceFilePath	= _T("");
						}
						byte	FromXml(XmlUtils::CXmlNode &oContentNode)
						{
							XmlUtils::CXmlNode oSourceNode;
							oContentNode.GetNode(_T("fo:source"), oSourceNode);

							m_strSourceFilePath				= oSourceNode.GetTextExt();
							if ((m_strSourceFilePath == _T("")) && (m_strSourceFilePath == _T("NULL")))
							{
								m_strSourceFilePath	= _T("");
								return 1;
							}
							CString strTempDirectoryPath	= oSourceNode.GetAttributeOrValue(_T("dir"), _T(""));
							strTempDirectoryPath			= ConvertToLongPathName(strTempDirectoryPath);

							m_strSourceFilePath.Replace( _T('/'), _T('\\'));
							m_strSourceFilePath = strTempDirectoryPath + _T("\\word\\") + m_strSourceFilePath;

							return 0;
						}
						BYTE	GetType()
						{
							return 1;
						}
						void	CalculateRect(CAlternate2::CAreaProperties& m_oAreaProperties)
						{
						}
					public:
						void	Draw(IAVSRenderer* renderer, CDocumentManager &documentManager, const RectF& rect, const REAL& descent, const REAL &completeness) {
							RectF rectMm = convertFromPtToMm(rect);
							_transformer.applyTransform(renderer, rectMm);

							if (m_strSourceFilePath != _T("")) {
								BSTR bstrSourceFilePath	= m_strSourceFilePath.AllocSysString();
								renderer->DrawImageFromFile(bstrSourceFilePath, rectMm.X, rectMm.Y, rectMm.Width, rectMm.Height);
								SysFreeString(bstrSourceFilePath);
							}
							else
								DrawBorder(renderer, rect, descent, completeness);
						}

					public:
						virtual void transform(const float& angle, const bool& horzFlip, const bool& vertFlip) {
							_transformer.setTransform(angle, horzFlip, vertFlip);
						}
						virtual void clearTransform() {
							_transformer.clearTransform();
						}

					private:
						CString	ConvertToLongPathName(const CString &strShortPath)
						{
							DWORD nLength		= GetLongPathName(strShortPath, NULL, 0);
							TCHAR *pLongPath	= new TCHAR[nLength];
							GetLongPathName(strShortPath, pLongPath, nLength);
							CString strLongPath	= CString(pLongPath);
							delete[] pLongPath;

							return strLongPath;
						}
					private:
						void	DrawBorder(IAVSRenderer* renderer, const RectF& rect, const float& descent, const float &completeness)
						{
							const float ptToMm = 25.4f / 72.0f;

							renderer->put_PenColor(0x00);
							renderer->put_PenSize(0.1);
							renderer->put_PenAlpha(255);
							renderer->PathCommandStart();
							renderer->PathCommandMoveTo(rect.X * ptToMm, rect.Y * ptToMm);
							renderer->PathCommandLineTo(rect.X * ptToMm + rect.Width * ptToMm, rect.Y * ptToMm);
							renderer->PathCommandLineTo(rect.X * ptToMm + rect.Width * ptToMm, rect.Y * ptToMm + rect.Height * ptToMm);
							renderer->PathCommandLineTo(rect.X * ptToMm, rect.Y * ptToMm + rect.Height * ptToMm);
							renderer->PathCommandLineTo(rect.X * ptToMm, rect.Y * ptToMm);
							renderer->PathCommandClose();
							renderer->DrawPath(0x01);
							renderer->PathCommandEnd();
						}
					};

					class CShapeC : public CContent
					{
					public:
						CString	m_strSourceXml;
						XmlUtils::CXmlNode m_oXmlNode;
						
						CGeomShapeInfo _info;

					public:
						CShapeC()
							: m_strSourceXml(_T(""))
						{
						}
						~CShapeC()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							m_strSourceXml	= _T("");
						}
						byte	FromXml(XmlUtils::CXmlNode& oContentNode)
						{
							XmlUtils::CXmlNode oShapeNode;
							if (oContentNode.GetNode(_T("shape"), oShapeNode)) {
								m_oXmlNode = oShapeNode;
								return 0;
							}

							return 1;
						}
						BYTE	GetType()
						{
							return 2;
						}
					public:
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const RectF& oRect
								, const REAL& fDescent, const REAL &fCompleteness) {
							const REAL fPtToMm	= 25.4f / 72.0f;

							RectF oRectMm;
							oRectMm.X		= oRect.X * fPtToMm;
							oRectMm.Y		= oRect.Y * fPtToMm;
							oRectMm.Width	= oRect.Width * fPtToMm;
							oRectMm.Height	= oRect.Height * fPtToMm;

							CShape oShape(NSBaseShape::ppt, 0);
							CMetricInfo oMetric;

							oMetric.m_lMillimetresHor = (DWORD)documentManager.GetPageWidthMm();
							oMetric.m_lMillimetresVer = (DWORD)documentManager.GetPageHeightMm();

							oShape.LoadFromXML(m_oXmlNode);

							SetInfo(_info, oRectMm);

							#ifdef PPT_DEF
							oShape.ToRenderer(pRenderer, _info, oMetric, 0.0, 1.0);
							#else
							DrawNonPPTShape(pRenderer, oShape, _info, oMetric);
							#endif
						}
					private:
						void SetInfo(CGeomShapeInfo& oInfo, RectF oRectMm) {
							oInfo.m_dLeft	= (double)(oRectMm.X);
							oInfo.m_dTop	= (double)(oRectMm.Y);
							oInfo.m_dWidth	= (double)(oRectMm.Width);
							oInfo.m_dHeight	= (double)(oRectMm.Height);
						}
						void DrawNonPPTShape(IAVSRenderer* renderer, CShape& shape, CGeomShapeInfo& info, CMetricInfo& metric) {
							CString strXml = shape.ToXml(info, metric, 0.0, 1.0);

							strXml = _T("<shapes>") + strXml + _T("</shapes>");

							XmlUtils::CXmlNode oXmlNode;
							if (oXmlNode.FromXmlString(strXml))
							{
								XmlUtils::CXmlNodes oListGraphics;
								if (oXmlNode.GetNodes(_T("ImagePaint-DrawGraphicPath"), oListGraphics))
								{
									LONG lCount = oListGraphics.GetCount();
									for (LONG i = 0; i < lCount; ++i)
									{
										XmlUtils::CXmlNode oNode;
										oListGraphics.GetAt(i, oNode);

										CDrawGraphicPath oGraphicPath;
										oGraphicPath.InternalFromXmlNode(oNode);

										oGraphicPath.Draw(renderer);										
										renderer->PathCommandEnd();
									}
								}
							}
						}
					public:
						virtual void transform(const float& angle, const bool& horzFlip, const bool& vertFlip) {
							CContent::transform(angle, horzFlip, vertFlip);
							_info.m_dRotate = angle * 180 / M_PI;
							_info.m_bFlipH = horzFlip;
							_info.m_bFlipV = vertFlip;
						}
						virtual void clearTransform() {
							CContent::clearTransform();
							_info.m_dRotate = 0;
							_info.m_bFlipH = false;
							_info.m_bFlipV = false;
						}
					public:
						virtual void sendBeginCommand(IAVSRenderer2* renderer
								, const CAlternate2::CAreaProperties::GeometryProperties& geomPr) {
							IAVSRenderer* pRend = NULL;
							renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
							pRend->BeginCommand(c_nShape);
							RELEASEINTERFACE(pRend);
							renderer->CommandLong2(c_nShapeWrap, geomPr.isAnchored());
						}
						virtual void sendEndCommand(IAVSRenderer2* renderer
								, const CAlternate2::CAreaProperties::GeometryProperties& geomPr) {
							IAVSRenderer* pRend = NULL;
							renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
							pRend->EndCommand(c_nShape);
							RELEASEINTERFACE(pRend);
						}
					};

					class CBlockC : public CContent
					{
					public:
						CBlock*  m_Block;

					private:
						double	 m_dHeightRegion;
						double   m_dWidthRegion;

					private:
						double	 m_dHeightPage;
						double   m_dWidthPage;
						double	 m_dLeftBorder;
						double   m_dRightBorder;

					public:
						CAtlArray<CLine*> m_lines;

						CBlockFrameAttributes*	m_pBlockFrameAttributes;
					public:
						CBlockC()
							:m_dHeightRegion(0), m_dWidthRegion(0), m_pBlockFrameAttributes(NULL)
						{
							m_Block = new CBlock();
							m_pBlockFrameAttributes = new CBlockFrameAttributes();	
						}
						~CBlockC()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							delete m_Block;
							size_t nFLinesCount	= m_lines.GetCount();
							for (size_t nIndex = 0; nIndex < nFLinesCount; ++nIndex)
							{
								RELEASEOBJECT(m_lines[nIndex]);
							}
							RELEASEOBJECT(m_pBlockFrameAttributes);
						}
						void	Update(CGrapher* pGrapher, RectF& oRect, REAL& fDescent, CAlternate2::CAreaProperties& m_oAreaProperties)
						{
							m_dHeightPage = (double)pGrapher->m_DocumentManager->GetPageHeightMm();
							m_dWidthPage  = (double)pGrapher->m_DocumentManager->GetPageWidthMm();
							m_dLeftBorder = (double)pGrapher->m_DocumentManager->GetPageLeftBorderMm();
							m_dRightBorder= (double)pGrapher->m_DocumentManager->GetPageRightBorderMm();


							/*m_dHeightPage = (double)CDocumentManager::GetPageHeightMm() * 72 / 25.4;
							m_dWidthPage  = (double)CDocumentManager::GetPageWidthMm() * 72 / 25.4;
							m_dLeftBorder = (double)CDocumentManager::GetPageLeftBorderPt();
							m_dRightBorder= (double)CDocumentManager::GetPageRightBorderPt();*/

							double dWidth = 0;
							if (m_pBlockFrameAttributes->m_width > 0)
								dWidth = min((m_dRightBorder - m_dLeftBorder), m_pBlockFrameAttributes->m_width);
							else 
								dWidth = m_dRightBorder - m_dLeftBorder;


							RectF rect(0, 0, dWidth * 25.4 / 72 , m_dHeightPage* 25.4 / 72);
							//RectF rect(0, 0, (m_dRightBorder - m_dLeftBorder) * 25.4 / 72, m_dHeightPage);

							CRegion textRegion;
							textRegion.FromRectMm(rect);

							CPage page;

							page.StartPrintingFlow();
							m_Block->IgnoreAnchors(true);
							m_Block->FillRegion(textRegion, page, true, pGrapher);

							page.GetLines(m_lines);

							m_dHeightRegion = textRegion.GetFilledHeight();
							m_dWidthRegion  = GetFillWidth(dWidth);

							oRect.X		 = 0;
							oRect.Y		 = 0;
							oRect.Width  = max(m_dWidthRegion, m_pBlockFrameAttributes->m_width) + m_pBlockFrameAttributes->m_hSpace;
							oRect.Height = max(m_dHeightRegion, m_pBlockFrameAttributes->m_height) + m_pBlockFrameAttributes->m_vSpace;

							m_oAreaProperties.SetAreaRect(oRect);
						}

						byte	FromXml(XmlUtils::CXmlNode& contentNode, CDocumentManager& documentManager)
						{
							#ifdef TME_NODE_XML
							CString xml = contentNode.GetXml();
							#endif

							XmlUtils::CXmlNode oBlockNode;
							contentNode.GetNode(_T("fo:block"), oBlockNode);

							m_Block->InTable(false);
							CAtlArray<CString> curCommands;
							bool inCommandField = false; 

							m_Block->FromXmlNode(oBlockNode, documentManager, false, inCommandField, curCommands);
							m_pBlockFrameAttributes->FromXmlNode(oBlockNode);

							m_Block->m_pBlockAttributes->m_nSpaceAfter = 0;
							m_Block->m_pBlockAttributes->m_nSpaceBefore = 0;

							return 0;
						}						
						BYTE	GetType()
						{
							return 3;
						}
					public:
						void	CalculateRect(CAlternate2::CAreaProperties& m_oAreaProperties) {
							
							RectF oRect   = m_oAreaProperties.AreaRectPt();
							PointF oPoint = m_oAreaProperties.RelPointPt();
							double dX = 0;
							if (m_pBlockFrameAttributes->m_xAlign == _T("right"))
							{
								if (m_pBlockFrameAttributes->m_hAnchor == _T("margin"))
									dX = m_dRightBorder - m_dWidthRegion - oPoint.X;
								else if (m_pBlockFrameAttributes->m_hAnchor == _T("page"))
									dX = m_dWidthPage - m_dWidthRegion - oPoint.X;
							}
							else if (m_pBlockFrameAttributes->m_xAlign == _T("left"))
							{
								if (m_pBlockFrameAttributes->m_hAnchor == _T("margin"))
									dX = 0;
								else if (m_pBlockFrameAttributes->m_hAnchor == _T("page"))
									dX = -oPoint.X;
							}
							else if (m_pBlockFrameAttributes->m_x > 0)
							{
								dX = m_pBlockFrameAttributes->m_x;
								if (m_pBlockFrameAttributes->m_hAnchor == _T("page"))
									dX -= oPoint.X;
							}
							else if (m_pBlockFrameAttributes->m_dropCap == _T("drop"))
							{
								dX = oRect.GetLeft();
							}
							else if (m_dWidthRegion < m_dLeftBorder)
							{
								dX = m_dLeftBorder - m_dWidthRegion - oPoint.X;
							}

							double dY = 0;
							if (m_pBlockFrameAttributes->m_y > 0)
							{
								dY = m_pBlockFrameAttributes->m_y;
								if (m_pBlockFrameAttributes->m_vAnchor == _T("page"))
									dY -= oPoint.Y;
							}
							else 
							{
								dY = oRect.GetTop();
							}

							oRect.X = dX;
							oRect.Y = dY;
							m_oAreaProperties.SetAreaRect(oRect);

							dX = dX + oPoint.X;
							dY = dY + oPoint.Y;

							double dDeltaX = 0;
							double dDeltaY = 0;

							size_t nCount = m_lines.GetCount();
							if (nCount > 0)
							{
								CLine* pFirstLine = m_lines[0];
								dDeltaX  = dX;// - pFirstLine->GetRect().GetLeft();
								dDeltaY  = dY;// - pFirstLine->GetRect().GetTop();
							}
							for (size_t i = 0; i < nCount; ++i)
							{
								m_lines[i]->MoveOn(dDeltaX, dDeltaY);
							}					
						}
						void	Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const RectF& rect, const float& descent, const float& completeness) {	
							#ifdef TME_DRAW_LETTER_BORDER
							CItem::drawRect(renderer, rect);
							#endif
														
							size_t nCount = m_lines.GetCount();
							for (size_t i = 0; i < nCount; ++i)
								m_lines[i]->Draw(renderer, documentManager, completeness);
						}
					public:
						void	MoveOn(const float& deltaX, const float& deltaY) {
							for (size_t i = 0; i < m_lines.GetCount(); ++i)
								m_lines[i]->MoveOn(deltaX, deltaY);
						}
					private:
						double	GetFillWidth(double dWidth)
						{
							//double widthRegion = (m_dRightBorder - m_dLeftBorder)/* * 72.0f / 25.4f */;
							size_t size = m_lines.GetCount();
							double minPersist = dWidth;
							for( size_t i = 0; i < size; ++i)
							{
								double dCurrentPersist = (double)m_lines[i]->GetPersistWidth();
								if (minPersist > dCurrentPersist)
									minPersist = dCurrentPersist;
							}
							return (dWidth - minPersist);
						}
					};
					class CPageNumberC : public CContent
					{
					private:
						CString		m_strText;
						bool		m_bIsPageCount;

						CCharacter*	m_pCharacter;

					public:
						CPageNumberC()
							: m_strText(_T("")), m_bIsPageCount(false), 
							m_pCharacter(NULL)
						{
						}
						CPageNumberC(CCharacter *pCharacter, bool bIsPageCount = false)
							: m_strText(_T("")), m_bIsPageCount(bIsPageCount), 
							m_pCharacter(pCharacter)
						{
						}
						~CPageNumberC()
						{
							RELEASEOBJECT(m_pCharacter);
						}
					public:
						void	Update(CGrapher* pGrapher, RectF& oRect, REAL& fDescent, CAlternate2::CAreaProperties& m_oAreaProperties)
						{
							if (!m_bIsPageCount)
								m_strText	= pGrapher->m_DocumentManager->getPageIndexString();
							else
								m_strText	= pGrapher->m_DocumentManager->getPageCountString();

							oRect		= pGrapher->GetRectForString(m_strText, m_pCharacter);
							fDescent	= pGrapher->GetDescent(m_pCharacter);

							SetOnBaseline(oRect, fDescent);
						}
						void	CalculateRect(CAlternate2::CAreaProperties& m_oAreaProperties)
						{
						}
					public:
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const RectF& oRect , const REAL& fDescent, const REAL &fCompleteness)
						{
							if (oRect.Width == 0)
							{
								return;
							}
							BSTR bstrText	= m_strText.AllocSysString();

							m_pCharacter->m_oAttributes.m_oFont.SetToRenderer(pRenderer);
							m_pCharacter->m_oAttributes.m_oBrush.SetToRenderer(pRenderer);
							m_pCharacter->m_oAttributes.m_oShadow.SetToRenderer(pRenderer);
							m_pCharacter->m_oAttributes.m_oEdge.SetToRenderer(pRenderer);

							const float fPtToMm	= 25.4f / 72.0f;
							pRenderer->CommandDrawText(bstrText, oRect.X * fPtToMm, oRect.Y * fPtToMm, oRect.Width * fPtToMm, oRect.Height * fPtToMm, (oRect.Height - fDescent)  * fPtToMm);
							SysFreeString(bstrText);							
						}

					private:
						void	SetOnBaseline(RectF& oRect, REAL& fDescent)
						{
							MoveTo(oRect, 0, fDescent - oRect.Height);

							if (m_pCharacter->m_oAttributes.m_nBaselineShift == 1)
							{
								MoveOn(oRect, -2 * fDescent);
							}
							else if (m_pCharacter->m_oAttributes.m_nBaselineShift == 2)
							{
								MoveOn(oRect, fDescent);
							}
						}
						void	MoveTo(RectF& oRect, const REAL &fNewX, const REAL &fNewY)
						{
							oRect.X	= fNewX;
							oRect.Y	= fNewY;
						}
						void	MoveOn(RectF& oRect, const REAL &fDeltaY)
						{
							oRect.Y	+= fDeltaY;
						}
					};					
					class CGroupC : public CContent	{
						class GroupTransformer : public ContentTransformer {
						public:
							void applyTransformToChild(const RectF& parentRect, RectF& childRect
									, CAlternate2::CAreaProperties::GeometryProperties& geomPr) {
								PointF parentCenter(parentRect.X + parentRect.Width / 2
										, parentRect.Y + parentRect.Height / 2);
								PointF childCenter(childRect.X + childRect.Width / 2
										, childRect.Y + childRect.Height / 2);
								PointF childCenterRight(childCenter.X + childRect.Width
										, childCenter.Y);

								float oldDx = childCenterRight.X - childCenter.X;
								float oldDy = childCenterRight.Y - childCenter.Y;
								float oldR = sqrt(pow(oldDx, 2) + pow(oldDy, 2));

								transformPoint(parentCenter, childCenter);
								transformPoint(parentCenter, childCenterRight);

								float newDx = childCenterRight.X - childCenter.X;
								float newDy = childCenterRight.Y - childCenter.Y;
								float newR = sqrt(pow(newDx, 2) + pow(newDy, 2));

								float angle = 0;
								float r = oldR * newR;
								if (r != 0) {
									angle = acos((oldDx * newDx + oldDy * newDy) / (oldR * newR));
									if (oldDx * newDy - oldDy * newDx < 0)
										angle *= -1;
								}
								
								if (getFlipXCoeff() == -1)
									geomPr.changeHFlip();
								if (getFlipYCoeff() == -1)
									geomPr.changeVFlip();

								childRect.X = childCenter.X - childRect.Width / 2;
								childRect.Y = childCenter.Y - childRect.Height / 2;

								geomPr.setRotationAngle(geomPr.getRotationAngle() + angle);
							}
						private:
							void transformPoint(const PointF& center, PointF& point) {
								float x = point.X - center.X;
								float y = point.Y - center.Y;

								float rotate = getRotate();
								float cosA = cos(rotate);
								float sinA = sin(rotate);

								point.X = cosA * x - sinA * y + center.X;
								point.Y = sinA * x + cosA * y + center.Y;
							}

						} _transformer;

					public:
						CGroupC() {
						}
						~CGroupC() {
							Clear(true);
						}
					public:
						void Clear(const bool& complete = false) {
							if (!complete)
								_Alternates.RemoveAll();
						}
						byte FromXml(XmlUtils::CXmlNode& contentNode, CDocumentManager& documentManager) {
							XmlUtils::CXmlNode groupNode;
							if (contentNode.GetNode(_T("group"), groupNode)) {
								ReadArea(groupNode);
								ReadAlternates(groupNode, documentManager);
							}

							return 0;
						}
						void Update(CGrapher* grapher, RectF& rect, float& descent, CAlternate2::CAreaProperties& areaProperties)
						{
							int alternatesCount = (int) _Alternates.GetCount();
							for (int i = 0; i < alternatesCount; ++i) {
								RectF relatedRect = RelatedRect(_Alternates[i], _AreaProperties[i], rect);
								_Alternates[i].Update(grapher, relatedRect, descent, areaProperties);
							}
						}
						void Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const RectF& rect
								, const float& descent, const float &completeness) {
							int alternatesCount = (int) _Alternates.GetCount();
							for (int i = 0; i < alternatesCount; ++i) {
								RectF relatedRect = RelatedRect(_Alternates[i], _AreaProperties[i], rect);

								CAlternate2::CAreaProperties::GeometryProperties geomPr(_AreaProperties[i].getGeomPr());
								_transformer.applyTransformToChild(rect, relatedRect, geomPr);

								_Alternates[i].Draw(renderer, documentManager, relatedRect, descent, geomPr, completeness);
							}
						}
					public:
						virtual void transform(const float& rotate, const bool& horzFlip, const bool& vertFlip) {
							_transformer.setTransform(rotate, horzFlip, vertFlip);
						}
						virtual void clearTransform() {
							_transformer.clearTransform();
						}

					public:
						virtual void sendBeginCommand(IAVSRenderer2* renderer
								, const CAlternate2::CAreaProperties::GeometryProperties& geomPr) {
							IAVSRenderer* pRend = NULL;
							renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
							pRend->BeginCommand(c_nShapeGroup);
							RELEASEINTERFACE(pRend);
						}
						virtual void sendEndCommand(IAVSRenderer2* renderer
								, const CAlternate2::CAreaProperties::GeometryProperties& geomPr) {
							IAVSRenderer* pRend = NULL;
							renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
							pRend->EndCommand(c_nShapeGroup);
							RELEASEINTERFACE(pRend);
						}
					private:
						RectF _OriginArea;

						CArray<CAlternate2::CAltContent> _Alternates;
						CArray<CAlternate2::CAreaProperties> _AreaProperties;
					private:
						void ReadArea(XmlUtils::CXmlNode& groupNode) {
							XmlUtils::CXmlNode node;
							if (groupNode.GetNode(_T("originX"), node))
								_OriginArea.X = XmlUtils::GetFloat(node.GetAttribute(_T("val")));
							if (groupNode.GetNode(_T("originY"), node))
								_OriginArea.Y = XmlUtils::GetFloat(node.GetAttribute(_T("val")));
							if (groupNode.GetNode(_T("originWidth"), node))
								_OriginArea.Width = XmlUtils::GetFloat(node.GetAttribute(_T("val")));
							if (groupNode.GetNode(_T("originHeight"), node))
								_OriginArea.Height = XmlUtils::GetFloat(node.GetAttribute(_T("val")));
						}
						void ReadAlternates(XmlUtils::CXmlNode& groupNode, CDocumentManager& documentManager) {
							XmlUtils::CXmlNodes alternateNodes;
							if (groupNode.GetNodes(_T("fo:alternate-content"), alternateNodes)) {
								int alternatesCount = (int) alternateNodes.GetCount();
								for (int i = 0; i < alternatesCount; ++i) {
									XmlUtils::CXmlNode alternateNode;
									alternateNodes.GetAt(i, alternateNode);
									_Alternates.Add();
									_Alternates[_Alternates.GetCount() - 1].FromXml(alternateNode, documentManager);
									_AreaProperties.Add();
									_AreaProperties[_AreaProperties.GetCount() - 1].FromXml(alternateNode);
								}
							}
						}
					private:
						RectF RelatedRect(CAlternate2::CAltContent& alternate, CAlternate2::CAreaProperties& areaProperty, const RectF& rect) {
							float xOriginToPt = rect.Width / _OriginArea.Width;
							float yOriginToPt = rect.Height / _OriginArea.Height;

							RectF relatedRect = areaProperty.AreaRectPt();

							relatedRect.X = (relatedRect.X*20 - _OriginArea.X)*xOriginToPt + rect.X;
							relatedRect.Y = (relatedRect.Y*20 - _OriginArea.Y)*yOriginToPt + rect.Y;
							relatedRect.Width *= xOriginToPt*20;
							relatedRect.Height *= yOriginToPt*20;

							return relatedRect;
						}
					};
					class CTextC {
					private:
						CFlow* _Flow;
						CArray<CLine*> _Lines;

						RectF _filledRect;
						bool _mustFit;

					public:
						CTextC() 
								: _mustFit(false) {
							_Flow = NULL;
						}
						~CTextC()
						{
							Clear(true);
						}
					public:
						void Clear(const bool& complete = false) {
							if (_Flow != NULL)
								RELEASEOBJECT(_Flow);

							int linesCount = (int) _Lines.GetCount();
							for (int i = 0; i < linesCount; ++i)
								RELEASEOBJECT(_Lines[i]);

							if (!complete)
								_Lines.RemoveAll();
						}
						byte FromXml(XmlUtils::CXmlNode& contentNode, CDocumentManager& documentManager) {
							Clear();
							
							_Flow = new CFlow();
							_Flow->FromXmlNode(contentNode, documentManager);
							_Flow->Static(true);

							readMustFit(contentNode);

							return 0;
						}
						void readMustFit(XmlUtils::CXmlNode& contentNode) {
							CString mustFit = contentNode.GetAttribute(_T("mustFit"));
							_mustFit = mustFit == _T("t");
						}
						void Update(CGrapher* grapher, RectF& rect, float& descent, CAlternate2::CAreaProperties& areaProperties) {
							const float ptToMm	= 25.4f / 72.0f;
							RectF rectMm(0, 0, rect.Width*ptToMm, rect.Height*ptToMm);
							
							CRegion region;
							region.FromRectMm(rectMm);
							CPage page;

							page.StartPrintingFlow();
							_Flow->FillRegion(region, page, grapher);
							_filledRect = region.GetFilledRectPt();

							page.GetLines(_Lines);
							page.FinishPrintingFlow();
						}
					public:
						void MoveOn(const float& deltaX, const float& deltaY) {
							int linesCount = (int) _Lines.GetCount();
							for (int i = 0; i < linesCount; ++i)
								_Lines[i]->MoveOn(deltaX, deltaY);
						}
					public:
						void Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const RectF& rect, const float& descent, const float& completeness)
						{
							clipOn(renderer, rect);

							MoveOn(rect.X, rect.Y);
							int linesCount = (int) _Lines.GetCount();
							for (int i = 0; i < linesCount; ++i)
								_Lines[i]->Draw(renderer, documentManager, completeness);

							clipOff(renderer);
						}

					public:
						void fitRegionIfNecessary(CAlternate2::CAreaProperties& areaPr) {
							if (_mustFit)
								areaPr.SetAreaRect(_filledRect, true);
						}

					private:
						void clipOn(IAVSRenderer* renderer, const RectF& rectPt) 
						{
							RectF rect = toMm(rectPt);

							renderer->BeginCommand(c_nClipType);
							renderer->BeginCommand(c_nPathType);
							renderer->PathCommandMoveTo(rect.X, rect.Y);
							renderer->PathCommandLineTo(rect.X + rect.Width, rect.Y);
							renderer->PathCommandLineTo(rect.X + rect.Width, rect.Y + rect.Height);
							renderer->PathCommandLineTo(rect.X, rect.Y + rect.Height);
							renderer->PathCommandClose();
							renderer->EndCommand(c_nPathType);
							renderer->EndCommand(c_nClipType);
							renderer->PathCommandEnd();
						}
						void clipOff(IAVSRenderer* renderer) 
						{
							renderer->BeginCommand(c_nResetClipType);
							renderer->EndCommand(c_nResetClipType);
						}
						RectF toMm(const RectF& rect) {
							const float ptToMm = 25.4f / 72.0f;

							RectF rectMm(rect);
							rectMm.X *= ptToMm;
							rectMm.Y *= ptToMm;
							rectMm.Width *= ptToMm;
							rectMm.Height *= ptToMm;

							return rectMm;
						}
					};
					// ### ### ###
					class CBlockInputParameters
					{
					private:
						float	m_SpaceBefore;

					public:
						CBlockInputParameters() : 
						  m_SpaceBefore(0.0f)
						  {
						  }
						  void	Clear()
						  {
							  m_SpaceBefore = 0.0f;
						  }
					public:
						float	SpaceBefore()
						{
							return m_SpaceBefore;
						}
						void	SpaceBefore(const float& spaceBefore)
						{
							m_SpaceBefore = spaceBefore;
						}
					};

					class CCommonBlock {
					public:
						CCommonBlock()
							: m_InTable(false), m_InColumn(false)
						{
						}
						virtual ~CCommonBlock()
						{
						}
					public:
						virtual bool	Empty() const
						{
							return true;
						}
						virtual bool	Complete() const
						{
							return true;
						}
						virtual bool	AtBeginning() const
						{
							return false;
						}
						virtual	bool	AtEnd() const
						{
							return true;
						}
						virtual bool	UsualBlock() const
						{
							return true;
						}
						virtual byte	KeepNext() const
						{
							return 0;
						}
						bool	InTable() const
						{
							return m_InTable;
						}
						void	InTable(const bool& inTable)
						{
							m_InTable = inTable;
						}
						void	InColumn(const bool& inColumn)
						{
							m_InColumn	= inColumn;
						}
						virtual bool transferToNextPage(CGrapher* grapher) {
							Renew(grapher);

							return true;
						}
					public:
						virtual void	InputParams(CBlockInputParameters& inputParams, CDocumentManager& documentManager, const bool& update = false)
						{
							inputParams.Clear();
						}
						virtual void estimateWidth(CGrapher* grapher, float& widthMin, float& widthScore)
						{
							widthMin = 0;
							widthScore = 0;
						}
						virtual void	Renew(CGrapher* grapher, const bool& full = true)
						{
						}
					public:
						virtual bool	FillRegion(CRegion &oRegion, CPage &oPage, const bool& bIsMustRenew, CGrapher* pGrapher)
						{
							return false;
						}

					protected:
						bool	m_InTable;
						bool	m_InColumn;

					protected:
						void addCommandToPage(CPage& page, SharedPtr<RendererCommand>& command
								, const bool& toLast = false) {
							if (!command.isEmpty() && !command->isAdded()) {
								page.addRendererCommand(command, toLast);
								
								command->setAdded(true);
							}
						}
					};

					// ### ###
					class CBlock : public CCommonBlock, private CXmlAttributeReader {
					private:
						class CAlternates
						{
						private:
							CAtlArray<CAlternate2*>	m_Inlines;
							CAtlArray<CAlternate2*>	m_Anchors;

							float	m_RegionOffset;
							bool	m_Added;

						public:
							CAlternates() : 
							  m_Inlines(), m_Anchors(),
								  m_Added(false), 
								  m_RegionOffset(0.0f)
							  {
							  }
							  ~CAlternates()
							  {
								  Clear();
							  }
							  void	Clear()
							  {
								  m_Inlines.RemoveAll();
								  for (int index = 0; index < (int)m_Anchors.GetCount(); ++index)
									  RELEASEOBJECT((m_Anchors[index]));
								  m_Anchors.RemoveAll();
								  m_Added	= false;
								  m_RegionOffset = 0.0f;
							  }
						public:
							CAtlArray<CAlternate2*>&	Inlines()
							{
								return m_Inlines;
							}

							CAtlArray<CAlternate2*>&	Anchors()
							{
								return m_Anchors;
							}

						public:
							bool	HasInlines()
							{
								return (m_Inlines.GetCount() != 0);
							}
							bool	HasAnchors() {
								return (m_Anchors.GetCount() != 0);
							}
							bool isOnlyOneLetter() const {
								if (m_Anchors.GetCount() != 1)
									return false;
								return m_Anchors[0]->isLetter();
							}
							bool	Added() const 
							{
								return m_Added;
							}
						public:
							void	Add(CAlternate2* alternate)
							{
								if (alternate->Inline())
									m_Inlines.Add(alternate);
								else
									m_Anchors.Add(alternate);
							}
							void	Update(CGrapher* grapher)
							{
								for (int index = 0; index < (int)m_Inlines.GetCount(); ++index)
									m_Inlines[index]->UpdateContent(grapher);
								for (int index = 0; index < (int)m_Anchors.GetCount(); ++index)
									m_Anchors[index]->UpdateContent(grapher);
							}
							bool Place(CPage& page, CRegion region, const CBlockAttributes* blockAttrs
									, const RectF& line, const int& position = -1) {
								if (!HasAnchors())
									return true;

								if (m_RegionOffset > 0)
									region.TruncateLeftPt(m_RegionOffset);
								else if (blockAttrs->m_fStartIndent != 0)
									region.TruncateLeftPt(blockAttrs->m_fStartIndent);

								bool fullRelate = true;
								for (int index = 0; index < (int)m_Anchors.GetCount(); ++index)	{
									m_Anchors[index]->Related(false);
									if (!m_Anchors[index]->Placed()) {
										int anchorPosition = m_Anchors[index]->InlinePosition();
										if ((position < 0) || (anchorPosition <= position)) {
											m_Anchors[index]->relate(page, region, line);
											m_Anchors[index]->Related(true);
										}
									}
								}

								page.AddAnchorContent(m_Anchors);
								return fullRelate;
							}
							void	Renew()
							{
								m_Added = false;
								for (int index = 0; index < (int)m_Anchors.GetCount(); ++index)
									m_Anchors[index]->Placed(false);
							}
							void	RegionSettings(CPage& page, CRegion& region) {
								CAreaManager* area = page.GetAreaManager();
								RectF firstLine = region.GetRectPt();
								firstLine.Height = 2.0;
								CArray<RectF> rects;
								area->GetArea(firstLine, rects);
								if (rects.GetCount() != 0) {
									m_RegionOffset = rects[0].X - firstLine.X;
								}
							}
						};

						class CItems
						{
						public:
							enum ENumProcess;
						private:
							enum EPartState {
								EmptyPart = 0, SymbolsInPart, WordInPart
							};
							class CBlockState
							{
							private:
								ENumProcess	m_NumProcess;

							public:
								CBlockState() :
								  m_NumProcess(NumNone)
								  {
								  }
							public:
								void	Renew()
								{
									m_NumProcess = NumProcessed;
								}
							public:
								bool	NumProcess(const ENumProcess& numProcess)
								{
									if (m_NumProcess == NumProcessed)
										return true;
									if ((m_NumProcess == NumAddedWithTab) && (numProcess == NumNone))
									{
										m_NumProcess = NumProcessed;
										return false;
									}
									if (numProcess == NumProcessed)
									{
										m_NumProcess = NumProcessed;
										return true;
									}
									if ((m_NumProcess == NumNone) && (numProcess == NumAdded))
									{
										m_NumProcess = NumAdded;
										return true;
									}
									if ((m_NumProcess == NumAdded) && (numProcess == NumAddedWithTab))
									{
										m_NumProcess = NumAddedWithTab;
										return true;
									}
									return true;
								}
							};
						public:
							enum EBreakType {
								NoneBreak = 0, LineBreak,PageBreak, ColumnBreak
							};

							enum ENumProcess {
								NumNone = 0, NumAdded, NumAddedWithTab, NumProcessed
							};
							struct SPartProperties
							{
								bool	m_BreakBefore;
								bool	m_HeightChanged;
								bool	m_WrapBySymbol;
								bool	m_Force; 
								bool	m_Single;
								int	_items;
								bool	m_RuptureIfSingle;
								EPartState	m_State;
								bool	m_Tab;
								bool _limitTabs;
								EBreakType	m_Break;
								bool _isPpt;
								bool _ignoreWidthLimit;
								bool _firstLine;

								SPartProperties() 
									: m_BreakBefore(false), 
									m_HeightChanged(false), 
									m_WrapBySymbol(false), m_Force(false), m_Single(false), 
									_items(0),
									m_RuptureIfSingle(false), 
									m_State(EmptyPart), 
									m_Tab(false), _limitTabs(false),
									m_Break(NoneBreak),
									_isPpt(false),
									_ignoreWidthLimit(false), 
									_firstLine(false) {
								}
							};
						private:
							CString	m_Text;
							CArray<CItem*>	m_Items;
							CArray<CCharacter*>	m_Characters;

							CBlockState	m_BlockState;

							int m_CurrentItem;
							int	m_CurrentItemFill;
							std::stack<int>	m_CurrentItemsSaved;

							bool	m_Prepared;

							// ##################
							friend class CTextMeasurer;
						private:
							void		Text(const CString& text)
							{
								m_Text = text;
							}
							CString&	Text()
							{
								return m_Text;
							}
							CArray<CItem*>&	Items()
							{
								return m_Items;
							}
							CArray<CCharacter*>&	Characters()
							{
								return m_Characters;
							}
							void	AddText(const CString& text)
							{
								m_Text += text;
							}
							void	AddCharacter(CCharacter* character)
							{
								m_Characters.Add(character);
							}

							CCharacter*	GetCharacter(const int& index)
							{
								return m_Characters[index];
							}
							int		CountCharacters()
							{
								return (int)m_Characters.GetCount();
							}
							// ##################
						private:
							bool	RuptureAndAddToPart(CItem* item, RectF& area, CArray<CItem*>& items, SPartProperties& properties, CGrapher* grapher)
							{
								CItem* newItem = NULL;
								if (item->CanRupture())
									newItem = item->Rupture(area.Width, grapher, properties.m_WrapBySymbol);
								if ((newItem == NULL) && properties.m_WrapBySymbol)
									newItem = item->Rupture(area.Width, grapher, true);
								if (newItem)
								{
									m_Items.InsertAt(m_CurrentItem, newItem);
									AddToPart(newItem, area, items, properties);
									properties.m_State = WordInPart;
									if (newItem->GetType() != Space)
										return true;
									else
										return false;
								}
								else if (properties.m_Single)
								{
									if (properties.m_RuptureIfSingle && item->CanRupture(grapher, area.Width))
									{
										newItem = item->Rupture(area.Width, grapher, true);
										if (newItem != NULL)
										{
											m_Items.InsertAt(m_CurrentItem, newItem);
											AddToPart(newItem, area, items, properties);
											properties.m_State = WordInPart;
											
											return (newItem->GetType() != Space);
										}
									}
									else if (properties.m_RuptureIfSingle && properties._items != 0) {
										return false;
									}
									else if (properties.m_Force || item->GetType() == Alternate || item->IsOneSymbol()) {
										AddToPart(item, area, items, properties);
										properties.m_State = WordInPart;
										return true;
									}
									else if (properties.m_RuptureIfSingle) {
										newItem = item->Rupture(area.Width, grapher, true, true);
										if (newItem != NULL) {
											m_Items.InsertAt(m_CurrentItem, newItem);
											AddToPart(newItem, area, items, properties);
											properties.m_State = WordInPart;
											
											return (newItem->GetType() != Space);
										}
									}
								}
								return false;
							}
							void	AddToPart(CItem* item, RectF& area, CArray<CItem*>& items, SPartProperties& properties)
							{
								assert(!properties.m_HeightChanged);
								if (area.Height == 0) {
									AddToPart(item, area, items);
									++properties._items;
								}
								else
								{
									float height = item->GetHeightPt();
									if (height <= area.Height) {
										++properties._items;
										AddToPart(item, area, items);
									}
									else
									{
										area.Height = height;
										properties.m_HeightChanged = true;
									}
								}
							}
							void	AddToPart(CItem* item, RectF& area, CArray<CItem*>& items)
							{
								float width = item->GetWidthPt();
								area.Width -= width;
								area.X += width;
								items.Add(item);
								++m_CurrentItem;
							}
							void	RemoveLastItemFromPart(RectF& area, CArray<CItem*>& items)
							{
								int lastItem = (int)items.GetCount() - 1;
								float width = items[lastItem]->GetWidthPt();
								area.Width += width;
								area.X -= width;
								items.RemoveAt(lastItem);
								--m_CurrentItem;
							}

							void	UpdatePartState(const CItemType& itemType, SPartProperties& properties, CArray<CItem*>& items)
							{
								if (!properties.m_Single)
									return;
								if (itemType == Alternate) {
									properties.m_State = WordInPart;
									properties.m_Single = false;
									return;
								}
								else if (itemType == LineEnd) {
									properties.m_State = WordInPart;
									properties.m_Single = false;
									return;
								}
								if (properties.m_State == EmptyPart)
								{
									if (itemType == Symbols)
										properties.m_State = SymbolsInPart;
								}
								else if (properties.m_State == SymbolsInPart)
								{
									if (itemType == Space)
									{
										properties.m_State = WordInPart;
										properties.m_Single = false;
									}
								}
							}
							bool	ProcessItem(CItem* item, RectF& area, SPartProperties& properties, CGrapher* grapher)
							{
								bool mustBreak = false;
								CItemType itemType = item->GetType();
								if (itemType == TabStop)
								{
									CTabParams tab = grapher->m_DocumentManager->GetTabSize(area.X);
									((CTabStop*)item)->GetRect().Width	= tab.GetPosition();
									((CTabStop*)item)->m_bLeader		= tab.GetLeader();
									((CTabStop*)item)->m_bType			= tab.GetType();

									float width = item->GetWidthPt();
									if (width == 0)
										mustBreak = true;

									if (width > area.Width)
										properties.m_Tab = true;
								}
								else if (itemType == LineEnd)
								{
									if (((CLineEnd*)item)->GetCharacter()->m_nAdditionalParam == 1)
										properties.m_Break = PageBreak;
									else if (((CLineEnd*)item)->GetCharacter()->m_nAdditionalParam == 2)
										properties.m_Break = ColumnBreak;
									else
										properties.m_Break = LineBreak;
								}

								return mustBreak;
							}
							bool	CheckBreak(CItem* item, RectF& area, CArray<CItem*>& items, SPartProperties& properties)
							{
								if (properties.m_Break == NoneBreak)
									return false;
								else if (properties.m_Break == LineBreak) {
									if (properties._isPpt)
										AddToPart(item, area, items, properties);
									else
										if (items.GetCount() == 0) {
											//AddToPart(item, area, items, properties);
											properties.m_Break = NoneBreak;
											return false;
										}
								}
								else
									AddToPart(item, area, items, properties);

								properties.m_State = WordInPart;
								return true;
							}
							void	RuptureOrRemoveLastSymbolItemsInPart(CItem* item, RectF& area, CArray<CItem*>& items, SPartProperties& properties, CGrapher* grapher)
							{
								CItemType itemType = item->GetType();
								if (itemType != Space && itemType != Alternate) {
									int lastItem = (int)items.GetCount() - 1;
									int lastItemInPart = lastItem;
									while (lastItemInPart >= 0) {
										CItem* currItem = items[lastItemInPart];
										itemType = currItem->GetType();
										if (itemType != Space && itemType != Alternate && itemType != TabStop 
											&& itemType != LineEnd)
											--lastItemInPart;
										else
											break;
									}
									
									if (lastItemInPart > 0)
										for (int i = 0; i < lastItem - lastItemInPart; ++i)
											RemoveLastItemFromPart(area, items);
								}
								else
								{
									while (!AtEnd())
									{
										CItem* currItem = m_Items[m_CurrentItem];
										itemType = currItem->GetType();
										if (itemType == Space)
											++m_CurrentItem;
										else
											break;
									}
								}
							}
							bool	ProcessIfRightTab(CItem* itemTab, RectF& area, CArray<CItem*>& items, SPartProperties& properties, CGrapher* grapher)
							{
								CItemType itemType = itemTab->GetType();
								if (itemType == TabStop)
								{
									if (((CTabStop*)itemTab)->m_bType != 1)
										return false;

									RectF areaTmp = area;
									float tabWidth = ((CTabStop*)itemTab)->GetRect().Width;
									areaTmp.Width = tabWidth;
									((CTabStop*)itemTab)->GetRect().Width = 0;
									AddToPart(itemTab, area, items, properties);

									while (!AtEnd())
									{
										CItem* item = m_Items[m_CurrentItem];
										float width = item->GetWidthPt();
										if (width < areaTmp.Width)
										{
											AddToPart(item, areaTmp, items, properties);
											if (properties.m_HeightChanged)
												break;
										}
										else
											break;
									}

									((CTabStop*)itemTab)->GetRect().Width = areaTmp.Width;
									area.X += tabWidth;
									area.Width -= tabWidth;
									area.Height = areaTmp.Height;
									return true;
								}
								return false;
							}
						public:
							CItems() : 
							  m_Text(_T("")), 
								  m_Items(), m_Characters(), 
								  m_BlockState(), 
								  m_CurrentItem(0), m_CurrentItemFill(0), 
								  m_CurrentItemsSaved(),
								  m_IgnoreNegativeWidth(false),
								  m_Prepared(false)
							  {
							  }
							~CItems()
							  {
								  Clear();
							  }
							void	Clear()
							  {
								  m_Text	= _T("");

								  for (int index = 0; index < (int)m_Items.GetCount(); ++index)
									  RELEASEOBJECT(m_Items[index]);
								  m_Items.RemoveAll();

								  for (int index = 0; index < (int)m_Characters.GetCount(); ++index)
									  RELEASEOBJECT(m_Characters[index]);
								  m_Characters.RemoveAll();

								  m_CurrentItem = 0;
								  m_CurrentItemFill = 0;
								  assert(m_CurrentItemsSaved.empty());

								  m_IgnoreNegativeWidth = false;

								  m_Prepared = false;
							  }
						public:
							bool	m_IgnoreNegativeWidth;
						public:
							bool	IsEmpty() const
							{
								return ((m_Characters.GetCount() == 0) || (m_Text == _T("")));
							}
							bool	IsReady() const
							{
								return (m_Items.GetCount() != 0);
							}

							bool	AtBeginning() const
							{
								return (m_CurrentItem == 0);
							}
							bool	AtEnd() const
							{
								return m_CurrentItem >= (int)m_Items.GetCount() && (IsEmpty() || m_Prepared);
							}
							bool	Prepared() const
							{
								return m_Prepared;
							}
							void	InlineAdded()
							{
								m_BlockState.NumProcess(NumProcessed);
							}
						public:
							void	CurrentItem(const int& currentItem)
							{
								if (!Prepared())
									assert(Prepared());
								m_CurrentItem = currentItem;
							}
							int		CurrentItem()
							{
								assert(Prepared());
								return m_CurrentItem;
							}
							int		Count()
							{
								assert(Prepared());
								return (int)m_Items.GetCount();
							}
							CItem*	operator[](const int& index)
							{
								assert(Prepared());
								return m_Items[index];
							}
						public:
							void	SaveCurrentItem()
							{
								assert(Prepared());
								m_CurrentItemsSaved.push(m_CurrentItem);
							}
							void	RemoveCurrentItem()
							{
								assert(Prepared());
								assert(!m_CurrentItemsSaved.empty());
								m_CurrentItemsSaved.pop();
							}
							void	RestoreCurrentItem()
							{
								assert(Prepared());
								assert(!m_CurrentItemsSaved.empty());
								m_CurrentItem = m_CurrentItemsSaved.top();
								m_CurrentItemsSaved.pop();
							}
							bool	EmptySaves()
							{
								return m_CurrentItemsSaved.empty();
							}
							void	SaveCurrentItemFill()
							{
								m_CurrentItemFill = m_CurrentItem;
							}
							void	Renew(const bool& full = true)
							{
								if (!Prepared())
									return;
								//assert(Prepared());
								for (int index = 0; index < Count(); ++index)
								{
									m_Items[index]->RestoreRect();
									m_Items[index]->SetOnBaseline();
								}
								if (full)
								{
									m_CurrentItem = 0;
									m_CurrentItemFill = 0;
									m_BlockState.Renew();
								}
								else
									m_CurrentItem = m_CurrentItemFill;
							}
						public:
							void	AddItem(const CString& text, CCharacter* character, const ENumProcess& numProcess = NumProcessed)
							{
								assert(!Prepared());
								CString text2 = text;
								character->Transform(text2);
								character->CheckFontRange(text2);
								if (m_BlockState.NumProcess(numProcess))
								{
									m_Text += text2;
									m_Characters.Add(character);
								}
								else
								{
									m_Text = text2 + m_Text;
									m_Characters.InsertAt(0, character);
								}
							}
							void	Prepare(CGrapher* grapher, CBlockAttributes* blockAttributes, CAtlArray<CAlternate2*>& inlines, 
								CAtlArray<CAlternate2*>& anchors, CAtlArray<CFootnote*>& footnotes)
							{
								if (Prepared())
									return;
								grapher->m_DocumentManager->m_ItemManager.PrepareItems(m_Text, blockAttributes, m_Characters, 
									inlines, anchors, footnotes, m_Items, grapher);
								m_Prepared = true;
							}							
						public:
							void	FillArea(RectF& area, CArray<CItem*>& items, SPartProperties& properties, CGrapher* grapher)
							{
								bool first = AtBeginning();
								assert(!properties.m_HeightChanged);
								properties.m_State = EmptyPart;
								while (!AtEnd())
								{
									CItem* item = m_Items[m_CurrentItem];
									if (ProcessItem(item, area, properties, grapher))
										break;
									if (ProcessIfRightTab(item, area, items, properties, grapher))
									{
										if (properties.m_HeightChanged)
											break;
										continue;
									}

									if (CheckBreak(item, area, items, properties))
										break;

									float width = item->GetWidthPt();
									if (m_IgnoreNegativeWidth) {
										m_IgnoreNegativeWidth = false;
										area.Width = width;
									}
									if (width <= area.Width || properties.m_Tab || properties._ignoreWidthLimit) {
										CItemType itemType = item->GetType();
										UpdatePartState(itemType, properties, items);
										AddToPart(item, area, items, properties);
										if (properties.m_HeightChanged)
											break;
									}
									else
									{
										if (!RuptureAndAddToPart(item, area, items, properties, grapher))
											RuptureOrRemoveLastSymbolItemsInPart(item, area, items, properties, grapher);
										break;
									}
								}
								if (!first && properties.m_State == EmptyPart)
									items.RemoveAll();
							}
						};
						class State {
						public:
							static enum EStopPrintReason {
								AnyReason = 0, NotEnoughWidthReason
							};
						
						public:
							EStopPrintReason _stopPrintReason;
							bool _transferedToNextPage;

						public:
							State()
								: _stopPrintReason(AnyReason), 
								_transferedToNextPage(false) {
							}

						public:
							void reset() {
								_stopPrintReason = AnyReason;
								_transferedToNextPage = false;								
							}
						};
						
					private:
						class CComplexFieldChar
						{
						private:
							class CParam
							{
							private:
								CString	m_strName;
								CString	m_strValue;

							public:
								CParam(const CString& strName = _T(""), const CString& strValue = _T(""))
									: m_strName(strName), m_strValue(strValue)
								{
								}
								CParam(const CParam& oSrc)
								{
									m_strName = oSrc.m_strName;
									m_strValue = oSrc.m_strValue;
								}
							public:
								CString& Name()
								{
									return m_strName;
								}
								CString& Value()
								{
									return m_strValue;
								}
							};
							class CCode
							{
							private:
								CString			m_strName;
								CArray<CParam>	m_arrParams;
								bool			m_bIsContent;

							public:
								CCode(const CString& strName, const bool& bIsContent)
									: m_strName(strName), m_arrParams(), 
									m_bIsContent(bIsContent)
								{
								}
								CCode(const CCode& oSrc)
								{
									m_strName = oSrc.m_strName;
									m_arrParams.Copy(oSrc.m_arrParams);
									m_bIsContent = oSrc.m_bIsContent;
								}
							public:
								CString&	Name()
								{
									return m_strName;
								}
								void		AddParam(const CString& strName, const CString& strValue = _T(""))
								{
									CParam oParam(strName, strValue);
									m_arrParams.Add(oParam);
								}
								int			FindParam(const CString& strName)
								{
									int iParamsCount = (int)m_arrParams.GetCount();
									for (int iIndex = 0; iIndex < iParamsCount; ++iIndex)
										if (m_arrParams[iIndex].Name() == strName)
											return iIndex;
									return -1;
								}
								void		SetParam(const int& iIndex, const CString& strValue)
								{
									if (iIndex < 0)
										AddParam(_T("input"), strValue);
									else if (iIndex < (int)m_arrParams.GetCount())
										m_arrParams[iIndex].Value() = strValue;
								}
								CString		GetParam(const int& iIndex)
								{
									if ((iIndex >= 0) && (iIndex < (int)m_arrParams.GetCount()))
										return m_arrParams[iIndex].Value();
									else
										return _T("");
								}
								bool		IsOnlyString()
								{
									return !m_bIsContent;
								}
							};
							class CDateTime
							{
							private:
								SYSTEMTIME	m_oSystemTime;

							public:
								CDateTime()
								{
									GetSystemTime(&m_oSystemTime);
								}
							public:
								CString	GetDateAndTime(const CString& strFormat)
								{
									CString strDateTime = _T(" ");
									DWORD nLength = GetDateFormat(LOCALE_USER_DEFAULT, 0, NULL, strFormat, NULL, 0);
									if (nLength > 0)
									{
										TCHAR *pstrDateTime = new TCHAR[nLength];
										GetDateFormat(LOCALE_USER_DEFAULT, 0, &m_oSystemTime, strFormat,
											pstrDateTime, nLength);
										strDateTime = CString(pstrDateTime);
										RELEASEARRAYOBJECTS(pstrDateTime);
									}
									return strDateTime;
								}
							};
						private:
							CArray<CCode>	m_arrCodes;
							bool	m_isInTOC;
						public:
							byte	m_nState;
						public:
							CString	m_strCommand;
							CString m_strResult;
							int		m_iCodeIndex;
							CAtlArray<CString>	m_arrCommands;
							CCharacter *m_pCharacter;

						public:
							CComplexFieldChar()
								: m_arrCodes(), 
								m_nState(0), 
								m_strCommand(_T("")), m_strResult(_T("")), 
								m_iCodeIndex(-1),
								m_pCharacter(NULL)
							{
								InitCodes();
							}
							~CComplexFieldChar()
							{
								if (m_pCharacter != NULL)
									delete m_pCharacter;
							}
						public:
							bool	ProcessFieldCharNode(XmlUtils::CXmlNode& oFieldCharNode)
							{
								CString strType	= oFieldCharNode.GetAttribute(_T("type"));
								if (strType == _T("begin"))
								{
									Begin();
								}
								else if (strType == _T("separate"))
								{
									Separate();
									/*m_arrCommands.Add(m_strCommand);
									RecountPositionInTOC();*/
								}
								else if (strType == _T("end"))
								{
									End();

									/*size_t nCount = m_arrCommands.GetCount();
									if (nCount > 0)
									{
									m_arrCommands.RemoveAt(nCount - 1);
									}
									m_strCommand = _T("");
									RecountPositionInTOC();*/
								}
								return (m_nState == 0);
							}
							void	ProcessFieldChar()
							{
								if (IsSeparate())
								{
									int spaces = 0;
									for (; spaces < m_strCommand.GetLength(); ++spaces)
										if (m_strCommand[spaces] != _T(' '))
											break;
									m_arrCommands.Add(m_strCommand.Right(m_strCommand.GetLength() - spaces));
									RecountPositionInTOC();
								}
								else if (IsEnd())
								{									
									size_t nCount = m_arrCommands.GetCount();
									if (nCount > 0)
									{
										m_arrCommands.RemoveAt(nCount - 1);
									}
									m_strCommand = _T("");
									RecountPositionInTOC();
									//Parse();
								}
							}
							void	RecountPositionInTOC()
							{
								m_isInTOC = IsIn(_T("TOC"));
							}
							bool	IsIn(CString nameCommand)
							{
								size_t nCount = m_arrCommands.GetCount();
								for (size_t i = 0; i < nCount; ++i)
								{
									if (m_arrCommands[i].Find(nameCommand) != -1)
										return true;
								}
								return false;
							}
							bool	LastIs(CString nameCommand)
							{
								size_t nCount = m_arrCommands.GetCount();
								if (nCount == 0)
									return false;

								return (m_arrCommands[nCount - 1].Find(nameCommand) == 0);									
							}
							bool	IsInTOC()
							{
								return m_isInTOC;
							}							
							void	ProcessInstructionText(XmlUtils::CXmlNode& oInstructionTextNode)
							{
								AddText(oInstructionTextNode.GetTextExt());
							}
							bool	IsInProcess()
							{
								return (m_nState != 0);
							}
							bool	IsInCommandProcess()
							{
								return (m_nState == 1);
							}
							bool	IsInContentProcess()
							{
								return (m_nState == 2);
							}
							void	InCommandProcess()
							{
								m_nState = 1;
							}
							void	InContentProcess()
							{
								m_nState = 2;
							}
							bool	CanFillContent()
							{
								return !(IsIn(_T("PAGE")) && !IsIn(_T("PAGEREF")));
							}
							bool	IsOnlyString()
							{
								if (m_iCodeIndex == -1)
									return true;
								else
									return m_arrCodes[m_iCodeIndex].IsOnlyString();
							}
							CContent*	GetContent(CCharacter* pCharacter)
							{
								if (m_iCodeIndex == 0)
									return CreatePage(pCharacter);
								return NULL;
							}
							CString		GetString(CCharacter* pCharacter)
							{
								if (m_iCodeIndex == 1)
									return CreateMacrobutton(pCharacter);
								else if (m_iCodeIndex == 2)
									return CreateHyperlink(pCharacter);
								else if (m_iCodeIndex == 3)
									return CreateDate(pCharacter);
								pCharacter->m_lCount = 1;
								return _T(" ");
							}
							void	Interrupt()
							{
								m_strCommand = _T("");
								m_strResult = _T("");
								End();
							}
						private:
							void	Begin()
							{
								m_nState = 1;
							}
							bool	IsBegin()
							{
								return (m_nState == 1);
							}
							void	Separate()
							{
								m_nState = 2;
							}
							bool	IsSeparate()
							{
								return (m_nState == 2);
							}
							void	End()
							{
								m_nState = 0;
							}
							bool	IsEnd()
							{
								return (m_nState == 0);
							}
						private:
							void	AddText(const CString& strText)
							{
								if (IsBegin())
									m_strCommand += strText;
								else if (IsSeparate())
									m_strResult += strText;
							}
							void	Parse()
							{
								CString strCode = _T("");
								CString strParams = _T("");
								DetachCodeFromParams(m_strCommand, strCode, strParams);
								m_iCodeIndex	= FindCode(strCode);
								if (m_iCodeIndex != -1)
								{
									ParseParams(strParams);
								}
							}
							void	ParseParams(const CString& strParams)
							{
								int iParamIndex = -1;
								int iStartPos = 0;
								bool bNameNotValue = true;
								bool bCopyTheRest = false;
								int iLength = strParams.GetLength();
								while (iStartPos < iLength)
								{
									if (bNameNotValue)
									{
										int iPrevStartPos = iStartPos;
										iStartPos = FindFirstNotSpace(strParams, iStartPos);
										int iFinishPos = strParams.Find(_T(' '), iStartPos);
										CString strName = strParams.Mid(iStartPos, iFinishPos - iStartPos);
										iParamIndex = m_arrCodes[m_iCodeIndex].FindParam(strName);
										if (iParamIndex != -1)
										{
											iStartPos = iFinishPos;
											bNameNotValue = false;
										}
										else
										{
											iStartPos = iPrevStartPos;
											bNameNotValue = false;
											bCopyTheRest = true;
										}
									}
									else
									{
										if (bCopyTheRest)
										{
											CString strValue = strParams.Mid(iStartPos, iLength - iStartPos);
											m_arrCodes[m_iCodeIndex].SetParam(iParamIndex, strValue);
											break;
										}
										else
										{
											for (; iStartPos < iLength; ++iStartPos)
											{
												if (strParams[iStartPos] != _T(' '))
													break;
											}
											int iFinishPos = iStartPos;
											if (strParams[iStartPos] == _T('"'))
											{
												if (iStartPos == iLength - 1)
												{
													bCopyTheRest = true;
													continue;
												}
												CString strValue;
												iFinishPos = strParams.Find(_T('"'), ++iStartPos);
												if (iFinishPos == 0)
												{
													--iStartPos;
													iFinishPos = iLength;
													strValue = strParams.Mid(iStartPos, iFinishPos - iStartPos);
												}
												else
												{
													strValue = strParams.Mid(iStartPos, iFinishPos - iStartPos);
													++iFinishPos;
												}
												m_arrCodes[m_iCodeIndex].SetParam(iParamIndex, strValue);
												iStartPos = iFinishPos;
											}
											else
											{
												for (; iFinishPos < iLength; ++iFinishPos)
												{
													if (strParams[iFinishPos] == _T(' '))
														break;
												}
												CString strValue = strParams.Mid(iStartPos, iFinishPos - iStartPos);
												m_arrCodes[m_iCodeIndex].SetParam(iParamIndex, strValue);
												iStartPos = iFinishPos;
											}
											bNameNotValue = true;
										}
									}
								}
							}
							bool	IsParsed()
							{
								return (m_nState != 0);
							}
						private:
							int		FindFirstNotSpace(const CString& strText, int iStartIndex = 0)
							{
								int iIndex = iStartIndex;
								for (; iIndex < strText.GetLength(); ++iIndex)
									if (strText[iIndex] != _T(' '))
										break;
								return iIndex;
							}
							void	DetachCodeFromParams(CString strText, CString& strCode, CString& strParams)
							{
								int iCodeStartPosition = FindFirstNotSpace(strText);
								int iFirstAfterCodePosition = strText.Find(_T(" "), iCodeStartPosition);
								bool bCodeWithParams = true;
								if (iFirstAfterCodePosition == -1)
								{
									iFirstAfterCodePosition = strText.GetLength();
									bCodeWithParams = false;
								}
								strCode = strText.Mid(iCodeStartPosition, iFirstAfterCodePosition - iCodeStartPosition);

								if (bCodeWithParams)
								{
									int iParamsStartPosition = FindFirstNotSpace(strText, iFirstAfterCodePosition);
									strParams = strText.Mid(iParamsStartPosition, strText.GetLength() - iParamsStartPosition);
								}
								else
									strParams = _T("");
							}
							int		FindCode(const CString& strCode)
							{
								int iCodesCount = (int)m_arrCodes.GetCount();
								for (int iIndex = 0; iIndex < iCodesCount; ++iIndex)
									if (m_arrCodes[iIndex].Name() == strCode)
										return iIndex;
								return -1;
							}
						private:
							void	InitCodes()
							{
								m_arrCodes.Add(Page());
								m_arrCodes.Add(Macrobutton());
								m_arrCodes.Add(Hyperlink());
								m_arrCodes.Add(Date());
							}
							CCode	Page()
							{
								CCode oPage(_T("PAGE"), true);
								return oPage;
							}
							CContent*	CreatePage(CCharacter* pCharacter)
							{
								CPageNumberC* pPageNumberC = new CPageNumberC(pCharacter);
								return pPageNumberC;
							}
							CCode	Macrobutton()
							{
								CCode oMacrobutton(_T("MACROBUTTON"), false);
								oMacrobutton.AddParam(_T("NoMacro"));
								return oMacrobutton;
							}
							CString		CreateMacrobutton(CCharacter* pCharacter)
							{
								CString strText = _T("");
								int iParamNoMacroIndex = m_arrCodes[m_iCodeIndex].FindParam(_T("NoMacro"));
								if (iParamNoMacroIndex != -1)
								{
									strText += m_arrCodes[m_iCodeIndex].GetParam(iParamNoMacroIndex);
								}
								int iInputParamIndex = m_arrCodes[m_iCodeIndex].FindParam(_T("input"));
								if (iInputParamIndex != -1)
								{
									strText += m_arrCodes[m_iCodeIndex].GetParam(iInputParamIndex);
								}

								pCharacter->m_lCount = strText.GetLength();
								return strText;
							}
							CCode	Hyperlink()
							{
								CCode oHyperlink(_T("HYPERLINK"), false);
								return oHyperlink;
							}
							CString		CreateHyperlink(CCharacter* pCharacter)
							{
								CString strText = _T("");
								int iInputParamIndex = m_arrCodes[m_iCodeIndex].FindParam(_T("input"));
								if (iInputParamIndex != -1)
								{
									strText = m_arrCodes[m_iCodeIndex].GetParam(iInputParamIndex);
								}
								pCharacter->m_oLink.nMode = 2;
								pCharacter->m_oLink.strTarget = strText;
								pCharacter->m_oLink.strTarget.Remove('"');

								pCharacter->m_lCount = m_strResult.GetLength();
								return m_strResult;
							}
							CCode	Date()
							{
								CCode oDate(_T("DATE"), false);
								oDate.AddParam(_T("\\@"));
								return oDate;
							}
							CString		CreateDate(CCharacter* pCharacter)
							{
								CDateTime oDateTime;
								CString strFormat;
								int iParamFormatIndex = m_arrCodes[m_iCodeIndex].FindParam(_T("\\@"));
								if (iParamFormatIndex != -1)
								{
									strFormat = m_arrCodes[m_iCodeIndex].GetParam(iParamFormatIndex);
								}
								CString strDateTime = oDateTime.GetDateAndTime(strFormat);
								pCharacter->m_lCount = strDateTime.GetLength();

								return strDateTime;
							}
						};
						CComplexFieldChar	m_oComplexFieldChar;
						byte				m_nCharState;
					public:
						CBlockAttributes*	m_pBlockAttributes;

						CAlternates	m_Alternates;
						CItems	m_Items;
						State _state;

						CAtlArray<CFootnote*>	m_arrFootnotes;
						int						m_iFootnoteIndex;

						CString	m_strID;
						bool	m_mergeBordersWithNextBlock;
						bool	m_mergeBordersWithPrevBlock;

						bool m_removeSpaceBefore;
						bool m_removeSpaceAfter;
						bool m_IgnoreAnchors;

					private:
						BlockPr _blockPr;

						RCommand _blockStartCommand;
						RCommand _blockEndCommand;
						RCommand pageBreakCommand;

					public:
						CBlock() 
							: CCommonBlock(), 
							m_pBlockAttributes(NULL), 
							m_Alternates(), m_Items(), 
							m_arrFootnotes(), m_iFootnoteIndex(0), 
							m_strID(_T("")), 
							m_oComplexFieldChar(), m_nCharState(0),
							m_IgnoreAnchors(false) {
							m_mergeBordersWithNextBlock = false;
							m_mergeBordersWithPrevBlock = false;

							m_removeSpaceBefore = false;
							m_removeSpaceAfter = false;
						}
						~CBlock()
						{
							Clear();
						}
						void	Clear()
						{
							RELEASEOBJECT(m_pBlockAttributes);

							m_Items.Clear();
							m_Alternates.Clear();

							m_arrFootnotes.RemoveAll();
							m_iFootnoteIndex	= 0;

							m_strID	= _T("");

							m_nCharState	= 0;

							m_mergeBordersWithNextBlock = false;
							m_mergeBordersWithPrevBlock = false;

							m_removeSpaceBefore = false;
							m_removeSpaceAfter = false;
							m_IgnoreAnchors = false;
						}
					public:
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode, CDocumentManager& documentManager, CBlockAttributes* pBlockAttributes = NULL, 
							CCharacter* pCharacter = NULL) {
							Clear();
							documentManager.BlockCreated();

							readHeadingLevel(oXmlNode);
							LoadID(oXmlNode);

							if (pBlockAttributes == NULL)
								m_pBlockAttributes	= new CBlockAttributes();
							else
								m_pBlockAttributes	= pBlockAttributes;
							m_pBlockAttributes->FromXmlNode2(oXmlNode);

							CCharacter *pDefaultCharacter = NULL;
							if (pCharacter == NULL)
								pDefaultCharacter	= new CCharacter();
							else {
								pDefaultCharacter	= new CCharacter();
								*pDefaultCharacter	= *pCharacter;
							}
							m_oComplexFieldChar.RecountPositionInTOC();
							pDefaultCharacter->FromXmlNode2(oXmlNode);							

							ProcessNodesWithReader(oXmlNode, pDefaultCharacter, documentManager);
							RELEASEOBJECT(pDefaultCharacter);

							initCommands();
						}
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode, CDocumentManager& documentManager, const bool& bIsFirst, bool& inCommandField, CAtlArray<CString>&	curCommands)
						{
							if (inCommandField == true)
								m_oComplexFieldChar.InCommandProcess();
							m_oComplexFieldChar.m_arrCommands.Copy(curCommands);
							m_oComplexFieldChar.RecountPositionInTOC();

							FromXmlNode(oXmlNode, documentManager);
							if (bIsFirst)
							{
								m_pBlockAttributes->m_nBreakBeforeType	= 0;
							}

							if (m_oComplexFieldChar.IsInCommandProcess())
								inCommandField = true;
							else
								inCommandField = false;

							curCommands.Copy(m_oComplexFieldChar.m_arrCommands);
						}
						void	FromString(const CString& text, CGrapher* grapher, const NSStructures::CFont& font, const NSStructures::CBrush& brush, const NSStructures::CFormat& format)
						{
							int length = text.GetLength();
							m_pBlockAttributes = new CBlockAttributes();
							m_pBlockAttributes->m_lCount = length;
							m_pBlockAttributes->m_nTextAlignment = format.StringAlignmentHorizontal;

							CCharacter* character = new CCharacter();
							character->m_lCount = length;
							character->m_oAttributes.m_oFont.FromFont(font);
							character->m_oAttributes.m_oBrush.FromBrush(brush);

							m_Items.AddItem(text, character);
							PrepareItems(grapher);
						}
					public:
						inline bool	Empty()	const
						{
							return m_Items.IsEmpty();
						}
						inline bool	Complete() const {
							return AtEnd() || (Empty() && m_Alternates.Added())
								|| (_state._stopPrintReason == State::NotEnoughWidthReason);
						}
						inline bool	AtBeginning() const
						{
							return m_Items.AtBeginning();
						}
						inline bool	AtEnd() const
						{
							return m_Items.AtEnd();
						}
						inline bool	UsualBlock() const
						{
							return true;
						}
						inline byte KeepNext() const
						{
							return m_pBlockAttributes->m_nKeepNext;
						}
						inline void	IgnoreAnchors(const bool& ignoreAnchors)
						{
							m_IgnoreAnchors = ignoreAnchors;
						}
						void estimateWidth(CGrapher* pGrapher, float& widthMin, float& widthScore)
						{
							if (!m_Items.IsReady())
								PrepareItems(pGrapher);

							float widthMinEstimate = 0;
							float widthMinAddition = 0;

							float indentEstimate = 0;

							float& startIndent = m_pBlockAttributes->m_fStartIndent;
							float& endIndent = m_pBlockAttributes->m_fEndIndent;
							float& textIndent = m_pBlockAttributes->m_fTextIndent;

							if (startIndent > 0) {
								indentEstimate += startIndent;
								widthMinAddition += startIndent;
							}
							if (endIndent > 0) {
								indentEstimate += endIndent;
								widthMinAddition += endIndent;
							}

							widthScore = 0;

							if (textIndent > 0) {
								widthMinEstimate = textIndent;
								widthScore += textIndent;
							}

							int itemsCount = (int) m_Items.Count();
							for (int i = 0; i < itemsCount; ++i) {
								float itemWidth = m_Items[i]->GetRect().Width;
								if (itemWidth > widthMinEstimate)
									widthMinEstimate = itemWidth;
								widthScore += itemWidth;
							}

							widthMin = widthMinEstimate + widthMinAddition;
							widthScore += indentEstimate;
						}
						inline bool transferToNextPage(CGrapher* grapher) {
							if (!_state._transferedToNextPage) {
								Renew(grapher);
								_state._transferedToNextPage = true;

								return true;
							}
							else
								return false;
						}
					public:
						void	InputParams(CBlockInputParameters& inputParams, CDocumentManager& documentManager,  const bool& update = false)
						{
							if (!documentManager.UseHtmlSpacing())
								return;

							if (update)
							{
								m_pBlockAttributes->m_nSpaceBefore += inputParams.SpaceBefore();								
								inputParams.SpaceBefore(m_pBlockAttributes->m_nSpaceAfter);
							}
							else
							{
								// TODO:  ppt   -    
								if (inputParams.SpaceBefore() >= m_pBlockAttributes->m_nSpaceBefore)
								{
									inputParams.SpaceBefore(0.0f);
									m_pBlockAttributes->m_nSpaceBefore = 0.0f;
								}
								else
								{
									m_pBlockAttributes->m_nSpaceBefore -= inputParams.SpaceBefore();
								}
							}
						}
						void	Renew(CGrapher* grapher, const bool& full = true) {
							m_Alternates.Renew();
							m_Items.Renew(full);

							for (size_t nIndex = 0; nIndex < m_arrFootnotes.GetCount(); ++nIndex)
								m_arrFootnotes[nIndex]->Static(true);
							m_iFootnoteIndex = 0;

							m_Alternates.Update(grapher);

							if (full && m_pBlockAttributes->m_nBreakBeforeType < 0)
								m_pBlockAttributes->m_nBreakBeforeType *= -1;
							
							initCommands();
						}
					public:
						bool	FillRegion(CRegion &oRegion, CPage &oPage, const bool& bIsMustRenew, CGrapher* pGrapher) {
							CRegion origRegion = oRegion;
							RectF rectBefore = oRegion.GetFilledRectPt();
							if (m_removeSpaceBefore && m_pBlockAttributes->m_contextualSpacing == _T("true"))
								if (m_removeSpaceBefore)
									m_pBlockAttributes->m_nSpaceBefore = 0;

							if (m_removeSpaceAfter && m_pBlockAttributes->m_contextualSpacing == _T("true"))
								m_pBlockAttributes->m_nSpaceAfter = 0;

							if (CheckBreakBefore(oRegion, oPage))
								return true;

							if (bIsMustRenew)
								m_Alternates.Renew();

							PrepareItems(pGrapher);
							m_Alternates.Update(pGrapher);
							if (AtBeginning())
								m_Alternates.RegionSettings(oPage, oRegion);
							oPage.StartBlock();
							if (m_Items.Prepared())
								addAnchors(oPage, origRegion, m_Items.CurrentItem());
							else
								addAnchors(oPage, origRegion, 0);

							if (m_Items.IsEmpty() && !m_Alternates.HasInlines()) {
								return m_Alternates.HasAnchors();
							}
							if (bIsMustRenew && m_Items.Prepared())
								Renew(pGrapher);

							SetTabStops(oRegion, *pGrapher->m_DocumentManager);

							if (m_InColumn)
								m_Items.SaveCurrentItemFill();

							bool atBeginning = m_Items.AtBeginning();
							bool disableRemoveLastAnchors = pGrapher->m_DocumentManager->GetStaticProcessing();
							bool somePrinted = false;

							preparePrinting();

							if (m_pBlockAttributes->m_pTopSide != NULL && !m_mergeBordersWithPrevBlock)
								oRegion.TrancateRectPt(m_pBlockAttributes->m_pTopSide->m_oPen.m_oPen.Size / 0.3528f);

							if (AtBeginning() && !InTable())
								addCommandToPage(oPage, _blockStartCommand);

							do {
								CLine* line = getLine(oRegion, oPage, pGrapher);

								bool lineBefore = (line != NULL);
								if (lineBefore)
									lineBefore = (line->GetItemsCount() > 1);
								if (CheckBreakBefore(oRegion, oPage, lineBefore))
								{
									if (line) {
										oPage.AddLine(line);
										addAnchors(oPage, origRegion, line->GetRect(), m_Items.CurrentItem());
										somePrinted	= true;
										disableRemoveLastAnchors = true;
									}
									break;
								}
								if (line == NULL) {
									if (!Complete()) {
										oRegion.SetComplete();
										if ((m_pBlockAttributes->m_nKeepTogether) && atBeginning)
											if (somePrinted) {
												oPage.DeleteBlocks(1);
												Renew(pGrapher);
												disableRemoveLastAnchors = true;
											}
										if (!oPage.isWithNonDummyLines())
											disableRemoveLastAnchors = true;
									}
									break;
								}

								oPage.AddLine(line);
								addAnchors(oPage, origRegion, line->GetRect(),m_Items.CurrentItem());
								oRegion.SetProcessing();
								somePrinted = true;
							} while (true);

							if (AtEnd() && !InTable())
								addCommandToPage(oPage, _blockEndCommand, true);

							if (!AtEnd() && !disableRemoveLastAnchors)
								oPage.RemoveAnchorContent(m_Items.CurrentItem());

							RectF rectAfter = oRegion.GetFilledRectPt();

							RectF rectMetrics(rectAfter.GetLeft(), rectBefore.GetBottom(), rectAfter.GetRight() - rectAfter.GetLeft(), rectAfter.GetBottom() - rectBefore.GetBottom());							

							RectF rect = GetRectInRegion(rectMetrics);
							if (!oPage.InFootnoteProcess())	{
								DrawBorder(oPage, rect);
								DrawBackground(oPage, rect);
							}

							return somePrinted;
						}
					private:
						inline bool	Prepared()
						{
							return m_Items.Prepared();
						}
					private:
						RectF   GetRectInRegion(RectF rect)
						{

							double dStartIndent	= (double)m_pBlockAttributes->m_fStartIndent;
							double dEndIndent	= (double)m_pBlockAttributes->m_fEndIndent;
							double dTextIndent	= (double)m_pBlockAttributes->m_fTextIndent;
							double dSpaceAfter	= (double)m_pBlockAttributes->m_nSpaceAfter;
							double dSpaceBefore	= (double)m_pBlockAttributes->m_nSpaceBefore;
							if (dTextIndent < 0)
								dStartIndent += dTextIndent;
							//TODO   
							double dLeft	= rect.GetLeft() + dStartIndent - 4;
							double dRight	= rect.GetRight();
							double dTop		= rect.GetTop();
							double dBottom	= rect.GetBottom();// + 4;
							if (m_mergeBordersWithPrevBlock == false)
								dTop += dSpaceBefore;
							if (m_mergeBordersWithNextBlock == false)
								dBottom -= dSpaceAfter;

							return RectF(dLeft, dTop, dRight - dLeft, dBottom - dTop);
						}
						void	DrawBorder(CPage &oPage, RectF rect)
						{
							if (m_pBlockAttributes->m_pLeftSide != NULL)
							{
								oPage.AddGraphicLine(m_pBlockAttributes->m_pLeftSide->GetGraphicLine(rect.X, rect.Y, 
									rect.X, rect.Y + rect.Height));
							}
							if (m_pBlockAttributes->m_pRightSide != NULL)
							{
								oPage.AddGraphicLine(m_pBlockAttributes->m_pRightSide->GetGraphicLine(rect.X + rect.Width, rect.Y, 
									rect.X + rect.Width, rect.Y + rect.Height));
							}
							if ((m_pBlockAttributes->m_pTopSide != NULL) && !m_mergeBordersWithPrevBlock)
							{
								oPage.AddGraphicLine(m_pBlockAttributes->m_pTopSide->GetGraphicLine(rect.X, rect.Y, 
									rect.X + rect.Width, rect.Y));
							}
							if (m_pBlockAttributes->m_pBottomSide != NULL && !m_mergeBordersWithNextBlock)
							{
								oPage.AddGraphicLine(m_pBlockAttributes->m_pBottomSide->GetGraphicLine(rect.X, rect.Y + rect.Height, 
									rect.X + rect.Width, rect.Y + rect.Height));
							}
						}
						void	DrawBackground(CPage &oPage, RectF rect)
						{
							if (m_pBlockAttributes->m_pRect != NULL)
							{
								oPage.AddBackgroundRect(m_pBlockAttributes->GetBackgroundRect(rect.X, rect.Y, rect.Width, rect.Height));
							}
						}
					private:
						CLine* getLine(CRegion& region, CPage& page, CGrapher* grapher) {
							bool firstLine = false;
							if (AtBeginning()) {
								PrepareItems(grapher);
								firstLine = true;
							}
							else if (m_Items.AtBeginning())
								firstLine = true;
							if (m_Items.AtEnd())
								return NULL;

							grapher->m_DocumentManager->m_ItemManager.SetWrapFlags(m_pBlockAttributes->m_nWrapFlags);
							CAreaManager* areaManager = page.GetAreaManager();

							RectF lineArea = GetRectWithMargins(region.GetRectPt(), firstLine);
							if (!checkArea(lineArea))
								return NULL;

							if (!IgnoreNegativeWidth(lineArea))
								return NULL;

							m_Items.SaveCurrentItem();
							CLine* resultLine = CreateLine(lineArea, areaManager, firstLine, grapher);

							if (resultLine != NULL) {
								ApplyLineHeight(lineArea, resultLine, &grapher->m_DocumentManager->m_ItemManager);
								if (resultLine->IsContainsAlternate() && resultLine->GetHeightPt() > lineArea.Height)
									lineArea.Height = resultLine->GetHeightPt();
								
								float lineHeight = lineArea.Height;
								float lineHeightDelta = lineArea.Height - resultLine->GetHeightPt();
								float truncatedHeight = lineArea.Height + (lineArea.Y - region.GetRectPt().Y);
								if (AtEnd())
									ApplySpaceAfter(lineArea, &grapher->m_DocumentManager->m_ItemManager);

								if (region.GetRectPt().Height >= truncatedHeight || page.IsEmptyBody()
										|| !grapher->m_DocumentManager->m_ItemManager.IsFo()) {
									if (lineHeightDelta < 0) {
										float newDescent = lineHeight * (resultLine->getOriginDescent() / resultLine->GetHeightPt());
										float offset = lineHeightDelta + resultLine->GetDescent() - newDescent;
										resultLine->MoveOn(0, offset);
									}

									region.TrancateRectPt(lineArea.Height + (lineArea.Y - region.GetRectPt().Y));
									m_Items.RemoveCurrentItem();
									if (firstLine && m_strID != _T(""))
										grapher->m_DocumentManager->m_Linker.AddTarget(m_strID, resultLine->GetRect()
												, *grapher->m_DocumentManager);

									if (resultLine->GetFootnoteCount() > 0) {
										CRegion footnoteRegion	= region;

										page.StartFootnote();
										for (int i = 0; i < resultLine->GetFootnoteCount(); ++i) {
											if (m_iFootnoteIndex >= (int) m_arrFootnotes.GetCount())
												break;

											m_arrFootnotes[m_iFootnoteIndex]->FillRegion(footnoteRegion, page, grapher);
											++m_iFootnoteIndex;
										}
										page.FinishFootnote();

										float footnoteHeight = region.GetRegionHeightPt() - footnoteRegion.GetRegionHeightPt();
										region.TruncateHeightPt(footnoteHeight);
										page.changeFootnoteHeight(footnoteHeight);
									}
								}
								else {
									resultLine->RestoreItems();
									RELEASEOBJECT(resultLine);
									m_Items.RestoreCurrentItem();
								}
							}
							else
								m_Items.RemoveCurrentItem();

							return resultLine;
						}
						CLine*	CreateLine(RectF& oLineArea, CAreaManager* pAreaManager, const bool& bIsFirstLine, CGrapher* pGrapher)
						{
							CLine* pResultLine = NULL;
							RectF areaOrigin = oLineArea;

							InitLineArea(oLineArea);
							if (bIsFirstLine)
								ApplySpaceBefore(oLineArea, &pGrapher->m_DocumentManager->m_ItemManager);

							CArray<RectF> arrRests;
							REAL fAvaliableHeight	= pAreaManager->GetArea(oLineArea, arrRests);

							if (arrRests.GetCount() == 0) {
								CLine* line = new CLine();
								line->setDummy(true);
								line->GetRect().Height = oLineArea.Height;
								return line;
							}

							if (fAvaliableHeight < 0)
								pResultLine	= CreateLineInAreaNotWrap(oLineArea, bIsFirstLine, pGrapher);
							else
								pResultLine	= CreateLineInAreaWrap2(oLineArea, areaOrigin, bIsFirstLine, pAreaManager, pGrapher);

							if (pResultLine != NULL)
								pResultLine->MoveOn(arrRests[0].X, oLineArea.Y - pResultLine->GetRect().Y);
							return pResultLine;
						}
					private:
						CLine*	CreateLineInAreaWrap2(RectF& area, const RectF& areaOrigin, const bool& first, CAreaManager* areaManager, CGrapher* grapher)
						{
							CItemManager* itemManager = &grapher->m_DocumentManager->m_ItemManager;
							do
							{
								bool wrap = false;
								CArray<RectF> areas;
								areaManager->GetArea(area, areas);
								int areasCount = (int)areas.GetCount();
								if (areasCount > 0)
								{
									float wrapDelta = areas[0].Width - area.Width;
									if ((wrapDelta > 1) || (wrapDelta < -1))
										wrap = true;
									float offsetX = areas[0].X;
									CArray<CItem*>*	itemsInAreas = new CArray<CItem*>[areasCount];
									CItems::SPartProperties properties;
									properties.m_WrapBySymbol = itemManager->WrapBySymbol();
									properties.m_RuptureIfSingle = (areasCount == 1);
									properties.m_Force = m_InTable;
									m_Items.SaveCurrentItem();
									for (int index = 0; index < areasCount; ++index)
									{
										/*if (areas[index].Width <= 9)
											continue;*/
										properties.m_Single = true;
										FillArea(areas[index], itemsInAreas[index], properties, first, grapher);
										if (properties.m_HeightChanged)
										{
											area.Y = areaOrigin.Y;
											area.Height = areas[index].Height;
											area.Width = areaOrigin.Width;

											if (first)
												ApplySpaceBefore(area, itemManager);
											m_Items.RestoreCurrentItem();
											break;
										}
									}
									if (properties.m_HeightChanged)
									{
										properties.m_HeightChanged = false;
										continue;
									}

									m_Items.RemoveCurrentItem();

									if (properties.m_Break == CItems::PageBreak)
										m_pBlockAttributes->m_nBreakBeforeType = 1;
									else if ((properties.m_Break == CItems::ColumnBreak))
										m_pBlockAttributes->m_nBreakBeforeType = 2;

									if (areasCount == 1)
									{
										CArray<CItem*>& items = itemsInAreas[0];
										int itemsCount = (int)items.GetCount();
										CLine* line	= new CLine();
										for (int index = 0; index < itemsCount; ++index)
											line->AddItem(items[index]);
										if (areas[0].Width > 0)
											line->SetPersistWidth(areas[0].Width);
										line->SetAlign(GetTextAlign(), first, m_Items.AtEnd());
										return line;
									}
									else {
										CCompositeLine* compositeLine = new CCompositeLine();
										for (int index = 0; index < areasCount; ++index)
										{
											CArray<CItem*>& items = itemsInAreas[index];
											int itemsCount = (int)items.GetCount();
											CLine* line = new CLine();
											for (int index2 = 0; index2 < itemsCount; ++index2)
												line->AddItem(items[index2]);

											line->SetPersistWidth(areas[index].Width);
											line->MoveOn(areas[index].X - offsetX, 0.0f);

											compositeLine->AddItem(line);
										}
										compositeLine->SetAlign(GetTextAlign(), first, m_Items.AtEnd());
										return compositeLine;
									}
								}
								else
									return NULL;
							} 
							while (true);
						}
						CLine*	CreateLineInAreaNotWrap(RectF& area, const bool& first, CGrapher* grapher) {
							CArray<CItem*> items;
							RectF areaTmp = area;
							areaTmp.Height = 0;
							CItems::SPartProperties properties;
							properties.m_WrapBySymbol = grapher->m_DocumentManager->m_ItemManager.WrapBySymbol();
							properties.m_Force = m_InTable | !grapher->m_DocumentManager->m_ItemManager.IsFo();
							properties.m_Single = true;
							properties._limitTabs = m_InTable;
							properties.m_RuptureIfSingle = true;
							properties._isPpt = !grapher->m_DocumentManager->m_ItemManager.IsFo();
							properties._ignoreWidthLimit = properties._isPpt && m_pBlockAttributes->_oneLine;
							properties._firstLine = AtBeginning();
							m_Items.FillArea(areaTmp, items, properties, grapher);

							int itemsCount = (int)items.GetCount();
							if (itemsCount != 0)
							{
								if (properties.m_Break == CItems::PageBreak)
									m_pBlockAttributes->m_nBreakBeforeType = 1;
								else if ((properties.m_Break == CItems::ColumnBreak))
									m_pBlockAttributes->m_nBreakBeforeType = 2;

								CLine* line	= new CLine();
								for (int index = 0; index < itemsCount; ++index)
									line->AddItem(items[index]);

								area.Height	= line->GetHeightPt();
								if (areaTmp.Width > 0 || properties._ignoreWidthLimit)
									line->SetPersistWidth(areaTmp.Width);
								line->SetAlign(GetTextAlign(), first, m_Items.AtEnd());
								return line;
							}
							else
								return NULL;
						}
					private:
						void	FillArea(RectF& area, CArray<CItem*>& items, CItems::SPartProperties& properties, const bool& first, CGrapher* grapher)
						{
							RectF areaTmp = area;
							m_Items.FillArea(areaTmp, items, properties, grapher);
							area.Width = areaTmp.Width;
							area.Height = areaTmp.Height;
						}							
						void	InitLineArea(RectF& oLineArea)
						{
							oLineArea.Height	= m_Items[m_Items.CurrentItem()]->GetHeightPt();
						}

						void	SetTabStops(CRegion& oRegion, CDocumentManager& documentManager)
						{
							float fLeftBorder	= oRegion.GetRectPt().X;
							float fRightBorder	= fLeftBorder + oRegion.GetRectPt().Width;
							documentManager.SetDefaultTab(m_pBlockAttributes->m_fDefaultTabSize, fLeftBorder, fRightBorder);
							for (size_t nIndex = 0; nIndex < m_pBlockAttributes->m_arrTabs.GetCount(); ++nIndex)
								documentManager.AddTab(m_pBlockAttributes->m_arrTabs[nIndex]);							
						}
						RectF	GetRectWithMargins(const RectF &oRectF, const bool &bIsFirstLine)
						{
							RectF oRectFWithMargins	= oRectF;
							if (bIsFirstLine == true)
								if ((m_pBlockAttributes->m_pBulletProperties == NULL) || (m_pBlockAttributes->m_fTextIndent < 0))
								{
									oRectFWithMargins.X		+= m_pBlockAttributes->m_fTextIndent;
									oRectFWithMargins.Width	-= m_pBlockAttributes->m_fTextIndent;
								}
								oRectFWithMargins.X		+= m_pBlockAttributes->m_fStartIndent;
								oRectFWithMargins.Width	-= m_pBlockAttributes->m_fStartIndent + m_pBlockAttributes->m_fEndIndent;

								return oRectFWithMargins;
						}
					private:
						void	PrepareItems(CGrapher* grapher)
						{
							//m_Alternates.Update(grapher);
							m_Items.Prepare(grapher, m_pBlockAttributes, m_Alternates.Inlines(), m_Alternates.Anchors(), m_arrFootnotes);
						}

					private:
						void	ProcessNodesWithReader(XmlUtils::CXmlNode& oXmlNode, CCharacter* pDefaultCharacter, CDocumentManager& documentManager)
						{
							XmlUtils::CXmlNodes oNodes;
							if (oXmlNode.GetNodes(_T("*"), oNodes)) {
								INT iListLength	= oNodes.GetCount();

								for (INT iIndex = 0; iIndex < iListLength; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oNodes.GetAt(iIndex, oNode);

									ProcessNode(oNode, pDefaultCharacter, documentManager);
								} 
							}

							if (m_Alternates.HasAnchors() && !m_Alternates.isOnlyOneLetter() && m_Items.IsEmpty()) {
								CCharacter* settings = new CCharacter(*pDefaultCharacter);
								settings->m_lCount = 1;
								m_Items.AddItem(_T(" "), settings);
							}
						}
						void	ProcessNode(XmlUtils::CXmlNode& oNode, CCharacter* pDefaultCharacter, CDocumentManager& documentManager, const bool &bAsInline = false)	{
							CString strNodeName = oNode.GetName();
							if (m_Alternates.HasInlines())
								m_Items.InlineAdded();
							if (strNodeName == _T("field-char"))
							{		
								if (m_oComplexFieldChar.ProcessFieldCharNode(oNode)) {
									if (m_oComplexFieldChar.LastIs(_T("PAGE")) && !m_oComplexFieldChar.LastIs(_T("PAGEREF"))) {
										CCharacter *pCharacter;
										if (m_oComplexFieldChar.m_pCharacter != NULL)
											pCharacter = new CCharacter(*m_oComplexFieldChar.m_pCharacter);
										else
											pCharacter = new CCharacter(*pDefaultCharacter);
										CPageNumberC* pPageNumber	= new CPageNumberC(pCharacter, false);
										CAlternate2* pAlternate		= new CAlternate2();
										pAlternate->Inline(true);
										pAlternate->SetContent(pPageNumber, m_pBlockAttributes->m_lCount);
										m_Alternates.Add(pAlternate);
										if (m_oComplexFieldChar.m_pCharacter != NULL) {
											delete m_oComplexFieldChar.m_pCharacter;
											m_oComplexFieldChar.m_pCharacter = NULL;
										}
									} else if (m_oComplexFieldChar.LastIs(_T("NUMPAGES"))) {
										CCharacter *pCharacter;
										if (m_oComplexFieldChar.m_pCharacter != NULL)
											pCharacter = new CCharacter(*m_oComplexFieldChar.m_pCharacter);
										else
											pCharacter = new CCharacter(*pDefaultCharacter);
										CPageNumberC* pPageNumber	= new CPageNumberC(pCharacter, true);
										CAlternate2* pAlternate		= new CAlternate2();
										pAlternate->Inline(true);
										pAlternate->SetContent(pPageNumber, m_pBlockAttributes->m_lCount);
										m_Alternates.Add(pAlternate);
										if (m_oComplexFieldChar.m_pCharacter != NULL) {
											delete m_oComplexFieldChar.m_pCharacter;
											m_oComplexFieldChar.m_pCharacter = NULL;
										}
									}
								}
								m_oComplexFieldChar.ProcessFieldChar();
								/*if (m_oComplexFieldChar.IsInContentProcess() && (m_oComplexFieldChar.LastIs(_T("PAGE")) && !m_oComplexFieldChar.LastIs(_T("PAGEREF"))))
								{
								CCharacter *pCharacter	= new CCharacter(*pDefaultCharacter);
								CPageNumberC* pPageNumber	= new CPageNumberC(pCharacter, false);
								CAlternate2* pAlternate		= new CAlternate2();
								pAlternate->Inline(true);
								pAlternate->SetContent(pPageNumber, m_pBlockAttributes->m_lCount);
								m_Alternates.Add(pAlternate);	
								}	*/							
							}
							else if (strNodeName == _T("fo:inline"))
							{									
								CCharacter *pCharacter	= new CCharacter(*pDefaultCharacter);
								pCharacter->FromXmlNode3(oNode, m_oComplexFieldChar.IsInTOC());

								if (m_oComplexFieldChar.m_pCharacter == NULL)
									m_oComplexFieldChar.m_pCharacter = new CCharacter(*pCharacter);
								if (!m_oComplexFieldChar.IsInContentProcess() || m_oComplexFieldChar.CanFillContent())
								{
									ProcessNodesWithReader(oNode, pCharacter, documentManager);
									RELEASEOBJECT(pCharacter);
								}
							}
							else if (strNodeName == _T("text"))
							{
								/*if (m_oComplexFieldChar.IsInProcess())
								m_oComplexFieldChar.ProcessInstructionText(oNode);*/
								if (m_oComplexFieldChar.IsInCommandProcess())
									m_oComplexFieldChar.ProcessInstructionText(oNode);
								else if (!m_oComplexFieldChar.IsInContentProcess() || m_oComplexFieldChar.CanFillContent())
								{
									CCharacter *pCharacter	= new CCharacter(*pDefaultCharacter);
									pCharacter->FromXmlNode3(oNode, m_oComplexFieldChar.IsInTOC());
									if (pCharacter->m_lCount != 0)
									{
										m_pBlockAttributes->m_lCount	+= pCharacter->m_lCount;
										m_Items.AddItem(oNode.GetTextExt(), pCharacter);
									}
									else
										RELEASEOBJECT(pCharacter);
								}
							}
							else if (strNodeName == _T("fo:numbering")) {
								CString id = oNode.GetAttribute(_T("id"));
								CString numId = oNode.GetAttribute(_T("numId"));
								int level = oNode.ReadAttributeOrValueInt(_T("level"), -1);
								if (id == _T("") || level == -1)
									return;
								CCharacter* character = new CCharacter(*pDefaultCharacter);
								character->FromXmlNode2(oNode);
								CString numbering = documentManager.getListElement(id, numId, level);
								character->m_lCount = numbering.GetLength();
								if (character->m_lCount != 0) {
									_blockPr.setNumberingLevel(level);

									character->setIsLabel(true);
									m_pBlockAttributes->m_lCount += character->m_lCount;
									m_Items.AddItem(numbering, character, CItems::NumAdded);
								}
								else
									RELEASEOBJECT(character);
							}
							else if (strNodeName == _T("fo:tab"))
							{
								CCharacter *pCharacter = new CCharacter(*pDefaultCharacter);
								pCharacter->m_lCount = 1;
								m_pBlockAttributes->m_lCount += pCharacter->m_lCount;
								m_Items.AddItem(_T("\t"), pCharacter, CItems::NumAddedWithTab);
							}
							else if (strNodeName == _T("fo:leader"))
							{
								if (!m_oComplexFieldChar.IsInCommandProcess()) {
									CCharacter *pCharacter = new CCharacter(*pDefaultCharacter);
									pCharacter->FromXmlNode3(oNode, m_oComplexFieldChar.IsInTOC());
									pCharacter->m_lCount = 1;
									m_pBlockAttributes->m_lCount += 1;
									m_Items.AddItem(_T(" "), pCharacter);
								}
							}
							else if (strNodeName == _T("fo:basic-link"))
							{
								m_oComplexFieldChar.Interrupt();
								CLinkProperties oLink;
								oLink.FromXmlNode2(oNode);
								if (oLink.nMode != 0)
								{
									CLinkProperties oPrevLink	= pDefaultCharacter->m_oLink;
									pDefaultCharacter->m_oLink	= oLink;
									ProcessNodesWithReader(oNode, pDefaultCharacter, documentManager);
									pDefaultCharacter->m_oLink	= oPrevLink;
								}
							}
							else if (strNodeName == _T("fo:break"))
							{
								CCharacter *pCharacter	= new CCharacter(*pDefaultCharacter);
								pCharacter->m_lCount	= 1;
								CString strBreak		= oNode.GetAttributeOrValue(_T("break-before"), _T(""));
								if (strBreak == _T("page"))
									pCharacter->m_nAdditionalParam	= 1;
								else if (strBreak == _T("column"))
									pCharacter->m_nAdditionalParam	= 2;
								if (m_InTable)
									pCharacter->m_nAdditionalParam	= 0;
								m_pBlockAttributes->m_lCount += 1;
								m_Items.AddItem(_T("\n"), pCharacter, CItems::NumNone);
							}
							else if (strNodeName == _T("fo:alternate-content"))
							{
								CAlternate2	*pAlternate	= new CAlternate2();
								if (!pAlternate->FromXml(oNode, documentManager))
								{
									pAlternate->InlinePosition(m_pBlockAttributes->m_lCount);
									m_Alternates.Add(pAlternate);
								}
								else
									RELEASEOBJECT(pAlternate);
							}
							else if (strNodeName == _T("fo:footnote"))
							{
								ProcessNodesWithReader(oNode, pDefaultCharacter, documentManager);
							}
							else if (strNodeName == _T("fo:footnote-body"))
							{
								CFootnote* pFootnote = new CFootnote();
								pFootnote->FromXmlNode(oNode, documentManager);
								pFootnote->SetInlinePosition(m_pBlockAttributes->m_lCount);
								m_arrFootnotes.Add(pFootnote);
							}
							else if (strNodeName == _T("fo:page-number"))
							{
								CCharacter *pCharacter	= new CCharacter(*pDefaultCharacter);
								CPageNumberC* pPageNumber	= new CPageNumberC(pCharacter, false);
								CAlternate2* pAlternate		= new CAlternate2();
								pAlternate->Inline(true);
								pAlternate->SetContent(pPageNumber, m_pBlockAttributes->m_lCount);
								m_Alternates.Add(pAlternate);								
							}
							else if (strNodeName == _T("fo:page-number-citation"))
							{
								CCharacter *pCharacter	= new CCharacter(*pDefaultCharacter);
								CPageNumberC* pPageNumber	= new CPageNumberC(pCharacter, true);
								CAlternate2* pAlternate		= new CAlternate2();
								pAlternate->Inline(true);
								pAlternate->SetContent(pPageNumber, m_pBlockAttributes->m_lCount);
								m_Alternates.Add(pAlternate);
							}
						}
						void	LoadID(XmlUtils::CXmlNode& oXmlNode)
						{
							m_strID	= oXmlNode.GetAttributeOrValue(_T("id"), _T(""));
						}
						void readHeadingLevel(XmlUtils::CXmlNode& blockXml) {
							_blockPr.setHeadingLevel(blockXml.ReadAttributeInt(_T("headingLvl"), 0));
						}
						bool	CreateHyperlink(XmlUtils::CXmlNode& oXmlNode, CLinkProperties& oLink)
						{
							static CString strHyperlink	= _T("HYPERLINK");
							CString strText	= oXmlNode.GetText();
							INT iIndex		= 0;
							while (strText[iIndex] == _T(' '))
							{
								++iIndex;
							}
							INT iIndex2	= 0;
							for (; iIndex2 < 9; ++iIndex2)
							{
								if (strText[iIndex + iIndex2] != strHyperlink[iIndex2])
								{
									break;
								}
							}
							if (iIndex2 == 9)
							{
								INT iPosL	= strText.Find(_T('"'), 0);
								INT iPosR	= -1;
								if (iPosL >= 0)
								{
									++iPosL;
									iPosR	= strText.Find(_T('"'), iPosL);
								}
								if (iPosR >= 0)
								{
									CString strID	= strText.Mid(iPosL, iPosR - iPosL);
									oLink.strTarget		= strID;
								}

								return true;
							}
							else
							{
								return false;
							}
						}
						void addAnchors(CPage& page, CRegion& region, const int& position = -1) {
							RectF line = region.GetRectPt();
							line.Height = 0;
							
							addAnchors(page, region, line, position);
						}
						void addAnchors(CPage& page, CRegion& region, const RectF& line, const int& position = -1) {
							if (!m_IgnoreAnchors)
								m_Alternates.Place(page, region, m_pBlockAttributes, line, position);
						}
					private:
						void preparePrinting() {
							_state._stopPrintReason = State::AnyReason;
						}
					private:
						bool checkArea(const RectF& area) {
							if (area.Width <= 0) {
								_state._stopPrintReason = State::NotEnoughWidthReason;
								return false;
							}

							return true;
						}
						bool	CheckBreakBefore(CRegion& oRegion, CPage &oPage, const bool& lineBefore = false)
						{
							if ((m_pBlockAttributes->m_nBreakBeforeType > 0))
							{
								if (oPage.IsEmptyBody() && !lineBefore) {
									m_pBlockAttributes->m_nBreakBeforeType = 0;
									return false;
								}
								oRegion.SetComplete();
								oRegion.BreakType(m_pBlockAttributes->m_nBreakBeforeType);
								m_pBlockAttributes->m_nBreakBeforeType *= -1;

								addCommandToPage(oPage, pageBreakCommand);

								return true;
							}
							else
								return false;
						}
						CAlignHorizontal	GetTextAlign()
						{
							CAlignHorizontal eTextAlign	= LeftAlign;							
							switch (m_pBlockAttributes->m_nTextAlignment)
							{
							case 1:
								eTextAlign = CenterAlign;
								break;
							case 2:
								eTextAlign = RightAlign;
								break;
							case 3:
								if (!m_Items.AtEnd())
									eTextAlign = WideAlign;
								break;
							}

							return eTextAlign;
						}
						inline void		ApplySpaceBefore(RectF& oLineArea, CItemManager* pItemManager)
						{
							oLineArea.Y += pItemManager->GetPlcOrPxl(m_pBlockAttributes->m_nSpaceBefore, oLineArea.Height);
						}
						inline void		ApplySpaceAfter(RectF& oLineArea, CItemManager* pItemManager)
						{
							oLineArea.Height += pItemManager->GetPlcOrPxl(m_pBlockAttributes->m_nSpaceAfter, oLineArea.Height);
						}
						inline void		ApplyLineHeight(RectF& oLineArea, CLine *pLine, CItemManager* pItemManager)
						{
							if (m_pBlockAttributes->m_fLineHeight != 0)
							{
								if (pLine->IsContainsAlternate())
								{
									REAL fSpaceAfter	= pItemManager->GetPlcOrPxl(m_pBlockAttributes->m_fLineHeight, pLine->GetTextHeight()) - pLine->GetTextHeight();
									oLineArea.Height	+= fSpaceAfter;
								}
								else
								{
									oLineArea.Height = pItemManager->GetPlcOrPxl(m_pBlockAttributes->m_fLineHeight, oLineArea.Height);
								}
							}
							else if ((m_pBlockAttributes->m_nLineSpacing != 0))
							{
								oLineArea.Height = pItemManager->GetPlcOrPxl(m_pBlockAttributes->m_nLineSpacing, oLineArea.Height, 1);
							}
						}
					private:
						bool	IgnoreNegativeWidth(const RectF& area)
						{
							if ((area.Width <= 0) && (!InTable()))
								m_Items.m_IgnoreNegativeWidth = true;
							else
								m_Items.m_IgnoreNegativeWidth = false;
							return true;
						}
					private:
						void initCommands() {
							_blockStartCommand = SharedPtr<RendererCommand>(
									new BlockStartRendererCommand(
									_blockPr.getHeadingLevel(), _blockPr.getNumberingLevel(), 
									m_pBlockAttributes->m_nTextAlignment));
							_blockEndCommand = SharedPtr<RendererCommand>(
									new BlockEndRendererCommand(_blockStartCommand));
							pageBreakCommand = SharedPtr<RendererCommand>(new PageBreakRendererCommand());
						}
					};

					class CTable : public CCommonBlock, private CXmlAttributeReader	{
					private:
						class CCell
						{
						public:
							CFlow	*m_pFlow;

							INT	m_iNumberColumnsSpanned;
							INT	m_iNumberRowsSpanned;
							REAL	m_fWidth;

							INT	m_bCarring;
							bool _negativeRect;

							RectF	m_oRectF;

							CPaddingProperties		m_oPaddingProperties;
							CBorderProperties		m_oBorderProperties;
							CBackgroundProperties	m_oBackgroundProperties;
						
						private:
							float _widthMin;
							float _widthScore;

						public:
							CCell() 
								: m_pFlow(NULL), 
								m_iNumberColumnsSpanned(1), m_iNumberRowsSpanned(1), 
								m_fWidth(0.0f),
								m_bCarring(0), _negativeRect(false), 
								m_oRectF(), 
								m_oPaddingProperties(), m_oBorderProperties(), m_oBackgroundProperties(),
								_widthMin(-1), _widthScore(-1)
							{
							}
							~CCell()
							{
								Clear();
							}
							void	Clear()
							{
								RELEASEOBJECT(m_pFlow);

								m_iNumberColumnsSpanned	= 1;
								m_iNumberRowsSpanned	= 1;
								m_fWidth	= 0.0f;

								m_bCarring	= 0;

								m_oRectF	= RectF();

								m_oPaddingProperties.Clear();
								m_oBackgroundProperties.Clear();
								m_oBorderProperties.Clear();

								_widthMin = -1;
								_widthScore = -1;
							}
						public:
							bool	FromXmlNode(XmlUtils::CXmlNode &oXmlNode, CDocumentManager& documentManager)
							{
								CString strVMerge	= oXmlNode.GetAttributeOrValue(_T("v-merge"), _T("reset"));
								if (strVMerge == _T("continue"))
								{
									Clear();
									return false;
								}

								m_pFlow	= new CFlow();
								m_pFlow->FromXmlNode(oXmlNode, documentManager, TRUE);

								m_iNumberColumnsSpanned	= oXmlNode.ReadAttributeOrValueInt(_T("number-columns-spanned"), 1);
								m_iNumberRowsSpanned	= oXmlNode.ReadAttributeOrValueInt(_T("number-rows-spanned"), 1);

								m_oPaddingProperties.FromXmlNode(oXmlNode);
								m_oBorderProperties.FromXmlNode2(oXmlNode);
								m_oBackgroundProperties.FromXmlNode2(oXmlNode);

								return true;
							}
						public:
							bool	Complete()
							{
								return m_pFlow->Complete() || _negativeRect;
							}
							bool	IsEmpty()
							{
								return m_pFlow->Empty();
							}
							void estimateWidth(CGrapher* grapher, float& widthMin, float& widthScore)
							{
								if ((_widthMin >= 0) || (_widthScore >= 0)) {
									widthMin = _widthMin;
									widthScore = _widthScore;
								}
								else if (m_pFlow) {
									m_pFlow->estimateWidth(grapher, widthMin, widthScore);
								} else {
									widthMin = _widthMin = 0;
									widthScore = _widthScore = 0;
								}
							}							
						public:
							bool	FillRegion(CRegion& region, CPage& page, const bool& isMustRenew, CGrapher* grapher)
							{
								float fSize = 0.0f;
								if (m_oBorderProperties.m_pLeftSide != NULL)
									fSize = (REAL)m_oBorderProperties.m_pLeftSide->m_oPen.m_oPen.Size;

								CBounds oPaddings;
								oPaddings.SetAsMarginPt(m_oPaddingProperties.m_fPaddingLeft + fSize, m_oPaddingProperties.m_fPaddingTop, 
									m_oPaddingProperties.m_fPaddingRight, m_oPaddingProperties.m_fPaddingBottom);

								oPaddings.SetAsRectPt(region.GetRectPt());
								RectF rect = oPaddings.GetRectPt();
								region.SetRect(rect);

								page.beginCell();

								bool result = true;
								if (rect.Width > 0) {
									region.SetReady();
									result = m_pFlow->FillRegion(region, page, grapher);
								}
								else {
									_negativeRect = true;
									region.SetComplete();
								}

								page.endCell();

								return result;
							}							
							void	DrawBorder(CPage &oPage)
							{
								if (m_oBorderProperties.m_pLeftSide != NULL)
								{
									oPage.AddGraphicLine(m_oBorderProperties.m_pLeftSide->GetGraphicLine(m_oRectF.X, m_oRectF.Y, 
										m_oRectF.X, m_oRectF.Y + m_oRectF.Height));
								}
								if (m_oBorderProperties.m_pRightSide != NULL)
								{
									oPage.AddGraphicLine(m_oBorderProperties.m_pRightSide->GetGraphicLine(m_oRectF.X + m_oRectF.Width, m_oRectF.Y, 
										m_oRectF.X + m_oRectF.Width, m_oRectF.Y + m_oRectF.Height));
								}
								if ((m_oBorderProperties.m_pTopSide != NULL) )
								{
									oPage.AddGraphicLine(m_oBorderProperties.m_pTopSide->GetGraphicLine(m_oRectF.X, m_oRectF.Y, 
										m_oRectF.X + m_oRectF.Width, m_oRectF.Y));
								}
								if (m_oBorderProperties.m_pBottomSide != NULL)
								{
									oPage.AddGraphicLine(m_oBorderProperties.m_pBottomSide->GetGraphicLine(m_oRectF.X, m_oRectF.Y + m_oRectF.Height, 
										m_oRectF.X + m_oRectF.Width, m_oRectF.Y + m_oRectF.Height));
								}
								/*/
								oPage.AddGraphicLine(m_oBorderProperties.m_pBottomSide->GetGraphicLine(m_oRectF.X, m_oRectF.Y, 
								m_oRectF.X + m_oRectF.Width, m_oRectF.Y + m_oRectF.Height));
								//*/

								--m_bCarring;
							}
							void	DrawBackground(CPage &oPage)
							{
								if (m_oBackgroundProperties.m_pRect != NULL)
								{
									oPage.AddBackgroundRect(m_oBackgroundProperties.GetBackgroundRect(m_oRectF.X, m_oRectF.Y, m_oRectF.Width, m_oRectF.Height));
								}
							}
						};

					private:
						CAtlArray<float> m_arrColumnWidth;
						bool _colsCalculated;

						CAtlArray<float> m_arrRowHeight;
						INT				m_iColumnCount;
						INT				m_iRowCount;

						REAL			m_fWidth;
						REAL			m_fSummaryWidth;

						INT				m_iCurrentRow;
						INT				m_iPageStartRow;

						CAtlArray<CCell*>	m_arrCells;

						CBlockAttributes _blockAttributes;

						RCommand _tableStartCommand;
						RCommand _tableEndCommand;

					public:
						CTable()
							: CCommonBlock()
							, _colsCalculated(false)
							, m_iColumnCount(0), m_iRowCount(0)
							, m_fWidth(0.0f), m_fSummaryWidth(0.0f)
							, m_iCurrentRow(-1), m_iPageStartRow(0) {
						}
						~CTable() {
							Clear();
						}
						void	Clear() {
							m_arrColumnWidth.RemoveAll();
							m_arrRowHeight.RemoveAll();
							m_iColumnCount	= 0;
							m_iRowCount		= 0;

							m_fWidth		= 0.0f;
							m_fSummaryWidth	= 0.0f;

							m_iCurrentRow	= -1;
							m_iPageStartRow	= 0;

							for (size_t nIndex1 = 0; nIndex1 < m_arrCells.GetCount(); ++nIndex1)
							{
								if (m_arrCells[nIndex1] != NULL)
								{
									for (size_t nIndex2 = nIndex1 + 1; nIndex2 < m_arrCells.GetCount(); ++nIndex2)
									{
										if (m_arrCells[nIndex2] == m_arrCells[nIndex1])
										{
											m_arrCells[nIndex2]	= NULL;
										}
									}
									RELEASEOBJECT(m_arrCells[nIndex1]);									
								}
							}

							m_arrCells.RemoveAll();

							_blockAttributes.Clear();

							initCommands();
						}
					public:
						bool	FromXmlNode(XmlUtils::CXmlNode &oXmlNode, CDocumentManager& documentManager) {
							Clear();
							documentManager.BlockCreated();
							documentManager.SetStaticProcessing(true);

							setCommands(oXmlNode.GetXml());

							m_fWidth = FloatPtAttributeFromXml(oXmlNode, _T("width"), -1.0f);

							m_iCurrentRow	= 0;
							_blockAttributes.FromXmlNode2(oXmlNode);

							if (!TryLoadCols(oXmlNode)) {
								m_iColumnCount = CalculateMaxCols(oXmlNode);
								if (m_iColumnCount > 0)
									m_arrColumnWidth.InsertAt(0, -1.0f, m_iColumnCount);
							}

							XmlUtils::CXmlNode oBodyNode;
							if (oXmlNode.GetNode(_T("fo:table-body"), oBodyNode))
							{
								XmlUtils::CXmlNodes oRowNodes;
								if (oBodyNode.GetNodes(_T("fo:table-row"), oRowNodes))
								{
									m_iRowCount	= oRowNodes.GetCount();
									if (m_iRowCount == 0)
									{
										documentManager.SetStaticProcessing(false);
										return false;
									}

									//m_arrCells.SetCount(m_iColumnCount * m_iRowCount);
									//for (int iIndex = 0; iIndex < (int)m_arrCells.GetCount(); ++iIndex)
									//	m_arrCells[iIndex]	= NULL;
									m_arrCells.InsertAt(0, NULL, m_iColumnCount * m_iRowCount);
										
									for (INT iIndex = 0; iIndex < m_iRowCount; ++iIndex)
									{
										XmlUtils::CXmlNode oRowNode;
										oRowNodes.GetAt(iIndex, oRowNode);
										LoadNewRow(oRowNode, documentManager);

										REAL fRowHeight	= oRowNode.ReadAttributeOrValueFloat(_T("height"), 0);
										m_arrRowHeight.Add(fRowHeight);
									}
								}
							}

							m_iCurrentRow	= 0;
							documentManager.SetStaticProcessing(false);

							return true;
						}
					public:
						bool	Empty()	const
						{
							return (m_iCurrentRow == -1);
						}
						bool	Complete() const
						{
							return (m_iCurrentRow == -1);							
						}
						bool	UsualBlock() const
						{
							return false;
						}
						void estimateWidth(CGrapher* grapher, float& widthMin, float& widthScore)
						{
							widthMin = 0;
							widthScore = 0;

							int rowsCount = m_iRowCount;
							int colsCount = m_iColumnCount;

							int index = 0;
							for (int i = 0; i < rowsCount; ++i) {
								float rowWidthMin = 0;
								for (int j = 0; j < colsCount; ++j) {
									if (!m_arrCells[index])
										continue;

									float cellWidthMin = 0;
									float cellWidthScore = 0;
									m_arrCells[index]->estimateWidth(grapher, cellWidthMin, cellWidthScore);

									rowWidthMin += cellWidthMin;
									widthScore += cellWidthScore;

									++index;
								}
							}
						}
					public:
						void	Renew(CGrapher* pGrapher, const bool& full = true) {
							_colsCalculated = false;
							m_iCurrentRow	= 0;
							m_iPageStartRow	= 0;

							for (size_t nIndex1 = 0; nIndex1 < m_arrCells.GetCount(); ++nIndex1)
								if  ((m_arrCells[nIndex1] != NULL) && (m_arrCells[nIndex1]->m_pFlow != NULL))
									m_arrCells[nIndex1]->m_pFlow->Static(true);

							initCommands();
						}
					public:
						bool	FillRegion(CRegion &oRegion, CPage &oPage, const bool& bIsMustRenew, CGrapher* pGrapher) {
							if (m_fWidth < 0)
								m_fWidth *= -oRegion.GetRectPt().Width;

							if (!InTable())
								addCommandToPage(oPage, _tableStartCommand);
							
							normalizeWidth();

							if (bIsMustRenew)
								Renew(pGrapher);

							CRegion oRegionWithMargins = makeRegionWithSettings(oRegion);

							bool bIsSomePrinted	= false;

							if (!_colsCalculated) {
								calcColsWidths(m_arrColumnWidth, m_fWidth, *pGrapher);
								_colsCalculated = true;
							}

							pGrapher->m_DocumentManager->SetStaticProcessing(true);
							int iPreviousRow = 0;
							while (m_iCurrentRow != -1) {
								iPreviousRow	= m_iCurrentRow;
								oRegion.SetProcessing();

								if (!OutputRow(oRegionWithMargins, oPage, pGrapher)) {
									bIsSomePrinted	= true;
									oRegion.SetComplete();
									break;
								}
							}
							pGrapher->m_DocumentManager->SetStaticProcessing(false);

							if (!InTable() && m_iCurrentRow == -1)
								addCommandToPage(oPage, _tableEndCommand, true);

							m_iPageStartRow	= iPreviousRow;

							oRegion.TrancateRectPt(oRegion.GetRegionHeightPt() - oRegionWithMargins.GetRegionHeightPt());

							return bIsSomePrinted;
						}
					private:
						bool	OutputRow(CRegion &oRegion, CPage &oPage, CGrapher* pGrapher)
						{
							bool bIsCompletePrinted	= true;
							bool bIsSomePrinted		= false;

							REAL fRowHeight	= 0.0f;
							REAL fOffsetX	= 0.0f;

							CAtlArray<CCell*> arrRowCells;
							CAtlArray<CCell*> arrUnrowCells;
							CAtlArray<CCell*> arrLastUnrowCells;

							INT iCurrentCellIndex	= 0;
							INT iRawStartIndex		= m_iCurrentRow * m_iColumnCount;
							CCell *pCell			= NULL;

							while (iCurrentCellIndex < m_iColumnCount)
							{
								pCell				= m_arrCells[iRawStartIndex + iCurrentCellIndex];
								if (!pCell)
								{
									++iCurrentCellIndex;
									continue;
								}

								if (m_arrColumnWidth[iCurrentCellIndex] == 0.0f)
								{
									++iCurrentCellIndex;
									continue;
								}

								BOOL bIsLastRow	= FALSE;
								if (m_iCurrentRow == m_iRowCount - 1)
								{
									bIsLastRow	= TRUE;
								}
								else if (pCell != m_arrCells[iRawStartIndex + m_iColumnCount + iCurrentCellIndex])
								{
									bIsLastRow	= TRUE;
								}

								if (bIsLastRow == TRUE)
								{
									arrRowCells.Add(pCell);
								}
								else
								{
									arrUnrowCells.Add(pCell);
								}

								BOOL bIsDrawing	= FALSE;
								if (m_iCurrentRow == m_iPageStartRow)
								{
									bIsDrawing	= TRUE;
								}
								else if (pCell != m_arrCells[iRawStartIndex - m_iColumnCount + iCurrentCellIndex])
								{
									bIsDrawing	= TRUE;
								}
								else
								{
									arrLastUnrowCells.Add(pCell);
									if (!pCell->Complete())
									{
										bIsDrawing	= TRUE;
									}
								}

								REAL fCellHeight	= 0.0f;

								if (bIsDrawing == TRUE)
								{
									RectF	oCellRect	= oRegion.GetRectPt();
									oCellRect.X			+= fOffsetX;
									oCellRect.Width		= 0.0f;

									while (pCell == m_arrCells[iRawStartIndex + iCurrentCellIndex])
									{
										oCellRect.Width	+= m_arrColumnWidth[iCurrentCellIndex];
										++iCurrentCellIndex;
										if (iCurrentCellIndex == m_iColumnCount)
										{
											break;
										}
									}
									pCell->m_oRectF	= oCellRect;
									CRegion oCellRegion;
									oCellRegion.SetReady();
									oCellRegion.SetRect(oCellRect);
									if (pCell->FillRegion(oCellRegion, oPage, false, pGrapher))
									{
										bIsSomePrinted	= true;
										pCell->m_oRectF.Height	= oRegion.GetRegionHeightPt() - oCellRegion.GetRegionHeightPt();
									}
									else
									{
										pCell->m_oRectF.Height	= 0.0f;
									}

									if (bIsLastRow == TRUE)
									{
										fCellHeight	= pCell->m_oRectF.Height;
									}
									else
									{
										fCellHeight	= 0.0f;
									}
								}
								else 
								{
									iCurrentCellIndex += pCell->m_iNumberColumnsSpanned;

									if (bIsLastRow == TRUE)
									{
										fCellHeight	= pCell->m_oRectF.Height;
									}
								}

								fOffsetX	+= pCell->m_oRectF.Width;

								if (fRowHeight < fCellHeight)
								{
									fRowHeight	= fCellHeight;
								}

								if (!pCell->Complete())
								{
									bIsCompletePrinted	= FALSE;
									pCell->m_bCarring	= 2;
								}
							}

							if (fRowHeight < m_arrRowHeight[m_iCurrentRow])
							{
								fRowHeight	= m_arrRowHeight[m_iCurrentRow];
							}
							if (fRowHeight == 0)
								fRowHeight = 12;

							if (bIsSomePrinted == TRUE)
							{
								for (size_t nIndex = 0; nIndex < arrRowCells.GetCount(); ++nIndex)
								{
									if (bIsCompletePrinted == FALSE)
									{
										arrRowCells[nIndex]->m_bCarring	= 2;
									}
									arrRowCells[nIndex]->m_oRectF.Height	=  fRowHeight + oRegion.GetRectPt().Y - arrRowCells[nIndex]->m_oRectF.Y;
									arrRowCells[nIndex]->DrawBorder(oPage);
									arrRowCells[nIndex]->DrawBackground(oPage);
								}
							}

							if (bIsCompletePrinted == FALSE)
							{
								for (size_t nIndex = 0; nIndex < arrUnrowCells.GetCount(); ++nIndex)
								{
									REAL fFullHeight	= arrUnrowCells[nIndex]->m_oRectF.Height;

									arrUnrowCells[nIndex]->m_oRectF.Height	=  fRowHeight + oRegion.GetRectPt().Y - arrUnrowCells[nIndex]->m_oRectF.Y;
									arrUnrowCells[nIndex]->DrawBorder(oPage);
									arrUnrowCells[nIndex]->DrawBackground(oPage);

									arrUnrowCells[nIndex]->m_oRectF.Height	= 0;
								}
								if (m_iCurrentRow != 0)
								{
									for (size_t nIndex = 0; nIndex < arrLastUnrowCells.GetCount(); ++nIndex)
									{
										arrLastUnrowCells[nIndex]->m_oRectF.Height	= oRegion.GetRectPt().Y - arrLastUnrowCells[nIndex]->m_oRectF.Y;
										arrLastUnrowCells[nIndex]->DrawBorder(oPage);
										arrLastUnrowCells[nIndex]->DrawBackground(oPage);

										arrLastUnrowCells[nIndex]->m_oRectF.Height	= 0;
									}
								}
							}
							else
							{
								for (size_t nIndex = 0; nIndex < arrUnrowCells.GetCount(); ++nIndex)
								{
									arrUnrowCells[nIndex]->m_oRectF.Height	-= fRowHeight;
								}
							}

							oRegion.TrancateRectPt(fRowHeight);

							arrRowCells.RemoveAll();
							arrUnrowCells.RemoveAll();
							arrLastUnrowCells.RemoveAll();

							if (bIsCompletePrinted == TRUE)
							{
								++m_iCurrentRow;
								if (m_iCurrentRow == m_iRowCount)
								{
									m_iCurrentRow	= -1;
								}
							}

							return bIsCompletePrinted;
						}
					private:
						bool	TryLoadCols(XmlUtils::CXmlNode &oXmlNode)
						{
							XmlUtils::CXmlNodes oColumnNodes;
							if (oXmlNode.GetNodes(_T("fo:table-column"), oColumnNodes))
							{
								m_iColumnCount	= oColumnNodes.GetCount();
								if (m_iColumnCount == 0)
								{
									return false;
								}

								for (INT nIndex = 0; nIndex < m_iColumnCount; ++nIndex)
								{
									XmlUtils::CXmlNode oNode;
									oColumnNodes.GetAt(nIndex, oNode);

									REAL fColumnWidth	= FloatPtAttributeFromXml(oNode, _T("column-width"), -1.0f);
									m_fSummaryWidth	+= fColumnWidth;

									m_arrColumnWidth.Add(fColumnWidth);
								}

								m_fWidth	= m_fSummaryWidth;

								return true;
							}

							return false;
						}
						INT		CalculateMaxCols(XmlUtils::CXmlNode &oXmlNode)
						{
							XmlUtils::CXmlNode oBodyNode;
							if (oXmlNode.GetNode(_T("fo:table-body"), oBodyNode))
							{
								XmlUtils::CXmlNodes oRowNodes;
								if (oBodyNode.GetNodes(_T("fo:table-row"), oRowNodes))
								{
									INT iMaxColsCount	= 0;

									INT iRowCount	= oRowNodes.GetCount();
									for (INT iRowIndex = 0; iRowIndex < iRowCount; ++iRowIndex)
									{
										INT iColsCount	= 0;

										XmlUtils::CXmlNode oRowNode;
										oRowNodes.GetAt(iRowIndex, oRowNode);
										XmlUtils::CXmlNodes oCellNodes;
										if (oRowNode.GetNodes(_T("fo:table-cell"), oCellNodes))
										{
											INT iCellCount	= oCellNodes.GetCount();
											for (INT iCellIndex = 0; iCellIndex < iCellCount; ++iCellIndex)
											{
												XmlUtils::CXmlNode oCellNode;
												oCellNodes.GetAt(iCellIndex, oCellNode);

												iColsCount	+= oCellNode.ReadAttributeOrValueInt(_T("number-columns-spanned"), 1);
											}
										}

										if (iColsCount > iMaxColsCount)
										{
											iMaxColsCount	= iColsCount;
										}
									}

									return iMaxColsCount;
								}
							}

							return 0;
						}
						void	LoadNewRow(XmlUtils::CXmlNode &oXmlNode, CDocumentManager& documentManager)
						{
							++m_iCurrentRow;
							INT iCurrentCell	= m_iColumnCount * (m_iCurrentRow - 1);
							int iFreeCellsCount	= m_iColumnCount;

							XmlUtils::CXmlNodes oCellNodes;
							if (oXmlNode.GetNodes(_T("fo:table-cell"), oCellNodes))
							{
								INT iCellCount	= oCellNodes.GetCount();
								iFreeCellsCount	-= iCellCount;
								for (INT iIndex = 0; iIndex < iCellCount; ++iIndex)
								{
									XmlUtils::CXmlNode oCellNode;
									oCellNodes.GetAt(iIndex, oCellNode);

									CCell *pNewCell	= new CCell();
									pNewCell->m_oBorderProperties	= _blockAttributes;
									if (pNewCell->FromXmlNode(oCellNode, documentManager))
									{
										for (; iCurrentCell < m_iColumnCount * m_iCurrentRow; ++iCurrentCell)
										{
											if (m_arrCells[iCurrentCell] == NULL)
												break;
										}
									}
									else
									{
										if (iCurrentCell >= m_iColumnCount * m_iCurrentRow)
										{
											RELEASEOBJECT(pNewCell);
											return;
										}
										if ((m_arrCells[iCurrentCell] == NULL) && (m_iCurrentRow != 1))
										{
											m_arrCells[iCurrentCell]	= m_arrCells[iCurrentCell - m_iColumnCount];
										}
										++iCurrentCell;
										RELEASEOBJECT(pNewCell);
										continue;
									}

									if (iCurrentCell == m_iColumnCount * m_iCurrentRow)
										return;

									if (pNewCell->m_iNumberColumnsSpanned > iFreeCellsCount + 1)
									{
										pNewCell->m_iNumberColumnsSpanned	= iFreeCellsCount + 1;
										iFreeCellsCount	= 0;
									}

									INT iCellIndex	= iCurrentCell;
									for (INT iSpanRowIndex = 0; iSpanRowIndex < pNewCell->m_iNumberRowsSpanned; ++iSpanRowIndex)
									{
										for (INT iSpanColIndex = 0; iSpanColIndex < pNewCell->m_iNumberColumnsSpanned; ++iSpanColIndex)
										{
											m_arrCells[iCellIndex]	= pNewCell;
											iCellIndex++;
										}

										iCellIndex += m_iColumnCount - pNewCell->m_iNumberColumnsSpanned;
									}

									iCurrentCell += pNewCell->m_iNumberColumnsSpanned;
								}
							}
						}
					private:
						CRegion makeRegionWithSettings(const CRegion& region) {
							float widthDelta = region.GetRectPt().Width - m_fWidth;
							float offsetX = calcAlignOffset(widthDelta);
							
							CBounds bounds;
							bounds.SetAsMarginPt(_blockAttributes.m_fStartIndent + offsetX
								, _blockAttributes.m_nSpaceBefore, _blockAttributes.m_fEndIndent, 0.0f);
							
							CRegion regionWithSettings = region;
							regionWithSettings.ChangeRect(bounds);

							return regionWithSettings;
						}

						float calcAlignOffset(const float& widthDelta) {
							switch (_blockAttributes.m_nTextAlignment) {
							case 1:
								return widthDelta / 2;
							case 2:
								return widthDelta;
							}

							return 0;
						}
					private:
						void calcColsWidths(CAtlArray<float>& colsWidths, const float& widthMax
								, CGrapher& grapher) {
							int& colsCount = m_iColumnCount;
							int& rowsCount = m_iRowCount;
							
							CAtlArray<float> colsScores;
							colsScores.InsertAt(0, 1, colsCount);

							CAtlArray<float> colsWidthsMin;
							colsWidthsMin.InsertAt(0, 0, colsCount);

							CAtlArray<float> rowScores;
							rowScores.InsertAt(0, 0, colsCount);

							CAtlArray<float> rowWidthsMin;
							rowWidthsMin.InsertAt(0, 0, colsCount);
							
							for (int row = 0; row < rowsCount; ++row) {
								calcRowWidths(rowWidthsMin, rowScores, row, widthMax,grapher);

								for (int col = 0; col < colsCount; ++col) {
									int colsSpanned = 1;
									while (isSpanned(row, col + colsSpanned, 1))
										++colsSpanned;

									float spanColsWidthMin = rowWidthsMin[col] / colsSpanned;
									float spanColsScore = rowScores[col] / colsSpanned;
									for (int i = col; i < col + colsSpanned; ++i) {
										if (colsWidthsMin[i] < spanColsWidthMin)
											colsWidthsMin[i] = spanColsWidthMin;
										colsScores[i] += spanColsScore;
									}

									col += colsSpanned - 1;
								}
							}

							float width = 0;
							float score = 0;
							int colsAutoCount = 0;
							for (int i = 0; i < colsCount; ++i)
								if (m_arrColumnWidth[i] > 0) {
									width += m_arrColumnWidth[i];
								}
								else {
									width += colsWidthsMin[i];
								score += colsScores[i];
								++colsAutoCount;
								}

							colsWidths.SetCount(colsCount);

							float widthDelta = widthMax - width;
							for (int i = 0; i < colsCount; ++i)
								if (m_arrColumnWidth[i] > 0)
									colsWidths[i] = m_arrColumnWidth[i];
								else {
									colsWidths[i] = colsWidthsMin[i];
								
									float coeff = colsScores[i] / score;
									colsWidths[i] += coeff * widthDelta;
								}
						}

						void calcRowWidths(CAtlArray<float>& colsWidths, CAtlArray<float>& colsScores
								, const int& row, const float& rowWidthMax, CGrapher& grapher) {
							int& colsCount = m_iColumnCount;
							float rowWidth = 0;
							int colsAutoCount = 0;

							for (int i = 0; i < colsCount; ++i) {
								float colWidth = m_arrColumnWidth[i];
								if (colWidth > 0) {
									colsWidths[i] = colWidth;
									rowWidth += colWidth;
								}
								else {
									colsWidths[i] = -1;
									++colsAutoCount;
								}

								colsScores[i] = 0;
							}

							for (int i = 0; i < colsCount; ++i) {
								if (colsWidths[i] != -1)
									continue;
								
								CCell* cell	= m_arrCells[row * colsCount + i];
								if (!cell || isSpanned(row, i)) {
									colsWidths[i] = 0;
									colsScores[i] = 0;
									continue;
								}

								float cellWidth = cell->m_fWidth;
								if (cellWidth > 0) {
									colsWidths[i] = cellWidth;
									rowWidth += cellWidth;

									--colsAutoCount;
								}
								else {
									float widthMin = 0;
									float widthScore = 0;
									cell->estimateWidth(&grapher, widthMin, widthScore);

									colsScores[i] = widthScore;
									
									cellWidth = widthMin;
									if (cellWidth >= 0) {
										colsWidths[i] = -cellWidth;
										rowWidth += cellWidth;
									}
								}
							}
							
							if (colsAutoCount > 0) {
								float colWidthReduction	= (rowWidth - rowWidthMax) / colsAutoCount;
								for (int i = 0; i < colsCount; ++i) {
									if (colsWidths[i] < 0)
										colsWidths[i] = -colsWidths[i];
									if (colWidthReduction > 0)
										colsWidths[i] -= colWidthReduction;
								}
							}
						}

					private:
						bool isSpanned(const int& row, const int& col, const char& type = 0)
						{
							if (row >= m_iRowCount || col >= m_iColumnCount)
								return false;

							bool spanned = false;

							CCell* cell = m_arrCells[row * m_iColumnCount + col];
							if (!cell)
								return false;

							if (row > 0 && type != 1)
								if (cell == m_arrCells[(row - 1) * m_iColumnCount + col])
									spanned = true;

							if (col > 0 && type != 2)
								if (cell == m_arrCells[row * m_iColumnCount + col - 1])
									spanned = true;

							return spanned;
						}
						void normalizeWidth()
						{
							if (m_fWidth <= 0)
								return;

							if (m_fSummaryWidth > m_fWidth)
							{
								REAL fDeltaPct	= m_fWidth / m_fSummaryWidth;

								for (INT iIndex = 0; iIndex < m_iColumnCount; ++iIndex)
								{
									m_arrColumnWidth[iIndex] *= fDeltaPct;
								}

								m_fSummaryWidth	= m_fWidth;
							}
						}
					private:
						void initCommands() {
							if (!_tableStartCommand.isEmpty())
								_tableStartCommand->setAdded(false);
							if (!_tableEndCommand.isEmpty())
								_tableEndCommand->setAdded(false);
						}
						void setCommands(const CString& xml) {
							_tableStartCommand = SharedPtr<RendererCommand>(new TableStartRendererCommand(xml));
							_tableEndCommand = SharedPtr<RendererCommand>(new TableEndRendererCommand());
						}
					};
					// ### ### ###
					class CFlow
					{
					public:
						enum EPosPageType {
							OddPageType = 0, EvenPageType, FirstPageType							
						};
					public:
						CFlow() 
							: m_strName(_T("")), m_Static(false), m_Complete(false), 
							m_arrBlocks(), m_nCurrentBlock(0), 
							m_eTextAlignVertical(FarAlignVert)
						{
						}
						~CFlow()
						{
							Clear();
						}
						void	Clear()
						{
							m_strName = _T("");
							m_Static = false;
							m_Complete = false;

							for (size_t nIndex = 0; nIndex < m_arrBlocks.GetCount(); ++nIndex)
								RELEASEOBJECT(m_arrBlocks[nIndex]);
							m_arrBlocks.RemoveAll();
							m_nCurrentBlock = 0;

							m_eTextAlignVertical = FarAlignVert;
						}
					public:
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode, CDocumentManager& documentManager, const bool& inTable = false)
						{
							m_strName = oXmlNode.GetAttributeOrValue(_T("flow-name"), _T(""));
							m_PageType = ReadPosPageType(oXmlNode);

							XmlUtils::CXmlNodes oNodes;
							if (oXmlNode.GetNodes(_T("*"), oNodes))
							{
								INT iListLenght	= oNodes.GetCount();
								bool bIsFirst	= true;
								bool inCommandField = false;
								CAtlArray<CString>	curCommands;

								for (INT iIndexNode = 0; iIndexNode < iListLenght; ++iIndexNode)
								{
									XmlUtils::CXmlNode oBlockNode;
									oNodes.GetAt(iIndexNode, oBlockNode);

									CString strBlockName = oBlockNode.GetName();									
									if (strBlockName == _T("fo:block"))
									{
										CBlock *pBlock	= new CBlock;
										pBlock->InTable(inTable);
										pBlock->FromXmlNode(oBlockNode, documentManager, bIsFirst, inCommandField, curCommands);
										m_arrBlocks.Add(pBlock);

										bIsFirst = false;
									}
									else if (strBlockName == _T("fo:table"))
									{
										CTable *pTable	= new CTable();
										pTable->InTable(inTable);
										if (pTable->FromXmlNode(oBlockNode, documentManager))
											m_arrBlocks.Add(pTable);
										else
											RELEASEOBJECT(pTable);
									}
								}
							}
						}
					public:
						inline bool	Empty()
						{
							return (m_arrBlocks.GetCount() == 0);
						}
						inline void	Complete(const bool& complete)
						{
							m_Complete = complete;
						}
						inline bool	Complete() const
						{
							return m_Complete;
						}
						inline void	Static(const bool& staticFlow)
						{
							m_Static = staticFlow;
						}
						inline bool	Static() const
						{
							return m_Static;
						}
						inline const CString&	Name() const
						{
							return m_strName;
						}
						inline const EPosPageType	PageType() const
						{
							return m_PageType;
						}
					public:
						void estimateWidth(CGrapher* grapher, float& widthMin, float& widthScore) const
						{
							widthMin = 0;
							widthScore = 0;
							
							int blocksCount = (int) m_arrBlocks.GetCount();
							for (int i = 0; i < blocksCount; ++i) {
								float blockWidthMin = 0;
								float blockWidthScore = 0;
								m_arrBlocks[i]->estimateWidth(grapher, blockWidthMin, blockWidthScore);
								
								if (blockWidthMin > widthMin)
									widthMin = blockWidthMin;
								widthScore += blockWidthScore;
							}
						}
						inline size_t	GetCurrentBlock() const
						{
							return m_nCurrentBlock;
						}
						inline void		SetCurrentBlock(size_t currentBlock, CGrapher* grapher)
						{
							for (size_t nIndex = currentBlock; nIndex < m_arrBlocks.GetCount(); ++nIndex)
								m_arrBlocks[nIndex]->Renew(grapher, false);
							m_nCurrentBlock = currentBlock;
						}
					public:
						bool	FillRegion(CRegion &oRegion, CPage &oPage, CGrapher* pGrapher, const bool& inColumn = false)
						{
							oPage.StartPrintingFlow();

							if (Complete() && !Static())
								return false;
							if (oRegion.IsComplete())
								return false;

							if (Static() && !inColumn)
								m_nCurrentBlock = 0;

							pGrapher->m_DocumentManager->SetBorders(oRegion);

							Complete(true);
							bool somePrinted = false;
							CBlockInputParameters blockParams;
							for (size_t nIndex = m_nCurrentBlock; nIndex < m_arrBlocks.GetCount(); ++nIndex)
							{
								CCommonBlock *pBlock = m_arrBlocks[nIndex];

								if (pBlock->UsualBlock())
								{
									CCommonBlock *pPrevBlock;
									bool mergeWithPrev = false;
									bool removeSpaceBefore = false;
									if (nIndex != 0)
									{
										pPrevBlock = m_arrBlocks[nIndex - 1];
										if (pPrevBlock->UsualBlock())
										{
											mergeWithPrev = ((CBlock*)pPrevBlock)->m_mergeBordersWithNextBlock;
											removeSpaceBefore = ((CBlock*)pPrevBlock)->m_removeSpaceAfter;
										}
									}
									((CBlock*)pBlock)->m_mergeBordersWithPrevBlock = mergeWithPrev;
									((CBlock*)pBlock)->m_removeSpaceBefore		   = removeSpaceBefore;

									CCommonBlock *pNextBlock;
									bool mergeWithNext = false;
									bool removeSpaceAfter = false;
									if (nIndex != m_arrBlocks.GetCount() - 1)
									{
										pNextBlock = m_arrBlocks[nIndex + 1];
										if (pNextBlock->UsualBlock())
										{
											mergeWithNext	 = EqualsAreas(pBlock, pNextBlock);
											removeSpaceAfter = EqualsStyles(pBlock, pNextBlock);
										}
									}
									((CBlock*)pBlock)->m_mergeBordersWithNextBlock = mergeWithNext;
									((CBlock*)pBlock)->m_removeSpaceAfter		   = removeSpaceAfter;
								}

								bool blockSomePrinted = false;
								pBlock->InColumn(inColumn);
								pBlock->InputParams(blockParams, *pGrapher->m_DocumentManager);
								if (pBlock->FillRegion(oRegion, oPage, m_Static, pGrapher))
								{
									blockSomePrinted = true;
									somePrinted = true;
								}
								pBlock->InputParams(blockParams, *pGrapher->m_DocumentManager, true);
								if (pBlock->Complete())
									pGrapher->m_DocumentManager->BlockProcessed();
								else
									Complete(false);

								if (oRegion.IsComplete()) {
									if (nIndex != m_nCurrentBlock) {
										if (!blockSomePrinted && (m_arrBlocks[nIndex - 1]->KeepNext())
											&& oRegion.BreakType() == 0)
											if (m_arrBlocks[nIndex - 1]->transferToNextPage(pGrapher)) {
												oPage.DeleteBlocks(2);
												--nIndex;
											}
									}

									m_nCurrentBlock	= nIndex;
									if (pBlock->Complete())
										++m_nCurrentBlock;
									Complete(false);
									break;
								}

								if (nIndex == m_arrBlocks.GetCount() - 1)
									m_nCurrentBlock = nIndex;
							}

							AlignInRegion(oRegion, oPage, m_eTextAlignVertical);

							return somePrinted;
						}
						void	AlignInRegion(CRegion &oRegion, CPage &oPage, 
							CAlignVertical eTextAlignVertical)
						{
							REAL fOffsetPt = oRegion.GetRegionHeightPt();
							switch(eTextAlignVertical)
							{
							case CenterAlignVert :
								fOffsetPt /= 2;
								break;
							case FarAlignVert :
								fOffsetPt = 0;
								break;
							}

							oPage.MoveFlowOn(0, fOffsetPt);
						}
					public:
						bool	EqualsAreas(CCommonBlock* block1, CCommonBlock* block2)
						{
							CBlockAttributes* blockAttr1 = ((CBlock*)block1)->m_pBlockAttributes;
							CBlockAttributes* blockAttr2 = ((CBlock*)block2)->m_pBlockAttributes;

							if (blockAttr1->m_fStartIndent == ((CBlock*)block2)->m_pBlockAttributes->m_fStartIndent
								&& blockAttr1->m_fEndIndent == blockAttr2->m_fEndIndent
								&& ((blockAttr1->m_oBrush.m_oBrush.Alpha1 != 0 && blockAttr2->m_oBrush.m_oBrush.Alpha1 != 0)
								||(blockAttr1->m_oBrush.m_oBrush.Alpha1 == 0 && blockAttr2->m_oBrush.m_oBrush.Alpha1 == 0))
								&& blockAttr2->GetBorderType() == blockAttr1->GetBorderType())
								return true;
							return false;
						}

						bool	EqualsStyles(CCommonBlock* block1, CCommonBlock* block2)
						{
							if (((CBlock*)block1)->m_pBlockAttributes->m_pStyle == ((CBlock*)block2)->m_pBlockAttributes->m_pStyle)
								return true;
							return false;
						}
					public:
						CAtlArray<CCommonBlock*>&	GetBlocks()
						{
							return m_arrBlocks;
						}
						void	SetAlignVertical(const CAlignVertical& alingVertical)
						{
							m_eTextAlignVertical = alingVertical;
						}

					private:
						CString	m_strName;
						bool	m_Static;
						bool	m_Complete;
						EPosPageType	m_PageType;

						CAtlArray<CCommonBlock*>	m_arrBlocks;
						size_t						m_nCurrentBlock;

						CAlignVertical	m_eTextAlignVertical;

					private:
						EPosPageType	ReadPosPageType(XmlUtils::CXmlNode& flow)
						{
							CString type = flow.GetAttribute(_T("type"), _T("odd"));
							EPosPageType pageType = OddPageType;
							if (type == _T("odd"))
								pageType = OddPageType;
							else if (type == _T("even"))
								pageType = EvenPageType;
							else if (type == _T("first"))
								pageType = FirstPageType;
							return pageType;
						}
					};

					class CFootnote : public CFlow, public CItem
					{
					private:
						INT	m_iInlinePosition;

					public:
						CFootnote()
							: CFlow(), CItem(), 
							m_iInlinePosition(-1)
						{
						}
						~CFootnote()
						{
							Clear();
						}
					public:
						void	Clear()
						{
							m_iInlinePosition	= -1;

							CItem::Clear();
							CFlow::Clear();
						}
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode, CDocumentManager& documentManager)
						{
							CFlow::FromXmlNode(oXmlNode, documentManager);
						}
						CItemType	GetType()
						{
							return Footnote;
						}
						INT			GetInilinePosition()
						{
							return m_iInlinePosition;
						}
						void		SetInlinePosition(const INT& iInlinePosition)
						{
							m_iInlinePosition	= iInlinePosition;
						}
						void		SetOnBaseline(const bool& bIsInit = true)
						{
							m_oRect	= RectF(0.0f, 0.0f, 0.0f, 0.0f);
						}
					public:
						bool	FillRegion(CRegion& oRegion, CPage &oPage, CGrapher* pGrapher)
						{
							return CFlow::FillRegion(oRegion, oPage, pGrapher);
						}
						void	Draw(IAVSRenderer *pRenderer, CDocumentManager& documentManager, const float &fCompleteness)
						{
						}
					};

					class CPageSequence {
					private:
						CLayoutMasterSet*		m_pLayoutMasterSet;
						CFlowMap*				m_pFlowMap;
						CPageSequenceMaster*	m_pPageSequenceMaster;

						CPageSequence* _prev;

						CString	m_strMasterReference;

						CAtlArray<CFlow>	m_arrFlows;
						CAtlArray<CFlow>	m_arrStaticContent;

						bool	m_Complete;
						bool	m_reWriteColumns;

					public:
						CPageSequence(CLayoutMasterSet *pLayoutMasterSet = NULL) 
							: m_pLayoutMasterSet(pLayoutMasterSet),
							_prev(NULL),
							m_pFlowMap(NULL), 
							m_pPageSequenceMaster(NULL), 
							m_strMasterReference(_T("")),  
							m_arrFlows(), m_arrStaticContent(), 
							m_Complete(true),
							m_reWriteColumns(true) {
						}
						~CPageSequence()
						  {
							  Clear();
						  }
						void	Clear()
						  {
							  m_pLayoutMasterSet = NULL;
							  m_pFlowMap = NULL;
							  m_pPageSequenceMaster = NULL;

							  m_strMasterReference = _T("");

							  m_arrFlows.RemoveAll();
							  m_arrStaticContent.RemoveAll();

							  m_Complete = true;
							  m_reWriteColumns = true;

							  _prev = NULL;
						  }
					public:
						void	loadFromXml(XmlUtils::CXmlNode& pageSequenceXml, CDocumentManager& documentManager)
						{
							CString strFlowMapName	= pageSequenceXml.GetAttributeOrValue(_T("flow-map-reference"), _T(""));
							m_pFlowMap				= m_pLayoutMasterSet->FindFlowMap(strFlowMapName);

							m_strMasterReference	= pageSequenceXml.GetAttributeOrValue(_T("master-reference"), _T(""));
							m_pPageSequenceMaster	= m_pLayoutMasterSet->FindPageSequenceMaster(m_strMasterReference);

							documentManager.SetStaticProcessing(true);
							XmlUtils::CXmlNodes oStaticContentNodes;
							if (pageSequenceXml.GetNodes(_T("fo:static-content"), oStaticContentNodes))
							{
								INT iCount	= oStaticContentNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oStaticContentNodes.GetAt(iIndex, oNode);
									m_arrStaticContent.Add();
									m_arrStaticContent[iIndex].FromXmlNode(oNode, documentManager);
									m_arrStaticContent[iIndex].Static(true);
								}
							}
							documentManager.SetStaticProcessing(false);

							XmlUtils::CXmlNodes oFlowNodes;
							if (pageSequenceXml.GetNodes(_T("fo:flow"), oFlowNodes))
							{
								INT iCount	= oFlowNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oFlowNodes.GetAt(iIndex, oNode);
									m_arrFlows.Add();
									m_arrFlows[iIndex].FromXmlNode(oNode, documentManager);
								}
							}
						}
					public:
						void	SetLayoutMasterSet(CLayoutMasterSet* layoutMasterSet)
						{
							m_pLayoutMasterSet = layoutMasterSet;
						}
						bool	Continuous()
						{
							CSimplePageMaster* simplePageMaster = m_pLayoutMasterSet->FindSimplePageMaster(GetPageMasterName());
							assert(simplePageMaster != NULL);
							return simplePageMaster->Continuous();
						}
						void	Complete(const bool& complete)
						{
							m_Complete = complete;
						}
						bool	Complete()
						{
							return m_Complete;
						}
					public:
						CPage*	NewPage(CGrapher* grapher, const CPage* prevPage)
						{
							CSimplePageMaster* pageMaster = m_pLayoutMasterSet->FindSimplePageMaster(GetPageMasterName());

							CPage *page = CPage::createNew(prevPage);
							page->SetPageMaster(pageMaster);

							CItemManager* itemManager = &grapher->m_DocumentManager->m_ItemManager;
							itemManager->SetWidthL(page->GetWidthL());
							itemManager->SetHeightL(page->GetHeightL());
							itemManager->SetMultiplierX(page->GetWidthPt());
							itemManager->SetMultiplierY(page->GetHeightPt());

							if (Complete())
								grapher->m_DocumentManager->setPageNumbering(pageMaster->getPageNumbering());

							grapher->m_DocumentManager->nextPage();

							return page;
						}
						void	ContinuePage(CPage& page)
						{
							page.SetPageMaster(m_pLayoutMasterSet->FindSimplePageMaster(GetPageMasterName()));
							page.Continuous(true);
						}
						bool	FillPage(CPage& page, CGrapher* grapher) {
							grapher->m_DocumentManager->SetPage(page);

							if (!page.Continuous()) {
								#ifndef TME_DONT_OUTPUT_HEADER_FOOTER
								addHeaderCommandToPage(page);

								FillHeader(page, grapher);
								FillFooter(page, grapher);

								addHeaderCommandToPage(page, false);
								#endif
								TruncateBody(page);
							}
							else {
								float filledHeight = page.FilledHeight();
								TruncateBodyTop(page, filledHeight);
								TruncateBodyBottom(page, page.getFootnoteHeight());
								float footerHeight = page.FooterHeight();
							}

							FillBody(page, grapher);
							CalculateFilledHeight(page);

							return Complete();
						}
						void	ReWriteColumns(bool flag)
						{
							m_reWriteColumns = flag;
						}
					public:
						CFlow*	GetFirstFlow()
						{
							return &m_arrFlows[0];
						}

					private:
						void	FillBody(CPage& page, CGrapher* grapher)
						{
							Complete(true);
							page.StartBody();
							for (int index = 0; index < (int)m_arrFlows.GetCount(); ++index) {
								CFlow& flow = m_arrFlows[index];
								PrintFlow(page, &flow, grapher);
							}
							page.FinishBody();
						}
						void	FillHeader(CPage& page, CGrapher* grapher) {
							int pageIndex = grapher->m_DocumentManager->getPageIndex();
							FillStatic(page, grapher, _T("xsl-region-before"), PageType(pageIndex));
						}
						void	FillFooter(CPage& page, CGrapher* grapher) {
							int pageIndex = grapher->m_DocumentManager->getPageIndex();
							FillStatic(page, grapher, _T("xsl-region-after"), PageType(pageIndex));
						}
						void	FillStatic(CPage& page, CGrapher* grapher, const CString& name, const CFlow::EPosPageType& pageType)
						{
							grapher->m_DocumentManager->SetStaticProcessing(true);
							bool found = false;
							for (int index = 0; index < (int) m_arrStaticContent.GetCount(); ++index)
							{
								CFlow& flow = m_arrStaticContent[index];
								if ((flow.Name() == name) && (flow.PageType() == pageType))
								{
									PrintFlow(page, &flow, grapher);
									found = true;
									break;
								}
							}
							if (!found) {
								if (!FillStatic(page, grapher, name) && _prev != NULL)
									_prev->FillStatic(page, grapher, name, pageType);

							}
							grapher->m_DocumentManager->SetStaticProcessing(false);
						}

						bool FillStatic(CPage& page, CGrapher* grapher, const CString& name)
						{
							bool found = false;

							grapher->m_DocumentManager->SetStaticProcessing(true);
							for (int index = 0; index < (int) m_arrStaticContent.GetCount(); ++index)
							{
								CFlow& flow = m_arrStaticContent[index];
								if ((flow.Name() == name) && (flow.PageType() == CFlow::OddPageType)) {
									found = true;
									PrintFlow(page, &flow, grapher);
									break;
								}
							}
							grapher->m_DocumentManager->SetStaticProcessing(false);

							return found;
						}
					private:
						void	PrintFlow(CPage& oPage, CFlow* pFlow, CGrapher* pGrapher)
						{
							CAtlArray<CString> arrRegionNames;
							GetRegionNames(pFlow->Name(), arrRegionNames);

							for (size_t nRegionIndex = 0; nRegionIndex < arrRegionNames.GetCount(); ++nRegionIndex)
							{
								CString name = arrRegionNames[nRegionIndex];
								if (name == _T("xsl-region-after"))
									pFlow->SetAlignVertical(NearAlignVert);
								size_t iLength;
								double dSizeBlocksInColumns	 = 0;
								double dWidthBlocksInColumns = 0;
								size_t sBlockInBeginColumns = 0;
								size_t sLineInBeginColumns = 0;
								size_t sFirstColumn;
								int footLineInBeginColumns = 0;
								int graphLineInBeginColumns = 0;
								int backgroundInBeginColumns = 0;
								bool completeRegions = false;
								CRegion **ppRegion	= oPage.GetRegions(arrRegionNames[nRegionIndex], iLength);
								if (ppRegion != NULL) {	
									addColumnCommandToPage(oPage, (int) iLength);

									size_t i = 0;
									for (size_t i = 0; i < iLength;i++)
									{
										CRegion *pRegion = ppRegion[i];
										if (pRegion != NULL)
										{
											if (!pRegion->IsComplete())
											{
												if (pRegion->IsColumn() && (i == 0 || !ppRegion[i -1]->IsColumn()))
												{
													sBlockInBeginColumns = pFlow->GetCurrentBlock();
													sLineInBeginColumns = oPage.GetLastLine();
													graphLineInBeginColumns = oPage.GetLastGraphLine();
													backgroundInBeginColumns = oPage.GetLastBackground();
													sFirstColumn = i;
												}

												if (!pRegion->IsColumn())
												{
													pFlow->FillRegion(*pRegion, oPage, pGrapher);
													if (pRegion->BreakType() == 1)
														completeRegions = true;
												}
												else
												{
													if (completeRegions)
														continue;
													if (i == 0)
														pFlow->FillRegion(*pRegion, oPage, pGrapher, true);
													else
														pFlow->FillRegion(*pRegion, oPage, pGrapher);
													if (pRegion->BreakType() == 1)
														completeRegions = true;
													double dWidth = pRegion->GetFilledWidth();
													float delta = 0;
													float filledHeight = oPage.FilledHeight();
													float top = pRegion->GetOriginalRectPt().GetTop();
													if (filledHeight > top)
														delta = filledHeight - top;
													dSizeBlocksInColumns  += (pRegion->GetFilledHeight() - delta) * dWidth;
													dWidthBlocksInColumns += dWidth;
													if (pFlow->Complete() && m_reWriteColumns)
													{
														size_t sLastColumn;
														double dHeightRegion = pRegion->GetOriginalRectPt().GetBottom() - top - delta;
														dWidthBlocksInColumns += GetWidthNextColumns(ppRegion, iLength, i, sLastColumn);
														double dNewHeight = dSizeBlocksInColumns / dWidthBlocksInColumns;
														bool flowIsEmpty = false;
														int iStep = 5;														

														do
														{
															pFlow->Complete(false);
															oPage.ClearDerivedLines((int) sLineInBeginColumns);
															oPage.ClearDerivedGraphLines(graphLineInBeginColumns);
															oPage.ClearDerivedBackgrounds(backgroundInBeginColumns);
															pFlow->SetCurrentBlock(sBlockInBeginColumns, pGrapher);
															ReWriteColumns(ppRegion, sFirstColumn, sLastColumn, dNewHeight, pFlow, oPage, pGrapher);
															flowIsEmpty = pFlow->Complete();
															if (flowIsEmpty == false)
																dNewHeight += iStep;
															else
																break;
														}
														while (dNewHeight <= dHeightRegion);
														i = sLastColumn;
													}
												}
											}
										}
										else
										{
											break;
										}
									}
									if (!pFlow->Complete())
										Complete(false);
								}
								RELEASEARRAYOBJECTS(ppRegion);
							}
						}
						double  GetWidthNextColumns(CRegion **ppRegion, size_t iLength, size_t position, size_t& sLastColumn)
						{
							double dWidth = 0;
							size_t i;
							for(i = position  + 1; i < iLength; i++)
							{
								CRegion *pRegion = ppRegion[i];
								if (pRegion != NULL && pRegion->IsColumn())
								{
									dWidth += pRegion->GetFilledWidth();
								}
								else 
								{
									sLastColumn = i - 1;
									break;
								}
							}
							if (i == iLength )
								sLastColumn = iLength - 1;
							return dWidth;
						}
						void	ReWriteColumns(CRegion **ppRegion, size_t sFirstColumn, size_t sLastColumn, double dNewHeight, CFlow* pFlow, CPage& oPage, CGrapher* pGrapher, const float& prevSectionBottom = 0.0f)
						{
							bool completeRegions = false;
							for (size_t i = sFirstColumn; i <= sLastColumn; i++)
							{
								CRegion *pRegion = ppRegion[i];
								if (completeRegions)
									continue;
								if (pRegion != NULL)
								{
									pRegion->SetComplete();
									pRegion->CheckRect();
									float filledHeight = oPage.FilledHeight();
									float deltaY = filledHeight - pRegion->GetRectPt().GetTop();
									if (deltaY > 0)
										pRegion->TrancateRectPt(deltaY);
									pRegion->TruncateHeightPt(pRegion->GetRectPt().GetBottom() - pRegion->GetRectPt().GetTop() - dNewHeight);

									pFlow->FillRegion(*pRegion, oPage, pGrapher);
									if (pRegion->BreakType() == 1)
										completeRegions = true;
								}
							}
						}						
					private:
						void	TruncateBody(CPage& page)
						{
							CRegion* regionBefore = page.GetRegion(_T("xsl-region-before"));
							if ((regionBefore != NULL) && !regionBefore->IsReady())
							{
								float headerBottom = regionBefore->GetRectPt().GetTop();
								TruncateBodyTop(page, headerBottom);
							}
							CRegion* regionAfter = page.GetRegion(_T("xsl-region-after"));
							if ((regionAfter != NULL) && !regionAfter->IsReady())
							{
								float footerHeightEmpty = regionAfter->GetRectPt().GetBottom() - regionAfter->GetRectPt().GetTop();
								float footerTop = regionAfter->GetOriginalRectPt().GetTop() + footerHeightEmpty;
								TruncateBodyBottom(page, footerTop, true);
							}
						}
						void	TruncateBodyTop(CPage& page, const float& y)
						{
							size_t count = 0;
							CRegion** bodyRegions = page.GetRegions(_T("xsl-region-body"), count);
							for (int i = 0; i < (int)count; ++i)
								if (bodyRegions[i])
								{
									float bodyTop = bodyRegions[i]->GetRectPt().GetTop();
									float deltaY = y - bodyTop;
									if (deltaY > 0)
										bodyRegions[i]->TrancateRectPt(deltaY);
								}
								delete[] bodyRegions;
						}
						void	TruncateBodyBottom(CPage& page, const float& y, bool bSaveFooterPos = false)
						{
							size_t count = 0;
							CRegion** bodyRegions = page.GetRegions(_T("xsl-region-body"), count);
							for (int i = 0; i < (int) count; ++i)
								if (bodyRegions[i])
								{
									float bodyBottom = bodyRegions[i]->GetRectPt().GetBottom();
									float deltaY = bodyBottom - y;
									if (deltaY > 0)
									{
										bodyRegions[i]->TruncateHeightPt(deltaY);
										if (bSaveFooterPos == true)
											page.FooterHeight(y);
									}
								}
							delete[] bodyRegions;
						}
						void	CalculateFilledHeight(CPage& page)
						{
							float filledHeight = page.FilledHeight();
							size_t count = 0;
							CRegion** bodyRegions = page.GetRegions(_T("xsl-region-body"), count);
							for (int i = 0; i < (int) count; ++i)
								if (bodyRegions[i])
								{
									float bodyTop = bodyRegions[i]->GetRectPt().GetTop();
									if (bodyTop > filledHeight)
										filledHeight = bodyTop;
								}
								delete[] bodyRegions;
								page.FilledHeight(filledHeight);
						}
					private:
						void	GetRegionNames(const CString &strFlowName, CAtlArray<CString> &arrRegionNames)
						{
							if (m_pFlowMap != NULL)
							{
								m_pFlowMap->GetRegionNames(strFlowName, arrRegionNames);
							}
							if (arrRegionNames.GetCount() == 0)
							{
								arrRegionNames.Add(strFlowName);
							}
						}
						CString	GetPageMasterName()
						{
							if (m_pPageSequenceMaster != NULL)
							{
								return m_pPageSequenceMaster->GetPageMasterName();
							}
							else
							{
								return m_strMasterReference;
							}
						}
						CFlow::EPosPageType	PageType(const int& pageIndex) const
						{
							if (pageIndex % 2 == 0)
								return CFlow::EvenPageType;
							else if (pageIndex == 1)
								return CFlow::FirstPageType;
							else
								return CFlow::OddPageType;
						}

					public:
						void setPrev(CPageSequence* prev) {
							_prev = prev;
						}
					private:
						void addHeaderCommandToPage(CPage& page, const bool& beginCommand = true) {
							RCommand command;
							if (beginCommand)
								command = SharedPtr<RendererCommand>(new HeaderStartRendererCommand());
							else
								command = SharedPtr<RendererCommand>(new HeaderEndRendererCommand());
							page.addRendererCommand(command, !beginCommand);
						}
						void addColumnCommandToPage(CPage& page, const int columnCount) {
							page.addRendererCommand(
									SharedPtr<RendererCommand>(new ColumnCountRendererCommand(columnCount)));
						}
					};

					class CPageMasterAlternatives
					{
					public:
						CString	m_strOddPageMasterReference;
						CString	m_strEvenPageMasterReference;

					public:
						// ---
						CPageMasterAlternatives() 
							: m_strOddPageMasterReference(_T("")), 
							m_strEvenPageMasterReference(_T(""))
						{
						}
						CPageMasterAlternatives(const CPageMasterAlternatives &oSrc)
						{
							m_strOddPageMasterReference		= oSrc.m_strOddPageMasterReference;
							m_strEvenPageMasterReference	= oSrc.m_strEvenPageMasterReference;
						}
						// ---
						~CPageMasterAlternatives()
						{
							Clear();
						}
						// ---
						void	Clear()
						{
							m_strOddPageMasterReference		= _T("");
							m_strEvenPageMasterReference	= _T("");
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							XmlUtils::CXmlNodes oPageMasterReferenceNodes;
							if (oXmlNode.GetNodes(_T("fo:conditional-page-master-reference"), oPageMasterReferenceNodes))
							{
								INT iCount	= oPageMasterReferenceNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oPageMasterReferenceNode;
									oPageMasterReferenceNodes.GetAt(iIndex, oPageMasterReferenceNode);
									CString strOddOrEven	= oPageMasterReferenceNode.GetAttributeOrValue(_T("odd-or-even"), _T(""));
									if (strOddOrEven == _T("odd"))
									{
										m_strOddPageMasterReference	= oPageMasterReferenceNode.GetAttributeOrValue(_T("master-reference"), _T(""));
									}
									else if (strOddOrEven == _T("even"))
									{
										m_strEvenPageMasterReference	= oPageMasterReferenceNode.GetAttributeOrValue(_T("master-reference"), _T(""));
									}
								}
							}
						}

					};
					class CPageSequenceMaster
					{
					public:
						CString	m_strName;

						CAtlArray<CPageMasterAlternatives>	m_arrPageMasterAlternatives;

					public:
						// ---
						CPageSequenceMaster() 
							: m_strName(_T("")), 
							m_arrPageMasterAlternatives()
						{
						}
						CPageSequenceMaster(const CPageSequenceMaster &oSrc)
						{
							m_strName	= oSrc.m_strName;

							m_arrPageMasterAlternatives.Copy(oSrc.m_arrPageMasterAlternatives);
						}
						// ---
						~CPageSequenceMaster()
						{
							Clear();
						}
						// ---
						void	Clear()
						{
							m_strName	= _T("");

							m_arrPageMasterAlternatives.RemoveAll();
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
						{
							m_strName	= oXmlNode.GetAttributeOrValue(_T("master-name"), _T(""));

							XmlUtils::CXmlNodes oPageMasterAlternativesNodes;
							if (oXmlNode.GetNodes(_T("fo:repeatable-page-master-alternatives"), oPageMasterAlternativesNodes))
							{
								INT iCount	= oPageMasterAlternativesNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++ iIndex)
								{
									XmlUtils::CXmlNode oPageMasterAlternativesNode;
									oPageMasterAlternativesNodes.GetAt(iIndex, oPageMasterAlternativesNode);

									m_arrPageMasterAlternatives.Add();
									m_arrPageMasterAlternatives[iIndex].FromXmlNode(oPageMasterAlternativesNode);
								}
							}							
						}
						// ---
						CString	GetPageMasterName()
						{
							if (!m_arrPageMasterAlternatives.IsEmpty())
							{
								return m_arrPageMasterAlternatives[0].m_strEvenPageMasterReference;
							}
							else
							{
								return _T("");
							}
						}
					};
					class CFlowAssignment
					{
					private:
						CAtlArray<CString>	m_arrSources;
						CAtlArray<CString>	m_arrTargets;

					public:
						// ---
						CFlowAssignment() 
							: m_arrSources(), m_arrTargets()
						{
						}
						CFlowAssignment(const CFlowAssignment &oFlowAssignment)
						{
							m_arrSources.Copy(oFlowAssignment.m_arrSources);
							m_arrTargets.Copy(oFlowAssignment.m_arrTargets);
						}
						// ---
						~CFlowAssignment()
						{
							Clear();							
						}
						// ---
						CFlowAssignment& operator=(const CFlowAssignment &oFlowAssignment)
						{
							m_arrSources.Copy(oFlowAssignment.m_arrSources);
							m_arrTargets.Copy(oFlowAssignment.m_arrTargets);

							return *this;
						}
						// ---
						void	Clear()
						{
							m_arrSources.RemoveAll();
							m_arrTargets.RemoveAll();
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							XmlUtils::CXmlNode oSourceNode;
							oXmlNode.GetNode(_T("fo:flow-source-list"), oSourceNode);
							XmlUtils::CXmlNodes oSpecifierNodes;
							if (oSourceNode.GetNodes(_T("fo:flow-name-specifier"), oSpecifierNodes))
							{
								int iCount = oSpecifierNodes.GetCount();
								for (int iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oSpecifierNodes.GetAt(iIndex, oNode);
									CString strFlowNameReference = oNode.GetAttributeOrValue(_T("flow-name-reference"), _T(""));
									if (strFlowNameReference != _T(""))
									{
										m_arrSources.Add(strFlowNameReference);
									}
								}
							}
							XmlUtils::CXmlNode oTargetNode;
							oXmlNode.GetNode(_T("fo:flow-target-list"), oTargetNode);
							if (oTargetNode.GetNodes(_T("fo:region-name-specifier"), oSpecifierNodes))
							{
								int iCount = oSpecifierNodes.GetCount();
								for (int iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oSpecifierNodes.GetAt(iIndex, oNode);
									CString strRegionNameReference = oNode.GetAttributeOrValue(_T("region-name-reference"), _T(""));
									if (strRegionNameReference != _T(""))
									{
										m_arrTargets.Add(strRegionNameReference);
									}
								}
							}
						}
						// ---
						CAtlArray<CString>*	Find(const CString& strSource)
						{
							for (size_t nIndex = 0; nIndex < m_arrSources.GetCount(); ++nIndex)
							{
								if (m_arrSources[nIndex] == strSource)
								{
									return &m_arrTargets;
								}
							}

							return NULL;
						}
					};

					class CFlowMap
					{
					private:
						CString	m_strName;

						CAtlArray<CFlowAssignment>	m_arrAssignments;

					public:
						// ---
						CFlowMap() 
							: m_strName(_T("")), 
							m_arrAssignments()
						{
						}
						CFlowMap(const CFlowMap &oPageMap)
						{
							m_strName	= oPageMap.m_strName;

							m_arrAssignments.Copy(oPageMap.m_arrAssignments);
						}
						// ---
						~CFlowMap()
						{
							Clear();
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
						{
							m_strName	= oXmlNode.GetAttributeOrValue(_T("flow-map-name"), _T(""));
							if (m_strName != _T(""))
							{
								XmlUtils::CXmlNodes oAssignmentNodes;
								if (oXmlNode.GetNodes(_T("fo:flow-assignment"), oAssignmentNodes))
								{
									INT iCount	= oAssignmentNodes.GetCount();
									for (INT iIndex = 0; iIndex < iCount; ++iIndex)
									{
										XmlUtils::CXmlNode oNode;
										oAssignmentNodes.GetAt(iIndex, oNode);

										CFlowAssignment oFlowAssignment;
										oFlowAssignment.FromXmlNode(oNode);

										m_arrAssignments.Add(oFlowAssignment);
									}
								}
							}
						}
						// ---
						void	Clear()
						{
							m_strName	= _T("");

							m_arrAssignments.RemoveAll();
						}
						// ---
						inline BOOL	IsEqualName(const CString& strName)
						{
							return (m_strName == strName);
						}
						// ---
						void	GetRegionNames(const CString& strFlowName, CAtlArray<CString>& arrRegionNames)
						{
							for (size_t nIndex = 0; nIndex < m_arrAssignments.GetCount(); ++nIndex)
							{
								CAtlArray<CString>* pArrTargets	= m_arrAssignments[nIndex].Find(strFlowName);
								if (pArrTargets)
								{
									arrRegionNames.Copy(*pArrTargets);
								}
							}
						}
					};

					class CSimplePageMaster : private CXmlAttributeReader
					{
					private:
						CString	m_strName;

						REAL	m_fWidthPt;
						REAL	m_fHeightPt;

						REAL	m_fWidthL;
						REAL	m_fHeightL;

						CBounds	m_oBounds;
						CBrush  m_oBrush;

						CAtlArray<CRegion>	m_arrRegions;

						PageNumbering _pageNum;

						bool	m_Continuous;

					private:
						void	LoadAttributesFromXml(XmlUtils::CXmlNode& attributes)
						{
							CString continuous = attributes.GetAttribute(_T("continuous"), _T("false"));
							if (continuous == _T("true"))
								m_Continuous = true;
						}

					public:
						CSimplePageMaster() 
							: m_strName(_T("")), 
							m_fWidthPt(0.0f), m_fHeightPt(0.0f), 
							m_fWidthL(0.0f), m_fHeightL(0.0f), 
							m_oBounds(), 
							m_oBrush(), m_arrRegions(), 
							m_Continuous(false)
						{
						}
						~CSimplePageMaster()
						{
							Clear();							
						}
					public:
						void	Clear()
						{
							m_strName	= _T("");

							m_fWidthPt	= 0.0f;
							m_fHeightPt	= 0.0f;

							m_fWidthL	= 0.0f;
							m_fHeightL	= 0.0f;

							m_oBounds.Clear();
							m_arrRegions.RemoveAll();

							m_Continuous = false;
						}
						void	loadFromXml(XmlUtils::CXmlNode& oXmlNode, CDocumentManager& documentManager)
						{
							m_strName	= oXmlNode.GetAttributeOrValue(_T("master-name"), _T(""));

							if (documentManager.m_ItemManager.IsFo())
							{
								m_fWidthPt	= FloatPtAttributeFromXml(oXmlNode, _T("page-width"), 612.0f);
								m_fHeightPt	= FloatPtAttributeFromXml(oXmlNode, _T("page-height"), 792.0f);
							}
							else
							{
								m_fWidthPt	= FloatPtAttributeFromXml(oXmlNode, _T("page-width"));
								m_fHeightPt	= FloatPtAttributeFromXml(oXmlNode, _T("page-height"));
							}

							m_fWidthL	= m_fWidthPt;
							m_fHeightL	= m_fHeightPt;

							m_oBounds.FromXmlNodeFo2(oXmlNode);
							ReadRegions(oXmlNode);

							XmlUtils::CXmlNode pageNumberingXml;
							if (oXmlNode.GetNode(_T("pageNumbering"), pageNumberingXml))
								_pageNum.loadFromXml(pageNumberingXml);

							LoadAttributesFromXml(oXmlNode);
						}
					public:
						inline REAL	GetWidthPt()
						{
							return m_fWidthPt;
						}
						inline REAL	GetHeightPt()
						{
							return m_fHeightPt;
						}
						inline REAL	GetWidthL()
						{
							return m_fWidthL;
						}
						inline REAL	GetHeightL()
						{
							return m_fHeightL;
						}
						inline RectF	GetRectPt()
						{
							return m_oBounds.GetRectPt();
						}
						const PageNumbering& getPageNumbering() const {
							return _pageNum;
						}
						inline bool	Continuous()
						{
							return m_Continuous;
						}
					public:
						inline BOOL	IsEqualName(const CString& strName)
						{
							return (m_strName == strName);
						}
					public:
						void		ResetRegions()
						{
							for (size_t nIndex = 0; nIndex < m_arrRegions.GetCount(); ++nIndex)
							{
								m_arrRegions[nIndex].SetComplete();
								m_arrRegions[nIndex].CheckRect();
							}
						}
						CRegion*	GetRegion(const CString& strRegionName)
						{
							CRegion *pRegion	= NULL;

							for (size_t nIndex = 0; nIndex < m_arrRegions.GetCount(); ++nIndex)
							{
								if (m_arrRegions[nIndex].IsEqualName(strRegionName))
								{
									pRegion	= &m_arrRegions[nIndex];
									break;
								}
							}

							return pRegion;
						}
						CRegion**	GetRegions(const CString& strRegionName, size_t& length)
						{
							CRegion **ppRegion	= new CRegion*[m_arrRegions.GetCount()];
							size_t i = 0;

							for (size_t nIndex = 0; nIndex < m_arrRegions.GetCount(); ++nIndex)
							{
								if (m_arrRegions[nIndex].IsEqualName(strRegionName))
								{
									ppRegion[i++]	= &m_arrRegions[nIndex];
								}
							}
							length = i;

							return ppRegion;
						}
					public:
						// --- non-fo
						void	Init(const REAL& fWidthL, const REAL& fHeightL, 
							const REAL& fWidthMm, const REAL& fHeightMm, const CBounds& oBounds, const CBrush& oBrush)
						{
							m_fWidthL	= fWidthL;
							m_fHeightL	= fHeightL;

							m_oBounds.SetAsRectMm(fWidthMm, fHeightMm);
							m_fWidthPt	= m_oBounds.GetRectPt().Width;
							m_fHeightPt	= m_oBounds.GetRectPt().Height;

							for (size_t nIndex = 0; nIndex < m_arrRegions.GetCount(); ++nIndex)
							{
								m_arrRegions[nIndex].SetBounds(oBounds);
							}

							m_oBrush = oBrush;
						}
					private:
						void	ReadRegions(XmlUtils::CXmlNode &oXmlNode, const bool& bMustInitRegions = true)
						{
							bool bSkip = true;
							m_oBounds.SetAsRectPt(m_fWidthPt, m_fHeightPt);

							RectF oPrevRectPt	= RectF();

							XmlUtils::CXmlNodes oNodes;
							if (oXmlNode.GetNodes(_T("*"), oNodes))
							{
								INT iListLength	= oNodes.GetCount();

								for (INT iIndex = 0; iIndex < iListLength; ++iIndex)
								{									
									XmlUtils::CXmlNode oNode;
									oNodes.GetAt(iIndex, oNode);

									if (oNode.GetAttribute(_T("hasCols")) != _T(""))
									{
										XmlUtils::CXmlNode oNodeCols;
										oNode.GetNode(_T("w:cols"), oNodeCols);										

										int  iColsCount	 = oNodeCols.ReadAttributeInt(_T("w:num"), 1);
										BOOL bEqualWidth = XmlUtils::GetBoolean(oNodeCols.GetAttribute(_T("w:equalWidth"), _T("true")));
										double  dSpace	 = oNodeCols.ReadAttributeInt(_T("w:space"), 720) / 20;

										double*  dWidthCols = new double[iColsCount];
										double*  dSpaceCols = new double[iColsCount];
										if (iColsCount != 1)
										{
											if (bEqualWidth == FALSE)
											{
												XmlUtils::CXmlNodes oNodeColGroup;
												if (oNodeCols.GetNodes(_T("w:col"), oNodeColGroup))
												{
													int iGroupLength	= oNodeColGroup.GetCount();
													double dMarginLeft = 0;
													for (int iInd = 0; iInd < iGroupLength; ++iInd)
													{
														XmlUtils::CXmlNode oNodeCol;
														oNodeColGroup.GetAt(iInd, oNodeCol);

														dWidthCols[iInd] = oNodeCol.ReadAttributeDouble(_T("w:w"), 0) / 20;
														dSpaceCols[iInd] = oNodeCol.ReadAttributeDouble(_T("w:space"), 0) / 20;
													}
												}
											}
											else 
											{
												double dWidth = (m_oBounds.GetRectPt().GetRight() - m_oBounds.GetRectPt().GetLeft() - dSpace * (iColsCount - 1)) / iColsCount;												
												for (int iInd = 0; iInd < iColsCount; ++iInd)
												{
													dWidthCols[iInd] = dWidth;
													dSpaceCols[iInd] = dSpace;
												}
											}
											double dMarginLeft = 0;
											for (int iInd = 0; iInd < iColsCount; ++iInd)
											{
												AddRegion(oNode, dMarginLeft, dWidthCols[iInd], oPrevRectPt);

												dMarginLeft += (dWidthCols[iInd] + dSpaceCols[iInd]);	
											}											
										}
										else
										{
											AddRegion(oNode, oPrevRectPt);											
										}	
										delete [] dWidthCols;
										delete [] dSpaceCols;
									}
									else
									{
										AddRegion(oNode, oPrevRectPt);										
									}
								} 
							}							
							/*XmlUtils::CXmlReader oReader;
							if (oReader.SetXmlString(oXmlNode.GetXml()) && oReader.ReadNodeList(_T("*")))
							{
							INT iListLength	= oReader.GetLengthList();

							for (INT iIndex = 0; iIndex < iListLength; ++iIndex)
							{
							XmlUtils::CXmlNode oNode;
							oNode.FromXmlString(oReader.ReadNodeXml(iIndex));

							m_arrRegions.Add();
							m_arrRegions[iIndex].FromXmlNodeFo(oNode);

							m_arrRegions[iIndex].SetBounds(m_oBounds);
							m_arrRegions[iIndex].ApplyDisplayAlign(oPrevRectPt);
							}
							}*/
						}
						void	AddRegion(XmlUtils::CXmlNode oNode, double dMarginLeft, double dWidth, RectF& oPrevRectPt)
						{
							size_t iCount = m_arrRegions.GetCount();
							m_arrRegions.Add();
							m_arrRegions[iCount].FromXmlNodeFo(oNode);							
							m_arrRegions[iCount].SetIsColumn();

							CBounds cBounds = m_arrRegions[iCount].GetBounds();
							cBounds.SetAsRectPt(m_oBounds.GetRectPt());
							m_arrRegions[iCount].AddBounds(dMarginLeft, 0, cBounds.GetRectPt().GetRight() - cBounds.GetRectPt().GetLeft()- dMarginLeft - dWidth, 0);
							m_arrRegions[iCount].SetBounds(m_oBounds);
							m_arrRegions[iCount].ApplyDisplayAlign(oPrevRectPt);
						}
						void	AddRegion(XmlUtils::CXmlNode oNode, RectF& oPrevRectPt)
						{
							size_t iCount = m_arrRegions.GetCount();
							m_arrRegions.Add();
#ifdef AVS_LOG_DEBUG
							CRegion *p1 = &m_arrRegions[iCount];
#endif
							m_arrRegions[iCount].FromXmlNodeFo(oNode, true);

							m_arrRegions[iCount].SetBounds(m_oBounds);
							//m_arrRegions[iCount].ApplyDisplayAlign(oPrevRectPt);
						}
					};

					class CLayoutMasterSet
					{
					private:
						CAtlArray<CSimplePageMaster>	m_arrSimplePageMasters;
						CAtlArray<CFlowMap>				m_arrFlowMaps;
						CAtlArray<CPageSequenceMaster>	m_arrPageSequenceMasters;

					public:
						// ---
						CLayoutMasterSet()
							: m_arrSimplePageMasters(), 
							m_arrFlowMaps(), 
							m_arrPageSequenceMasters()
						{
						}
						// ---
						~CLayoutMasterSet()
						{
							Clear();
						}
						// ---
						void	Clear()
						{
							m_arrSimplePageMasters.RemoveAll();
							m_arrFlowMaps.RemoveAll();
							m_arrPageSequenceMasters.RemoveAll();
						}
						// ---
						void	FromXmlNode(XmlUtils::CXmlNode& oXmlNode, CDocumentManager& documentManager)
						{
							XmlUtils::CXmlNodes oPageTypeNodes;
							if (oXmlNode.GetNodes(_T("fo:simple-page-master"), oPageTypeNodes))
							{
								INT iCount = oPageTypeNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oPageTypeNodes.GetAt(iIndex, oNode);

									m_arrSimplePageMasters.Add();
									m_arrSimplePageMasters[iIndex].loadFromXml(oNode, documentManager);
								}
							}

							XmlUtils::CXmlNodes oPageMapNodes;
							if (oXmlNode.GetNodes(_T("fo:flow-map"), oPageMapNodes))
							{
								INT iCount = oPageMapNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oPageMapNodes.GetAt(iIndex, oNode);

									m_arrFlowMaps.Add();
									m_arrFlowMaps[m_arrFlowMaps.GetCount() - 1].FromXmlNode(oNode);
								}
							}

							XmlUtils::CXmlNodes oPageSequenceMasterNodes;
							if (oXmlNode.GetNodes(_T("fo:page-sequence-master"), oPageSequenceMasterNodes))
							{
								INT iCount = oPageSequenceMasterNodes.GetCount();
								for (INT iIndex = 0; iIndex < iCount; ++iIndex)
								{
									XmlUtils::CXmlNode oNode;
									oPageSequenceMasterNodes.GetAt(iIndex, oNode);

									m_arrPageSequenceMasters.Add();
									m_arrPageSequenceMasters[m_arrPageSequenceMasters.GetCount() - 1].FromXmlNode(oNode);
								}
							}
						}
						// ---
						CSimplePageMaster*		FindSimplePageMaster(const CString& strName)
						{
							size_t nPageTypesCount	= m_arrSimplePageMasters.GetCount();
							for (size_t nIndex = 0; nIndex < nPageTypesCount; ++nIndex)
							{
								if (m_arrSimplePageMasters[nIndex].IsEqualName(strName))
								{
									return &m_arrSimplePageMasters[nIndex];
								}
							}

							return NULL;
						}
						CFlowMap*				FindFlowMap(const CString& strName)
						{
							size_t nPageMapsCount	= m_arrFlowMaps.GetCount();
							for (size_t nIndex = 0; nIndex < nPageMapsCount; ++nIndex)
							{
								if (m_arrFlowMaps[nIndex].IsEqualName(strName))
								{
									return &m_arrFlowMaps[nIndex];
								}
							}

							return NULL;
						}
						CPageSequenceMaster*	FindPageSequenceMaster(const CString& strName)
						{
							size_t nPageSequenceMastersCount	= m_arrPageSequenceMasters.GetCount();
							for (size_t nIndex = 0; nIndex < nPageSequenceMastersCount; ++nIndex)
							{
								if ( m_arrPageSequenceMasters[nIndex].m_strName == strName )
								{
									return &m_arrPageSequenceMasters[nIndex];
								}
							}

							return NULL;
						}
					};

					class CPage	{
					private:
						CSimplePageMaster*	m_pSimplePageMaster;

						CAtlArray<CLine*>	m_arrLines;
						CAtlArray<size_t>	m_arrBlockBeginIndexes;
						int				m_nFlowBeginIndex;
						int				m_nFlowEndIndex;

						CAtlArray<CLine*>	m_arrFLines;
						CAtlArray<size_t>	m_arrBlockFBeginIndexes;
						REAL				m_fFootnoteStart;
						int					m_iCurrentFootnoteIndex;

						CAtlArray<size_t>			m_arrGraphicLinesBeginIndexes;
						CAtlArray<CGraphicLine*>	m_arrGraphicLines;
						int						m_nGraphicLinesBeginIndex;
						int						m_nGraphicLinesEndIndex;

						CAtlArray<size_t>			m_arrBackgroundBeginIndexes;
						CAtlArray<CBackgroundRect*> m_arrBackgroundsRects;
						int						m_nBackgroundBeginIndex;
						int						m_nBackgroundEndIndex;

						CAreaManager			m_oAreaManager;
						CAtlArray<CAlternate2*>	m_arrAnchors;
						int	m_AnchorFirst;
						int m_AnchorBlockFirst;
						int	m_AnchorLast;

						bool	m_bFootnoteProcessing;
						bool	m_bBodyProcessing;

						bool m_bIsEmptyBody;
						bool _withNonDummyLines;

						bool	m_Continuous;
						float	m_FilledHeight;
						float	m_FooterHeight;
						float _footnoteHeight;

						bool _beginCell;

						CArray<RCommand> _lineCommands;

					public:
						CPage() 
							: m_pSimplePageMaster(NULL), 
							m_arrLines(), m_arrBlockBeginIndexes(), 
							m_nFlowBeginIndex(0), m_nFlowEndIndex(0), 
							m_nGraphicLinesBeginIndex(0), m_nGraphicLinesEndIndex(0),
							m_nBackgroundBeginIndex(0),	m_nBackgroundEndIndex(0),
							m_arrFLines(), m_arrBlockFBeginIndexes(), m_fFootnoteStart(0.0f), m_iCurrentFootnoteIndex(-1), 
							m_arrGraphicLines(), m_arrBackgroundsRects(), 
							m_oAreaManager(), m_arrAnchors(), 
							m_AnchorFirst(0), m_AnchorLast(0), m_AnchorBlockFirst(0), 
							m_bFootnoteProcessing(false), m_bBodyProcessing(false), 
							m_bIsEmptyBody(true), _withNonDummyLines(false), 
							m_Continuous(false), m_FilledHeight(0.0f), m_FooterHeight(-1), _footnoteHeight(0), 
							_beginCell(false) {
						}
						~CPage()
						{
							Clear();
						}
						void	Clear()
						{	
							m_pSimplePageMaster	= NULL;

							size_t nLinesCount	= m_arrLines.GetCount();
							for (size_t nIndex = 0; nIndex < nLinesCount; ++nIndex) {
								RELEASEOBJECT(m_arrLines[nIndex]);
							}
							m_arrLines.RemoveAll();
							m_arrBlockBeginIndexes.RemoveAll();
							m_nFlowBeginIndex	= 0;
							m_nFlowEndIndex		= 0;

							size_t nFLinesCount	= m_arrFLines.GetCount();
							for (size_t nIndex = 0; nIndex < nFLinesCount; ++nIndex)
							{
								RELEASEOBJECT(m_arrFLines[nIndex]);
							}
							m_arrFLines.RemoveAll();
							m_arrBlockFBeginIndexes.RemoveAll();
							m_fFootnoteStart	= 0.0f;
							m_iCurrentFootnoteIndex = -1;

							size_t nGraphicLinesCount	= m_arrGraphicLines.GetCount();
							for (size_t nIndex = 0; nIndex < nGraphicLinesCount; ++nIndex)
							{
								RELEASEOBJECT(m_arrGraphicLines[nIndex]);
							}
							m_nGraphicLinesBeginIndex = 0;
							m_nGraphicLinesEndIndex   = 0;
							m_arrGraphicLines.RemoveAll();

							size_t nBackgroundsRectsCount	= m_arrBackgroundsRects.GetCount();
							for (size_t nIndex = 0; nIndex < nBackgroundsRectsCount; ++nIndex)
							{
								RELEASEOBJECT(m_arrBackgroundsRects[nIndex]);
							}
							m_nBackgroundBeginIndex = 0;
							m_nBackgroundEndIndex	= 0;
							m_arrBackgroundsRects.RemoveAll();

							m_oAreaManager.Clear();
							m_arrAnchors.RemoveAll();
							m_AnchorFirst = 0;
							m_AnchorLast = 0;
							m_AnchorBlockFirst = 0;

							m_bFootnoteProcessing	= false;

							m_Continuous = false;
							m_FilledHeight = 0.0f;
							m_FooterHeight = -1;
							_footnoteHeight = 0;

							_beginCell = false;
						}
					public:
						void	ClearDerivedLines(int from)
						{
							for (size_t i = m_nFlowEndIndex; i != from; i--)
								m_arrLines.RemoveAt(i - 1);
							m_nFlowEndIndex = from;
						}
						void	ClearDerivedGraphLines(int startIndex)
						{
							for (int i = m_nGraphicLinesEndIndex; i != startIndex; --i)
								m_arrGraphicLines.RemoveAt(i - 1);
							m_nGraphicLinesEndIndex = startIndex;
						}
						void	ClearDerivedBackgrounds(int startIndex)
						{
							for (int i = m_nBackgroundEndIndex; i != startIndex; --i)
								m_arrBackgroundsRects.RemoveAt(i - 1);
							m_nBackgroundEndIndex = startIndex;
						}
						inline size_t GetLastLine()
						{
							return m_nFlowEndIndex;
						}
						inline int	GetLastGraphLine() const
						{
							return (int)m_nGraphicLinesEndIndex;
						}
						inline int	GetLastBackground() const
						{
							return (int)m_nBackgroundEndIndex;
						}
						inline void	SetPageMaster(CSimplePageMaster* pSimplePageMaster)
						{
							m_pSimplePageMaster	= pSimplePageMaster;
							m_pSimplePageMaster->ResetRegions();
						}
						inline REAL	GetWidthPt() const
						{
							if (m_pSimplePageMaster)
								return m_pSimplePageMaster->GetWidthPt();
							else
								return 0;

						}
						inline REAL	GetHeightPt() const
						{
							if (m_pSimplePageMaster)
								return m_pSimplePageMaster->GetHeightPt();
							else
								return 0;
						}
						inline REAL	GetWidthL()
						{
							return m_pSimplePageMaster->GetWidthL();
						}
						inline REAL	GetHeightL()
						{
							return m_pSimplePageMaster->GetHeightL();
						}
						inline REAL	GetWidthMm() const
						{
							return GetWidthPt() * (25.4f / 72.0f);
						}
						inline REAL	GetHeightMm() const
						{
							return GetHeightPt() * (25.4f / 72.0f);
						}
						inline CAreaManager*	GetAreaManager()
						{
							return &m_oAreaManager;
						}
						inline CAtlArray<CLine*>& GetRefLines ()
						{
							return m_arrLines;
						}
						int getBlockCount() const {
							return (int)m_arrBlockBeginIndexes.GetCount();
						}
						void getBlockMetric(const int& block, float& height, float& offset) {
							height = 0;
							offset = 0;

							int blockCount = (int)m_arrBlockBeginIndexes.GetCount();
							if (block < 0 || block >= blockCount)
								return;

							int firstLine = m_arrBlockBeginIndexes[block];
							int lastLine = block + 1 == blockCount ? m_arrLines.GetCount() - 1 : m_arrBlockBeginIndexes[block + 1] - 1;

							if (firstLine >= (int)m_arrLines.GetCount())
								return;

							float bottom = m_arrLines[lastLine]->GetRect().GetBottom();
							offset = m_arrLines[firstLine]->GetRect().GetTop();
							height =  bottom - offset;
						}
					public:
						inline void	StartFootnote()
						{
							m_bFootnoteProcessing	= true;
							m_iCurrentFootnoteIndex = -1;
							if (m_arrFLines.GetCount() != 0)
							{
								m_iCurrentFootnoteIndex	= (int)m_arrFLines.GetCount() - 1;
							}
						}
						inline void	FinishFootnote()
						{
							m_bFootnoteProcessing	= false;
							size_t nLastIndex = m_arrFLines.GetCount() - 1;
							if (nLastIndex - m_iCurrentFootnoteIndex > 0)
							{
								REAL fOffset	= m_arrFLines[m_iCurrentFootnoteIndex + 1]->GetRect().Y - m_fFootnoteStart;

								for (size_t nIndex = m_iCurrentFootnoteIndex + 1; nIndex <= nLastIndex; ++nIndex)
								{
									m_arrFLines[nIndex]->MoveOn(0, -fOffset);
								}
								m_fFootnoteStart	+= m_arrFLines[nLastIndex]->GetRect().GetBottom() - m_arrFLines[m_iCurrentFootnoteIndex + 1]->GetRect().Y;
							}
						}
						inline void	StartBlock()
						{
							if (!m_bFootnoteProcessing)
							{
								m_arrBlockBeginIndexes.Add((unsigned int)m_arrLines.GetCount());
								m_arrBlockFBeginIndexes.Add((unsigned int)m_arrFLines.GetCount());

								m_arrGraphicLinesBeginIndexes.Add((unsigned int)m_arrGraphicLines.GetCount());
								m_arrBackgroundBeginIndexes.Add((unsigned int)m_arrBackgroundsRects.GetCount());

								m_AnchorBlockFirst = m_AnchorLast;
							}
						}
						inline void	FinishBlock()
						{
						}
						inline void StartBody()
						{
							m_bBodyProcessing	= true;
						}
						inline void	FinishBody()
						{
							m_bBodyProcessing	= false;
						}
						inline bool IsEmptyBody() {
							return (m_bIsEmptyBody && !m_bFootnoteProcessing);
						}
						inline bool isWithNonDummyLines() const {
							return _withNonDummyLines;
						}
						inline void	DeleteBlocks(int number)
						{
							while (number-- > 0)
							{
								int lastBlock = (int)m_arrBlockBeginIndexes.GetCount() - 1;
								if (lastBlock < 0)
									break;

								int blockFirstLine = (int)m_arrBlockBeginIndexes[lastBlock];
								int lineCount = (int)m_arrLines.GetCount();
								if (blockFirstLine < lineCount)
								{
									for (int index = blockFirstLine; index < (int)lineCount; ++index)
										RELEASEOBJECT(m_arrLines[index]);

									m_arrLines.RemoveAt(blockFirstLine, lineCount - blockFirstLine);
									m_nFlowEndIndex	= blockFirstLine;
								}
								m_arrBlockBeginIndexes.RemoveAt(lastBlock);

								int blockFirstFootLine = (int)m_arrBlockFBeginIndexes[lastBlock];
								int footLineCount = (int)m_arrFLines.GetCount();
								if (blockFirstFootLine < footLineCount)
								{
									for (int index = blockFirstFootLine; index < footLineCount; ++index)
										RELEASEOBJECT(m_arrFLines[index]);

									m_arrFLines.RemoveAt(blockFirstFootLine, footLineCount - blockFirstFootLine);
								}
								m_arrBlockFBeginIndexes.RemoveAt(lastBlock);

								int blockFirstGraphLine = (int)m_arrGraphicLinesBeginIndexes[lastBlock];
								int graphLineCount = (int)m_arrGraphicLines.GetCount();
								if (blockFirstGraphLine < graphLineCount)
								{
									for (int index = blockFirstGraphLine; index < graphLineCount; ++index)
										RELEASEOBJECT(m_arrGraphicLines[index]);

									m_arrGraphicLines.RemoveAt(blockFirstGraphLine, graphLineCount - blockFirstGraphLine);
									m_nGraphicLinesEndIndex	= blockFirstGraphLine;
								}

								int blockFirstBackground = (int)m_arrBackgroundBeginIndexes[lastBlock];
								int backgroundCount = (int)m_arrBackgroundsRects.GetCount();
								if (blockFirstBackground < backgroundCount)
								{
									for (int index = blockFirstBackground; index < backgroundCount; ++index)
										RELEASEOBJECT(m_arrBackgroundsRects[index]);

									m_arrBackgroundsRects.RemoveAt(blockFirstBackground, backgroundCount - blockFirstBackground);
									m_nBackgroundEndIndex = blockFirstBackground;
								}
							}
						}
						inline void	AddLine(CLine* line) {
							if (m_bFootnoteProcessing)
								m_arrFLines.Add(line);
							else {
								if (_beginCell) {
									_beginCell = false;
									line->setStartCell();
								}

								if (m_bBodyProcessing && !line->isDummy()) {
									m_bIsEmptyBody = false;
									_withNonDummyLines = true;
								}
								
								setRendererCommandsToLine(line);
								m_arrLines.Add(line);

								++m_nFlowEndIndex;
							}
						}
						void		AddAnchorContent(CAtlArray<CAlternate2*> &arrAnchors) {
							size_t anchorCount = arrAnchors.GetCount();
							if (anchorCount && !m_bFootnoteProcessing && m_bBodyProcessing)
								m_bIsEmptyBody = false;

							size_t nAnchorIndex	= m_arrAnchors.GetCount();
							RectF oPageRectPt	= m_pSimplePageMaster->GetRectPt();

							for (size_t nIndex = 0; nIndex < anchorCount; ++nIndex)	{
								CAlternate2* anchor = arrAnchors[nIndex];
								if (!arrAnchors[nIndex]->Related())
									continue;

								float zIndex = arrAnchors[nIndex]->getZIndex();
								int count = (int) m_arrAnchors.GetCount();
								int insIndex = 0;
								for (; insIndex < count; ++insIndex)
									if (zIndex < m_arrAnchors[insIndex]->getZIndex())
										break;
								if (insIndex == count)
									m_arrAnchors.Add(arrAnchors[nIndex]);
								else
									m_arrAnchors.InsertAt((size_t) insIndex, arrAnchors[nIndex]);
								arrAnchors[nIndex]->setInHeader(!m_bBodyProcessing);

								m_oAreaManager.AddWrapPolygon(arrAnchors[nIndex]->Points(), 
									arrAnchors[nIndex]->DistanceLeft(), arrAnchors[nIndex]->DistanceRight());
								arrAnchors[nIndex]->Placed(true);

								++m_AnchorLast;
							}
						}
						void		RemoveAnchorContent(int position)
						{
							if (m_AnchorBlockFirst == m_AnchorLast)
								return;
							int index = m_AnchorLast - 1;
							while (index >= m_AnchorBlockFirst) {
								if (m_arrAnchors[index]->InlinePosition() != position)
									break;
								m_arrAnchors[index]->Placed(false);
								--index;
							}
							++index;
							if (index != m_AnchorLast) {
								m_arrAnchors.RemoveAt(index, m_AnchorLast - index);
								m_AnchorLast = (int)m_arrAnchors.GetCount();
							}

						}
						void		AddGraphicLine(CGraphicLine *pGraphicLine)
						{
							m_arrGraphicLines.Add(pGraphicLine);
							++m_nGraphicLinesEndIndex;
						}
						void		AddBackgroundRect(CBackgroundRect *pBackgroundRect)
						{
							m_arrBackgroundsRects.Add(pBackgroundRect);
							++m_nBackgroundEndIndex;
						}
						// ---
						inline void	StartPrintingFlow()
						{
							if (!m_bFootnoteProcessing)
							{
								m_nFlowBeginIndex = (int) m_arrLines.GetCount();
								m_nFlowEndIndex = (int) m_arrLines.GetCount();

								m_nGraphicLinesBeginIndex = (int) m_arrGraphicLines.GetCount();
								m_nGraphicLinesEndIndex   = (int) m_arrGraphicLines.GetCount();

								m_nBackgroundBeginIndex = (int) m_arrBackgroundsRects.GetCount();
								m_nBackgroundEndIndex	= (int) m_arrBackgroundsRects.GetCount();
								m_AnchorFirst = (int) m_arrAnchors.GetCount();
								m_AnchorLast = (int) m_arrAnchors.GetCount();
							}
						}
						inline void	FinishPrintingFlow()
						{
							if (!m_bFootnoteProcessing)
							{
								m_nFlowBeginIndex	= 0;
								m_nFlowEndIndex		= 0;

								m_nGraphicLinesBeginIndex = 0;
								m_nGraphicLinesEndIndex   = 0;

								m_nBackgroundBeginIndex = 0;
								m_nBackgroundEndIndex	= 0;

								m_AnchorFirst = 0;
								m_AnchorLast = 0;
							}
						}

						void		MoveFlowOn(const REAL &fDeltaX, const REAL &fDeltaY)
						{
							if (!m_bFootnoteProcessing)
							{
								for (int nIndex = m_nGraphicLinesBeginIndex; nIndex < (int) m_arrGraphicLines.GetCount(); ++nIndex)
								{
									m_arrGraphicLines[nIndex]->MoveOn(fDeltaX * 25.4 / 72, fDeltaY * 25.4 / 72);
								}

								for (int nIndex = m_nBackgroundBeginIndex; nIndex < (int) m_arrBackgroundsRects.GetCount(); ++nIndex)
								{
									m_arrBackgroundsRects[nIndex]->MoveOn(fDeltaX * 25.4 / 72, fDeltaY * 25.4 / 72);
								}

								for (int nIndex = m_nFlowBeginIndex; nIndex < m_nFlowEndIndex; ++nIndex)
								{
									m_arrLines[nIndex]->MoveOn(fDeltaX, fDeltaY);
								}
								for (int i = m_AnchorFirst; i < m_AnchorLast; ++i)
									if (m_arrAnchors[i]->AnchoredToRegion())
										m_arrAnchors[i]->MoveOn(fDeltaX, fDeltaY);
							}
						}
					public:
						void beginCell() {
							_beginCell = true;							
						}
						void endCell() {
							_beginCell = false;
							if (m_arrLines.GetCount() > 0) {
								int last = (int) m_arrLines.GetCount() - 1;
								m_arrLines[last]->setFinishCell();
							}
						}
						// ---
						void	Draw(IAVSRenderer* pRenderer, CDocumentManager& documentManager, const float& fCompleteness)
						{
							documentManager.SetPage(*this);
							for (size_t nIndex = 0; nIndex < m_arrBackgroundsRects.GetCount(); ++nIndex)
								m_arrBackgroundsRects[nIndex]->Draw(pRenderer, fCompleteness);

							for (size_t nIndex = 0; nIndex < m_arrAnchors.GetCount(); ++nIndex) {
								CAlternate2* anchor = m_arrAnchors[nIndex];
								if (anchor->getZIndex() < 0)
									anchor->Draw(pRenderer, documentManager, fCompleteness);
							}
							
							DrawLines(pRenderer, documentManager, fCompleteness);
							drawFootnoteLines(pRenderer, documentManager, fCompleteness);

							for (size_t nIndex = 0; nIndex < m_arrAnchors.GetCount(); ++nIndex) {
								CAlternate2* anchor = m_arrAnchors[nIndex];
								if (anchor->getZIndex() >= 0)
									anchor->Draw(pRenderer, documentManager, fCompleteness);
							}

							for (size_t nIndex = 0; nIndex < m_arrGraphicLines.GetCount(); ++nIndex)
								m_arrGraphicLines[nIndex]->Draw(pRenderer, fCompleteness);
						}
						void Draw(IAVSRenderer* renderer, CDocumentManager& documentManager, const int& block, const float& completeness) {
							int blockCount = (int)m_arrBlockBeginIndexes.GetCount();
							if (block < 0 || block >= blockCount)
								return;

							int firstLine = m_arrBlockBeginIndexes[block];
							int lastLine = block + 1 == blockCount ? m_arrLines.GetCount() - 1 : m_arrBlockBeginIndexes[block + 1] - 1;

							if (firstLine >= (int)m_arrLines.GetCount())
								return;

							for (int i = firstLine; i <= lastLine; ++i)
								m_arrLines[i]->Draw(renderer, documentManager, completeness);
						}
						void DrawLines(IAVSRenderer* renderer, CDocumentManager& documentManager, const float& completeness) {
							for (size_t index = 0; index < m_arrLines.GetCount(); ++index)
								m_arrLines[index]->Draw(renderer, documentManager, completeness);
						}
						void drawFootnoteLines(IAVSRenderer* renderer, CDocumentManager& documentManager, const float& completeness) {
							IAVSRenderer2* renderer2 = NULL;
							if (!SUCCEEDED(renderer->QueryInterface(__UUIDOF__IAVSRENDERER2, (void**)&renderer2)))
								renderer2 = NULL;

							if (renderer2 != NULL)
								renderer->BeginCommand(c_nFootnote);

							if (m_fFootnoteStart != 0.0f) {
								CRegion* pRegionBody	= GetRegion(_T("xsl-region-body"));
								RectF originalRect		= pRegionBody->GetOriginalRectPt();
								REAL fRegionBottom		= originalRect.GetBottom();
								if ((m_FooterHeight >= 0) && (m_FooterHeight < fRegionBottom))
									fRegionBottom = m_FooterHeight;

								CGraphicLine* grL = new CGraphicLine();
								double koef = 25.4f / 72.0f;
								//TODO   
								grL->SetStartPoint( originalRect.GetLeft() * koef, (fRegionBottom - m_fFootnoteStart)*koef - 1);
								grL->SetFinishPoint( originalRect.GetLeft() * koef + 50, (fRegionBottom - m_fFootnoteStart)*koef - 1);
								CPen pen;
								pen.m_oPen.Size = 0.2;
								grL->SetPen(pen);
								AddGraphicLine(grL);

								for (size_t nIndex = 0; nIndex < m_arrFLines.GetCount(); ++nIndex)
									m_arrFLines[nIndex]->MoveOn(0, fRegionBottom - m_fFootnoteStart);								

								m_fFootnoteStart = 0.0f;
							}

							for (size_t nIndex = 0; nIndex < m_arrFLines.GetCount(); ++nIndex)
								m_arrFLines[nIndex]->Draw(renderer, documentManager, completeness);

							if (renderer2 != NULL)
								renderer->EndCommand(c_nFootnote);

							RELEASEINTERFACE(renderer2);
						}
						// ---
						void GetLines(CAtlArray<CLine*>& lines)
						{
							lines.Copy(m_arrLines);
							m_arrLines.RemoveAll();
						}

						inline CRegion*		GetRegion(const CString& strRegionName)
						{
							return m_pSimplePageMaster->GetRegion(strRegionName);
						}
						inline CRegion**	GetRegions(const CString& strRegionName, size_t& iLength)
						{
							return m_pSimplePageMaster->GetRegions(strRegionName, iLength);
						}

					public:
						bool	Continuous() const 
						{
							return m_Continuous;
						}
						void	Continuous(const bool& continuous)
						{
							m_Continuous = continuous;
						}
						float	FilledHeight() const 
						{
							return m_FilledHeight;
						}
						void	FilledHeight(const float& filledHeight)
						{
							m_FilledHeight = filledHeight;
						}
						float	FooterHeight()
						{
							return m_FooterHeight;
						}
						void	FooterHeight(const float& footerHeight)
						{
							m_FooterHeight = footerHeight;
						}
						void	Merge(CPage* page)
						{
							m_arrLines.Append(page->m_arrLines);
							m_arrFLines.Append(page->m_arrFLines);

							m_arrGraphicLines.Append(page->m_arrGraphicLines);
							m_arrBackgroundsRects.Append(page->m_arrBackgroundsRects);
							m_arrAnchors.Append(page->m_arrAnchors);
						}
						void changeFootnoteHeight(const float& footnoteHeight) {
							_footnoteHeight += footnoteHeight;
						}
						float getFootnoteHeight() const {
							return _footnoteHeight;
						}
					public:
						bool	InFootnoteProcess()
						{
							return m_bFootnoteProcessing;
						}


					public:
						void addRendererCommand(const SharedPtr<RendererCommand>& command
								, const bool& toLast = false) {
							if (toLast)
								if (removeEmptyCommands(command))
									return;
							
							_lineCommands.Add(command);
							if (toLast) {
								int last = (int) m_arrLines.GetCount() - 1;
								if (last >= 0)
									setRendererCommandsToLine(m_arrLines[last]);
							}
						}
						bool removeEmptyCommands(const SharedPtr<RendererCommand>& command) {
							int commandCount = (int) _lineCommands.GetCount();
							if (commandCount > 0 && command->getType() == RendererCommand::END 
									&& command->getName() != RendererCommand::COMMON) {
								RendererCommand::RendererCommandName name = command->getName();
								for (int i = commandCount - 1; i >= 0; --i)
									if (_lineCommands[i]->getName() == name)
										if (_lineCommands[i]->getType() == RendererCommand::BEGIN) {
											_lineCommands.RemoveAt(i, commandCount - i);
											return true;
										}
							}
							return false;
						}
						void setRendererCommandsToLine(CLine* line) {
							if (_lineCommands.GetCount() != 0) {
								setAttrsOfRendererCommands(line);
								line->setRendererCommands(_lineCommands);
								_lineCommands.RemoveAll();
							}
						}
						void setAttrsOfRendererCommands(CLine* line) {
							for (int i = 0; i < (int) _lineCommands.GetCount(); ++i)
								_lineCommands[i]->setRect(line->GetRect());
						}

					public:
						static CPage* createNew(const CPage* prevPage = NULL) {
							CPage* page = new CPage();
							if (prevPage != NULL) {
								page->_lineCommands.Copy(prevPage->_lineCommands);
								page->_footnoteHeight = prevPage->_footnoteHeight;
							}

							return page;
						}
					};
				public:

					class TextMeasurer2 {
						CTextMeasurer* tm;
						int block;

						float height;
						float offset;

					public:
						TextMeasurer2(CTextMeasurer* tm = NULL, const int& block = 0, const float& height = 0, const float& offset = 0)
								: tm(tm), block(block)
								, height(height), offset(offset) {}
						void DrawFirstPage(IAVSRenderer* renderer, const float& completeness) {
							if (tm)
								tm->DrawFirstPage(renderer, completeness, block);
						}

						float GetHeight() const {
							return height;
						}
						float GetOffset() const {
							return offset;
						}
					};
					
					CLayoutMasterSet		m_oLayoutMasterSet;
					CArray<CPageSequence*>	m_arrPageSequence;

					INT		m_iPageSequenceIndex;
					bool	m_bIsFo;
					REAL	m_fCompleteness;

					IAVSRenderer*		m_pRenderer;
					CDocumentManager	m_DocumentManager;

					IAVSFontManager*	m_pFontManager;
					bool handlesFontManager;

					CAtlArray<CPage*>	m_arrPages;
					size_t				m_nCurrentPage;

					REAL	m_fWidthMm;
					REAL	m_fHeightMm;

					CString	m_strText;
					bool	m_bIsFormated;

					bool	m_bVertical;
					bool	m_bRightToLeft;

					double	m_dAngle;
					long	m_lFlags;
					RectF	m_oLayoutRect;
					CBrush  m_oBrush;
					CBackgroundProperties m_oBGPage;

				public:
					CTextMeasurer() 
						: m_oLayoutMasterSet(), m_arrPageSequence(), 
						m_iPageSequenceIndex(0), m_bIsFo(false), m_fCompleteness(1.0f), 
						m_pRenderer(NULL), m_DocumentManager(), 
						m_pFontManager(NULL), handlesFontManager(true), 
						m_arrPages(), m_nCurrentPage(0), 
						m_fWidthMm(0), m_fHeightMm(0), 
						m_strText(_T("")), m_bIsFormated(false), 
						m_bVertical(false), m_bRightToLeft(false), 
						m_dAngle(0.0), m_lFlags(0), m_oBGPage(),
						m_oLayoutRect()
					{
						InitFontManager();
					}
					CTextMeasurer(IAVSFontManager* fm) 
						: m_oLayoutMasterSet(), m_arrPageSequence(), 
						m_iPageSequenceIndex(0), m_bIsFo(false), m_fCompleteness(1.0f), 
						m_pRenderer(NULL), m_DocumentManager(), 
						m_pFontManager(fm), handlesFontManager(false), 
						m_arrPages(), m_nCurrentPage(0), 
						m_fWidthMm(0), m_fHeightMm(0), 
						m_strText(_T("")), m_bIsFormated(false), 
						m_bVertical(false), m_bRightToLeft(false), 
						m_dAngle(0.0), m_lFlags(0), m_oBGPage(),
						m_oLayoutRect() {}
					~CTextMeasurer()
					{
						InternalClear();
						if (handlesFontManager)
							RELEASEINTERFACE(m_pFontManager);
					}
				public:
					virtual INT		InternalGetRequiredStreamSize()
					{
						return 0;
					}
					virtual void	InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
					}
					virtual void	InternalFromXmlNode(XmlUtils::CXmlNode &oXmlNode) {
						InternalClear();
						
						#ifdef TME_DEBUG
						CString strXml	= oXmlNode.GetXml();
						FILE *pDebugInfoFile;
						fopen_s(&pDebugInfoFile, "c:\\temp\\debug_fo_doc.xml", "w");
						fwprintf_s(pDebugInfoFile, L"%s", strXml);
						fclose(pDebugInfoFile);
						#endif
						IWorker_OnProgress(0.0f);

						CItemManager *pItemManager	= &m_DocumentManager.m_ItemManager;
						m_DocumentManager.m_FontManager = m_pFontManager;

						#ifdef TME_DEBUG_VSXML
						oXmlNode.FromXmlFile(_T("c:\\temp\\vs.xml"));
						#endif

						if (oXmlNode.GetAttributeOrValue(_T("xmlns:fo"), _T("")) == _T("http://www.w3.org/1999/XSL/Format"))
						{
							pItemManager->SetIsFo(TRUE);
							InternalFromFoXml(oXmlNode);
						}
						else
						{
							pItemManager->SetIsFo(FALSE);
							InternalFromNonFoXml(oXmlNode);
						}

						IWorker_OnProgress(0.1f);
					}
					void			InternalFromNonFoXml2(XmlUtils::CXmlNode &oXmlNode, LONG& lMode, double dWidthMM, double dHeightMM)
					{
						InternalClear();
						IWorker_OnProgress(0.0f);

						CItemManager *pItemManager	= &m_DocumentManager.m_ItemManager;
						pItemManager->SetIsFo(FALSE);

						m_bIsFo	= FALSE;

						SetDefaultLayoutMasterSet(m_oLayoutMasterSet);

						m_strText = oXmlNode.GetAttributeOrValueExt(_T("text"), _T(""));
						FromXmlString(m_strText);

						if (m_strText == _T(""))
						{
							m_bIsFormated	= TRUE;

							return;
						}

						m_dAngle		= 0;
						m_lFlags		= 0;

						m_bVertical		= 0;
						m_bRightToLeft	= 0;

						REAL fWidthL	= 1000;
						REAL fHeightL	= 1000;

						m_fWidthMm	= (REAL)dWidthMM;
						m_fHeightMm	= (REAL)dHeightMM;

						pItemManager->SetWidthL(fWidthL);
						pItemManager->SetHeightL(fHeightL);
						REAL fMmToPt	= 72.0f / 25.4f;
						pItemManager->SetMultiplierX(m_fWidthMm * fMmToPt);
						pItemManager->SetMultiplierY(m_fHeightMm * fMmToPt);

						CSimplePageMaster *pSimplePageMaster	= m_oLayoutMasterSet.FindSimplePageMaster(_T("SPM"));

						m_oLayoutRect.X			= (REAL)(dWidthMM / 2.0 - 10000);
						m_oLayoutRect.Width		= (REAL)(20000);
						m_oLayoutRect.Y			= (REAL)(dHeightMM / 2.0 - 10000);
						m_oLayoutRect.Height	= (REAL)(20000);

						if ((m_oLayoutRect.Width == 0) || (m_oLayoutRect.Height == 0))
						{
							InternalClear();

							m_bIsFormated	= TRUE;

							return;
						}

						CBounds oBounds;
						oBounds.SetAsRectMm(m_oLayoutRect);

						pSimplePageMaster->Init(fWidthL, fHeightL, m_fWidthMm, m_fHeightMm, oBounds, m_oBrush);

						CTextAttributes oAttributes;
						XmlUtils::CXmlNode oAttributeNode;
						if (oXmlNode.GetNode(_T("Attributes"), oAttributeNode))
							oAttributes.FromXmlNode(oAttributeNode);

						CAtlArray<CBlockAttributes*> arPFs;
						CBlockAttributes *pParagraph = new CBlockAttributes();
						pParagraph->FromXmlNode(oAttributeNode, m_DocumentManager);
						pParagraph->m_lCount = m_strText.GetLength();
						//pParagraph->m_nTextAlignment = oAttributes.m_nTextAlignmentHorizontal;
						arPFs.Add(pParagraph);
						arPFs[0]->m_bIsFirst = true;

						CAtlArray<CCharacter*> arCFs;
						CCharacter *pCharacter = new CCharacter();
						pCharacter->FromXmlNode(oXmlNode);
						pCharacter->m_lCount = m_strText.GetLength();
						pCharacter->m_oAttributes = oAttributes;
						arCFs.Add(pCharacter);

						m_arrPageSequence.Add(new CPageSequence());
						SetDefaultPageSequence(*m_arrPageSequence[0]);

						CFlow* pFirstFlow	= m_arrPageSequence[0]->GetFirstFlow();

						pFirstFlow->SetAlignVertical((CAlignVertical)oAttributes.m_nTextAlignmentVertical);

						if (!Normalize(&arPFs, &arCFs, pFirstFlow->GetBlocks()))
						{
							InternalClear();
						}

						m_arrPages.RemoveAll();
					}
					virtual void	InternalClear()
					{
						m_oLayoutMasterSet.Clear();
						for (int i = 0; i < (int) m_arrPageSequence.GetCount(); ++i)
							if (m_arrPageSequence[i])
								delete m_arrPageSequence[i];
						m_arrPageSequence.RemoveAll();

						m_iPageSequenceIndex	= 0;
						m_bIsFo			= false;
						m_fCompleteness	= 1.0f;

						m_pRenderer		= NULL;

						m_DocumentManager.Clear();
						for (size_t nIndex = 0; nIndex < m_arrPages.GetCount(); ++nIndex)
						{
							RELEASEOBJECT(m_arrPages[nIndex]);
						}
						m_arrPages.RemoveAll();
						m_nCurrentPage	= 0;

						m_fWidthMm		= 0;
						m_fHeightMm		= 0;

						m_strText		= _T("");
						m_bIsFormated	= FALSE;

						m_bVertical		= FALSE;
						m_bRightToLeft	= FALSE;

						m_dAngle		= 0.0f;
						m_lFlags		= 0;
						m_oLayoutRect	= RectF();
					}
				public:
					void	IWorker_Resume()
					{

					}
					void	IWorker_Pause()
					{

					}
					void	IWorker_Stop()
					{

					}
					void	IWorker_OnNewPage(double dWidthMm, double dHeightMm)
					{

					}

					void	IWorker_OnCompletePage()
					{
					}
					void	IWorker_OnProgress(double dCompleteness)
					{
						double d	= dCompleteness;
					}
				public:
					void	SetRenderer(IAVSRenderer *pRenderer)
					{
						m_pRenderer	= pRenderer;
					}
					bool	IsFormated()
					{
						return true;
					}					
					void	RotateRect(RectF& oRect)
					{
						REAL fWidth		= oRect.Width;
						REAL fHeight	= oRect.Height;

						REAL fCenterX	= oRect.X + fWidth / 2;
						REAL fCenterY	= oRect.Y + fHeight / 2;

						oRect.Width		= fHeight;
						oRect.Height	= fWidth;
						oRect.X			= fCenterX - fHeight / 2;
						oRect.Y			= fCenterY - fWidth / 2;
					}
				public:					
					void	InitDrawText(IAVSRenderer *pRenderer, REAL fWidhtPix, REAL fHeightPix)
					{
					}
					bool	DrawText(IAVSRenderer* renderer, const float& completeness
								, const bool& transform = true, const bool& draw = true, const int& block = -1)
					{
						#ifdef TME_EMPTY_DRAWING
						return true;
						#endif

						if (m_bIsFormated && (m_arrPages.GetCount() <= 0))
							return true;

						if (!m_bIsFormated) {
							m_DocumentManager.RendererType(renderer);
							CGrapher* grapher	= new CGrapher();
							grapher->DocumentManager(&m_DocumentManager);
							grapher->FontManager(m_pFontManager);
							
							int lastPage = (int) m_arrPages.GetCount() - 1;
							CPage* prevPage = (lastPage != -1) ? m_arrPages[lastPage] : NULL;
							CPage* newPage = (*m_arrPageSequence[m_iPageSequenceIndex]).NewPage(grapher, prevPage);
							if ((m_iPageSequenceIndex == (int)m_arrPageSequence.GetCount() - 1) 
								|| !(*m_arrPageSequence[m_iPageSequenceIndex + 1]).Continuous())
								(*m_arrPageSequence[m_iPageSequenceIndex]).ReWriteColumns(false);

							bool complete = (*m_arrPageSequence[m_iPageSequenceIndex]).FillPage(*newPage, grapher);
							if (complete)
							{
								while (m_iPageSequenceIndex < (int)m_arrPageSequence.GetCount() - 1)
								{
									++m_iPageSequenceIndex;
									if ((*m_arrPageSequence[m_iPageSequenceIndex]).Continuous())
									{
										(*m_arrPageSequence[m_iPageSequenceIndex]).ContinuePage(*newPage);
										if ((m_iPageSequenceIndex == (int)m_arrPageSequence.GetCount() - 1) 
											|| !(*m_arrPageSequence[m_iPageSequenceIndex + 1]).Continuous())
											(*m_arrPageSequence[m_iPageSequenceIndex]).ReWriteColumns(false);
										complete = (*m_arrPageSequence[m_iPageSequenceIndex]).FillPage(*newPage, grapher);
										if (!complete)
											break;
									}
									else
									{
										complete = false;
										break;
									}
								}
							}

							if (complete && (m_iPageSequenceIndex == (int)m_arrPageSequence.GetCount() - 1))
								m_bIsFormated = true;

							RELEASEOBJECT(grapher);

							if (!newPage->IsEmptyBody())
							{
								m_arrPages.Add(newPage);
								m_nCurrentPage = m_arrPages.GetCount() - 1;

								if (draw)
									DrawPage(newPage, renderer, completeness, transform, block);

								if (m_bIsFo)
									Progress(0.0f);
								else
									Progress(1.0f);
							}

							if (m_bIsFo && m_bIsFormated)
							{
								Progress(1.0f);
								SendLinkerXml(renderer);
							}
						}

						return m_bIsFormated;
					}
					void	DrawTextInRect(BSTR bstrText, const RectF& textRect, IAVSRenderer* renderer, const NSStructures::CFont& font, const NSStructures::CBrush& brush, const NSStructures::CFormat& format, ITextPainter* textPainter)
					{
						CItemManager* itemManager = &m_DocumentManager.m_ItemManager;
						m_DocumentManager.m_FontManager = m_pFontManager;
						m_DocumentManager.m_TextPainter = textPainter;
						itemManager->SetWrapFlags(format.WordWrapBySymbol);

						CRegion textRegion;
						textRegion.FromRectMm(textRect);

						CPage page;

						CGrapher* grapher = new CGrapher();
						grapher->FontManager(m_pFontManager);
						grapher->DocumentManager(&m_DocumentManager);

						CBlock block;
						CString text(bstrText);						
						block.FromString(text, grapher, font, brush, format);

						page.StartPrintingFlow();
						block.FillRegion(textRegion, page, false, grapher);
						float offset = textRegion.GetRegionHeightPt();
						switch (format.StringAlignmentVertical)
						{
						case 1 :
							offset /= 2;
							break;
						case 0 :
							offset = 0;
							break;
						}
						page.MoveFlowOn(0, offset);

						page.Draw(renderer, m_DocumentManager, 1.0f);

						RELEASEOBJECT(grapher);
					}

					void	DrawFirstPage(IAVSRenderer* renderer, const float& completeness, const int& block = -1) {
						if (m_arrPages.GetCount())
							DrawPage(m_arrPages[0], renderer, completeness, false, block);
						else
							DrawText(renderer, completeness, false, true, block);
					}
					void	PrepareFirstPage(IAVSRenderer* renderer) {
						DrawText(renderer, 0, false, false);
					}
					void	TruncateByBlocks(CArray<TextMeasurer2>& blockMeasurers, const bool& removeEmpty = false) {
						blockMeasurers.RemoveAll();
						if (m_arrPages.GetCount() == 0)
							return;

						CPage& page = *m_arrPages[0];
						float height, offset;
						for (int i = 0; i < m_arrPages[0]->getBlockCount(); ++i) {
							page.getBlockMetric(i, height, offset);
							if (removeEmpty && height == 0)
								continue;
							blockMeasurers.Add(TextMeasurer2(this, i, height, offset));
						}
					}

				private:
					void	InternalFromFoXml(XmlUtils::CXmlNode &oXmlNode)
					{
						m_bIsFo	= true;

						XmlUtils::CXmlNode oDocInfoNode;
						if (oXmlNode.GetNode(_T("fo:doc-info"), oDocInfoNode))
							m_DocumentManager.loadFromXml(oDocInfoNode);

						//int iPageCount	= oDocInfoNode.ReadAttributeInt(_T("pages"), 0);
						//m_DocumentManager.SetPageCount(iPageCount);
						XmlUtils::CXmlNode fontTable;
						if (oDocInfoNode.GetNode(_T("font-table"), fontTable))							
							m_DocumentManager.FontTableFromXml(fontTable);
						m_DocumentManager.UseHtmlSpacing(XmlUtils::GetBoolean(oDocInfoNode.GetAttribute(_T("useHtmlSpacing"), _T("true"))) == TRUE);

						XmlUtils::CXmlNode oBackgroundNode;
						oDocInfoNode.GetNode(_T("w:background"), oBackgroundNode);
						m_oBGPage.PageBGFromXml(oBackgroundNode);

						XmlUtils::CXmlNode oLayoutMasterSetNode;
						oXmlNode.GetNode(_T("fo:layout-master-set"), oLayoutMasterSetNode);

						m_oLayoutMasterSet.Clear();
						m_oLayoutMasterSet.FromXmlNode(oLayoutMasterSetNode, m_DocumentManager);

						m_arrPageSequence.RemoveAll();
						CPageSequence* prev = NULL;
						XmlUtils::CXmlNodes	oPageSequenceNodes;
						oXmlNode.GetNodes(_T("fo:page-sequence"), oPageSequenceNodes);
						for (INT iIndex = 0; iIndex < oPageSequenceNodes.GetCount(); ++iIndex) {
							m_arrPageSequence.Add(new CPageSequence());
							(*m_arrPageSequence[iIndex]).SetLayoutMasterSet(&m_oLayoutMasterSet);
							(*m_arrPageSequence[iIndex]).setPrev(prev);
							prev = m_arrPageSequence[iIndex];

							XmlUtils::CXmlNode oPageSequenceNode;
							oPageSequenceNodes.GetAt(iIndex, oPageSequenceNode);
							(*m_arrPageSequence[iIndex]).loadFromXml(oPageSequenceNode, m_DocumentManager);
						}

						m_arrPages.RemoveAll();
					}					
					void	InternalFromNonFoXml(XmlUtils::CXmlNode &oXmlNode)
					{
						m_bIsFo	= FALSE;
						m_DocumentManager.UseHtmlSpacing(false);

						SetDefaultLayoutMasterSet(m_oLayoutMasterSet);

						m_strText = oXmlNode.GetAttributeOrValueExt(_T("text"), _T(""));
						FromXmlString(m_strText);

						if (m_strText == _T(""))
						{
							m_bIsFormated	= TRUE;

							return;
						}

						m_dAngle	= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
						m_lFlags	= XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("flags"), _T("0")));

						m_bVertical		= (0 != XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("vertical"), _T("0"))));
						m_bRightToLeft	= (0 != XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("righttoleft"), _T("0"))));

						REAL fWidthL	= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("widthl"), _T("0")));
						REAL fHeightL	= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("heightl"), _T("0")));

						m_fWidthMm	= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("widthmm"), _T("0")));
						m_fHeightMm	= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("heightmm"), _T("0")));

						CItemManager *pItemManager	= &m_DocumentManager.m_ItemManager;
						pItemManager->SetWidthL(fWidthL);
						pItemManager->SetHeightL(fHeightL);
						REAL fMmToPt	= 72.0f / 25.4f;
						pItemManager->SetMultiplierX(m_fWidthMm * fMmToPt);
						pItemManager->SetMultiplierY(m_fHeightMm * fMmToPt);

						CSimplePageMaster *pSimplePageMaster	= m_oLayoutMasterSet.FindSimplePageMaster(_T("SPM"));

						m_oLayoutRect.X			= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						m_oLayoutRect.Width		= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0"))) 
							- m_oLayoutRect.X;
						m_oLayoutRect.Y			= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						m_oLayoutRect.Height	= (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0"))) 
							- m_oLayoutRect.Y;

						if ((m_oLayoutRect.Width == 0) || (m_oLayoutRect.Height == 0))
						{
							InternalClear();

							m_bIsFormated	= TRUE;

							return;
						}

						if (m_bVertical == TRUE)
						{
							RotateRect(m_oLayoutRect);

							m_dAngle	= 90.0f;
							m_lFlags	= 0;
						}

						CBounds oBounds;
						oBounds.SetAsRectMm(m_oLayoutRect);

						pSimplePageMaster->Init(fWidthL, fHeightL, m_fWidthMm, m_fHeightMm, oBounds, m_oBrush);

						CTextAttributes oAttributes;
						XmlUtils::CXmlNode oAttributeNode;
						if (oXmlNode.GetNode(_T("Attributes"), oAttributeNode))
						{
							oAttributes.FromXmlNode(oAttributeNode);
						}

						CAtlArray<CBlockAttributes*> arPFs;
						XmlUtils::CXmlNodes oNodesPFs;
						if (oXmlNode.GetNodes(_T("Paragraph"), oNodesPFs))
						{
							INT iCount = oNodesPFs.GetCount();
							for (INT iIndex = 0; iIndex < iCount; ++iIndex)
							{
								XmlUtils::CXmlNode oNode;
								oNodesPFs.GetAt(iIndex, oNode);
								arPFs.Add(new CBlockAttributes());
								arPFs[iIndex]->m_nTextAlignment = oAttributes.m_nTextAlignmentHorizontal;
								arPFs[iIndex]->FromXmlNode(oNode, m_DocumentManager);								
							}
							if (iCount == 0)
							{
								CBlockAttributes *pParagraph = new CBlockAttributes();
								pParagraph->FromXmlNode(oXmlNode, m_DocumentManager);
								pParagraph->m_lCount = m_strText.GetLength();
								pParagraph->m_nTextAlignment = oAttributes.m_nTextAlignmentHorizontal;
								arPFs.Add(pParagraph);
							}
							arPFs[0]->m_bIsFirst	= TRUE;
						}

						CAtlArray<CCharacter*> arCFs;
						XmlUtils::CXmlNodes oNodesCFs;
						if (oXmlNode.GetNodes(_T("Character"), oNodesCFs))
						{
							INT iCount = oNodesCFs.GetCount();
							for (INT iIndex = 0; iIndex < iCount; ++iIndex)
							{
								XmlUtils::CXmlNode oNode;
								oNodesCFs.GetAt(iIndex, oNode);
								arCFs.Add(new CCharacter());
								arCFs[iIndex]->m_oAttributes = oAttributes;								
								arCFs[iIndex]->FromXmlNode(oNode, m_DocumentManager);
							}
							if (iCount == 0)
							{
								CCharacter *pCharacter = new CCharacter();
								pCharacter->FromXmlNode(oXmlNode, m_DocumentManager);
								pCharacter->m_lCount = m_strText.GetLength();
								pCharacter->m_oAttributes = oAttributes;
								arCFs.Add(pCharacter);
							}
						}

						m_arrPageSequence.Add(new CPageSequence());
						SetDefaultPageSequence(*m_arrPageSequence[0]);

						CFlow* pFirstFlow	= (*m_arrPageSequence[0]).GetFirstFlow();

						pFirstFlow->SetAlignVertical((CAlignVertical)oAttributes.m_nTextAlignmentVertical);

						if (!Normalize(&arPFs, &arCFs, pFirstFlow->GetBlocks()))
							InternalClear();

						m_arrPages.RemoveAll();
					}
					void	SetDefaultLayoutMasterSet(CLayoutMasterSet &oLayoutMasterSet)
					{
						oLayoutMasterSet.Clear();

						CString strXml	= _T("<fo:layout-master-set xmlns:fo=\"http://www.w3.org/1999/XSL/Format\">");
						strXml			+= _T("<fo:simple-page-master master-name=\"SPM\" margin-left=\"0pt\" margin-top=\"0pt\" margin-right=\"0pt\" margin-bottom=\"0pt\">");
						strXml			+= _T("<fo:region-body region-name=\"R\"/>");
						strXml			+= _T("</fo:simple-page-master>");
						strXml			+= _T("<fo:flow-map flow-map-name=\"FM\">");
						strXml			+= _T("<fo:flow-assignment>");
						strXml			+= _T("<fo:flow-source-list>");
						strXml			+= _T("<fo:flow-name-specifier flow-name-reference=\"F\"/>");
						strXml			+= _T("</fo:flow-source-list>");
						strXml			+= _T("<fo:flow-target-list>");
						strXml			+= _T("<fo:region-name-specifier region-name-reference=\"R\"/>");
						strXml			+= _T("</fo:flow-target-list>");
						strXml			+= _T("</fo:flow-assignment>");
						strXml			+= _T("</fo:flow-map>");
						strXml			+= _T("<fo:page-sequence-master master-name=\"PSM\">");
						strXml			+= _T("<fo:repeatable-page-master-alternatives>");
						strXml			+= _T("<fo:conditional-page-master-reference odd-or-even=\"odd\" master-reference=\"SPM\"/>");
						strXml			+= _T("<fo:conditional-page-master-reference odd-or-even=\"even\" master-reference=\"SPM\"/>");
						strXml			+= _T("</fo:repeatable-page-master-alternatives>");
						strXml			+= _T("</fo:page-sequence-master>");
						strXml			+= _T("</fo:layout-master-set>");

						XmlUtils::CXmlNode oNode;
						oNode.FromXmlString(strXml, _T("xmlns:fo='http://www.w3.org/1999/XSL/Format'"));
						oLayoutMasterSet.FromXmlNode(oNode, m_DocumentManager);
					}
					void	SetDefaultPageSequence(CPageSequence &oPageSequence)
					{
						oPageSequence.Clear();

						CString strXml = _T("<fo:page-sequence xmlns:fo=\"http://www.w3.org/1999/XSL/Format\" master-reference=\"PSM\" flow-map-reference=\"FM\">");
						strXml += _T("<fo:flow flow-name=\"R\">");
						strXml += _T("</fo:flow>");
						strXml += _T("</fo:page-sequence>");

						XmlUtils::CXmlNode oNode;
						oNode.FromXmlString(strXml, _T("xmlns:fo=\"http://www.w3.org/1999/XSL/Format\""));
						oPageSequence.SetLayoutMasterSet(&m_oLayoutMasterSet);
						oPageSequence.loadFromXml(oNode, m_DocumentManager);
					}
					bool	Normalize(CAtlArray<CBlockAttributes*> *pArrayPFs, CAtlArray<CCharacter*> *pArrayCFs, 
						CAtlArray<CCommonBlock*> &arrParagraphs)
					{
						size_t nParagraphsCount		= pArrayPFs->GetCount();
						size_t nCharactersCount		= pArrayCFs->GetCount();
						size_t nTextLength			= m_strText.GetLength();
						size_t nCurrentSymbolIndex	= 0;

						size_t nIndex			= 0;
						size_t nSymbolsCount	= 0;
						for (; nIndex < nParagraphsCount; ++nIndex)
						{
							nSymbolsCount	+= (*pArrayPFs)[nIndex]->m_lCount;
							if (nSymbolsCount >= nTextLength)
							{
								break;
							}
						}
						if (nIndex < nParagraphsCount - 1)
						{
							for (size_t i = nIndex + 1; i < nParagraphsCount; ++i)
							{
								RELEASEOBJECT((*pArrayPFs)[i]);
							}
							pArrayPFs->RemoveAt(nIndex + 1, nParagraphsCount - nIndex - 1);
							nParagraphsCount	= pArrayPFs->GetCount();
						}
						if (nSymbolsCount != nTextLength)
						{
							CBlockAttributes *pParagraph	= (*pArrayPFs)[nParagraphsCount - 1];
							pParagraph->m_lCount					+= (LONG)(nTextLength - nSymbolsCount);
						}

						size_t nCurrentCharacter	= 0;
						for (size_t nIndex = 0; nIndex < nParagraphsCount; ++ nIndex)
						{
							CBlockAttributes *pParagraph = (*pArrayPFs)[nIndex];
							CBlock *pTextParagraph = new CBlock;
							arrParagraphs.Add(pTextParagraph);	
							pTextParagraph->m_pBlockAttributes = pParagraph;
							size_t nSymbolsCount	= 0;
							size_t nParagraphLen	= pParagraph->m_lCount;
							for (; nCurrentCharacter < nCharactersCount; ++nCurrentCharacter)
							{
								if (nSymbolsCount >= nParagraphLen)
								{
									break;
								}
								CCharacter *pCharacter	= (*pArrayCFs)[nCurrentCharacter];
								size_t nCharacterLen	= pCharacter->m_lCount;
								if (nCharacterLen == 0)
								{
									RELEASEOBJECT(pCharacter);
									continue;
								}
								if (nSymbolsCount + nCharacterLen <= nParagraphLen)
								{
									nSymbolsCount	+= nCharacterLen;
									pTextParagraph->m_Items.AddCharacter(pCharacter);
								}
								else
								{
									CCharacter *pNewCharacter		= new CCharacter();
									pNewCharacter->m_oAttributes	= pCharacter->m_oAttributes;
									pNewCharacter->m_lCount			= (LONG)(nParagraphLen - nSymbolsCount);
									pCharacter->m_lCount			-= pNewCharacter->m_lCount;
									nSymbolsCount					+= pNewCharacter->m_lCount;
									pTextParagraph->m_Items.AddCharacter(pNewCharacter);
									break;
								}
							}
							if (nSymbolsCount < nParagraphLen)
							{
								if (pTextParagraph->m_Items.CountCharacters() != 0)
								{
									int nLastCharacterIndex	= pTextParagraph->m_Items.CountCharacters() - 1;
									CCharacter *pLastCharacter	= pTextParagraph->m_Items.GetCharacter(nLastCharacterIndex);
									pLastCharacter->m_lCount	+= (LONG)(nParagraphLen - nSymbolsCount);
								}
								else
								{
									CCharacter *pNewCharacter		= new CCharacter();
									size_t nLastCharacterIndex		= nCharactersCount - 1;
									CCharacter *pLastCharacter		= (*pArrayCFs)[nLastCharacterIndex];
									pNewCharacter->m_oAttributes	= pLastCharacter->m_oAttributes;
									pNewCharacter->m_lCount			= pTextParagraph->m_pBlockAttributes->m_lCount;									
									pTextParagraph->m_Items.AddCharacter(pNewCharacter);
								}
							}
							pTextParagraph->m_Items.Text(m_strText.Mid((int)nCurrentSymbolIndex, (int)pParagraph->m_lCount));
							nCurrentSymbolIndex			+= pParagraph->m_lCount;
						}
						if (nCurrentCharacter < nCharactersCount)
						{
							for (size_t i = nCurrentCharacter; i < nCharactersCount; ++i)
							{
								RELEASEOBJECT((*pArrayCFs)[i]);
							}
							pArrayCFs->RemoveAt(nCurrentCharacter, nCharactersCount - nCurrentCharacter);
						}
						m_bIsFormated = false;
						return TRUE;
					}

				private:
					void	DrawPage(CPage* page, IAVSRenderer* renderer, const float& completeness, const bool& transform = true, const int& block = -1) {
						if (m_bIsFo) {
							m_fWidthMm = page->GetWidthMm();
							m_fHeightMm = page->GetHeightMm();
							IWorker_OnNewPage(m_fWidthMm, m_fHeightMm);

							CBackgroundRect* pageRect = m_oBGPage.GetBackgroundRect2(0, 0, m_fWidthMm, m_fHeightMm);
							pageRect->Draw(renderer, completeness);
							RELEASEOBJECT(pageRect);

							if (block == -1)
								page->Draw(renderer, m_DocumentManager, completeness);
							else
								page->Draw(renderer, m_DocumentManager, block, completeness);
							IWorker_OnCompletePage();
						}
						else {
							#ifdef TME_DEBUG_VSXML
							m_fWidthMm = page->GetWidthMm();
							m_fHeightMm = page->GetHeightMm();
							IWorker_OnNewPage(m_fWidthMm, m_fHeightMm);
							#endif

							SetRenderer(renderer);
							if (transform)
								renderer->SetCommandParams(m_dAngle, m_oLayoutRect.X, m_oLayoutRect.Y, 
										m_oLayoutRect.Width, m_oLayoutRect.Height, m_lFlags);
							if (block == -1)
								page->Draw(renderer, m_DocumentManager, completeness);
							else
								page->Draw(renderer, m_DocumentManager, block, completeness);
							if (transform)
								renderer->SetCommandParams(0, -1, -1, -1, -1, 0);
							
							#ifdef TME_DEBUG_VSXML
							IWorker_OnCompletePage();
							#endif
						}
					}
					void	Progress(const float &fProgress)
					{
						if (fProgress > 0.0f)
						{
							IWorker_OnProgress(fProgress);
						}
						else
						{
							IWorker_OnProgress(m_DocumentManager.GetCompleteness());
						}
					}
					void	SendLinkerXml(IAVSRenderer *pRenderer)
					{
						BSTR bstrHyperlinkXml	= m_DocumentManager.m_Linker.GenerateXml();

						pRenderer->BeginCommand(c_nHyperlinkType);
						pRenderer->CommandDrawText(bstrHyperlinkXml, 0, 0, 0, 0, 0);
						pRenderer->EndCommand(c_nHyperlinkType);
					}
				protected:
					void	InitFontManager()
					{
						m_pFontManager = CGrapher::GenerateFontManager();
						//ADDREFINTERFACE(m_pFontManager);
					}

				};
			}
		}
	}
}