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

#include <QString>
#include <QComboBox>
#include <QPainter>

#include "canvas_delegate_draw_graph.hpp"
#include "graph_completer.hpp"
#include "window.hpp"

Canvas_Delegate_Draw_Graph::Canvas_Delegate_Draw_Graph(
        const int &size_in_pixels, draw_graph_mode_type *draw_graph_flag, Graph<Point> *input_planar_graph, Window *window,
        bool reset_contents) :
    draw_graph_flag_(draw_graph_flag), input_planar_graph_(input_planar_graph)
{
    window_ = window;
    message_ = &(window_->message_received_);
    size_in_pixels_ = size_in_pixels;
    points_radius_ = 0.06;

    reset_canvas_delegate(reset_contents);
}

void Canvas_Delegate_Draw_Graph::set_points_radius(double radius)
{
    points_radius_ = radius;
    return;
}

void Canvas_Delegate_Draw_Graph::reset_canvas_delegate(bool reset_contents)
{
    common_reset();
    if (reset_contents)
    {
        points_radius_ = 0.06;
        input_planar_graph_->reset_graph();
    }
    is_vertex_selected_ = false;
    vertex_created_ = false;
    edge_added_ = false;
    vertex_selected_ = false;
    return;
}

void Canvas_Delegate_Draw_Graph::reset_selected_stuff()
{
    //std::cout << "Entering Canvas_Delegate_Draw_Graph::reset_selected_stuff" << std::endl;
    is_vertex_selected_ = false;
    vertex_created_ = false;
    edge_added_ = false;
    vertex_selected_ = false;
    common_reset_selected_stuff();

    return;
}

void Canvas_Delegate_Draw_Graph::set_vertex_selected(bool is_vertex_selected, const unsigned int index_vertex_selected)
{
    is_vertex_selected_ = is_vertex_selected;
    index_vertex_selected_ = index_vertex_selected;
    return;
}


void Canvas_Delegate_Draw_Graph::paint_event()
{
    draw_planar_graph(*input_planar_graph_, points_radius_);

    if (is_vertex_highlighted_)
    {
        mark_circle(Circle(input_planar_graph_->get_content_by_index(index_vertex_highlighted_), points_radius_), QColor("red"));
    }

    if (is_vertex_selected_)
    {
        mark_circle(Circle(input_planar_graph_->get_content_by_index(index_vertex_selected_), points_radius_), QColor("blue"));
    }

    return;
}

void Canvas_Delegate_Draw_Graph::mouse_press_event()
{
    unsigned int n=input_planar_graph_->nb_vertices();
    is_vertex_under_mouse_ = input_planar_graph_->index_point_containing(index_vertex_under_mouse_, point_clicked_, points_radius_);

    if (*draw_graph_flag_==DRAW_GRAPH_VERTICES)
    {
        if (!is_vertex_under_mouse_)
        {
            unsigned int index_vertex_too_close;
            bool is_vertex_too_close = input_planar_graph_->index_point_containing(
                        index_vertex_too_close, point_clicked_, 2*points_radius_);
            if (is_vertex_too_close)
            {
                QString qstring = QString("No vertex added (too close from vertex %1 %2)")
                        .arg(QChar(0x00B0))
                        .arg(index_vertex_too_close + 1);
                *message_ = qstring;
                window_->signal_received(MESSAGE);
            }
            else
            {
                input_planar_graph_->add_isolated_vertex(n+1, point_clicked_, QColor(rand()%256,rand()%256,rand()%256));
                vertex_created_ = true;
                QString qstring = QString("Vertex n%1 %2 added!")
                        .arg(QChar(0x00B0))
                        .arg(n+1);
                *message_ = qstring;
                window_->signal_received(MESSAGE);
                n++;
                if (n>=2)
                {
                    window_->signal_received(GRAPH_COMPLETABLE_ON);
                    window_->signal_received(DRAW_EDGES_ON);
                }
            }
        }
        else
        {
            input_planar_graph_->remove_vertex_by_index(index_vertex_under_mouse_);
            input_planar_graph_->rename_all_labels();
            QString qstring = QString("Vertex n%1 %2 deleted!")
                    .arg(QChar(0x00B0))
                    .arg(index_vertex_under_mouse_+1);
            *message_ = qstring;
            window_->signal_received(MESSAGE);
            n--;
            if (n<2)
            {
                window_->signal_received(GRAPH_COMPLETABLE_OFF);
                window_->signal_received(DRAW_EDGES_OFF);
            }
            is_vertex_selected_ = false;
            is_vertex_highlighted_ = false;
        }
    }

    else if (*draw_graph_flag_==DRAW_GRAPH_EDGES)
    {
        if (is_vertex_under_mouse_)
        {
            if (!is_vertex_selected_)
            {
                is_vertex_selected_ = true;
                vertex_selected_ = true;
                index_vertex_selected_ = index_vertex_under_mouse_;
                QString qstring = QString("Vertex n%1 %2 selected")
                        .arg(QChar(0x00B0))
                        .arg(index_vertex_selected_ + 1);
                *message_ = qstring;
                window_->signal_received(MESSAGE);
            }
            else if (index_vertex_under_mouse_==index_vertex_selected_)
            {
                is_vertex_selected_ = false;
            }
            else if (input_planar_graph_->are_neighbor_indices(index_vertex_under_mouse_, index_vertex_selected_))
            {
                input_planar_graph_->remove_edge_by_indices(index_vertex_under_mouse_, index_vertex_selected_);
                edge_added_ = true;
                QString qstring = QString("Edge %1 <-> %2 deleted!")
                        .arg(index_vertex_under_mouse_ + 1)
                        .arg(index_vertex_selected_ + 1);
                *message_ = qstring;
                window_->signal_received(MESSAGE);
                is_vertex_selected_ = false;
            }
            else
            {
                Graph_Completer GC;
                GC.set_graph(input_planar_graph_);
                segment S(input_planar_graph_->get_content_by_index(index_vertex_under_mouse_),
                          input_planar_graph_->get_content_by_index(index_vertex_selected_));
                if (GC.crosses_an_edge(S))
                {
                    QString qstring = QString("Illegal edge! (no intersections allowed)");
                    *message_ = qstring;
                    window_->signal_received(MESSAGE);
                }
                else
                {
                    input_planar_graph_->add_edge_by_indices(index_vertex_under_mouse_, index_vertex_selected_);
                    edge_added_ = true;
                    QString qstring = QString("Edge %1 <-> %2 added!")
                            .arg(index_vertex_under_mouse_ + 1)
                            .arg(index_vertex_selected_ + 1);
                    *message_ = qstring;
                    window_->signal_received(MESSAGE);
                }
                is_vertex_selected_ = false;
            }
        }
    }
    else if (*draw_graph_flag_==DRAW_GRAPH_DONE)
    {
        is_vertex_highlighted_ = is_vertex_under_mouse_;
        index_vertex_highlighted_ = index_vertex_under_mouse_;
        window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
    }
    return;
}

void Canvas_Delegate_Draw_Graph::mouse_release_event()
{
    is_vertex_highlighted_ = input_planar_graph_->index_point_containing(index_vertex_highlighted_, point_released_, points_radius_);
    if (*draw_graph_flag_==DRAW_GRAPH_DONE)
    {
        window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
    }
    return;
}

void Canvas_Delegate_Draw_Graph::mouse_move_event()
{
    is_vertex_under_mouse_ = input_planar_graph_->index_point_containing(index_vertex_under_mouse_, point_under_mouse_, points_radius_);
    if (!click_on_)
    {
        is_vertex_highlighted_ = is_vertex_under_mouse_;
        index_vertex_highlighted_ = index_vertex_under_mouse_;
    }
    if (*draw_graph_flag_==DRAW_GRAPH_DONE && is_vertex_highlighted_)
    {
        window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
    }

    QString message;
    if (is_vertex_under_mouse_)
    {
        if (*draw_graph_flag_ == DRAW_GRAPH_VERTICES)
        {
            if (!(vertex_created_ && index_vertex_under_mouse_ + 1 == input_planar_graph_->nb_vertices()))
            {
                message = QString("Vertex n%1 %2 (click to delete this vertex)")
                        .arg(QChar(0x00B0))
                        .arg(index_vertex_under_mouse_+1);

                *message_ = message;
                window_->signal_received(MESSAGE);
                vertex_created_ = false;
            }
        }
        else if (*draw_graph_flag_ == DRAW_GRAPH_EDGES)
        {
            if (is_vertex_selected_)
            {
                if (index_vertex_selected_ != index_vertex_under_mouse_)
                {
                    message = QString("Vertex n%1 %2 (click to add/delete an edge)")
                            .arg(QChar(0x00B0))
                            .arg(index_vertex_under_mouse_+1);

                    *message_ = message;
                    window_->signal_received(MESSAGE);
                }
                else if (!vertex_selected_)
                {
                    message = QString("Vertex n%1 %2 (click to unselect this vertex)")
                            .arg(QChar(0x00B0))
                            .arg(index_vertex_under_mouse_+1);

                    *message_ = message;
                    window_->signal_received(MESSAGE);
                    vertex_selected_ = false;
                }
            }
            else
            {
                if (!edge_added_)
                {
                    message = QString("Vertex n%1 %2 (click to select this vertex)")
                            .arg(QChar(0x00B0))
                            .arg(index_vertex_under_mouse_+1);

                    *message_ = message;
                    window_->signal_received(MESSAGE);
                    edge_added_ = false;
                }
            }
        }
        else
        {
            message = QString("Vertex n%1 %2")
                    .arg(QChar(0x00B0))
                    .arg(index_vertex_under_mouse_+1);

            *message_ = message;
            window_->signal_received(MESSAGE);
        }
    }
    else
    {
        vertex_created_ = false;
        edge_added_ = false;
        vertex_selected_ = false;
    }

    if (!is_vertex_under_mouse_)
    {
        window_->signal_received(MESSAGE, -1);
    }

    return;
}
