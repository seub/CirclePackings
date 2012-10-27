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

#include <QPainter>
#include <QString>
#include <QApplication>

#include "canvas_delegate.hpp"
#include "graph.hpp"
#include "curve.hpp"
#include "point.hpp"
#include "circle.hpp"
#include "triangulation.hpp"
#include "triangle.hpp"
#include "window.hpp"


Canvas_Delegate::Canvas_Delegate()
{
}

void Canvas_Delegate::rescale(int size_in_pixels)
{
    //std::cout << "Entering Canvas_Delegate::rescale" << std::endl;
    size_in_pixels_ = size_in_pixels;
    scale_ = zoom_*size_in_pixels/3.0;
    origin_x_in_pixels_ = size_in_pixels/2.0 + shift_x_*scale_;
    origin_y_in_pixels_ = size_in_pixels/2.0 + shift_y_*scale_;

    return;
}

void Canvas_Delegate::common_reset()
{
    //std::cout << "Entering Canvas_Delegate::common_reset" << std::endl;
    scale_ = size_in_pixels_/3.0;
    origin_x_in_pixels_ = int_round(size_in_pixels_/2.0);
    origin_y_in_pixels_ = int_round(size_in_pixels_/2.0);
    shift_x_ = 0.0;
    shift_y_ = 0.0;
    zoom_ = 1.0;

    click_on_ = false;
    is_vertex_highlighted_ = false;
    is_vertex_under_mouse_ = false;

    return;
}

void Canvas_Delegate::common_reset_selected_stuff()
{
    is_vertex_highlighted_ = false;
    is_vertex_under_mouse_ = false;
    return;
}

void Canvas_Delegate::reset_selected_stuff()
{
    common_reset_selected_stuff();
    return;
}

void Canvas_Delegate::set_vertex_highlighted(bool is_vertex_highlighted, const unsigned int &index_vertex_highlighted)
{
    is_vertex_highlighted_ = is_vertex_highlighted;
    index_vertex_highlighted_ = index_vertex_highlighted;
    return;
}

double Canvas_Delegate::y_min_canvas()
{
    return imag(pixels_to_complex_coordinates(integer_coordinates(0, size_in_pixels_)));
}

double Canvas_Delegate::y_max_canvas()
{
    return imag(pixels_to_complex_coordinates(integer_coordinates(0, 0)));
}

integer_coordinates Canvas_Delegate::complex_to_pixels_coordinates(const complex_number &z) const
{
    int X = int_round(real(z)*scale_ + origin_x_in_pixels_);
    int Y = int_round(-imag(z)*scale_ + origin_y_in_pixels_);
    return integer_coordinates(X,Y);
}

complex_number Canvas_Delegate::pixels_to_complex_coordinates(const integer_coordinates &xy) const
{
    double x = (1.0*xy.first - 1.0*origin_x_in_pixels_)*1.0/scale_;
    double y = (1.0*origin_y_in_pixels_ - 1.0*xy.second)*1.0/scale_;
    return complex_number(x,y);
}

void Canvas_Delegate::draw_point(const Point &point, const QColor &color)
{
    int x = complex_to_pixels_coordinates(point.get_affix()).first;
    int y = complex_to_pixels_coordinates(point.get_affix()).second;
    pen_.setWidth(1);
    pen_.setColor(color);
    painter_->setPen(pen_);
    painter_->drawPoint(x,y);
    return;
}

void Canvas_Delegate::draw_segment(const segment &S, const QColor &color, const int &width)
{
    int x_1 = complex_to_pixels_coordinates(S.first.get_affix()).first;
    int y_1 = complex_to_pixels_coordinates(S.first.get_affix()).second;
    int x_2 = complex_to_pixels_coordinates(S.second.get_affix()).first;
    int y_2 = complex_to_pixels_coordinates(S.second.get_affix()).second;
    pen_.setWidth(width);
    pen_.setColor(color);
    painter_->setPen(pen_);
    painter_->drawLine(QPoint(x_1, y_1), QPoint(x_2, y_2));
    return;
}

void Canvas_Delegate::draw_triangle(const Triangle &T, const QColor &color, bool draw_borders)
{
    Point A, B, C;
    T.get_ABC(A, B, C);
    complex_number z_A = A.get_affix(), z_B = B.get_affix(), z_C = C.get_affix();
    integer_coordinates xy_A = complex_to_pixels_coordinates(z_A);
    integer_coordinates xy_B = complex_to_pixels_coordinates(z_B);
    integer_coordinates xy_C = complex_to_pixels_coordinates(z_C);
    brush_.setColor(color);
    if (draw_borders)
    {
        pen_.setWidth(1);
        pen_.setColor(QColor("black"));
        painter_->setPen(pen_);
    }
    else
    {
        painter_->setPen(color);
    }

    painter_->setBrush(brush_);
    QVector<QPoint> triangle;
    triangle.append(QPoint(xy_A.first, xy_A.second));
    triangle.append(QPoint(xy_B.first, xy_B.second));
    triangle.append(QPoint(xy_C.first, xy_C.second));
    painter_->drawPolygon(QPolygon(triangle));
    return;
}

void Canvas_Delegate::draw_circle(const Circle &circle, QColor const& color, int height_compensation)
{
    double r = circle.get_radius();
    complex_number first_corner = circle.get_affix() + complex_number(-r,r);
    complex_number second_corner = circle.get_affix() + complex_number(r,-r);
    integer_coordinates first_corner_pixels = complex_to_pixels_coordinates(first_corner);
    integer_coordinates second_corner_pixels = complex_to_pixels_coordinates(second_corner);
    int width = second_corner_pixels.first - first_corner_pixels.first;
    int height = second_corner_pixels.second - first_corner_pixels.second;
    brush_.setColor(color);
    painter_->setBrush(brush_);
    painter_->drawEllipse(first_corner_pixels.first, first_corner_pixels.second, width, height +
                          height_compensation);
    return;
}

void Canvas_Delegate::mark_circle(const Circle &circle, const QColor &color)
{
    double r = circle.get_radius();
    complex_number first_corner = circle.get_affix() + complex_number(-r,r);
    complex_number second_corner = circle.get_affix() + complex_number(r,-r);
    integer_coordinates first_corner_pixels = complex_to_pixels_coordinates(first_corner);
    integer_coordinates second_corner_pixels = complex_to_pixels_coordinates(second_corner);
    int width = second_corner_pixels.first - first_corner_pixels.first;
    int height = second_corner_pixels.second - first_corner_pixels.second;
    pen_.setColor(color);
    pen_.setWidth(3);
    painter_->setPen(pen_);
    painter_->setBrush(Qt::NoBrush);
    painter_->drawEllipse(first_corner_pixels.first, first_corner_pixels.second, width, height);
    return;
}

void Canvas_Delegate::mark_triangle(const Triangle &T, const QColor &color)
{
    //std::cout << "throw test in Canvas_Delegate::mark_triangle" << std::endl;
    //throw(QString("ERROR in Canvas_Delegate::mark_triangle: sample error"));

    Point A, B, C;
    T.get_ABC(A, B, C);
    complex_number z_A = A.get_affix(), z_B = B.get_affix(), z_C = C.get_affix();
    integer_coordinates xy_A = complex_to_pixels_coordinates(z_A);
    integer_coordinates xy_B = complex_to_pixels_coordinates(z_B);
    integer_coordinates xy_C = complex_to_pixels_coordinates(z_C);
    pen_.setColor(color);
    pen_.setWidth(3);
    painter_->setPen(pen_);
    painter_->setBrush(Qt::NoBrush);
    QVector<QPoint> triangle;
    triangle.append(QPoint(xy_A.first, xy_A.second));
    triangle.append(QPoint(xy_B.first, xy_B.second));
    triangle.append(QPoint(xy_C.first, xy_C.second));
    painter_->drawPolygon(QPolygon(triangle));
    return;
}

void Canvas_Delegate::draw_curve(const Curve& curve)
{
    QColor black("black");
    unsigned int i;
    for (i=0; i<curve.nb_points(); i++)
    {
        draw_segment(curve.get_segment(i), black, 2);
    }
    return;
}

void Canvas_Delegate::draw_incomplete_curve(const std::vector<Point> &nodes, const QColor &color)
{
    unsigned int i;
    if (nodes.size() > 1)
    {
        for (i=0; i<nodes.size()-1; i++)
        {
            draw_segment(segment(nodes[i], nodes[i+1]), color, 2);
        }
    }
    return;
}


void Canvas_Delegate::draw_nodes(const std::vector<Point> &nodes, const double &nodes_radius, const QColor &color)
{
    unsigned int i;
    painter_->setPen(Qt::NoPen);
    for (i=0; i<nodes.size(); i++)
    {
        draw_circle(Circle(nodes[i], nodes_radius), color);
    }
    return;
}

void Canvas_Delegate::draw_tangents(const std::vector<Point> &nodes,
                                    const std::vector<complex_number> &tangent_factors,
                                    const double &control_points_radius)
{
    QColor color("red");
    unsigned int i, N = nodes.size();
    complex_number z_left, z_right;
    Point T;
    for (i=0; i<N; i++)
    {
        z_left = nodes[(N+i-1)%N].get_affix();
        z_right = nodes[(i+1)%N].get_affix();
        T = Point(nodes[i].get_affix() + tangent_factors[i] * (z_right - z_left));
        draw_segment(segment(nodes[i], T), color);
        mark_circle(Circle(T, control_points_radius), color);
    }
    return;
}

void Canvas_Delegate::draw_circle_packing(const Graph<Circle> &circle_packing,
                                          bool is_circle_containing_all, unsigned int index_circle_containing_all)
{
    if (draw_borders_)
    {
        pen_.setWidth(1);
        pen_.setColor(QColor("black"));
        painter_->setPen(pen_);
    }
    else
    {
        painter_->setPen(Qt::NoPen);
    }

    unsigned int i;
    if (is_circle_containing_all)
    {
        painter_->setOpacity(0.5);
        draw_circle(circle_packing.get_content_by_index(index_circle_containing_all),
                    circle_packing.get_color_by_index(index_circle_containing_all));
        painter_->setOpacity(1.0);

        for(i=0; i<circle_packing.nb_vertices(); i++)
        {
            if (i != index_circle_containing_all)
            {
                draw_circle(circle_packing.get_content_by_index(i), circle_packing.get_color_by_index(i));
            }
        }
    }
    else
    {
        for(i=0; i<circle_packing.nb_vertices(); i++)
        {
            draw_circle(circle_packing.get_content_by_index(i), circle_packing.get_color_by_index(i));
        }
    }
    return;
}

void Canvas_Delegate::draw_tiling_cp(const Graph<Circle> &tiling_cp)
{
    if (draw_borders_)
    {
        pen_.setWidth(1);
        pen_.setColor(QColor("black"));
        painter_->setPen(pen_);
    }
    else
    {
        painter_->setPen(Qt::NoPen);
    }

    complex_number centre, top_left_corner, bottom_right_corner;
    double radius;
    int height, height_compensation;
    integer_coordinates top_left_corner_pixels, top_left_corner_pixels_below, bottom_right_corner_pixels;

    unsigned int i;
    for(i=0; i<tiling_cp.nb_vertices(); i++)
    {

        radius = tiling_cp.get_content_by_index(i).get_radius();
        centre = tiling_cp.get_affix_by_index(i);

        top_left_corner = centre + complex_number(-radius,radius);
        bottom_right_corner = centre + complex_number(radius,-radius);
        top_left_corner_pixels = complex_to_pixels_coordinates(top_left_corner);
        top_left_corner_pixels_below = complex_to_pixels_coordinates(top_left_corner + complex_number(radius, -sqrt(3.0)*radius));
        bottom_right_corner_pixels = complex_to_pixels_coordinates(bottom_right_corner);
        height = bottom_right_corner_pixels.second - top_left_corner_pixels.second;
        height_compensation =  int_round(scale_*((2.0-sqrt(3.0))*radius)) -
                (height - (top_left_corner_pixels_below.second - top_left_corner_pixels.second));

        draw_circle(tiling_cp.get_content_by_index(i), tiling_cp.get_color_by_index(i), height_compensation);
    }

    return;
}

void Canvas_Delegate::draw_triangulation(const Triangulation &triangulation)
{
    pen_.setWidth(1);
    painter_->setPen(pen_);

    Triangle T;
    unsigned int i;
    for (i=0; i<triangulation.nb_triangles(); i++)
    {
        if (triangulation.get_triangle(i, T))
        {
            draw_triangle(T, triangulation.get_color(i), draw_borders_);
        }
    }
}

void Canvas_Delegate::draw_empty_triangulation(const Triangulation &triangulation)
{
    painter_->setOpacity(0.2);

    QColor transparent(0, 0, 0, 0);
    Triangle T;
    unsigned int i;
    for (i=0; i<triangulation.nb_triangles(); i++)
    {
        if (triangulation.get_triangle(i, T))
        {
            draw_triangle(T, transparent, true);
        }
    }
    painter_->setOpacity(1.0);
}

void Canvas_Delegate::draw_planar_graph(const Graph<Point> &planar_graph, const double& graph_points_radius)
{
    std::vector<vertex_label> N;
    Point P_i, P_j;
    unsigned int i, j;

    for (i=0; i<planar_graph.nb_vertices(); i++)
    {
        P_i = planar_graph.get_content_by_index(i);
        N = planar_graph.get_neighbors_by_index(i);
        for (j=0; j<N.size(); j++)
        {
            if (N[j]< (int) i)
            {
                continue;
            }
            P_j = planar_graph.get_content_by_label(N[j]);
            draw_segment(segment(P_i, P_j));
        }
    }

    painter_->setPen(Qt::NoPen);

    for (i=0; i<planar_graph.nb_vertices(); i++)
    {
        P_i = planar_graph.get_content_by_index(i);
        draw_circle(Circle(P_i, graph_points_radius), planar_graph.get_color_by_index(i));
    }

    return;
}

void Canvas_Delegate::draw_tiling(const Graph<Circle> &tiling)
{
    QColor black("black");
    unsigned int i;
    for (i=0; i<tiling.nb_vertices(); i++)
    {
        draw_point(tiling.get_content_by_index(i).get_centre(), black);
    }
    return;
}

bool Canvas_Delegate::draw_pixel(int x, int y, QRgb color)
{
    if(x<0 || y<0 || x >= size_in_pixels_ || y >= size_in_pixels_)
    {
        return false;
    }

    *(canvas_image_data_ + y * canvas_image_width_ + x) = color;
    return true;
}



void Canvas_Delegate::wheel_event(const int &delta, integer_coordinates point_coordinates_in_pixels)
{
    //std::cout << "Entering Canvas_Delegate::wheel_event with delta = " << delta << std::endl;
    double coeff = pow(2,0.5*delta/240.0);
    zoom(coeff, point_coordinates_in_pixels, true);
    return;
}

void Canvas_Delegate::shift_event()
{
    complex_number delta = scale_*(point_under_mouse_.get_affix() - point_clicked_.get_affix());
    double x_shift = real(delta);
    double y_shift = imag(delta);

    if(origin_x_in_pixels_ + x_shift> scale_*1.5)
    {
        origin_x_in_pixels_ = int_round(scale_*1.5);

    }
    else if(origin_x_in_pixels_ + x_shift < size_in_pixels_-scale_*1.5)
    {
        origin_x_in_pixels_ = int_round(size_in_pixels_-scale_*1.5);
    }
    else
    {
        origin_x_in_pixels_ = origin_x_in_pixels_ + x_shift;
    }

    if(origin_y_in_pixels_ - y_shift > scale_*1.5)
    {
        origin_y_in_pixels_ = int_round(scale_*1.5);
    }
    else if(origin_y_in_pixels_ - y_shift < size_in_pixels_-scale_*1.5)
    {
        origin_y_in_pixels_ = int_round(size_in_pixels_ -scale_*1.5);
    }
    else
    {
        origin_y_in_pixels_ = origin_y_in_pixels_ - y_shift;
    }
    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    return;
}


void Canvas_Delegate::zoom(const double &coeff, integer_coordinates point_coordinates_in_pixels, bool zoom_on_mouse)
{
    bool update_point_under_mouse = false;
    double coeff2;
    int x, y;
    double X, Y;
    integer_coordinates last;

    if (zoom_on_mouse)
    {
        x = point_coordinates_in_pixels.first;
        y = point_coordinates_in_pixels.second;
        X = real(point_under_mouse_.get_affix());
        Y = imag(point_under_mouse_.get_affix());
    }
    else
    {
        x = size_in_pixels_/2;
        y = size_in_pixels_/2;
        complex_number Z = pixels_to_complex_coordinates(integer_coordinates(x, y));
        X = real(Z);
        Y = imag(Z);
        last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    }


    if(scale_*coeff<size_in_pixels_/3)
    {
        coeff2 = size_in_pixels_/(scale_*3);
        zoom_ = 1;
    }
    else
    {
        coeff2 = coeff;
        zoom_*=coeff;
    }

    scale_ *= coeff2;

    if(int_round(x- scale_*X)> scale_*1.5)
    {
        origin_x_in_pixels_ = scale_*1.5;
        update_point_under_mouse = true;
    }
    else if(int_round(x- scale_*X) < size_in_pixels_-scale_*1.5)
    {
        origin_x_in_pixels_ = size_in_pixels_-scale_*1.5;
        update_point_under_mouse = true;
    }
    else
    {
        //origin_x_in_pixels_ = size_in_pixels_/2 -coeff2*(size_in_pixels_/2-origin_x_in_pixels_);
        origin_x_in_pixels_ = int_round(x- scale_*X);
    }

    if(int_round(y+ scale_*Y) > scale_*1.5)
    {
        origin_y_in_pixels_ = scale_*1.5;
        update_point_under_mouse = true;
    }
    else if(int_round(y+ scale_*Y) < size_in_pixels_-scale_*1.5)
    {
        origin_y_in_pixels_ = size_in_pixels_ -scale_*1.5;
        update_point_under_mouse = true;
    }
    else
    {
        //origin_y_in_pixels_ = size_in_pixels_/2 - coeff2*(size_in_pixels_/2-origin_y_in_pixels_);
        origin_y_in_pixels_ = int_round(y+ scale_*Y);
    }


    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    if (update_point_under_mouse && zoom_on_mouse)
    {
        //point_under_mouse_ = Point(pixels_to_complex_coordinates(point_coordinates_in_pixels));
        set_point_under_mouse(point_coordinates_in_pixels);
        mouse_move_event();
    }
    else if(!zoom_on_mouse)
    {
        set_point_under_mouse(last);
        mouse_move_event();
    }

    return;
}


void Canvas_Delegate::shift_left()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int x_shift = 20;
    if(origin_x_in_pixels_ + x_shift> scale_*1.5)
    {
        origin_x_in_pixels_ = scale_*1.5;
    }
    else if(origin_x_in_pixels_ + x_shift < size_in_pixels_-scale_*1.5)
    {
        origin_x_in_pixels_ = size_in_pixels_-scale_*1.5;
    }
    else
    {
        origin_x_in_pixels_ = origin_x_in_pixels_ + x_shift;
    }
    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}

void Canvas_Delegate::shift_right()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int x_shift = -20;
    if(origin_x_in_pixels_ + x_shift> scale_*1.5)
    {
        origin_x_in_pixels_ = scale_*1.5;
    }
    else if(origin_x_in_pixels_ + x_shift < size_in_pixels_-scale_*1.5)
    {
        origin_x_in_pixels_ = size_in_pixels_-scale_*1.5;
    }
    else
    {
        origin_x_in_pixels_ = origin_x_in_pixels_ + x_shift;
    }
    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}

void Canvas_Delegate::shift_up()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int y_shift = -20;
    if(origin_y_in_pixels_ - y_shift > scale_*1.5)
    {
        origin_y_in_pixels_ = scale_*1.5;
    }
    else if(origin_y_in_pixels_ - y_shift < size_in_pixels_-scale_*1.5)
    {
        origin_y_in_pixels_ = size_in_pixels_ -scale_*1.5;
    }
    else
    {
        origin_y_in_pixels_ = origin_y_in_pixels_ - y_shift;
    }
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}

void Canvas_Delegate::shift_down()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int y_shift = 20;
    if(origin_y_in_pixels_ - y_shift > scale_*1.5)
    {
        origin_y_in_pixels_ = scale_*1.5;
    }
    else if(origin_y_in_pixels_ - y_shift < size_in_pixels_-scale_*1.5)
    {
        origin_y_in_pixels_ = size_in_pixels_ -scale_*1.5;
    }
    else
    {
        origin_y_in_pixels_ = origin_y_in_pixels_ - y_shift;
    }
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}

void Canvas_Delegate::set_point_clicked(const integer_coordinates &xy)
{
    point_clicked_ = Point(pixels_to_complex_coordinates(xy));
}

void Canvas_Delegate::set_point_released(const integer_coordinates &xy)
{
    point_released_ = Point(pixels_to_complex_coordinates(xy));
}


void Canvas_Delegate::set_point_under_mouse(const integer_coordinates &xy)
{
    point_under_mouse_ = Point(pixels_to_complex_coordinates(xy));
}

void Canvas_Delegate::set_left(bool left)
{
    left_canvas_ = left;
    return;
}
