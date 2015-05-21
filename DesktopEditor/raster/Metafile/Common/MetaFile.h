#ifndef _METAFILE_COMMON_METAFILE_H
#define _METAFILE_COMMON_METAFILE_H

#include "MetaFileTypes.h"
#include "MetaFileUtils.h"
#include "MetaFileObjects.h"
#include "MetaFileClip.h"
#include "../../../fontengine/FontManager.h"

namespace MetaFile
{
	class IMetaFileBase
	{
	public:
		IMetaFileBase()
		{
			m_pBufferData = NULL;
			m_bError      = false;
			m_pOutput     = NULL;
			m_oStream.SetStream(NULL, 0);
		}
		virtual ~IMetaFileBase()
		{
			this->Close();
		}
		
		virtual void         PlayMetaFile() = 0;
		virtual void         ClearFile() {/*Нельзя делать чисто виртуальной, потому что вызывается в деструкторе*/}
		virtual TRect*       GetDCBounds() = 0;
		virtual double       GetPixelHeight() = 0;
		virtual double       GetPixelWidth() = 0;
		virtual int          GetTextColor() = 0;
 		virtual IFont*       GetFont() = 0;
		virtual IBrush*      GetBrush() = 0;
		virtual IPen*        GetPen() = 0;
		virtual unsigned int GetTextAlign() = 0;
		virtual unsigned int GetTextBgMode() = 0;
		virtual int          GetTextBgColor() = 0;
		virtual unsigned int GetFillMode() = 0;
		virtual TPointD      GetCurPos() = 0;
		virtual TXForm*      GetInverseTransform() = 0;
		virtual TXForm*      GetTransform() = 0;
		virtual unsigned int GetMiterLimit() = 0;
		virtual unsigned int GetRop2Mode() = 0;
		virtual IClip*       GetClip() = 0;
		virtual int          GetCharSpace() = 0;

		bool          OpenFromFile(const wchar_t* wsFilePath)
		{
			this->Close();

			NSFile::CFileBinary oFile;
			oFile.OpenFile(wsFilePath);
			int lFileSize = oFile.GetFileSize();

			m_pBufferData = new BYTE[lFileSize];
			if (!m_pBufferData)
				return false;

			DWORD lReadedSize;
			oFile.ReadFile(m_pBufferData, lFileSize, lReadedSize);

			m_oStream.SetStream(m_pBufferData, lFileSize);
			oFile.CloseFile();

			return true;
		}
		void          Close()
		{
			RELEASEARRAYOBJECTS(m_pBufferData);
			m_pOutput = NULL;
			m_oStream.SetStream(NULL, 0);
			m_bError = false;

			this->ClearFile();
		}
		void          Scan()
		{
			IOutputDevice* pOutput = m_pOutput;
			m_pOutput = NULL;
			PlayMetaFile();
			m_pOutput = pOutput;

			this->ClearFile();
		}
		CFontManager* GetFontManager()
		{
			return m_pFontManager;
		}
		void          SetFontManager(CFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;
		}
		void          SetOutputDevice(IOutputDevice* pOutput)
		{
			m_pOutput = pOutput;
		}
		void          SetError()
		{
			m_bError = true;
		}
		bool          CheckError()
		{
			return m_bError;
		}

	protected:

		CDataStream    m_oStream;
		IOutputDevice* m_pOutput;

	private:

		BYTE*          m_pBufferData;		
		CFontManager*  m_pFontManager;
		bool           m_bError;
	};
}

#endif //_METAFILE_COMMON_METAFILE_H
