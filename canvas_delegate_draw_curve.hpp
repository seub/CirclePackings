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

#ifndef CANVAS_DELEGATE_DRAW_CURVE_HPP
#define CANVAS_DELEGATE_DRAW_CURVE_HPP

#include "point.hpp"
#include "curve.hpp"
#include "graph.hpp"
#include "canvas_delegate.hpp"

class QComboBox;

class Canvas_Delegate_Draw_Curve : public Canvas_Delegate
{
public:
    Canvas_Delegate_Draw_Curve(const unsigned int &size_in_pixels, const draw_curve_mode_type * const draw_curve_flag,
                               const curve_choice_type * const curve_flag, Curve * const curve, Graph<Circle> * const tiling,
                               std::vector<Point> * const nodes, std::vector<complex_number> * const tangents,
                               Graph<Circle> * const circle_packing,
                               Window* window, bool reset_contents = true);
    void reset_canvas_delegate(bool reset_contents = true); // Does not reset tiling
    void reset_selected_stuff();

    void paint_event();
    void mouse_press_event();
    void mouse_release_event();
    void mouse_move_event();

    void set_nb_sides(const unsigned int &nb_sides);
    void set_smooth(bool smooth);

    bool beziers_mode();

private:

    const draw_curve_mode_type * const draw_curve_flag_;
    const curve_choice_type * const curve_choice_flag_;
    bool smooth_;
    int nb_sides_;

    Graph<Circle> * const tiling_;

    Curve * const curve_;
    std::vector<Point> * const nodes_;
    std::vector<complex_number> * const tangent_factors_;
    int nodes_radius_in_pixels_;

    bool is_node_point_under_mouse_;
    unsigned int node_point_under_mouse_;
    bool is_node_point_selected_;
    unsigned int node_point_selected_;
    bool node_point_created_;

    bool is_tangent_control_point_under_mouse_;
    unsigned int tangent_control_point_under_mouse_;
    bool is_tangent_control_point_selected_;
    unsigned int tangent_control_point_selected_;

    Graph<Circle> * const circle_packing_;

    void set_node_point_under_mouse(Point const &point);
    void set_tangent_control_point_under_mouse(Point const &point);
};

#endif // CANVAS_DELEGATE_DRAW_CURVE_HPP
