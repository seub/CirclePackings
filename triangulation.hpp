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

#ifndef TRIANGULATION_HPP
#define TRIANGULATION_HPP

#include "basic_types.hpp"
#include "triangle.hpp"

#include <QColor>

class Triangulation
{
    friend class Graph_Triangulater;

public:
    Triangulation();

    void reset_triangulation();

    bool get_triangle(const unsigned int &index, Triangle &output) const;
    Triangle get_triangle(const unsigned int &index) const;
    bool get_draw_borders() const;
    void set_draw_borders(bool b);
    QColor get_color(const unsigned int &index) const;
    std::vector<QColor> get_colors() const;
    unsigned int nb_colors() const;
    void set_colors(std::vector<QColor> &colors);
    unsigned int nb_triangles() const;
    unsigned int nb_points() const;
    integer_coordinates get_tiling_coordinates(const unsigned int &index) const;
    void cyclic_sort_triangles();
    void cyclic_sort_triangle(const unsigned int &index);

    void add_point(const Point &new_point, const integer_coordinates &tiling_coordinates);
    void add_triangle(const unsigned int &point_index_1, const unsigned int &point_index_2, const unsigned int &point_index_3);

    void color(const Triangulation_coloring &coloring);
    bool triangle_containing(unsigned int &output_index, double &alpha, double &beta, double &gamma, const Point &point) const;
    void nearest_exterior_triangle(unsigned int &output_index, double &alpha, double &beta, double &gamma, const Point &point) const;

    bool are_adjacent_triangles(const unsigned int &index_1, const unsigned int &index_2) const;

private:
    void extrema_tiling(int &x_min, int &x_max, int &y_min, int &y_max);
    void color_plain();
    void color_stripes_1();
    void color_stripes_2();
    void color_stripes_3();
    void color_triangles();
    void color_squares();
    void color_squares_2();
    void color_shading();
    void color_null();
    std::vector<unsigned int> compute_erdos() const;
    void color_erdos();

    std::vector<Point> points_;
    std::vector< std::vector<unsigned int> > triangles_;
    std::vector<integer_coordinates> tiling_coordinates_;
    std::vector<QColor> colors_;
    unsigned int nb_interior_points_;
    unsigned int nb_interior_triangles_;
};

#endif // TRIANGULATION_HPP
