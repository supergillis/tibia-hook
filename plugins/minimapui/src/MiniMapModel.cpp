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

#include <PathInterface.h>

#include <QtCore>
#include <QDebug>

MiniMapModel::MiniMapModel(SenderInterface* sender, MiniMapPluginInterface* miniMap, PathFinderPluginInterface* pathFinder):
    sender_(sender),
    miniMap_(miniMap),
    pathFinder_(pathFinder) {
}

MiniMapFloorInterface* MiniMapModel::floor(int floorIndex) {
    return miniMap_->floor(floorIndex);
}

QList<Position> MiniMapModel::path(const Position& end) {
    Position start;
    start.x = playerPos_.x;
    start.y = playerPos_.y;
    start.z = playerPos_.z;

    return path(start, end);
}

QList<Position> MiniMapModel::path(const Position& start, const Position& end) {
    if (pathFinder_ == NULL) {
        return QList<Position>();
    }

    PathInterface* path = pathFinder_->createNewPath(miniMap_, start, end, -1);
    if (path == NULL) {
        return QList<Position>();
    }

    QList<Position> result = path->path();
    delete path;
    return result;
}

void MiniMapModel::walk(const QList<Position>& path) {
    // TODO put walker in separate plugin
    path_ = path;

    if (path_.length() > 0) {
        QObject::disconnect(this, SIGNAL(playerPositionChanged(const Position&)), this, SLOT(playerMoved(const Position&)));
        QObject::connect(this, SIGNAL(playerPositionChanged(const Position&)), this, SLOT(playerMoved(const Position&)));

        // Intialize walking
        playerMoved(playerPos_);
    }
}

void MiniMapModel::playerMoved(const Position& pos) {
    // TODO clean up this mess!
    qDebug() << "path length" << path_.length();

    if (!path_.empty()) {
        Position nextPos = path_.takeFirst();
        qint16 dx = nextPos.x - pos.x;
        qint16 dy = nextPos.y - pos.y;

        qDebug() << "current" << pos.x << pos.y << pos.z;
        qDebug() << "next" << nextPos.x << nextPos.y << nextPos.z;
        qDebug() << "delta" << dx << dy;

        if (dx == 0 && dy == 0) {
            playerMoved(pos);
            return;
        }
        else if (dx <= 1 && dx >= -1 && dy <= 1 && dy >= -1) {
            quint8 packetType = 0;
            if (dx == -1 && dy == -1) {
                // Northwest
                packetType = 109;
            }
            else if (dx == -1 && dy == 0) {
                // West
                packetType = 104;
            }
            else if (dx == -1 && dy == 1) {
                // Southwest
                packetType = 108;
            }
            else if (dx == 0 && dy == -1) {
                // North
                packetType = 101;
            }
            else if (dx == 0 && dy == 1) {
                // South
                packetType = 103;
            }
            else if (dx == 1 && dy == -1) {
                // Northeast
                packetType = 106;
            }
            else if (dx == 1 && dy == 0) {
                // East
                packetType = 102;
            }
            else if (dx == 1 && dy == 1) {
                // Southeast
                packetType = 107;
            }
            if (packetType != 0) {
                PacketBuilderInterface* builder = sender_->createPacket(1);
                builder->writeU8(packetType);

                PacketInterface* packet = builder->build();
                sender_->sendToServer(QByteArray((const char*) packet->data(), packet->length()));

                delete packet;
                delete builder;
                return;
            }
        }
    }
    qDebug() << "disconnect";
    QObject::disconnect(this, SIGNAL(playerPositionChanged(const Position&)), this, SLOT(playerMoved(const Position&)));
}

void MiniMapModel::handlePacket(PacketReaderInterface& reader) {
    // TODO put position tracker in separate plugin

    quint8 type = reader.readU8();
    if (type == 100) {
        // Totally new position
        playerPos_.x = reader.readU16();
        playerPos_.y = reader.readU16();
        playerPos_.z = reader.readU8();
    }
    else if (type == 101) {
        // Up
        playerPos_.y--;
    }
    else if (type == 102) {
        // Right
        playerPos_.x++;
    }
    else if (type == 103) {
        // Down
        playerPos_.y++;
    }
    else if (type == 104) {
        // Left
        playerPos_.x--;
    }
    else if (type == 190) {
        // Up
        playerPos_.x++;
        playerPos_.y++;
        playerPos_.z--;
    }
    else if (type == 191) {
        // Up
        playerPos_.x--;
        playerPos_.y--;
        playerPos_.z++;
    }

    qDebug() << "player position changed";

    emit playerPositionChanged(playerPos_);
}
