#include "ProxyManager.h"

/*template<>
void TemplateableProxyManager<ReadOnlyProxyInterface, void>::handlePacket(PacketReader& reader) {
    quint8 type = reader.peekU8();

    // Iterate all proxies
    ProxyList proxies = proxies_.value(type);
    foreach (ReadOnlyProxyInterface* proxy, proxies) {
        proxy->handlePacket(reader);
    }
}

template<>
bool TemplateableProxyManager<ProxyInterface, bool>::handlePacket(PacketReader& reader) {
    quint8 type = reader.peekU8();

    // Iterate all proxies and stop when one returns false
    ProxyList proxies = proxies_.value(type);
    foreach (ProxyInterface* proxy, proxies) {
        if (!proxy->handlePacket(reader)) {
            return false;
        }
    }
    return true;
}*/
