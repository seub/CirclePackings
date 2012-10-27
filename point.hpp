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

#ifndef POINT_H
#define POINT_H

#include "basic_types.hpp"



class Point
{
    friend std::ostream & operator<<(std::ostream &out, const Point &P);
public:
    Point() {}
    Point(double x, double y) {affix_ = complex_number(x, y);}
    Point(const complex_number &affix);

    const complex_number & get_affix() const;
    Point projection_on_segment(const Point &first_endpoint, const Point &second_endpoint) const;

private:
    complex_number affix_;
};

typedef std::pair<Point, Point> segment;

bool are_intersecting(const segment &S_1, const segment &S_2);

#endif // POINT_H
