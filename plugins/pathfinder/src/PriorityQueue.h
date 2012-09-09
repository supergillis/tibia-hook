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

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QList>
#include <QListIterator>

template<class T>
class DefaultComparator {
public:
    bool operator()(const T& a, const T& b) {
        return a < b;
    }
};

template<class T, class Comparator = DefaultComparator<T> >
class PriorityQueue: private QList<T> {
public:
    PriorityQueue(): QList<T>() {}

    QListIterator<T> iterator() {
        return QListIterator<T>(*this);
    }

    int length() {
        return QList<T>::length();
    }

    void clear() {
        return QList<T>::clear();
    }

    bool empty() const {
        return QList<T>::empty();
    }

    bool remove(const T& value) {
        return QList<T>::removeAll(value);
    }

    void enqueue(const T& value) {
        for (int index = 0; index < QList<T>::size(); ++index) {
            if (comparator_(value, QList<T>::at(index)))  {
                QList<T>::insert(index, value);
                return;
            }
        }
        QList<T>::append(value);
    }

    T dequeue() {
        return QList<T>::takeFirst();
    }

    T& head() {
        return QList<T>::first();
    }

    const T& head() const {
        return QList<T>::first();
    }

private:
    Comparator comparator_;
};

#endif
