#include "TextArea.h"

TextEdit::TextEdit(QWidget *parent) : QPlainTextEdit(parent) {
    connect(m_zoomIn, &QShortcut::activated, [=](){
        QFont f = font();
        f.setPointSize(f.pointSize() + 2);
        this->setFont(f);
    });
    connect(m_zoomOut, &QShortcut::activated, [=](){
        QFont f = font();
        f.setPointSize(f.pointSize() - 2);
        this->setFont(f);
    });
}

TextEdit::~TextEdit() {

}

void TextEdit::setCompleter(QCompleter *completer) {
    if (m_completer)
        m_completer->disconnect(this);
    m_completer = completer;
    if (!m_completer)
        return;
    m_completer->setWidget(this);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &TextEdit::insertCompletion);
}

QCompleter *TextEdit::completer() const {
    return m_completer;
}

void TextEdit::insertCompletion(const QString &completion) {
    if (m_completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - m_completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString TextEdit::textUnderCursor() const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void TextEdit::focusInEvent(QFocusEvent *e) {
    if (m_completer)
        m_completer->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void TextEdit::keyPressEvent(QKeyEvent *e) {
    if (m_completer && m_completer->popup()->isVisible()) {
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return;
       default:
           break;
       }
    }
    const bool isShortcut = (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_E);
    if (!m_completer || !isShortcut)
        QPlainTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
                             e->modifiers().testFlag(Qt::ShiftModifier);
   if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
       return;
   static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
   const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
   QString completionPrefix = textUnderCursor();
   if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < LENGTH
                     || eow.contains(e->text().right(1)))) {
       m_completer->popup()->hide();
       return;
   }
   if (completionPrefix != m_completer->completionPrefix()) {
       m_completer->setCompletionPrefix(completionPrefix);
       m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
   }
   QRect cr = cursorRect();
   cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
               + m_completer->popup()->verticalScrollBar()->sizeHint().width());
   m_completer->complete(cr);
}
