/*This file is part of Circle Packings.

    Circle Packings is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Circle Packings is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Circle Packings.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <QWidget>

#include "basic_types.hpp"

class Canvas_Delegate;
class Window;

class Canvas : public QWidget
{
    Q_OBJECT

    friend class Window;
    friend class Configuration;
    friend class Circle_Packings_Application;

public:
    Canvas(Window *const window, bool left);
    ~Canvas();

    void reset_canvas(bool repaint_canvas = true);

    bool print_in_file(const QString &filename = QString("CP.png"), int quality = 80);

private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent * event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void rescale();
    void reset_selected_stuff();

    void set_left(bool left);
    bool get_is_vertex_highlighted() const;
    void set_is_vertex_highlighted(bool b);
    const unsigned int & get_index_vertex_highlighted() const;
    void set_index_vertex_highlighted(const unsigned int &index);

    unsigned int size_in_pixels_;
    Canvas_Delegate* canvas_delegate_;
    bool left_canvas_;
    Window *const window_;
    QPainter *canvas_painter_;
};


#endif // CANVAS_HPP
