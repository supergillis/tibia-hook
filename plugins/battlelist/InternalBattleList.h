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

#ifndef INTERNALBATTLELIST_H
#define INTERNALBATTLELIST_H

#define BATTLELIST_ADDRESS 0x844d360
#define BATTLELIST_LENGTH 250
#define BATTLELIST_NAME_LENGTH 32

/**
  * Credits to DarkstaR
  *
  * http://tpforums.org/forum/thread-7914-post-71664.html#pid71664
  */
namespace InternalBattleList {

struct Outfit
{
	quint32 id;
	quint32 colorHead;
	quint32 colorBody;
	quint32 colorLegs;
	quint32 colorFeet;
	quint32 addon;
	quint32 mount;
} __attribute__((packed));

struct Position
{
	quint32 x;
	quint32 y;
	quint32 z;
} __attribute__((packed));

struct Entry
{
	quint32 id; //+0
	quint8 name[BATTLELIST_NAME_LENGTH]; //+4
	Position position; //+36
	quint32 screenOffsetHorizontal; //+48
	quint32 screenOffsetVertical; //+52
	quint32 unknown1; //+56
	quint32 unknown2; //+60
	quint32 unknown3; //+64
	quint32 unknown4; //+68
	quint32 unknown5; //+72
	quint32 isWalking; //+76
	quint32 direction; //+80
	quint32 unknown6; //+84
	quint32 unknown7; //+88
	quint32 unknown8; //+92
	Outfit outfit; //+96
	quint32 lightType; //+124
	quint32 lightColor; //+128
	quint32 squareColor; //+132
	quint32 squareTime; //+136
	quint32 healthPercent; //+140
	quint32 walkSpeed; //+144
	quint32 isVisible; //+148
	quint32 skullIcon; //+152
	quint32 partyIcon; //+156
	quint32 unknown9; //+160
	quint32 warIcon; //+164
	quint32 isBlocking; //+168
} __attribute__((packed));

struct List
{
	Entry entries[BATTLELIST_LENGTH];
} __attribute__((packed));

}

#endif // INTERNALBATTLELIST_H
