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

#include "mobius.hpp"

Mobius::Mobius()
{
    a_ = 1.0;
    b_ = 0.0;
    c_ = 0.0;
    d_ = 1.0;
}


Mobius::Mobius(const complex_number &a, const complex_number &b, const complex_number &c, const complex_number &d)
{
    a_ = a;
    b_ = b;
    c_ = c;
    d_ = d;
}

Mobius::Mobius(const complex_number &a)
{
    a_ = 1.0;
    b_ = -a;
    c_ = -conj(a);
    d_ = 1.0;
}

Mobius operator*(const Mobius &M_1, const Mobius&M_2)
{
    complex_number a = M_1.a_*M_2.a_ + M_1.b_*M_2.c_;
    complex_number b = M_1.a_*M_2.b_ + M_1.b_*M_2.d_;
    complex_number c = M_1.c_*M_2.a_ + M_1.d_*M_2.c_;
    complex_number d = M_1.c_*M_2.b_ + M_1.d_*M_2.d_;

    return Mobius(a,b,c,d);
}

Point operator*(const Mobius &M, const Point &P)
{
    complex_number z = P.get_affix();
    complex_number res_affix = (M.a_*z + M.b_)/(M.c_*z + M.d_);

    return Point(res_affix);
}

Circle operator*(const Mobius &M, const Circle &C)
{
    complex_number a = M.a_;
    complex_number b = M.b_;
    complex_number c = M.c_;
    complex_number d = M.d_;

    complex_number z = C.get_centre().get_affix();
    double r = C.get_radius();

    complex_number center_affix = ((a*z + b)*conj(c*z + d) - r*r*a*conj(c))/(norm(c*z + d) - r*r*norm(c));
    double radius = (r*std::abs(a*d - b*c))/std::abs(norm(c*z + d) - r*r*norm(c));

    return Circle(Point(center_affix), radius);
}
