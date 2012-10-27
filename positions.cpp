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

#include "positions.hpp"
#include "mobius.hpp"
#include "canvas.hpp"

#include <QString>
#include <QComboBox>

void Positions::reset(const positions_algo_type positions_algo, const Graph<Empty> &G,
                      const unsigned int &zero, const unsigned int &one,
                      const unsigned int &two, std::vector<double> *ptr_radii,
                      const complex_number &angle, const double &tol,
                      Graph<Circle> *canvas_circle_packing, Canvas* canvas, bool show_live_cp, bool *stop)
{
    positions_algo_ = positions_algo;
    G_ = G;
    zero_ = zero;
    one_ = one;
    two_ = two;
    ptr_radii_ = ptr_radii;
    angle_ = angle;
    
    tol_ = tol;
    canvas_circle_packing_ = canvas_circle_packing;
    canvas_ = canvas;
    show_live_cp_ = show_live_cp;
    
    if (show_live_cp)
    {
        connect(this, SIGNAL(show_live_cp()), canvas_, SLOT(update()));
    }
    
    stop_ = stop;
}

void Positions::run()
{
    unsigned int counter = 0;
    do
    {
        msleep(30);
        radii_ = *ptr_radii_;
        compute_positions();
        if (show_live_cp_)
        {
            synchronise_angles();
            counter++;
            if (counter==2)
            {
                *canvas_circle_packing_ = output_circle_packing_;
                emit show_live_cp();
                counter = 0;
            }
        }
        compute_max_error();
    }
    while (!(error_ < tol_) && !*stop_); // !(error_ < tol_) instead of (error_ > tol_) to include NaN case

    if (!show_live_cp_)
    {
        synchronise_angles();
    }
    
    if (show_live_cp_)
    {
        disconnect(this, SIGNAL(show_live_cp()), canvas_, SLOT(update()));
    }
    return;
}

void Positions::compute_positions()
{
    switch(positions_algo_)
    {
    case REGULAR:
        compute_positions_regular();
        break;
    case DOWNWARDS:
        compute_positions_downwards();
        break;
    case UPWARDS:
        compute_positions_upwards();
        break;
    default:
        std::cout << "ERROR in Positions::compute_positions: flag problem" << std::endl;
        throw(QString("ERROR in Positions::compute_positions: flag problem"));
    }
    
    if (positions_algo_ == REGULAR || positions_algo_ == DOWNWARDS || positions_algo_ == UPWARDS)
    {
        std::vector<Circle> contents;
        unsigned int i;
        for (i=0; i<radii_.size(); i++)
        {
            contents.push_back(Circle(Point(positions_[i]), radii_[i]));
        }
        
        output_circle_packing_ = G_.concretize_graph(contents);
        output_circle_packing_.remove_vertex_by_index(0);
        output_circle_packing_.apply_mobius(Mobius(0.0, 1.0, 1.0, 0.0));
        output_circle_packing_.apply_mobius(output_circle_packing_.optimal_mobius());
    }

    
    /*clock_t t0 = clock();
    std::cout << "Max position error: " << compute_max_error();
    clock_t t1 = clock();
    std::cout << " (computed in " << (t1 - t0)*1.0/CLOCKS_PER_SEC << "s)" << std::endl;*/
    return;
}

const Graph<Circle> & Positions::get_output_circle_packing() const
{
    return output_circle_packing_;
}

void Positions::synchronise_angles()
{
    complex_number u_left = angle_;
    complex_number u_right = output_circle_packing_.direction_from_center_of_mass(0);
    
    Mobius M(conj(u_right)*u_left, 0.0, 0.0, 1.0);
    output_circle_packing_.apply_mobius(M);
    return;
}

const double & Positions::get_error() const
{
    return error_;
}


void Positions::compute_positions_regular()
{
    int n = radii_.size();
    positions_.resize(n);
    positions_[0] = complex_number(0.0, 0.0);
    positions_[one_] = complex_number(radii_[0]+radii_[one_],0);
    
    std::vector<bool> placements;
    placements.resize(n);
    fill( placements.begin(), placements.end(), false );
    placements[0] = true;
    placements[one_] = true;
    
    int nb_positioned = 2;
    int j;
    
    std::vector<int> N = G_.get_neighbors_by_index(0);
    
    unsigned int k;
    for (k=1; k<N.size(); k++)
    {
        positions_[N[k]] = compute_next_position(
                    positions_[0], positions_[N[k-1]], radii_[0], radii_[N[k-1]], radii_[N[k]], 1);
        placements[N[k]] = true;
        nb_positioned++;
    }
    
    unsigned int i=0;
    while (nb_positioned < n)
    {
        i++;
        i %= n;
        if (!placements[i])
        {
            continue;
        }
        N = G_.get_neighbors_by_index(i);
        j = 1;
        while (j< (int) N.size() && !(placements[N[j]]))
        {
            j++;
        }
        if (j== (int) N.size())
        {
            continue;
        }
        for (k=1; k<N.size(); k++)
        {
            if (placements[N[(j+k)%N.size()]])
            {
                continue;
            }
            positions_[N[(j+k)%N.size()]] = compute_next_position(
                        positions_[i], positions_[N[(j+k-1)%N.size()]],
                        radii_[i], radii_[N[(j+k-1)%N.size()]], radii_[N[(j+k)%N.size()]], 1);
            placements[N[(j+k)%N.size()]] = true;
            nb_positioned++;
        }
    }
    
    return;
}

void Positions::compute_positions_upwards()
{
    unsigned int n = radii_.size();
    positions_.resize(n);
    positions_[0] = complex_number(0.0, 0.0);
    positions_[one_] = complex_number(radii_[0]+radii_[one_], 0.0);
    
    std::vector<bool> placements;
    placements.resize(n);
    fill(placements.begin(), placements.end(), false);
    placements[0] = true;
    placements[one_] = true;
    
    unsigned int nb_positioned = 2;
    
    std::vector<vertex_label> N = G_.get_neighbors_by_index(0);
    
    unsigned int k;
    for (k=1; k<N.size(); k++)
    {
        positions_[N[k]] = compute_next_position(
                    positions_[0], positions_[N[k-1]], radii_[0], radii_[N[k-1]], radii_[N[k]], 1);
        placements[N[k]] = true;
        nb_positioned++;
    }
    
    unsigned int i=0;
    int v_0, v_1;
    while (nb_positioned < n)
    {
        i++;
        if (i==n)
        {
            std::cout << "ERROR in Positions::compute_positions_upwards: not all vertices have been positioned"
                      << std::endl;
            throw(QString("ERROR in Positions::compute_positions_upwards: not all vertices have been positioned"));
        }
        
        if (!placements[i])
        {
            std::cout << "ERROR in Positions::compute_positions_upwards: vertex "
                      << i << " has not been positioned" << std::endl;
            throw(QString("ERROR in Positions::compute_positions_upwards: vertex not positioned"));
        }
        
        v_0 = G_.get_neighbor_label_by_indices(i,0);
        v_1 = G_.get_neighbor_label_by_indices(i,1);
        
        if (!placements[v_0])
        {
            std::cout << "ERROR in Positions::compute_positions_upwards: the right neighbor of "
                      << i << " (" << v_0 << ") has not been placed" << std::endl;
            throw(QString("ERROR in Positions::compute_positions_upwards: the right neighbor not placed"));
        }
        
        if (!placements[v_1])
        {
            
            positions_[v_1] = compute_next_position(
                        positions_[i], positions_[v_0], radii_[i], radii_[v_0], radii_[v_1], 1);
            placements[v_1] = true;
            nb_positioned++;
        }
    }
    
    return;
}

void Positions::compute_positions_downwards()
{
    unsigned int n = radii_.size();
    positions_.resize(n);
    positions_[0] = complex_number(0.0,0.0);
    positions_[one_] = complex_number(radii_[0]+radii_[one_],0.0);
    
    std::vector<bool> placements;
    placements.resize(n);
    fill(placements.begin(), placements.end(), false);
    placements[0] = true;
    placements[one_] = true;
    unsigned int nb_positioned = 2;
    
    std::vector<vertex_label> N = G_.get_neighbors_by_index(0);
    
    //std::cout << "Placing zero neighbors: ";
    unsigned int k;
    for (k=1; k<N.size(); k++)
    {
        positions_[N[k]] = compute_next_position(
                    positions_[0], positions_[N[k-1]], radii_[0], radii_[N[k-1]], radii_[N[k]], 1);
        placements[N[k]] = true;
        //std::cout << N[k] << " ";
        nb_positioned++;
    }
    
    //std::cout << std::endl << "Placing the rest: ";
    unsigned int i=n;
    int v_0, v_1;
    while (nb_positioned < n)
    {
        i--;
        if (i==0)
        {
            std::cout << "ERROR in Positions::compute_positions_downwards: not all vertices have been positioned"
                      << std::endl;
            throw(QString("ERROR in Positions::compute_positions_downwards: not all vertices have been positioned"));
            break;
        }
        
        if (!placements[i])
        {
            std::cout << "ERROR in Positions::compute_positions_downwards: vertex "
                      << i << " has not been positioned" << std::endl;
            throw(QString("ERROR in Positions::compute_positions_downwards: vertex not positioned"));
            break;
        }
        
        v_0 = G_.get_neighbor_label_by_indices(i, 0);
        v_1 = G_.get_last_neighbor(i);
        
        if (!placements[v_0])
        {
            std::cout << "ERROR in Positions::compute_positions_downwards: right neighbor of "
                      << i << " (" << v_0 << ") has not been positioned" << std::endl;
            
            throw(QString("ERROR in Positions::compute_positions_downwards: right neighbor not positioned"));
        }
        
        if (!placements[v_1])
        {
            
            positions_[v_1] = compute_next_position(positions_[v_0], positions_[i], radii_[v_0], radii_[i], radii_[v_1], 1);
            placements[v_1] = true;
            
            nb_positioned++;
            //std::cout << v_1 << " ";
        }
    }
    //std::cout << std::endl;
    
    return;
}

double Positions::compute_single_error(const unsigned int &index) const
{
    if (index >= output_circle_packing_.nb_vertices())
    {
        std::cout << "ERROR in Positions::compute_single_error: bad index" << std::endl;
        throw(QString("ERROR in Positions::compute_single_error: bad index"));
    }
    complex_number z = output_circle_packing_.get_affix_by_index(index);
    double r = output_circle_packing_.get_content_by_index(index).get_radius();
    
    
    
    std::vector<vertex_label> N = output_circle_packing_.get_neighbors_by_index(index);
    
    complex_number z_i = output_circle_packing_.get_affix_by_index(N[0]-1);
    double r_i = output_circle_packing_.get_content_by_index(N[0]-1).get_radius();
    double max = std::abs(norm(z_i - z) - (r_i+r)*(r_i+r)), e;
    unsigned int i;
    for (i=1; i<N.size(); i++)
    {
        if (N[i] < (int) index + 1)
        {
            continue;
        }
        z_i = output_circle_packing_.get_affix_by_index(N[i]-1);
        r_i = output_circle_packing_.get_content_by_index(N[i]-1).get_radius();
        e = std::abs(norm(z_i - z) - (r_i+r)*(r_i+r));
        if (e > max)
        {
            max = e;
            //u = N[i];
        }
    }
    //std::cout << "index = " << index << ", error max = " << max << " for neighbor " << u << std::endl;
    return max;
}

void Positions::compute_max_error()
{
    double error, max = compute_single_error(0);
    unsigned int i;
    for (i=1; i<output_circle_packing_.nb_vertices(); i++)
    {
        error = compute_single_error(i);
        if (error > max)
        {
            max = error;
        }
    }
    
    error_ = sqrt(max);
    return;
}

complex_number compute_next_position(const complex_number &pivot_affix, const complex_number &neighbor_affix,
                                     const double &pivot_radius, const double &neighbor_radius, const double &next_radius,
                                     const int &orientation)
{
    double a = pivot_radius + neighbor_radius;
    double b = pivot_radius + next_radius;
    double c = neighbor_radius + next_radius;
    double x = (a*a+b*b-c*c)/(2*a);
    double y = sqrt(b*b-x*x);
    complex_number u = neighbor_affix - pivot_affix;
    
    return pivot_affix + complex_number(x, y*orientation) * u/abs(u);
}
