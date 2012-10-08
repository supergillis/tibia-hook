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

#include <AStarGridInterface.h>
#include <HpaStarGridInterface.h>
#include <MiniMapFloorInterface.h>
#include <MiniMapPluginInterface.h>
#include <PathFinderPluginInterface.h>
#include <PluginManagerInterface.h>
#include <PositionTrackerPluginInterface.h>
#include <WalkerPluginInterface.h>

#include <Position.h>

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

#define MAP_MINIMUM_Z 0
#define MAP_MAXIMUM_Z 13

class MiniMapFloorGrid: public AStarGridInterface {
public:
    MiniMapFloorGrid(MiniMapFloorInterface* floor):
        floor_(floor) {
    }

    void forEachNeighbour(const Position& position, AStarVisitor& visitor) {
        static qint8 dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
        static qint8 dy[8] = {0, 0, 1, -1, 1, 1, -1, -1};

        // Only iterate [0, 3], ignore diagonal for now
        for (quint8 index = 0; index < 4; index++) {
            quint16 nx = position.x + dx[index];
            quint16 ny = position.y + dy[index];

            quint8 data = floor_->dataAt(nx, ny);
            if (data != 255) {
                quint8 cost = (data * 100) / 120;
                visitor.visitNeighbour(Position(nx, ny, position.z), cost);
            }
        }
    }

private:
    MiniMapFloorInterface* floor_;
};

#define CLUSTER_WIDTH 32
#define CLUSTER_HEIGHT 32

#define MINIMAP_CHANGE_LEVEL 0xFFFFFF00

class NopPathBuilder: public AStarPathBuilderInterface {
public:
    void rebuildPath(const AStarNodeInterface*) {
        // Do nothing
    }
};

class MiniMapHpaGrid: public HpaStarGridInterface {
public:
    MiniMapHpaGrid(PathFinderPluginInterface* finder, MiniMapPluginInterface* minimap):
        finder_(finder),
        minimap_(minimap) {
    }
	virtual ~MiniMapHpaGrid() {}

    quint32 clusterWidth() const { return CLUSTER_WIDTH; }
    quint32 clusterHeight() const { return CLUSTER_HEIGHT; }

    bool blocking(const Position& position) const { return blocking(position.x, position.y, position.z); }
    bool blocking(quint16 x, quint16 y, quint8 z) const { return minimap_->floor(z)->dataAt(x, y) == 255; }

    bool floorChange(const Position& position) const { return blocking(position.x, position.y, position.z); }
    bool floorChange(quint16 x, quint16 y, quint8 z) const { return minimap_->floor(z)->colorAt(x, y) == MINIMAP_CHANGE_LEVEL; }

    void partition(HpaStarPartitioner& partitioner) {
        QRect bounds;
        for (int z = 7; z <= 7; ++z) {
            MiniMapFloorInterface* floor = minimap_->floor(z);
            bounds = bounds.united(floor->boundary());
        }

        for (int z = 7; z <= 7; ++z) {
            for (int x = 0; x < bounds.width(); x += CLUSTER_WIDTH) {
                for (int y = 0; y < bounds.height(); y += CLUSTER_HEIGHT) {
                    partitioner.partition(x + bounds.x(), y + bounds.y(), z);
                }
            }
        }
    }

    bool reachable(const Position& start, const Position& end, const QRect& boundary, quint32* cost = NULL) const {
        if (start.z != end.z) {
            return false;
        }

        MiniMapFloorInterface* floor = minimap_->floor(start.z);
        MiniMapFloorGrid grid(floor);
        NopPathBuilder builder;
        return finder_->findPath(grid, builder, start, end, boundary, cost);
    }

private:
    PathFinderPluginInterface* finder_;
    MiniMapPluginInterface* minimap_;
};

class MiniMapUIPlugin;
class MiniMapView: public QGraphicsView {
    Q_OBJECT

public:
    MiniMapView(MiniMapUIPlugin* plugin, PluginManagerInterface* plugins, QWidget* parent = 0);
    ~MiniMapView();

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
    MiniMapPluginInterface* minimap_;
    PathFinderPluginInterface* finder_;
    PositionTrackerPluginInterface* tracker_;
    WalkerPluginInterface* walker_;

    QGraphicsScene* scene_;
    QGraphicsLineItem* horizontalLine_;
    QGraphicsLineItem* verticalLine_;

    MiniMapUIPlugin* plugin_;

    QPoint mousePosition_;
    QList<double> scales_;
    quint8 scaleIndex_;
    quint8 floorIndex_;
};

#endif
