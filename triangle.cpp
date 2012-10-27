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

#include "triangle.hpp"

Triangle::Triangle()
{
}

Triangle::Triangle(const Point &A, const Point &B, const Point &C) : A_(A), B_(B), C_(C)
{
}

Point Triangle::get_A() const
{
    return A_;
}

Point Triangle::get_B() const
{
    return B_;
}

Point Triangle::get_C() const
{
    return C_;
}

void Triangle::get_ABC(Point &A, Point &B, Point &C) const
{
    A = A_;
    B = B_;
    C = C_;
    return;
}

void Triangle::get_ABC_by_decreasing_y(Point &A, Point &B, Point &C) const
{
    complex_number xy_A = A_.get_affix();
    complex_number xy_B = B_.get_affix();
    complex_number xy_C = C_.get_affix();
    if(imag(xy_A) >=  imag(xy_B))
    {
        if(imag(xy_A) >=  imag(xy_C))
        {
            A = A_;
            if(imag(xy_B) >=  imag(xy_C))
            {
                B = B_;
                C = C_;
            }
            else
            {
                B = C_;
                C = B_;
            }
        }
        else
        {
            A = C_;
            B = A_;
            C = B_;
        }
    }
    else
    {
        if(imag(xy_B) >=  imag(xy_C))
        {
            A = B_;
            if(imag(xy_A) >=  imag(xy_C))
            {
                B = A_;
                C = C_;
            }
            else
            {
                C = A_;
                B = C_;
            }
        }
        else
        {
            A = C_;
            B = B_;
            C = A_;
        }
    }

    return;
}

bool Triangle::barycentric_coordinates(double &alpha, double &beta, double &gamma, const Point &M) const
{
    complex_number z_A = A_.get_affix(), z_B = B_.get_affix(), z_C = C_.get_affix(), z_M = M.get_affix();
    double x_0 = real(z_C) - real(z_M), y_0 = imag(z_C) - imag(z_M);
    double x_1 = real(z_C) - real(z_A), y_1 = imag(z_C) - imag(z_A);
    double x_2 = real(z_C) - real(z_B), y_2 = imag(z_C) - imag(z_B);
    double det = x_1*y_2 - y_1*x_2;
    if (det == 0)
    {
        return false;
    }
    alpha = (y_2*x_0 - x_2*y_0)/det;
    beta = (-y_1*x_0 + x_1*y_0)/det;
    gamma = 1 - alpha - beta;
    return true;
}

Point Triangle::point_from_barycentric_coordinates(const double &alpha,const double &beta,const double &gamma) const
{
    complex_number z_A = A_.get_affix(), z_B = B_.get_affix(), z_C = C_.get_affix(), z_M;
    z_M = (alpha*z_A + beta*z_B + gamma*z_C);
    return Point(z_M);
}
bool Triangle::is_inside(double &alpha, double &beta, double &gamma, const Point &M) const
{
    if(!barycentric_coordinates(alpha, beta, gamma, M))
    {
        return false;
    }
    else
    {
        return alpha<=1 && beta<=1 && gamma<=1 && alpha>=0 && beta>=0 && gamma>=0;
    }

}

void Triangle::get_extrema(double &x_min, double &x_max, double &y_min, double &y_max) const
{
    complex_number z_A = A_.get_affix(), z_B = B_.get_affix(), z_C = C_.get_affix();

    x_min = real(z_A);
    x_max = x_min;
    y_min = imag(z_A);
    y_max = y_min;

    if (real(z_B) < x_min)
    {
        x_min = real(z_B);
    }
    else if (real(z_B) > x_max)
    {
        x_max = real(z_B);
    }

    if (imag(z_B) < y_min)
    {
        y_min = imag(z_B);
    }
    else if (imag(z_B) > y_max)
    {
        y_max = imag(z_B);
    }

    if (real(z_C) < x_min)
    {
        x_min = real(z_C);
    }
    else if (real(z_C) > x_max)
    {
        x_max = real(z_C);
    }

    if (imag(z_C) < y_min)
    {
        y_min = imag(z_C);
    }
    else if (imag(z_C) > y_max)
    {
        y_max = imag(z_C);
    }


    return;
}


