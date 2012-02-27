#ifndef DATAQUEUE_H_
#define DATAQUEUE_H_

#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>

class DataQueue {
public:
	int size();
	bool empty();

	void enqueue(const QByteArray&);
	QByteArray dequeue();

private:
	QQueue<QByteArray> queue_;
	QMutex mutex_;
};

#endif /* DATAQUEUE_H_ */
