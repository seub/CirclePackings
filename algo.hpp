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

#ifndef ALGO_H
#define ALGO_H

#include <QThread>

#include "basic_types.hpp"
#include "graph.hpp"
#include "positions.hpp"

class QComboBox;


class Algo : public QThread
{
    Q_OBJECT

    friend class Tests;

public:
    Algo();

    void compute_radii();

    const Graph<Circle> & get_ouput_circle_packing() const;
    const double & get_radii_error() const;
    const double & get_positions_error() const;
    const double & get_time() const;
    const unsigned int & get_nb_iterations() const;


    void set_delta(const double &delta);
    void reset_algo(Graph<Empty> &G, complex_number &angle, const radii_algo_type &radii_algo,
                    const positions_algo_type &positions_algo, const double& tol_radii = 0,
                    const double& tol_positions = 0, Graph<Circle> *canvas_circle_packing = NULL, Canvas* canvas = NULL,
                    bool show_live_cp = false);

public slots:
    void run();
    void stop();
    void stop_computing(bool b=true);

private:
    void choose_zero_one_two();
    void create_sines();

    // Standard
    double total_neighbors_angle(const unsigned int &index);
    double standard_radius_update(const unsigned int &index);
    void standard_radii_update();
    void standard_compute_radii();
    void compute_error();

    // Super
    double super_total_neighbors_angle(unsigned int index, double &error);
    double super_radius_update(const unsigned int &index, double &error);
    void super_radii_update(double &error);
    void super_compute_radii();
    double compute_lambda(const std::vector<double> &radii_0, const double &lambda);

    Graph<Empty> G_;
    complex_number angle_;
    unsigned int zero_;
    unsigned int one_;
    unsigned int two_;
    double tol_radii_;
    double tol_positions_;
    double delta_;

    std::vector<double> radii_;
    double error_;
    Graph<Circle> output_circle_packing_;

    radii_algo_type radii_algo_;
    positions_algo_type positions_algo_;
    double time_;
    unsigned int nb_iterations_;
    bool stop_;
    //QTimer *timer_;

    std::vector<double> sin_pi_over_n_;
    std::vector<unsigned int> neighbors_lists_sizes_;

    Positions P_;

    Graph<Circle> *canvas_circle_packing_;
    Canvas* canvas_;
    bool show_live_cp_;
};





#endif
