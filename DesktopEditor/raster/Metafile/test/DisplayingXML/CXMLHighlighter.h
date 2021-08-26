#ifndef CXMLHIGHLIGHTER_H
#define CXMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextEdit>

class CXMLHighlighter : public QSyntaxHighlighter
{
        Q_OBJECT
    public:
        CXMLHighlighter(QObject * parent);
        CXMLHighlighter(QTextDocument * parent);
        CXMLHighlighter(QTextEdit * parent);

        void PaintByRegex(const QString&    qsText,
                                QPainter*   pPainter,
                          const QRect&      oRect,
                                QFont*      pFont = NULL);

    protected:
        virtual void highlightBlock(const QString & text);

    private:
        void    highlightByRegex(const QTextCharFormat & format,
                                 const QRegExp & regex,
                                 const QString & text);

        QStringList GetStringByRegex(   const QString& qsText,
                                        const QTextCharFormat & format,
                                        const QRegExp & regex);

        void setPointSize(QFont *pFont);
        void SetRegexes();
        void SetFormats();

    private:
        QTextCharFormat     m_xmlKeywordFormat;
        QTextCharFormat     m_xmlElementFormat;
        QTextCharFormat     m_xmlAttributeFormat;
        QTextCharFormat     m_xmlValueFormat;
        QTextCharFormat     m_xmlCommentFormat;
        QTextCharFormat     m_xmlTextFormat;

        QList<QRegExp>      m_xmlKeywordRegexes;
        QRegExp             m_xmlElementRegex;
        QRegExp             m_xmlAttributeRegex;
        QRegExp             m_xmlValueRegex;
        QRegExp             m_xmlCommentRegex;
        QRegExp             m_xmlTextRegex;
};

#endif // CXMLHIGHLIGHTER_H
