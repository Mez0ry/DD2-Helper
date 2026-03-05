#include "Popup.hpp"
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QSequentialAnimationGroup>
#include <QVBoxLayout>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qscreen.h>
#include <qsystemtrayicon.h>
#include <qtimer.h>

internal::IPopup::IPopup(const QString& title, const QString& description, const QList<std::shared_ptr<QPushButton>>& buttons, int show_duration_ms, const QIcon& title_icon,  QWidget* parent)
    : QWidget(parent), m_ShowDuration(show_duration_ms), m_CloseButton(new QPushButton(QIcon("://resources/icons/generic/close.svg"), "")),
    m_TitleLabel(new QLabel(title)),
    m_DescriptionLabel(new QLabel(description)),
    m_OptionalButtons(buttons){

    this->setObjectName("IPopup");
    m_CloseButton->setObjectName("CloseButton");

    m_CloseButton->setMinimumSize(QSize(8,8));
    m_CloseButton->setMaximumSize(QSize(14, 14));

    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(false);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

    m_TitleLabel->setWordWrap(true);
    m_TitleLabel->setObjectName("TitleLabel");

    m_DescriptionLabel->setWordWrap(true);
    m_DescriptionLabel->setObjectName("DescriptionLabel");

    m_TitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_DescriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QHBoxLayout* layout = new QHBoxLayout(this);
    QVBoxLayout* vbox = new QVBoxLayout();

    m_HorizontalLine = new QFrame;
    m_HorizontalLine->setFrameShape(QFrame::HLine);
    m_HorizontalLine->setObjectName("Line");
    m_HorizontalLine->setFixedHeight(1);

    QHBoxLayout* icons_layout = new QHBoxLayout();
    QHBoxLayout* window_name_layout = new QHBoxLayout();
    window_name_layout->addWidget(new QLabel(QApplication::activeWindow()->windowTitle()));

    auto text_label = new QLabel(QApplication::activeWindow()->windowTitle());
    auto icon_label = new QLabel();
    icon_label->setPixmap(QIcon(":/resources/icons/logo/logo.png").pixmap(QSize(24, 24)));

    icons_layout->addWidget(icon_label);
    icons_layout->addWidget(text_label);
    icons_layout->addStretch();
    icons_layout->addWidget(m_CloseButton);
    vbox->addLayout(icons_layout);

    vbox->addWidget(m_HorizontalLine);

    QHBoxLayout* title_with_icon_layout = new QHBoxLayout();

    if(!title_icon.isNull()){
        m_TitleIcon = new QPushButton(title_icon, "");
        m_TitleIcon->setObjectName("TitleIcon");
        m_TitleIcon->setStyleSheet("background-color: rgba(0,0,0,0); ");
        title_with_icon_layout->addWidget(m_TitleIcon);
    }

    title_with_icon_layout->addWidget(m_TitleLabel);
    title_with_icon_layout->addStretch();
    vbox->addLayout(title_with_icon_layout);
    vbox->addWidget(m_DescriptionLabel);
    if(!m_OptionalButtons.empty()){
        QHBoxLayout* buttons_horizontal_layout = new QHBoxLayout();

        for(auto& button : m_OptionalButtons){
            button.get()->setObjectName("ActionButton");
            buttons_horizontal_layout->addWidget(button.get());
        }
        buttons_horizontal_layout->addStretch();
        vbox->addLayout(buttons_horizontal_layout);
    }

    layout->addStretch();
    layout->addLayout(vbox);

    layout->addStretch();
    this->setLayout(layout);

    layout->setSizeConstraints(QBoxLayout::SetMinimumSize, QBoxLayout::SetDefaultConstraint);

    QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacity_effect);

    m_FadeInAnim = new QPropertyAnimation(opacity_effect, "opacity");
    m_FadeInAnim->setDuration(550);
    m_FadeInAnim->setStartValue(0.0);
    m_FadeInAnim->setEndValue(1);

    m_FadeOutAnim = new QPropertyAnimation(opacity_effect, "opacity");
    m_FadeOutAnim->setDuration(550);
    m_FadeOutAnim->setStartValue(1);
    m_FadeOutAnim->setEndValue(0.0);

    m_AnimationGroup = new QSequentialAnimationGroup(this);
    m_AnimationGroup->addAnimation(m_FadeInAnim);
    m_AnimationGroup->addPause(m_ShowDuration);
    m_AnimationGroup->addAnimation(m_FadeOutAnim);

    connect(m_AnimationGroup, &QSequentialAnimationGroup::finished, this, [this](){
        this->hide();
        emit Finished();
    });

    m_DescriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_CloseButton->connect(m_CloseButton, &QPushButton::clicked, this, [this](){
        emit Finished();
        this->close();

    });

    this->setMinimumWidth(300);
    hide();
}

void internal::IPopup::ResizeForContent() {
    QFontMetrics desc_fm(m_DescriptionLabel->font()), title_fm(m_TitleLabel->text());
    int text_width = qMax(desc_fm.horizontalAdvance(m_DescriptionLabel->text()), title_fm.horizontalAdvance(m_TitleLabel->text()));

    this->resize(text_width, this->layout()->geometry().height());

    this->layout()->update();

    this->updateGeometry();
    this->adjustSize();
}

void internal::IPopup::showEvent(QShowEvent *) {
    QRect screen_geometry = QApplication::primaryScreen()->geometry();

    auto position = screen_geometry.bottomRight() - rect().bottomRight();

    const int margin_bottom = screen_geometry.height() * 0.06;
    position.setY(position.y() - margin_bottom);

    const int margin_right = screen_geometry.width() * 0.01;
    position.setX(position.x() - margin_right);

    move(position);
    raise();
    m_AnimationGroup->start();
}

void internal::IPopup::paintEvent(QPaintEvent* ev) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(ev);
}

void Popup::ShowSystemTrayNotification(const QString& title, const QString& description, const QIcon& icon, QSystemTrayIcon::MessageIcon message_icon_type, QWidget* parent)
{
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(parent);
    trayIcon->setIcon(QIcon(":/resources/icons/logo/logo.png"));
    trayIcon->show();

    if (icon.isNull()) {
        trayIcon->showMessage(title, description, message_icon_type, 5000);
    } else {
        trayIcon->showMessage(title, description, icon, 5000);
    }
}

void Popup::CustomPopup(const QString& title,
                        const QString& description,
                        int show_duration_ms,
                        const QIcon& title_icon,
                        const QList<std::shared_ptr<QPushButton>>& buttons,
                        QWidget* parent)
{
    internal::IPopup* popup{nullptr};

    if (!popup)
        popup = new internal::IPopup(title, description, buttons, show_duration_ms, title_icon);

    popup->ResizeForContent();

    popup->raise();
    popup->show();
}

PopupManager& PopupManager::GetInstance()
{
    static PopupManager popup_manager;
    return popup_manager;
}

void PopupManager::OnPopupFinished(std::shared_ptr<internal::IPopup> finished_popup)
{
    if (!finished_popup) {
        return;
    }

    if (!m_PopupsQueue.isEmpty() && m_PopupsQueue.head() == finished_popup) {
        m_PopupsQueue.dequeue();
    }

    ShowNextPopup();
}

void PopupManager::ShowNextPopup()
{
    if (m_PopupsQueue.isEmpty()) {
        return;
    }

    std::shared_ptr<internal::IPopup> curr_popup = m_PopupsQueue.head();

    if (!curr_popup) {
        return;
    }

    if (!curr_popup->isVisible()) {
        curr_popup->ResizeForContent();
        curr_popup->update();
        curr_popup->raise();
        curr_popup->show();
    }
}
