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

#include "MiniMapModel.h"

#include <ReadOnlyProxyInterface.h>

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

class MiniMapView: public QGraphicsView {
    Q_OBJECT
    
public:
    MiniMapView(QWidget* = 0);

    void setModel(MiniMapModel*);
    void setCenter(const QPointF& centerPoint);

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);

public slots:
    void refresh();
    
private:
    QGraphicsScene scene_;
    QMap<quint8, MiniMapFloorInterface*> cache_;

    MiniMapModel* model_;
    MiniMapFloorInterface* floor_;
    quint8 floorIndex_;

    // For zooming
    QList<double> scales_;
    qint8 scaleIndex_;

    // For dragging
    QPoint mousePosition_;
};

#endif
