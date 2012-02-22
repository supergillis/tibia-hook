#ifndef DATAEVENT_H_
#define DATAEVENT_H_

#include <QObject>
#include <QEvent>
#include <QByteArray>

class DataEvent: public QEvent {
public:
	enum EventType {
		Client = QEvent::User,
		Server = QEvent::User + 1
	};

	DataEvent(EventType, const QByteArray&);

	const QByteArray& data() const;

private:
	QByteArray data_;
};

#endif /* DATAEVENT_H_ */
