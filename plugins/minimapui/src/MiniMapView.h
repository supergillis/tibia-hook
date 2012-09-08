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

#include <MiniMapFloorInterface.h>
#include <ReadOnlyProxyInterface.h>
#include <PositionTrackerPluginInterface.h>
#include <WalkerPluginInterface.h>

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

#define MAP_MINIMUM_Z 0
#define MAP_MAXIMUM_Z 13

class MiniMapView: public QGraphicsView {
    Q_OBJECT

public:
    MiniMapView(PositionTrackerPluginInterface* positionTracker, WalkerPluginInterface* walker, QWidget* parent = 0);
    ~MiniMapView();

    void setModel(MiniMapModel* model);

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);

protected slots:
    void setPosition(const Position& position);

protected:
    void clear();
    void refresh();
    
private:
    QGraphicsScene* scene_;
    QGraphicsLineItem* horizontalLine_;
    QGraphicsLineItem* verticalLine_;

    PositionTrackerPluginInterface* positionTracker_;
    WalkerPluginInterface* walker_;
    MiniMapModel* model_;

    QPoint mousePosition_;
    QList<double> scales_;
    quint8 scaleIndex_;
    quint8 floorIndex_;
};

#endif
