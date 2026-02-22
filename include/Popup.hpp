#ifndef __POPUP_HPP__
#define __POPUP_HPP__

#include <QLabel>
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
namespace internal{

    class IPopup : public QWidget{
        Q_OBJECT
    private:
        const int m_ShowDuration{3000};
    public:
        explicit IPopup(const QString& title, const QString& description, int show_duration_ms = 3000, QWidget* parent = nullptr);

        void showEvent(QShowEvent*) override;
        void ResizeForContent();
    private slots:
        void onFadeOutFinished();
    protected:
        QLabel* m_TitleLabel;
        QLabel* m_DescriptionLabel;

    private:
        QPropertyAnimation* m_FadeInAnim;
        QPropertyAnimation* m_FadeOutAnim;
        QSequentialAnimationGroup* m_AnimationGroup;
    };

    class InfoPopup : public IPopup {
        Q_OBJECT
    private:
        QColor m_Color;
    public:
        explicit InfoPopup(const QString& title, const QString& description, const QColor& background_color = "#181827", int show_duration_ms = 3000, QWidget* parent = nullptr);

        void showEvent(QShowEvent*) override;
        void paintEvent(QPaintEvent*) override;

    public slots:
        void SetTitle(const QString& title){}
        void SetDescription(const QString& description){}
        void SetBgColor(const QColor& background_color){}
    };
}

namespace Popup{
    inline void ShowInfo(const QString& title, const QString& description, const QColor& background_color = "#181827", int show_duration_ms = 3000, QSize popup_size = {0, 0}, QWidget* parent = nullptr){
        internal::InfoPopup* popup{nullptr};

        if(!popup)
            popup = new internal::InfoPopup(title, description, background_color, show_duration_ms, parent);

        if(popup_size.isEmpty()){
            popup->ResizeForContent();
        }else{
            popup->setFixedSize(popup_size);
        }
        popup->raise();
        popup->show();

    }
}
#endif // __POPUP_HPP__
