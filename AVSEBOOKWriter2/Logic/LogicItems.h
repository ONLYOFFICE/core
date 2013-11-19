#pragma once
#include "../Writers/Format.h"

namespace NSEBook
{
	class CLogicTable : public ILogicItem
	{
	public:
		bool							m_bIsEmbeddedFonts;
		LONG 							m_lDstFormat;
		NSEBook::CFormat*				m_pFormat;
		NSCommon::CFontDstGenerator*	m_pFontsDst;
		NSCommon::CFontManager*			m_pFontManager;

		CString m_strTableXml;
	public:
		CLogicTable()
		{
			m_eType			= ILogicItem::elitTable;
			m_strTableXml	= _T("");
			m_pFontsDst		= NULL;
			m_pFontManager	= NULL;
			m_pFormat		= NULL;
			m_bIsEmbeddedFonts = false;
			m_lDstFormat	= 0;
		}
		~CLogicTable()
		{
		}

		virtual double GetPositionY()
		{
			return m_rcBounds.top;
		}
		virtual double GetPositionX()
		{
			return 0;
		}

		void LoadFromXml(CString strTable)
		{
			m_strTableXml = _T("");
			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlString(strTable))
			{
				NSCommon::CStringWriter oWriter;
				WriteTable(oNode, oWriter);
				m_strTableXml = oWriter.GetCString();
			}
		}

	protected:
		void WriteTable(XmlUtils::CXmlNode& oNode, NSCommon::CStringWriter& oWriter)
		{
			oWriter.WriteStringC(_T("<table style=\"border-collapse:collapse;width:auto;align:left;margin-left:0cm;margin-bottom:0cm\">"));

			XmlUtils::CXmlNode oNodeTable;
			if (oNode.GetNode(_T("fo:table-body"), oNodeTable))
			{
				XmlUtils::CXmlNodes oRows;
				if (oNodeTable.GetNodes(_T("fo:table-row"), oRows))
				{
					int nCount = oRows.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oRow;
						oRows.GetAt(i, oRow);
						WriteTR(oRow, oWriter);
					}
				}
			}

			oWriter.WriteStringC(_T("</table>"));
		}
		void WriteTR(XmlUtils::CXmlNode& oNode, NSCommon::CStringWriter& oWriter)
		{
			oWriter.WriteStringC(_T("<tr>"));

			XmlUtils::CXmlNodes oColomns;
			if (oNode.GetNodes(_T("fo:table-cell"), oColomns))
			{
				int nCount = oColomns.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oColomn;
					oColomns.GetAt(i, oColomn);

					CString strMergeV = oColomn.GetAttribute(_T("v-merge"));
					if (_T("continue") == strMergeV)
						continue;

					WriteTD(oColomn, oWriter);
				}
			}

			oWriter.WriteStringC(_T("</tr>"));
		}
		void WriteTD(XmlUtils::CXmlNode& oNode, NSCommon::CStringWriter& oWriter)
		{
			oWriter.WriteStringC(_T("<td valign=\"top\""));

			int colspan = oNode.ReadAttributeInt(_T("number-columns-spanned"), 1);
			int rowspan = oNode.ReadAttributeInt(_T("number-rows-spanned"), 1);

			if (1 != colspan)
			{
				CString strMem = _T("");
				strMem.Format(_T(" colspan=\"%d\""), colspan);
				oWriter.WriteStringC(strMem);
			}
			if (1 != rowspan)
			{
				CString strMem = _T("");
				strMem.Format(_T(" rowspan=\"%d\""), rowspan);
				oWriter.WriteStringC(strMem);
			}

			oWriter.WriteStringC(_T(" style=\""));

			oWriter.WriteStringC(_T("border:solid black 1.0pt;"));

			oWriter.WriteStringC(_T("\">"));

			XmlUtils::CXmlNodes oBlocks;
			if (oNode.GetNodes(_T("fo:block"), oBlocks))
			{
				int nCount = oBlocks.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oBlock;
					oBlocks.GetAt(i, oBlock);

					WriteBlock(oBlock, oWriter);
				}
			}

			oWriter.WriteStringC(_T("</td>"));
		}
		void WriteBlock(XmlUtils::CXmlNode& oNode, NSCommon::CStringWriter& oWriter)
		{
			CString strAlign = oNode.GetAttribute(_T("text-align"), _T("left"));
			oWriter.WriteStringC(_T("<p style=\"text-align:") + strAlign + _T(";\">"));
			
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetChilds(oChilds))
			{
				int nCount = oChilds.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oItem;
					oChilds.GetAt(i, oItem);

					CString strItem = oItem.GetName();

					if (_T("fo:inline") == strItem)
					{
						XmlUtils::CXmlNode oNodeStyle	= oItem.ReadNode(_T("style-complete"));
						XmlUtils::CXmlNode oNodeText	= oItem.ReadNode(_T("text"));

						NSEBook::CEbookParagraph::CSpan oSpan;
						oSpan.m_oText += oNodeText.GetTextExt();

						oSpan.m_oFont.Name		= oNodeStyle.GetAttribute(_T("font-family"));
						oSpan.m_oFont.Bold		= (_T("bold") == oNodeStyle.GetAttribute(_T("font-weight")));
						oSpan.m_oFont.Italic	= (_T("italic") == oNodeStyle.GetAttribute(_T("font-style")));
						
						CString strSize			= oNodeStyle.GetAttribute(_T("font-size"));
						oSpan.m_oFont.Size		= 0;
						if (2 < strSize.GetLength())
						{
							strSize.Delete(strSize.GetLength() - 2, 2);
							oSpan.m_oFont.Size = XmlUtils::GetDouble(strSize);
						}
						if (0 == oSpan.m_oFont.Size)
							oSpan.m_oFont.Size = 10;

						m_pFontsDst->AddFont(&oSpan.m_oFont, true, m_pFontManager);

						CString strStyle1 = _T("");
						CString strStyle2 = _T("");

						if (m_bIsEmbeddedFonts)
						{
							CString strName = oSpan.m_oFont.Name;
							strName.Replace(_T(" "), _T("_avs_space_"));
							
							strStyle1 = (_T("<span style=\"font-family:") + strName + _T(";"));
							if (oSpan.m_oFont.Bold)
								strStyle1 += _T("font-weight:bold;");
							else
								strStyle1 += _T("font-weight:normal;");

							if (oSpan.m_oFont.Italic)
								strStyle1 += _T("font-style:italic;\">");
							else
								strStyle1 += _T("font-style:normal;\">");

							strStyle2 = _T("</span>");
						}
						else
						{
							if (oSpan.m_oFont.Bold)
							{
								strStyle1 += _T("<b>");
								strStyle2 += _T("</b>");
							}
							if (oSpan.m_oFont.Italic)
							{
								strStyle1 += _T("<i>");
								strStyle2 = (_T("</i>") + strStyle2);
							}
							if (oSpan.m_oFont.Underline != 0) 
							{
								strStyle1 += _T("<u>");
								strStyle2 = (_T("</u>") + strStyle2);
							}
						}

						oWriter.WriteStringC(strStyle1);
						if (2 != m_lDstFormat)
						{
							oWriter.WriteTextXML(oSpan.m_oText);
						}
						else
						{
							oWriter.WriteTextHTML(oSpan.m_oText);
						}
						oWriter.WriteStringC(strStyle2);
					}
					else if (_T("fo:alternate-content") == strItem)
					{
						XmlUtils::CXmlNode node1;
						if (oItem.GetNode(_T("fo:property"), node1))
						{
							XmlUtils::CXmlNode node2;
							if (node1.GetNode(_T("fo:graphic"), node2))
							{
								XmlUtils::CXmlNode node3;
								if (node2.GetNode(_T("fo:source"), node3))
								{
									CString strFile = node3.GetAttribute(_T("dir"));
									strFile += _T("\\word\\");
									CString strName = node3.GetTextExt();
									strName.Replace(TCHAR('/'), TCHAR('\\'));
									
									strFile += strName;

									CFile oFile;
									HRESULT hr = oFile.OpenFile(strFile);
									oFile.CloseFile();

									if (S_OK == hr)
									{
										double _x = 0;
										double _y = 0;
										double _w = 10000;
										double _h = 10000;

										NSCommon::CImageInfo oInfo = m_pFormat->m_oImageManager.WriteImage(strFile, _x, _y, _w, _h);
										
										if (0 == m_lDstFormat)
										{
											CString strImage = _T("");
											strImage.Format(_T("<img src=\"media/image%d.%s\" alt=\"\"/>"), oInfo.m_lID, 
												(NSCommon::itJPG == oInfo.m_eType) ? _T("jpg") : _T("png"));
											oWriter.WriteStringC(strImage);
										}
										else if (1 == m_lDstFormat)
										{
											CString strImage = _T("");
											strImage.Format(_T("<image xlink:href=\"#image%d.%s\"/>"), oInfo.m_lID, 
												(NSCommon::itJPG == oInfo.m_eType) ? _T("jpg") : _T("png"));
											oWriter.WriteStringC(strImage);
										}
										else if (2 == m_lDstFormat)
										{
											CString strImage = _T("");
											strImage.Format(_T("<img recindex=\"%d\" align=\"baseline\" alt=\"\"/>"), oInfo.m_lID);
											oWriter.WriteStringC(strImage);
										}
									}
								}
							}
						}
					}
				}
			}

			oWriter.WriteStringC(_T("</p>"));
		}
	};

	class IGraphicCommand
	{
	public:
		enum EGraphicCommandType
		{
			egctPath	= 0,
			egctText	= 1
		};

	public:
		EGraphicCommandType m_eType;

	public:
		IGraphicCommand()
		{
			m_eType = egctPath;
		}
	};

	class CItemPath : public IGraphicCommand
	{
	private:
		class CPathMeta
		{
		private:
			enum PathCommand
			{
				// pathcommands		
				pcMoveTo				= 0,
				pcLineTo				= 1,
				pcCurveTo				= 2,
				pcClose					= 3
			};
		private:
			// сам метафайл
			BYTE* m_pBuffer;
			BYTE* m_pBufferMem;
			
			size_t m_lPosition;
			size_t m_lSize;

		public:
			CPathMeta()
			{
				Clear();
			}
			~CPathMeta()
			{
				RELEASEARRAYOBJECTS(m_pBuffer);
			}

		protected:

			AVSINLINE void Clear()
			{
				m_lSize		= 0;
				m_lPosition = 0;

				m_pBuffer = NULL;
			}

			AVSINLINE void CheckBufferSize(size_t lPlus)
			{
				if (NULL != m_pBuffer)
				{
					size_t nNewSize = m_lPosition + lPlus;

					if (nNewSize >= m_lSize)
					{
						while (nNewSize >= m_lSize)
						{
							m_lSize *= 2;
						}
						
						BYTE* pNew = new BYTE[m_lSize];
						memcpy(pNew, m_pBuffer, m_lPosition);

						RELEASEARRAYOBJECTS(m_pBuffer);
						m_pBuffer = pNew;
					}
				}
				else
				{
					m_lSize		= 1000;
					m_pBuffer	= new BYTE[m_lSize];

					CheckBufferSize(lPlus);
				}
			}

			AVSINLINE void WriteCommandType(const PathCommand& eType)
			{
				CheckBufferSize(sizeof(BYTE));

				*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
				m_lPosition += sizeof(BYTE);
			}
			inline void WriteDouble(const double& dValue)
			{
				CheckBufferSize(sizeof(double));

				*((double*)(m_pBuffer + m_lPosition)) = dValue; 
				m_lPosition += sizeof(double);
			}
			inline void WriteBYTE(const PathCommand& eType, const BYTE& lValue)
			{
				CheckBufferSize(2 * sizeof(BYTE));

				*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
				m_lPosition += sizeof(BYTE);

				*(m_pBuffer + m_lPosition)	= lValue; 
				m_lPosition += sizeof(BYTE);
			}
			inline void WriteLONG(const PathCommand& eType, const LONG& lValue)
			{
				CheckBufferSize(sizeof(BYTE) + sizeof(LONG));

				*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
				m_lPosition += sizeof(BYTE);

				*((LONG*)(m_pBuffer + m_lPosition))	= lValue; 
				m_lPosition += sizeof(LONG);
			}
			inline void WriteDouble(const PathCommand& eType, const double& dValue)
			{
				CheckBufferSize(sizeof(BYTE) + sizeof(double));

				*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
				m_lPosition += sizeof(BYTE);

				*((double*)(m_pBuffer + m_lPosition)) = dValue; 
				m_lPosition += sizeof(double);
			}
			inline void Write(const PathCommand& eCommand, const double& f1, const double& f2)
			{
				size_t lMem = sizeof(BYTE) + 2 * sizeof(double);

				CheckBufferSize(lMem);

				*(m_pBuffer + m_lPosition)			= (BYTE)eCommand;	m_lPosition += sizeof(BYTE);
				*((double*)(m_pBuffer + m_lPosition))= f1;				m_lPosition += sizeof(double);
				*((double*)(m_pBuffer + m_lPosition))= f2;				m_lPosition += sizeof(double);
			}

			inline void Write(const PathCommand& eCommand, const double& f1, const double& f2, const double& f3, const double& f4, const double& f5, const double& f6)
			{
				size_t lMem = sizeof(BYTE) + 6 * sizeof(double);

				CheckBufferSize(lMem);

				*(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += sizeof(BYTE);
				*((double*)(m_pBuffer + m_lPosition))= f1;		m_lPosition += sizeof(double);
				*((double*)(m_pBuffer + m_lPosition))= f2;		m_lPosition += sizeof(double);
				*((double*)(m_pBuffer + m_lPosition))= f3;		m_lPosition += sizeof(double);
				*((double*)(m_pBuffer + m_lPosition))= f4;		m_lPosition += sizeof(double);
				*((double*)(m_pBuffer + m_lPosition))= f5;		m_lPosition += sizeof(double);
				*((double*)(m_pBuffer + m_lPosition))= f6;		m_lPosition += sizeof(double);
			}
		public:
			
			// -------------------------------------------------------------------------------------
			AVSINLINE void _MoveTo(double fX, double fY)
			{
				Write(pcMoveTo, fX, fY);
			}
			AVSINLINE void _LineTo(double fX, double fY)
			{
				Write(pcLineTo, fX, fY);
			}
			AVSINLINE void _CurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
			{
				Write(pcCurveTo, fX1, fY1, fX2, fY2, fX3, fY3);
			}
			AVSINLINE void _Close()
			{
				WriteCommandType(pcClose);
			}
		public:
			AVSINLINE LONG ReadLONG()
			{
				LONG l = *((LONG*)m_pBufferMem);
				m_pBufferMem += sizeof(LONG);
				return l;
			}
			AVSINLINE BYTE ReadByte()
			{
				BYTE l = *m_pBufferMem;
				++m_pBufferMem;
				return l;
			}
			AVSINLINE double ReadDouble()
			{
				double l = *((double*)m_pBufferMem);
				m_pBufferMem += sizeof(double);
				return l;
			}
		public:
			BOOL Draw(IAVSRenderer* pRenderer)
			{
				if (NULL == pRenderer)
					return FALSE;

				// отрисовываем все объекты...
				m_pBufferMem = m_pBuffer;

				// здесь старая команда нужна для того, чтобы не посылать подряд хмлки с пеном, брашем...
				// (например выставился цвет и альфа - а надо послать только один (!!!) раз)
				PathCommand eCurCommand;

				HRESULT hRes = S_OK;
				while (m_lPosition > ((size_t)(m_pBufferMem - m_pBuffer)))
				{
					eCurCommand = (PathCommand)ReadByte();
					
					switch (eCurCommand)
					{
					case pcMoveTo:
						{
							double fX = ReadDouble();
							double fY = ReadDouble();

							hRes = pRenderer->PathCommandMoveTo(fX, fY);
							
							break;
						}
					case pcLineTo:
						{
							double fX = ReadDouble();
							double fY = ReadDouble();

							hRes = pRenderer->PathCommandLineTo(fX, fY);
							
							break;
						}
					case pcCurveTo:
						{
							double fX1 = ReadDouble();
							double fY1 = ReadDouble();
							double fX2 = ReadDouble();
							double fY2 = ReadDouble();
							double fX3 = ReadDouble();
							double fY3 = ReadDouble();

							hRes = pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
							
							break;
						}
					case pcClose:
						{
							hRes = pRenderer->PathCommandClose();
							
							break;
						}
					default:
						{
							// мало ли что тут произошло, уж лучше выйдем
							return FALSE;
						}
					};

					if (S_OK != hRes)
						break;
				}
				return (S_OK == hRes);
			}
		};
	public:
		NSStructures::CPen		m_oPen;
		NSStructures::CBrush	m_oBrush;

		LONG					m_lType;
		CDoubleRect				m_rcBounds;
		CPathMeta				m_oGeometry;

	public:
		CItemPath()
		{
			m_rcBounds.left		= 50000;
			m_rcBounds.right	= -1;
			m_rcBounds.top		= 50000;
			m_rcBounds.bottom	= -1;
		}
		~CItemPath()
		{
		}
	};
	class CItemText
	{
	public:
		NSStructures::CBrush	m_oBrush;
		NSStructures::CFont		m_oFont;
		_bstr_t					m_bsText;
		_bstr_t					m_bsGid;

	public:
		CItemText() : m_bsText(), m_bsGid()
		{
		}
		~CItemText()
		{
		}
	};

	class CLogicGraphic : public ILogicItem
	{
	public:
		CAtlList<IGraphicCommand*> m_arCommands;

	public:
		CLogicGraphic()
		{
			m_eType		= ILogicItem::elitGraphic;
		}
		~CLogicGraphic()
		{
		}

		virtual double GetPositionY()
		{
			return m_rcBounds.top;
		}
		virtual double GetPositionX()
		{
			return 0;
		}
	};
	class CLogicImage : public ILogicItem
	{
	public: 
		CImageInfo m_oImageInfo;

	public:
		CLogicImage()
		{
			m_oImageInfo.m_lID = -1;
			m_eType = ILogicItem::elitGraphic;
		}
		~CLogicImage()
		{
		}

		virtual double GetPositionY()
		{
			return m_rcBounds.top;
		}
		virtual double GetPositionX()
		{
			return m_rcBounds.left;
		}
	};

	class CLogicParagraph : public ILogicItem
	{
	public:
		CEbookParagraph	m_oParagraph;

	public:
		CLogicParagraph()
		{
			m_eType = elitTextParagraph;
		}
		CLogicParagraph(const CEbookParagraph& oSrc)
		{
			m_eType = elitTextParagraph;
			m_oParagraph = oSrc;
		}
		~CLogicParagraph()
		{
		}

	public:
		virtual double GetPositionY()
		{
			return m_rcBounds.top;
		}
		virtual double GetPositionX()
		{
			return 0;
		}
	};
	
	class ILogicPage
	{
	public:
		NSStructures::CPen*							m_pPen;
		NSStructures::CBrush*						m_pBrush;
		NSStructures::CFont*						m_pFont;
		NSStructures::CShadow*						m_pShadow;
		NSStructures::CEdgeText*					m_pEdgeText;

		NSCommon::CMatrix*							m_pTransform;
		AVSGraphics::IAVSGraphicSimpleComverter*	m_pSimpleGraphicsConverter;

		double										m_dWidth;
		double										m_dHeight;

		CImageManager								m_oManager;

		LONG										m_lCurrentCommand;
		CAtlList<ILogicItem*>						m_arItems;

		CImageManager*								m_pImageManager;

		NSCommon::CFontDstGenerator					m_oFontsGen;
		NSCommon::CFontManager						m_oFontManager;

		CImageInfo									m_oInfoWatermark;
		CDoubleRect									m_oRectWatermark;
		bool										m_bIsDumpWatermark;

	public:
		
		virtual void ConvertToEbookPage(NSEBook::CFormat& oFormat)										= 0;
		virtual void Clear()																			= 0;

		virtual void MoveTo(double& x, double& y)														= 0;
		virtual void LineTo(double& x, double& y)														= 0;
		virtual void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)	= 0;
		virtual void Close()																			= 0;
		virtual void BeginPath()																		= 0;
		virtual void EndPath()																			= 0;
		virtual void DrawPath(LONG lType)																= 0;

		virtual void WriteText(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset) = 0;

		virtual void WriteTable(NSEBook::ILogicItem* pItem)  = 0;
		virtual void BeginShape()				= 0;
		virtual void EndShape()					= 0;

		virtual void WriteImage(IUnknown* punkImage, double& x, double& y, double& width, double& height)	= 0;	
		virtual void WriteImage(CString strFilePath, double& x, double& y, double& width, double& height)	= 0;

		virtual void BeginPage()		= 0;
		virtual void EndPage()			= 0;
		virtual void CloseFile(NSEBook::CFormat& oFormat)		= 0;

		virtual void InitProp()			= 0;
		virtual void WritePagePreak()
		{
			if (0 == m_arItems.GetCount())
				return;

			ILogicItem* pItem = m_arItems.GetTail();
			if (NULL != pItem)
				pItem->m_bIsPageBreakAfter = true;
		}

	public:
		virtual void CheckWatermark(CFormat& oFormat)
		{
			if (m_bIsDumpWatermark && (-1 != m_oInfoWatermark.m_lID))
			{
				// записываем watermark
				CEbookGraphicObject* pObject	= new CEbookGraphicObject();
				pObject->m_oInfo				= m_oInfoWatermark;
				pObject->m_rcBounds				= m_oRectWatermark;

				if (0 == oFormat.m_listSections.GetCount())
				{
					// такого быть не может
					oFormat.m_listSections.AddTail();
				}
				CSection& oSection = oFormat.m_listSections.GetTail();
				oSection.m_arItems.AddTail(pObject);
			}
			m_bIsDumpWatermark = false;
		}
	};
}