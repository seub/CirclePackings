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

#include "tests.hpp"
#include "graph.hpp"
#include "graph_tiler.hpp"
#include "window.hpp"
#include "canvas.hpp"
#include "positions_mixer.hpp"

#include <QGridLayout>
#include <fstream>


Tests::Tests(const unsigned int &tiling_size, const double &tol_radii) :
    tiling_size_(tiling_size), tol_radii_(tol_radii)
{
    canvas_size_in_pixels_ = 594;
    radii_algo_ = SUPER;
}

const std::vector<double> & Tests::get_deltas() const
{
    return deltas_;
}

const std::vector<double> & Tests::get_delta_overall_scores() const
{
    return delta_overall_scores_;
}

const std::vector<double> & Tests::get_alphas() const
{
    return alphas_;
}

const std::vector<double> & Tests::get_alpha_overall_scores() const
{
    return alpha_overall_scores_;
}


void Tests::launch_delta_tests(const double &delta_min, const double &delta_max, const double &delta_step)
{
    delta_min_ = delta_min;
    delta_max_ = delta_max;
    delta_step_ = delta_step;

    clock_t t0 = clock();

    std::cout << "---------------" << std::endl;
    std::cout << "LAUNCHING DELTA TESTS" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    create_curves();
    create_graphs();
    create_deltas();
    run_delta_tests();

    clock_t t1 = clock();
    time_ = (t1 - t0)*1.0/CLOCKS_PER_SEC ;

    std::cout << std::endl
              << "total time: " << time_ << "s"
              << std::endl;

    print_delta_results();

    std::cout << std:: endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "TESTS FINISHED, SEE OUTPUT TEXT FILE" << std::endl;
    std::cout << "------------------------------------" << std::endl << std::endl;


    return;
}

void Tests::launch_mix_tests(const double &alpha_min, const double &alpha_max, const double &alpha_step)
{
    alpha_min_ = alpha_min;
    alpha_max_ = alpha_max;
    alpha_step_ = alpha_step;

    clock_t t0 = clock();

    std::cout << "---------------" << std::endl;
    std::cout << "LAUNCHING MIX TESTS" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    create_curves();
    create_graphs();
    create_alphas();
    run_alpha_tests();

    clock_t t1 = clock();
    time_ = (t1 - t0)*1.0/CLOCKS_PER_SEC ;

    std::cout << std::endl
              << "total time: " << time_ << "s"
              << std::endl;

    print_alpha_results();

    std::cout << std:: endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "TESTS FINISHED, SEE OUTPUT TEXT FILE" << std::endl;
    std::cout << "------------------------------------" << std::endl << std::endl;


    return;
}

void Tests::create_alphas()
{
    {
        alphas_.clear();

        double alpha = alpha_min_;
        while (alpha < alpha_max_+0.000001)
        {
            alphas_.push_back(alpha);
            alpha += alpha_step_;

        }
    }
    return;
}

void Tests::create_deltas()
{
    deltas_.clear();

    double delta = delta_min_;
    while (delta < delta_max_+0.000001)
    {
        deltas_.push_back(delta);
        delta += delta_step_;

    }
}

void Tests::create_curves()
{
    curves_.clear();

    std::vector<Point> nodes;
    std::vector<complex_number> tangents;
    Curve curve;

    nodes.push_back(Point(complex_number(1.0, 1.0)));
    curve.create_curve(nodes, tangents, RECTANGLE, false, 0, 0);
    curves_.push_back(curve);

    nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 0.5)));
    curve.create_curve(nodes, tangents, RECTANGLE, false, 0, 0);
    curves_.push_back(curve);

    /*nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 0.0)));
    curve.create_curve(nodes, POLYGON, false, 3, 0);
    curves_.push_back(curve);*/

    nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 0.0)));
    curve.create_curve(nodes, tangents, POLYGON, false, 4, 0);
    curves_.push_back(curve);

    nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 0.0)));
    curve.create_curve(nodes, tangents, POLYGON, false, 5, 0);
    curves_.push_back(curve);

    nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 1.0)*(sqrt(2.0)/2.0)));
    curve.create_curve(nodes, tangents, ELLIPSE, false, 100, 0);
    curves_.push_back(curve);

    nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 0.5)*(sqrt(2.0)/2.0)));
    curve.create_curve(nodes, tangents, ELLIPSE, false, 100, 0);
    curves_.push_back(curve);

    nodes.clear();
    nodes.push_back(Point(complex_number(1.0, 1.0)*(1.0/1.4171161)));
    curve.create_curve(nodes, tangents, EPICYCLOID, false, 3, 0);
    curves_.push_back(curve);
}

void Tests::create_graphs()
{
    Graph<Circle> tiling;
    Graph_Tiler GT0;
    GT0.reset_for_tiling(&tiling, 3*2*tiling_size_*1.0/canvas_size_in_pixels_);
    GT0.create_tiling();

    Graph<Empty> input_graph;
    Graph<Circle> dumb_graph;

    complex_number angle;

    graphs_.clear();
    unsigned int j;
    for (j=0; j<curves_.size(); j++)
    {
        dumb_graph.reset_graph();
        input_graph.reset_graph();
        Graph_Tiler GT;
        GT.reset_for_circle_packing(&tiling, &dumb_graph, &input_graph, &angle, &curves_[j]);
        GT.create_circle_packing();
        graphs_.push_back(input_graph);
        input_angles_.push_back(angle);
    }
    return;
}

void Tests::run_delta_tests()
{
    delta_results_.clear();

    complex_number dumb_angle;

    double delta_i;
    std::vector<double> results_i;

    unsigned int i, j;
    for (i=0; i<deltas_.size(); i++)
    {
        delta_i = deltas_[i];
        results_i.clear();
        for (j=0; j<curves_.size(); j++)
        {
            std::cout << "delta = " << delta_i << ", curve n°" << j << ", nb vertices = " << graphs_[j].nb_vertices() << " ...  ";
            algo_.reset_algo(graphs_[j], dumb_angle, SUPER, REGULAR, tol_radii_);
            algo_.set_delta(delta_i);
            algo_.stop_computing(false);
            algo_.compute_radii();
            results_i.push_back(algo_.get_nb_iterations());
            std::cout << "nb iterations = " << algo_.get_nb_iterations() << std::endl;
        }
        delta_results_.push_back(results_i);
    }


    average_nb_iterations_.clear();
    unsigned int total;
    for (j=0; j<curves_.size(); j++)
    {
        total = 0;
        for (i=0; i<delta_results_.size(); i++)
        {
            total += delta_results_[i][j];
        }
        average_nb_iterations_.push_back(total*1.0/deltas_.size());
    }


    delta_results_corrected_ = delta_results_;
    for (i=0; i<delta_results_.size(); i++)
    {
        for (j=0; j<curves_.size(); j++)
        {
            delta_results_corrected_[i][j] /= average_nb_iterations_[j];
        }
    }


    double score_i;
    delta_overall_scores_.clear();
    for (i=0; i<deltas_.size(); i++)
    {
        score_i = 0;
        for (j=0; j<curves_.size(); j++)
        {
            score_i += delta_results_corrected_[i][j];
        }
        delta_overall_scores_.push_back(score_i/curves_.size());
    }

    sort_delta_scores();


    return;
}

void Tests::run_alpha_tests()
{
    alpha_results_.clear();

    create_updown_circle_packings();

    double alpha_i;
    std::vector<double> results_i;

    Positions_Mixer P;
    double error;
    unsigned int i, j;
    for (i=0; i<alphas_.size(); i++)
    {
        alpha_i = alphas_[i];
        results_i.clear();
        for (j=0; j<curves_.size(); j++)
        {
            Graph<Circle> output;
            P.reset(&(graphs_[j]), &(upwards_circle_packings_[j]), &(downwards_circle_packings_[j]), &output, alpha_i);
            P.run();
            error = 0;//P.get_error()/(std::max(upwards_errors_[j], downwards_errors_[j]));
            results_i.push_back(error);
            std::cout << "alpha = " << alpha_i << ", curve n°" << j << ", nb vertices = " << graphs_[j].nb_vertices()
                      << ", error = " << error << std::endl;
        }
        alpha_results_.push_back(results_i);
    }


    average_alpha_results_.clear();
    double total;
    for (j=0; j<curves_.size(); j++)
    {
        total = 0;
        for (i=0; i<alpha_results_.size(); i++)
        {
            total += alpha_results_[i][j];
        }
        average_alpha_results_.push_back(total*1.0/alphas_.size());
    }


    alpha_results_corrected_ = alpha_results_;
    for (i=0; i<alpha_results_.size(); i++)
    {
        for (j=0; j<curves_.size(); j++)
        {
            alpha_results_corrected_[i][j] /= average_alpha_results_[j];
        }
    }


    double score_i;
    alpha_overall_scores_.clear();
    for (i=0; i<alphas_.size(); i++)
    {
        score_i = 0;
        for (j=0; j<curves_.size(); j++)
        {
            score_i += alpha_results_corrected_[i][j];
        }
        alpha_overall_scores_.push_back(score_i/curves_.size());
    }

    sort_alpha_scores();


    return;
}

void Tests::create_updown_circle_packings()
{
    Positions P;
    unsigned int j;
    double error;
    for (j=0; j<curves_.size(); j++)
    {
        std::cout << "Computing downwards circle packing n°" << j+1 << " (" << graphs_[j].nb_vertices()
                  << " vertices)... " << std::endl;
        algo_.reset_algo(graphs_[j], input_angles_[j], SUPER, DOWNWARDS, tol_radii_);
        algo_.stop_computing(false);
        algo_.compute_radii();
        P.reset(DOWNWARDS, algo_.G_, algo_.zero_, algo_.one_, algo_.two_, &(algo_.radii_), input_angles_[j], 0);
        P.radii_ = algo_.radii_;
        P.compute_positions();
        P.synchronise_angles();
        downwards_circle_packings_.push_back(P.get_output_circle_packing());
        P.compute_max_error();
        error = P.get_error();
        downwards_errors_.push_back(error);
        std::cout << "Error = " << error << std::endl;

        std::cout << "Computing upwards circle packing n°" << j+1 << " (" << graphs_[j].nb_vertices()
                  << " vertices)... " << std::endl;
        algo_.reset_algo(graphs_[j], input_angles_[j], SUPER, UPWARDS, tol_radii_);
        algo_.stop_computing(false);
        algo_.compute_radii();
        P.reset(UPWARDS, algo_.G_, algo_.zero_, algo_.one_, algo_.two_, &(algo_.radii_), input_angles_[j], 0);
        P.radii_ = algo_.radii_;
        P.compute_positions();
        P.synchronise_angles();
        upwards_circle_packings_.push_back(P.get_output_circle_packing());
        P.compute_max_error();
        error = P.get_error();
        upwards_errors_.push_back(error);
        std::cout << "Error = " << error << std::endl;
    }
    return;
}

void Tests::sort_delta_scores()
{
    std::vector< std::pair<unsigned int, double> > scores;
    unsigned int i;
    for (i=0; i<deltas_.size(); i++)
    {
        scores.push_back(std::pair<unsigned int, double>(i, delta_overall_scores_[i]));
    }
    sort(scores.begin(), scores.end(), compare_scores);

    deltas_sorted_by_score_.clear();
    for (i=0; i<deltas_.size(); i++)
    {
        deltas_sorted_by_score_.push_back(deltas_[scores[i].first]);
    }
    return;
}

void Tests::sort_alpha_scores()
{
    std::vector< std::pair<unsigned int, double> > scores;
    unsigned int i;
    for (i=0; i<alphas_.size(); i++)
    {
        scores.push_back(std::pair<unsigned int, double>(i, alpha_overall_scores_[i]));
    }
    sort(scores.begin(), scores.end(), compare_scores);

    alphas_sorted_by_score_.clear();
    for (i=0; i<alphas_.size(); i++)
    {
        alphas_sorted_by_score_.push_back(alphas_[scores[i].first]);
    }
    return;
}

void Tests::print_delta_results()
{
    QString string = QString("Delta_tests_results_TS%1 E%2")
            .arg(tiling_size_)
            .arg(tol_radii_);
    const char* filename= string.toStdString().c_str();

    std::ofstream file;
    file.open (filename);

    file << "-------------------------------------------------" << std::endl
         << "TESTS RESULTS" << std::endl
         << "-------------------------------------------------" << std::endl
         << std::endl
         << "*** PARAMETERS: ***" << std::endl;

    if(radii_algo_ == STANDARD)
    {
        file << "Radii algorithm : Standard" << std::endl;
    }
    else
    {
        file << "Radii algorithm : Super" << std::endl;
    }

    file << "delta_min = " << delta_min_<< ", delta_max = " << delta_max_ << ", delta_step = " << delta_step_
         << " (" << deltas_.size() << " deltas)" << std::endl
         << "Tiling size = " << tiling_size_ << std::endl
         << "Curves : Square / 1:2 Rectange / Triangle / 4-Polygon / 5-Polygon / Circle / 1:2 Ellipse / Cardioid" << std::endl
         << "Graphs number of vertices, edges:";
    unsigned int k, nb_vertices, nb_edges;
    for (k=0; k<graphs_.size(); k++)
    {
        nb_vertices =  graphs_[k].nb_vertices();
        nb_edges = graphs_[k].nb_edges();
        file << " " << nb_vertices << ", " << nb_edges << " /";
    }
    file << std::endl << std::endl << std::endl;

    file << "*** TOTAL TIME: ***" << std::endl
         << time_ << "s" << std::endl << std::endl;


    file << "*** NUMBER OF ITERATIONS: ***" << std::endl << std::endl;
    file << "                  Square  / Rect    / Tri     / Kite    / Penta   / Circle  / Ellipse / Cardioid" << std::endl;
    file << std::endl;

    unsigned int i, j;
    for(i=0; i<deltas_.size(); i++)
    {
        file << "delta = " << deltas_[i] << "    ";
        for (j=0; j<curves_.size(); j++)
        {
            file << delta_results_corrected_[i][j] << " / ";
        }
        file << std::endl;
    }

    file << std::endl;
    file << "Average nb iterations: ";
    for (j=0; j<average_nb_iterations_.size(); j++)
    {
        file << average_nb_iterations_[j] << " / ";
    }
    file << std::endl << std::endl << std::endl;
    file << "*** OVERALL SCORES: ***" << std::endl;
    file << "Delta / Score" << std::endl;
    for (i=0; i<deltas_.size(); i++)
    {
        file << deltas_[i] << " / " << delta_overall_scores_[i] << std::endl;
    }

    file << std::endl << std::endl << std::endl;
    file << "*** DELTAS SORTED BY SCORE: ***" << std::endl;
    for (i=0; i<deltas_.size(); i++)
    {
        file << deltas_sorted_by_score_[i] << " ";
    }
    file << std::endl;

    file.close();

    return;
}

void Tests::print_alpha_results()
{
    QString string = QString("Alpha_tests_results_TS%1 E%2")
            .arg(tiling_size_)
            .arg(tol_radii_);
    const char* filename= string.toStdString().c_str();

    std::ofstream file;
    file.open (filename);

    file << "-------------------------------------------------" << std::endl
         << "TESTS RESULTS" << std::endl
         << "-------------------------------------------------" << std::endl
         << std::endl
         << "*** PARAMETERS: ***" << std::endl;

    file << "alpha_min = " << alpha_min_<< ", alpha_max = " << alpha_max_ << ", alpha_step = " << alpha_step_
         << " (" << alphas_.size() << " alphas)" << std::endl
         << "Tiling size = " << tiling_size_ << std::endl
         << "Curves : Square / 1:2 Rectange / 4-Polygon / 5-Polygon / Circle / 1:2 Ellipse / Cardioid" << std::endl
         << "Graphs number of vertices, edges:";

    unsigned int k, nb_vertices, nb_edges;
    for (k=0; k<graphs_.size(); k++)
    {
        nb_vertices =  graphs_[k].nb_vertices();
        nb_edges = graphs_[k].nb_edges();
        file << " " << nb_vertices << ", " << nb_edges << " /";
    }
    file << std::endl << std::endl << std::endl;

    file << "*** TOTAL TIME: ***" << std::endl
         << time_ << "s" << std::endl << std::endl;


    file << "*** SCORES: ***" << std::endl << std::endl;
    file << "                  Square  / Rect    / Tri     / Kite    / Penta   / Circle  / Ellipse / Cardioid" << std::endl;
    file << std::endl;

    unsigned int i, j;
    for(i=0; i<alphas_.size(); i++)
    {
        file << "alpha = " << alphas_[i] << "    ";
        for (j=0; j<curves_.size(); j++)
        {
            file << alpha_results_corrected_[i][j] << " / ";
        }
        file << std::endl;
    }

    file << std::endl;
    file << "Average score: ";
    for (j=0; j<average_alpha_results_.size(); j++)
    {
        file << average_alpha_results_[j] << " / ";
    }
    file << std::endl << std::endl << std::endl;
    file << "*** OVERALL SCORES: ***" << std::endl;
    file << "Alpha / Score" << std::endl;
    for (i=0; i<alphas_.size(); i++)
    {
        file << alphas_[i] << " / " << alpha_overall_scores_[i] << std::endl;
    }

    file << std::endl << std::endl << std::endl;
    file << "*** ALPHAS SORTED BY SCORE: ***" << std::endl;
    for (i=0; i<alphas_.size(); i++)
    {
        file << alphas_sorted_by_score_[i] << " ";
    }
    file << std::endl;

    file.close();

    return;
}


bool compare_scores(const std::pair<unsigned int, double> &score_1, const std::pair<unsigned int, double> &score_2)
{
    return score_1.second < score_2.second;
}
