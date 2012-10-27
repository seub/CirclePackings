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

#include "triangulation.hpp"
#include "curve.hpp"

Triangulation::Triangulation()
{
}

void Triangulation::reset_triangulation()
{
    points_.clear();
    triangles_.clear();
    tiling_coordinates_.clear();
    return;
}

bool Triangulation::get_triangle(const unsigned int &index, Triangle &output) const
{
    if (index >= triangles_.size())
    {
        std::cout << "ERROR in Triangulation::get_triangle: bad index " << index
                  << " (triangulation only has " << triangles_.size() << " triangles)" << std::endl;
        return false;
    }
    std::vector<unsigned int> tri = triangles_[index];
    Point A = points_[tri[0]];
    Point B = points_[tri[1]];
    Point C = points_[tri[2]];
    output = Triangle(A, B, C);
    return true;
}

Triangle Triangulation::get_triangle(const unsigned int &index) const
{
    if (index >= triangles_.size())
    {
        std::cout << "ERROR in Triangulation::get_triangle: bad index " << index
                  << " (triangulation only has " << triangles_.size() << " triangles)" << std::endl;
        throw(QString("ERROR in Triangulation::get_triangle: bad index"));
    }
    std::vector<unsigned int> tri = triangles_[index];
    Point A = points_[tri[0]];
    Point B = points_[tri[1]];
    Point C = points_[tri[2]];
    return Triangle(A, B, C);
}

integer_coordinates Triangulation::get_tiling_coordinates(const unsigned int &index) const
{
    if (index >= tiling_coordinates_.size())
    {
        std::cout << "ERROR in Triangulation::get_tiling_coordinates: bad index" << std::endl;
        throw(QString("ERROR in Triangulation::get_tiling_coordinates: bad index"));
    }
    return tiling_coordinates_[index];
}

unsigned int Triangulation::nb_points() const
{
    return points_.size();
}

QColor Triangulation::get_color(const unsigned int &index) const
{
    if (index >= colors_.size())
    {
        std::cout << "ERROR in Triangulation::get_color: bad index " << index
                  << " (triangulation only has " << colors_.size() << " colors (note: number of triangles is "
                  << triangles_.size() << ")" << std::endl;
        throw(QString("ERROR in Triangulation::get_color: bad index"));
    }
    return colors_[index];
}

std::vector<QColor> Triangulation::get_colors() const
{
    return colors_;
}

unsigned int Triangulation::nb_colors() const
{
    return colors_.size();
}

void Triangulation::set_colors(std::vector<QColor> &colors)
{
    if (colors.size() != triangles_.size())
    {
        std::cout << "ERROR in Triangulation::set_colors: size problem" << std::endl;
        throw(QString("ERROR in Triangulation::set_colors: size problem"));
    }
    colors_ = colors;
    return;
}

unsigned int Triangulation::nb_triangles() const
{
    return triangles_.size();
}

void Triangulation::add_point(const Point &new_point, const integer_coordinates &tiling_coordinates)
{
    points_.push_back(new_point);
    tiling_coordinates_.push_back(tiling_coordinates);
    return;
}

void Triangulation::add_triangle(const unsigned int &point_index_1, const unsigned int &point_index_2,
                                 const unsigned int &point_index_3)
{
    std::vector<unsigned int> tri;
    tri.push_back(point_index_1);
    tri.push_back(point_index_2);
    tri.push_back(point_index_3);
    triangles_.push_back(tri);
    return;
}


void Triangulation::cyclic_sort_triangles()
{
    unsigned int i;
    for(i=0; i<triangles_.size(); i++)
    {
        cyclic_sort_triangle(i);
    }
    return;
}

void Triangulation::cyclic_sort_triangle(const unsigned int &index)
{
    if (index >= triangles_.size())
    {
        std::cout << "ERROR in Triangulation::cyclic_sort_triangle: bad index" << std::endl;
        throw(QString("ERROR in Triangulation::cyclic_sort_triangle: bad index"));
    }
    std::vector<unsigned int> tri = triangles_[index];
    integer_coordinates xy_0 = tiling_coordinates_[tri[0]], xy_1 = tiling_coordinates_[tri[1]],
            xy_2 = tiling_coordinates_[tri[2]];
    unsigned int i_1 = 0, i_2 = 1, i_3 = 2, swtch;
    if (xy_1.second < xy_0.second || (xy_1.second==xy_0.second && xy_1.first < xy_0.first))
    {
        swtch = i_1;
        i_1 = i_2;
        i_2 = swtch;
    }

    xy_1 = tiling_coordinates_[tri[i_2]];
    if (xy_2.second < xy_1.second || (xy_2.second==xy_1.second && xy_2.first < xy_1.first))
    {
        swtch = i_2;
        i_2 = i_3;
        i_3 = swtch;
    }

    xy_0 = tiling_coordinates_[tri[i_1]];
    xy_1 = tiling_coordinates_[tri[i_2]];

    if (xy_1.second < xy_0.second || (xy_1.second==xy_0.second && xy_1.first < xy_0.first))
    {
        swtch = i_1;
        i_1 = i_2;
        i_2 = swtch;
    }

    tri[0] = triangles_[index][i_1];
    tri[1] = triangles_[index][i_2];
    tri[2] = triangles_[index][i_3];
    triangles_[index] = tri;

    return;
}

void Triangulation::color(const Triangulation_coloring &coloring)
{
    switch(coloring)
    {
    case T_PLAIN:
        color_plain();
        break;
    case T_STRIPES_1:
        color_stripes_1();
        break;
    case T_STRIPES_2:
        color_stripes_2();
        break;
    case T_STRIPES_3:
        color_stripes_3();
        break;
    case T_TRIANGLES:
        color_triangles();
        break;
    case T_SQUARES:
        color_squares();
        break;
    case T_SQUARES_2:
        color_squares_2();
        break;
    case T_SHADING:
        color_shading();
        break;
    case T_IMAGE:
        color_null();
        break;
    default:
        std::cout << "ERROR in Graph_Triangulater::color: flag problem" << std::endl;
        throw(QString("ERROR in Graph_Triangulater::color: flag problem"));
    }

    return;
}

void Triangulation::color_plain()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color(100, 215, 255);
    unsigned int i;
    for (i=0; i<N; i++)
    {
        colors_.push_back(color);
    }
    return;
}

void Triangulation::color_stripes_1()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color;
    QColor color_1(138, 43, 226);
    QColor color_2(51, 204, 255);
    integer_coordinates xy;
    unsigned int i;
    for (i=0; i<N; i++)
    {
        xy = tiling_coordinates_[triangles_[i][0]];
        color = (xy.second%2 == 0) ? color_1 : color_2;
        colors_.push_back(color);
    }
    return;
}

void Triangulation::color_stripes_2()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color;
    QColor color_1(255, 130, 0);
    QColor color_2(255, 210, 10);
    integer_coordinates xy_0, xy_1, xy;
    unsigned int i;
    for (i=0; i<N; i++)
    {
        xy_0 = tiling_coordinates_[triangles_[i][0]];
        xy_1 = tiling_coordinates_[triangles_[i][1]];
        xy = xy_0.second == xy_1.second? xy_1 : xy_0;
        color = ((xy.first + (xy.second+1)/2)%2==0) ? color_1 : color_2;
        colors_.push_back(color);
    }
    return;
}

void Triangulation::color_stripes_3()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color;
    QColor color_1(55, 215, 55);
    QColor color_2(30, 130, 30);
    integer_coordinates xy;
    unsigned int i;
    for (i=0; i<N; i++)
    {
        xy = tiling_coordinates_[triangles_[i][0]];
        color = ((xy.first + xy.second/2)%2 == 0) ? color_1 : color_2;
        colors_.push_back(color);
    }
    return;
}


void Triangulation::color_triangles()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color;
    QColor color_2 = QColor(255, 222, 173);
    QColor color_1 = QColor(255, 165, 0);
    integer_coordinates xy_1,xy_2;
    unsigned int i;
    for (i=0; i<N; i++)
    {
        xy_1 = tiling_coordinates_[triangles_[i][0]];
        xy_2 = tiling_coordinates_[triangles_[i][1]];
        color = (xy_1.second == xy_2.second) ? color_1 : color_2;
        colors_.push_back(color);
    }
    return;
}

void Triangulation::color_squares()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color;
    QColor color_1(198, 226, 255);
    QColor color_2(67, 110, 238);
    integer_coordinates xy, xy_0, xy_1;
    unsigned int i;
    for (i=0; i<N; i++)
    {
        xy_0 = tiling_coordinates_[triangles_[i][0]];
        xy_1 = tiling_coordinates_[triangles_[i][1]];
        xy = (xy_0.second == xy_1.second) ? xy_1 : xy_0;
        color = ((xy.first - xy.second/2)%2 == 0) ? color_1 : color_2;
        colors_.push_back(color);
    }
    return;
}

void Triangulation::color_squares_2()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    QColor color;
    QColor color_1(198, 226, 255);
    QColor color_2(67, 110, 238);
    integer_coordinates xy, xy_0, xy_1;
    unsigned int i;
    for (i=0; i<N; i++)
    {
        xy_0 = tiling_coordinates_[triangles_[i][0]];
        xy_1 = tiling_coordinates_[triangles_[i][1]];
        xy = (xy_0.second == xy_1.second) ? xy_1 : xy_0;
        color = ( ((xy.first +3*((xy.second+1)/2))/2 - xy.second/2)%2 == 0 ) ? color_1 : color_2;
        colors_.push_back(color);
    }
    return;
}

void Triangulation::extrema_tiling(int &x_min, int &x_max, int &y_min, int &y_max)
{
    integer_coordinates xy = tiling_coordinates_[0];
    x_min = xy.first;
    x_max = xy.first;
    y_min = xy.second;
    y_max = xy.second;

    unsigned int i;
    for(i=1; i<tiling_coordinates_.size(); i++)
    {
        xy = tiling_coordinates_[i];
        if (xy.first < x_min)
        {
            x_min = xy.first;
        }
        if (xy.first > x_max)
        {
            x_max = xy.first;
        }
        if (xy.second < y_min)
        {
            y_min = xy.second;
        }
        if (xy.second > y_max)
        {
            y_max = xy.second;
        }
    }

    return;
}

void Triangulation::color_shading()
{

    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);

    int x_min, x_max, y_min, y_max;
    double x, y;
    extrema_tiling(x_min, x_max, y_min, y_max);

    int color1[3]={255,255,255};
    int color2[3]={255,0,0};
    int color3[3]={255,255,0};
    int color4[3]={0,0,255};

    int rouge, vert, bleu;
    double t1,t2;
    integer_coordinates xy_0, xy_1;

    unsigned int i;
    for(i=0; i<triangles_.size(); i++)
    {
        xy_0 = tiling_coordinates_[triangles_[i][0]];
        xy_1 = tiling_coordinates_[triangles_[i][1]];
        x = (xy_0.first == xy_1.first) ? xy_0.first + 0.5 : xy_0.first;
        y = (xy_0.first == xy_1.first) ? xy_0.second + 0.33 : xy_0.second + 0.67;
        t1=(x-x_min)*1.0/(x_max-x_min);
        t2=(y-y_min)*1.0/(y_max-y_min);
        rouge=(int) (t1*color1[0]+(1-t1)*color2[0]+t2*color3[0]+(1-t2)*color4[0])/2;
        vert=(int) ((t1*color1[1]+(1-t1)*color2[1]+t2*color3[1]+(1-t2)*color4[1])/2);
        bleu=(int) ((t1*color1[2]+(1-t1)*color2[2]+t2*color3[2]+(1-t2)*color4[2])/2);
        rouge = std::min(std::max(rouge, 0),255);
        vert = std::min(std::max(vert, 0),255);
        bleu = std::min(std::max(bleu, 0),255);
        colors_.push_back(QColor(rouge,vert,bleu));
    }
    return;
}

void Triangulation::color_null()
{
    colors_.resize(triangles_.size());
    return;
}

std::vector<unsigned int> Triangulation::compute_erdos() const
{
    unsigned int n = triangles_.size();
    std::vector<unsigned int> res;
    res.resize(n);

    std::vector<bool> computed;
    computed.resize(n);
    fill(computed.begin(),computed.end(),false);

    std::vector<unsigned int> previously_computed;
    std::vector<unsigned int> new_computed;

    unsigned int nb_computed=0;
    unsigned int nb_erdos = 0;

    unsigned int k;
    for (k = nb_interior_triangles_; k<triangles_.size(); k++)
    {
        res[k] = nb_erdos + 1;
        computed[k] = true;
        previously_computed.push_back(k);
        nb_computed++;
    }
    nb_erdos++;

    unsigned int i, j;
    while(nb_computed<n && nb_erdos<n)
    {
        new_computed.clear();
        for(i=0; i<previously_computed.size(); i++)
        {
            for (j=0; j<nb_interior_triangles_; j++)
            {
                if(!computed[j])
                {
                    if (are_adjacent_triangles(previously_computed[i], j))
                    {
                        res[j] = nb_erdos+1;
                        computed[j] = true;
                        new_computed.push_back(j);
                        nb_computed++;
                    }
                }
            }
        }
        previously_computed = new_computed;
        nb_erdos++;
    }

    std::cout << "max nb erdos = " << nb_erdos << std::endl;
    return res;
}

void Triangulation::color_erdos()
{
    unsigned int N = triangles_.size();
    colors_.clear();
    colors_.reserve(N);
    std::vector<unsigned int> erdos = compute_erdos();

    QColor color_1 = QColor(100, 149, 237);
    QColor color_2 = QColor(25, 25, 112);
    QColor color;

    unsigned int i;

    for(i=0; i<N; i++)
    {
        if ((erdos[i]/2)%2 == 0)
        {
            color = color_1;
        }
        else
        {
            color = color_2;
        }
        colors_.push_back(color);
    }
    return;
}

bool Triangulation::are_adjacent_triangles(const unsigned int &index_1, const unsigned int &index_2) const
{
    if (index_1>=triangles_.size() || index_2>= triangles_.size())
    {
        std::cout << "ERROR in Triangulation::are_adjacent_triangles: bad index" << std::endl;
        throw(QString("ERROR in Triangulation::are_adjacent_triangles: bad index"));
    }
    std::vector<unsigned int> t_1 = triangles_[index_1], t_2 = triangles_[index_2];

    unsigned int nb_common_points = 0;
    unsigned int i, j;
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            if (t_1[i] == t_2[j])
            {
                nb_common_points++;
            }
        }
    }
    return nb_common_points == 2;
}


bool Triangulation::triangle_containing(unsigned int &output_index,
                                        double &alpha, double &beta, double &gamma,const Point &point) const
{
    unsigned int i;
    for(i=0; i<triangles_.size(); i++)
    {
        if(get_triangle(i).is_inside(alpha, beta, gamma, point))
        {
            output_index = i;
            return true;
        }
    }
    return false;
}


void Triangulation::nearest_exterior_triangle(unsigned int &output_index, double &alpha, double &beta, double &gamma,
                                              const Point &point) const
{
    unsigned int i;
    double distance, distance_min;
    Point proj;
    Curve tri;
    Triangle triangle;
    std::vector<Point> nodes;
    std::vector<complex_number> tangents;

    nodes.resize(3);
    i = nb_interior_triangles_;
    triangle = get_triangle(i);
    triangle.get_ABC(nodes[0], nodes[1], nodes[2]);
    tri.create_custom(nodes, tangents);
    tri.point_projection(proj, point);
    distance_min = norm(proj.get_affix() - point.get_affix());
    output_index = i;
    for(i=nb_interior_triangles_+1; i<triangles_.size(); i++)
    {
        if(!triangle.barycentric_coordinates(alpha, beta, gamma, point))
        {
            continue;
        }
        triangle = get_triangle(i);
        triangle.get_ABC(nodes[0], nodes[1], nodes[2]);
        tri.create_custom(nodes, tangents);
        tri.point_projection(proj, point);
        distance = norm(proj.get_affix() - point.get_affix());
        if(distance < distance_min )
        {
            distance_min = distance;
            output_index = i;
        }
    }
    triangle = get_triangle(output_index);
    triangle.barycentric_coordinates(alpha,beta,gamma,point);
    return;
}
