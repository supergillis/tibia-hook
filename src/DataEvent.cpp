#include "DataEvent.h"

DataEvent::DataEvent(DataEvent::DataType type, const QByteArray& data) :
		QEvent(DataEvent::EventType), data_(data), dataType_(type) {
}

DataEvent::DataEvent(DataEvent::DataType type, const quint8* data, quint32 length) :
		QEvent(DataEvent::EventType), data_((char*) data, length), dataType_(type) {
}

const QByteArray& DataEvent::data() const {
	return data_;
}

DataEvent::DataType DataEvent::dataType() const {
	return dataType_;
}
