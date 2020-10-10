#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QtGui>
#include <QHash>
#include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

protected:
    void listPattern(QStringList keywordPatterns, QColor color);
    void hashTag();
    void singleComment();
    void quotationFormat();
    void functionFormat();
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule {
        QRegExp m_pattern;
        QTextCharFormat m_format;
    };
    QRegExp m_commentStartExpression;
    QRegExp m_commentEndExpression;
    QTextCharFormat m_multiLineCommentFormat;
    QVector<HighlightingRule> m_highlightingRules;
};

#endif
