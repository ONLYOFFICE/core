#include "CCustomView.h"

#include <QPixmap>
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat=0);

CCustomView::CCustomView(QWidget *parent)
    : QGraphicsView(parent),
      m_pBitmap(NULL)
{}

CCustomView::~CCustomView()
{
    Clear();
}

unsigned int CCustomView::GetHeightMetafile() const
{
    return m_pBitmap->GetHeight();
}

unsigned int CCustomView::GetWidthMetafile() const
{
    return m_pBitmap->GetWidth();
}

void CCustomView::DrawMetafile(const std::wstring& wsFilePath)
{

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    Clear();
    m_pBitmap = Gdiplus::Bitmap::FromFile(wsFilePath.c_str());

    HBITMAP handleToSliceRet = NULL;
    m_pBitmap->GetHBITMAP(Gdiplus::Color::Transparent, &handleToSliceRet);
    QPixmap oPixmap = qt_pixmapFromWinHBITMAP(handleToSliceRet);

    QGraphicsScene *pScene = new QGraphicsScene();

    pScene->addPixmap(oPixmap);

    setScene(pScene);
}

void CCustomView::Clear()
{
    if (NULL != m_pBitmap)
        delete m_pBitmap;
}
