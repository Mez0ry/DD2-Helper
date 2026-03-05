#include "CustomButtons.hpp"
#include <QEvent>

SvgLinkButton::SvgLinkButton(const QString &button_text,
                             const QString &icon_path,
                             QUrl url,
                             QWidget *parent)
    : QPushButton(button_text, parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto str = icon_path.mid(icon_path.lastIndexOf('.') + 1, icon_path.size());

    if (str != "svg") {
        qDebug() << "file in the specified path is not svg";
    }

    m_SvgRenderer = std::make_unique<QSvgRenderer>
    (icon_path, this);

    connect(this, &QPushButton::clicked, this, [url]() { return QDesktopServices::openUrl(url); });
}

void SvgLinkButton::paintEvent(QPaintEvent *event)
{
    QStyleOptionButton option;
    initStyleOption(&option);
    option.text = "";

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing
                           | QPainter::SmoothPixmapTransform);

    style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);

    auto button_size = this->size();

    QSize icon_size(14, 14);

    const int margin = button_size.width() * 0.05;

    const double scale_factor = 0.2;

    const int svg_width = static_cast<int>
    (button_size.width() * scale_factor);
    const int svg_height = static_cast<int>
    (svg_width
     * (icon_size.height()
        / static_cast<double>
        (icon_size.width())));

    QSize scaled_icon_size(svg_width, svg_height);
    QPoint icon_point(margin / 2, this->rect().bottom() - scaled_icon_size.height());

    if (m_SvgRenderer) {
        m_SvgRenderer->render(&painter, QRect(icon_point, scaled_icon_size));
    }

    int text_x = icon_point.x() + icon_size.width() + (margin * 2);
    int text_width = button_size.width() - text_x - margin;
    QRect text_rect(text_x, margin, text_width, button_size.height() - 2 * margin);

    style()->drawItemText(&painter,
                          text_rect,
                          Qt::AlignVCenter | Qt::AlignLeft,
                          option.palette,
                          isEnabled(),
                          this->text(),
                          QPalette::ButtonText);
}

bool SvgLinkButton::event(QEvent *e)
{
    if (e->type() == QEvent::HoverEnter) {
        this->setCursor(Qt::PointingHandCursor);
    } else if (e->type() == QEvent::HoverLeave) {
        this->setCursor(Qt::ArrowCursor);
    }
    return QWidget::event(e);
}
