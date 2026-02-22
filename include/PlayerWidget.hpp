#ifndef __PLAYER__WIDGET_HPP
#define __PLAYER__WIDGET_HPP
#include <QComboBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QWidget>

#include <QThread>
#include <QFuture>

class Player;

class PlayerWidget : public QWidget {
    Q_OBJECT
public:
    PlayerWidget(std::shared_ptr<Player> player, QWidget *parent = nullptr);

    ~PlayerWidget();

    QList<QCheckBox*> GetCheckBoxes() const;
    void Show(bool visible);

private:
    std::shared_ptr<Player> m_Player;
    QList<QCheckBox*> m_CheckBoxes;
};

#endif // __PLAYER__WIDGET_HPP
