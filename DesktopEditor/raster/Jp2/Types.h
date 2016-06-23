#pragma once
#include "Reader.h"
#include "../../graphics/Defines.h"
#include <algorithm>

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Constant definitions
	//-------------------------------------------------------------------------------------------------------------------------------

#define MAX_PATH_LEN 4096 // Максимальная длина для имен файлов 

#define J2K_MAXRLVLS 33					    // Максимальное число уровней разложения
#define J2K_MAXBANDS (3 * J2K_MAXRLVLS - 2)	// Максимальное число обастей, связанных с уровнями разложения

#define J2K_MAXSSFTR 2    // Max SubSampling Factor (Значения больше 2 могут привести к ошибке)

	void* Malloc(size_t nSize);

	template<typename T>
	void  Free(T*& pMem)
	{
		if (NULL != pMem)
		{
			free(pMem);
			pMem = NULL;
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Enum definitions
	//-------------------------------------------------------------------------------------------------------------------------------

	// Progression order
	typedef enum EProgressionOrder
	{
		poUnknown = -1, // Неизвестный порядок
		poLRCP    =  0, // layer-resolution-component-precinct
		poRLCP    =  1, // resolution-layer-component-precinct
		poRPCL    =  2, // resolution-precinct-component-layer
		poPCRL    =  3, // precinct-component-resolution-layer
		poCPRL    =  4  // component-precinct-resolution-layer
	} ProgressionOrder;

	// Поддерживаемые цветовые пространства
	typedef enum EColorSpace
	{
		csUnknown = -1, // цветовое пространство неопределенно
		csRGB     =  1, // sRGB
		csGray    =  2, // Grayscale
		csYUV     =  3  // YUV
	} ColorSpace;

	// Поддерживаемые форматы
	typedef enum ECodecFormat
	{
		codecUnknown = -1, // неизвестный кодек
		codecJ2K     =  0, // JPEG-2000 codestream : чтение/запись
		codecJPT     =  1, // JPT-stream (JPEG 2000, JPIP) : только чтение
		codecJP2     =  2, // JPEG-2000 file format (Jp2-wrapper) : чтение/запись
		codecMj2     =  3, // Motion JPEG-2000
	} CodecFormat;

	// Ограничения по декодированию Jpeg2000-Codestream
	typedef enum ELimitDecoding
	{
		declimNoLimitation      = 0, // Нет ограничений при декодировании. Декодируем Jpeg2000-codestream полностью
		declimLimitToMainHeader = 1	 // Декодируем только Main Header
	} LimitDecoding;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Event (Обработка сообщений)
	//-------------------------------------------------------------------------------------------------------------------------------

#define EVT_ERROR	1 // Error
#define EVT_WARNING	2 // Warning
#define EVT_INFO	4 // Debug event

#define MSG_SIZE  512 // Максимальный размер сообщения

#define JP2_ERROR_NO_ERROR           0x0000
#define JP2_ERROR_NOT_ENOUGH_MEMORY  0x0001


	//-------------------------------------------------------------------------------------------------------------------------------
	// Параметры кодека (компрессии/декомпрессии)
	//-------------------------------------------------------------------------------------------------------------------------------

	// Progression order changes (POC) (Annex A.6.6 Table A-32)
	typedef struct TPOC
	{
		int              nRSpoc;         // Resolution index for the start of a progression
		int              nCSpoc;         // Component index for the start of a progression.
		int              nLYEpoc;        // Layer index for the end of a progression
		int              nREpoc;         // Resolution index for the end of a progression
		int              nCEpoc;         // Component index for the end of a progression
		ProgressionOrder ePpoc;          // Progression order.
		int              nTile;
		char             sProgOrder[4];
	} POC;

	// Параметры компрессии
	typedef struct TEncoderParams
	{
		bool             bTileSizeOn;          // False: размеры тайлов не указаны, true: размеры тайлов указаны
		int              nXTOsiz;              // XTOsiz
		int              nYTOsiz;              // YTOsiz
		int              nXTsiz;               // XTsiz
		int              nYTsiz;               // YTsiz
		int              nDistoAlloc;          // allocation by rate/distortion
		int              nFixedAlloc;          // allocation by fixed layer
		int              nFixedQuality;        // fixed_quality
		int             *pMatrix;              // fixed layer
		char            *sComment;             // Комментарий
		int              nCodingStyle;         // CodingStyle
		ProgressionOrder eProgOrder;           // Progression order (default LRCP)
		POC              aoPOC[32];            // Progression order changes
		int              nPOCsCount;           // Количество progression order changes (POC), default to 0
		int              nLayersCount;         // Количество лэйеров
		float            afRates[100];         // Массив норм
		float            afDistoratio[100];    // Массив искажений
		int              nResolutionsCount;    // Число разрешений(уровней разложения)
		int              nCodeBlockWidthInit;  // Исходная ширина блока (по стандарту 64)
		int              nCodeBlockHeightInit; // Исходная высота блока (по стандарту 64)
		int              nCodeBlockStyle;      // CodeBlockStyle
		int              nTransformId;         // 1: необратимое вэйвлет преобразование ( Irreversible DTW ), 0: компрессия без потерь (по стандарту)
		int              nROIComponentIndex;   // Region of interest: затрагиваемые компоненты [0..3], -1 означает, что ROI отсутствует
		int              nROIShift;            // Region of interest: shift
		int              nPrecinctCount;                     // Количество областей
		int              anPrecinctWidthInit[J2K_MAXRLVLS];  // Исходные ширины областей
		int              anPrecinctHeightInit[J2K_MAXRLVLS]; // Исходные высоты областей

		int              nComponentsCount;       // Количество компонент в исходном файле
		char             sInFile[MAX_PATH_LEN];  // Имя исходного файла
		char             sOutfile[MAX_PATH_LEN]; // Имя результируещего файла
		int              nIndexOn;               // Создаем ли файл с индексайией (по стандарту 0)
		char             sIndex[MAX_PATH_LEN];   // Файл индексации
		int              nImageOffsetX0;         // Сдиг исходной картинки по Х
		int              nImageOffsetY0;         // Сдиг исходной картинки по Y
		int              nSubSamplingDx;         // SubSampling X         
		int              nSubSamplingDy;         // SubSampling Y
		int              nDecoderFormat;         // Формат исходного файла. 0: PGX, 1: PxM, 2: BMP
		int              nEncoderFormat;         // Формат выходного файла. 0: J2K, 1: JP2, 2: JPT
	} EncoderParams;

	// Параметры декомпрессии
	typedef struct TDecoderParams
	{
		int           nReduce;                // Если != 0, тогда разрешение исходного изображения делится на 2^(nReduce). Если == 0, или не используется, тогда изображение декодируется с полным разрешением.
		int           nLayer;                 // Устанавливаем максимальное число лэеров, которые мы будем декодировать.
		LimitDecoding eLimitDecoding;         // Будем декодировать полностью файл, или только заголовок.

		char          sInFile[MAX_PATH_LEN];  // Имя исходного файла
		char          sOutfile[MAX_PATH_LEN]; // Имя результируещего файла
		int           nDecoderFormat;         // Формат исходного файла. 0: J2k, 1: Jp2, 2: Jpt
		int           nEncoderFormat;         // Формат выходного файла. 0: PGX, 1: PxM, 2: BMP
	} DecoderParams;

	// Общие поля для JPEG-2000 компрессии и декомпресии
	//EventManager *pEventManager;   /* Указатель на обработчик сообщений (events) */\
		//void         *pClientData;     /* */\

#define CommonFields \
	bool          bIsDecompressor; /* Чтобы узнать компрессия или декомпрессия */\
	ECodecFormat  eCodecFormat;	   /* Выбранный кодек */\
	void         *pJ2k;            /* Указатель на J2K кодек */\
	void         *pJp2;            /* Указатель на JP2 кодек */\
	void         *pMj2;            /* Указатель на JP2 кодек */\
	int           nErrorCode       /* Номер ошибки */

	// Поля, которые исользуются как для чтения, так и для записи
	typedef struct TCommonStruct
	{
		CommonFields;
	} CommonStruct;

	typedef CommonStruct* PCommon;

	// Параметры компрессии
	typedef struct TCInfo
	{
		CommonFields; // Общие поля
		// Поля характерные только для компрессии
	} CInfo;

	// Параметры декомпрессии
	typedef struct TDInfo
	{
		CommonFields; // Общие поля
		// Поля характерные только для декомпрессии
	} DInfo;


	//-------------------------------------------------------------------------------------------------------------------------------
	// Image
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TImageComponent
	{
		int  nXRsiz;           // XRsiz: горизонтальное разделение i-ой компоненты
		int  nYRsiz;           // YRsiz: вертикальное разделение i-ой компоненты
		int  nWidth;           // Ширина картинки
		int  nHeight;          // Высота картинки
		int  nXOsiz;           // Сдвиг компоненты по Х по отношению ко всему изображению
		int  nYOsiz;           // Сдвиг компоненты по У по отношению ко всему изображению
		int  nPrecision;       // Precision
		int  nBPP;             // Bits Per Pixel
		int  nSigned;          // Signed - 1, unsigned - 0
		int  nDecodedResCount; // Количество Resolutions в кодированном изображении
		int  nFactorDiv2;      // Количество делений на 2 результирующей картинки, по сравнению с размером исходной
		int *pData;            // Данные изображения для i-ой компоненты
	} ImageComponent;

	typedef struct TImage
	{
		int             nXOsiz;      // XOsiz: горизонтальный cдвиг от начальной точки всей сетки к области с реальным изображением
		int             nYOsiz;      // YOsiz: вертикальный сдвиг от начальной точки всей сетки к области с реальным изображением
		int             nXsiz;       // Xsiz: ширина сетки
		int             nYsiz;       // Ysiz: высота сетки
		int             nCsiz;       // Csiz: количество компонент в изображении
		ColorSpace      eColorSpace; // Цветовое пространство: sRGB, Greyscale или YUV
		ImageComponent *pComponents; // Компоненты изображения
	} Image;

	typedef struct TImageComponentParams
	{
		int nXRsiz;     // XRsiz: горизонтальное разделение i-ой компоненты
		int nYRsiz;     // YRsiz: вертикальное разделение i-ой компоненты
		int nWidth;     // Ширина
		int nHeight;    // Высота картинки
		int nXoffset;   // Сдвиг компоненты по Х по отношению ко всему изображению
		int nYoffset;   // Сдвиг компоненты по У по отношению ко всему изображению
		int nPrecision; // Precision
		int nBPP;       // Bits Per Pixel
		int nSigned;    // Signed - 1, unsigned - 0
	} ImageComponentParams;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Tag-tree coder (TagTree.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	// Tag node
	typedef struct TTagNode
	{
		struct TTagNode *pParent;
		int              nValue;
		int              nLow;
		int              nKnown;
	} TagNode;

	// Tag tree
	typedef struct TTagTree
	{
		int      nLeafsHorCount;
		int      nLeafsVerCount;
		int      nNodesCount;
		TagNode *pNodes;
	} TagTree;

	//-------------------------------------------------------------------------------------------------------------------------------
	//  JPEG2000 codestream (J2k.h)
	//-------------------------------------------------------------------------------------------------------------------------------

#define J2K_CP_CSTY_PRT 0x01 // CodingParams - CodingStyle - Partition (Tables A-20, A-23)
#define J2K_CP_CSTY_SOP 0x02
#define J2K_CP_CSTY_EPH 0x04

#define J2K_CCP_CSTY_PRT 0x01

#define J2K_CCP_CBLKSTY_LAZY    0x01
#define J2K_CCP_CBLKSTY_RESET   0x02
#define J2K_CCP_CBLKSTY_TERMALL 0x04
#define J2K_CCP_CBLKSTY_VSC     0x08
#define J2K_CCP_CBLKSTY_PTERM   0x10
#define J2K_CCP_CBLKSTY_SEGSYM  0x20

#define J2K_CCP_QNTSTY_NOQNT 0 // ComponentCodingParams - Quantization style - No quantization (Tables A-28, A-29)
#define J2K_CCP_QNTSTY_SIQNT 1 // ComponentCodingParams - Quantization style - Scalar implicit (Tables A-28, A-30)
#define J2K_CCP_QNTSTY_SEQNT 2 // ComponentCodingParams - Quantization style - Scalar explicit (Tables A-28, A-30)

	//-------------------------------------------------------------------------------------------------------------------------------

	// Marker Segments (Annex A.2 Table A-2)
#define J2K_MS_SOC 0xff4f	// SOC
#define J2K_MS_SOT 0xff90	// SOT
#define J2K_MS_SOD 0xff93	// SOD
#define J2K_MS_EOC 0xffd9	// EOC
#define J2K_MS_SIZ 0xff51	// SIZ
#define J2K_MS_COD 0xff52	// COD
#define J2K_MS_COC 0xff53	// COC
#define J2K_MS_RGN 0xff5e	// RGN
#define J2K_MS_QCD 0xff5c	// QCD
#define J2K_MS_QCC 0xff5d	// QCC
#define J2K_MS_POC 0xff5f	// POC
#define J2K_MS_TLM 0xff55	// TLM
#define J2K_MS_PLM 0xff57	// PLM
#define J2K_MS_PLT 0xff58	// PLT
#define J2K_MS_PPM 0xff60	// PPM
#define J2K_MS_PPT 0xff61	// PPT
#define J2K_MS_SOP 0xff91	// SOP
#define J2K_MS_EPH 0xff92	// EPH
#define J2K_MS_CRG 0xff63	// CRG
#define J2K_MS_COM 0xff64	// COM
	//////////////////////////////////////////////////
#define J2K_MS_EPC 0xff68	/**< EPC marker value (Part 11: JPEG 2000 for Wireless) */
#define J2K_MS_EPB 0xff66	/**< EPB marker value (Part 11: JPEG 2000 for Wireless) */ 
#define J2K_MS_ESD 0xff67	/**< ESD marker value (Part 11: JPEG 2000 for Wireless) */ 
#define J2K_MS_RED 0xff69	/**< RED marker value (Part 11: JPEG 2000 for Wireless) */
#define J2K_MS_SEC 0xff65    /**< SEC marker value (Part 8: Secure JPEG 2000) */
#define J2K_MS_INSEC 0xff94  /**< INSEC marker value (Part 8: Secure JPEG 2000) */

	//-------------------------------------------------------------------------------------------------------------------------------

	// Значения определяющие статус декодирования(когда декодируется Main Header)
	typedef enum EJ2kStatus
	{
		j2kstateMHSOC  = 0x0001, // SOC маркер
		j2kstateMHSIZ  = 0x0002, // SIZ маркер
		j2kstateMH     = 0x0004, // Процесс декодирования находится в Main Header
		j2kstateTPHSOT = 0x0008, // Процесс декодирования находится в Tile Part Header, и сейчас читается маркер SOT
		j2kstateTPH    = 0x0010, // Процесс декодирования находится в Tile Part Header
		j2kstateMT     = 0x0020, // Только что был прочитан маркер EOC
		j2kstateNEOC   = 0x0040  // При декодировании не воспринимаем маркер EOC, потому что Сodestream сокращенный
	} J2kStatus;

	//-------------------------------------------------------------------------------------------------------------------------------

	// Размер шага квантования 
	typedef struct TQuantStepSize
	{
		int nExponent;
		int nMantissa;
	} QuantStepSize;

	// Tile-component coding parameters (Annex A.6.2 Table A-23)
	typedef struct TTileCompCodingParams
	{
		int           nCodingStyle;                   // Coding style
		int           nResolutionsCount;              // Decomposition levels
		int           nCodeBlockWidth;                // Code-block size width
		int           nCodeBlockHeight;               // Code-block size height
		int           nCodeBlockStyle;                // Code-block context
		int           nTransformID;                   // Wavelet-преобразвание
		int           nQuantStyle;                    // Тип квантования
		QuantStepSize aoStepSizes[J2K_MAXBANDS];      // Размер шага, используемый для квантования
		int           nGuardBitsCount;                // Number of guard bits
		int           nROIShift;                      // SPrgn: Region Of Interest shift (Table A-25)
		int           anPrecinctWidth[J2K_MAXRLVLS];  // Precinct width
		int           anPrecinctHeight[J2K_MAXRLVLS]; // Precinct height	
	} TileCompCodingParams;

	// Tile coding parameters : данная структура используется для хранения параметров кодирования/декодирования одинаковых для всех 
	// ячеек(tiles) ( подобно информации COD, COC в Main Header )
	typedef struct TTileCodingParams
	{
		int                   nFirst;            // Флаг. 1 : первый part-tile данного tile
		int                   nCodingStyle;      // Scod: Coding style (Table A-12)
		ProgressionOrder      eProgOrder;        // SPcod, Ppod: Progression order
		int                   nLayersCount;      // SPcod: Number of layers
		int                   nMCT;              // SPcod: Multiple component transform
		float                 afRates[100];      // Размеры layers
		int                   nPOCsCount;        // Количество POC ( progression order changes )
		int                   nUsePOC;           // Флаг; используем ли маркер POC. 0 - нет, 1 - да.
		POC                   aoPOC[32];         // Progression order changes
		unsigned char        *pPPTData;          // Храним указатель на Packet header, чтобы испольщовать его в Tire2 декодировании
		unsigned char        *pPPTDataFirst;     // Указатель на начало первого пакета в PacketHeader, если PPT используется
		int                   nPPT;              // Флаг. Если для текущего Tile нашли маркер PPT, тогда флаг равен 1.
		int                   nPPTStore;         // Используем, если нашли несколько маркеров PPT (храним их общую длину в байтах здесь)
		int                   nPPTLength;        // PPM bug 1
		float                 afDistoRatio[100]; // Добавляем Fixed quality
		TileCompCodingParams *pTCCP;             // Tile-component coding parameters
	} TileCodingParams;

	// Coding parameters
	typedef struct TCodingParams
	{
		int               nDistoAlloc;    // allocation by rate/distortion
		int               nFixedAlloc;    // allocation by fixed layer
		int               nFixedQuality;  // Добавляем Fixed quality
		int               nReduceFactor;  // Если данное значение отлично от 0, тогда исходное разрешение делим на 2^(nReduceFactor); если равно 0, или не используется, декодируем изображение с полным разрешением
		int               nLayerFactor;   // Если данное значение отлично от 0, тогда декодируем только первый "layer"; если равно 0, или не используется, декодируем все "layers"
		LimitDecoding     eLimitDecoding; // Ограничения на декодирование.
		int               nIndexOn;       // 0 - нет нумерации, 1 - есть.
		int               nXTOsiz;        // XTOsiz: свдиг по горизонтали первого Tile
		int               nYTOsiz;        // YTOsiz: сдвиг по вертикали первого Tile
		int               nXTsiz;         // XTsiz: ширина одного Tile по отношению к общей сетке
		int               nYTsiz;         // YTsiz: высота одного Tile по отношению к общей сетке
		char             *sComment;       // Комментарий
		int               nXTilesCount;   // Количество Tiles по горизонтали
		int               nYTilesCount;   // Количество Tiles по вертикали
		int              *pTileIndex;     // Порядковые номера Tiles в codestream
		int               nTileIndexSize; // Размер массива pTileIndex
		unsigned char    *pPPMData;       // Храним указатель на Packet header, чтобы испольщовать его в Tire2 декодировании
		unsigned char    *pPPMDataFirst;  // Указатель на начало первого пакета в PacketHeader, если PPM используется
		int               nPPM;           // Флаг. Если для текущего Tile нашли маркер PPM, тогда флаг равен 1.
		int               nPPMStore;      // Используем, если нашли несколько маркеров PPM (храним их общую длину в байтах здесь)
		int               nPPMPrevious;   // Используем, если нашли несколько маркеров PPM (случай, если предыдущий маркер PPM был еще не закончен)
		int               nPPMLength;     // PPM bug 1
		TileCodingParams *pTCP;           // Tile coding parameters
		int              *pMatrix;        // Fixed layer
	} CodingParams;

	// Информация относительно пакета в Tile
	typedef struct TPacketInfo
	{
		int    nStartPos; // Начальная позиция
		int    nEndPos;   // Конечная позиция
		double dDisto;
	} PacketInfo;

	// Информация, относящаяся к Tiles внутри изображения
	typedef struct TTileInfo
	{
		double     *pdThreshold;          // Значение порога для каждого layer
		int         nTileCount;           // Общее количество Tiles
		int         nStartPos;            // Начальная позиция
		int         nEndHeader;           // Конечная позиция для заголовка
		int         nEndPos;              // Конечная позиция
		int         anXPrecinctCount[33]; // Количество областей (precinct) по Х для каждого уровня разрешения (resolution level)
		int         anYPrecinctCount[33]; // Количество областей (precinct) по Y для каждого уровня разрешения (resolution level)
		int         anPrecinctWidth[33];  // Ширина области (в степени 2), для какждого уровня разрешения
		int         anPrecinctHeight[33]; // Высота области (в степени 2), для какждого уровня разрешения
		PacketInfo *pPacket;              // PacketInfo
		int         nNBPix;               // Добавляем Fixed Quality
		double      dDistoTile;           // Добавляем Fixed Quality
	} TileInfo;

	typedef struct TImageInfo
	{
		int              nIndexOn;        // 0 - нет нумерации, 1 - есть.
		double           dDistoMax;       // Максимальный уровень сокращения искажений для всего изображения
		int              nPacketCount;    // Количество пакетов
		int              nIndexWrite;     //
		int              nImageWidth;     // Ширина картинки
		int              nImageHeight;    // Высота картинки
		ProgressionOrder eProgOrder;      // Progression Order
		int              nXTsiz;          // Размер tile по Х
		int              nYTsiz;          // Размер tile по Y
		int              nXTOsiz;         // Сдвиг первого тайла по Х
		int              nYTOsiz;         // Сдвиг первого тайла по Y
		int              nXTilesCount;    // Количество тайлов по Х
		int              nYTilesCount;    // Количество тайлов по Y
		int              nCompCount;      // Количество компонент
		int              nLayersCount;    // Количество layers
		int              nDecompCount;    // Количество уровней разложения
		int              nMainHeadEnd;    // Позиция окончания Main Header
		int              nCodestreamSize; // Размер codestream
		TileInfo        *pTile;           // TileInfo
	} ImageInfo;

	// JPEG-2000 codestream reader/writer
	typedef struct TJ2kCodestream
	{
		PCommon           pCodecInfo;     // Общая информация для кодирования и декодирования
		int               nState;         // Здесь хранам текущую позицию декодера Codestream'a ( Main header, Tile header, конец декодирования )
		int               nCurTileIndex;  // Номер текущего тайла (т.е. который в данный момент кодируется/декодируется)
		//unsigned char    *pEndOfTile;     // Используется для определения позиции конца тайла в codestream (чтобы определить нормальный ли codestream)
		int				  nSizeOfTile;
		int               nSOTStartPos;   // Сохраняем начальную позицию маркера SOT, чтобы после кодирования текущего тайла мы могли вернуться и записать длину данного маркера
		int               nSODStartPos;   // Аналогично, для маркера SOD
		int               nPosCorrection; // Необходимо для корректировки позиции, поскольку J2k-файл пишется в несколько частей, во время кодирования
		unsigned char   **ppTileData;     // Массив с данными всех тайлов
		int              *pTileLen;       // Массив, хранящий размер данных для каждого тайла
		TileCodingParams *pDefaultTCP;    // Используется только при декомпрессии: храним общую информацию для всех тайлов ( COD, COC )
		Image            *pImage;         // Указатель на изображения, которое мы либо кодируем, либо декодируем
		CodingParams     *pCodingParams;  // Указатель на структуру, хранящую информацию о кодировании
		ImageInfo        *pImageInfo;     // 
		CReader           *pStreamIO;      // Input / Output stream
	} J2kCodestream;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Tile Coder/Decoder (Tile.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TTCDSegment
	{
		int            nPassesCount;
		int            nLength;
		unsigned char *pData;
		int            nMaxPasses;
		int            nNewPassesCount;
		int            nNewLength;
	} TCDSegment;

	typedef struct TTCDPass
	{
		int            nRate;
		double         dDistortionDec;
		int            nTerm;
		int            nLen;
	} TCDPass;

	typedef struct TLayer
	{
		int            nPassesCount; // Количество проходов
		int            nLen;         // Длина информации
		double         dDisto;       // Отклонение
		unsigned char *pData;        // Данные
	} Layer;

	typedef struct TCodeBlock
	{
		int           nX0;                  // Размеры CodeBlock
		int           nY0;                  // (x0, y0) - левый верхний угол
		int           nX1;                  // (х1, у1) - правый нижний угол
		int           nY1;                  // 
		int           nBPSCount;            //
		int           nLenBitsCount;        //
		int           nLen;                 // Длина
		int           nPassesCount;         // Количество уже завершенных проходов для данного CodeBlock
		int           nNewPassesCount;      // Количество проходов добавленных к данному CodeBlock
		int           nSegmentsCount;       // Количество сегментов
		TCDSegment    aSegments[100];		// Сами сегменты
		unsigned char  aData[8192];			 // Данные
		int           nPassesCountInLayers; // Количество проходов в лэйре
		Layer         aLayers[100];				// Информация о лэйерах	
		int           nTotalPassesCount;    // Общее число проходов
		TCDPass       aPasses[100];         // Информация о проходах
	} CodeBlock;

	typedef struct TPrecinct
	{
		int        nX0;            // Размеры Precinct
		int        nY0;            // (x0, y0) - левый верхний угол
		int        nX1;            // (х1, у1) - правый нижний угол
		int        nY1;            // 
		int        nCountInWidth;  // Количество блоков в ширину
		int        nCountInHeight; // Количество блоков в высоту
		CodeBlock *pCodeBlocks;    // CodeBlocks
		TagTree   *pInclTree;      // Дерево включения
		TagTree   *pIMSBTree;      // Дерево IMSB
	} Precinct;

	typedef struct TBand
	{
		int       nX0;        // Размеры Band
		int       nY0;        // (x0, y0) - левый верхний угол
		int       nX1;        // (х1, у1) - правый нижний угол
		int       nY1;        // 
		int       nBandIndex; // Номер текущего Band
		Precinct *pPrecincts; // Информация о областях(Precinct), входящий в данный Band
		int       nBPSCount;  //
		float     fStepSize;  //
	} Band;

	typedef struct TResolution
	{
		int  nX0;         // Размеры Resolution
		int  nY0;         // (x0, y0) - левый верхний угол
		int  nX1;         // (х1, у1) - правый нижний угол
		int  nY1;         // 
		int  nPrecCountW; // Количество областей (Precinct) по ширине
		int  nPrecCountH; // Количество областей (Precinct) по высоте
		int  nBandsCount; // Количество Sub-band
		Band aBands[3];   // Sub-bands
	} Resolution;

	typedef struct TTileComp
	{
		int         nX0;               // Размеры TileComp 
		int         nY0;               // (x0, y0) - левый верхний угол
		int         nX1;               // (х1, у1) - правый нижний угол
		int         nY1;               // 
		int         nResolutionsCount; // Число разрешений
		Resolution *pResolutions;      // Разрешения (Resolutions)
		int        *pData;             // Данные компоненты
		int         nNBPix;            // Fixed_quality
	} TileComp;

	typedef struct TTile
	{
		int       nX0;               // Размеры Tile
		int       nY0;               // (x0, y0) - левый верхний угол
		int       nX1;               // (х1, у1) - правый нижний угол
		int       nY1;               // 
		int       nComponentsCount;  // Количество компонент
		TileComp *pComponents;       // Компоненты
		int       nNBPix;            // Fixed_quality
		double    dDistoTile;        // Fixed_quality
		double    adDistoLayer[100]; // Fixed_quality
	} Tile;

	typedef struct TTCDImage
	{
		int   nTilesCountW; // Количество тайлов по ширине
		int   nTilesCountH; // Количество тайлов по высоте
		Tile *pTiles;       // Тайлы
	} TCDImage;

	typedef struct TTCD
	{
		PCommon           pCodecInfo;    // Информация о кодеке
		TCDImage         *pTCDImage;     // Кодированное Изображение
		Image            *pImage;        // Изображение
		CodingParams     *pCodingParams; // Параметры
		Tile             *pTCDTile;      // Указатель на текущий кодируемый/декодируемый тайл
		TileCodingParams *pTCP;          // Параметры кодирования/декодирования для всех тайлов
		int               nTCDTileIndex; // Номер текущего кодируемого/декодируемого тайла
	} TCD;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Jp2 file format (Jp2.h)
	//-------------------------------------------------------------------------------------------------------------------------------

#define JPIP_JPIP 0x6a706970

	// Jp2 boxes (Table I-2)
#define JP2_JP   0x6a502020 // JPEG 2000 signature box
#define JP2_PRFL 0x7072666c // Profile box
#define JP2_FTYP 0x66747970 // File type box
#define JP2_JP2H 0x6a703268 // JP2 header box
#define JP2_IHDR 0x69686472 // Image header box
#define JP2_COLR 0x636f6c72 // Colour specification box
#define JP2_JP2C 0x6a703263 // Contiguous codestream box
#define JP2_URL  0x75726c20 // URL box
#define JP2_DBTL 0x6474626c // ???
#define JP2_BPCC 0x62706363 // Bits per component box
#define JP2_JP2  0x6a703220 // File type fields
#define JP2_CDEF 0x63646566 // Component Definition box

	//-------------------------------------------------------------------------------------------------------------------------------

	// Jp2 component
	typedef struct TJp2Component
	{
		int nDepth;
		int nSigned;
		int nBPCC;
	} Jp2Component;

	typedef struct TJp2Color
	{
		BYTE *icc_profile_buf;
		DWORD icc_profile_len;

		//opj_jp2_cdef_t *jp2_cdef;
		//opj_jp2_pclr_t *jp2_pclr;

		bool has_colr;
	} Jp2Color;

	// Jp2 file format reader/writer
	typedef struct TJp2Stream
	{
		PCommon        pCodecInfo;       // Общая информация для кодирования и декодирования
		J2kCodestream *pJ2k;             // Указатель на структуру для чтения/записи JPEG2000 Codestream

		unsigned int   nWidth;           // Ширина изображения
		unsigned int   nHeight;          // Высота изображения
		unsigned int   nComponentsCount; // Число компонент
		unsigned int   nBPC;             // Bits per component
		unsigned int   nCompressionType; // CompressionType
		unsigned int   nColorSpaceUnk;   // Colourspace Unknown
		unsigned int   nIPR;             // Intellectual Property

		unsigned int   nMethod;          // Color specification method
		unsigned int   nPrec;            // Precedence
		unsigned int   nApprox;          // Colourspace approximation
		unsigned int   nEnumCS;          // Enumerated colourspace

		unsigned int   nBrand;           // Brand
		unsigned int   nMinVersion;      // MinVersion
		unsigned int   nCompListLength;  // Длина списка Compatibility list
		unsigned int  *pCompList;        // Compatibility list

		Jp2Component  *pComponents;
		unsigned int   nJ2kCodestreamOffset;
		unsigned int   nJ2kCodestreamLength;

		Jp2Color	   oColor;
	} Jp2Stream;

	// Jp2 Box
	typedef struct TJp2Box
	{
		int nLength;
		int nType;
		int nInitPos;
	} Jp2Box;

	//-------------------------------------------------------------------------------------------------------------------------------
	// MQ-coder (ArithmeticCoder.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	// См. Annex C. fcd15444-1.pdf ( MQ-coder Encoder/Decoder )

#define MQC_NUMCTXS 32

	typedef struct TMQCoderState
	{
		unsigned int          nLPSValue; // вероятность для наименьшего вероятностного символа (Least Probable Symbol) (0.75->0x8000, 1.5->0xffff)
		int                   nMPS;      // наибольший вероятностный символ ( 0 или 1 ) (Most Probable Symbol)
		struct TMQCoderState *pNextMPS;  // Указатель на следующее состояние, если следующий закодированный символ MPS
		struct TMQCoderState *pNextLPS;  // Указатель на следующее состояние, если следующий закодированный символ LPS
	} MQCoderState;

	// MQ-coder
	typedef struct TMQCoder
	{
		unsigned int   nCregister;
		unsigned int   nAregister;
		unsigned int   nCt;

		unsigned char *pBufferPointer;
		unsigned char *pStart;
		unsigned char *pEnd;
		MQCoderState  *pContexts[MQC_NUMCTXS];
		MQCoderState **ppCurContext;
	} MQCoder;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Raw (Raw.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TRaw
	{
		unsigned char  nBuffer;        // Временный буфер, в котором происходит побитовое кодирование/декодирование
		unsigned int   nFreeBitsCount; // Количество бит, свободных для чтения или записи
		unsigned int   nMaxLen;        // Максимальная длина для декодирования
		unsigned int   nLen;           // Длина декодированной информации
		unsigned char *pBufferPointer; // Указатель на текущую позицию в буфере
		unsigned char *pBufferStart;   // Указатель на начало буфера
		unsigned char *pBufferEnd;     // Указатель на конец буфера
	} Raw;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Jpt (Jpt.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	// Message Header JPT stream structure
	typedef struct TJPTMessageHeader
	{
		unsigned int nId;            // In-class Identifier
		unsigned int nLastByte;      // Last byte information
		unsigned int nClassId;       // Class Identifier
		unsigned int nCSnId;         // CSn : index identifier
		unsigned int nMessageOffset; // Message offset
		unsigned int nMessageLength; // Message length
		unsigned int nLayerNB;       // Auxiliary for JPP case
	} JPTMessageHeader;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Packet Iterator 
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TPacketResolution
	{
		int nDx;
		int nDy;
		int nWidth;
		int nHeight;
	} PacketResolution;

	typedef struct TPacketComponent
	{
		int               nDx;
		int               nDy;
		int               nResolutionsCount;
		PacketResolution *pResolutions;
	} PacketComponent;

	typedef struct TPacketIterator
	{
		short int       *pInclude;         // Флаг, сигнализирующий использовался ли данный пакет (используется в POC)
		int              nStepLayer;       // LayerStep используется для локализации пакета в pInclude
		int              nStepResolution;  // ResoultionStep используется для локализации пакета в pInclude
		int              nStepComponent;   // ComponentStep используется для локализации пакета в pInclude
		int              nStepPrecinct;    // PrecinctStep используется для локализации пакета в pInclude

		int              nIndexComponent;  // Номер компоненты для определения пакета
		int              nIndexResolution; // Номер уровня детализации для определения пакета
		int              nIndexPrecinct;   // Номер области для определения пакета
		int              nIndexLayer;      // Номер лэйера для определения пакета  

		int              nFirst;           // Флаг, определяющий первый ли это пакет
		POC              oPOC;             // Progression Order Change
		int              nComponentsCount; // Число компонент

		PacketComponent *pComponents;
		int              nTileX0;          //
		int              nTileY0;          // Границы текущего тайла
		int              nTileX1;          //
		int              nTileY1;          //
		int              nX;
		int              nY;
		int              nDx;
		int              nDy;
	} PacketIterator;

	//-------------------------------------------------------------------------------------------------------------------------------
	//  Tier-1 coding (кодирование коэффициентов)
	//-------------------------------------------------------------------------------------------------------------------------------

#define T1_NMSEDEC_BITS 7

#define T1_MAXCBLKW 1024	// Максимальный размер CodeBlock (по ширине)
#define T1_MAXCBLKH 1024	// Максимальный размер CodeBlock (по высоте)

#define T1_SIG_NE 0x0001	// Ориентация контекста : Северо-восточное направление
#define T1_SIG_SE 0x0002	// Ориентация контекста : юго-восточное направление
#define T1_SIG_SW 0x0004	// Ориентация контекста : юго-западное направление
#define T1_SIG_NW 0x0008	// Ориентация контекста : северо-западное направление
#define T1_SIG_N  0x0010	// Ориентация контекста : северное направление
#define T1_SIG_E  0x0020	// Ориентация контекста : восточное направление
#define T1_SIG_S  0x0040	// Ориентация контекста : южное направление
#define T1_SIG_W  0x0080	// Ориентация контекста : западное направление
#define T1_SIG_OTH  (T1_SIG_N|T1_SIG_NE|T1_SIG_E|T1_SIG_SE|T1_SIG_S|T1_SIG_SW|T1_SIG_W|T1_SIG_NW)
#define T1_SIG_PRIM (T1_SIG_N|T1_SIG_E|T1_SIG_S|T1_SIG_W)

#define T1_SGN_N 0x0100
#define T1_SGN_E 0x0200
#define T1_SGN_S 0x0400
#define T1_SGN_W 0x0800
#define T1_SGN (T1_SGN_N|T1_SGN_E|T1_SGN_S|T1_SGN_W)

#define T1_SIG    0x1000
#define T1_REFINE 0x2000
#define T1_VISIT  0x4000

#define T1_NUMCTXS_ZC 9
#define T1_NUMCTXS_SC 5
#define T1_NUMCTXS_MAG 3
#define T1_NUMCTXS_AGG 1
#define T1_NUMCTXS_UNI 1

#define T1_CTXNO_ZC 0
#define T1_CTXNO_SC  (T1_CTXNO_ZC  + T1_NUMCTXS_ZC )
#define T1_CTXNO_MAG (T1_CTXNO_SC  + T1_NUMCTXS_SC )
#define T1_CTXNO_AGG (T1_CTXNO_MAG + T1_NUMCTXS_MAG)
#define T1_CTXNO_UNI (T1_CTXNO_AGG + T1_NUMCTXS_AGG)
#define T1_NUMCTXS   (T1_CTXNO_UNI + T1_NUMCTXS_UNI)

#define T1_NMSEDEC_FRACBITS (T1_NMSEDEC_BITS - 1)

#define T1_TYPE_MQ  0  // Нормальное кодирование, с использованием Arithmetic entropy coding (Annex C)
#define T1_TYPE_RAW 1  // Нет никакого кодирования, информация в Codestream содержится в сыром виде

	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TTier1
	{
		PCommon  pCodecInfo;  // Информация о кодеке
		MQCoder *pMQCoder;    // Указатель на структуру для чтения/записи MQ-кодированных данных
		Raw     *pRaw;        // Указатель на структуру для чтения/записи некодированных данных

		int      lut_ctxno_zc[1024];
		int      lut_ctxno_sc[256];
		int      lut_ctxno_mag[4096];
		int      lut_spb[256];
		int      lut_nmsedec_sig[1 << T1_NMSEDEC_BITS];
		int      lut_nmsedec_sig0[1 << T1_NMSEDEC_BITS];
		int      lut_nmsedec_ref[1 << T1_NMSEDEC_BITS];
		int      lut_nmsedec_ref0[1 << T1_NMSEDEC_BITS];

		int      aData[T1_MAXCBLKH][T1_MAXCBLKW];          // Данные codeblock
		int      aFlags[T1_MAXCBLKH + 2][T1_MAXCBLKH + 2]; // Флаги

	} Tier1;

	//-------------------------------------------------------------------------------------------------------------------------------
	//  Tier-2 coding
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TTier2
	{
		PCommon       pCodecInfo;
		Image        *pImage;        // Encoding: указатель на исходную картинку. Decoding: указатель на результирующую картинку.
		CodingParams *pCodingParams;
	} Tier2;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Mj2 file format (Mj2.h)
	//-------------------------------------------------------------------------------------------------------------------------------

#define MJ2_JP    0x6a502020
#define MJ2_FTYP  0x66747970
#define MJ2_MJ2   0x6d6a7032
#define MJ2_MJ2S  0x6d6a3273
#define MJ2_MDAT  0x6d646174
#define MJ2_MOOV  0x6d6f6f76
#define MJ2_MVHD  0x6d766864
#define MJ2_TRAK  0x7472616b
#define MJ2_TKHD  0x746b6864
#define MJ2_MDIA  0x6d646961
#define MJ2_MDHD  0x6d646864
#define MJ2_MHDR  0x6d686472
#define MJ2_HDLR  0x68646C72
#define MJ2_MINF  0x6d696e66
#define MJ2_VMHD  0x766d6864
#define MJ2_SMHD  0x736d6864
#define MJ2_HMHD  0x686d6864
#define MJ2_DINF  0x64696e66
#define MJ2_DREF  0x64726566
#define MJ2_URL   0x75726c20
#define MJ2_URN   0x75726e20
#define MJ2_STBL  0x7374626c
#define MJ2_STSD  0x73747364
#define MJ2_STTS  0x73747473
#define MJ2_STSC  0x73747363
#define MJ2_STSZ  0x7374737a
#define MJ2_STCO  0x7374636f
#define MJ2_MOOF  0x6d6f6f66
#define MJ2_FREE  0x66726565
#define MJ2_SKIP  0x736b6970
#define MJ2_JP2C  0x6a703263
#define MJ2_FIEL  0x6669656c
#define MJ2_JP2P  0x6a703270
#define MJ2_JP2X  0x6a703278
#define MJ2_JSUB  0x6a737562
#define MJ2_ORFO  0x6f72666f
#define MJ2_MVEX  0x6d766578
#define MJ2_JP2   0x6a703220
#define MJ2_J2P0  0x4a325030

	// Decompressed format used in parameters YUV = 0
#define YUV_DFMT 1 

	// Compressed format used in parameters MJ2 = 0
#define MJ2_CFMT 2


	//-------------------------------------------------------------------------------------------------------------------------------

	// Time To Sample
	typedef struct TMj2_TimeToSample
	{
		int nSampleCount;
		int nSampleDelta;
	} Mj2_TimeToSample;

	// Chunk
	typedef struct TMj2_Chunk
	{
		int nNumSamples;
		int nSampleDescriptionIndex;
		int nOffset;
	} Mj2_Chunk;

	// Sample to chunk
	typedef struct TMj2_SampleToChunk
	{
		int nFirstChunk;
		int nSamplesPerChunk;
		int nSampleDescriptionIndex;
	} Mj2_SampleToChunk;

	// Sample 
	typedef struct TMj2_Sample
	{
		unsigned int unSampleSize;
		unsigned int unOffset;
		unsigned int unSampleDelta;
	} Mj2_Sample;

	// URL
	typedef struct TMj2_Url
	{
		int anLocation[4];
	} Mj2_Url;

	// URN
	typedef struct TMj2_Urn
	{
		int anName[4];
		int anLocation[4];
	} Mj2_Urn;

	// Image Track Parameters
	typedef struct TMj2_TrackParams
	{
		PCommon           pCodecInfo;
		int               nTrackID;
		int               nTrackType;
		unsigned int      unCreationTime;
		unsigned int      unModificationTime;
		int               nDuration;
		int               nTimescale;
		int               nLayer;
		int               nVolume;
		int               nLanguage;
		int               nBalance;
		int               nMaxPDUsize;
		int               nAvgPDUsize;
		int               nMaxBitrate;
		int               nAvgBitrate;
		int               nSlidingAvgBitrate;
		int               nGraphicsMode;
		int               anOpColor[3];
		int               nNumUrl;
		Mj2_Url*          pUrl;
		int               nNumUrn;
		Mj2_Urn*          pUrn;
		int               anDim[2];
		int               nWidth;
		int               nHeight;
		int               nVisualWidth;
		int               nVisualHeight;
		int               nCbCr_SubSampling_dx;
		int               nCbCr_SubSampling_dy;
		int               nSampleRate;
		int               nSampleDescription;
		int               nHorResolution;
		int               nVerResolution;
		int               anCompressorName[8];
		int               nDepth;
		unsigned char     unFieldCount;
		unsigned char     unFieldOrder;
		unsigned char     unOr_FieldCount;
		unsigned char     unOr_FieldOrder;
		int               nNumBr;
		unsigned int*     pBr;
		unsigned char     unNumJp2x;
		unsigned char*    pJp2xData;
		unsigned char     unHsub;
		unsigned char     unVsub;
		unsigned char     unHoff;
		unsigned char     unVoff;
		int               anTransMatrix[9];
		unsigned int      unNumSamples;       // Number of samples
		int               nTransorm;
		int               nHandlerType;
		int               nNameSize;
		unsigned char     unSameSampleSize;
		int               nNumTimeToSample;
		Mj2_TimeToSample* pTimeToSample;      // Time to sample
		unsigned int      unNumChunks;
		Mj2_Chunk*        pChunk;
		int               nNumSamplesToChunk;
		Mj2_SampleToChunk*pSampleToChunk;
		char*             sName;
		Jp2Stream         oJp2;
		Mj2_Sample       *pSample;             // Sample parameters
	} Mj2_TrackParams;

	// MJ2 box
	typedef struct TMj2_Box
	{
		int nLength;
		int nType;
		int nInitPos;
	} Mj2_Box;

	// MJ2 Movie
	typedef struct TMj2_Movie
	{
		PCommon          pCodecInfo;
		J2kCodestream*   pJ2k;
		unsigned int     unBrand;
		unsigned int     unMinVersion;
		int              nCompListLength;
		unsigned int*    pCompList;
		unsigned int     unCreationTime;
		unsigned int     unModificationTime;
		int              nTimescale;
		unsigned int     unDuration;
		int              nRate;
		int              nNumVtk;
		int              nNumStk;
		int              nNumHtk;
		int              nVolume;
		int              anTransMatrix[9];
		int              nNextTrackId;
		Mj2_TrackParams* pTrack;
	} Mj2_Movie;

	// Compression parameters
	typedef struct TMj2_EncoderParameters
	{
		TEncoderParams pJ2k_Parameters;
		int            nDecoderFormat;       // input file format 0:MJ2
		int            nEncoderFormat;       // output file format 0:YUV
		int            anDim[2];             // Portion of the image coded
		int            nWidth;               // YUV Frame width
		int            nHeight;              // YUV Frame height
		int            nCbCr_SubSampling_dx; // Sample rate of YUV 4:4:4, 4:2:2 or 4:2:0	
		int            nCbCr_SubSampling_dy; // Sample rate of YUV 4:4:4, 4:2:2 or 4:2:0
		int            nFrameRate;           //   Image Frame Rate
		int            nNumComps;            // In YUV files, numcomps always considered as 3
		int            nPrec;                // In YUV files, precision always considered as 8
	} Mj2_EncoderParameters;


	// Bit input-output stream (BitIO)
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TBitIO
	{
		unsigned char *pStart;          // Указатель на начало буфера
		unsigned char *pEnd;            // Указатель на конец буфера
		unsigned char *pCurPos;         // Указатель на текущую позицию в буфере
		unsigned int   nBuffer;         // Буфер
		int            nValidBitsCount; // Coder   : число свободных бит для записи
		// Decoder : число доступных бит для чтения
	} BitIO;
}
