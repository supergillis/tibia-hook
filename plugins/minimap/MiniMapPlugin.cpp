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

#include "MiniMapPlugin.h"

#include <stdexcept>

#include <QVariantMap>
#include <QPainter>

#define SETTING_FOLDER "folder"

void MiniMapPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
    if(!settings->value(SETTING_FOLDER).isValid()) {
        throw std::runtime_error("Could not load minimap folder!");
    }

    miniMap_ = new MiniMap(settings->value(SETTING_FOLDER).toString());
}

void MiniMapPlugin::uninstall() {
}

MiniMapInterface* MiniMapPlugin::miniMap() {
    return miniMap_;
}

// Export plugin
Q_EXPORT_PLUGIN2(minimap, MiniMapPlugin)

const int MiniMap::MINIMAP_FILE_DIMENSION(256);

MiniMap::MiniMap(const QString& directory): directory_(directory) {
}

QImage MiniMap::imageForFloor(quint8 z) const {
    QStringList fileFilters;
    QString fileFilter;
    fileFilter.sprintf("*%02d.map", z);
    fileFilters << fileFilter;

    QStringList files = directory_.entryList(fileFilters);
    QRect bounds = mapFilesBoundary(files);

    QImage image(bounds.width(), bounds.height(), QImage::Format_RGB32);
    image.fill(Qt::black);

    QPainter painter;
    painter.begin(&image);

    foreach(const QString& file, files) {
        Position position = positionForMapFile(file);
        if(position.z() == z) {
            QString absoluteFile = directory_.absoluteFilePath(file);
            QImage mapFileImage = imageForMapFile(absoluteFile);
            int x = position.x() - bounds.left();
            int y = position.y() - bounds.top();
            painter.drawImage(mapFileImage.rect().translated(x, y), mapFileImage, mapFileImage.rect());
        }
    }
    painter.end();

    return image;
}

QImage MiniMap::imageForMapFile(const QString& mapFile) {
    QImage image(MINIMAP_FILE_DIMENSION, MINIMAP_FILE_DIMENSION, QImage::Format_RGB32);
    QFile file(mapFile);

    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        // Read all bytes at once
        quint8 data[2][MINIMAP_FILE_DIMENSION][MINIMAP_FILE_DIMENSION];
        file.read((char*) data, 2 * MINIMAP_FILE_DIMENSION * MINIMAP_FILE_DIMENSION);

        // Loop through the array
        for (int x = 0; x < MINIMAP_FILE_DIMENSION; x++) {
            for (int y = 0; y < MINIMAP_FILE_DIMENSION; y++) {
                quint8 color = data[0][x][y];
                image.setPixel(x, y, mapColor(color));
            }
        }
    }
    return image;
}

QRect MiniMap::mapFilesBoundary(const QStringList& mapFiles) {
    quint16 left = 0;
    quint16 right = 0;
    quint16 top = 0;
    quint16 bottom = 0;
    bool first = true;
    foreach (const QString& mapFile, mapFiles) {
        Position position = positionForMapFile(mapFile);
        if (first) {
            left = right = position.x();
            top = bottom = position.y();
            first = false;
        }
        else {
            if (position.x() < left) {
                left = position.x();
            }
            else if (position.x() > right) {
                right = position.x();
            }

            if (position.y() < top) {
                top = position.y();
            }
            else if (position.y() > bottom) {
                bottom = position.y();
            }
        }
    }
    return QRect(left, top, right - left + MINIMAP_FILE_DIMENSION, bottom - top + MINIMAP_FILE_DIMENSION);
}

Position MiniMap::positionForMapFile(const QString& fileName) {
    if (fileName.length() == 12 || fileName.length() == 8) {
        quint16 x = fileName.mid(0, 3).toUInt() * MINIMAP_FILE_DIMENSION;
        quint16 y = fileName.mid(3, 3).toUInt() * MINIMAP_FILE_DIMENSION;
        quint8 z = fileName.mid(6, 2).toUInt();
        return Position(x, y, z);
    }
    return Position();
}

int MiniMap::mapColor(quint8 color) {
    return (0x330000 * (color / 36 % 6) + 0x003300 * (color / 6 % 6) + 0x000033 * (color % 6));
}
