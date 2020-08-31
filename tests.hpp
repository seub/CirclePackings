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

#ifndef TESTS_HPP
#define TESTS_HPP

#include "basic_types.hpp"
#include "curve.hpp"
#include "algo.hpp"
#include "graph.hpp"

class Window;

class Tests
{
public:
    Tests(const unsigned int &tiling_size, const double &tol_radii);
    void launch_delta_tests(double const& delta_min, double const& delta_max, const double &delta_step);
    void launch_mix_tests(const double &alpha_min, const double &alpha_max, const double &alpha_step);

    const std::vector<double> & get_deltas() const;
    const std::vector<double> & get_delta_overall_scores() const;
    const std::vector<double> & get_alphas() const;
    const std::vector<double> & get_alpha_overall_scores() const;

private:
    void create_deltas();
    void create_alphas();
    void create_curves();
    void create_graphs();
    void run_delta_tests();
    void run_alpha_tests();
    void print_delta_results();
    void print_alpha_results();
    void sort_delta_scores();
    void sort_alpha_scores();

    void create_updown_circle_packings();

    unsigned int tiling_size_;
    radii_algo_type radii_algo_;
    Algo algo_;
    std::vector<Curve> curves_;
    std::vector< Graph<Empty> > graphs_;
    std::vector<complex_number> input_angles_;

    std::vector<double> deltas_;
    std::vector<double> deltas_sorted_by_score_;

    std::vector< std::vector<double> > delta_results_; // each element is the vector of results when curves vary, for a given delta.
    std::vector< std::vector<double> > delta_results_corrected_;
    std::vector<unsigned int> average_nb_iterations_;
    std::vector<double> delta_overall_scores_;
    unsigned int canvas_size_in_pixels_;

    double delta_min_;
    double delta_max_;
    double delta_step_;
    double tol_radii_;

    std::vector<double> alphas_;
    std::vector<double> alphas_sorted_by_score_;
    std::vector< std::vector<double> > alpha_results_; // each element is the vector of results when curves vary, for a given alpha.
    std::vector<double> average_alpha_results_;
    std::vector< std::vector<double> > alpha_results_corrected_;
    std::vector<double> alpha_overall_scores_;

    std::vector< Graph<Circle> > downwards_circle_packings_;
    std::vector< Graph<Circle> > upwards_circle_packings_;

    std::vector<double> downwards_errors_;
    std::vector<double> upwards_errors_;

    double alpha_min_;
    double alpha_max_;
    double alpha_step_;

    double time_;
};

bool compare_scores(const std::pair<unsigned int, double> &score_1, const std::pair<unsigned int, double> &score_2);

#endif // TESTS_HPP
