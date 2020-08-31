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

#include "canvas_delegate_draw_curve.hpp"
#include "window.hpp"

#include <QString>
#include <QComboBox>
#include <QPainter>

Canvas_Delegate_Draw_Curve::Canvas_Delegate_Draw_Curve(
        const unsigned int &size_in_pixels, const draw_curve_mode_type *const draw_curve_flag, const curve_choice_type * const curve_flag,
        Curve * const curve, Graph<Circle> * const tiling, std::vector<Point> * const nodes,
        std::vector<complex_number> *const tangents, Graph<Circle> *const circle_packing, Window *window, bool reset_contents) :
    draw_curve_flag_(draw_curve_flag), curve_choice_flag_(curve_flag), tiling_(tiling), curve_(curve),
    nodes_(nodes), tangent_factors_(tangents), circle_packing_(circle_packing)
{
    window_ = window;
    message_ = &(window_->message_received_);
    size_in_pixels_ = size_in_pixels;
    common_reset();
    smooth_ = false;
    nodes_radius_in_pixels_ = 6;

    reset_canvas_delegate(reset_contents);
}


void Canvas_Delegate_Draw_Curve::reset_canvas_delegate(bool reset_contents)
{
    common_reset();

    if (reset_contents)
    {
        curve_->reset_curve();
        nodes_->clear();
        nodes_->reserve(20);
        tangent_factors_->clear();
        circle_packing_->reset_graph();
    }

    smooth_ = false;
    is_node_point_selected_ = false;
    is_node_point_under_mouse_ = false;
    node_point_created_ = false;
    is_tangent_control_point_selected_ = false;
    is_tangent_control_point_under_mouse_ = false;
    return;
}

void Canvas_Delegate_Draw_Curve::reset_selected_stuff()
{
    is_node_point_under_mouse_ = false;
    is_node_point_selected_ = false;
    is_tangent_control_point_under_mouse_ = false;
    is_tangent_control_point_selected_ = false;
    node_point_created_ = false;

    common_reset_selected_stuff();
    return;
}

void Canvas_Delegate_Draw_Curve::set_node_point_under_mouse(Point const &point)
{
    unsigned int i;
    for (i=0; i<nodes_->size(); i++)
    {
        if (abs((*nodes_)[i].get_affix() - point.get_affix()) < nodes_radius_in_pixels_/scale_)
        {
            is_node_point_under_mouse_ = true;
            node_point_under_mouse_ = i;
            return;
        }
    }
    is_node_point_under_mouse_ = false;
    return;
}

void Canvas_Delegate_Draw_Curve::set_tangent_control_point_under_mouse(Point const &point)
{
    complex_number T;
    unsigned int i, N = nodes_->size();
    if (tangent_factors_->size() != nodes_->size())
    {
        std::cout << "ERROR in Canvas_Delegate_Draw_Curve::set_tangent_control_point_under_mouse: bad size" << std::endl;
        throw(QString("ERROR in Canvas_Delegate_Draw_Curve::set_tangent_control_point_under_mouse: bad size"));
    }
    else if (!beziers_mode())
    {
        std::cout << "ERROR in Canvas_Delegate_Draw_Curve::set_tangent_control_point_under_mouse: never supposed to reach here"
                  << std::endl;
        throw(QString("ERROR in Canvas_Delegate_Draw_Curve::set_tangent_control_point_under_mouse: never supposed to reach here"));
    }

    for (i=0; i<N; i++)
    {
        T = (*nodes_)[i].get_affix() + (*tangent_factors_)[i] *
                ((*nodes_)[(i+1)%N].get_affix() - (*nodes_)[(N+i-1)%N].get_affix());



        if (abs(T - point.get_affix()) < 0.8*nodes_radius_in_pixels_/scale_)
        {
            is_tangent_control_point_under_mouse_ = true;
            tangent_control_point_under_mouse_ = i;
            return;
        }
    }
    is_tangent_control_point_under_mouse_ = false;
    return;
}

void Canvas_Delegate_Draw_Curve::set_smooth(bool smooth)
{
    smooth_ = smooth;
    if (!smooth)
    {
        is_tangent_control_point_under_mouse_ = false;
        is_tangent_control_point_selected_ = false;
    }
    return;
}

bool Canvas_Delegate_Draw_Curve::beziers_mode()
{
    return *draw_curve_flag_ != DRAW_CURVE_NODES  && *draw_curve_flag_ != DRAW_CURVE_DONE
            && smooth_ && *curve_choice_flag_ == CUSTOM;
}

void Canvas_Delegate_Draw_Curve::set_nb_sides(const unsigned int &nb_sides)
{
    nb_sides_ = nb_sides;
    return;
}

void Canvas_Delegate_Draw_Curve::paint_event()
{
    //std::cout << "Entering Canvas_Delegate_Draw_Curve::paint_event" << std::endl;

    draw_tiling(*tiling_);
    if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
    {
        draw_tiling_cp(*circle_packing_);
        if (is_vertex_highlighted_ && (!is_node_point_under_mouse_) && (!is_node_point_selected_)
                && (!is_tangent_control_point_under_mouse_) && !(is_tangent_control_point_selected_))
        {
            mark_circle(circle_packing_->get_content_by_index(index_vertex_highlighted_), QColor("red"));
        }
    }
    draw_nodes(*nodes_, nodes_radius_in_pixels_/scale_);

    if (*draw_curve_flag_==DRAW_CURVE_NODES && nodes_->size()>0)
    {
        mark_circle(Circle((*nodes_)[0], nodes_radius_in_pixels_/scale_), QColor("red"));
        draw_incomplete_curve(*nodes_);
    }
    else
    {
        draw_curve(*curve_);
    }

    if (beziers_mode())
    {
        draw_tangents(*nodes_, *tangent_factors_, 0.8*nodes_radius_in_pixels_/scale_);
    }

    if (is_node_point_under_mouse_)
    {
        if(node_point_under_mouse_<nodes_->size())
        {
            mark_circle(Circle((*nodes_)[node_point_under_mouse_], nodes_radius_in_pixels_/scale_), QColor("green"));
        }
        else
        {
            std::cout << "ERROR in Canvas_Delegate_Draw_Curve::paint_event: node index problem" << std::endl;
            throw(QString("ERROR in Canvas_Delegate_Draw_Curve::paint_event: node index problem"));
        }
    }

    if (is_tangent_control_point_under_mouse_ && !is_node_point_under_mouse_ && beziers_mode())
    {
        if(tangent_control_point_under_mouse_<nodes_->size() && tangent_factors_->size() == nodes_->size())
        {
            unsigned int i = tangent_control_point_under_mouse_, N = nodes_->size();
            Point T((*nodes_)[i].get_affix() +
                    (*tangent_factors_)[i] * ((*nodes_)[(i+1)%N].get_affix() - (*nodes_)[(N+i-1)%N].get_affix()));
            mark_circle(Circle(T, 0.8*nodes_radius_in_pixels_/scale_), QColor("green"));
        }
        else
        {
            std::cout << "ERROR in Canvas_Delegate_Draw_Curve::paint_event: tangent index problem" << std::endl;
            throw(QString("ERROR in Canvas_Delegate_Draw_Curve::paint_event: tangent index problem"));
        }
    }

    //std::cout << "Leaving Canvas_Delegate_Draw_Curve::paint_event" << std::endl;
    return;
}

void Canvas_Delegate_Draw_Curve::mouse_press_event()
{
    set_node_point_under_mouse(point_clicked_);
    if (beziers_mode())
    {
        set_tangent_control_point_under_mouse(point_clicked_);
    }

    if (is_node_point_under_mouse_)
    {
        is_node_point_selected_=true;
        node_point_selected_ = node_point_under_mouse_;
        if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
        {
            window_->signal_received(CURVE_READY_OFF);
        }
    }

    else if (beziers_mode() && is_tangent_control_point_under_mouse_)
    {
        is_tangent_control_point_selected_=true;
        tangent_control_point_selected_ = tangent_control_point_under_mouse_;
        if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
        {
            window_->signal_received(CURVE_READY_OFF);
        }
    }

    else if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
    {
        is_vertex_under_mouse_ = circle_packing_->index_circle_containing(index_vertex_under_mouse_, point_clicked_);
        set_vertex_highlighted(is_vertex_under_mouse_, index_vertex_under_mouse_);

        if (*draw_curve_flag_==DRAW_CURVE_DONE)
        {
            window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
        }
    }

    if (*draw_curve_flag_==DRAW_CURVE_NODES)
    {
        if (is_node_point_selected_)
        {
            if (node_point_selected_==0 && nodes_->size()>2)
            {
                curve_->create_custom(*nodes_, *tangent_factors_, false);
                window_->signal_received(CURVE_CREATED);
            }
        }
        else
        {
            nodes_->push_back(point_clicked_);
            node_point_created_ = true;
            QString qstring = QString("Node added!");
            *message_ = qstring;
            window_->signal_received(MESSAGE);
        }
    }
    return;
}

void Canvas_Delegate_Draw_Curve::mouse_release_event()
{

    set_node_point_under_mouse(point_released_);
    if (beziers_mode())
    {
        set_tangent_control_point_under_mouse(point_released_);
    }

    if ((is_node_point_selected_ || is_tangent_control_point_selected_)
            && (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY
                || *draw_curve_flag_==DRAW_CURVE_DONE))
    {
        curve_->create_curve(*nodes_, *tangent_factors_, *curve_choice_flag_, smooth_, nb_sides_, node_point_selected_);
        window_->signal_received(CURVE_READY_ON);
    }

    is_node_point_selected_ = false;
    is_tangent_control_point_selected_ = false;
    return;
}

void Canvas_Delegate_Draw_Curve::mouse_move_event()
{
    if (!click_on_)
    {
        set_node_point_under_mouse(point_under_mouse_);
    }

    if (beziers_mode() && !click_on_)
    {
        set_tangent_control_point_under_mouse(point_under_mouse_);
    }

    if ((!is_node_point_selected_) && (!is_tangent_control_point_selected_) &&
            (*draw_curve_flag_==DRAW_CURVE_INVALID
             || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
            && !click_on_)
    {
        is_vertex_under_mouse_ = circle_packing_->index_circle_containing(index_vertex_under_mouse_, point_under_mouse_);
        set_vertex_highlighted(is_vertex_under_mouse_, index_vertex_under_mouse_);
        if (*draw_curve_flag_==DRAW_CURVE_DONE && is_vertex_highlighted_)
        {
            window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
        }
    }

    if (click_on_ && is_node_point_selected_)
    {
        (*nodes_)[node_point_selected_] = point_under_mouse_;
        curve_->create_curve(*nodes_, *tangent_factors_, *curve_choice_flag_, smooth_, nb_sides_, node_point_selected_);
    }
    else if (click_on_ && is_tangent_control_point_selected_
             && beziers_mode())
    {
        unsigned int i = tangent_control_point_selected_, N = nodes_->size();
        complex_number actual_tangent = point_under_mouse_.get_affix() - (*nodes_)[i].get_affix();
        complex_number default_tangent = (*nodes_)[(i+1)%N].get_affix() - (*nodes_)[(N+i-1)%N].get_affix();
        if (actual_tangent != complex_number(0.0, 0.0))
        {
            (*tangent_factors_)[i] = (1.0/default_tangent)*actual_tangent;
        }
        curve_->create_curve(*nodes_, *tangent_factors_, *curve_choice_flag_, smooth_, nb_sides_, node_point_selected_);
    }

    QString message;
    if (is_node_point_under_mouse_)
    {
        if (node_point_under_mouse_ == 0 && *draw_curve_flag_ == DRAW_CURVE_NODES && nodes_->size()>2)
        {
            message = QString("Click on this node to close the curve");

            *message_ = message;
            window_->signal_received(MESSAGE);
        }
        else if (!(*draw_curve_flag_ == DRAW_CURVE_NODES && node_point_under_mouse_+1 == nodes_->size()
                   && node_point_created_))
        {
            message = QString("Click and drag to move this node");

            *message_ = message;
            window_->signal_received(MESSAGE);
        }
    }
    else
    {
        node_point_created_ = false;
        if (is_tangent_control_point_under_mouse_ && beziers_mode())
        {
            message = QString("Click and drag to move this control point");

            *message_ = message;
            window_->signal_received(MESSAGE);
        }
        else if (is_vertex_highlighted_)
        {
            message = QString("Vertex n%1 %2")
                    .arg(QChar(0x00B0))
                    .arg(index_vertex_highlighted_ + 1);
            *message_ = message;
            window_->signal_received(MESSAGE);
        }
    }

    if ((!is_node_point_under_mouse_) && (!is_vertex_under_mouse_) &&
            !(is_tangent_control_point_under_mouse_ && beziers_mode()))
    {
        window_->signal_received(MESSAGE, -1);
    }
    return;
}
