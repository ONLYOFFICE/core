#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "GlobalParams.h"

class StringExt;
class XRef;
class Array;
class Stream;
class Parser;
class Dict;
class Function;
class OutputDev;
class GrFontDict;
class GrFont;
class GrPattern;
class GrTilingPattern;
class GrShadingPattern;
class GrShading;
class GrFunctionShading;
class GrAxialShading;
class GrRadialShading;
class GrGouraudTriangleShading;
class GrPatchMeshShading;
struct GrPatch;
class GrState;
struct GrColor;
class GrColorSpace;
class Graphics;
class PDFRectangle;
class AnnotBorderStyle;

//-------------------------------------------------------------------------------------------------------------------------------

enum GrClipType 
{
	clipNone,
	clipNormal,
	clipEO
};

enum ArgType 
{
	argBool,           // Boolean
	argInt,            // Integer
	argNum,            // Number (integer or real)
	argString,         // String
	argName,           // Name
	argArray,          // Array
	argProps,          // Properties (Dictionary или Name)
	argSCN,            // scn/SCN
	argNone            // используется, чтобы избежать пустых полей при инициализации
};

#define maxArgs 33

struct Operator 
{
	char     sName[4];
	int      nArgumentsCount;
	ArgType  arrArguments[maxArgs];
    void    (Graphics::*pFunction)(Object pArgs[], int nArgsCount);
};

//-------------------------------------------------------------------------------------------------------------------------------
// GrResources
//-------------------------------------------------------------------------------------------------------------------------------

class GrResources 
{
public:

	GrResources(XRef *pXref, Dict *pResourcesDict, GrResources *pNext, GlobalParams *pGlobalParams);
	~GrResources();

	GrFont    *LookupFont          (char *sName);
	BOOL       LookupXObject       (char *sName, Object *pObject);
	BOOL       LookupAndCopyXObject(char *sName, Object *pObject);
	void       LookupColorSpace    (char *sName, Object *pObject);
	GrPattern *LookupPattern       (char *sName);
	GrShading *LookupShading       (char *sName);
	BOOL       LookupExtGState     (char *sName, Object *pObject);

	GrResources *GetNext() 
	{ 
		return m_pNext; 
	}

private:

	GrFontDict  *m_pFonts;
	Object       m_oXObjectDict;
	Object       m_oColorSpaceDict;
	Object       m_oPatternDict;
	Object       m_oShadingDict;
	Object       m_oExtGStateDict;
	GrResources *m_pNext;

	GlobalParams *m_pGlobalParams;
};

//-------------------------------------------------------------------------------------------------------------------------------
// Graphics
//-------------------------------------------------------------------------------------------------------------------------------

class Graphics 
{
public:

	Graphics(GlobalParams *pGlobalParams, XRef *pXref, OutputDev *pOut, int nPageNumber, Dict *pResourcesDict, double dHorDPI, double dVerDPI, PDFRectangle *pBox, PDFRectangle *pCropBox, int nRotate, BOOL (*pAbortCheckCallBack)(void *pData) = NULL, void *pAbortCheckData = NULL);

	Graphics(GlobalParams *pGlobalParams, XRef *pXref, OutputDev *pOut, Dict *pResourcesDict, PDFRectangle *pBox, PDFRectangle *pCropBox, BOOL (*pAbortCheckCallBack)(void *pData) = NULL, void *pAbortCheckData = NULL);

	~Graphics();

	// Инициализируем Парсер по pObject, предварительно проводим проверку: Является ли pObject потоком или массивом потоков.
	void Display(Object *pObject, BOOL bTopLevel = TRUE);

	// Рисуем annotation
	void DrawAnnotation(Object *pStream, AnnotBorderStyle *pBorderStyle, double dMinX, double dMinY, double dMaxX, double dMaxY);

	// Сохраняем GState.
	void SaveGState();

	// Восстанавливаем предыдущий GState.
	void RestoreGState();

	// Считываем текущий GState.
	GrState *GetGState() 
	{
		return m_pGState; 
	}

private:

	void StartParse(BOOL bTopLevel);
	void ExecuteOperator(Object *pCommand, Object arrArguments[], int nArgumentsCount);
	Operator *FindOperator(char *sName);
	BOOL CheckArgumentType(Object *pArgument, ArgType eType);
	int GetPos();

	// Graphics state
	void OperatorSave              (Object arrArguments[], int nArgumentsCount);
	void OperatorRestore           (Object arrArguments[], int nArgumentsCount);
	void OperatorConcat            (Object arrArguments[], int nArgumentsCount);
	void OperatorSetDash           (Object arrArguments[], int nArgumentsCount);
	void OperatorSetFlat           (Object arrArguments[], int nArgumentsCount);
	void OperatorSetLineJoin       (Object arrArguments[], int nArgumentsCount);
	void OperatorSetLineCap        (Object arrArguments[], int nArgumentsCount);
	void OperatorSetMiterLimit     (Object arrArguments[], int nArgumentsCount);
	void OperatorSetLineWidth      (Object arrArguments[], int nArgumentsCount);
	void OperatorSetRenderingIntent(Object arrArguments[], int nArgumentsCount);
	void OperatorSetExtGState      (Object arrArguments[], int nArgumentsCount);
	void MakeSoftMask(Object *pStream, BOOL bAlpha, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, Function *pTransferFunc, GrColor *pBackdropColor);

	// Colour spaces
	void OperatorSetFillGray        (Object arrArguments[], int nArgumentsCount);
	void OperatorSetStrokeGray      (Object arrArguments[], int nArgumentsCount);
	void OperatorSetFillCMYKColor   (Object arrArguments[], int nArgumentsCount);
	void OperatorSetStrokeCMYKColor (Object arrArguments[], int nArgumentsCount);
	void OperatorSetFillRGBColor    (Object arrArguments[], int nArgumentsCount);
	void OperatorSetStrokeRGBColor  (Object arrArguments[], int nArgumentsCount);
	void OperatorSetFillColorSpace  (Object arrArguments[], int nArgumentsCount);
	void OperatorSetStrokeColorSpace(Object arrArguments[], int nArgumentsCount);
	void OperatorSetFillColor       (Object arrArguments[], int nArgumentsCount);
	void OperatorSetStrokeColor     (Object arrArguments[], int nArgumentsCount);
	void OperatorSetFillColorN      (Object arrArguments[], int nArgumentsCount);
	void OperatorSetStrokeColorN    (Object arrArguments[], int nArgumentsCount);

	// Path construction
	void OperatorMoveTo   (Object arrArguments[], int nArgumentsCount);
	void OperatorLineTo   (Object arrArguments[], int nArgumentsCount);
	void OperatorCurveTo  (Object arrArguments[], int nArgumentsCount);
	void OperatorCurveTo1 (Object arrArguments[], int nArgumentsCount);
	void OperatorCurveTo2 (Object arrArguments[], int nArgumentsCount);
	void OperatorRectangle(Object arrArguments[], int nArgumentsCount);
	void OperatorClosePath(Object arrArguments[], int nArgumentsCount);

	// Path-painting
	void OperatorEndPath          (Object arrArguments[], int nArgumentsCount);
	void OperatorStroke           (Object arrArguments[], int nArgumentsCount);
	void OperatorCloseStroke      (Object arrArguments[], int nArgumentsCount);
	void OperatorFill             (Object arrArguments[], int nArgumentsCount);
	void OperatorEOFill           (Object arrArguments[], int nArgumentsCount);
	void OperatorFillStroke       (Object arrArguments[], int nArgumentsCount);
	void OperatorCloseFillStroke  (Object arrArguments[], int nArgumentsCount);
	void OperatorEOFillStroke     (Object arrArguments[], int nArgumentsCount);
	void OperatorCloseEOFillStroke(Object arrArguments[], int nArgumentsCount);
	void OperatorShadingFill      (Object arrArguments[], int nArgumentsCount);
	void DoPatternFill(BOOL bEOFill);
	void DoPatternStroke();
	void DoTilingPatternFill (GrTilingPattern  *pPattern, BOOL bStroke, BOOL bEOFill);
	void DoShadingPatternFill(GrShadingPattern *pPattern, BOOL bStroke, BOOL bEOFill);
	void DoFunctionShadingFill(GrFunctionShading *pShading);
	void DoFunctionShadingFill(GrFunctionShading *pShading, double dMinX, double dMinY, double dMaxX, double dMaxY, GrColor *pColors, int nDepth);
	void DoAxialShadingFill   (GrAxialShading    *pShading);
	void DoRadialShadingFill  (GrRadialShading   *pShading);
	void DoGouraudTriangleShadingFill(GrGouraudTriangleShading *pShading);
	void GouraudFillTriangle(double dAx, double dAy, GrColor *pColorA, double dBx, double dBy, GrColor *pColorB, double dCx, double dCy, GrColor *pColorC, int nComponentsCount, int nDepth);
	void DoPatchMeshShadingFill(GrPatchMeshShading *pShading);
	void MeshFillPatch(GrPatch *pPatch, int nComponentsCount, int nDepth);
	void DoEndPath();

	// Clipping paths
	void OperatorClip  (Object arrArguments[], int nArgumentsCount);
	void OperatorEOClip(Object arrArguments[], int nArgumentsCount);

	// Text objects
	void OperatorBeginText(Object arrArguments[], int nArgumentsCount);
	void OperatorEndText  (Object arrArguments[], int nArgumentsCount);

	// Text state
	void OperatorSetCharSpacing (Object arrArguments[], int nArgumentsCount);
	void OperatorSetFont        (Object arrArguments[], int nArgumentsCount);
	void OperatorSetTextLeading (Object arrArguments[], int nArgumentsCount);
	void OperatorSetTextRender  (Object arrArguments[], int nArgumentsCount);
	void OperatorSetTextRise    (Object arrArguments[], int nArgumentsCount);
	void OperatorSetWordSpacing (Object arrArguments[], int nArgumentsCount);
	void OperatorSetHorizScaling(Object arrArguments[], int nArgumentsCount);

	// Text positioning
	void OperatorTextMove     (Object arrArguments[], int nArgumentsCount);
	void OperatorTextMoveSet  (Object arrArguments[], int nArgumentsCount);
	void OperatorSetTextMatrix(Object arrArguments[], int nArgumentsCount);
	void OperatorTextNextLine (Object arrArguments[], int nArgumentsCount);

	// Text showing
	void OperatorShowText       (Object arrArguments[], int nArgumentsCount);
	void OperatorMoveShowText   (Object arrArguments[], int nArgumentsCount);
	void OperatorMoveSetShowText(Object arrArguments[], int nArgumentsCount);
	void OperatorShowSpaceText  (Object arrArguments[], int nArgumentsCount);
	void DoShowText(StringExt *seString);

	// XObjects
	void OperatorXObject(Object arrArguments[], int nArgumentsCount);
	void DoImage(Object *pRef, Stream *pStream, BOOL bInlineImage);
	void DoForm(Object *pStream);
	void DoForm(Object *pStream, Dict *pResourcesDict, double *pMatrix, double *pBBox, BOOL bTranspGroup = FALSE, BOOL bSoftMask = FALSE, GrColorSpace *pBlendingColorSpace = NULL, BOOL bIsolated = FALSE, BOOL bKnockout = FALSE, BOOL bAlpha = FALSE, Function *pTransferFunctions = NULL, GrColor *pBackdropColor = NULL);

	// Inline images
	void OperatorBeginImage(Object arrArguments[], int nArgumentsCount);
	void OperatorImageData (Object arrArguments[], int nArgumentsCount);
	void OperatorEndImage  (Object arrArguments[], int nArgumentsCount);
	Stream *BuildImageStream();

	// Type 3 fonts
	void OperatorSetCharWidth  (Object arrArguments[], int nArgumentsCount);
	void OperatorSetCacheDevice(Object arrArguments[], int nArgumentsCount);

	// Compatibility
	void OperatorBeginIgnoreUndef(Object arrArguments[], int nArgumentsCount);
	void OperatorEndIgnoreUndef  (Object arrArguments[], int nArgumentsCount);

	// Marked content
	void OperatorBeginMarkedContent(Object arrArguments[], int nArgumentsCount);
	void OperatorEndMarkedContent  (Object arrArguments[], int nArgumentsCount);
	void OperatorMarkPoint         (Object arrArguments[], int nArgumentsCount);

	void PushResources(Dict *pResourcesDict);
	void PopResources();

private:

	XRef        *m_pXref;            // Таблица Xref для данного PDF - документа
	OutputDev   *m_pOut;             // Output device
	BOOL         m_bSubPage;         // Sub-page object?
	BOOL         m_bPrintCommands;   // print the drawing commands (for debugging)
	GrResources *m_pResources;       // Resources
	int          m_nUpdateLevel;

	GrState     *m_pGState;          // Текущий GState
	BOOL         m_bFontChanged;     // True, если шрифт или текстовая матрица изменились
	GrClipType   m_eClip;            // Clip type
	int          m_nIgnoreUndef;     // Текущий уровень вложенности для BX/EX
	double       m_arrBaseMatrix[6]; // Стандартная матрица для последних объектов Page/Form/Pattern
	int          m_nFormDepth;

	Parser      *m_pParser;          // Парсер для содержимого страницы

	BOOL       (*m_pAbortCheckCallBack)(void *pData); // Проверка на отмену
	void        *m_pAbortCheckData;

	static Operator OperatorsTable[]; // Таблица графических операторов

#ifdef _DEBUG
	FILE        *m_pDumpFile;          // Файл для распечатки команд, записанных в PDF
#endif

	GlobalParams *m_pGlobalParams;
};

#endif /* _GRAPHICS_H */
