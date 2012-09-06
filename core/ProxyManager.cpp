#include "ProxyManager.h"

template<>
void TemplateableProxyManager<ReadOnlyProxyInterface, void>::handlePacket(const PacketReaderInterface* reader) {
    quint8 type = reader->peekU8();

    // Iterate all proxies
    QList<ReadOnlyProxyInterface*> proxies = proxies_.value(type);
    foreach (ReadOnlyProxyInterface* proxy, proxies) {
        proxy->handlePacket(reader);
    }
}

template<>
bool TemplateableProxyManager<ProxyInterface, bool>::handlePacket(const PacketReaderInterface* reader) {
    quint8 type = reader->peekU8();

    // Iterate all proxies and stop when one returns false
    QList<ProxyInterface*> proxies = proxies_.value(type);
    foreach (ProxyInterface* proxy, proxies) {
        if (!proxy->handlePacket(reader)) {
            return false;
        }
    }
    return true;
}
