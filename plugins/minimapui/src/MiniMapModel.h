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

#ifndef MINIMAPMODEL_H
#define MINIMAPMODEL_H

#include <MiniMapFloorInterface.h>
#include <MiniMapPluginInterface.h>
#include <PathFinderPluginInterface.h>
#include <Position.h>
#include <PositionTrackerPluginInterface.h>
#include <ReadOnlyProxyInterface.h>
#include <SenderInterface.h>

#include <QImage>
#include <QObject>

class MiniMapModel: public QObject {
    Q_OBJECT

public:
    MiniMapModel(SenderInterface* sender, MiniMapPluginInterface* miniMap, PathFinderPluginInterface* pathFinder, PositionTrackerPluginInterface* positionTracker);

    MiniMapFloorInterface* floor(int floorIndex);

    MiniMapPluginInterface* miniMap() { return miniMap_; }
    PathFinderPluginInterface* pathFinder() { return pathFinder_; }

    QList<Position> path(const Position& end);
    QList<Position> path(const Position& start, const Position& end);

    void walk(const QList<Position> &path);

private slots:
    void playerMoved(const Position& position);

private:
    SenderInterface* sender_;
    MiniMapPluginInterface* miniMap_;
    PathFinderPluginInterface* pathFinder_;
    PositionTrackerPluginInterface* positionTracker_;

    QList<Position> path_;
};

#endif
