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

#include "MiniMapModel.h"

#include <QtCore>
#include <QDebug>

MiniMapModel::MiniMapModel(SenderInterface* sender, MiniMapPluginInterface* miniMap, PathFinderPluginInterface* pathFinder, PositionTrackerPluginInterface* positionTracker):
    sender_(sender),
    miniMap_(miniMap),
    pathFinder_(pathFinder),
    positionTracker_(positionTracker) {
}

MiniMapFloorInterface* MiniMapModel::floor(int floorIndex) {
    return miniMap_->floor(floorIndex);
}

QList<Position> MiniMapModel::path(const Position& end) {
    if (positionTracker_ == NULL) {
        return QList<Position>();
    }
    return path(positionTracker_->position(), end);
}

QList<Position> MiniMapModel::path(const Position& start, const Position& end) {
    if (pathFinder_ == NULL) {
        return QList<Position>();
    }

    QList<Position> path = pathFinder_->findPath(miniMap_, start, end);
    return path;
}
