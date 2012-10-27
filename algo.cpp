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

#include <time.h>

#include "algo.hpp"
#include "positions.hpp"

Algo::Algo()
{
    delta_ = 0.0658;
}

void Algo::reset_algo(Graph<Empty> &G, complex_number &angle, const radii_algo_type &radii_algo,
                      const positions_algo_type &positions_algo, const double &tol_radii, const double &tol_positions,
                      Graph<Circle> *canvas_circle_packing, Canvas *canvas, bool show_live_cp)
{
    unsigned int i;

    G_ = G;
    angle_ = angle;

    radii_.resize(G_.nb_vertices());
    std::fill(radii_.begin(),radii_.end(), 1.0);

    delta_ = 0.0658;
    tol_radii_ = tol_radii;
    tol_positions_ = tol_positions;
    radii_algo_ = radii_algo;
    positions_algo_ = positions_algo;
    choose_zero_one_two();

    neighbors_lists_sizes_.clear();
    neighbors_lists_sizes_.reserve(G_.nb_vertices());


    for (i=0; i<G_.nb_vertices(); i++)
    {
        neighbors_lists_sizes_.push_back(G_.get_neighbors_by_index(i).size());
    }

    create_sines();

    canvas_circle_packing_ = canvas_circle_packing;
    canvas_ = canvas;
    show_live_cp_ = show_live_cp;

    return;
}

void Algo::create_sines()
{
    unsigned int nb_neighbors, nb_neighbors_max;
    unsigned int i, j;
    sin_pi_over_n_.clear();

    nb_neighbors_max = 0;


    for (i=1; i<G_.nb_vertices(); i++)
    {
        if (i == one_ || i == two_)
        {
            continue;
        }
        else
        {
            nb_neighbors = neighbors_lists_sizes_[i];
            if (nb_neighbors > nb_neighbors_max)
            {
                nb_neighbors_max = nb_neighbors;
            }
        }
    }

    for (j=1; j<=nb_neighbors_max; j++)
    {
        sin_pi_over_n_.push_back(sin(M_PI/(1.0*j)));
    }

    return;
}

const Graph<Circle> & Algo::get_ouput_circle_packing() const
{
    return output_circle_packing_;
}

const double & Algo::get_radii_error() const
{
    return error_;
}

const double & Algo::get_positions_error() const
{
    return P_.get_error();
}

void Algo::choose_zero_one_two()
{
    unsigned int k;

    zero_ = 0;

    if (positions_algo_==REGULAR || positions_algo_==DOWNWARDS)
    {
        one_ = G_.get_neighbors_by_index(0).front();
    }
    else if (positions_algo_ == UPWARDS)
    {
        one_ = G_.nb_vertices()-1;

        if (G_.get_neighbor_index_by_label(k, 0, one_))
        {
            G_.rotate_neighbors_by_index(0, k);
        }
        else
        {
            std::cout << "ERROR in void Algo::choose_zero_one_two: one is not a zero neighbor" << std::endl;
            throw(QString("ERROR in void Algo::choose_zero_one_two: one is not a zero neighbor"));
        }
    }
    else
    {
        std::cout << "ERROR in void Algo::choose_zero_one_two: flag problem" << std::endl;
        throw(QString("ERROR in void Algo::choose_zero_one_two: flag problem"));
    }

    two_ = G_.get_neighbors_by_index(0).back();
    return;
}

void Algo::compute_radii()
{
    switch(radii_algo_)
    {
    case STANDARD:
        standard_compute_radii();
        break;

    case SUPER:
        super_compute_radii();
        break;

    default:
        std::cout << "ERROR in Algo::compute_radii: flag problem" << std::endl;
        throw(QString("ERROR in Algo::compute_radii: flag problem"));
    }
    return;
}

double Algo::total_neighbors_angle(const unsigned int &index)
{

    std::vector<vertex_label> N;
    double res=0.;
    unsigned int i = 0, v;
    double a,b,c,t;

    N = G_.get_neighbors_by_index(index);
    v = N.size();


    for (i=0; i<v; i++)
    {
        a = radii_[N[i]] + radii_[index];
        b = radii_[N[(i+1)%v]] + radii_[index];
        c = radii_[N[i]] + radii_[N[(i+1)%v]];
        t = (a*a + b*b - c*c)/(2*a*b);
        if (t>1)
        {
            std::cout << "WARNING in Algo::total_neighbors_angle: cannot take acos of " << t << std::endl;
        }
        else if (t<-1)
        {
            res += M_PI;
            std::cout << "WARNING in Algo::total_neighbors_angle: cannot take acos of " << t << std::endl;
        }
        else
        {
            res += acos(t);
        }
    }
    return res;
}

double Algo::standard_radius_update(const unsigned int &index)
{
    int n;
    double s;
    double rv;

    n = G_.get_neighbors_by_index(index).size();
    s = sin(total_neighbors_angle(index)*0.5/n);
    rv = radii_[index]*s/(1-s);
    s = sin(M_PI/n);
    return (1-s)*rv/s;
}

void Algo::standard_radii_update()
{
    unsigned int i, n;
    std::vector<double> res;

    n = radii_.size();
    res.reserve(n);
    res.push_back(1.0);

    for(i=1; i<n; i++)

        if(i!= one_ && i!= two_)
        {
            res.push_back(standard_radius_update(i));
        }
        else
        {
            res.push_back(1.0);
        }
    radii_ = res;
    return;
}

void Algo::compute_error()
{
    double res = 0;
    double temp;
    unsigned int i = 1;

    while (i< radii_.size())
    {
        if (i!=one_ && i!=two_)
        {
            temp = (2*M_PI - total_neighbors_angle(i));
            res += temp*temp;
        }
        i++;
    }
    error_ = sqrt(res);
    return;
}

double Algo::super_total_neighbors_angle(unsigned int index, double &error)
{
    std::vector<vertex_label> N = G_.get_neighbors_by_index(index);
    double vars[5]; // res, r_left, r_right, r, t
    vars[0] = 0;
    vars[3]= radii_[index];
    vars[2] = radii_[N.back()];


    unsigned int v = N.size();
    for (unsigned int i=0; i<v; i++)
    {
        vars[1] = vars[2];
        vars[2] = radii_[N[i]];
        vars[4] = 1 - 2*vars[1]*vars[2]/((vars[1]+vars[3])*(vars[2]+vars[3]));
        if (vars[4]<1 && vars[4]>-1)
        {
            vars[0] += acos(vars[4]);
        }
        else if (vars[4]<-1)
        {
            vars[0] += M_PI;
            //std::cout << "WARNING in Algo::super_total_neighbors_angle: cannot take acos of " << t << std::endl;
        }
        /*else
        {
            //std::cout << "WARNING in Algo::super_total_neighbors_angle: cannot take acos of " << t << std::endl;
        }*/
    }

    error += (vars[0]-2*M_PI)*(vars[0]-2*M_PI);
    return vars[0];
}


void Algo::standard_compute_radii()
{
    unsigned int k = 0, counter = 0;
    double tol;

    tol = tol_radii_;

    //std::cout << "one_ = " << one_ << ", two_ = " << two_ << std::endl << std::endl;
    while (!stop_)
    {
        counter++;
        standard_radii_update();
        k++;
        if (k==128)
        {
            compute_error();
            if (error_<tol)
            {
                stop();
                break;
            }
            k=0;
        }
    }

    nb_iterations_ = counter;
    return;
}

void Algo::super_compute_radii()
{
    double tol;
    double error, error_0;
    double lambda = -1.0, lambda_0, lambda_1;
    bool flag = false, flag_0;
    unsigned int i, k=0, counter=0;
    unsigned int n;
    std::vector<double> radii_save;   
    double delta;


    tol = tol_radii_;
    error = 1 + tol;
    n = radii_.size();
    radii_save.resize(n);
    delta = delta_;



    while(!stop_)
    {
        counter++;
        k++;
        k %= 3;
        error_0 = error;
        flag_0 = flag;
        lambda_0 = lambda;

        radii_save = radii_;
        super_radii_update(error);
        error_=error;

        if (error<tol)
        {
            stop();
            break;
        }

        if(k==0)
        {
            lambda = error/error_0;
            flag = true;
            if(flag_0 && lambda<1.0)
            {
                error = lambda*error;
                if(fabs(lambda-lambda_0)<delta) // (error<delta?error:delta))
                {
                    lambda /= (1.0-lambda);
                }
                lambda_1 = compute_lambda(radii_save,lambda);
                lambda = (lambda<lambda_1)?lambda:lambda_1;

                for(i=1; i<n; i++)
                {
                    if (i== one_ || i==two_)
                    {
                        continue;
                    }
                    radii_[i] += lambda*(radii_[i]-radii_save[i]);
                }
                flag=false;
            }
        }
    }

    nb_iterations_ = counter;
    return;
}

double Algo::compute_lambda(const std::vector<double> &radii_0, const double &lambda)
{
    double lambda_0 = -1.0, lambda_1;
    unsigned int i;

    for(i=1; i< radii_.size(); i++)
    {
        if (i==one_ || i==two_)
        {
            continue;
        }
        if(radii_0[i] == radii_[i])
        {
            continue;
        }

        lambda_1 = radii_[i]/(radii_0[i]-radii_[i]);
        if(lambda_1 < 0)
        {
            continue;
        }
        else if(lambda_0 <0)
        {
            lambda_0 = lambda_1;
        }
        else
        {
            lambda_0 = (lambda_0<lambda_1)?lambda_0:lambda_1;
        }
    }

    if(lambda_0 <0.0)
    {
        return lambda;
    }
    else
    {
        return lambda_0/2;
    }
}

void Algo::super_radii_update(double &error)
{
    std::vector<double> Vres;
    unsigned int i;

    error = 0;
    Vres.reserve(radii_.size());
    Vres.push_back(1.0);

    for(i=1; i<radii_.size(); i++)
    {
        if(i!=one_ && i!=two_)
        {
            Vres.push_back(super_radius_update(i, error));
        }
        else
        {
            Vres.push_back(1.0);
        }
    }

    error = sqrt(error);
    radii_ = Vres;
    return;
}

void Algo::stop()
{
    stop_ = true;
    return;
}

void Algo::stop_computing(bool b)
{
    stop_ = b;
    return;
}

void Algo::set_delta(const double &delta)
{
    delta_ = delta;
    return;
}

const double & Algo::get_time() const
{
    return time_;
}

const unsigned int & Algo::get_nb_iterations() const
{
    return nb_iterations_;
}

void Algo::run()
{

    clock_t t0, t1;

    stop_ = false;
    t0 = clock();
    P_.reset(positions_algo_, G_, zero_, one_, two_, &radii_, angle_, tol_positions_,
             canvas_circle_packing_, canvas_, show_live_cp_, &stop_);
    connect(&P_, SIGNAL(finished()), this, SLOT(stop_computing()));
    P_.start();
    compute_radii();
    P_.wait();
    t1 = clock();
    time_ = (t1-t0)*1.0/CLOCKS_PER_SEC;
    output_circle_packing_ = P_.get_output_circle_packing();
    return;
}

double Algo::super_radius_update(const unsigned int &index, double &error)
{
    double vars[2]; // s, rv
    int n = neighbors_lists_sizes_[index];
    vars[0] = sin(super_total_neighbors_angle(index, error)*0.5/n);
    vars[1] = radii_[index]*vars[0]/(1-vars[0]);
    vars[0] = sin_pi_over_n_[n-1];
    return (1-vars[0])*vars[1]/vars[0];
}

