#ifndef CCUSTOMVIEW_H
#define CCUSTOMVIEW_H

#include <QGraphicsView>

class CCustomView : public QGraphicsView
{
    Q_OBJECT
public:
    CCustomView(QWidget *parent);
    ~CCustomView();

    void DrawMetafile(std::wstring wsFilePath);

};

#endif // CCUSTOMVIEW_H
