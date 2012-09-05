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

#ifndef MINIMAPVIEW_H
#define MINIMAPVIEW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include <QKeyEvent>

#include "MiniMapModel.h"

class MiniMapView: public QScrollArea {
    Q_OBJECT
    
public:
    MiniMapView(QWidget* = 0);

    void setModel(MiniMapModel*);

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);

public slots:
    void refresh();
    
private:
    QLabel* imageLabel_;
    QScrollArea* scrollArea_;

    MiniMapModel* model_;
    quint8 floor_;

    // Caching
    QMap<quint8, QImage> cache_;
    QImage image_;

    // For zooming
    QList<double> scales_;
    qint8 scaleIndex_;

    // For dragging
    QPoint mousePosition_;
};

#endif
