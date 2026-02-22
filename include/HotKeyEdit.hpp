#ifndef __HOTKEY__EDIT_HPP__
#define __HOTKEY__EDIT_HPP__

#include <QApplication>
#include <QDebug>
#include <QKeySequence>
#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QMetaProperty>
#include <QMouseEvent>
#include <QWidget>
#include <qabstractspinbox.h>
#include <qboxlayout.h>
#include <QToolTip>

class HotKeySequence;

class HotKeyEdit : public QWidget {
    Q_OBJECT
private:
    QKeySequenceEdit *m_KeySequenceEdit;
    QLineEdit *m_LineEdit;
    QString m_FullSequence;
    char m_Delimiter;
public:
    HotKeyEdit(QWidget *parent = nullptr, const char delimiter = '+');

    void SetHotKey(const HotKeySequence& hotkey_sequence);

    QString GetText() const;
signals:
    bool HotkeyChanged(const QString& new_hotkey);
    bool EditingFinished() const;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // __HOTKEY__EDIT_HPP__
