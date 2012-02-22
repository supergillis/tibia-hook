#ifndef SAFEQUEUE_H_
#define SAFEQUEUE_H_

#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

class SafeQueue {
public:
	int size();
	bool empty();

	void enqueue(const QByteArray& value);
	QByteArray dequeue();

private:
	QQueue<QByteArray> queue_;
	QMutex mutex_;
};

#endif /* SAFEQUEUE_H_ */
