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

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "basic_types.hpp"
#include "point.hpp"

class Circle
{
    friend std::ostream & operator<<(std::ostream &out, const Circle &C);

public:
    Circle() {}
    Circle(const Point &centre, const double &radius);

    const Point & get_centre() const;
    const double & get_radius() const;
    const complex_number & get_affix() const;

private:
    Point centre_;
    double radius_;
};



#endif // CIRCLE_HPP
