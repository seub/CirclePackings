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


#ifndef AFFINE_TRANSFORM_HPP
#define AFFINE_TRANSFORM_HPP


#include "basic_types.hpp"
#include "point.hpp"

class Triangle;

class Affine_Transform
{

public:
    Affine_Transform();
    Affine_Transform(double a_11, double a_12, double a_21, double a_22, double t_x, double t_y);

    bool inverse(Affine_Transform &output);
    bool match_triangles(const Triangle &T_1, const Triangle &T_2);

    void set_for_match_triangles_in_pixels(double *input_scale, int *input_origin_x_in_pixels, int *input_origin_y_in_pixels,
                                           double *output_scale, int *output_origin_x_in_pixels, int *output_origin_y_in_pixels);
    bool match_triangles_in_pixels(const Triangle &T_1, const Triangle &T_2);
    void match_coordinates_in_pixels();

    void compose(const Affine_Transform &f, const Affine_Transform &g);
    Point kick(const Point &M);
    integer_coordinates kick(const integer_coordinates& xy);

private:
    double a_11_, a_12_, a_21_, a_22_;
    double t_x_, t_y_;

    // The following are for match_triangles_in_pixels
    double *input_scale_;
    int *input_origin_x_in_pixels_;
    int *input_origin_y_in_pixels_;

    double *output_scale_;
    int *output_origin_x_in_pixels_;
    int *output_origin_y_in_pixels_;
};

#endif // AFFINE_TRANSFORM_HPP
