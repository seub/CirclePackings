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

#include "triangulation_search_tree.hpp"

Triangulation_Search_Tree::Triangulation_Search_Tree()
{
    leaf_ = true;
    max_depth_ = 6;
    nb_max_triangles_leaf_ = 9;
}

Triangulation_Search_Tree::Triangulation_Search_Tree(const std::vector< unsigned int> &indices,
                                                     const Point &center,
                                                     double radius, unsigned int depth, Triangulation *triangulation)
{
    triangulation_ = triangulation;
    max_depth_ = 6;
    nb_max_triangles_leaf_ = 9;
    center_ = center;
    radius_ = radius;
    depth_ = depth;
    triangles_ = (indices.size() <= nb_max_triangles_leaf_ || depth_ > max_depth_) ? (indices) : (std::vector<unsigned int>());
    leaf_ = (indices.size() <= nb_max_triangles_leaf_ || depth_ > max_depth_);

    if(!leaf_)
    {
        std::vector< unsigned int> ru_indices, lu_indices, ld_indices, rd_indices;
        splitting_indices_list(indices,
                               ru_indices,lu_indices,ld_indices,rd_indices);
        ru_subtree_ = new Triangulation_Search_Tree(ru_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(1.0 ,1.0)),
                                                    radius_/2.0, depth_ + 1 , triangulation);
        lu_subtree_ = new Triangulation_Search_Tree(lu_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(-1.0 ,1.0)),
                                                    radius_/2.0, depth_ + 1, triangulation);
        ld_subtree_ = new Triangulation_Search_Tree(ld_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(-1.0, -1.0)),
                                                    radius_/2.0, depth_ + 1, triangulation);
        rd_subtree_ = new Triangulation_Search_Tree(rd_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(1.0 ,-1.0)),
                                                    radius_/2.0, depth_ + 1, triangulation);
    }
}

void Triangulation_Search_Tree::reset_search_tree(Triangulation *triangulation)
{
    triangulation_ = triangulation;
    if(!leaf_)
    {
        delete ru_subtree_;
        delete lu_subtree_;
        delete ld_subtree_;
        delete rd_subtree_;
    }
    center_ = Point(complex_number(0,0));
    radius_ = 1.5;
    depth_ = 0;

    std::vector<unsigned int> indices;
    indices.reserve(triangulation_->nb_triangles());
    unsigned int i;
    for(i=0; i<triangulation_->nb_triangles(); i++)
    {
        indices.push_back(i);
    }

    triangles_ = indices.size() <= nb_max_triangles_leaf_ ? indices : std::vector<unsigned int>();
    leaf_ = (indices.size() <= nb_max_triangles_leaf_);

    if(!leaf_)
    {
        std::vector< unsigned int> ru_indices, lu_indices, ld_indices, rd_indices;
        splitting_indices_list(indices,
                               ru_indices,lu_indices,ld_indices,rd_indices);
        ru_subtree_ = new Triangulation_Search_Tree(ru_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(1,1)),
                                                    radius_/2.0, depth_ + 1, triangulation_);
        lu_subtree_ = new Triangulation_Search_Tree(lu_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(-1,1)),
                                                    radius_/2.0, depth_ + 1, triangulation_);
        ld_subtree_ = new Triangulation_Search_Tree(ld_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(-1,-1)),
                                                    radius_/2.0, depth_ + 1, triangulation_);
        rd_subtree_ = new Triangulation_Search_Tree(rd_indices,
                                                    Point(center_.get_affix()+radius_/2.0*complex_number(1,-1)),
                                                    radius_/2.0, depth_ + 1, triangulation_);
    }
}

Triangulation_Search_Tree::~Triangulation_Search_Tree()
{
    if(leaf_)
    {
        return;
    }
    else
    {
        delete ru_subtree_;
        delete lu_subtree_;
        delete ld_subtree_;
        delete rd_subtree_;
        return;
    }
}

void Triangulation_Search_Tree::splitting_indices_list(const std::vector< unsigned int> &indices,
                                                       std::vector< unsigned int> &ru_indices,
                                                       std::vector< unsigned int> &lu_indices,
                                                       std::vector< unsigned int> &ld_indices,
                                                       std::vector< unsigned int> &rd_indices) const
{
    ru_indices.clear();
    lu_indices.clear();
    ld_indices.clear();
    rd_indices.clear();

    ru_indices.reserve(indices.size());
    lu_indices.reserve(indices.size());
    ld_indices.reserve(indices.size());
    rd_indices.reserve(indices.size());

    unsigned int i;

    double x_center = real(center_.get_affix()), y_center = imag(center_.get_affix());

    Parallel_Square ru(x_center, y_center, radius_);
    Parallel_Square lu(x_center - radius_, y_center, radius_);
    Parallel_Square ld(x_center - radius_, y_center - radius_, radius_);
    Parallel_Square rd(x_center, y_center - radius_, radius_);

    Triangle T;
    for(i=0; i< indices.size(); i++)
    {
        T = triangulation_->get_triangle(indices[i]);
        if(ru.meets_triangle(T))
        {
            ru_indices.push_back(indices[i]);
        }
        if(lu.meets_triangle(T))
        {
            lu_indices.push_back(indices[i]);
        }
        if(ld.meets_triangle(T))
        {
            ld_indices.push_back(indices[i]);
        }
        if(rd.meets_triangle(T))
        {
            rd_indices.push_back(indices[i]);
        }
    }
    return;
}

unsigned int Triangulation_Search_Tree::which_quarter(const Point &point) const
{
    complex_number z = point.get_affix() - center_.get_affix();
    if(z.real()<0.)
    {
        if(z.imag()<0.)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if(z.imag()<0.)
        {
            return 4;
        }
        else
        {
            return 1;
        }
    }
}

bool Triangulation_Search_Tree::get_containing_triangle_index(double &alpha, double &beta, double &gamma,
                                                              unsigned int &output_index, const Point &point) const
{

    if(leaf_)
    {
        unsigned int i;
        for(i = 0; i< triangles_.size(); i++)
        {
            if((triangulation_->get_triangle(triangles_[i])).is_inside(alpha, beta, gamma, point))
            {
                output_index = triangles_[i];
                return true;
            }
        }
        return false;
    }
    else
    {
        unsigned int q = which_quarter(point);
        switch(q)
        {
        case 1:
            return ru_subtree_->get_containing_triangle_index(alpha, beta, gamma,output_index, point);
            break;
        case 2:
            return lu_subtree_->get_containing_triangle_index(alpha, beta, gamma,output_index, point);
            break;
        case 3:
            return ld_subtree_->get_containing_triangle_index(alpha, beta, gamma,output_index, point);
            break;
        case 4:
            return rd_subtree_->get_containing_triangle_index(alpha, beta, gamma,output_index, point);
            break;
        default:
            std::cout << "Error in Triangulation_Search_Tree::get_containing_triangle_index" << std::endl;
            throw(QString("Error in Triangulation_Search_Tree::get_containing_triangle_index"));
        }
    }
}

void Triangulation_Search_Tree::cout_tree()
{
    if(!leaf_)
    {
        ru_subtree_->cout_tree();
        lu_subtree_->cout_tree();
        ld_subtree_->cout_tree();
        rd_subtree_->cout_tree();
    }
    else if(triangles_.size()>0)
    {
        unsigned int i;
        for(i=1; i< depth_;i++)
        {
            std::cout << " ";
        }for(i=0; i< triangles_.size();i++)
        {
            std::cout << triangles_[i] << " ";
        }
        std::cout << std::endl;
    }

}


bool Parallel_Square::contains_point(const Point &point) const
{
    complex_number z = point.get_affix();
    double x = real(z);
    double y = imag(z);

    return (x >= x_min_ && x <= x_min_ + side_length_ && y >= y_min_ && y <= y_min_ + side_length_);
}

bool Parallel_Square::meets_segment(const Point &first_endpoint, const Point &second_endpoint) const
{
    if (contains_point(first_endpoint) || contains_point(second_endpoint))
    {
        return true;
    }
    else
    {
        complex_number z_0 = first_endpoint.get_affix();
        complex_number z_1 = second_endpoint.get_affix();
        double x_0 = real(z_0), y_0 = imag(z_0);
        double x_1 = real(z_1), y_1 = imag(z_1);

        double t_xmin, t_xmax, t_ymin, t_ymax;
        if (x_0 == x_1)
        {
            if (x_0 < x_min_ || x_0 > x_min_ + side_length_)
            {
                return false;
            }
            else
            {
                if (y_0 < y_1)
                {
                    return (y_1 > y_min_ && y_0 < y_min_);
                }
                else if (y_0 > y_1)
                {
                    return (y_0 > y_min_ && y_1 < y_min_);
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            if (x_0 > x_1)
            {
                t_xmin = (x_min_ - x_1)/(x_0 - x_1);
                t_xmax = (x_min_ + side_length_ - x_1)/(x_0 - x_1);
            }
            else
            {
                t_xmax = (x_min_ - x_1)/(x_0 - x_1);
                t_xmin = (x_min_ + side_length_ - x_1)/(x_0 - x_1);
            }

            if (t_xmax < 0 || t_xmin > 1)
            {
                return false;
            }
            else
            {
                if (y_0 == y_1)
                {
                    return (y_0 >= y_min_ && y_0 <= y_min_ + side_length_);
                }
                else
                {
                    if (y_0 > y_1)
                    {
                        t_ymin = (y_min_ - y_1)/(y_0 - y_1);
                        t_ymax = (y_min_ + side_length_ - y_1)/(y_0 - y_1);
                    }
                    else
                    {
                        t_ymax = (y_min_ - y_1)/(y_0 - y_1);
                        t_ymin = (y_min_ + side_length_ - y_1)/(y_0 - y_1);
                    }

                    if (t_ymax < 0 || t_ymin > 1)
                    {
                        return false;
                    }
                    else
                    {
                        return (t_xmax >= t_ymin && t_xmin <= t_ymax);
                    }
                }
            }

        }


    }
}

bool Parallel_Square::meets_triangle(const Triangle &T) const
{
    Point A, B ,C;
    T.get_ABC(A, B, C);

    if (contains_point(C))
    {
        return true;
    }
    else
    {
        if(meets_segment(A,B))
        {
            return true;
        }
        else if(meets_segment(B,C))
        {
            return true;
        }
        else if(meets_segment(C,A))
        {
            return true;
        }
        else
        {
            double alpha, beta, gamma;
            return T.is_inside(alpha, beta, gamma, Point(x_min_, y_min_));
        }
    }

    return false;
}
