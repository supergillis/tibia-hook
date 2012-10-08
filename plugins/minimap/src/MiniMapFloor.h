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

#ifndef MINIMAPFLOOR_H
#define MINIMAPFLOOR_H

#include <MiniMapFloorInterface.h>
#include <MiniMapPartInterface.h>

class MiniMapFloor: public MiniMapFloorInterface {
public:
    MiniMapFloor(const QString&, quint8);
    ~MiniMapFloor();

    const QList<MiniMapPartInterface*>& parts() { return parts_; }
    const QRect& boundary() const { return bounds_; }
    quint8 z() const { return z_; }

    QRgb colorAt(quint16 x, quint16 y) const;
    quint8 dataAt(quint16 x, quint16 y) const;

private:

    QList<MiniMapPartInterface*> parts_;
    QRect bounds_;
    quint8 z_;
};

#endif
