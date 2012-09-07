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

#include "MiniMapFloor.h"
#include "MiniMapPart.h"

#include <QDir>

MiniMapFloor::MiniMapFloor(const QString& directory, quint8 floor) {
    QStringList fileFilters;
    QString fileFilter;
    fileFilter.sprintf("*%02d.map", floor);
    fileFilters << fileFilter;

    QDir dir(directory);
    QStringList fileNames = dir.entryList(fileFilters);
    foreach (const QString& fileName, fileNames) {
        try {
            MiniMapPart* part = new MiniMapPart(dir.absoluteFilePath(fileName));
            parts_.append(part);
        }
        catch (std::exception& exception) {
        }
    }

    quint16 left = 0;
    quint16 right = 0;
    quint16 top = 0;
    quint16 bottom = 0;

    // Calculate bounds
    bool first = true;
    foreach (const MiniMapPartInterface* part, parts_) {
        if (first) {
            left = right = part->x();
            top = bottom = part->y();
            first = false;
        }
        else {
            quint16 partLeft = part->x();
            quint16 partRight = part->x() + part->width();
            if (partLeft < left) {
                left = partLeft;
            }
            else if (partRight > right) {
                right = partRight;
            }

            quint16 partTop = part->y();
            quint16 partBottom = part->y() + part->height();
            if (partTop < top) {
                top = partTop;
            }
            else if (partBottom > bottom) {
                bottom = partBottom;
            }
        }
    }

    bounds_ =  QRect(left, top, right - left, bottom - top);
    z_ = floor;
}

MiniMapFloor::~MiniMapFloor() {
    foreach (const MiniMapPartInterface* part, parts_) {
        delete part;
    }
}
