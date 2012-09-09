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
class PriorityQueue: protected QList<T> {
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
