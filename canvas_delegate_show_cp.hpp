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

#ifndef CANVAS_DELEGATE_SHOW_CP_HPP
#define CANVAS_DELEGATE_SHOW_CP_HPP

#include "basic_types.hpp"
#include "point.hpp"
#include "graph.hpp"
#include "mobius.hpp"
#include "canvas_delegate.hpp"

class QComboBox;

class Canvas_Delegate_Show_CP : public Canvas_Delegate
{
public:
    Canvas_Delegate_Show_CP(const int &size_in_pixels, const mode_type * const mode_flag,
                            const draw_graph_mode_type * const draw_graph_flag,
                            const draw_curve_mode_type * const draw_curve_flag, Graph<Circle> * const circle_packing_save,
                            Graph<Circle> * const circle_packing, Window *window, bool reset_contents = true);

    void reset_canvas_delegate(bool reset_contents = true);
    void reset_selected_stuff();

    void paint_event();
    void mouse_press_event();
    void mouse_release_event();
    void mouse_move_event();
    void ctrl_released();
    void ctrl_pressed();
    void zoom(const double &delta, integer_coordinates point_coordinates_in_pixels = integer_coordinates(0,0), bool zoom_on_mouse = false);
    void shift_event();
    void shift_left();
    void shift_right();
    void shift_up();
    void shift_down();

private:
    bool is_ready();
    void set_circle_containing_all();

    unsigned int index_circle_containing_all_;
    bool is_circle_containing_all_;

    const mode_type * const mode_flag_;
    const draw_graph_mode_type * const draw_graph_flag_;
    const draw_curve_mode_type * const draw_curve_flag_;

    Mobius mobius_;
    Graph<Circle> * const circle_packing_save_;
    Graph<Circle> * const circle_packing_;
};

#endif // CANVAS_DELEGATE_SHOW_CP_HPP
