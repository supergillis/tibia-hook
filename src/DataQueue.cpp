#include "DataQueue.h"

int DataQueue::size() {
	QMutexLocker locker(&mutex_);
	return queue_.size();
}

bool DataQueue::empty() {
	QMutexLocker locker(&mutex_);
	return queue_.empty();
}

void DataQueue::enqueue(const QByteArray& data) {
	QMutexLocker locker(&mutex_);
	queue_.enqueue(data);
}

QByteArray DataQueue::dequeue() {
	QMutexLocker locker(&mutex_);
	return queue_.dequeue();
}
