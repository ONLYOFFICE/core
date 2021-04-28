#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QResizeEvent>
#include <QMessageBox>
#include <QTextEdit>
#include <QDebug>
#include <QFile>

#include "../../../../common/Directory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage oImage(QString::fromStdWString(NSFile::GetProcessDirectory() + L"//test.png"));
    QGraphicsScene *pGraphicsScene = new QGraphicsScene;
    pGraphicsScene->addPixmap(QPixmap::fromImage(QImage(oImage)));
    ui->graphicsView->setScene(pGraphicsScene);

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

    m_wsFileName = NSFile::GetProcessDirectory() + L"//test.xml";
    ReadFile();
    QTextCursor cursor = ui->DataEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->DataEdit->setTextCursor(cursor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ReadFile()
{
    XmlUtils::CXmlNode oXmlNode;
    oXmlNode.FromXmlFile(m_wsFileName);

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
    QFile out(QString::fromStdWString(m_wsFileName));

    if (out.open(QIODevice::WriteOnly))
    {
        out.write(ui->DataEdit->toPlainText().toUtf8());
        out.close();
        QMessageBox::information(this, "Information", "Saving the file was successful");
    }
    else
        QMessageBox::warning(this, "Warning", "Couldn't open file for saving");
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
