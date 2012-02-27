#include "ReadOnlyPacketProxy.h"

ReadOnlyPacketProxy::ReadOnlyPacketProxy(const QByteArray& data) :
		ReadOnlyPacketInterface(), packet_(data) {
}

ReadOnlyPacketProxy::ReadOnlyPacketProxy(const quint8* data, quint16 length) :
		ReadOnlyPacketInterface(), packet_(data, length) {
}
