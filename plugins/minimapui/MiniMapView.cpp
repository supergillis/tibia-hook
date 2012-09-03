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

MiniMapView::MiniMapView(QWidget* parent):
    QScrollArea(parent),
    model_(NULL),
    scaleFactor_(1.0) {
    imageLabel_ = new QLabel;
    imageLabel_->setBackgroundRole(QPalette::Base);
    imageLabel_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel_->setScaledContents(true);

    setBackgroundRole(QPalette::Dark);
    setWidget(imageLabel_);
}

MiniMapView::~MiniMapView() {
}

void MiniMapView::refresh() {
    if (model_== NULL) {
        return;
    }
    QImage image = model_->image(7);
    QPixmap pixmap = QPixmap::fromImage(image);
    imageLabel_->setPixmap(pixmap);
    imageLabel_->adjustSize();
}

void MiniMapView::setModel(MiniMapModel* model) {
    model_ = model;
    refresh();
}

void MiniMapView::scaleFactor(double factor) {
    Q_ASSERT(imageLabel_->pixmap());

    scaleFactor_ = factor == 0 ? 1.0 : factor * scaleFactor_;
    imageLabel_->resize(scaleFactor_ * imageLabel_->pixmap()->size());

    adjustScrollBar(horizontalScrollBar(), factor);
    adjustScrollBar(verticalScrollBar(), factor);
}

void MiniMapView::adjustScrollBar(QScrollBar* scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MiniMapView::wheelEvent(QWheelEvent* event) {
    if((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
        int delta = event->delta();
        double factor = ((double) delta / (double) 240) + 1;
        scaleFactor(factor);

        // Don't delegate this event
        event->accept();
    }
    else {
        QScrollArea::wheelEvent(event);
    }
}
