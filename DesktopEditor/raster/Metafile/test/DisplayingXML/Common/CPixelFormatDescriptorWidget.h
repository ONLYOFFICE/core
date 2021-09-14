#ifndef CPIXELFORMATDESCRIPTORWDIGET_H
#define CPIXELFORMATDESCRIPTORWDIGET_H

#include "CSharedWidget.h"

class CPixelFormatDescriptorWidget : public CSharedWidget
{
public:
        explicit CPixelFormatDescriptorWidget(QWidget *pParent = NULL);
        virtual ~CPixelFormatDescriptorWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CPIXELFORMATDESCRIPTOR_H
