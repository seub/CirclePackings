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

#include "point.hpp"

std::ostream & operator<<(std::ostream &out, const Point &P)
{
    out << "point of coordinates " << P.affix_;
    return out;
}

Point::Point(const complex_number &affix)
{
    affix_ = affix;

    return;
}

const complex_number & Point::get_affix() const
{
    return affix_;
}

Point Point::projection_on_segment(const Point &first_endpoint, const Point &second_endpoint) const
{
    complex_number z = affix_, z_g = first_endpoint.affix_, z_d = second_endpoint.affix_;

    double x = real(z), x_g = real(z_g), x_d = real(z_d);
    double y = imag(z), y_g = imag(z_g), y_d = imag(z_d);

    double lambda = ((x-x_g)*(x_d-x_g) + (y-y_g)*(y_d-y_g))/norm(z_d - z_g);

    if (lambda > 0 && lambda < 1)
    {
        return Point(z_g + lambda*(z_d-z_g));
    }
    else if (norm(z-z_g) <= norm(z-z_d))
    {
        return Point(z_g);
    }
    else
    {
        return Point(z_d);
    }
}

bool are_intersecting(const segment &S_1, const segment &S_2)
{
    complex_number A_1 = S_1.first.get_affix();
    complex_number B_1 = S_1.second.get_affix();
    complex_number A_2 = S_2.first.get_affix();
    complex_number B_2 = S_2.second.get_affix();

    double det_1 = imag(conj(B_1 - A_1)*(B_2 - A_2));
    double det_2 = imag(conj(A_2 - A_1)*(B_2 - A_2));
    double det_3 = imag(conj(A_2 - A_1)*(B_1 - A_1));

    double lambda, mu;

    if (det_1==0) // Segments are parallel
    {
        if (det_2==0) // The four points are aligned (pain)
        {
            //std::cout << "WARNING (in are_intersecting): the four points are aligned" << std::endl;
            if (A_1==B_1)
            {
                std::cout << "WARNING (in are_intersecting): the first segment is collapsed to a point" << std::endl;
                if (A_2==B_2)
                {
                    std::cout << "WARNING (in are_intersecting): both segments are collapsed to a point" << std::endl;
                    return A_1==A_2;
                }
                complex_number nu = (A_1 - A_2)/(B_2 - A_2);
                if (imag(nu)==0)
                {
                    return (real(nu)>=0 && real(nu)<=1);
                }
                else
                {
                    return false;
                }
            }
            else
            {
                lambda = real((A_2 - A_1)/(B_1 - A_1));
                mu = real((B_2 - A_1)/(B_1 - A_1));
                return ((lambda>=0 && lambda<=1) || (mu>=0 && mu<=1) || (lambda<0 && mu>1) || (mu<0 && lambda>1));
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        lambda = det_2/det_1;
        mu = det_3/det_1;
        return (lambda>=0 && lambda<=1 && mu>=0 && mu<=1);
    }
}
