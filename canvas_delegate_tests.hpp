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

#ifndef CANVAS_DELEGATE_TESTS_HPP
#define CANVAS_DELEGATE_TESTS_HPP

#include "basic_types.hpp"
#include "canvas_delegate.hpp"
#include "curve.hpp"

class Canvas_Delegate_Tests : public Canvas_Delegate
{
public:
    Canvas_Delegate_Tests(const unsigned int &size_in_pixels, const std::vector<double> &deltas,
                          const std::vector<double> &overall_scores);
    void reset_canvas_delegate();
    void reset_selected_stuff();

    void paint_event();
    void mouse_press_event() {}
    void mouse_release_event() {}
    void mouse_move_event();
    void set_curve_point_under_mouse(const Point &point);


private:
    void create_curve();

    double points_radius_;
    std::vector<double> deltas_;
    std::vector<double> overall_scores_;
    std::vector<Point> curve_points_;

    bool is_curve_point_under_mouse_;
    unsigned int curve_point_under_mouse_;
};

#endif // CANVAS_DELEGATE_TESTS_HPP
