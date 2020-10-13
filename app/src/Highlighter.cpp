#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b"     << "\\bclass\\b"        << "\\bconst\\b"
                    << "\\bfriend\\b"   << "\\binline\\b"       << "\\bint\\b"
                    << "\\blong\\b"     << "\\bnamespace\\b"    << "\\boperator\\b"
                    << "\\bprivate\\b"  << "\\bprotected\\b"    << "\\bpublic\\b"
                    << "\\bshort\\b"    << "\\bsignals\\b"      << "\\bsigned\\b"
                    << "\\btemplate\\b" << "\\bunion\\b"        << "\\bunsigned\\b"
                    << "\\bdouble\\b"   << "\\benum\\b"         << "\\bvoid\\b"
                    << "\\bslots\\b"    << "\\bstruct\\b"       << "\\bvolatile\\b";
    listPattern(keywordPatterns, QColor(102, 217, 239));
    keywordPatterns.clear();
    keywordPatterns << "\\bexplicit\\b" << "\\bconst\\b"        << "\\bdefault\\b"
                    << "\\bvirtual\\b"  << "\\btypename\\b"     << "\\bstatic\\b"
                    << "\\binline\\b"   << "\\btypedef\\b";
    listPattern(keywordPatterns, QColor(233, 40, 108));
    hashTag();
    quotationFormat();
    singleComment();
    functionFormat();
    m_multiLineCommentFormat.setForeground(QColor(132, 129, 108));
    m_commentStartExpression = QRegExp("/\\*");
    m_commentEndExpression = QRegExp("\\*/");
}

void Highlighter::listPattern(QStringList keywordPatterns, QColor color) {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(color);
    keywordFormat.setFontWeight(QFont::Bold);
    foreach (const QString &pattern, keywordPatterns) {
        rule.m_pattern = QRegExp(pattern);
        rule.m_format = keywordFormat;
        m_highlightingRules.append(rule);
    }
}

void Highlighter::singleComment() {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(132, 129, 108));
    rule.m_pattern = QRegExp("//[^\n]*");
    rule.m_format = keywordFormat;
    m_highlightingRules.append(rule);
}

void Highlighter::hashTag() {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(233, 40, 108));
    rule.m_pattern = QRegExp("\\#\\w+");
    rule.m_format = keywordFormat;
    m_highlightingRules.append(rule);
}

void Highlighter::quotationFormat() {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(228, 217, 115));
    rule.m_pattern = QRegExp("\".*\"|<.*>");
    rule.m_format = keywordFormat;
    m_highlightingRules.append(rule);
}

void Highlighter::functionFormat() {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setFontItalic(true);
    keywordFormat.setForeground(QColor(166, 226, 46));
    rule.m_pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.m_format = keywordFormat;
    m_highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, m_highlightingRules) {
        QRegExp expression(rule.m_pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.m_format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = m_commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = m_commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
             commentLength = endIndex - startIndex + m_commentEndExpression.matchedLength();
        setFormat(startIndex, commentLength, m_multiLineCommentFormat);
        startIndex = m_commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
