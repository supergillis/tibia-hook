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

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QScrollBar>

MiniMapView::MiniMapView(PositionTrackerPluginInterface* positionTracker, QWidget* parent):
    QGraphicsView(parent),
    scene_(new QGraphicsScene(this)),
    positionTracker_(positionTracker),
    model_(NULL),
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
}

MiniMapView::~MiniMapView() {
    // Don't forget to clean up cache
    clear();
}

void MiniMapView::setModel(MiniMapModel* model) {
    if (model_ != NULL && positionTracker_ != NULL) {
        positionTracker_->disconnectPositionChanged(this, SLOT(setPosition(const Position&)));
    }

    model_ = model;
    floorIndex_ = 7;

    if (model_ != NULL && positionTracker_ != NULL) {
        positionTracker_->connectPositionChanged(this, SLOT(setPosition(const Position&)));
    }

    refresh();
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
    if (model_== NULL) {
        return;
    }

    // Clear the scene but prevent from removing the lines
    clear();

    // Check floor boundary
    if (floorIndex_ < MAP_MINIMUM_Z || floorIndex_ > MAP_MAXIMUM_Z) {
        return;
    }

    // Populate scene
    MiniMapFloorInterface* floor = model_->floor(floorIndex_);
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
    if ((event->buttons() & Qt::RightButton) == Qt::RightButton) {
        event->accept();

        QPointF mapped(mapToScene(event->pos()));
        Position end;
        end.x = (quint16) mapped.x();
        end.y = (quint16) mapped.y();
        end.z = floorIndex_;

        // Walk this path
        model_->walk(path);
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
