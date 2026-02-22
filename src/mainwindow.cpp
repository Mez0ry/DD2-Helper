#include "mainwindow.h"
#include "Popup.hpp"
#include "PlayerWidget.hpp"
#include "SendHotkey.hpp"
#include "ui_mainwindow.h"

#include <QFile>
#include <QSettings>
#include "UserConfig.hpp"
#include "Utils.hpp"
#include <qtimer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_UserInterface(new Ui::MainWindow)
{
    m_UserInterface->setupUi(this);

    QSettings settings("Mezory", "ColorPicker");
    settings.beginGroup("styles");
    settings.setValue("dark_theme",":/resources/app_styles/dark_theme.qss");
    settings.setValue("light_theme",":/resources/app_styles/light_theme.qss");
    settings.endGroup();

    connect(UserConfig::GetInstance().get(), &UserConfig::IsParsed, [&](){
        qDebug() << "User config has been successfully deseralized" << '\n';
        try {
            auto ui = Utils::GetUserInterface();

            connect(ui->save_hotkey_button, &QPushButton::clicked, this, [](){
                UserConfig::GetInstance()->SaveConfig();
                Popup::ShowInfo("Information", "User hotkeys successfully saved");
            });

            struct KeyPair {
                QString key_name;
                HotKeyEdit* edit_field;
            };

            const std::vector<KeyPair> keys_to_setup = {
                {"ReadyButton", ui->ready_hotkey},
                {"AutoReady", ui->auto_ready_hotkey},
                {"RetryButton", ui->retry_hotkey},
                {"DropBuildResourceButton", ui->drop_build_resource_hotkey},
                {"DropBuildResourceInGameBind", ui->drop_build_resource_in_game_hotkey},
                {"DropAbilityMana", ui->drop_mana_hotkey},
                {"DropAbilityManaInGameBind", ui->drop_mana_in_game_hotkey},
                {"InitiateBuff", ui->initiate_buffing_hotkey}
            };

            for (const auto& pair : keys_to_setup) {
                auto hotkey = UserConfig::GetInstance()->GetKeyBind(pair.key_name);
                pair.edit_field->SetHotKey(hotkey);

                connect(pair.edit_field, &HotKeyEdit::HotkeyChanged, [pair](){
                    QString info_text = QString("hotkey for %1 is now: %2").arg(pair.key_name, pair.edit_field->GetText());
                    Popup::ShowInfo("Hotkey changed", info_text);

                    UserConfig::GetInstance()->ChangeKeyBind(pair.key_name, HotKeySequence(pair.edit_field->GetText().toStdString()));
                });

            }

            qDebug() << "Hotkeys loaded successfully.";
        } catch (const std::exception& e) {
            qWarning() << "Error loading hotkeys: " << e.what();
            Popup::ShowInfo("Error", "Failed to load hotkeys");
        }
    });

    UserConfig::GetInstance()->LoadConfig();

    auto app_dir = QCoreApplication::applicationDirPath();
    const auto user_config_path = QString(app_dir + "/resources/user.json");

    m_FilesystemWatcher.addPath(user_config_path);

    connect(&m_FilesystemWatcher, &QFileSystemWatcher::fileChanged, [user_config_path](const QString& path){
        if(path == user_config_path){
            UserConfig::GetInstance()->LoadConfig();
        }
    });

    connect(m_UserInterface->UpdatePlayerListButton, &QPushButton::pressed, this, [&, this](){
        m_WindowList.Clear();
        m_UserInterface->playerListBox->clear();
        m_PlayerList.Clear();
        m_PlayersWidgets.clear();

        auto host = Window::Find(L"Dungeon Defenders 2");
        if(host){
            m_WindowList.AddWindow(std::make_shared<Window>(host));
        }
        auto alt_windows = Window::FindAll(L"[#] Dungeon Defenders 2 [#]");

        for(auto win : alt_windows){
            if(win){
                m_WindowList.AddWindow(win);
            }
        }

        if(!m_WindowList){
            Popup::ShowInfo("Information", "DD2 windows were not found, are you sure that you launched the game?");
            return;
        }

        while(m_UserInterface->verticalLayout->count() > 0) {
            delete m_UserInterface->verticalLayout->takeAt(0)->widget();
        }

        for(int index = 0; index < m_WindowList.Size(); ++index){
            auto curr_win = m_WindowList[index];
            auto player = std::make_shared<Player>(curr_win);
            m_PlayerList.AddPlayer(player);

            m_UserInterface->playerListBox->addItem((index == 0) ? QString("Host player") : QString("Alt: ") + QString::number(index));

            auto player_widget = new PlayerWidget(player, m_UserInterface->groupBox);
            m_PlayersWidgets.append(player_widget);
            m_UserInterface->verticalLayout->addWidget(player_widget);
            player_widget->Show(false);
        }

        disconnect(m_UserInterface->playerListBox, QOverload<int>::of(&QComboBox::currentIndexChanged), nullptr, nullptr);

        connect(m_UserInterface->playerListBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index){
            if(m_PlayersWidgets.empty()){
                return;
            }

            for(int i = 0; i < m_PlayersWidgets.size(); ++i) {
                m_PlayersWidgets[i]->Show(i == index);
            }
        });

        m_UserInterface->playerListBox->update();
    });

    auto ui = Utils::GetUserInterface();

    connect(ui->checkBox_3, &QCheckBox::checkStateChanged, [this](Qt::CheckState check_state){
        if(check_state == Qt::CheckState::Checked){
            for(auto& player_widgets : m_PlayersWidgets){
                if(player_widgets->GetCheckBoxes().empty()){
                    continue;
                }

                auto auto_ready_checkbox = player_widgets->GetCheckBoxes()[0];
                auto_ready_checkbox->setCheckState(Qt::CheckState::Checked);
            }
        }else if(check_state == Qt::CheckState::Unchecked){
            for(auto& player_widgets : m_PlayersWidgets){
                if(player_widgets->GetCheckBoxes().empty()){
                    continue;
                }

                auto auto_ready_checkbox = player_widgets->GetCheckBoxes()[0];
                auto_ready_checkbox->setCheckState(Qt::CheckState::Unchecked);
            }
        }
    });

    QTimer *update_logic_timer = new QTimer(this);
    update_logic_timer->setSingleShot(false);

    connect(update_logic_timer, &QTimer::timeout, this, &MainWindow::Update);

    update_logic_timer->start(30);
}

MainWindow::~MainWindow()
{
    delete m_UserInterface;
}

void MainWindow::Update() {
    if(!m_PlayerList){
        return;
    }

    auto ui = Utils::GetUserInterface();
    auto sync_input_cb = ui->checkBox_4;

    static auto auto_ready_hotkey = UserConfig::GetInstance()->GetKeyBind("AutoReady");
    static auto mirrored_type_hotkeys = UserConfig::GetInstance()->GetKeyBind("MirroredTypeHotkey");

    static auto drop_build_resource_hotkey = UserConfig::GetInstance()->GetKeyBind("DropBuildResourceButton");
    static auto drop_build_resource_in_game_hotkey = UserConfig::GetInstance()->GetKeyBind("DropBuildResourceInGameBind");

    static auto drop_ability_mana_hotkey = UserConfig::GetInstance()->GetKeyBind("DropAbilityMana");
    static auto drop_ability_mana_in_game_hotkey = UserConfig::GetInstance()->GetKeyBind("DropAbilityManaInGameBind");

    static auto initiate_buff_hotkey = UserConfig::GetInstance()->GetKeyBind("InitiateBuff");

    if(auto_ready_hotkey.Pressed()){
        for(auto& player : m_PlayerList){
            player->SetAutoReady(!player->IsAutoReady());
        }
    }

    if(initiate_buff_hotkey.Pressed()){
        int index = 0;
        for(auto& player : m_PlayerList){
            auto player_window = player->GetWindow();

            if(player_window->IsForegrounded() && !player->IsBuffing()){
                player->StartInitiateBuffing();
                player->SetBuffing(true);
                m_PlayersWidgets[index]->GetCheckBoxes()[1]->setCheckState(Qt::CheckState::Checked);
            }else if(player->IsBuffing() && player_window->IsForegrounded()){
                player->SetBuffing(false);
                m_PlayersWidgets[index]->GetCheckBoxes()[1]->setCheckState(Qt::CheckState::Unchecked);
            }

            ++index;
        }
    }

    const auto is_synced = (sync_input_cb->checkState() == Qt::CheckState::Checked);

    if(is_synced){
        for(auto& key : mirrored_type_hotkeys){
            if(HotKeySequence::Pressed(key)){
                for(auto player : m_PlayerList){
                    player->AddCommand<SendHotkey>(player, key);
                }
            }
        }
    }

    struct DropTypeHotkey{
        DropTypeHotkey(const HotKeySequence& _drop_hotkey, const HotKeySequence& _in_game, QCheckBox* const _checkbox, const std::function<void(std::shared_ptr<Player>)>& _action)
            : drop_hotkey(_drop_hotkey), hotkey_in_game(_in_game), checkbox(_checkbox), action(_action){}
        HotKeySequence drop_hotkey, hotkey_in_game;
        QCheckBox* const checkbox;
        std::function<void(std::shared_ptr<Player>)> action;
    };

    const QList<DropTypeHotkey> drop_type_hotkeys{
        DropTypeHotkey(drop_build_resource_hotkey, drop_build_resource_in_game_hotkey, ui->checkBox_5, [](std::shared_ptr<Player> player){if(!player) return; player->DropBuildResource();}),
        DropTypeHotkey(drop_ability_mana_hotkey, drop_ability_mana_in_game_hotkey, ui->checkBox_6, [](std::shared_ptr<Player> player){if(!player) return; player->DropMana();})
    };

    for(auto& drop_type : drop_type_hotkeys){
        if(drop_type.drop_hotkey.Pressed()){
            auto start_iter = m_PlayerList.begin();
            auto end_iter = m_PlayerList.end();
            auto alts_only = (drop_type.checkbox != nullptr && !drop_type.checkbox->isChecked());

            if(alts_only){
                uint8_t skip_n_elems = 1; // skipping host player (always first in vector of players list)
                start_iter = std::next(m_PlayerList.begin(), skip_n_elems);
                end_iter = m_PlayerList.end();
            }

            std::for_each(start_iter, end_iter, drop_type.action);

        }
    }

    for(auto& player : m_PlayerList){
        const auto player_window = player->GetWindow();

        if(player->IsAutoReady()){
            auto hotkey_sequence = UserConfig::GetInstance()->GetKeyBind("ReadyButton");
            auto player_window = player->GetWindow();

            hotkey_sequence.PressKeys(*player_window, 50, 50); // add slider and QLineEdit for the m_holdtime, release time
            ui->checkBox_3->setCheckState(Qt::CheckState::Checked);
        }else{
            ui->checkBox_3->setCheckState(Qt::CheckState::Unchecked);
        }
    }

    m_PlayerList.ExecuteCommands();
}
