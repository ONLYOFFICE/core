#ifndef CLOGFONTEXDV_H
#define CLOGFONTEXDV_H

#include "CSharedWidget.h"

class CLogFontExDv : public CSharedWidget
{
public:
        explicit CLogFontExDv(QWidget *pParent = NULL);
        virtual ~CLogFontExDv();

        virtual QList<QStandardItem*> GetData() const override;
};

class CLogFontEx : public CSharedWidget
{
public:
        explicit CLogFontEx(QWidget *pParent = NULL);
        virtual ~CLogFontEx();

        virtual QList<QStandardItem*> GetData() const override;
};

class CLogFont : public CSharedWidget
{
public:
        explicit CLogFont(QWidget *pParent = NULL);
        virtual ~CLogFont();

        virtual QList<QStandardItem*> GetData() const override;
};

class CDesignVector : public CSharedWidget
{
public:
        explicit CDesignVector(QWidget *pParent = NULL);
        virtual ~CDesignVector();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createValue_clicked();
private:
        unsigned int m_unCounter;
};

class CUniversalFontIdWidget : public CSharedWidget
{
public:
        explicit CUniversalFontIdWidget(QWidget *pParent = NULL);
        virtual ~CUniversalFontIdWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CLOGFONTEXDV_H
