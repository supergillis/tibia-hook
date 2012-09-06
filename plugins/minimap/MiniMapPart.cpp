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

#include "MiniMapPart.h"

#include <stdexcept>

#include <QFile>
#include <QFileInfo>

const int MiniMapPart::MINIMAP_FILE_WIDTH(256);
const int MiniMapPart::MINIMAP_FILE_HEIGHT(256);

MiniMapPart::MiniMapPart(const QString& fileName) {
    QFile file(fileName);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error(("Could not load minimap " + fileName).toStdString());
    }

    // Read all bytes at once
    quint8 data[2][MINIMAP_FILE_WIDTH][MINIMAP_FILE_HEIGHT];
    file.read((char*) data, 2 * MINIMAP_FILE_WIDTH * MINIMAP_FILE_HEIGHT);

    // Construct the image
    QImage image(MINIMAP_FILE_WIDTH, MINIMAP_FILE_HEIGHT, QImage::Format_RGB32);
    for (int x = 0; x < MINIMAP_FILE_WIDTH; x++) {
        for (int y = 0; y < MINIMAP_FILE_HEIGHT; y++) {
            quint8 color = data[0][x][y];
            image.setPixel(x, y, mapColor(color));
        }
    }

    QString name = QFileInfo(fileName).baseName();
    x_ = name.mid(0, 3).toUInt() * MINIMAP_FILE_WIDTH;
    y_ = name.mid(3, 3).toUInt() * MINIMAP_FILE_HEIGHT;
    z_ = name.mid(6, 2).toUInt();
    image_ = image;
}

MiniMapPart* MiniMapPart::load(const QString& fileName) {
    try {
        return new MiniMapPart(fileName);
    }
    catch (std::exception& exception) {
        return NULL;
    }
}

int MiniMapPart::mapColor(quint8 color) {
    return (0x330000 * (color / 36 % 6) + 0x003300 * (color / 6 % 6) + 0x000033 * (color % 6));
}
