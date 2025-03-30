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
#include "MetaFile.h"
#include "Common/MetaFileTypes.h"
#include "Common/MetaFileRenderer.h"
#include "../../raster/BgraFrame.h"
#include "../../graphics/pro/Graphics.h"

#ifdef METAFILE_SUPPORT_WMF_EMF
#include "Wmf/WmfInterpretator/CWmfInterpretatorSvg.h"
#include "Emf/EmfInterpretator/CEmfInterpretatorSvg.h"
#endif

namespace MetaFile
{
	IMetaFile* Create(NSFonts::IApplicationFonts *pAppFonts)
	{
		return new CMetaFile(pAppFonts);
	}

	/**
	 * @brief CMetaFile Constructor
	 * @param pAppFonts
	 *
	 * Create a font manager accordingly Applications Fonts for the
	 * appropriate metafile.
	 */
	CMetaFile::CMetaFile(NSFonts::IApplicationFonts *pAppFonts) : MetaFile::IMetaFile(pAppFonts)
	{
		m_pFontManager = NULL;
		m_pAppFonts = pAppFonts;

		if (pAppFonts)
		{
			m_pFontManager = pAppFonts->GenerateFontManager();
			NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
			pMeasurerCache->SetStreams(pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}

		m_lType  = 0;
	}

	/**
	 * @brief CMetaFile::get_FontManager
	 * @return Pointer of current Font Manager
	 */
	NSFonts::IFontManager* CMetaFile::get_FontManager()
	{
		return m_pFontManager;
	}

	/**
	 * @brief CMetaFile Destructor
	 *
	 * Close metafile and release memory, allocated for Font Manager
	 */
	CMetaFile::~CMetaFile()
	{
		Close();
		RELEASEINTERFACE(m_pFontManager);
	}

	/**
	 * @brief CMetaFile::ConvertToSvg
	 * @param unWidth - width of picture from metafile (0 - default)
	 * @param unHeight - height of picture from metafile (0 - default)
	 * @return string containing svg content
	 */
	std::wstring CMetaFile::ConvertToSvg(unsigned int unWidth, unsigned int unHeight)
	{

	#ifdef METAFILE_SUPPORT_WMF_EMF
		if (c_lMetaWmf == m_lType)
		{
			m_oWmfFile.SetOutputDevice(InterpretatorType::Svg, unWidth, unHeight);
			m_oWmfFile.PlayMetaFile();
			return ((CWmfInterpretatorSvg*)m_oWmfFile.GetWmfParser()->GetInterpretator())->GetFile();
		}
		else if (c_lMetaEmf == m_lType)
		{
			m_oEmfFile.SetOutputDevice(InterpretatorType::Svg, unWidth, unHeight);
			m_oEmfFile.PlayMetaFile();
			return ((CEmfInterpretatorSvg*)m_oEmfFile.GetEmfParser()->GetInterpretator())->GetFile();
		}
	#endif
		return L"";
	}

	/**
	 * @brief Methods for conversation in test examples
	 */
#ifdef METAFILE_SUPPORT_WMF_EMF
	/**
	 * @brief CMetaFile::ConvertToXml
	 * @param wsFilePath - path to the file being saving (must be .xml)
	 *
	 * Convert and save metafile data to .xml
	 */
	void CMetaFile::ConvertToXml(const wchar_t *wsFilePath)
	{
		if (NULL == wsFilePath)
			return;

		m_oEmfFile.SetOutputDevice(wsFilePath, InterpretatorType::XML);
		m_oEmfFile.PlayMetaFile();
	}

	/**
	 * @brief CMetaFile::LoadFromXmlFile
	 * @param wsFilePath - path to the source file (must be .xml)
	 * @return if correct reading - return true, elde - false
	 *
	 * Load meta file content from source .xml file.
	 * Remake Font Manager for metafile
	 */
	bool CMetaFile::LoadFromXmlFile(const wchar_t *wsFilePath)
	{
		m_pFontManager->ClearOwnerCache();

		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_WMF_EMF_XML
		if (m_oEmfFile.OpenFromXmlFile(wsFilePath) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}
			m_oEmfFile.Close();
		}
	#endif

		return false;
	}

	/**
	 * @brief CMetaFile::ConvertToEmf
	 * @param wsFilePath - path to the file being saving (must be .emf)
	 */
	void CMetaFile::ConvertToEmf(const wchar_t *wsFilePath)
	{
		if (m_lType != c_lMetaEmf || m_oEmfFile.GetEmfParser()->GetType() != EmfParserType::EmfxParser)
			return;

		m_oEmfFile.SetOutputDevice(wsFilePath, InterpretatorType::Emf);
		m_oEmfFile.PlayMetaFile();

		//TODO:: сохранение в *.emf файл
	}
#endif

	/**
	 * @brief CMetaFile::LoadFromFile
	 * @param wsFilePath - path to source file
	 * @return if correct reading - return true, elde - false
	 *
	 * Load from source file
	 * Remake Font Manager for metafile, for each picture
	 * Check file extansion (wmf, emf, svm, svg)
	 */
	bool CMetaFile::LoadFromFile(const wchar_t *wsFilePath)
	{
		m_pFontManager->ClearOwnerCache();

	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_WMF_EMF
		if (m_oWmfFile.OpenFromWmfFile(wsFilePath) == true)
		{
			m_oWmfFile.Scan();

			if (!m_oWmfFile.CheckError())
			{
				m_lType = c_lMetaWmf;
				return true;
			}
			m_oWmfFile.Close();
		}

		if (m_oEmfFile.OpenFromEmfFile(wsFilePath) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}
			m_oEmfFile.Close();
		}
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		if (m_oSvmFile.OpenFromFile(wsFilePath) == true)
		{
			m_oSvmFile.Scan();

			if (!m_oSvmFile.CheckError())
			{
				m_lType = c_lMetaSvm;
				return true;
			}

			m_oSvmFile.Close();
		}
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		if (m_oSvgFile.OpenFromFile(wsFilePath) == true)
		{
			m_lType = c_lMetaSvg;
			return true;
		}
	#endif

		return false;
	}

	/**
	 * @brief CMetaFile::LoadFromBuffer
	 * @param pBuffer - pointer of buffer whith metacontent
	 *  for example, the buffer obtained after reading the file
	 *   @code
	 *   	NSFile::CFileBinary file;
	 *  	file.OpenFile(L"file_name");
	 *  	DWORD file_size = file.GetFileSize();
	 *  	BYTE* data = new BYTE[file_size];
	 *  	file.ReadFile(data, file_size);
	 *   @endcode
	 * @param unSize - buffer size (size of file or readed size)
	 *   @code
	 *   	DWORD readed_size;
	 *   	file.ReadFile(data, file_size, readed_size);
	 *   @endcode
	 * @return if correct format load for extansion - return true,
	 *   else - false
	 *
	 * Load metafile content from buffer
	 * Remake Font Manager for metafile, for each picture
	 * Check type of content in buffer, appropriate extension (wmf, emf, svm, svg)
	 */
	bool CMetaFile::LoadFromBuffer(BYTE *pBuffer, unsigned int unSize)
	{
		if (NULL == pBuffer || 0 == unSize)
			return false;

		m_pFontManager->ClearOwnerCache();

	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_WMF_EMF
		if (m_oWmfFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_oWmfFile.Scan();

			if (!m_oWmfFile.CheckError())
			{
				m_lType = c_lMetaWmf;
				return true;
			}
			m_oWmfFile.Close();
		}

		if (m_oEmfFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}
			m_oEmfFile.Close();
		}
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		if (m_oSvmFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_oSvmFile.Scan();

			if (!m_oSvmFile.CheckError())
			{
				m_lType = c_lMetaSvm;
				return true;
			}

			m_oSvmFile.Close();
		}
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		if (m_oSvgFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_lType = c_lMetaSvg;
			return true;
		}
	#endif

		return false;
	}

	/**
	 * @brief CMetaFile::LoadFromString
	 * @param data - source string, containing svg metadata (.svg extension type)
	 * @return if correct read svg content - return true, else - false
	 *
	 * Load .svg content from wide string
	 * Remake Font Manager for metafile, for each picture
	 */
	bool CMetaFile::LoadFromString(const std::wstring& data)
	{
		m_pFontManager->ClearOwnerCache();
#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);

		if (m_oSvgFile.ReadFromWString(data) == true)
		{
			m_lType = c_lMetaSvg;
			return true;
		}
#endif
		return false;
	}

	/**
	 * @brief CMetaFile::SetTempDirectory
	 * @param dir - path to working directory
	 */
	void CMetaFile::SetTempDirectory(const std::wstring& dir)
	{
#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetWorkingDirectory(dir);
#endif
	}

	/**
	 * @brief CMetaFile::DrawOnRenderer
	 * @param pRenderer - class instance of CGraphicsRenderer, which will render
	 *  meta content
	 * @param dX - start coordinate for X axis
	 * @param dY - start coordinate for Y axis
	 * @param dWidth - width of picture from metafile
	 * @param dHeight - height of picture from metafile
	 * @param wsXmlFilePath - path to the file being saveing as xml commands from
	 * 	metafile (default - nullptr)
	 * @return if none render - return false, else - true
	 *
	 * Check type of metacontent
	 * Draw the meta file picture on renderer
	 */
	bool CMetaFile::DrawOnRenderer(IRenderer* pRenderer, double dX, double dY,
								   double dWidth, double dHeight, const wchar_t* wsXmlFilePath)
	{
		if (NULL == pRenderer)
			return false;

		pRenderer->BeginCommand(c_nImageType);

		switch (m_lType)
		{
		#ifdef METAFILE_SUPPORT_WMF_EMF
			case c_lMetaWmf:
			{
				CMetaFileRenderer oWmfOut(m_oWmfFile.GetWmfParser(), pRenderer, dX, dY, dWidth, dHeight);
				m_oWmfFile.SetOutputDevice((IOutputDevice*)&oWmfOut);
				m_oWmfFile.PlayMetaFile();
				break;
			}
			case c_lMetaEmf:
			{
				CMetaFileRenderer oEmfOut(m_oEmfFile.GetEmfParser(), pRenderer, dX, dY, dWidth, dHeight);
				if (wsXmlFilePath)
					m_oEmfFile.SetOutputDevice((IOutputDevice*)&oEmfOut, wsXmlFilePath);
				else
					m_oEmfFile.SetOutputDevice((IOutputDevice*)&oEmfOut);
				m_oEmfFile.PlayMetaFile();
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVM
			case c_lMetaSvm:
			{
				CMetaFileRenderer oSvmOut(&m_oSvmFile, pRenderer, dX, dY, dWidth, dHeight);
				m_oSvmFile.SetOutputDevice((IOutputDevice*)&oSvmOut);
				m_oSvmFile.PlayMetaFile();
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVG
			case c_lMetaSvg:
			{
				m_oSvgFile.Draw(pRenderer, dX, dY, dWidth, dHeight);
				break;
			}
		#endif
			default:
				break;
		}

		pRenderer->EndCommand(c_nImageType);
		return true;
	}

	/**
	 * @brief CMetaFile::Close
	 *
	 * Close each type file (type = 0)
	 */
	void CMetaFile::Close()
	{
	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.Close();
		m_oEmfFile.Close();
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.Close();
	#endif

		m_lType  = 0;
	}

	/**
	 * @brief CMetaFile::GetType
	 * @return type of metafile
	 * @enum
	 *	c_lMetaWmf = 0x01;
	 *  c_lMetaEmf = 0x02;
	 *  c_lMetaSvg = 0x04;
	 *  c_lMetaSvm = 0x05;
	 */
	int  CMetaFile::GetType()
	{
		return m_lType;
	}

	/**
	 * @brief CMetaFile::GetBounds
	 * @param pdX - pointer to saving X coordinate of bounds
	 * @param pdY - pointer to saving Y coordinate of bounds
	 * @param pdW - pointer to saving Width of bounds
	 * @param pdH - pointer to saving height of bounds
	 */
	void CMetaFile::GetBounds(double* pdX, double* pdY, double* pdW, double* pdH)
	{
		switch (m_lType)
		{
		#ifdef METAFILE_SUPPORT_WMF_EMF
			case c_lMetaWmf:
			{
				const TRectL& oRect{m_oWmfFile.GetBounds()};
				*pdX = oRect.Left;
				*pdY = oRect.Top;
				*pdW = oRect.Right  - oRect.Left;
				*pdH = oRect.Bottom - oRect.Top;
				break;
			}
			case c_lMetaEmf:
			{
				const TRectL& oRect{m_oEmfFile.GetBounds()};
				*pdX = oRect.Left;
				*pdY = oRect.Top;
				*pdW = oRect.Right  - oRect.Left;
				*pdH = oRect.Bottom - oRect.Top;
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVM
			case c_lMetaSvm:
			{
				const TRectL& oRect{m_oSvmFile.GetBounds()};
				*pdX = oRect.Left;
				*pdY = oRect.Top;
				*pdW = oRect.Right  - oRect.Left;
				*pdH = oRect.Bottom - oRect.Top;

				if (*pdW > 10000 || *pdH > 10000)
				{
					*pdW /= 10;
					*pdH /= 10;
				}
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVG
			case c_lMetaSvg:
			{
				m_oSvgFile.GetBounds(*pdX, *pdY, *pdW, *pdH);
				break;
			}
		#endif
			default:
			{
				*pdX = 0;
				*pdY = 0;
				*pdW = 0;
				*pdH = 0;
				break;
			}
		}

		if (*pdW < 0) *pdW = -*pdW;
		if (*pdH < 0) *pdH = -*pdH;
	}

	/**
	 * @brief CMetaFile::ConvertToRaster
	 * @param wsOutFilePath - path to the file being saving (must be raster graphics
	 *  like .bmp, .png, etc.)
	 * @param unFileType - type of raster file, see ENUM_CXIMAGE_FORMATS
	 * 	for example .bmp = 1, .png = 4
	 * @param unWidth - width of picture from metafile
	 * @param unHeight - height of picture from metafile (default -1)
	 * @param wsXmlOutFile(optional) - path to the file being saving metafile
	 *	commands to .xml (default nullptr)
	 *
	 * Create Graphics Renderer and Font Manager
	 * Draw metafile content on created renderer and save in raster graphics file
	 */
	void CMetaFile::ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight, const wchar_t* wsXmlOutFile)
	{
		if (nWidth == 0 || nHeight == 0)
			return;

		if (NULL == wsOutFilePath)
			return;

		if (wsXmlOutFile != NULL)
			m_oEmfFile.SetOutputDevice(NULL, wsXmlOutFile);

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		NSFonts::IFontManager* pFontManager = m_pAppFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		pGrRenderer->SetFontManager(pFontManager);

		if (-1 == nHeight)
		{
			double dX, dY, dW, dH;
			GetBounds(&dX, &dY, &dW, &dH);

			if (dW < 0)
				dW = -dW;
			if (dH < 0)
				dH = -dH;

			if (nWidth < 0) nWidth = (int)(dW * 96 / 25.4);
			nHeight = (int)((double)nWidth * dH / dW);
		}

		double dWidth  = 25.4 * nWidth / 96;				// Get the width and height from pixels to mm
		double dHeight = 25.4 * nHeight / 96;				// 96 - standart DPI for inch

		BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		if (!pBgraData)
		{
			double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

			nWidth = (int)(dKoef * nWidth);
			nHeight = (int)(dKoef * nHeight);

			dWidth  = 25.4 * nWidth / 96;
			dHeight = 25.4 * nHeight / 96;

			pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		}

		if (!pBgraData)
			return;

		unsigned int alfa = 0xffffff;
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			((unsigned int*)pBgraData)[i] = alfa;			// Set default tone (must be transparent and not white)
		}

		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->put_Width(dWidth);
		pGrRenderer->put_Height(dHeight);

		DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight, wsXmlOutFile);

		oFrame.SaveFile(wsOutFilePath, unFileType);
		oFrame.put_Data(NULL);

		RELEASEINTERFACE(pFontManager);
		RELEASEINTERFACE(pGrRenderer);

		if (pBgraData)
			free(pBgraData);
	}
}
