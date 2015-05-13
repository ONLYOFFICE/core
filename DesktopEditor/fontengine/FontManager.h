#ifndef _BUILD_FONT_ENGINE_FONTMANAGER_H_
#define _BUILD_FONT_ENGINE_FONTMANAGER_H_

#include "FontFile.h"
#include <map>

class CFontSelectFormat;
class CFontInfo;
class CFontStream
{
private:
	int m_lRef;

public:
	BYTE*	m_pData;
	LONG	m_lSize;
    bool    m_bIsAttach;

public:
	CFontStream();
	virtual ~CFontStream();

	virtual int AddRef();
	virtual int Release();
	

public:
	virtual INT CreateFromFile(const std::wstring& strFileName, BYTE* pDataUse = NULL);
};

class CApplicationFontStreams
{
private:
	// этот мап нужно периодически опрашивать и удалять неиспользуемые стримы
	std::map<std::wstring, CFontStream*> m_mapStreams;
public:

	CApplicationFontStreams();
	~CApplicationFontStreams();

public:
	CFontStream* GetStream(const std::wstring& strFile);
	void CheckStreams(std::map<std::wstring, bool>& mapFiles);
	void Clear();
};

class CFontsCache
{
	friend class CFontManager;
	friend class CApplicationFonts;
private:
	std::map<std::string, CFontFile*>	m_mapFiles;
	CApplicationFontStreams*			m_pApplicationFontStreams;
    
public:
    CFontsCache()
    {
        m_pApplicationFontStreams = NULL;
    }
    ~CFontsCache()
    {
        Clear();
    }
    void Clear()
    {
        for (std::map<std::string, CFontFile*>::iterator iter = m_mapFiles.begin(); iter != m_mapFiles.end(); ++iter)
        {
            CFontFile* pFile = iter->second;
            RELEASEOBJECT(pFile);
        }
        m_mapFiles.clear();
    }

public:
	inline void SetStreams(CApplicationFontStreams* pStreams) { m_pApplicationFontStreams = pStreams; }
	CFontFile* LockFont(FT_Library library, const std::wstring& strFileName, const LONG& lFaceIndex, const double& dSize);	
};

class CApplicationFonts;
class CFontManager
{
	friend class CApplicationFonts;
private:
	int m_lRef;
public:
	FT_Library		m_pLibrary;
	
	CFontFile*		m_pFont;
	CGlyphString	m_oString;

	std::wstring	m_sName;

    INT	m_bUseDefaultFont;
    double	m_fCharSpacing;
    INT	m_bStringGID;

    int m_lUnits_Per_Em;
    int m_lAscender;
    int m_lDescender;
    int m_lLineHeight;

    int m_nLOAD_MODE;

	CApplicationFonts*	m_pApplication;
	CFontsCache*		m_pOwnerCache;

public:
	CFontManager();
	~CFontManager();

public:
	void AfterLoad();
	void Initialize();

	void SetOwnerCache(CFontsCache* pCache);

	double UpdateSize(const double& dOldSize, const double& dDpi, const double& dNewDpi);
	
	INT LoadString1(const std::wstring& wsBuffer, const float& fX, const float& fY);	
	INT LoadString2(const std::wstring& wsBuffer, const float& fX, const float& fY);
	INT LoadString3(const LONG& gid, const float& fX, const float& fY);
	INT LoadString3C(const LONG& gid, const float& fX, const float& fY);
	INT LoadString2C(const LONG& wsBuffer, const float& fX, const float& fY);

	int GetKerning(UINT unPrevGID, UINT unGID);
	INT GetUnderline(float *pfStartX, float *pfStartY, float *pfEndX, float *pfEndY, float *pfSize);

    TFontCacheSizes MeasureChar(const LONG& lUnicode);
	TBBox MeasureString();
	TBBox MeasureString2();
	
	INT GetNextChar2(TGlyph*& pGlyph, float& fX, float& fY);

    INT SetTextMatrix(const double& fA, const double& fB, const double& fC, const double& fD, const double& fE, const double& fF);
	INT SetTextMatrix2(const double& fA, const double& fB, const double& fC, const double& fD, const double& fE, const double& fF);
	void SetStringGID(const INT& bStringGID);
	void SetCharSpacing(const double &dCharSpacing);

	INT GetStringPath(ISimpleGraphicsPath* pPath);

	// addref/release
	virtual int AddRef();
	virtual int Release();

	CFontInfo* GetFontInfoByParams(CFontSelectFormat& oFormat);
	CArray<CFontInfo*> GetAllStylesByFontName(const std::wstring& strName);
	INT LoadFontByName(const std::wstring& sName, const double& dSize, const LONG& lStyle, const double& dDpiX, const double& dDpiY);
	INT LoadFontFromFile(const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY);
	INT LoadFontFromFile2(CFontsCache* pCache, const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY);
    void CloseFont();

	std::wstring GetFontType() const;
	unsigned int GetNameIndex(const std::wstring& wsName) const;
	
public:
	static CFontFile* LoadFontFile(FT_Library library, CFontStream* pStream, LONG lFaceIndex);
};

#endif // _BUILD_FONT_ENGINE_FONTMANAGER_H_
