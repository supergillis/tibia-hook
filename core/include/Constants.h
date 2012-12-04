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

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {

enum TalkMode {
    None = 0,
    Say = 1,
    Whisper = 2,
    Yell = 3,
    PrivateFrom = 4,
    PrivateTo = 5,
    ChannelManagement = 6,
    Channel = 7,
    ChannelHighlight = 8,
    Spell = 9,
    NpcFrom = 10,
    NpcTo = 11,
    GamemasterBroadcast = 12,
    GamemasterChannel = 13,
    GamemasterPrivateFrom = 14,
    GamemasterPrivateTo = 15,
    Login = 16,
    Warning = 17,
    Game = 18,
    Failure = 19,
    Look = 20,
    DamageDealed = 21,
    DamageReceived = 22,
    Heal = 23,
    Exp = 24,
    DamageOthers = 25,
    HealOthers = 26,
    ExpOthers = 27,
    Status = 28,
    Loot = 29,
    TradeNpc = 30,
    Guild = 31,
    PartyManagement = 32,
    Party = 33,
    BarkLow = 34,
    BarkLoud = 35,
    Report = 36,
    HotkeyUse = 37,
    TutorialHint = 38,
    Thankyou = 39,
    Market = 40,
    BeyondLast = 41,

    // Deprecated
    MonsterYell = 42,
    MonsterSay = 43,
    Red = 44,
    Blue = 45,
    RVRChannel = 46,
    RVRAnswer = 47,
    RVRContinue = 48,
    MessageLast = 49,
    Invalid = 255
};

}

#endif
