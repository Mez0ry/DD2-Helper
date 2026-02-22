#include "HotKeyEdit.hpp"
#include "HotKeySequence.hpp"

void HotKeyEdit::enterEvent(QEnterEvent *event){
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_LineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_LineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred));

    connect(m_LineEdit, &QLineEdit::editingFinished, [this](){
        emit EditingFinished();
    });
}

void HotKeyEdit::leaveEvent(QEvent *event){;
    releaseMouse();
    m_LineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

HotKeyEdit::HotKeyEdit(QWidget *parent, const char delimiter) : QWidget(parent), m_Delimiter(delimiter) {
    this->setMouseTracking(true);

    m_KeySequenceEdit = new QKeySequenceEdit(this);

    m_KeySequenceEdit->hide();
    m_KeySequenceEdit->installEventFilter(this);

    m_LineEdit = new QLineEdit(this);
    m_LineEdit->setMouseTracking(true);
    m_LineEdit->setFocusPolicy(Qt::StrongFocus);
    m_LineEdit->setReadOnly(true);
    m_LineEdit->setPlaceholderText(QString("Press shortcut"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_LineEdit);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    setFocusPolicy(Qt::StrongFocus);
}

void HotKeyEdit::SetHotKey(const HotKeySequence &hotkey_sequence) {
    this->m_LineEdit->setText(QString(hotkey_sequence.GetSequenceAsStr().data()));

    qDebug() << m_LineEdit->text();
}

QString HotKeyEdit::GetText() const
{
    return m_LineEdit->text();
}

void HotKeyEdit::keyPressEvent(QKeyEvent *event) {
    m_KeySequenceEdit->setKeySequence(QKeySequence(event->modifiers() | event->key()));
    QString keys = m_KeySequenceEdit->keySequence().toString();
    m_FullSequence = keys;
    m_LineEdit->setText(keys);
    emit HotkeyChanged(m_FullSequence);
}

void HotKeyEdit::mousePressEvent(QMouseEvent *event) {
    QString button_names;

    if (event->buttons() & Qt::MiddleButton)     button_names += "MBUTTON ";
    if (event->buttons() & Qt::XButton1)         button_names += "X_BUTTON1 ";
    if (event->buttons() & Qt::XButton2)         button_names += "X_BUTTON2 ";

    if(button_names.isEmpty()){
        return;
    }

    m_FullSequence += m_Delimiter + button_names.trimmed();
    m_LineEdit->setText(m_FullSequence);
    emit HotkeyChanged(m_FullSequence);

    QWidget::mousePressEvent(event);
}
