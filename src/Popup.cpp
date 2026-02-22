#include "Popup.hpp"
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QSequentialAnimationGroup>
#include <QVBoxLayout>
#include <qapplication.h>
#include <qscreen.h>
#include <qtimer.h>

internal::IPopup::IPopup(const QString& title, const QString& description, int show_duration_ms, QWidget* parent)
    : QWidget(parent), m_ShowDuration(show_duration_ms),
    m_TitleLabel(new QLabel(title)),
    m_DescriptionLabel(new QLabel(description)) {

    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(false);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

    m_TitleLabel->setWordWrap(true);
    m_TitleLabel->setObjectName("TitleLabel");

    m_DescriptionLabel->setWordWrap(true);
    m_DescriptionLabel->setObjectName("DescriptionLabel");

    QHBoxLayout* layout = new QHBoxLayout(this);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(m_TitleLabel);
    vbox->addWidget(m_DescriptionLabel);
    layout->addStretch();
    layout->addLayout(vbox);
    layout->addStretch();

    QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacity_effect);

    m_FadeInAnim = new QPropertyAnimation(opacity_effect, "opacity");
    m_FadeInAnim->setDuration(550);
    m_FadeInAnim->setStartValue(0.0);
    m_FadeInAnim->setEndValue(0.8);

    m_FadeOutAnim = new QPropertyAnimation(opacity_effect, "opacity");
    m_FadeOutAnim->setDuration(550);
    m_FadeOutAnim->setStartValue(0.8);
    m_FadeOutAnim->setEndValue(0.0);

    m_AnimationGroup = new QSequentialAnimationGroup(this);
    m_AnimationGroup->addAnimation(m_FadeInAnim);
    m_AnimationGroup->addPause(m_ShowDuration);
    m_AnimationGroup->addAnimation(m_FadeOutAnim);
    connect(m_AnimationGroup, &QSequentialAnimationGroup::finished, this, &IPopup::hide);

    QRect screen_geometry = QApplication::primaryScreen()->geometry();

    m_DescriptionLabel->setMinimumWidth(screen_geometry.width() * 0.1);
    hide();
}

void internal::IPopup::ResizeForContent() {
    QFontMetrics fm_title(m_TitleLabel->font());
    QFontMetrics fm_desc(m_DescriptionLabel->font());

    QRect title_rect = fm_title.boundingRect(m_TitleLabel->text());
    int title_width = title_rect.width();
    int title_height = title_rect.height();

    QRect desc_rect = fm_desc.boundingRect(m_DescriptionLabel->text());
    int desc_width = desc_rect.width();
    int desc_height = desc_rect.height();

    int width = qMax(title_width, desc_width);
    int height = title_height + desc_height + (title_height * 2.5);
    width -= 50;

    resize(width, height);
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

void internal::IPopup::onFadeOutFinished() {
    m_FadeOutAnim->start();
}

internal::InfoPopup::InfoPopup(const QString& title, const QString& description, const QColor& background_color, const int show_time_ms, QWidget* parent) : IPopup(title, description, show_time_ms), m_Color(background_color){
    setObjectName("InfoPopup");

    m_TitleLabel->setWordWrap(true);
    m_TitleLabel->setObjectName("TitleLabel");

    m_DescriptionLabel->setWordWrap(true);
    m_DescriptionLabel->setObjectName("DescriptionLabel");
}

void internal::InfoPopup::showEvent(QShowEvent* event)
{
    IPopup::showEvent(event);
}

void internal::InfoPopup::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.fillRect(rect(), m_Color);
}
