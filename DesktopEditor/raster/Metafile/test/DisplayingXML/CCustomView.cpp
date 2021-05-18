#include "CCustomView.h"

#include <QPixmap>
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat=0);

#include <Windows.h>
using namespace std; // устраняет ошибки с min, max при компиляции (связано с подключением gdiplus)
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"gdiplus.lib")

CCustomView::CCustomView(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsView::paintEngine();
}

CCustomView::~CCustomView()
{

}

void CCustomView::DrawMetafile(std::wstring wsFilePath)
{

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    Gdiplus::Bitmap * bitmap = Gdiplus::Bitmap::FromFile(wsFilePath.c_str());

    HBITMAP handleToSliceRet = NULL;
    bitmap->GetHBITMAP(Gdiplus::Color::Transparent, &handleToSliceRet);
    QPixmap oPixmap = qt_pixmapFromWinHBITMAP(handleToSliceRet);

    QGraphicsScene *pScene = new QGraphicsScene();

    pScene->addPixmap(oPixmap);

    setScene(pScene);

    delete bitmap;
}
