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

#include "PositionTrackerPlugin.h"

#include <PacketCodes.h>
#include <PacketReader.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.positiontracker, PositionTrackerPlugin)
#endif

void PositionTrackerPlugin::install(HookInterface* hook, SettingsInterface*) throw(std::exception) {
    hook_ = hook;
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::MapFull, this);
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::MapTopRow, this);
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::MapRightRow, this);
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::MapBottomRow, this);
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::MapLeftRow, this);
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::FloorChangeUp, this);
    hook_->addIncomingReadOnlyProxy(PacketCodes::In::FloorChangeDown, this);
}

void PositionTrackerPlugin::uninstall() {
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::FloorChangeDown, this);
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::FloorChangeUp, this);
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::MapLeftRow, this);
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::MapBottomRow, this);
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::MapRightRow, this);
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::MapTopRow, this);
    hook_->removeIncomingReadOnlyProxy(PacketCodes::In::MapFull, this);
    hook_ = NULL;
}

Position PositionTrackerPlugin::position() const {
    return position_;
}

void PositionTrackerPlugin::connectPositionChanged(QObject* object, const char* slot, Qt::ConnectionType type) {
    QObject::connect(this, SIGNAL(positionChanged(Position)), object, slot, type);
}

void PositionTrackerPlugin::disconnectPositionChanged(QObject* object, const char* slot) {
    QObject::disconnect(this, SIGNAL(positionChanged(Position)), object, slot);
}

void PositionTrackerPlugin::handlePacket(PacketReader& reader) {
    quint8 type = reader.readU8();

    switch (type) {
    case PacketCodes::In::MapFull:
        position_.x = reader.readU16();
        position_.y = reader.readU16();
        position_.z = reader.readU8();
        break;
    case PacketCodes::In::MapTopRow:
        position_.y--;
        break;
    case PacketCodes::In::MapRightRow:
        position_.x++;
        break;
    case PacketCodes::In::MapBottomRow:
        position_.y++;
        break;
    case PacketCodes::In::MapLeftRow:
        position_.x--;
        break;
    case PacketCodes::In::FloorChangeUp:
        position_.x++;
        position_.y++;
        position_.z--;
        break;
    case PacketCodes::In::FloorChangeDown:
        position_.x--;
        position_.y--;
        position_.z++;
        break;
    }

    emit positionChanged(position_);
}
