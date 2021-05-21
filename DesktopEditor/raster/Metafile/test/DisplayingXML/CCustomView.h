#ifndef CCUSTOMVIEW_H
#define CCUSTOMVIEW_H

#include <QGraphicsView>

#include <Windows.h>
using namespace std; // устраняет ошибки с min, max при компиляции (связано с подключением gdiplus)
#include <gdiplus.h>
#pragma comment (lib,"gdiplus.lib")

class CCustomView : public QGraphicsView
{
    Q_OBJECT
public:
    CCustomView(QWidget *parent);
    ~CCustomView();

    unsigned int GetHeightMetafile() const;
    unsigned int GetWidthMetafile() const;

    void DrawMetafile(const std::wstring& wsFilePath);
private:
    Gdiplus::Bitmap *m_pBitmap;
    void Clear();

};

#endif // CCUSTOMVIEW_H
