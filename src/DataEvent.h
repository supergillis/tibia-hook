#ifndef DATAEVENT_H_
#define DATAEVENT_H_

#include <QByteArray>
#include <QEvent>

class DataEvent: public QEvent {
public:
	static const QEvent::Type EventType = QEvent::User;

	enum DataType {
		Client,
		Server
	};

	DataEvent(DataType, const QByteArray&);
	DataEvent(DataType, const quint8*, quint32);

	const QByteArray& data() const;
	DataType dataType() const;

private:
	QByteArray data_;
	DataType dataType_;
};

#endif /* DATAEVENT_H_ */
