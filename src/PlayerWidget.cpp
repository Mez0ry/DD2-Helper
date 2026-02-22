#include "PlayerWidget.hpp"
#include <QSpacerItem>
#include <QTimer>
#include "Player.hpp"
#include <UserConfig.hpp>

PlayerWidget::PlayerWidget(std::shared_ptr<Player> player, QWidget *parent) : QWidget(parent), m_Player(player)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    auto auto_ready = new QCheckBox("Auto ready", this);
    auto initiate_buffing = new QCheckBox("Initiate buffing", this);

    m_CheckBoxes.append(auto_ready);
    m_CheckBoxes.append(initiate_buffing);

    for(auto cb : m_CheckBoxes) {
        layout->addWidget(cb);
    }

    layout->setSpacing(7);
    layout->setContentsMargins(4, 4, 4, 4);
    auto vertical_spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addSpacerItem(vertical_spacer);
    setLayout(layout);

    connect(auto_ready,&QCheckBox::checkStateChanged,this,[auto_ready, player](Qt::CheckState new_state){
        if(!player){
            return;
        }

        if(auto_ready->checkState() == Qt::CheckState::Checked){
            //press auto ready for current player
            player->StartAutoReady();
        }else{
            player->StopAutoReady();
        }
    });

    connect(initiate_buffing, &QCheckBox::checkStateChanged, this, [auto_ready, player, initiate_buffing](Qt::CheckState new_state){
        if(!player){
            return;
        }

        if(initiate_buffing->checkState() == Qt::CheckState::Checked){
            player->StartInitiateBuffing();
            player->SetBuffing(true);
        }else{
           player->SetBuffing(false);
        }
    });
}

PlayerWidget::~PlayerWidget()
{
    for (auto cb : m_CheckBoxes)
        delete cb;
}

QList<QCheckBox *> PlayerWidget::GetCheckBoxes() const
{
    return m_CheckBoxes;
}

void PlayerWidget::Show(bool visible)
{
    setVisible(visible);
}
