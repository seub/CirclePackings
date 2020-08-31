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

#ifndef CANVAS_DELEGATE_DRAW_GRAPH_HPP
#define CANVAS_DELEGATE_DRAW_GRAPH_HPP

#include "basic_types.hpp"
#include "point.hpp"
#include "graph.hpp"
#include "canvas_delegate.hpp"

class QComboBox;

class Canvas_Delegate_Draw_Graph : public Canvas_Delegate
{
public:
    Canvas_Delegate_Draw_Graph(const int &size_in_pixels, draw_graph_mode_type *draw_graph_flag_, Graph<Point>* input_planar_graph,
                               Window *window, bool reset_contents = true);

    void reset_canvas_delegate(bool reset_contents = true);
    void reset_selected_stuff();

    void paint_event();
    void mouse_press_event();
    void mouse_release_event();
    void mouse_move_event();
    void set_points_radius(double radius);

private:
    void set_vertex_selected(bool is_vertex_selected, const unsigned int index_vertex_selected);

    const draw_graph_mode_type * const draw_graph_flag_;

    bool is_vertex_selected_;
    unsigned int index_vertex_selected_;

    bool vertex_created_;
    bool vertex_selected_;
    bool edge_added_;

    Graph<Point> * const input_planar_graph_;
    double points_radius_;

};

#endif // CANVAS_DELEGATE_DRAW_GRAPH_HPP
