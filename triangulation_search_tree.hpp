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

#ifndef TRIANGULATION_SEARCH_TREE_HPP
#define TRIANGULATION_SEARCH_TREE_HPP


#include "basic_types.hpp"
#include "point.hpp"
#include "curve.hpp"
#include "triangle.hpp"
#include "triangulation.hpp"

class Triangulation_Search_Tree
{
public:
    Triangulation_Search_Tree();
    Triangulation_Search_Tree(const std::vector<unsigned int> &indices,
                const Point &center,
                double radius, unsigned int depth, Triangulation *triangulation);
    ~Triangulation_Search_Tree();
    bool get_containing_triangle_index(double &alpha, double &beta, double &gamma,
                                       unsigned int &output_index, const Point &point) const;
    void reset_search_tree(Triangulation *triangulation);
    void cout_tree();

private :
    Triangulation *triangulation_;
    unsigned int which_quarter(const Point &point) const;
    void splitting_indices_list(const std::vector<unsigned int> &indices,
                             std::vector<unsigned int> &ru_indices,
                             std::vector< unsigned int> &lu_indices,
                             std::vector< unsigned int> &ld_indices,
                             std::vector< unsigned int> &rd_indices) const;
    unsigned int depth_;
    Point center_;
    double radius_;
    Triangulation_Search_Tree * ru_subtree_;
    Triangulation_Search_Tree * lu_subtree_;
    Triangulation_Search_Tree * ld_subtree_;
    Triangulation_Search_Tree * rd_subtree_;
    std::vector<unsigned int> triangles_;
    bool leaf_;
    unsigned int max_depth_;
    unsigned int nb_max_triangles_leaf_;
};

class Parallel_Square
{
public:
    Parallel_Square(double x_min, double y_min, double side_length) : x_min_(x_min), y_min_(y_min), side_length_(side_length) {}
    bool meets_triangle(const Triangle &T) const;

    bool contains_point(const Point &point) const;
    bool meets_segment(const Point &first_endpoint, const Point &second_endpoint) const;

private:
    double x_min_;
    double y_min_;
    double side_length_;

};

#endif // TRIANGULATION_SEARCH_TREE_HPP
