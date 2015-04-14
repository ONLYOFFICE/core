#pragma once

#include <vector>
#include <map>

#include <math.h>
#include "../../DesktopEditor/graphics/structures.h"
#include "../../DesktopEditor/graphics/GraphicsPath.h"

#if defined(_WIN32) || defined (_WIN64)
	#include <windows.h>
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../../DesktopEditor/common/ASCVariant.h"
    #include "../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#ifndef DBL_EPSILON
	#define DBL_EPSILON     2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */
#endif
// конвертация: миллиметры в координаты PDF

template <typename T>
inline double MMToPDFCoords( T tMM )
{
	return ( tMM / 25.4 ) * 72.0;
}
template <typename T>
inline double PDFCoordsToMM( T tX )
{
	return ( tX / 72.0 ) * 25.4;
}

template <typename T>
inline double PxToMM( T tX )
{
	return tX * 25.4 / 96;
}


// Следующие классы взяты из AVSOfficeSWFRenderer.
enum   ERendererState
{
	rendstateNone      = 0, // Все текущие команды закончены
	rendstateText      = 1, // Идет набивка текст
	rendstatePath      = 2, // Идет набивка пата
	rendstateClipPath  = 3, // Идет набивка пата для клипа
	rendstateClipText  = 4, // Идет набивка текста для клипа
	rendstateClipReset = 5, // Удаляем клип
	rendstateImage     = 6, // Обработка изображения
};

struct TMatrix
{
	float fA;
	float fB; 
	float fC;
	float fD;
	float fE;
	float fF;

	void Set(float fNewA, float fNewB, float fNewC, float fNewD, float fNewE, float fNewF)
	{
		fA = fNewA;
		fB = fNewB;
		fC = fNewC;
		fD = fNewD;
		fE = fNewE;
		fF = fNewF;
	}
};

struct TCommandParams
{
	double dAngle;
	double dLeft;
	double dTop;
	double dWidth;
	double dHeight;
	DWORD  nFlag;

	TMatrix oMatrix;
};

struct TCommandType
{
	int           nType; // Тип команды
	DWORD         nFlag; // Флаги, для данной команды          
	TCommandType *pPrev; // Указатель на предыдущую команду в стеке
};

class CMatrix
{
public:

	CMatrix()
	{
		Reset();
	}

	CMatrix(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		Set( dA, dB, dC, dD, dE, dF );
	}
	void Reset()
	{
		m_dA = 1;
		m_dB = 0;
		m_dC = 0;
		m_dD = 1;
		m_dE = 0;
		m_dF = 0;

		m_dRotate = 0;
		m_dSkewX  = 0;
		m_dSkewY  = 0;
		m_dScaleX = 1;
		m_dScaleY = 1;
	}

	void Set(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		m_dA = dA;
		m_dB = dB;
		m_dC = dC;
		m_dD = dD;
		m_dE = dE;
		m_dF = dF;

		Split2();
	}


	template <typename T> void DoTransform(T &tX, T &tY)
	{
		T tOldX = tX, tOldY = tY;

		tX = tOldX * m_dA + tOldY * m_dC + m_dE;
		tY = tOldX * m_dB + tOldY * m_dD + m_dF;
	}

	// В данном преобразовании не делается перенос
	template <typename T> void DoTransform2(T &tX, T &tY)
	{
		T tOldX = tX, tOldY = tY;

		tX = tOldX * m_dA + tOldY * m_dC;
		tY = tOldX * m_dB + tOldY * m_dD;
	}

	template <typename T> void DoITransform(T &tX, T &tY)
	{
		double dDet = m_dA * m_dD - m_dB * m_dC;
		if ( fabs( dDet ) < 0.001 )
			return;

		// Обратная матрица
		//             |     d         -b       0 |
		// det^(-1) *  |    -c          a       0 |
		//             | c*f - d*e  b*e - a*f   1 |

		double dIDet = 1 / dDet;
		double dIDet_2 = dIDet * dIDet;



		T tOldX = tX, tOldY = tY;

		tX = dIDet * ( tOldX * m_dD - tOldY * m_dC + m_dC * m_dF - m_dD * m_dE );
		tY = dIDet * (-tOldX * m_dB + tOldY * m_dA + m_dB * m_dE - m_dA * m_dF );
	}
	template <typename T> void DoITransform2(T &tX, T &tY)
	{
		double dDet = m_dA * m_dD - m_dB * m_dC;
		if ( fabs( dDet ) < 0.001 )
			return;

		// Обратная матрица
		//             | d  -b  0 |
		// det^(-1) *  |-c   a  0 |
		//             | 0   0  1 |

		double dIDet = 1 / dDet;

		T tOldX = tX, tOldY = tY;

		tX = dIDet * ( tOldX * m_dD - tOldY * m_dC );
		tY = dIDet * (-tOldX * m_dB + tOldY * m_dA );
	}
	double get_Rotate() const
	{
		return m_dRotate;
	}

	double get_SkewX() const
	{
		return m_dSkewX;
	}
	double get_SkewY() const
	{
		return m_dSkewY;
	}

	double get_ScaleX() const
	{
		return m_dScaleX;
	}

	double get_ScaleY() const
	{
		return m_dScaleY;
	}

	double get_A() const
	{
		return m_dA;
	}
	double get_B() const
	{
		return m_dB;
	}
	double get_C() const
	{
		return m_dC;
	}
	double get_D() const
	{
		return m_dD;
	}

	double get_E() const
	{
		return m_dE;
	}
	double get_F() const
	{
		return m_dF;
	}
private:

	void Split()
	{
		m_dScaleX = sqrt( m_dA * m_dA + m_dB * m_dB );
		m_dScaleY = sqrt( m_dC * m_dC + m_dD * m_dD );

		double dX_0 = 0; 
		double dY_0 = 1;

		DoTransform2( dX_0, dY_0 );

		double dX_1 = 1;
		double dY_1 = 0;

		DoTransform2( dX_1, dY_1 );

		m_dSkewX = atan2( dY_0, dX_0 ) - 1.57079632675;
		m_dSkewY = atan2( dY_1, dX_1 );

		m_dRotate = m_dSkewX;
	}

	void Split2()
	{
		// Данная функция работает специально под SWF::Matrix::ComputeMatrix
		m_dRotate = 0;
		m_dScaleX = m_dA;
		m_dSkewX  = m_dB;
		m_dSkewY  = m_dC;
		m_dScaleY = m_dD;
	}

private:

	double  m_dA;      //
	double  m_dB;      // Матрица преобразования
	double  m_dC;      // | a b 0 |
	double  m_dD;      // | c d 0 |
	double  m_dE;      // | e f 1 |
	double  m_dF;      // 

	double  m_dRotate; // Угол поворота

	double  m_dSkewX;  // Коэффициенты искожения
	double  m_dSkewY;  //

	double  m_dScaleX; // Коэффициенты сжатия/расширения
	double  m_dScaleY; // по осям oX,oY
};
class CRendererState
{
public:

	struct TMatrix
	{
		double dA;
		double dB;
		double dC;
		double dD;
		double dE;
		double dF;

		void Reset()
		{
			dA = 1; dB = 0;
			dC = 0; dD = 1;
			dE = 0; dF = 0;
		}

		void Set(double d_A, double d_B, double d_C, double d_D, double d_E, double d_F )
		{
			dA = d_A; dB = d_B;
			dC = d_C; dD = d_D;
			dE = d_E; dF = d_F;
		}
	};

public:

	CRendererState()
	{
		Reset();
	}

	~CRendererState()
	{
	}

	void Reset()
	{
		m_eState     = rendstateNone;
		m_ePrevState = rendstateNone;

		m_oMatrix.Reset();
		m_oPrevMatrix.Reset();
	}

	ERendererState get_State() const
	{
		return m_eState;
	}
	ERendererState get_PrevState() const
	{
		return m_ePrevState;
	}

	const TMatrix &get_PrevMatrix() const
	{
		return m_oPrevMatrix;
	}
	void set_NewState(ERendererState eState, CMatrix oMatrix)
	{
		m_ePrevState  = m_eState;
		m_oPrevMatrix = m_oMatrix;

		m_eState = eState;
		m_oMatrix.Set( oMatrix.get_A(), oMatrix.get_B(), oMatrix.get_C(), oMatrix.get_D(), oMatrix.get_E(), oMatrix.get_F() );
	}

	// Проверяем, изменилась ли матрица преобразований
	bool IsMatrixChanged()
	{
		if ( m_ePrevState == rendstateNone )
			return false;

		return ( fabs( m_oMatrix.dA - m_oPrevMatrix.dA  ) > 0.001 || fabs( m_oMatrix.dB - m_oPrevMatrix.dB  ) > 0.001 || fabs( m_oMatrix.dC - m_oPrevMatrix.dC  ) > 0.001 || fabs( m_oMatrix.dD - m_oPrevMatrix.dD  ) > 0.001 || fabs( m_oMatrix.dE - m_oPrevMatrix.dE  ) > 0.001 || fabs( m_oMatrix.dF - m_oPrevMatrix.dF  ) > 0.001 );
	}

	// Проверяем изменилась ли главная часть матрицы преобразований
	bool IsMatrixChanged2()
	{
		if ( m_ePrevState == rendstateNone )
			return false;

		return ( fabs( m_oMatrix.dA - m_oPrevMatrix.dA  ) > 0.001 || fabs( m_oMatrix.dB - m_oPrevMatrix.dB  ) > 0.001 || fabs( m_oMatrix.dC - m_oPrevMatrix.dC  ) > 0.001 || fabs( m_oMatrix.dD - m_oPrevMatrix.dD  ) > 0.001 );
	}

	void change_State(ERendererState eState)
	{
		m_eState = eState;
	}
private:

	ERendererState m_eState;      // Текущее состояние
	ERendererState m_ePrevState;  // Предыдущее состояние

	TMatrix        m_oMatrix;     // Текущая матрица
	TMatrix        m_oPrevMatrix; // Матрица предыдущего состояния

	// TO DO: Добавить, если надо, дополнительные параметры

};
class CContiniousText
{
public:

	struct TColor
	{
		unsigned char unR;
		unsigned char unG;
		unsigned char unB;
		unsigned char unA;
	};
	struct TMatrix
	{
		double dA;
		double dB;
		double dC; 
		double dD;
		double dE;
		double dF;

		void Reset()
		{
			dA = 1; dB = 0;
			dC = 0; dD = 1;
			dE = 0; dF = 0;
		}
		void Set(double d_A, double d_B, double d_C, double d_D, double d_E, double d_F)
		{
			dA = d_A; dB = d_B;
			dC = d_C; dD = d_D;
			dE = d_E; dF = d_F;
		}
	};

	struct TText
	{
		std::wstring  sText;
		std::wstring  sUnicodeText;

		float fX;
		float fY;
		float fWidth;
		float fHeight;
		float fBaseLineOffset;
		bool  bStringGID;
	};


public:

	CContiniousText()
	{
		Reset();
	}

	~CContiniousText()
	{
		Reset();
	}

	void Reset()
	{
        m_bFirstEntry = true;
		m_fX          = 0;
		m_fY          = 0;
		m_oMatrix.Reset();

		m_arrTexts   .clear();
		m_arrFonts   .clear();
		m_arrColors  .clear();
		m_arrMatrixes.clear();
		m_arrCharSpacing.clear();
	}
	void Add(CString sText, CString sUnicodeText, float fX, float fY, float fWidth, float fHeight, float fLineOffset, bool bStringGID, NSStructures::CFont oFont, NSStructures::CBrush oBrush, CMatrix oMatrix, double dCharSpacing)
	{
		TText oText;
		m_arrTexts.push_back( oText );

		double dX          = MMToPDFCoords( fX          );
		double dY          = MMToPDFCoords( fY          );
		double dWidth	   = MMToPDFCoords( fWidth      );
		double dHeight     = MMToPDFCoords( fHeight     );
		double dLineOffset = MMToPDFCoords( fLineOffset );

		int nLast = m_arrTexts.size() - 1;
		m_arrTexts[nLast].sText				= sText;
		m_arrTexts[nLast].sUnicodeText		= sUnicodeText;
		m_arrTexts[nLast].fX				= dX;
		m_arrTexts[nLast].fY				= dY;
		m_arrTexts[nLast].fWidth			= dWidth;
		m_arrTexts[nLast].fHeight			= dHeight;
		m_arrTexts[nLast].fBaseLineOffset	= dLineOffset;
		m_arrTexts[nLast].bStringGID		= bStringGID;

		TMatrix oNewMatrix;
		oNewMatrix.Set( oMatrix.get_A(), oMatrix.get_B(), oMatrix.get_C(), oMatrix.get_D(), oMatrix.get_E(), oMatrix.get_F() );

		TColor oColor;
		oColor.unR = (unsigned char) ( ( oBrush.Color1 & 0x000000FF )       );
		oColor.unG = (unsigned char) ( ( oBrush.Color1 & 0x0000FF00 ) >> 8  );
		oColor.unB = (unsigned char) ( ( oBrush.Color1 & 0x00FF0000 ) >> 16 );
		oColor.unA = (unsigned char) ( oBrush.Alpha1 );

		m_arrColors     .push_back( oColor       );
		m_arrFonts      .push_back( oFont        );
		m_arrMatrixes   .push_back( oNewMatrix   );
		m_arrCharSpacing.push_back( dCharSpacing );

		if ( m_bFirstEntry )
		{
            m_bFirstEntry = false;

			m_fX = dX;
			m_fY = dY;
			m_oMatrix = oMatrix;
		}
	}

	int  GetSize()
	{
		int nSize = m_arrTexts.size();
		if ( nSize == m_arrMatrixes.size() && nSize == m_arrFonts.size() && nSize == m_arrCharSpacing.size() && nSize == m_arrColors.size() )
			return nSize;

		return -1;
	}
	void LeaveLast()
	{
		// Оставляем последнюю запись. Это применяется, когда у последней
		// записи новая матрица преобразования.

		int nSizeNew = m_arrTexts.size() - 1;

		if (nSizeNew >0)
		{
			m_arrTexts.erase		(m_arrTexts.begin(),		m_arrTexts.begin()		+ nSizeNew);
			m_arrFonts.erase		(m_arrFonts.begin(),		m_arrFonts.begin()		+ nSizeNew);
			m_arrColors.erase		(m_arrColors.begin(),		m_arrColors.begin()		+ nSizeNew);
			m_arrMatrixes.erase		(m_arrMatrixes.begin(),		m_arrMatrixes.begin()	+ nSizeNew);
			m_arrCharSpacing.erase	(m_arrCharSpacing.begin(),	m_arrCharSpacing.begin() + nSizeNew);
		}
		if ( 1 == m_arrTexts.size() )
		{
            m_bFirstEntry = false;

			m_fX = m_arrTexts[0].fX;
			m_fY = m_arrTexts[0].fY;

			m_oMatrix.Set( m_arrMatrixes[0].dA, m_arrMatrixes[0].dB, m_arrMatrixes[0].dC, m_arrMatrixes[0].dD, m_arrMatrixes[0].dE, m_arrMatrixes[0].dF );
		}
		else
		{
            m_bFirstEntry = true;

			m_fX = 0;
			m_fY = 0;

			m_oMatrix.Reset();
		}

	}

	const CMatrix             &get_Matrix () const
	{
		return m_oMatrix;
	}
	const float               &get_X() const
	{
		return m_fX;
	}

	const float               &get_Y() const
	{
		return m_fY;
	}

	const TText  *get_Text   (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.size() )
			return &m_arrTexts[nIndex];

		return NULL;
	}

	const NSStructures::CFont *get_Font   (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.size() )
			return &m_arrFonts[nIndex];

		return NULL;
	}

	const TColor              *get_Color  (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.size() )
			return &m_arrColors[nIndex];

		return NULL;
	}
	const double               get_CharSp (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.size() )
			return m_arrCharSpacing[nIndex];

		return 0;
	}
private:

    bool                              m_bFirstEntry;

	float                             m_fX;             // Координаты первого текста 
	float                             m_fY;             //
	CMatrix                           m_oMatrix;        // Матрица первого текста

	std::vector<TText              > m_arrTexts;       // Список текстов
	std::vector<NSStructures::CFont> m_arrFonts;       // Список шрифтов к текстам
	std::vector<TColor             > m_arrColors;      // Список текстовых цветов
	std::vector<TMatrix            > m_arrMatrixes;    // Список матриц к текстам ( в матрицах изменяться должно только смещение )
	std::vector<double             > m_arrCharSpacing; // Межсимвольный интервал
};
class CFontLoader
{
public:
	CFontLoader()
	{
	}
	~CFontLoader()
	{
	}
	void Add(std::wstring sFilePath, std::wstring sFontName, std::wstring sEncodingName, std::wstring sToUnicodeName )
	{
		m_arrFilePath.push_back( sFilePath );
		m_arrFontName.push_back( sFontName );
		m_arrEncodingName.push_back( sEncodingName );
		m_arrToUnicodeName.push_back( sToUnicodeName );

		m_mapPathMap[sFilePath] =  m_arrFilePath.size() - 1 ;
	}
    bool Find(std::wstring sFilePath, std::wstring *psFontName, std::wstring *psEncodingName, std::wstring *psToUnicodeName)
	{
		std::map<std::wstring, int>::iterator pEl = m_mapPathMap.find( sFilePath );

		if ( m_mapPathMap.end() != pEl )
		{
			int nIndex			= pEl->second;
			*psFontName			= m_arrFontName[nIndex];
			*psEncodingName		= m_arrEncodingName[nIndex];
			*psToUnicodeName	= m_arrToUnicodeName[nIndex];
            return true;
		}

        return false;
	}

private:

	std::map<std::wstring, int>		m_mapPathMap;
	
	std::vector<std::wstring>		m_arrFilePath;
	std::vector<std::wstring>		m_arrFontName;
	std::vector<std::wstring>		m_arrEncodingName;
	std::vector<std::wstring>		m_arrToUnicodeName;
};
class CFindFonts
{
public:
	CFindFonts()
	{
		m_pBoldOut   = NULL;
		m_pItalicOut = NULL;

		m_pBoldIn    = NULL;
		m_pItalicIn  = NULL;

		m_nCount = 0;
		m_nSize  = 0;
	}
	~CFindFonts()
	{
		m_arrFontName.clear();
		m_arrFontFile.clear();

		if ( m_pBoldOut )
		{
			free( m_pBoldOut );
			m_pBoldOut = NULL;
		}

		if ( m_pItalicOut )
		{
			free( m_pItalicOut );
			m_pItalicOut = NULL;
		}

		if ( m_pBoldIn )
		{
			free( m_pBoldIn );
			m_pBoldIn = NULL;
		}

		if ( m_pItalicIn )
		{
			free( m_pItalicIn );
			m_pItalicIn = NULL;
		}

		m_nCount = 0;
		m_nSize  = 0;
	}

    void Add(std::wstring sFontName, std::wstring sFontFile, bool bBoldIn, bool bItalicIn, bool bBoldOut, bool bItalicOut)
	{
		Resize(1);
		
		m_arrFontName.push_back(sFontName);
		m_arrFontFile.push_back(sFontFile);

		m_pBoldIn[m_nCount]    = bBoldIn;
		m_pItalicIn[m_nCount]  = bItalicIn;

		m_pBoldOut[m_nCount]   = bBoldOut;
		m_pItalicOut[m_nCount] = bItalicOut;

		++m_nCount;
		return;
	}

    bool Find(std::wstring sFontName, std::wstring *pbsFontFile, bool *pbBold, bool *pbItalic)
	{
		for ( int nIndex = 0; nIndex < m_nCount; ++nIndex )
		{
			if ( sFontName == m_arrFontName[nIndex] && *pbBold == m_pBoldIn[nIndex] && *pbItalic == m_pItalicIn[nIndex] )
			{
				*pbsFontFile = m_arrFontFile[nIndex];

				*pbBold   = m_pBoldOut[nIndex];
				*pbItalic = m_pItalicOut[nIndex];

                return true;
			}
		}

        return false;
	}

private:

	void Resize(int nCount)
	{
		if ( m_nCount + nCount > m_nSize ) 
		{
			if ( m_nSize == 0 ) 
			{
				m_nSize = 32;
			}
			while ( m_nSize < m_nCount + nCount ) 
			{
				m_nSize *= 2;
			}
            m_pBoldOut   = (bool *)realloc( m_pBoldOut,   m_nSize * sizeof(bool));
            m_pItalicOut = (bool *)realloc( m_pItalicOut, m_nSize * sizeof(bool));

            m_pBoldIn    = (bool *)realloc( m_pBoldIn,    m_nSize * sizeof(bool));
            m_pItalicIn  = (bool *)realloc( m_pItalicIn,  m_nSize * sizeof(bool));
		}

	}

private:

	std::vector<std::wstring>	m_arrFontName;
	std::vector<std::wstring>	m_arrFontFile;

    bool *m_pBoldIn;
    bool *m_pItalicIn;

    bool *m_pBoldOut;
    bool *m_pItalicOut;

	int   m_nCount;
	int   m_nSize;
};
//class CGdiPath
//{
//public:
//
//	static const int c_nTextureBrush = 3008;
//
//public: 
//	CGdiPath()
//	{
//		m_pPath = NULL;
//	}
//	~CGdiPath()
//	{
//		Delete();
//	}
//
//	void Create()
//	{
//		if ( m_pPath )
//			Delete();
//		m_pPath = new Gdiplus::GraphicsPath(Gdiplus::FillModeWinding);
//	}
//	void Delete()
//	{
//		RELEASEOBJECT( m_pPath );
//	}
//
//	void GetBounds(float *pfX, float *pfY, float *pfWidth, float *pfHeight)
//	{
//		if ( !CheckValidate() )
//			return;
//
//		Gdiplus::RectF oPathBounds;
//		m_pPath->GetBounds( &oPathBounds );
//
//		*pfX      = oPathBounds.GetLeft();
//		*pfY      = oPathBounds.GetTop();
//		*pfWidth  = oPathBounds.GetRight() - oPathBounds.GetLeft();
//		*pfHeight = oPathBounds.GetBottom() - oPathBounds.GetTop();
//	}
//	// Функции для составления Path (так же как и в IAVSOfficeRendererTemplate)
//	inline STDMETHOD(MoveTo)(float fX, float fY)
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		m_pPath->StartFigure();
//		//RELEASEOBJECT(m_pPath);
//		//m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);
//
//		m_pPath->AddLine( fX, fY, fX, fY );
//		return S_OK;
//	}
//	inline STDMETHOD(LineTo)(float fX, float fY)
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		Gdiplus::PointF pointCur( 0, 0 );
//		CheckLastPoint( pointCur );
//		m_pPath->AddLine( pointCur.X, pointCur.Y, fX, fY );
//
//		return S_OK;
//	}
//	inline STDMETHOD(LinesTo)(SAFEARRAY** ppPoints)
//	{
//		if ( !CheckValidate() || ( NULL == ppPoints ) || ( NULL == *ppPoints ) )
//			return S_FALSE;
//
//		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
//		lCount /= 2;
//
//		double* pPoints = (double*)((*ppPoints)->pvData); 
//
//		CSimpleArray<Gdiplus::PointF> arPoints;
//
//		for (int nIndex = 0; nIndex < lCount; ++nIndex)
//		{
//			Gdiplus::PointF oPoint(*pPoints, *(pPoints + 1));
//			pPoints += 2;
//			arPoints.Add(oPoint);
//		}
//
//		if ( 1 < lCount )
//		{
//			m_pPath->AddLines(arPoints.m_aT, lCount);
//		}
//
//		return S_OK;
//	}
//
//	inline STDMETHOD(CurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		Gdiplus::PointF pointCur(0, 0);
//
//		CheckLastPoint(pointCur);
//
//		CSimpleArray<Gdiplus::PointF> arPoints;
//
//		arPoints.Add(pointCur);
//
//		pointCur.X = fX1; pointCur.Y = fY1;
//		arPoints.Add(pointCur);
//		pointCur.X = fX2; pointCur.Y = fY2;
//		arPoints.Add(pointCur);
//		pointCur.X = fX3; pointCur.Y = fY3;
//		arPoints.Add(pointCur);
//
//		//m_pPath->AddCurve(arPoints.m_aT, 4);
//		m_pPath->AddBeziers(arPoints.m_aT, 4);
//
//		return S_OK;
//	}
//	inline STDMETHOD(CurvesTo)(SAFEARRAY** ppPoints)
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
//		lCount /= 2;
//
//		double* pPoints = (double*)((*ppPoints)->pvData); 
//
//		CSimpleArray<Gdiplus::PointF> arPoints;
//
//		for (int nIndex = 0; nIndex < lCount; ++nIndex)
//		{
//			Gdiplus::PointF oPoint(*pPoints, *(pPoints + 1));
//			pPoints += 2;
//			arPoints.Add(oPoint);
//		}
//
//		int nSize = arPoints.GetSize();
//		if (1 < nSize)
//		{
//			//m_pPath->AddCurve(arPoints.m_aT, nSize);
//			m_pPath->AddBeziers(arPoints.m_aT, nSize);
//		}
//
//		return S_OK;
//	}
//	inline STDMETHOD(ArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		m_pPath->AddArc(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
//		return S_OK;
//	}
//	inline STDMETHOD(Close)()
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		m_pPath->CloseFigure();
//		return S_OK;
//	}
//	inline STDMETHOD(End)()
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		m_pPath->Reset();
//		return S_OK;
//	}
//
//	inline STDMETHOD(Start)()
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		m_pPath->StartFigure();
//		return S_OK;
//	}
//	inline STDMETHOD(GetCurrentPoint)(float* fX, float* fY)
//	{
//		if ( !CheckValidate() )
//			return S_FALSE;
//
//		if ( ( NULL == fX ) || ( NULL == fY ) )
//			return S_FALSE;
//
//		Gdiplus::PointF oPoint(0, 0);
//		CheckLastPoint(oPoint);
//
//		*fX = oPoint.X;
//		*fY = oPoint.Y;
//
//		return S_OK;
//	}
//
//private:
//
//	inline bool CheckValidate()
//	{
//		return ( NULL != m_pPath );
//	}
//	void CheckLastPoint(Gdiplus::PointF& pointCur)
//	{
//		if ( !CheckValidate() )
//			return;
//
//		int nCount = m_pPath->GetPointCount();
//
//		if ( 0 < nCount )
//		{
//			Gdiplus::PointF* pPoints = new Gdiplus::PointF[nCount];
//			m_pPath->GetPathPoints( pPoints, nCount );
//
//			pointCur.X = pPoints[nCount - 1].X;
//			pointCur.Y = pPoints[nCount - 1].Y;
//
//			delete []pPoints;
//		}
//	}
//
//private:
//
//	Gdiplus::GraphicsPath *m_pPath;
//};

//
//struct ImageUtils
//{
//	static inline IUnknown* LoadImage(CString sFilePath)
//	{
//#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
//		OfficeCore::IImageGdipFilePtr pImageFile;
//		pImageFile.CreateInstance(OfficeCore::CLSID_CImageGdipFile);
//
//		BSTR filename = sFilePath.AllocSysString();
//		pImageFile->OpenFile(filename);
//		SysFreeString(filename);
//
//		IUnknown* punkFrame = NULL;
//		pImageFile->get_Frame(&punkFrame);
//		return punkFrame;
//#else
//		ImageStudio::IImageTransforms* pTransform = NULL;
//		if (FAILED(CoCreateInstance(__uuidof(ImageStudio::ImageTransforms), NULL, CLSCTX_INPROC_SERVER, __uuidof(ImageStudio::IImageTransforms), (void**)(&pTransform))))
//			return NULL;
//
//		if (!pTransform)
//			return NULL;
//
//		sFilePath.Replace( _T("\\\\"), _T("\\") );
//
//		CFileDownloader* loader = NULL;
//		if (CFileDownloader::IsNeedDownload(sFilePath))
//		{
//			loader = new CFileDownloader(sFilePath, true);
//			if (loader)
//			{
//				//loader->StartWork(1);
//
//				//while(loader->IsRunned())
//				//{
//				//	Sleep(10);
//				//}
//
//				if (loader->IsFileDownloaded())
//				{
//					sFilePath = loader->GetFilePath();
//				}
//			}
//		}
//
//		CString sXml(L"<transforms><ImageFile-LoadImage sourcepath=\"");
//		sXml += sFilePath;
//		sXml += L"\"></ImageFile-LoadImage></transforms>";
//
//		if (!pTransform->SetXml(sXML))
//		{
//			RELEASEINTERFACE(pTransform);
//			RELEASEOBJECT(loader);
//			return NULL;
//		}
//
//		SysFreeString(bsXML);
//
//		if (!pTransform->Transform())
//		{
//			RELEASEINTERFACE(pTransform);
//			RELEASEOBJECT(loader);
//			return NULL;
//		}
//
//		VARIANT vImage;
//		pTransform->GetResult(0, &vImage);
//
//		RELEASEINTERFACE(pTransform);
//		RELEASEOBJECT(loader);
//
//		if (VT_UNKNOWN != vImage.vt)
//			return NULL;
//
//		return vImage.punkVal;
//#endif
//	}
//};

//	text utils

struct ParseUtils
{
    static inline bool DoubleValues (const CString& src, std::vector<double>& numbers)
	{
		CString number	=	_T("");			
		int length		=	src.GetLength();

		for (int i = 0; i < length; ++i)
		{
			if (L'-' == src [i])
			{
				if (i > 1)
				{
					if (L'e' == src [i - 1])
					{
						number.AppendChar(src[i]);
						continue;
					}
				}

				if (number.GetLength())
					numbers.push_back(_tstof(number));

				number	=	_T("");
			}

			if (isdigit(src[i]) || (src[i] == L'.') || (src[i] == L'-') || (src[i] == 'e'))
			{
				number.AppendChar(src[i]);
				continue;
			}	

			if (number.GetLength())
				numbers.push_back(_tstof(number));

			number	=	_T("");
		}

		if (number.GetLength ())
			numbers.push_back(_tstof(number));

		return (0 != numbers.size());
	}
};

static bool WriteFile_ (const CString& sTempLogo, const BYTE *cpLogo1, const size_t nSize = 0)
{
	const size_t nCount = (0 == nSize) ? strlen ((const char*) cpLogo1) : nSize;
	CFile  oFile;
	if (oFile.CreateFile(sTempLogo) == S_OK)
	{
		oFile.WriteFile((void*)cpLogo1,nCount);
		oFile.CloseFile();
		return true;
	}
	return false;
}
