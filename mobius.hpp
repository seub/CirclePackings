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

#ifndef MOBIUS_HPP
#define MOBIUS_HPP

#include "basic_types.hpp"
#include "point.hpp"
#include "circle.hpp"

class Mobius
{
    friend Mobius operator*(const Mobius &M_1, const Mobius&M_2);
    friend Point operator*(const Mobius &M, const Point &P);
    friend Circle operator*(const Mobius &M, const Circle &C);

public:
    Mobius();
    Mobius(const complex_number &a, const complex_number &b, const complex_number &c, const complex_number &d);
    Mobius(const complex_number &a);

private:
    complex_number a_, b_, c_, d_;
};






#endif // MOBIUS_HPP
