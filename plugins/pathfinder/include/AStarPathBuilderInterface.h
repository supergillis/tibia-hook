#ifndef ASTARPATHBUILDERINTERFACE_H
#define ASTARPATHBUILDERINTERFACE_H

#include <QList>

#include <AStarNodeInterface.h>
#include <Position.h>

class AStarPathBuilderInterface {
public:
    virtual void rebuildPath(const AStarNodeInterface* lastNode) = 0;
};

class PositionPathBuilder: public AStarPathBuilderInterface {
public:
    void rebuildPath(const AStarNodeInterface* lastNode) {
        const AStarNodeInterface* current = lastNode;

        // Reverse position list
        while (current != NULL) {
            positions_.prepend(current->position());
            current = current->parent();
        }
    }

    const QList<Position>& positions() const { return positions_; }

private:
    QList<Position> positions_;
};

class DirectionPathBuilder: public AStarPathBuilderInterface {
public:
    void rebuildPath(const AStarNodeInterface* lastNode) {
        const AStarNodeInterface* current = lastNode;

        // Reverse position list
        quint16 px = current->position().x;
        quint16 py = current->position().y;
        current = current->parent();
        while (current != NULL) {
            int dx = px - current->position().x;
            int dy = py - current->position().y;

            if (dx == -1 && dy == -1) {
                directions_.prepend(NORTHWEST);
            }
            else if (dx == 1 && dy == -1) {
                directions_.prepend(NORTHEAST);
            }
            else if (dx == -1 && dy == 1) {
                directions_.prepend(SOUTHWEST);
            }
            else if (dx == 1 && dy == 1) {
                directions_.prepend(SOUTHEAST);
            }
            else if (dx == -1) {
                directions_.prepend(WEST);
            }
            else if (dx == 1) {
                directions_.prepend(EAST);
            }
            else if (dy == -1) {
                directions_.prepend(NORTH);
            }
            else if (dy == 1) {
                directions_.prepend(SOUTH);
            }

            px = current->position().x;
            py = current->position().y;
            current = current->parent();
        }
    }

    const QList<Direction>& directions() const { return directions_; }

private:
    QList<Direction> directions_;
};

#endif
