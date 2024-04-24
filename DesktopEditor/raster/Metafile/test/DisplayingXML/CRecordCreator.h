/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#ifndef CRECORDCREATOR_H
#define CRECORDCREATOR_H

#include <QDialog>
#include <QStandardItem>

#include "MainWindow.h"
#include "Common/CSharedWidget.h"

namespace Ui {
        class CRecordCreator;
}

class CRecordCreator : public QDialog
{
        Q_OBJECT

public:
        explicit CRecordCreator(QWidget *parent = nullptr);
        virtual ~CRecordCreator();

        void SetMainWindow(MainWindow *pMainWindow);
        QStandardItem* CreateRecord();
private slots:
        void on_selectButton_clicked();
private:
        void CreatePointWidgets(const QString& qsName);
        void CreatePolyWidgets(const QString& qsName);
        void CreatePolyDrawWidgets(const QString& qsName);
        void CreateEXTEX(const QString& qsName);
        void CreateORGEX(const QString& qsName);
        void CreateFormWidget(const QString& qsName, const QString& qsFormName);
        void CreateColorWidgets(const QString& qsName, bool bBounds = false);
        void CreateEmptyWidgets(const QString& qsName);
        void CreateRectangleWidgets(const QString& qsName, const QString& qsReactangleName);
        void CreateScaleWidgets(const QString& qsName);
        void CreateArcWidgets(const QString& qsName);
        void CreateNotSupportedWidgets(const QString &qsName);
        void CreateExtTextOutWidget(const QString& qsName);
        void CreatePolyTextOutWidget(const QString& qsName);
        void CreateSetICMPRrofileWidget(const QString& qsName);

        void Create_Widgets_EMR_SETCOLORADJUSTMENT();
        void Create_Widgets_EMR_SETWORLDTRANSFORM();
        void Create_Widgets_EMR_MODIFYWORLDTRANSFORM();
        void Create_Widgets_EMR_CREATEPEN();
        void Create_Widgets_EMR_CREATEBRUSHINDIRECT();
        void Create_Widgets_EMR_ANGLEARC();
        void Create_Widgets_EMR_ROUNDRECT();
        void Create_Widgets_EMR_CREATEPALETTE();
        void Create_Widgets_EMR_SETPALETTEENTRIES();
        void Create_Widgets_EMR_RESIZEPALETTE();
        void Create_Widgets_EMR_EXTFLOODFILL();
        void Create_Widgets_EMR_LINETO();
        void Create_Widgets_EMR_FILLRGN();
        void Create_Widgets_EMR_FRAMERGN();
        void Create_Widgets_EMR_INVERTRGN();
        void Create_Widgets_EMR_PAINTRGN();
        void Create_Widgets_EMR_EXTSELECTCLIPRGN();
        void Create_Widgets_EMR_EXTCREATEFONTINDIRECTW();
        void Create_Widgets_EMR_CREATECOLORSPACE();
        void Create_Widgets_EMR_GLSBOUNDEDRECORD();
        void Create_Widgets_EMR_PIXELFORMAT();
        void Create_Widgets_EMR_SMALLTEXTOUT();
        void Create_Widgets_EMR_FORCEUFIMAPPING();
        void Create_Widgets_EMR_NAMEDESCAPE();
        void Create_Widgets_EMR_COLORCORRECTPALETTE();
        void Create_Widgets_EMR_GRADIENTFILL();
        void Create_Widgets_EMR_SETLINKEDUFIS();
        void Create_Widgets_EMR_SETTEXTJUSTIFICATION();
        void Create_Widgets_EMR_COLORMATCHTOTARGETW();
        void Create_Widgets_EMR_CREATECOLORSPACEW();

        void ConvertWidgets(const QString& qsName);
        void AddSharedWidget(CSharedWidget* pWidget);
        void AddSharedWidgets(std::vector<CSharedWidget*> arWidgets);

        void ClearData();
        void Close();

        Ui::CRecordCreator *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pNewItem;

        int m_nSelectedRecord;

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CRECORDCREATOR_H
