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

#include "Walker.h"

#include <QtCore>
#include <QDebug>

Walker::Walker(SenderInterface* sender, PositionTrackerPluginInterface* positionTracker):
    sender_(sender),
    positionTracker_(positionTracker) {
}

void Walker::walk(const QList<Position>& path) {
    if (path.length() > 0) {
        // Notify when we moved
        path_ = path;
        positionTracker_->connectPositionChanged(this, SLOT(moved(const Position&)));

        // Intialize walking
        moved(positionTracker_->position());
    }
}

void Walker::moved(const Position& position) {
    if (!path_.empty()) {
        Position nextPos = path_.takeFirst();
        int dx = nextPos.x - position.x;
        int dy = nextPos.y - position.y;

        if (dx == 0 && dy == 0) {
            moved(position);
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
    positionTracker_->disconnectPositionChanged(this, SLOT(moved(const Position&)));
}
