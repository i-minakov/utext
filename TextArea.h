#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <QCompleter>
#include <QTextEdit>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QPlainTextEdit>

#define LENGTH 1

class TextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);
    ~TextEdit() override;

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

    QCompleter *m_completer = nullptr;
};

#endif
