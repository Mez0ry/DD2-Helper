#include "Network.hpp"

std::shared_ptr<QNetworkAccessManager> Network::NetManager = nullptr;

void Network::InitializeNetManager() {
    if(!NetManager){
        NetManager = std::make_shared<QNetworkAccessManager>();
    }
}
