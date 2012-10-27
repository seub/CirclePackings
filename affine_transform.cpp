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

#include "affine_transform.hpp"
#include "triangle.hpp"

Affine_Transform::Affine_Transform()
{
}

Affine_Transform::Affine_Transform(double a_11, double a_12, double a_21, double a_22, double t_x, double t_y) :
    a_11_(a_11), a_12_(a_12), a_21_(a_21), a_22_(a_22), t_x_(t_x), t_y_(t_y)
{
}

bool Affine_Transform::inverse(Affine_Transform &output)
{
    double det = a_11_*a_22_ - a_12_*a_21_;

    if (det == 0)
    {
        return false;
    }
    else
    {
        output.compose(Affine_Transform(a_22_/det, -a_12_/det, -a_21_/det, a_11_/det, 0.0, 0.0), Affine_Transform(1.0, 0.0, 0.0, 1.0, -t_x_, -t_y_));
        return true;
    }
}

bool Affine_Transform::match_triangles(const Triangle &T_1, const Triangle &T_2)
{
    Point A_1, B_1, C_1, A_2, B_2, C_2;
    T_1.get_ABC(A_1,B_1,C_1);
    T_2.get_ABC(A_2,B_2,C_2);

    complex_number u_1 = B_1.get_affix() - A_1.get_affix();
    complex_number v_1 = C_1.get_affix() - A_1.get_affix();
    complex_number u_2 = B_2.get_affix() - A_2.get_affix();
    complex_number v_2 = C_2.get_affix() - A_2.get_affix();

    Affine_Transform f(real(u_1), real(v_1), imag(u_1), imag(v_1), 0.0, 0.0);
    Affine_Transform g(real(u_2), real(v_2), imag(u_2), imag(v_2), 0.0, 0.0);
    Affine_Transform f_inverse;

    if(!f.inverse(f_inverse))
    {
        return false;
    }

    Affine_Transform h;
    h.compose(g,f_inverse);
    complex_number t = A_2.get_affix() - (h.kick(A_1)).get_affix();
    compose(Affine_Transform(1.0, 0.0, 0.0, 1.0, real(t), imag(t)), h);
    return true;
}

void Affine_Transform::set_for_match_triangles_in_pixels(double *input_scale, int *input_origin_x_in_pixels, int *input_origin_y_in_pixels,
                                                         double *output_scale, int *output_origin_x_in_pixels, int *output_origin_y_in_pixels)
{
    input_scale_ = input_scale;
    input_origin_x_in_pixels_ = input_origin_x_in_pixels;
    input_origin_y_in_pixels_ = input_origin_y_in_pixels;

    output_scale_ = output_scale;
    output_origin_x_in_pixels_ = output_origin_x_in_pixels;
    output_origin_y_in_pixels_ = output_origin_y_in_pixels;

    /*std::cout << "input o_x = " << *input_origin_x_in_pixels_ << std::endl;
    std::cout << "input o_y = " << *input_origin_y_in_pixels_ << std::endl;
    std::cout << "output o_x = " << *output_origin_x_in_pixels_ << std::endl;
    std::cout << "output o_y = " << *output_origin_y_in_pixels_ << std::endl;*/

    return;
}

bool Affine_Transform::match_triangles_in_pixels(const Triangle &T_1, const Triangle &T_2)
{
    Affine_Transform f(1.0/(*input_scale_), 0.0, 0.0, -1.0/(*input_scale_),
                       -*input_origin_x_in_pixels_/(*input_scale_), *input_origin_y_in_pixels_/(*input_scale_));

    Affine_Transform h(*output_scale_, 0.0, 0.0, -(*output_scale_), *output_origin_x_in_pixels_, *output_origin_y_in_pixels_);

    Affine_Transform g;
    if(g.match_triangles(T_1, T_2))
    {
        compose(g, f);
        compose(h, *this);
        return true;
    }
    else
    {
        return false;
    }
}

void Affine_Transform::match_coordinates_in_pixels()
{
    Affine_Transform f(1.0/(*input_scale_), 0.0, 0.0, -1.0/(*input_scale_),
                       -*input_origin_x_in_pixels_/(*input_scale_), *input_origin_y_in_pixels_/(*input_scale_));

    Affine_Transform h(*output_scale_, 0.0, 0.0, -(*output_scale_), *output_origin_x_in_pixels_, *output_origin_y_in_pixels_);

    compose(h, f);
    return;
}


// Operators

Point Affine_Transform::kick(const Point &M)
{
    complex_number z = M.get_affix();
    double x = real(z);
    double y = imag(z);
    double X = a_11_*x + a_12_*y + t_x_;
    double Y = a_21_*x + a_22_*y + t_y_;
    return Point(complex_number(X,Y));
}

void Affine_Transform::compose(const Affine_Transform &f, const Affine_Transform &g)
{
    a_11_ = f.a_11_*g.a_11_ + f.a_12_*g.a_21_;
    a_12_ = f.a_11_*g.a_12_ + f.a_12_*g.a_22_;
    a_21_ = f.a_21_*g.a_11_ + f.a_22_*g.a_21_;
    a_22_ = f.a_21_*g.a_12_ + f.a_22_*g.a_22_;

    t_x_ = f.a_11_*g.t_x_ + f.a_12_*g.t_y_ + f.t_x_;
    t_y_ = f.a_21_*g.t_x_ + f.a_22_*g.t_y_ + f.t_y_;

    return;
}

integer_coordinates Affine_Transform::kick(const integer_coordinates& xy)
{
    int x = xy.first, y = xy.second;
    int X = int_round(a_11_*x + a_12_*y + t_x_);
    int Y = int_round(a_21_*x + a_22_*y + t_y_);
    return integer_coordinates(X,Y);
}
