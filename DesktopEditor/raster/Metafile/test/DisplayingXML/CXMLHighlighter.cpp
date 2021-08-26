#include "CXMLHighlighter.h"

#include <QPainter>
#include <QSize>

bool SplitString(const QString &str, int n, QStringList &list);

CXMLHighlighter::CXMLHighlighter(QObject * parent) :
        QSyntaxHighlighter(parent)
{
        SetRegexes();
        SetFormats();
}

CXMLHighlighter::CXMLHighlighter(QTextDocument * parent) :
        QSyntaxHighlighter(parent)
{
        SetRegexes();
        SetFormats();
}

CXMLHighlighter::CXMLHighlighter(QTextEdit * parent) :
        QSyntaxHighlighter(parent)
{
        SetRegexes();
        SetFormats();
}

void CXMLHighlighter::PaintByRegex(const QString &qsText, QPainter* pPainter, const QRect& oRect, QFont* pFont)
{
    setPointSize(pFont);

    QStringList arTextValues = GetStringByRegex(qsText, m_xmlTextFormat, m_xmlTextRegex);
    QStringList arElementValues = GetStringByRegex(qsText, m_xmlElementFormat, m_xmlElementRegex);

    QStringList arKeywordValues;

    typedef QList<QRegExp>::const_iterator Iter;
    Iter xmlKeywordRegexesEnd = m_xmlKeywordRegexes.end();
    for(Iter it = m_xmlKeywordRegexes.begin(); it != xmlKeywordRegexesEnd; ++it)
    {
        const QRegExp & regex = *it;
        arKeywordValues += GetStringByRegex(qsText, m_xmlKeywordFormat, regex);
    }

    QStringList arAttributeValues = GetStringByRegex(qsText, m_xmlAttributeFormat, m_xmlAttributeRegex);
    QStringList arCommentValues = GetStringByRegex(qsText, m_xmlCommentFormat, m_xmlCommentRegex);
    QStringList arValueValues = GetStringByRegex(qsText, m_xmlValueFormat, m_xmlValueRegex);

    unsigned int unX = oRect.x();
    unsigned int unY = oRect.y() + QFontMetrics((NULL != pFont) ? *pFont : QFont()).height();

    pPainter->save();

    pPainter->setPen(m_xmlKeywordFormat.foreground().color());
    pPainter->setFont(m_xmlKeywordFormat.font());

    pPainter->drawText(unX, unY, "<");

    unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), "<").width();

    if (!arAttributeValues.empty() && !arElementValues.empty())
        arElementValues[0] = arElementValues[0].mid(0, arElementValues[0].indexOf(' '));

    if (!arElementValues.empty())
    {
        pPainter->setPen(m_xmlElementFormat.foreground().color());
        pPainter->setFont(m_xmlElementFormat.font());

        pPainter->drawText(unX, unY, arElementValues[0]);

        unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), arElementValues[0] + ((!arAttributeValues.empty()) ? " " : "")).width();
    }

    if (!arAttributeValues.empty() && arAttributeValues.size() == arValueValues.size())
    {
        for (unsigned int i = 0; i < arAttributeValues.size(); ++i)
        {
            pPainter->setPen(m_xmlAttributeFormat.foreground().color());
            pPainter->setFont(m_xmlAttributeFormat.font());

            pPainter->drawText(unX, unY, arAttributeValues[i]);

            unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), arAttributeValues[i]).width();

            pPainter->setPen(m_xmlElementFormat.foreground().color());
            pPainter->setFont(m_xmlElementFormat.font());

            pPainter->drawText(unX, unY, "=");

            unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), "=").width();

            pPainter->setPen(m_xmlValueFormat.foreground().color());
            pPainter->setFont(m_xmlValueFormat.font());

            pPainter->drawText(unX, unY, arValueValues[i]);

            unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), arValueValues[i]).width();
        }
    }

    pPainter->setPen(m_xmlKeywordFormat.foreground().color());
    pPainter->setFont(m_xmlKeywordFormat.font());

    pPainter->drawText(unX, unY, ">");

    unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), ">").width();

    if (arElementValues.size() > 1)
    {
        unsigned int unStart = qsText.indexOf('>') + 1;
        unsigned int unEnd   = qsText.indexOf('<', unStart);

        if (unStart != unEnd)
            arTextValues.append(qsText.mid(unStart, unEnd - unStart));

        if (!arTextValues.empty())
        {
            QStringList arTexts;
            SplitString(arTextValues[0], 64, arTexts);

            pPainter->setPen(m_xmlTextFormat.foreground().color());
            pPainter->setFont(m_xmlTextFormat.font());

            QFontMetrics oFontMetrics(m_xmlTextFormat.font());

            for (unsigned int i = 0; i < arTexts.size(); ++i)
            {
                if (i != 0)
                    unX = oRect.x();

                pPainter->drawText(unX, unY, arTexts[i]);

                unX += oFontMetrics.width(arTexts[i]);
                if (i != arTexts.size() - 1)
                    unY += oFontMetrics.height();
            }

        }

        pPainter->setPen(m_xmlKeywordFormat.foreground().color());
        pPainter->setFont(m_xmlKeywordFormat.font());

        pPainter->drawText(unX, unY, "</");

        unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), "</").width();

        pPainter->setPen(m_xmlElementFormat.foreground().color());
        pPainter->setFont(m_xmlElementFormat.font());

        pPainter->drawText(unX, unY, arElementValues[0]);

        unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), arElementValues[0]).width();

        pPainter->setPen(m_xmlKeywordFormat.foreground().color());
        pPainter->setFont(m_xmlKeywordFormat.font());

        pPainter->drawText(unX, unY, ">");

        unX += pPainter->boundingRect(QRect(unX, oRect.y(), oRect.width(), oRect.height()), ">").width();
    }

    pPainter->restore();
}

void CXMLHighlighter::highlightBlock(const QString & text)
{
    highlightByRegex(m_xmlTextFormat, m_xmlTextRegex, text);
    highlightByRegex(m_xmlElementFormat, m_xmlElementRegex, text);

    typedef QList<QRegExp>::const_iterator Iter;
    Iter xmlKeywordRegexesEnd = m_xmlKeywordRegexes.end();
    for(Iter it = m_xmlKeywordRegexes.begin(); it != xmlKeywordRegexesEnd; ++it)
    {
        const QRegExp & regex = *it;
        highlightByRegex(m_xmlKeywordFormat, regex, text);
    }

    highlightByRegex(m_xmlAttributeFormat, m_xmlAttributeRegex, text);
    highlightByRegex(m_xmlCommentFormat, m_xmlCommentRegex, text);
    highlightByRegex(m_xmlValueFormat, m_xmlValueRegex, text);
}

void CXMLHighlighter::highlightByRegex(const QTextCharFormat & format,
                                                 const QRegExp & regex, const QString & text)
{
    int index = regex.indexIn(text);

    while(index >= 0)
    {
        int matchedLength = regex.matchedLength();
        setFormat(index, matchedLength, format);

        index = regex.indexIn(text, index + matchedLength);
    }
}

QStringList CXMLHighlighter::GetStringByRegex(const QString& qsText, const QTextCharFormat & format, const QRegExp & regex)
{
    int index = regex.indexIn(qsText);

    QStringList arStrings;

    while(index >= 0)
    {
        int matchedLength = regex.matchedLength();

        QString qsTextPart = qsText.mid(index, matchedLength);

        if (format == m_xmlTextFormat)
            qsTextPart = qsTextPart.mid(1, qsTextPart.length() - 2);
        else if (format == m_xmlElementFormat)
            qsTextPart = qsTextPart.mid(1);

        arStrings.append(qsTextPart);

        index = regex.indexIn(qsText, index + matchedLength);
    }

    return arStrings;
}

void CXMLHighlighter::setPointSize(QFont *pFont)
{
    if (NULL == pFont)
        return;

    m_xmlKeywordFormat.setFontPointSize(pFont->pointSize());
    m_xmlElementFormat.setFontPointSize(pFont->pointSize());
    m_xmlAttributeFormat.setFontPointSize(pFont->pointSize());
    m_xmlValueFormat.setFontPointSize(pFont->pointSize());
    m_xmlCommentFormat.setFontPointSize(pFont->pointSize());
    m_xmlTextFormat.setFontPointSize(pFont->pointSize());
}

void CXMLHighlighter::SetRegexes()
{
    m_xmlElementRegex.setPattern("<[?\\s]*[/]?[\\s]*([^\\n][^>]*)(?=[\\s/>])");
    m_xmlAttributeRegex.setPattern("\\w+(?=\\=)");
    m_xmlValueRegex.setPattern("\"[^\\n\"]+\"(?=[?\\s/>])");
    m_xmlCommentRegex.setPattern("<!--[^\\n]*-->");
    m_xmlTextRegex.setPattern(">[^\\n]*<");

    m_xmlKeywordRegexes = QList<QRegExp>() << QRegExp("<\\?") << QRegExp("/>")
                                           << QRegExp(">") << QRegExp("<") << QRegExp("</")
                                           << QRegExp("\\?>");
}

void CXMLHighlighter::SetFormats()
{
    m_xmlKeywordFormat.setForeground(Qt::blue);
    m_xmlKeywordFormat.setFontWeight(QFont::Bold);

    m_xmlElementFormat.setForeground(Qt::darkMagenta);
    m_xmlElementFormat.setFontWeight(QFont::Bold);

    m_xmlAttributeFormat.setForeground(Qt::darkGreen);
    m_xmlAttributeFormat.setFontWeight(QFont::Bold);
    m_xmlAttributeFormat.setFontItalic(true);

    m_xmlValueFormat.setForeground(Qt::darkRed);

    m_xmlCommentFormat.setForeground(Qt::gray);

    m_xmlTextFormat.setForeground(Qt::black);
    m_xmlTextFormat.setFontWeight(QFont::Bold);
}

bool SplitString(const QString &str, int n, QStringList &list)
{
    if (n < 1)
        return false;

    QString tmp(str);

    list.clear();

    while (!tmp.isEmpty())
    {
        list.append(tmp.left(n));
        tmp.remove(0, n);
    }

    return true;
}
