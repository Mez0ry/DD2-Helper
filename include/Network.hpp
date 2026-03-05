#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <QtNetwork/QNetworkAccessManager>
#include <memory>
#include <QtNetwork/QRestReply>

namespace Network {
    extern std::shared_ptr<QNetworkAccessManager> NetManager;
    void InitializeNetManager();

}; // namespace Network
#endif // NETWORK_HPP
