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

#ifndef MINIMAPVIEW_H
#define MINIMAPVIEW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>

#include "MiniMapModel.h"

class MiniMapView: public QScrollArea {
    Q_OBJECT
    
public:
    MiniMapView(QWidget* = 0);
    ~MiniMapView();

    void setModel(MiniMapModel*);

    void wheelEvent(QWheelEvent*);

public slots:
    void refresh();
    void scaleFactor(double);
    
private:
    static void adjustScrollBar(QScrollBar* scrollBar, double factor);

    MiniMapModel* model_;

    QLabel* imageLabel_;
    QScrollArea* scrollArea_;
    double scaleFactor_;
};

#endif
