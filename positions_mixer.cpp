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

#include "positions_mixer.hpp"

void Positions_Mixer::reset(Graph<Empty> *abstract_graph, Graph<Circle> *circle_packing_up,
                       Graph<Circle> *circle_packing_down, Graph<Circle> *output_circle_packing,
                       double beta)
{
    abstract_graph_ = abstract_graph;
    circle_packing_up_ = circle_packing_up;
    circle_packing_down_ = circle_packing_down;
    output_circle_packing_ = output_circle_packing;
    beta_ = beta;
    return;
}

void Positions_Mixer::run()
{
    //clock_t t_0 = clock();

    compute_up_weights();
    compute_down_weights();
    mix();

    //clock_t t_1 = clock();
    //std::cout << "Positions mixed in " << (t_1 - t_0)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    return;
}

void Positions_Mixer::compute_up_weights()
{
    unsigned int n = abstract_graph_->nb_vertices();
    up_weights_.clear();
    up_weights_.resize(n);

    std::vector<bool> placements;
    placements.resize(n);
    fill(placements.begin(), placements.end(), false);
    placements[0] = true;
    up_weights_[0] = 0;

    std::vector<vertex_label> N = abstract_graph_->get_neighbors_by_index(0);
    unsigned int one = N[0];
    unsigned int shift = 0;
    integer_coordinates xy, xy_max = abstract_graph_->get_tiling_coordinates_by_index(one);

    unsigned int k;
    for(k=1; k<N.size(); k++)
    {
        xy = abstract_graph_->get_tiling_coordinates_by_index(N[k]);
        if (xy.second > xy_max.second || (xy.second == xy_max.second && xy.first > xy_max.first))
        {
            xy_max = xy;
            one = N[k];
            shift = k;
        }
    }
    abstract_graph_->rotate_neighbors_by_index(0, shift);

    placements[one] = true;
    up_weights_[one] = 0;

    unsigned int nb_positioned = 2;

    N = abstract_graph_->get_neighbors_by_index(0);
    for (k=1; k<N.size(); k++)
    {
        placements[N[k]] = true;
        up_weights_[N[k]] = 1;
        nb_positioned++;
    }

    unsigned int i=0;
    int v_0, v_1;
    while (nb_positioned < n)
    {
        i++;
        if (i==n)
        {
            std::cout << "ERROR in Positions_Mixer::compute_up_weights: not all vertices have been positioned"
                      << std::endl;
            throw(QString("ERROR in Positions_Mixer::compute_up_weights: not all vertices have been positioned"));
        }

        if (!placements[i])
        {
            std::cout << "ERROR in Positions_Mixer::compute_up_weights: vertex "
                      << i << " has not been positioned" << std::endl;
            throw(QString("ERROR in Positions_Mixer::compute_up_weights: vertex not positioned"));
        }

        v_0 = abstract_graph_->get_neighbor_label_by_indices(i,0);
        v_1 = abstract_graph_->get_neighbor_label_by_indices(i,1);

        if (!placements[v_0])
        {
            std::cout << "ERROR in Positions_Mixer::compute_up_weights: the right neighbor of "
                      << i << " (" << v_0 << ") has not been placed" << std::endl;
            throw(QString("ERROR in Positions_Mixer::compute_up_weights: the right neighbor not placed"));
        }

        if (!placements[v_1])
        {
            up_weights_[v_1] = beta_*up_weights_[i] + (1-beta_)*up_weights_[v_0] + 1;
            placements[v_1] = true;
            nb_positioned++;
        }
    }
    return;
}

void Positions_Mixer::compute_down_weights()
{
    unsigned int n = abstract_graph_->nb_vertices();
    down_weights_.clear();
    down_weights_.resize(n);

    std::vector<bool> placements;
    placements.resize(n);
    fill(placements.begin(), placements.end(), false);
    placements[0] = true;
    down_weights_[0] = 0;

    std::vector<vertex_label> N = abstract_graph_->get_neighbors_by_index(0);
    unsigned int one = N[0];
    unsigned int shift = 0;
    integer_coordinates xy, xy_min = abstract_graph_->get_tiling_coordinates_by_index(one);
    unsigned int k;
    for(k=1; k<N.size(); k++)
    {
        xy = abstract_graph_->get_tiling_coordinates_by_index(N[k]);
        if (xy.second < xy_min.second || (xy.second == xy_min.second && xy.first < xy_min.first))
        {
            xy_min = xy;
            one = N[k];
            shift = k;
        }
    }
    abstract_graph_->rotate_neighbors_by_index(0, shift);

    placements[one] = true;
    down_weights_[one] = 0;
    unsigned int nb_positioned = 2;

    N = abstract_graph_->get_neighbors_by_index(0);
    for (k=1; k<N.size(); k++)
    {
        down_weights_[N[k]] = 1;
        placements[N[k]] = true;
        nb_positioned++;
    }

    unsigned int i=n;
    int v_0, v_1;
    while (nb_positioned < n)
    {
        i--;
        if (i==0)
        {
            std::cout << "ERROR in Positions_Mixer::compute_down_weights: not all vertices have been positioned"
                      << std::endl;
            throw(QString("ERROR in Positions_Mixer::compute_down_weights: not all vertices have been positioned"));
            break;
        }

        if (!placements[i])
        {
            std::cout << "ERROR in Positions_Mixer::compute_down_weights: vertex "
                      << i << " has not been positioned" << std::endl;
            throw(QString("ERROR in Positions_Mixer::compute_down_weights: vertex not positioned"));
            break;
        }

        v_0 = abstract_graph_->get_neighbor_label_by_indices(i, 0);
        v_1 = abstract_graph_->get_last_neighbor(i);

        if (!placements[v_0])
        {
            std::cout << "ERROR in Positions_Mixer::compute_compute_down_weights: right neighbor of "
                      << i << " (" << v_0 << ") has not been positioned" << std::endl;

            throw(QString("ERROR in Positions_Mixer::compute_compute_down_weights: right neighbor not placed"));
        }

        if (!placements[v_1])
        {
            down_weights_[v_1] = beta_*down_weights_[v_0] + (1-beta_)*down_weights_[i] + 1;
            placements[v_1] = true;
            nb_positioned++;
        }
    }
    return;
}

void Positions_Mixer::mix()
{
    complex_number affix_downwards, affix_upwards, new_affix;
    double radius_up, radius_down, new_radius;

    *output_circle_packing_ = *circle_packing_up_;

    double u_i, d_i;
    double coeff_up, coeff_down;
    unsigned int i, nb_vertices = output_circle_packing_->nb_vertices();

    if (circle_packing_down_->nb_vertices() != nb_vertices)
    {
        std::cout << "ERROR in Positions_Mixer::mix: wrong number of vertices" << std::endl;
        throw(QString("ERROR in Positions_Mixer::mix: wrong number of vertices"));
        return;
    }

    for(i=0; i<output_circle_packing_->nb_vertices(); i++)
    {
        u_i = up_weights_[i+1];
        d_i = down_weights_[i+1];
        coeff_up = d_i/(d_i + u_i);
        coeff_down = u_i/(d_i + u_i);
        //coeff_up = pow(coeff_up, alpha);
        //coeff_down = pow(coeff_down, alpha);

        affix_downwards = circle_packing_down_->get_affix_by_index(i);
        affix_upwards = circle_packing_up_->get_affix_by_index(i);
        radius_up = circle_packing_up_->get_content_by_index(i).get_radius();
        radius_down = circle_packing_down_->get_content_by_index(i).get_radius();
        new_affix = (coeff_down*affix_downwards + coeff_up*affix_upwards)/(coeff_up+coeff_down);
        new_radius = (coeff_down*radius_down + coeff_up*radius_up)/(coeff_up+coeff_down);
        output_circle_packing_->set_content_by_index(i, Circle(Point(new_affix), new_radius));
    }
    return;
}
