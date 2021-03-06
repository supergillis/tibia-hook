/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>

class DataQueue {
public:
    int size() {
        QMutexLocker locker(&mutex_);
        return queue_.size();
    }

    bool empty() {
        QMutexLocker locker(&mutex_);
        return queue_.empty();
    }

    void enqueue(const QByteArray& data) {
        QMutexLocker locker(&mutex_);
        queue_.enqueue(data);
    }

    QByteArray dequeue() {
        QMutexLocker locker(&mutex_);
        return queue_.dequeue();
    }

private:
	QQueue<QByteArray> queue_;
	QMutex mutex_;
};

#endif
