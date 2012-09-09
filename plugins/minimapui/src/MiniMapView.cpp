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

#include "MiniMapView.h"
#include "MiniMapUIPlugin.h"

#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QDebug>

QDebug operator<< (QDebug debug, const Direction& direction) {
    switch (direction) {
    case NORTH: debug << "north"; break;
    case NORTHEAST: debug << "northeast"; break;
    case EAST: debug << "east"; break;
    case SOUTHEAST: debug << "southeast"; break;
    case SOUTH: debug << "south"; break;
    case SOUTHWEST: debug << "southwest"; break;
    case WEST: debug << "west"; break;
    case NORTHWEST: debug << "northwest"; break;
    default: debug << "unknown"; break;
    }
    return debug;
}

QDebug operator<< (QDebug debug, const Position& position) {
    debug.nospace() << "[" << position.x << ", " << position.y << ", " << position.z << "]";
    return debug;
}

MiniMapView::MiniMapView(MiniMapUIPlugin* plugin, QWidget* parent):
    QGraphicsView(parent),
    scene_(new QGraphicsScene(this)),
    plugin_(plugin),
    floorIndex_(7) {
    scales_ << 0.25 << 0.35 << 0.50 << 0.75 << 1.00 << 1.25 << 1.50 << 2.00 << 2.50 << 3.00 << 4.00 << 6.00;
    scaleIndex_ = scales_.indexOf(1.00);

    setScene(scene_);
    setBackgroundBrush(Qt::black);

    QPen pen(Qt::white, 1, Qt::DashLine);
    horizontalLine_ = scene_->addLine(0, 0, 0, 0);
    horizontalLine_->setZValue(1);
    horizontalLine_->setPen(pen);
    verticalLine_ = scene_->addLine(0, 0, 0, 0);
    verticalLine_->setZValue(1);
    verticalLine_->setPen(pen);

    // Connect to the tracker
    if (plugin_->tracker_ != NULL) {
        plugin_->tracker_->connectPositionChanged(this, SLOT(setPosition(const Position&)));
    }

    // Initialize the view
    refresh();
}

MiniMapView::~MiniMapView() {
    // Don't forget to clean up cache
    clear();
}

void MiniMapView::setPosition(const Position& position) {
    if (position.z < MAP_MINIMUM_Z || position.z > MAP_MAXIMUM_Z) {
        return;
    }

    if (floorIndex_ != position.z) {
        floorIndex_ = position.z;
        refresh();
    }

    QRectF rect = sceneRect();
    horizontalLine_->setLine(rect.x(), position.y, rect.x() + sceneRect().width(), position.y);
    verticalLine_->setLine(position.x, rect.y(), position.x, rect.y() + sceneRect().height());
}

void MiniMapView::clear() {
    // Clear the scene but prevent from removing the lines
    foreach (QGraphicsItem* item, scene_->items()) {
        if (item->zValue() == 0) {
            scene_->removeItem(item);
        }
    }
}

void MiniMapView::refresh() {
    // Clear the scene but prevent from removing the lines
    clear();

    // Check floor boundary
    if (floorIndex_ < MAP_MINIMUM_Z || floorIndex_ > MAP_MAXIMUM_Z) {
        return;
    }

    // Populate scene
    MiniMapFloorInterface* floor = plugin_->minimap_->floor(floorIndex_);
    foreach (MiniMapPartInterface* part, floor->parts()) {
        QPixmap pixmap = QPixmap::fromImage(part->image());
        QGraphicsPixmapItem* item = scene_->addPixmap(pixmap);

        // Adjust position relative to bounds
        item->setPos(part->x(), part->y());
    }

    // Allows us to use real Tibia coordinates
    setSceneRect(floor->boundary());
}

void MiniMapView::mousePressEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton) {
        event->accept();
        mousePosition_ = event->pos();
    }
    if ((event->buttons() & Qt::RightButton) == Qt::RightButton && plugin_->finder_ != NULL) {
        event->accept();

        QPointF mapped(mapToScene(event->pos()));
        Position clicked;
        clicked.x = (quint16) mapped.x();
        clicked.y = (quint16) mapped.y();
        clicked.z = floorIndex_;

        MiniMapFloorGrid grid(plugin_->minimap_->floor(floorIndex_));

        if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
            // Find path between CTRL + right clicked positions
            static Position start;
            if (start.x == 0) {
                start = clicked;
            }
            else {
                QList<Direction> path = plugin_->finder_->findPath(&grid, start.x, start.y, clicked.x, clicked.y);

                qDebug() << "path from" << start << "to" << clicked;
                foreach (const Direction& direction, path) {
                    qDebug() << "  " << direction;
                }

                start.x = 0;
            }
        }
        else {
            // Find path from our position
            if (plugin_->walker_ != NULL && plugin_->tracker_ != NULL) {
                Position position = plugin_->tracker_->position();
                QList<Direction> path = plugin_->finder_->findPath(&grid, position.x, position.y, clicked.x, clicked.y);

                plugin_->walker_->walk(path);
            }
        }
    }
    else {
        QGraphicsView::mousePressEvent(event);
        return;
    }
}

void MiniMapView::mouseReleaseEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) != Qt::LeftButton) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    event->accept();
    mousePosition_ = QPoint();
}

void MiniMapView::mouseMoveEvent(QMouseEvent* event) {
    if((event->buttons() & Qt::LeftButton) != Qt::LeftButton || mousePosition_.isNull()) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    event->accept();

    int dx = mousePosition_.x() - event->pos().x();
    int dy = mousePosition_.y() - event->pos().y();
    mousePosition_ = event->pos();

    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
    verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
}

void MiniMapView::keyPressEvent(QKeyEvent* event) {
    if (event->key() != Qt::Key_Plus && event->key() != Qt::Key_Minus) {
        QGraphicsView::keyPressEvent(event);
        return;
    }

    event->accept();
    if ((event->key() == Qt::Key_Plus && floorIndex_ == MAP_MINIMUM_Z) ||
            (event->key() == Qt::Key_Minus && floorIndex_ == MAP_MAXIMUM_Z)) {
        return;
    }

    // Find the new floor
    floorIndex_ = event->key() == Qt::Key_Plus ? floorIndex_ - 1 : floorIndex_ + 1;

    // Load the new map tiles
    refresh();
}

void MiniMapView::wheelEvent(QWheelEvent* event) {
    if ((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier) {
        QGraphicsView::wheelEvent(event);
        return;
    }

    event->accept();
    if ((event->delta() < 0 && scaleIndex_ == 0) ||
            (event->delta() > 0 && scaleIndex_ == scales_.length() - 1)) {
        return;
    }

    // Find new scale factor
    scaleIndex_ = event->delta() > 0 ? scaleIndex_ + 1 : scaleIndex_ - 1;
    double scaleFactor = scales_.at(scaleIndex_);

    QPointF pointBeforeScale(mapToScene(event->pos()));

    // Do the actual scaling
    setTransform(QTransform::fromScale(scaleFactor, scaleFactor));

    QPointF pointAfterScale(mapToScene(event->pos()));
    QPointF offset = pointBeforeScale - pointAfterScale;

    // Adjust to the new center for correct zooming
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + offset.x());
    verticalScrollBar()->setValue(verticalScrollBar()->value() + offset.y());
}
