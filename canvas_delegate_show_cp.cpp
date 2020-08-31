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

#include "canvas_delegate_show_cp.hpp"
#include "window.hpp"

#include <QComboBox>
#include <QString>
#include <QApplication>
#include <QPainter>

Canvas_Delegate_Show_CP::Canvas_Delegate_Show_CP(
        const int &size_in_pixels, const mode_type *const mode_flag, const draw_graph_mode_type *const draw_graph_flag,
        const draw_curve_mode_type *const draw_curve_flag, Graph<Circle> *const circle_packing_save,
        Graph<Circle> *const circle_packing, Window *window, bool reset_contents) :
    mode_flag_(mode_flag), draw_graph_flag_(draw_graph_flag), draw_curve_flag_(draw_curve_flag),
    circle_packing_save_(circle_packing_save), circle_packing_(circle_packing)
{
    window_ = window;
    message_ = &(window_->message_received_);
    size_in_pixels_ = size_in_pixels;

    reset_canvas_delegate(reset_contents);
}

void Canvas_Delegate_Show_CP::reset_canvas_delegate(bool reset_contents)
{
    common_reset();
    if (reset_contents)
    {
        circle_packing_save_->reset_graph();
        circle_packing_->reset_graph();
        mobius_ = Mobius(complex_number(0.0, 0.0));
    }
    is_circle_containing_all_ = false;
    return;
}

void Canvas_Delegate_Show_CP::reset_selected_stuff()
{
    //is_circle_containing_all_ = false;
    common_reset_selected_stuff();
    return;
}

bool Canvas_Delegate_Show_CP::is_ready()
{
    return ((*mode_flag_==DRAW_GRAPH && *draw_graph_flag_==DRAW_GRAPH_DONE) ||
            (*mode_flag_==DRAW_CURVE && *draw_curve_flag_==DRAW_CURVE_DONE));
}

void Canvas_Delegate_Show_CP::set_circle_containing_all()
{
    double r;
    unsigned int i;
    for (i=1; i<circle_packing_->nb_vertices(); i++)
    {
        r = circle_packing_->get_content_by_index(i).get_radius();
        if (r>1 && norm(circle_packing_->get_affix_by_index(i))<r*r)
        {
            is_circle_containing_all_ = true;
            index_circle_containing_all_ = i;
            return;
        }
    }

    is_circle_containing_all_ = false;
    index_circle_containing_all_ = 0;
    return;
}


void Canvas_Delegate_Show_CP::paint_event()
{
    //std::cout << "Entering Canvas_Delegate_Show_CP::paint_event()" << std::endl;

    if (is_ready())
    {
        draw_circle_packing(*circle_packing_, is_circle_containing_all_, index_circle_containing_all_);

        if (is_vertex_highlighted_)
        {
            mark_circle(circle_packing_->get_content_by_index(index_vertex_highlighted_), QColor("red"));
        }
    }


    //std::cout << "Leaving Canvas_Delegate_Show_CP::paint_event()" << std::endl;
    return;
}

void Canvas_Delegate_Show_CP::mouse_press_event()
{
    if (is_ready())
    {
        is_vertex_highlighted_ = circle_packing_->index_circle_containing(index_vertex_highlighted_, point_clicked_);
        window_->signal_received(VERTEX_HIGHLIGHTED_RIGHT);
    }

    return;
}

void Canvas_Delegate_Show_CP::ctrl_pressed()
{
    if (click_on_)
    {
        mobius_ = Mobius(-point_under_mouse_.get_affix())*Mobius(point_clicked_.get_affix())*mobius_;
        point_clicked_ = point_under_mouse_;
    }
    return;
}

void Canvas_Delegate_Show_CP::ctrl_released()
{
    if (click_on_)
    {
        complex_number z_0 = point_clicked_.get_affix();
        complex_number z_1 = point_under_mouse_.get_affix();
        if (abs(z_0)!=0 && abs(z_1)!=0)
        {
            complex_number u_0 = z_0/abs(z_0);
            complex_number u_1 = z_1/abs(z_1);
            complex_number u = u_1*conj(u_0);

            mobius_ = Mobius(u, 0.0, 0.0, 1.0)*mobius_;
            point_clicked_ = point_under_mouse_;
        }
    }
    return;
}

void Canvas_Delegate_Show_CP::mouse_release_event()
{
    if (is_ready())
    {
        is_vertex_highlighted_ = circle_packing_->index_circle_containing(index_vertex_highlighted_, point_released_);
        window_->signal_received(VERTEX_HIGHLIGHTED_RIGHT);

        if (QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            complex_number z_0 = point_clicked_.get_affix();
            complex_number z_1 = point_released_.get_affix();
            if (abs(z_0)!=0 && abs(z_1)!=0)
            {
                complex_number u_0 = z_0/abs(z_0);
                complex_number u_1 = z_1/abs(z_1);
                complex_number u = u_1*conj(u_0);

                mobius_ = Mobius(u, 0.0, 0.0, 1.0)*mobius_;
            }
        }
        else
        {
            mobius_ = Mobius(-point_released_.get_affix())*Mobius(point_clicked_.get_affix())*mobius_;
        }
    }
    return;
}

void Canvas_Delegate_Show_CP::mouse_move_event()
{
    if (is_ready())
    {
        if (!click_on_)
        {
            is_vertex_highlighted_ = circle_packing_->index_circle_containing(index_vertex_highlighted_, point_under_mouse_);
            window_->signal_received(VERTEX_HIGHLIGHTED_RIGHT);
        }

        if (click_on_)
        {
            if (QApplication::keyboardModifiers() == Qt::ControlModifier)
            {
                complex_number z_0 = point_clicked_.get_affix();
                complex_number z_1 = point_under_mouse_.get_affix();
                if (abs(z_0)!=0 && abs(z_1)!=0)
                {
                    *circle_packing_ = *circle_packing_save_;
                    complex_number u_0 = z_0/abs(z_0);
                    complex_number u_1 = z_1/abs(z_1);
                    complex_number u = u_1*conj(u_0);

                    circle_packing_->apply_mobius(Mobius(u, 0.0, 0.0, 1.0)*mobius_);
                    set_circle_containing_all();
                }
            }
            else
            {
                *circle_packing_ = *circle_packing_save_;
                circle_packing_->apply_mobius(Mobius(-point_under_mouse_.get_affix())*Mobius(point_clicked_.get_affix())*mobius_);
                set_circle_containing_all();
            }
        }

        if (is_vertex_highlighted_)
        {
            QString message = QString("Vertex n%1 %2")
                    .arg(QChar(0x00B0))
                    .arg(index_vertex_highlighted_+1);
            *message_ = message;
            window_->signal_received(MESSAGE);
        }
        else
        {
            window_->signal_received(MESSAGE, -1);
        }
    }

    return;
}

void Canvas_Delegate_Show_CP::zoom(const double &coeff, integer_coordinates point_coordinates_in_pixels, bool zoom_on_mouse)
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int x = point_coordinates_in_pixels.first;
    int y = point_coordinates_in_pixels.second;
    double X = real(point_under_mouse_.get_affix());
    double Y = imag(point_under_mouse_.get_affix());
    scale_ *= coeff;
    zoom_ *= coeff;
    if(zoom_on_mouse)
    {
        origin_x_in_pixels_ = int_round(x- scale_*X);
        origin_y_in_pixels_ = int_round(y+ scale_*Y);
    }
    else
    {
        origin_x_in_pixels_ = size_in_pixels_/2 -coeff*(size_in_pixels_/2-origin_x_in_pixels_);
        origin_y_in_pixels_ = size_in_pixels_/2 -coeff*(size_in_pixels_/2-origin_y_in_pixels_);
    }
    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;

    if(!zoom_on_mouse)
    {
        set_point_under_mouse(last);
        mouse_move_event();
    }
    return;
}

void Canvas_Delegate_Show_CP::shift_event()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    complex_number delta = scale_*(point_under_mouse_.get_affix() - point_clicked_.get_affix());
    double x_shift = real(delta);
    double y_shift = imag(delta);

    origin_x_in_pixels_ = origin_x_in_pixels_ + x_shift;
    origin_y_in_pixels_ = origin_y_in_pixels_ - y_shift;


    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}
void Canvas_Delegate_Show_CP::shift_left()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int x_shift = 20;


    origin_x_in_pixels_ = origin_x_in_pixels_ + x_shift;

    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}

void Canvas_Delegate_Show_CP::shift_right()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int x_shift = -20;

    origin_x_in_pixels_ = origin_x_in_pixels_ + x_shift;

    shift_x_ = (origin_x_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();

    return;
}

void Canvas_Delegate_Show_CP::shift_up()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int y_shift = -20;

    origin_y_in_pixels_ = origin_y_in_pixels_ - y_shift;

    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}

void Canvas_Delegate_Show_CP::shift_down()
{
    integer_coordinates last = complex_to_pixels_coordinates(point_under_mouse_.get_affix());
    int y_shift = 20;


    origin_y_in_pixels_ = origin_y_in_pixels_ - y_shift;
    shift_y_ = (origin_y_in_pixels_ - size_in_pixels_/2.0)/scale_;
    set_point_under_mouse(last);
    mouse_move_event();
    return;
}
