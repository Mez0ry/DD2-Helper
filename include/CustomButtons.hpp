#ifndef __CUSTOM_BUTTONS_HPP
#define __CUSTOM_BUTTONS_HPP

#include <QDesktopServices>
#include <QSvgRenderer>
#include <memory>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qstyleoption.h>
#include <QUrl>

class SvgLinkButton final : public QPushButton
{
private:
    std::unique_ptr<QSvgRenderer> m_SvgRenderer;
public:
    SvgLinkButton(const QString& button_text, const QString& icon_path, QUrl url, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

    bool event(QEvent* e) override;
private:
};

#endif // !__CUSTOM_BUTTONS_HPP
