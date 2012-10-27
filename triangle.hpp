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

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP


#include "basic_types.hpp"
#include "point.hpp"

class Triangle
{
public:
    Triangle();
    Triangle(const Point &A, const Point &B, const Point &C);

    Point get_A() const;
    Point get_B() const;
    Point get_C() const;
    void get_ABC(Point &A, Point &B, Point &C) const;

    bool barycentric_coordinates(double &alpha, double &beta, double &gamma, const Point &M) const;
    bool is_inside(double &alpha, double &beta, double &gamma, const Point &M) const; //alpha, beta, gamma are the output barycentric coordinates of M
    Point point_from_barycentric_coordinates(const double &alpha,const double &beta,const double &gamma) const;

    void get_ABC_by_decreasing_y(Point &A, Point &B, Point &C) const;


    void get_extrema(double &x_min, double &x_max, double &y_min, double &y_max) const;

private:
    Point A_;
    Point B_;
    Point C_;
};

#endif // TRIANGLE_HPP
