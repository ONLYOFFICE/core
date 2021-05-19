#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QResizeEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextEdit>
#include <QDebug>
#include <QFile>

#include "../../../../graphics/pro/Fonts.h"
#include "../../../../graphics/pro/Graphics.h"
#include "../../../../fontengine/ApplicationFontsWorker.h"

#include "../../../../raster/BgraFrame.h"
#include "../../../../common/Directory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFont oFont;
    oFont.setPointSize(15);
    oFont.setBold(true);

    m_oRootTextFormat.setForeground(Qt::darkMagenta);
    m_oRootTextFormat.setFont(oFont);

    m_oRecordTextFormat.setForeground(Qt::blue);
    m_oRecordTextFormat.setFont(oFont);

    QFont oFont2;
    oFont2.setPointSize(15);

    m_oArgumentTextFormat.setForeground(Qt::darkGreen);
    m_oArgumentTextFormat.setFont(oFont2);

    m_oArgumentValueTextFormat.setForeground(Qt::red);
    m_oArgumentValueTextFormat.setFont(oFont2);

    m_oStandardTextFormat.setFont(oFont2);

    QFont oFont3;
    oFont3.setPointSize(15);
    oFont3.setItalic(true);

    m_oObjectTextFormat.setForeground(Qt::darkRed);
    m_oObjectTextFormat.setFont(oFont3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ReadFile()
{
    ui->DataEdit->clear();
    XmlUtils::CXmlNode oXmlNode;
    oXmlNode.FromXmlFile(m_wsPathToXmlFile);

    ReadXmlNode(oXmlNode);

    return true;
}

void MainWindow::ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, unsigned int unLevel)
{
    QTextCursor oTextCursor(ui->DataEdit->textCursor());

    QTextCharFormat oTextCharFormat;
    switch (unLevel)
    {
        case 0:
        {
            oTextCharFormat = m_oRootTextFormat;
            break;
        }
        case 1:
        {
            oTextCharFormat = m_oRecordTextFormat;
            break;
        }
        default:
        {
            oTextCharFormat = m_oObjectTextFormat;
            break;
        }

    }

    AddIndent(unLevel);
    if (0 >= oXmlNode.GetAttributesCount())
        oTextCursor.insertText(QString::fromStdWString(L'<' + oXmlNode.GetName() + L'>'), oTextCharFormat);
    else
    {
            oTextCursor.insertText(QString::fromStdWString(L'<' + oXmlNode.GetName()), oTextCharFormat);

            std::vector<std::wstring> arNameArguments, atValueArguments;
            oXmlNode.GetAllAttributes(arNameArguments, atValueArguments);

            for (unsigned int i = 0; i < arNameArguments.size(); ++i)
            {
                    oTextCursor.insertText(QString::fromStdWString(L' ' + arNameArguments[i] + L'='), m_oArgumentTextFormat);
                    oTextCursor.insertText(QString::fromStdWString(L'"' + atValueArguments[i] + L'"'), m_oArgumentValueTextFormat);
            }

            oTextCursor.insertText(">", oTextCharFormat);
    }

    XmlUtils::CXmlNodes oXmlChilds;
    bool bEndNode = false;

    if (oXmlNode.GetChilds(oXmlChilds))
    {
        oTextCursor.insertText("\n", oTextCharFormat);
        for (unsigned int i = 0; i < oXmlChilds.GetCount(); ++i)
        {
            XmlUtils::CXmlNode oXmlChild;
            if (oXmlChilds.GetAt(i, oXmlChild))
                ReadXmlNode(oXmlChild, unLevel + 1);
        }
        AddIndent(unLevel);
    }
    else
    {
        std::wstring wsText = oXmlNode.GetText();

        if (wsText.empty())
        {
           oTextCursor.deletePreviousChar();
           oTextCursor.insertText("/>\n", oTextCharFormat);
           bEndNode = true;
        }
        else
            oTextCursor.insertText(QString::fromStdWString(oXmlNode.GetText()), m_oStandardTextFormat);

    }

    if (!bEndNode)
        oTextCursor.insertText(QString::fromStdWString(L"</" + oXmlNode.GetName() + L">\n"), oTextCharFormat);

}

void MainWindow::AddIndent(unsigned int unLevel)
{
    QTextCursor oTextCursor(ui->DataEdit->textCursor());
    oTextCursor.insertText(QString(4 * unLevel, ' '), m_oRootTextFormat);
}

void MainWindow::WriteFile()
{
    QFile out(QString::fromStdWString(m_wsPathToXmlFile));

    if (out.open(QIODevice::WriteOnly))
    {
        out.write(ui->DataEdit->toPlainText().toUtf8());
        out.close();
        QMessageBox::information(this, "Information", "Saving the file was successful");
    }
    else
        QMessageBox::warning(this, "Warning", "Couldn't open file for saving");
}

void MainWindow::ConvertToRaster()
{
    if (m_wsPathToFile.empty())
        return;

    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
    pMetafile->LoadFromFile(m_wsPathToFile.c_str());
    std::wstring wsPathToRasterFile = NSFile::GetProcessDirectory() + L"/test.png";
    m_wsPathToXmlFile = NSFile::GetProcessDirectory() + L"/test.xml";

    pMetafile->ConvertToRaster(wsPathToRasterFile.c_str(), 4, ui->customView->GetWidthMetafile(),  ui->customView->GetHeightMetafile());

    pMetafile->Release();
    pFonts->Release();

    QImage oImage(QString::fromStdWString(wsPathToRasterFile));
    QGraphicsScene *pGraphicsScene = new QGraphicsScene;
    pGraphicsScene->addPixmap(QPixmap::fromImage(QImage(oImage)));
    ui->graphicsView->setScene(pGraphicsScene);
}

void MainWindow::resizeEvent(QResizeEvent *pResizeEvent)
{
    int h = pResizeEvent->size().height();
    int w = pResizeEvent->size().width();

    ui->horizontalLayoutWidget->setGeometry(QRect(0, 0, w, h));
}

void MainWindow::on_SaveButton_clicked()
{
    WriteFile();
}

void MainWindow::on_ChangeButton_clicked()
{
    m_wsPathToFile = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Metafile (*.emf *.wmf)")).toStdWString();
    ui->customView->DrawMetafile(m_wsPathToFile);
    ConvertToRaster();
    ReadFile();
}
