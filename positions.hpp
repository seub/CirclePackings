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

#ifndef POSITIONS_HPP
#define POSITIONS_HPP

#include <QThread>

#include "basic_types.hpp"
#include "graph.hpp"

class QComboBox;
class Canvas;

class Positions : public QThread
{
    Q_OBJECT

    friend class Top_Algo;
    friend class Tests;

public:
    Positions () {}
    void reset(const positions_algo_type positions_algo, const Graph<Empty> &G,
               const unsigned int &zero, const unsigned int &one,
               const unsigned int &two, std::vector<double> *ptr_radii,
               const complex_number &angle, const double &tol,
               Graph<Circle> *canvas_circle_packing = NULL, Canvas* canvas = NULL,
               bool show_live_cp = false, bool* stop = NULL);
    const Graph<Circle> &  get_output_circle_packing() const;
    const double & get_error() const;
    
signals:
    void show_live_cp();

public slots:
    void run();

private:
    void compute_positions();
    void compute_positions_regular();
    void compute_positions_downwards();
    void compute_positions_upwards();

    void synchronise_angles();

    void compute_max_error();
    double compute_single_error(const unsigned int &index) const;

    bool* stop_;

    positions_algo_type positions_algo_;
    Graph<Empty> G_;
    unsigned int zero_;
    unsigned int one_;
    unsigned int two_;

    std::vector<double> * ptr_radii_;
    std::vector<double> radii_;
    std::vector<complex_number> positions_;
    double error_;
    double tol_;

    complex_number angle_;
    Graph<Circle> output_circle_packing_;

    Graph<Circle> *canvas_circle_packing_;
    Canvas* canvas_;
    bool show_live_cp_;
};


complex_number compute_next_position(const complex_number &pivot_affix, const complex_number &neighbor_affix,
                                     const double &pivot_radius, const double &neighbor_radius, const double &next_radius,
                                     const int &orientation);



#endif // POSITIONS_HPP
