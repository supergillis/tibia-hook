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

#include <stdexcept>

#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.walker, WalkerPlugin)
#endif

Position operator+(const Position& position, const Direction& direction) {
    Position moved(position);
    switch (direction) {
    case NORTH: moved.y -= 1; break;
    case NORTHEAST: moved.y -= 1; moved.x += 1; break;
    case EAST: moved.x += 1; break;
    case SOUTHEAST: moved.y -= 1; moved.x += 1;  break;
    case SOUTH: moved.y += 1; break;
    case SOUTHWEST: moved.y -= 1; moved.x -= 1;  break;
    case WEST: moved.x -= 1; break;
    case NORTHWEST: moved.y -= 1; moved.x -= 1;  break;
    }
    return moved;
}

WalkerPlugin::WalkerPlugin():
    walker_(NULL) {
}

void WalkerPlugin::install(HookInterface* hook, SettingsInterface*) throw(std::exception) {
    QObject* plugin = hook->plugins()->findPluginByName("positiontracker");
    if(plugin == NULL) {
        throw std::runtime_error("The 'positiontracker' plugin must be loaded before loading the 'minimapui' plugin!");
    }

    PositionTrackerPluginInterface* positionPlugin = qobject_cast<PositionTrackerPluginInterface*>(plugin);
    if(positionPlugin == NULL) {
        throw std::runtime_error("The 'positiontracker' plugin could not be loaded!");
    }

    // Try to load the position plugin
    ChannelsPluginInterface* channelsPlugin = NULL;
    plugin = hook->plugins()->findPluginByName("channels");
    if(plugin != NULL) {
        channelsPlugin = qobject_cast<ChannelsPluginInterface*>(plugin);
    }

    walker_ = new Walker(hook->sender(), positionPlugin, channelsPlugin, this);
}

void WalkerPlugin::uninstall() {
    // Clean up objects
    delete walker_;
    walker_ = NULL;
}

void WalkerPlugin::walk(const QList<Direction>& directions) {
    walker_->walk(directions);
}

void WalkerPlugin::move(const Direction& direction) {
    walker_->move(direction);
}

Walker::Walker(SenderInterface* sender, PositionTrackerPluginInterface* positionTracker, ChannelsPluginInterface* channels, QObject* parent):
    QObject(parent),
    sender_(sender),
    positionTracker_(positionTracker),
    channels_(channels),
    channelId_(0) {
}

Walker::~Walker() {
    if (channels_ != NULL) {
        channels_->closeChannel(channelId_);
    }
}

void Walker::walk(const QList<Direction>& directions) {
    if (directions.length() > 0) {
        // Notify when we moved
        directions_ = directions;
        positionTracker_->connectPositionChanged(this, SLOT(moved(const Position&)));

        if (channels_ != NULL) {
            // Open walker channel
            if (channelId_ == 0) {
                channelId_ = channels_->openChannel("Walker");
            }
            channels_->postMessage(channelId_, "On the move!");
        }

        // Intialize walking
        tracking_ = false;
        moved(positionTracker_->position());
    }
}

void Walker::moved(const Position& position) {
    if (tracking_ && position.x != next_.x && position.y != next_.y && position.z != next_.z) {
        qDebug() << "going in the wrong direction!";
    }
    else if (!directions_.empty()) {
        Direction direction = directions_.takeFirst();
        next_ = position + direction;
        tracking_ = true;

        // Do the actual move
        move(direction);

        // Prevent from disconnecting
        return;
    }

    // Disconnect
    tracking_ = false;
    positionTracker_->disconnectPositionChanged(this, SLOT(moved(const Position&)));
}

void Walker::move(const Direction& direction) {
    quint8 packet = 0;

    switch (direction) {
    case NORTH: packet = 101; break;
    case NORTHEAST: packet = 106; break;
    case EAST: packet = 102; break;
    case SOUTHEAST: packet = 107;  break;
    case SOUTH: packet = 103; break;
    case SOUTHWEST: packet = 108;  break;
    case WEST: packet = 104; break;
    case NORTHWEST: packet = 109;  break;
    }

    if (packet != 0) {
        PacketBuilderInterface* builder = sender_->createPacket(1);
        builder->writeU8(packet);

        PacketInterface* packet = builder->build();
        sender_->sendToServer(QByteArray((const char*) packet->data(), packet->length()));

        delete packet;
        delete builder;
    }
}
