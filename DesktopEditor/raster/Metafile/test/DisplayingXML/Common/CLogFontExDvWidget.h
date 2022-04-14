#ifndef CLOGFONTEXDV_H
#define CLOGFONTEXDV_H

#include "CSharedWidget.h"

class CLogFontExDvWidget : public CSharedWidget
{
public:
        explicit CLogFontExDvWidget(QWidget *pParent = NULL);
        virtual ~CLogFontExDvWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

class CLogFontExWidget : public CSharedWidget
{
public:
        explicit CLogFontExWidget(QWidget *pParent = NULL);
        virtual ~CLogFontExWidget();
};

class CLogFontWidget : public CSharedWidget
{
public:
        explicit CLogFontWidget(QWidget *pParent = NULL);
        virtual ~CLogFontWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

class CDesignVectorWidget : public CSharedWidget
{
public:
        explicit CDesignVectorWidget(QWidget *pParent = NULL);
        virtual ~CDesignVectorWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createValue_clicked();
private:
        unsigned int m_unCounter;
};

class CLinkedUFISWidget  : public CSharedWidget
{
public:
        explicit CLinkedUFISWidget(QWidget *pParent = NULL);
        virtual ~CLinkedUFISWidget();
private slots:
        void on_createValue_clicked();
private:
        unsigned int m_unCounter;
};

class CUniversalFontIdWidget : public CSharedWidget
{
public:
        explicit CUniversalFontIdWidget(unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CUniversalFontIdWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        unsigned int m_unIndex;
};

#endif // CLOGFONTEXDV_H
