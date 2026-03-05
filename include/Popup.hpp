#ifndef __POPUP_HPP__
#define __POPUP_HPP__

#include <QLabel>
#include <QList>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSystemTrayIcon>
#include <QTextBlock>
#include <QWidget>
#include <memory>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qstyleoption.h>
#include <QtSvg/QSvgRenderer>
#include <QDesktopServices>
#include <QQueue>

class IPopup;

namespace internal{
    /**
        @brief consider apply maximum length to strings of title and description
    **/
    class IPopup : public QWidget{
        Q_OBJECT
    private:
        const int m_ShowDuration{3000};
        QFrame* m_HorizontalLine;
        QPushButton* m_CloseButton, *m_TitleIcon;
        QList<std::shared_ptr<QPushButton>> m_OptionalButtons;
    private:// Animation
        QPropertyAnimation* m_FadeInAnim;
        QPropertyAnimation* m_FadeOutAnim;
        QSequentialAnimationGroup* m_AnimationGroup;
    protected:
        QLabel* m_TitleLabel;
        QLabel* m_DescriptionLabel;
    public:
        explicit IPopup(const QString& title, const QString& description, const QList<std::shared_ptr<QPushButton>>& buttons = {}, int show_duration_ms = 3000, const QIcon& title_icon = QIcon(""), QWidget* parent = nullptr);

        void showEvent(QShowEvent*) override;
        void paintEvent(QPaintEvent*) override;
        void ResizeForContent();
    signals:
        void Finished();
    };
}

using empty_buttons_t = QList<std::shared_ptr<QPushButton>>;

class PopupManager final : public QWidget{
    Q_OBJECT
private:
    QQueue<std::shared_ptr<internal::IPopup>> m_PopupsQueue;
public:
    explicit PopupManager(QWidget* parent = nullptr) : QWidget(parent){}

    PopupManager(const PopupManager&) = delete;
    PopupManager& operator=(const PopupManager&) = delete;

    static PopupManager& GetInstance();

    template <typename TPopup, typename ...Args >
    void AddPopup(Args&& ... args);

private slots:
    void OnPopupFinished(std::shared_ptr<internal::IPopup> finished_popup);
private:
    void ShowNextPopup();
};

template<typename TPopup, typename ...Args>
inline void PopupManager::AddPopup(Args&& ... args)
{
    /**
     * @TODO Consider to make it one instance only and reuse it
    */
    std::shared_ptr<internal::IPopup> popup = std::make_shared<internal::IPopup>(std::forward<Args>(args)...);
    auto slot = std::bind(&PopupManager::OnPopupFinished, this, popup);

    connect(popup.get(), &internal::IPopup::Finished, this, slot);
    m_PopupsQueue.enqueue(popup);
    ShowNextPopup();
}

namespace Popup{

    void CustomPopup(const QString& title, const QString& description, int show_duration_ms = 3000, const QIcon& title_icon = QIcon(""), const QList<std::shared_ptr<QPushButton>>& buttons = {}, QWidget* parent = nullptr);
    /**
     * @brief Uses OS type notification
    **/
    void ShowSystemTrayNotification(const QString& title, const QString& description, const QIcon& icon = {}, QSystemTrayIcon::MessageIcon message_icon_type = QSystemTrayIcon::MessageIcon::NoIcon, QWidget* parent = nullptr);
}//!Popup

#endif // __POPUP_HPP__
