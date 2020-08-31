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

#include "canvas_delegate_tests.hpp"
#include "circle.hpp"


#include <QPainter>

Canvas_Delegate_Tests::Canvas_Delegate_Tests(const unsigned int &size_in_pixels,
                                             const std::vector<double> &deltas, const std::vector<double> &overall_scores) :
    deltas_(deltas), overall_scores_(overall_scores)
{
    size_in_pixels_ = size_in_pixels;
    points_radius_ = 0.01;
    create_curve();

    reset_canvas_delegate();
}

void Canvas_Delegate_Tests::reset_canvas_delegate()
{

    common_reset();
    is_curve_point_under_mouse_ = false;
    curve_point_under_mouse_ = -1;

    return;
}

void Canvas_Delegate_Tests::reset_selected_stuff()
{
    is_curve_point_under_mouse_ = false;
    common_reset_selected_stuff();
    return;
}

void Canvas_Delegate_Tests::create_curve()
{
    std::vector<double> abscissa;
    std::vector<double> ordinates;

    double delta_min = deltas_.front();
    double delta_max = deltas_.back();
    unsigned int N = deltas_.size();

    double a = 2.5/(delta_max - delta_min);
    double b = -1.0 - a*delta_min;

    unsigned int i;
    for(i=0; i<N; i++)
    {
        abscissa.push_back(a*deltas_[i]+b);
    }

    double score_min = *min_element(overall_scores_.begin(), overall_scores_.end());
    double score_max = *max_element(overall_scores_.begin(), overall_scores_.end());

    a = 2.5/(score_max - score_min);
    b = -1.0 - a*score_min;
    for(i=0; i<N; i++)
    {
        ordinates.push_back(a*overall_scores_[i]+b);
    }

    curve_points_.clear();
    for(i=0; i<N; i++)
    {
        curve_points_.push_back(Point(complex_number(abscissa[i], ordinates[i])));
    }

    return;
}

void Canvas_Delegate_Tests::set_curve_point_under_mouse(const Point &point)
{
    unsigned int i;
    for (i=0; i<curve_points_.size(); i++)
    {
        if (abs(curve_points_[i].get_affix() - point.get_affix()) < points_radius_)
        {
            is_curve_point_under_mouse_ = true;
            curve_point_under_mouse_ = i;
            return;
        }
    }
    is_curve_point_under_mouse_ = false;
    return;
}

void Canvas_Delegate_Tests::paint_event()
{
    draw_segment(segment(Point(complex_number(-1.25, -1.5)), Point(complex_number(-1.25, 1.5))));
    draw_segment(segment(Point(complex_number(-1.5, -1.25)), Point(complex_number(1.5, -1.25))));

    draw_incomplete_curve(curve_points_, QColor("blue"));
    draw_nodes(curve_points_, points_radius_, QColor("green"));

    if (is_curve_point_under_mouse_)
    {
        mark_circle(Circle(curve_points_[curve_point_under_mouse_], points_radius_), QColor("red"));
    }
    return;
}

void Canvas_Delegate_Tests::mouse_move_event()
{
    unsigned int old_curve_point_under_mouse = curve_point_under_mouse_;
    set_curve_point_under_mouse(point_under_mouse_);

    if (is_curve_point_under_mouse_ && curve_point_under_mouse_ != old_curve_point_under_mouse)
    {
        std::cout << "delta = " << deltas_[curve_point_under_mouse_] << ", score = "
                  << overall_scores_[curve_point_under_mouse_] << std::endl;
    }
    return;
}
