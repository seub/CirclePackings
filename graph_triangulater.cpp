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

#include "graph_triangulater.hpp"
#include "graph_tiler.hpp"
#include "triangulation_search_tree.hpp"

void Graph_Triangulater::reset(Graph<Circle>* circle_packing, Triangulation *triangulation, Graph<Empty> *abstract_graph,
                               Curve *curve, Triangulation_Search_Tree *triangulation_search_tree,
                               bool create_search_tree,
                               bool *valid, complex_number *angle,
                               Graph<Circle> *tiling_graph, Triangulation_coloring coloring)
{
    circle_packing_ = circle_packing;
    triangulation_ = triangulation;
    abstract_graph_ = abstract_graph;
    triangulations_search_tree_ = triangulation_search_tree;
    create_search_tree_ = create_search_tree;
    valid_ = valid;
    angle_ = angle;
    tiling_graph_ = tiling_graph;
    curve_ = curve;
    coloring_ = coloring;
    return;
}

void Graph_Triangulater::run()
{
    //std::cout << "Entering Graph_Triangulater::run" << std::endl;

    *valid_ = create_circle_and_triangle_packing();
    return;
}

bool Graph_Triangulater::create_circle_and_triangle_packing()
{
    //std::cout << "Entering Graph_Triangulater::create_circle_and_triangle_packing" << std::endl;

    Graph_Tiler GT;
    GT.reset_for_circle_packing(tiling_graph_, circle_packing_, abstract_graph_, angle_, curve_);
    bool res = GT.create_circle_packing();

    create_triangle_packing(res);

    triangulation_->color(coloring_);
    return res;
}

void Graph_Triangulater::create_triangle_packing(bool res)
{
    //std::cout << "Entering Graph_Triangulater::create_triangle_packing" << std::endl;

    bool zero_neighbors_success = false;

    triangulation_->reset_triangulation();
    unsigned int N = circle_packing_->nb_vertices();
    if (N > 0)
    {
        add_interior_points();
    }
    if (N > 1 && res)
    {
        zero_neighbors_success = add_exterior_points();
    }
    if (N > 2)
    {
        add_interior_triangles();
    }
    if (N > 1 && res && zero_neighbors_success)
    {
        add_exterior_triangles();
    }

    if (create_search_tree_)// && zero_neighbors_success && res)
    {
        triangulations_search_tree_->reset_search_tree(triangulation_);
    }
    return;
}

void Graph_Triangulater::add_interior_points()
{
    unsigned int i, N = circle_packing_->nb_vertices();
    for (i=0; i<N; i++)
    {
        triangulation_->add_point(circle_packing_->get_content_by_index(i).get_centre(),
                                  circle_packing_->get_tiling_coordinates_by_index(i));
    }
    triangulation_->nb_interior_points_ = N;
    return;
}

bool Graph_Triangulater::create_directions()
{
    std::vector<vertex_label> N = abstract_graph_->get_neighbors_by_index(0);
    directions_.clear();
    directions_.reserve(N.size());

    unsigned int index, prev_index;
    integer_coordinates prev_xy, xy;
    index = N.back() - 1;
    xy = circle_packing_->get_tiling_coordinates_by_index(index);
    unsigned int j;
    for (j=0; j<N.size(); j++)
    {
        prev_xy = xy;
        prev_index = index;
        index = N[j] - 1;
        xy = circle_packing_->get_tiling_coordinates_by_index(index);
        if (xy.first==prev_xy.first+1 && xy.second==prev_xy.second)
        {
            directions_.push_back(0);
        }
        else if (xy.first==prev_xy.first+(prev_xy.second%2==0) && xy.second==prev_xy.second+1)
        {
            directions_.push_back(1);
        }
        else if (xy.first==prev_xy.first+(prev_xy.second%2==0)-1 && xy.second==prev_xy.second+1)
        {
            directions_.push_back(2);
        }
        else if (xy.first==prev_xy.first-1 && xy.second==prev_xy.second)
        {
            directions_.push_back(3);
        }
        else if (xy.first==prev_xy.first-(prev_xy.second%2==1) && xy.second==prev_xy.second-1)
        {
            directions_.push_back(4);
        }
        else if (xy.first==prev_xy.first-(prev_xy.second%2==1)+1 && xy.second==prev_xy.second-1)
        {
            directions_.push_back(5);
        }
        else
        {
            if (!circle_packing_->are_neighbor_indices(index, prev_index))
            {
                std::cout << "ERROR in Graph_Triangulater::create_directions: zero neighbors problem" << std::endl;        
                throw(QString("ERROR in Graph_Triangulater::create_directions: zero neighbors problem"));
            }
            else
            {
                std::cout << "ERROR in Graph_Triangulater::create_directions: never supposed to reach here" << std::endl;
                throw(QString("ERROR in Graph_Triangulater::create_directions: never supposed to reach here"));
            }
            return false;
        }
    }
    return true;
}

void Graph_Triangulater::create_turns()
{
    turns_.clear();
    turns_.reserve(directions_.size());

    int direction;
    int next_direction = directions_.front();

    unsigned int j;
    for (j=1; j<directions_.size(); j++)
    {
        direction = next_direction;
        next_direction = directions_[j];
        turns_.push_back((6 + next_direction - direction) % 6);
    }
    direction = next_direction;
    next_direction = directions_.front();
    turns_.push_back((6 + next_direction - direction) % 6);
    return;
}

bool Graph_Triangulater::add_exterior_points()
{
    //std::cout << "Entering add Graph_Triangulater::add_exterior_points()" << std::endl;

    if(!create_directions())
    {
        return false;
    }

    create_turns();
    std::vector<vertex_label> N = abstract_graph_->get_neighbors_by_index(0);
    Point projection;
    integer_coordinates xy;
    complex_number z_g, z_d, z, u_3(0.5, sqrt(3.)/2);
    unsigned int i;
    for (i=0; i<N.size(); i++)
    {
        xy = triangulation_->get_tiling_coordinates(N[i]-1);
        z_g = circle_packing_->get_affix_by_index(N[i] - 1);
        z_d = (i+1 == N.size()) ? circle_packing_->get_affix_by_index(N.front() - 1) :
                                  circle_packing_->get_affix_by_index(N[i+1] - 1);
        z = z_g + u_3 * (z_d-z_g);
        switch(turns_[i])
        {
        case 0:
            curve_->point_projection(projection, Point(z));
            triangulation_->add_point(projection, neighbor_tiling_coordinates(xy,(directions_[i]+turns_[i]+1)%6));
            break;
        case 1:
            break;
        case 4:
            curve_->point_projection(projection,Point(z_g + (z_g - z_d)));
            triangulation_->add_point(projection, neighbor_tiling_coordinates(xy,(directions_[i]+turns_[i]+3)%6));
            curve_->point_projection(projection,Point(z_g + u_3 * u_3 * (z_d-z_g)));
            triangulation_->add_point(projection, neighbor_tiling_coordinates(xy,(directions_[i]+turns_[i]+2)%6));
            curve_->point_projection(projection, Point(z));
            triangulation_->add_point(projection, neighbor_tiling_coordinates(xy,(directions_[i]+turns_[i]+1)%6));
            break;
        case 5:
            curve_->point_projection(projection,Point(z_g + u_3 * u_3 * (z_d-z_g)));
            triangulation_->add_point(projection, neighbor_tiling_coordinates(xy,(directions_[i]+turns_[i]+2)%6));
            curve_->point_projection(projection, Point(z));
            triangulation_->add_point(projection, neighbor_tiling_coordinates(xy,(directions_[i]+turns_[i]+1)%6));
            break;
        default:
            std::cout << "ERROR in Graph_Triangulater::add_exterior_points: turning problem" << std::endl;
            throw(QString("ERROR in Graph_Triangulater::add_exterior_points: turning problem"));
        }
    }
    return true;
}

void Graph_Triangulater::add_interior_triangles()
{
    integer_coordinates xy_i, xy_j;
    std::vector<vertex_label> N;
    int y_max = circle_packing_->get_tiling_coordinates_by_index(circle_packing_->nb_vertices()-1).second;
    unsigned int first_neighbor, second_neighbor, third_neighbor;
    bool first_neighbor_found, second_neighbor_found, third_neighbor_found;
    unsigned int i, j;
    for (i=0; i<circle_packing_->nb_vertices(); i++)
    {
        xy_i = circle_packing_->get_tiling_coordinates_by_index(i);
        if (xy_i.second == y_max)
        {
            break;
        }

        N=circle_packing_->get_neighbors_by_index(i);

        first_neighbor_found = false;
        second_neighbor_found = false;
        third_neighbor_found = false;
        for (j=0; j<N.size(); j++)
        {
            xy_j = circle_packing_->get_tiling_coordinates_by_index(N[j]-1);
            if (xy_j.first==xy_i.first+1 && xy_j.second==xy_i.second)
            {
                first_neighbor_found = true;
                first_neighbor = N[j] - 1;
            }
            else if (xy_j.first==xy_i.first+(xy_i.second%2==0) && xy_j.second==xy_i.second+1)
            {
                second_neighbor_found = true;
                second_neighbor = N[j] - 1;
            }
            else if (xy_j.first==xy_i.first+(xy_i.second%2==0)-1 && xy_j.second==xy_i.second+1)
            {
                third_neighbor_found = true;
                third_neighbor = N[j] - 1;
            }
        }

        if (first_neighbor_found && second_neighbor_found)
        {
            triangulation_->add_triangle(i, first_neighbor, second_neighbor);
        }
        if (second_neighbor_found && third_neighbor_found)
        {
            triangulation_->add_triangle(i, second_neighbor, third_neighbor);
        }
    }
    triangulation_->nb_interior_triangles_ = triangulation_->nb_triangles();
    return;
}

void Graph_Triangulater::add_exterior_triangles()
{
    unsigned int n = circle_packing_->nb_vertices();
    unsigned int nb_points = triangulation_->nb_points();
    std::vector<vertex_label> N = abstract_graph_->get_neighbors_by_index(0);
    unsigned int i_1, i_2, i_3, j_1, j_2, j_3;
    unsigned int point_added_index = n;
    unsigned int i,next_i;
    integer_coordinates xy_g,xy_d;
    xy_d = circle_packing_->get_tiling_coordinates_by_index(N[0] - 1);
    bool kill_last = false;
    for (i=0; i<N.size(); i++)
    {
        next_i = (i+1)>=N.size()?0:(i+1);
        xy_g = xy_d;
        xy_d = circle_packing_->get_tiling_coordinates_by_index(N[next_i] - 1);

        switch(turns_[i])
        {
        case 0:
            i_1 = N[i] - 1;
            i_2 = N[next_i] - 1;
            i_3 = point_added_index;
            j_1 = N[next_i] - 1;
            j_2 = (point_added_index+1>nb_points-1)?n:point_added_index+1;
            j_3 = point_added_index;
            triangulation_->add_triangle(i_1, i_2, i_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            triangulation_->add_triangle(j_1, j_2, j_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            point_added_index++;
            break;
        case 1:
            if(i!=0)
            {
                triangulation_->triangles_.pop_back();
            }
            else
            {
                kill_last = true;
            }
            i_1 = N[i] - 1;
            i_2 = N[next_i] - 1;
            i_3 = (point_added_index-1>=n)?point_added_index-1:nb_points-1;
            j_1 = N[next_i] - 1;
            j_2 = (point_added_index+1>nb_points-1)?n:point_added_index;
            j_3 = (point_added_index-1>=n)?point_added_index-1:nb_points-1;
            triangulation_->add_triangle(i_1, i_2, i_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            triangulation_->add_triangle(j_1, j_2, j_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            break;
        case 4:
            i_1 = N[i] - 1;
            i_2 = point_added_index;
            i_3 = point_added_index + 1;
            j_1 = N[i] - 1;
            j_2 = point_added_index + 1;
            j_3 = point_added_index+2;
            triangulation_->add_triangle(i_1, i_2, i_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            triangulation_->add_triangle(j_1, j_2, j_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);

            i_1 = N[i] - 1;
            i_2 = N[next_i] - 1;
            i_3 = point_added_index + 2;
            j_1 = N[next_i] - 1;
            j_2 = point_added_index + 2;
            j_3 = (point_added_index+3>nb_points-1)?n:point_added_index+3;
            triangulation_->add_triangle(i_1, i_2, i_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            triangulation_->add_triangle(j_1, j_2, j_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            point_added_index+=3;
            break;
        case 5:
            j_1 = N[i] - 1;
            j_2 = point_added_index + 1;
            j_3 = point_added_index;
            triangulation_->add_triangle(j_1, j_2, j_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            i_1 = N[i] - 1;
            i_2 = N[next_i] - 1;
            i_3 = point_added_index + 1;
            j_1 = N[next_i] - 1;
            j_2 = point_added_index + 1;
            j_3 = (point_added_index+2>nb_points-1)?n:point_added_index+2;

            triangulation_->add_triangle(i_1, i_2, i_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            triangulation_->add_triangle(j_1, j_2, j_3);
            triangulation_->cyclic_sort_triangle(triangulation_->nb_triangles()-1);
            point_added_index+=2;
            break;
        default:
            std::cout << "ERROR in Graph_Triangulater::add_exterior_points: turning problem";
            throw(QString("ERROR in Graph_Triangulater::add_exterior_points: turning problem"));
        }
    }
    if(kill_last)
    {
        triangulation_->triangles_.pop_back();
    }
    return;
}

integer_coordinates Graph_Triangulater::neighbor_tiling_coordinates(
        const integer_coordinates &tiling_coordinates, int neighbor_index) const
{

    integer_coordinates xy;

    switch(neighbor_index)
    {
    case 0:
        xy.first=tiling_coordinates.first+1;
        xy.second=tiling_coordinates.second;
        break;

    case 1:
        xy.first=tiling_coordinates.first+(tiling_coordinates.second%2==0);
        xy.second=tiling_coordinates.second+1;
        break;

    case 2:
        xy.first=tiling_coordinates.first+(tiling_coordinates.second%2==0)-1;
        xy.second=tiling_coordinates.second+1;
        break;

    case 3:
        xy.first=tiling_coordinates.first-1;
        xy.second=tiling_coordinates.second;
        break;

    case 4:
        xy.first=tiling_coordinates.first-(tiling_coordinates.second%2==1);
        xy.second=tiling_coordinates.second-1;
        break;
    case 5:
        xy.first=tiling_coordinates.first-(tiling_coordinates.second%2==1)+1;
        xy.second=tiling_coordinates.second-1;
        break;

    default:
        std::cout << "ERROR in Graph_Triangulater::neighbor_tiling_coordinates: bad index" << std::endl;
        throw(QString("ERROR in Graph_Triangulater::neighbor_tiling_coordinates: bad index"));
    }

    return xy;
}
