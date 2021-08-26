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
        explicit CCustomView(QWidget *parent);
        virtual ~CCustomView();

        unsigned int GetHeightMetafile() const;
        unsigned int GetWidthMetafile() const;

        bool DrawMetafile(const std::wstring& wsFilePath);
        void Clear();

        virtual void wheelEvent(QWheelEvent * event) override;
    private:
        Gdiplus::Bitmap *m_pBitmap;

};

#endif // CCUSTOMVIEW_H
