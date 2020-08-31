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

#ifndef CANVAS_DELEGATE_HPP
#define CANVAS_DELEGATE_HPP


#include <QBrush>
#include <QPen>
#include <QColor>
#include <QComboBox>

#include "basic_types.hpp"
#include "point.hpp"

class QPainter;
class QString;
class Window;


class Circle;
class Curve;
template <typename T> class Graph;
class Triangle;
class Triangulation;

class Canvas_Delegate
{
    friend class Canvas;
    friend class Window;
    friend class Configuration;
    friend class Top_Menu;
    friend class Circle_Packings_Application;

public:
    Canvas_Delegate();
    virtual ~Canvas_Delegate() {}

protected:
    void rescale(int size_in_pixels);
    virtual void reset_canvas_delegate(bool) {}
    virtual void reset_canvas_delegate_for_computing() {}
    virtual void reset_selected_stuff();
    virtual void paint_event() {}
    virtual void mouse_press_event() {}
    virtual void mouse_release_event() {}
    virtual void mouse_move_event() {}
    void wheel_event(const int &delta, integer_coordinates point_coordinates_in_pixels);
    virtual void shift_event();
    virtual void ctrl_released() {}
    virtual void ctrl_pressed() {}

    void set_vertex_highlighted(bool is_vertex_highlighted, const unsigned int &index_vertex_highlighted);
    virtual void set_vertex_selected(bool, const unsigned int) {}
    void set_point_clicked(const integer_coordinates &xy);
    void set_point_released(const integer_coordinates &xy);
    void set_point_under_mouse(const integer_coordinates &xy);
    virtual void set_nb_sides(const unsigned int &) {}
    virtual void set_smooth(bool) {}
    virtual void set_shower(bool) {}
    virtual void set_points_radius(double) {}
    virtual void set_left(bool left);

    double y_min_canvas();
    double y_max_canvas();

    // Zoom
    virtual void zoom(const double &delta,
                      integer_coordinates point_coordinates_in_pixels = integer_coordinates(0,0), bool zoom_on_mouse = false);
    virtual void shift_left();
    virtual void shift_right();
    virtual void shift_up();
    virtual void shift_down();

    void common_reset();
    void common_reset_selected_stuff();

    integer_coordinates complex_to_pixels_coordinates(const complex_number &z) const;
    complex_number pixels_to_complex_coordinates(const integer_coordinates &xy) const;

    //Painting
    void draw_point(const Point &point, const QColor &color = QColor("black"));
    void draw_segment(const segment &S, const QColor &color = QColor("black"), const int &width=1);
    void draw_triangle(const Triangle &T, const QColor &color, bool draw_borders);
    void draw_circle(const Circle &circle, const QColor &color, int height_compensation = 0);
    void mark_circle(const Circle &circle, const QColor &color);
    void mark_triangle(const Triangle &T, const QColor &color);

    void draw_curve(const Curve &curve);
    void draw_nodes(const std::vector<Point> &nodes, const double &nodes_radius, const QColor &color = QColor("blue"));
    void draw_tangents(const std::vector<Point> &nodes, const std::vector<complex_number> &tangent_factors,
                       const double &control_points_radius);
    void draw_incomplete_curve(const std::vector<Point> &nodes, const QColor &color = QColor("black"));
    void draw_circle_packing(const Graph<Circle> &circle_packing, bool is_circle_containing_all = false,
                             unsigned int index_circle_containing_all = 0);
    void draw_triangulation(const Triangulation &triangulation);
    void draw_empty_triangulation(const Triangulation &triangulation);
    void draw_planar_graph(const Graph<Point> &planar_graph, const double& graph_points_radius);
    void draw_tiling(const Graph<Circle> &tiling);
    void draw_tiling_cp(const Graph<Circle> &tiling_cp);

    //Image
    bool draw_pixel(int x, int y, QRgb color);


    // Member variables
    int canvas_image_width_;
    QRgb *canvas_image_data_;

    bool left_canvas_;
    int size_in_pixels_;
    int origin_x_in_pixels_;
    int origin_y_in_pixels_;
    double scale_;
    double shift_x_;
    double shift_y_;
    double zoom_;

    bool is_vertex_highlighted_;
    unsigned int index_vertex_highlighted_;

    bool is_vertex_under_mouse_;
    unsigned int index_vertex_under_mouse_;

    Point point_clicked_;
    Point point_released_;
    Point point_under_mouse_;
    bool click_on_;

    bool draw_borders_;

    QPainter *painter_;
    QPen pen_;
    QBrush brush_;

    Window *window_;
    QString *message_;

    // Draw image stuff
    virtual void set_image_mode(bool) {}
    virtual void set_image_parameters() {}
};

#endif // CANVAS_DELEGATE_HPP
