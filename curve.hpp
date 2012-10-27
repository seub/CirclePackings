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

#ifndef CURVE_HPP
#define CURVE_HPP

#include "basic_types.hpp"
#include "point.hpp"


class Curve
{
    friend std::ostream & operator<<(std::ostream &out, const Curve &C);

public:
    Curve() {}

    void reset_curve();
    const Point & get_point(const unsigned int &index) const;
    const segment get_segment (const unsigned int &index) const;
    unsigned int nb_points() const;
    bool is_inside(const Point &point) const;

    void create_curve(std::vector<Point> &nodes, std::vector<complex_number> &tangents,
                      const curve_choice_type &curve_flag, bool smooth, const int &nb_sides, const unsigned int &node_index);
    void create_custom(std::vector<Point> &nodes, std::vector<complex_number> tangent_factors, bool smooth = false);
    void create_polygon(std::vector<Point> &nodes, const int &nb_sides);
    void create_rectangle(std::vector<Point> &nodes);
    void create_star(std::vector<Point> &nodes, const int &index_node_selected, const int &nb_points);
    void create_ellipse(std::vector<Point> &nodes, const int &nb_points);
    void create_cardioid(std::vector<Point> &nodes, const int &nb_points);
    void create_koch_segment(int depth, Point left_endpoint, Point right_endpoint);
    void create_koch_curve(std::vector<Point> &nodes, int depth);
    void create_epicycloid(std::vector<Point> &nodes, int nb_cusps, int nb_points);

    bool is_self_intersecting() const;
    void point_projection(Point &output, const Point &point) const;
    void get_extrema(double &x_min, double &x_max, double &y_min, double &y_max) const;
    Point center_of_mass() const;
    void get_max_dimensions(double &max_width, double &max_height) const;

private:    
    void add_point(const Point &point);
    void remove_point(const unsigned int& index);
    void replace_point(const unsigned int& index, const Point &new_point);
    void smooth_curve(std::vector<complex_number> &tangent_factors);

    std::vector<Point> points_;
};

#endif // CURVE_HPP
