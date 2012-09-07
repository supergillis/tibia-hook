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

#ifndef MINIMAPPART_H
#define MINIMAPPART_H

#include <MiniMapPartInterface.h>

#include <QByteArray>
#include <QImage>
#include <QRect>

class MiniMapPart: public MiniMapPartInterface {
public:
    MiniMapPart(const QString& fileName);

    const QImage& image() const { return image_; }
    const QByteArray& data() const { return data_; }

    quint8 dataAt(quint16 x, quint16 y) const { return data_[x * MINIMAP_FILE_WIDTH + y]; }

    quint16 width() const { return MINIMAP_FILE_WIDTH; }
    quint16 height() const { return MINIMAP_FILE_HEIGHT; }

    quint16 x() const { return x_; }
    quint16 y() const { return y_; }
    quint8 z() const { return z_; }

private:
    static const int MINIMAP_FILE_WIDTH;
    static const int MINIMAP_FILE_HEIGHT;

    static int mapColor(quint8 color);

    QImage image_;
    QByteArray data_;

    quint16 x_;
    quint16 y_;
    quint8 z_;
};

#endif // MINIMAPPART_H
