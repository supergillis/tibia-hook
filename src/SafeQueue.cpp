#include "SafeQueue.h"

int SafeQueue::size() {
	QMutexLocker locker(&mutex_);
	return queue_.size();
}

bool SafeQueue::empty() {
	QMutexLocker locker(&mutex_);
	return queue_.empty();
}

void SafeQueue::enqueue(const QByteArray& data) {
	QMutexLocker locker(&mutex_);
	queue_.enqueue(data);
}

QByteArray SafeQueue::dequeue() {
	QMutexLocker locker(&mutex_);
	return queue_.dequeue();
}
