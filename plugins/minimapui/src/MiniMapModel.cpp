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

#include <QDebug>

MiniMapModel::MiniMapModel(MiniMapInterface* miniMap): miniMap_(miniMap) {
}

MiniMapFloorInterface* MiniMapModel::floor(int floor) {
    return miniMap_->createNewFloor(floor);
}

void MiniMapModel::handlePacket(PacketReaderInterface& reader) {
    quint8 type = reader.readU8();
    if (type == 109) {
        qDebug() << "creatureMove";
    }
}
