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

#include "curve.hpp"

std::ostream & operator<<(std::ostream &out, const Curve &C)
{
    if (C.points_.size() == 0)
    {
        out << "The curve is empty." << std::endl;
    }
    else
    {
        out << "List of the curve's points coordinates: " << std::endl;
        out << "   ";
        unsigned int i;
        for (i=0; i<C.points_.size(); i++)
        {
            out << C.points_[i].get_affix() << " ";
        }
        out << std::endl;
    }
    return out;
}

void Curve::reset_curve()
{
    points_.clear();
    points_.reserve(20);

    return;
}

unsigned int Curve::nb_points() const
{
    return points_.size();
}

const Point & Curve::get_point(const unsigned int &index) const
{
    return points_[index];
}

const segment Curve::get_segment(const unsigned int &index) const
{
    return segment(points_[index], points_[(index+1)%points_.size()]);
}

bool Curve::is_inside(const Point & point) const
{
    unsigned int nb_intersections = 0;
    double x_left, y_left, x_right, y_right;
    complex_number z = point.get_affix();
    unsigned int n = points_.size();

    unsigned int i;
    for (i=0; i<n; i++)
    {
        x_left = real(points_[i].get_affix() - z);
        y_left = imag(points_[i].get_affix() - z);
        x_right = real(points_[(i+1)%n].get_affix() - z);
        y_right = imag(points_[(i+1)%n].get_affix() - z);
        if (x_left*x_right<0)
        {
            if (y_left + x_left*(y_right - y_left)/(x_left - x_right) > 0)
            {
                nb_intersections++;
            }
        }
        else if((x_left<x_right?x_left:x_right) == 0)
        {
            if (x_left==0)
            {
                if (y_left>0)
                {
                    nb_intersections++;
                }
            }
            else
            {
                if (y_right >0)
                {
                    nb_intersections++;
                }
            }
        }
    }
    return (nb_intersections%2 == 1);
}

void Curve::remove_point(const unsigned int &index)
{
    points_.erase(points_.begin() + index);
    return;
}

void Curve::add_point(const Point &point)
{
    points_.push_back(point);
    return;
}

void Curve::replace_point(const unsigned int &index, const Point &new_point)
{
    points_[index] = new_point;
    return;
}

void Curve::smooth_curve(std::vector<complex_number> &tangent_factors)
{
    unsigned int nb_points_segment = 20;

    unsigned int n = points_.size();
    std::vector<Point> new_points;
    new_points.reserve(n*nb_points_segment);

    complex_number z_left, z_right, a, b, c, d, tangent_left, tangent_right, t;

    z_right = points_[0].get_affix();
    tangent_right = 2.0 * tangent_factors[0] * (points_[1].get_affix() - points_.back().get_affix());

    unsigned int i,k;
    for(i=0; i<n; i++)
    {
        z_left = z_right;
        z_right = points_[(i+1)%n].get_affix();
        tangent_left = tangent_right;
        tangent_right = 2.0 * tangent_factors[(i+1)%n] * (points_[(i+2)%n].get_affix() - points_[i].get_affix());

        a = -2.0*z_right + 2.0*z_left + tangent_left + tangent_right;
        b = 3.0*z_right - 3.0*z_left - 2.0*tangent_left - tangent_right;
        c = tangent_left;
        d = z_left;

        for (k=0; k< nb_points_segment; k++)
        {
            t = k*1.0/nb_points_segment;
            new_points.push_back(Point(a*t*t*t+b*t*t+c*t+d));
        }
    }
    points_ = new_points;
    return;
}

void Curve::create_curve(std::vector<Point> &nodes, std::vector<complex_number> &tangents,
                         const curve_choice_type &curve_flag, bool smooth, const int &nb_sides, const unsigned int &node_index)
{
    switch(curve_flag)
    {
    case CUSTOM:
        create_custom(nodes, tangents, smooth);
        break;
    case RECTANGLE:
        create_rectangle(nodes);
        break;
    case POLYGON:
        create_polygon(nodes, nb_sides);
        break;
    case STAR:
        create_star(nodes, node_index, nb_sides);
        break;
    case ELLIPSE:
        create_ellipse(nodes, nb_sides);
        break;
    case EPICYCLOID:
        create_epicycloid(nodes, nb_sides, 300);
        break;
    case KOCH:
        create_koch_curve(nodes, nb_sides);
        break;
    default:
        std::cout << "ERROR in Curve::create_curve: flag problem" << std::endl;
        throw(QString("ERROR in Curve::create_curve: flag problem"));
    }

    return;
}

void Curve::create_custom(std::vector<Point> &nodes, std::vector<complex_number> tangent_factors, bool smooth)
{
    points_ = nodes;

    if (smooth)
    {
        smooth_curve(tangent_factors);
    }

    return;
}

void Curve::create_rectangle(std::vector<Point> &nodes)
{
    points_.clear();
    points_.reserve(4);

    complex_number z = nodes[0].get_affix();

    points_.push_back(Point(z));
    points_.push_back(Point(conj(z)));
    points_.push_back(Point(-z));
    points_.push_back(Point(-conj(z)));

    return;
}

void Curve::create_polygon(std::vector<Point> &nodes, const int &nb_sides)
{
    points_.resize(nb_sides);
    complex_number z = nodes[0].get_affix();
    double theta;

    int i;
    for (i=0; i<nb_sides; i++)
    {
        theta = 2*i*M_PI/nb_sides;
        points_[i] = Point(z*complex_number(cos(theta), sin(theta)));
    }
    return;
}

void Curve::create_star(std::vector<Point> &nodes, const int &index_node_selected, const int &nb_points)
{
    complex_number Z = nodes[0].get_affix();
    complex_number z = nodes[1].get_affix();
    complex_number u_0(cos(M_PI/nb_points), sin(M_PI/nb_points));

    if (index_node_selected==0)
    {
        z = abs(z)*Z*u_0/abs(Z);
        nodes[1] = Point(z);
    }
    else
    {
        Z = abs(Z)*z*conj(u_0)/abs(z);
        nodes[0] = Point(Z);
    }

    u_0 *= u_0;
    complex_number u(1.0, 0.0);
    points_.clear();
    int i;
    for (i=0; i<nb_points; i++)
    {
        points_.push_back(Point(Z*u));
        points_.push_back(Point(z*u));
        u *= u_0;
    }
    return;
}

void Curve::create_ellipse(std::vector<Point> &nodes, const int &nb_points)
{
    complex_number z = nodes[0].get_affix();
    double a = real(z)*sqrt(2.0);
    double b = imag(z)*sqrt(2.0);
    points_.clear();
    double t, cos_t, sin_t;

    int i;
    for (i=0; i<nb_points; i++)
    {
        t = i*2*M_PI/nb_points;
        cos_t = cos(t);
        sin_t = sin(t);
        points_.push_back(complex_number(a*cos_t, b*sin_t));
    }
    return;
}

void Curve::create_koch_segment(int depth, Point left_endpoint, Point right_endpoint)
{
    reset_curve();
    if (depth < 0)
    {
        std::cout << "ERROR in Curve::create_koch_segment: negative depth" << std::endl;
    }
    else if (depth == 0)
    {
        add_point(left_endpoint);
        add_point(right_endpoint);
        return;
    }
    else
    {
        Curve K;
        K.create_koch_segment(depth - 1, left_endpoint, right_endpoint);
        unsigned int N = K.nb_points();
        complex_number z_0, z_1, z_2, z_3, z_4;
        complex_number j(-0.5, 0.5*sqrt(3.0));
        unsigned int i;
        for (i=0; i+1<N; i++)
        {
            z_0 = K.points_[i].get_affix();
            z_4 = K.points_[i+1].get_affix();
            z_1 = (1.0/3.0)*(2.0*z_0 + z_4);
            z_3 = (1.0/3.0)*(z_0 + 2.0*z_4);
            z_2 = z_1 + j*(z_0 - z_1);
            add_point(Point(z_0));
            add_point(Point(z_1));
            add_point(Point(z_2));
            add_point(Point(z_3));
        }
        add_point(right_endpoint);
    }
    return;
}

void Curve::create_koch_curve(std::vector<Point> &nodes, int depth)
{
    reset_curve();
    complex_number z_0 = nodes[0].get_affix();
    complex_number j(-0.5, 0.5*sqrt(3.0));
    complex_number z_1 = j*z_0;
    complex_number z_2 = j*z_1;
    Curve K_1, K_2, K_3;
    K_1.create_koch_segment(depth, Point(z_0), Point(z_1));
    K_2.create_koch_segment(depth, Point(z_1), Point(z_2));
    K_3.create_koch_segment(depth, Point(z_2), Point(z_0));

    unsigned int i, N = K_1.nb_points();
    for (i=0; i+1< N; i++)
    {
        add_point(K_1.points_[i]);
    }
    for (i=0; i+1< N; i++)
    {
        add_point(K_2.points_[i]);
    }
    for (i=0; i+1< N; i++)
    {
        add_point(K_3.points_[i]);
    }

    return;
}

void Curve::create_epicycloid(std::vector<Point> &nodes, int nb_cusps, int nb_points)
{
    //std::cout << "Entering Curve::create_epicycloid with nb points = " << nb_points << std::endl;
    reset_curve();
    complex_number z = nodes[0].get_affix();

    int k = nb_cusps;
    double r = 1.3/(k + 2);

    double t, x, y;
    int i;
    for (i=0; i<nb_points; i++)
    {
        t = i*2*M_PI/nb_points;
        x = r*(k+1)*cos(t) - r*cos((k+1)*t);
        y = r*(k+1)*sin(t) - r*sin((k+1)*t);
        add_point(Point((1.0/(k*r))*z*complex_number(x, y)));
    }

    return;
}

bool Curve::is_self_intersecting() const
{
    if (points_.size() < 3)
    {
        return false;
    }

    complex_number z_1, z_2;
    segment s_1, s_2;
    unsigned int i, j, N = points_.size();
    for (i=0; i<N; i++)
    {
        z_1 = points_[i].get_affix() - points_[(i+N-1)%N].get_affix();
        z_2 = points_[(i+1)%N].get_affix() - points_[i].get_affix();

        if (imag(conj(z_2) * z_1) == 0)
        {
            if (real(z_1)*real(z_2) < 0)
            {
                return true;
            }
            else if (real(z_1)*real(z_2) == 0 && imag(z_1)*imag(z_2)<=0)
            {
                return true;
            }
        }

        s_1 = segment(points_[i], points_[(i+1)%N]);
        for (j=i+2; j<N; j++)
        {
            if (i==0 && j==N-1)
            {
                continue;
            }

            s_2 = segment(points_[j], points_[(j+1)%N]);
            if (are_intersecting(s_1, s_2))
            {
                return true;
            }
        }
    }
    return false;
}

void Curve::point_projection(Point &output, const Point &point) const
{
    unsigned int N = points_.size();

    if (N==0)
    {
        std::cout << "ERROR in Curve::point_projection: curve is empty" << std::endl;
        throw(QString("ERROR in Curve::point_projection: curve is empty"));
        return;
    }
    else if (N==1)
    {
        output = points_.front();
        return;
    }
    else if (N==2)
    {
        output = point.projection_on_segment(points_.front(), points_.back());
        return;
    }
    else
    {
        unsigned int i;
        Point current_projection, candidate = point.projection_on_segment(points_.back(), points_.front());
        double current_norm, min_norm = norm(point.get_affix() - candidate.get_affix());
        for(i=0; i<N-1; i++)
        {
            current_projection = point.projection_on_segment(points_[i], points_[i+1]);
            current_norm = norm(current_projection.get_affix() - point.get_affix());
            if (current_norm < min_norm)
            {
                min_norm = current_norm;
                candidate = current_projection;
            }
        }
        output = candidate;
        return;
    }
}

void Curve::get_extrema(double &x_min, double &x_max, double &y_min, double &y_max) const
{
    unsigned int N = points_.size();
    if (N == 0)
    {
        std::cout << "ERROR in Curve::get_extrema: the curve is empty!" << std::endl;
        throw(QString("ERROR in Curve::get_extrema: the curve is empty!"));

    }
    else
    {
        complex_number z = points_[0].get_affix();
        double x = real(z);
        double y = imag(z);
        x_min = x;
        x_max = x;
        y_min = y;
        y_max = y;

        unsigned int i;
        for (i=0; i<N; i++)
        {
            z = points_[i].get_affix();
            x = real(z);
            y = imag(z);

            if (x < x_min)
            {
                x_min = x;
            }
            else if (x > x_max)
            {
                x_max = x;
            }

            if (y < y_min)
            {
                y_min = y;
            }
            else if (y > y_max)
            {
                y_max = y;
            }
        }
    }

    return;
}

Point Curve::center_of_mass() const
{
    unsigned int N = points_.size();
    complex_number Z = complex_number(0.0, 0.0);

    if (N==0)
    {
        std::cout << "ERROR in Curve::get_extrema: the curve is empty!" << std::endl;
    }
    else
    {
        unsigned int i;
        for(i=0; i<N; i++)
        {
            Z += points_[i].get_affix();
        }
        Z = (1.0/N)*Z;
    }

    return Point(Z);
}

void Curve::get_max_dimensions(double &max_width, double &max_height) const
{
    double x_min, x_max, y_min, y_max;
    get_extrema(x_min, x_max, y_min, y_max);
    max_width = x_max - x_min;
    max_height = y_max - y_min;
    return;
}

