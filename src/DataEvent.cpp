#include "DataEvent.h"

DataEvent::DataEvent(EventType type, const QByteArray& data) :
		QEvent((QEvent::Type) type), data_(data) {
}

const QByteArray& DataEvent::data() const {
	return data_;
}
