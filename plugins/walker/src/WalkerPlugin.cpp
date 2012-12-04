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

#include "WalkerPlugin.h"

#include <PacketCodes.h>
#include <PacketBuilder.h>
#include <PluginLoader.h>
#include <SenderInterface.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.walker, WalkerPlugin)
#endif

WalkerPlugin::WalkerPlugin():
    sender_(NULL),
    tracker_(NULL) {
}

void WalkerPlugin::install(HookInterface* hook, SettingsInterface*) throw(std::exception) {
    sender_ = hook->sender();
    tracker_ = PluginLoader<PositionTrackerPluginInterface>::load(hook->plugins(), "positiontracker");
}

void WalkerPlugin::uninstall() {
    tracker_ = NULL;
    sender_ = NULL;
}

void WalkerPlugin::walk(const QList<Direction>& directions) {
    if (directions.length() > 0) {
        // Notify when we moved
        directions_ = directions;
        tracker_->connectPositionChanged(this, SLOT(moved(const Position&)));

        // Intialize walking
        walking_ = true;
        next_ = tracker_->position();
        moved(next_);
    }
}

void WalkerPlugin::moved(const Position& position) {
    if (walking_ && position == next_ && !directions_.empty()) {
        // Calculate next move
        Direction direction = directions_.takeFirst();
        next_ = position + direction;

        // Execute next move
        move(direction);
    }
    else {
        walking_ = false;
        tracker_->disconnectPositionChanged(this, SLOT(moved(const Position&)));
    }
}

void WalkerPlugin::move(const Direction& direction) {
    quint8 packet = 0;

    switch (direction) {
    case NORTH: packet = PacketCodes::In::MoveNorth; break;
    case NORTHEAST: packet = PacketCodes::In::MoveNorthEast; break;
    case EAST: packet = PacketCodes::In::MoveEast; break;
    case SOUTHEAST: packet = PacketCodes::In::MoveSouthEast;  break;
    case SOUTH: packet = PacketCodes::In::MoveSouth; break;
    case SOUTHWEST: packet = PacketCodes::In::MoveSouthWest;  break;
    case WEST: packet = PacketCodes::In::MoveWest; break;
    case NORTHWEST: packet = PacketCodes::In::MoveNorthWest;  break;
    }

    if (packet != 0) {
        PacketBuilder builder(1);
        builder.writeU8(packet);

        // Send fake walk packet
        sender_->sendToServer(builder.build());
    }
}
