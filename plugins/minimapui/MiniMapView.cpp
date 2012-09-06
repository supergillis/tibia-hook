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

#include <QGraphicsPixmapItem>
#include <QScrollBar>

MiniMapView::MiniMapView(QWidget* parent):
    QGraphicsView(parent),
    scene_(this),
    model_(NULL),
    floor_(NULL),
    floorIndex_(7) {
    scales_ << 0.25 << 0.35 << 0.50 << 0.75 << 1.00 << 1.25 << 1.50 << 2.00 << 2.50 << 3.00;
    scaleIndex_ = scales_.indexOf(1.00);

    setScene(&scene_);
    setBackgroundBrush(Qt::black);
}

void MiniMapView::setModel(MiniMapModel* model) {
    model_ = model;
    floor_ = NULL;
    floorIndex_ = 7;
    cache_.clear();

    // Reload model
    refresh();
}

void MiniMapView::refresh() {
    if (model_== NULL) {
        scene_.clear();
        return;
    }

    // Load image from cache or from the model
    if (cache_.contains(floorIndex_)) {
        floor_ = cache_.value(floorIndex_);
    }
    else {
        floor_ = model_->floor(floorIndex_);
        cache_.insert(floorIndex_, floor_);
    }

    // First cleanup before populating
    scene_.clear();

    // Populate scene
    const QRect& bounds = floor_->boundary();
    foreach (MiniMapPartInterface* part, floor_->parts()) {
        QPixmap pixmap = QPixmap::fromImage(part->image());
        QGraphicsPixmapItem* item = scene_.addPixmap(pixmap);

        // Adjust position relative to bounds
        item->setPos(part->x(), part->y());
    }

    // Allows us to use real Tibia coordinates
    setSceneRect(bounds);
}

void MiniMapView::mousePressEvent(QMouseEvent* event) {
    mousePosition_ = event->pos();
    event->accept();
}

void MiniMapView::mouseReleaseEvent(QMouseEvent* event) {
    mousePosition_ = QPoint();
    event->accept();
}

void MiniMapView::mouseMoveEvent(QMouseEvent* event) {
    if (!mousePosition_.isNull()) {
        event->accept();

        int dx = mousePosition_.x() - event->pos().x();
        int dy = mousePosition_.y() - event->pos().y();
        mousePosition_ = event->pos();

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
        verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
    }
    else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MiniMapView::keyPressEvent(QKeyEvent* event) {
    if (event->key() != Qt::Key_Plus && event->key() != Qt::Key_Minus) {
        QGraphicsView::keyPressEvent(event);
        return;
    }

    event->accept();
    if ((event->key() == Qt::Key_Plus && floorIndex_ == 0) ||
            (event->key() == Qt::Key_Minus && floorIndex_ == 13)) {
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
