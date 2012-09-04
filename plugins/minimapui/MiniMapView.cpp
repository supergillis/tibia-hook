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

MiniMapView::MiniMapView(QWidget* parent):
    QScrollArea(parent),
    model_(NULL),
    floor_(7) {
    scales_ << 0.25 << 0.50 << 0.75 << 1.00 << 1.25 << 1.50 << 2.00 << 2.50 << 3.00;
    scaleIndex_ = scales_.indexOf(1.00);
    imageLabel_ = new QLabel;
    imageLabel_->setBackgroundRole(QPalette::Base);

    setBackgroundRole(QPalette::Dark);
    setWidget(imageLabel_);
}

MiniMapView::~MiniMapView() {
}

void MiniMapView::setModel(MiniMapModel* model) {
    model_ = model;
    refresh();
}

void MiniMapView::refresh() {
    if (model_== NULL) {
        return;
    }
    QImage image = model_->imageForFloor(floor_);
    QPixmap pixmap = QPixmap::fromImage(image);
    double scaleFactor = scales_.at(scaleIndex_);

    imageLabel_->setPixmap(pixmap.scaled(pixmap.width() * scaleFactor, pixmap.height() * scaleFactor));
    imageLabel_->adjustSize();
}

void MiniMapView::mousePressEvent(QMouseEvent* event) {
    mousePosition_ = event->pos();

    // Don't delegate this event
    event->accept();
}

void MiniMapView::mouseReleaseEvent(QMouseEvent* event) {
    if(!mousePosition_.isNull()) {
        mousePosition_ = QPoint();

        // Don't delegate this event
        event->accept();
    }
    else {
        QScrollArea::mouseReleaseEvent(event);
    }
}

void MiniMapView::mouseMoveEvent(QMouseEvent* event) {
    if(!mousePosition_.isNull()) {
        int dx = mousePosition_.x() - event->pos().x();
        int dy = mousePosition_.y() - event->pos().y();
        mousePosition_ = event->pos();

        // Programatically scroll
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
        verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);

        // Don't delegate this event
        event->accept();
    }
    else {
        QScrollArea::mouseMoveEvent(event);
    }
}

void MiniMapView::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus) {
        floor_ = event->key() == Qt::Key_Plus ?
                    std::max(1, floor_ - 1) :
                    std::min(16, floor_ + 1);

        // Don't delegate this event
        event->accept();

        // Refresh the view
        refresh();
    }
    else {
        QScrollArea::keyPressEvent(event);
    }
}

void MiniMapView::wheelEvent(QWheelEvent* event) {
    if((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
        scaleIndex_ = event->delta() > 0 ? std::min(scaleIndex_ + 1, scales_.length() - 1) : std::max(scaleIndex_ - 1, 0);

        // Don't delegate this event
        event->accept();

        // Refresh the view
        refresh();
    }
    else {
        QScrollArea::wheelEvent(event);
    }
}
