/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"),
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

#ifndef PACKETTYPES_H
#define PACKETTYPES_H

#include <QtGlobal>

namespace PacketCodes {

namespace In {
const quint8 MapFull = 100,
MapTopRow = 101,
MapRightRow = 102,
MapBottomRow = 103,
MapLeftRow = 104,

FloorChangeUp = 190,
FloorChangeDown = 191,

Speak = 170,
ChannelList = 171,
ChannelOpen = 172,
ChannelOpenPrivate = 173,
ChannelClose = 179,


MoveNorth = 101,
MoveNorthEast = 106,
MoveEast = 102,
MoveSouthEast = 107,
MoveSouth = 103,
MoveSouthWest = 108,
MoveWest = 104,
MoveNorthWest = 109;
}

}

#endif
