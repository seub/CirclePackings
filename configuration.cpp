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

#include "configuration.hpp"


#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include "window.hpp"
#include "canvas.hpp"
#include "canvas_delegate.hpp"
#include "input_menu.hpp"
#include "graph_tiler.hpp"
#include "output_menu.hpp"


Configuration::Configuration(Window *window)
{
    window_ = window;
    reset_all_parameters();
}

void Configuration::reset_window_pointer(Window *window)
{
    window_ = window;
}

void Configuration::reset_all_parameters()
{

    mode_flag_ = START;

    draw_graph_flag_ = -1;
    graph_choice_flag_ = -1;
    graph_parameter_1_ = -1;
    graph_parameter_2_ = -1;
    input_planar_graph_.reset_graph();

    tiling_size_ = -1;
    coloring_type_ = -1;

    draw_curve_flag_1_ = -1;
    curve_choice_flag_1_ = -1;
    smooth_curve_1_ = false;
    nb_sides_1_ = 0;
    nodes_1_.clear();
    tangent_factors_1_.clear();

    draw_2_curves_flag_ = -1;
    inverse_ = false;

    draw_curve_flag_2_ = -1;
    curve_choice_flag_2_ = -1;
    smooth_curve_2_ = false;
    nb_sides_2_ = 0;
    nodes_2_.clear();
    tangent_factors_2_.clear();

    return;
}

void Configuration::update_parameters()
{
    reset_all_parameters();

    mode_flag_ = window_->mode_flag_;
    switch(mode_flag_)
    {
    case START:
        break;

    case DRAW_GRAPH:
        update_graph();
        break;

    case DRAW_CURVE:
        tiling_size_ = window_->input_menu_->tiling_size_spinbox_->value();
        update_curve_1();
        coloring_type_ = window_->input_menu_->choose_coloring_box_->currentIndex();
        break;

    case DRAW_2_CURVES:
        tiling_size_ = window_->input_menu_->tiling_size_spinbox_->value();
        draw_2_curves_flag_ = window_->draw_2_curves_flag_;
        inverse_ = window_->output_menu_->show_inverse_checkbox_->isChecked();
        update_curve_1();
        update_curve_2();
        coloring_type_ = window_->input_menu_->choose_coloring_box_2_->currentIndex();
        break;

    default:
        std::cout << "ERROR in Configuration::update_parameters : flag problem" << std::endl;
        throw(QString("ERROR in Configuration::update_parameters : flag problem"));
    }
}

void Configuration::update_graph()
{
    //Graph settings
    draw_graph_flag_ = window_->draw_graph_flag_;
    graph_choice_flag_ = window_->graph_choice_flag_;

    switch(graph_choice_flag_)
    {
    case CUSTOM_GRAPH:
        input_planar_graph_ = window_->maths_contents_.input_planar_graph_;
        break;

    case GRID:
        graph_parameter_1_ = window_->input_menu_->graph_parameter_spinbox_->value();
        break;

    case WEB:
        graph_parameter_1_ = window_->input_menu_->graph_parameter_spinbox_->value();
        graph_parameter_2_ = window_->input_menu_->graph_parameter_spinbox_2_->value();
        break;

    default :
        std::cout << "ERROR in Configuration::update_graph : flag problem" << std::endl;
        throw(QString("ERROR in Configuration::update_graph : flag problem"));
    }
    return;
}

void Configuration::update_curve_1()
{
    draw_curve_flag_1_ = window_->draw_curve_flag_;
    curve_choice_flag_1_ = window_->input_menu_->choose_curve_->currentIndex();

    if(curve_choice_flag_1_ == CUSTOM)
    {
        smooth_curve_1_ = window_->input_menu_->smooth_curve_->isChecked();
    }
    else
    {
        smooth_curve_1_ = false;
    }
    nb_sides_1_ = (curve_choice_flag_1_ == POLYGON || curve_choice_flag_1_ == STAR  ||
                   curve_choice_flag_1_ ==EPICYCLOID || curve_choice_flag_1_ ==KOCH)?
                window_->input_menu_->nb_points_spinbox_->value() : 100;

    nodes_1_ = window_->maths_contents_.nodes_1_;
    tangent_factors_1_ = window_->maths_contents_.tangent_factors_1_;

    return;
}

void Configuration::update_curve_2()
{
    draw_curve_flag_2_ = window_->draw_curve_flag_2_;
    curve_choice_flag_2_ = window_->input_menu_->choose_curve_2_->currentIndex();
    if(curve_choice_flag_2_ == CUSTOM)
    {
        smooth_curve_2_ = window_->input_menu_->smooth_curve_2_->isChecked();
    }
    else
    {
        smooth_curve_2_ = false;
    }
    nb_sides_2_ = (curve_choice_flag_2_ == POLYGON || curve_choice_flag_2_ == STAR  ||
                   curve_choice_flag_2_ ==EPICYCLOID || curve_choice_flag_2_ ==KOCH)?
                window_->input_menu_->nb_points_spinbox_2_->value() : 100;
    nodes_2_ = window_->maths_contents_.nodes_2_;
    tangent_factors_2_ = window_->maths_contents_.tangent_factors_2_;

    return;
}

void Configuration::restore_configuration()
{
    //std::cout << "Entering Configuration::restore_configuration" << std::endl;

    window_->setEnabled(false);
    window_->setUpdatesEnabled(false);

    window_->change_mode(mode_flag_, false);
    window_->input_menu_->choose_mode_->setCurrentIndex(mode_flag_);

    switch(mode_flag_)
    {
    case START:
        break;

    case DRAW_GRAPH :
        restore_graph_configuration();
        break;

    case DRAW_CURVE :
        restore_curve_configuration();
        break;

    case DRAW_2_CURVES :
        restore_2_curves_configuration();
        break;

    default:
        std::cout << "ERROR in Configuration::restore_configuration: flag problem" << std::endl;
    }


    window_->setEnabled(true);
    window_->setUpdatesEnabled(true);
    window_->update();
    return;
}

void Configuration::restore_graph_configuration()
{
    window_->input_menu_->empty_draw_graph_submenu();
    window_->draw_graph_flag_ = draw_graph_flag_;
    window_->graph_choice_flag_ = graph_choice_flag_;
    window_->input_menu_->choose_graph_combobox_->setCurrentIndex(graph_choice_flag_);

    if(graph_choice_flag_ == CUSTOM_GRAPH)
    {
        window_->maths_contents_.input_planar_graph_ = input_planar_graph_;
        window_->input_menu_->create_draw_graph_submenu();
        restore_custom_graph_menu();
    }
    else
    {
        restore_sample_graph_menu();
    }
    return;
}

void Configuration::restore_curve_configuration()
{
    window_->input_menu_->choose_coloring_box_->setCurrentIndex(coloring_type_);
    window_->draw_curve_flag_ = draw_curve_flag_1_;

    restore_curve_menu_1();
    window_->maths_contents_.nodes_1_ = nodes_1_;
    window_->maths_contents_.tangent_factors_1_ = tangent_factors_1_;
    restore_left_curve_canvas();
    if(tiling_size_ != window_->input_menu_->tiling_size_spinbox_->value())
    {
        window_->input_menu_->tiling_size_spinbox_->setValue(tiling_size_);
    }
    else
    {
        window_->set_tiling_size();
    }

    return;
}

void Configuration::restore_2_curves_configuration()
{
    window_->input_menu_->choose_coloring_box_2_->setCurrentIndex(coloring_type_);
    window_->draw_2_curves_flag_  = draw_2_curves_flag_;

    window_->draw_curve_flag_ = draw_curve_flag_1_;
    window_->draw_curve_flag_2_ = draw_curve_flag_2_;

    restore_curve_menu_1();
    restore_curve_menu_2();

    window_->maths_contents_.nodes_1_ = nodes_1_;
    window_->maths_contents_.nodes_2_ = nodes_2_;
    window_->maths_contents_.tangent_factors_1_ = tangent_factors_1_;
    window_->maths_contents_.tangent_factors_2_ = tangent_factors_2_;

    window_->output_menu_->show_inverse_checkbox_->setChecked(inverse_);
    window_->left_canvas_->canvas_delegate_->set_shower(inverse_);
    window_->right_canvas_->canvas_delegate_->set_shower(!inverse_);

    restore_left_curve_canvas();
    restore_right_curve_canvas();

    if(tiling_size_ != window_->input_menu_->tiling_size_spinbox_->value())
    {
        window_->input_menu_->tiling_size_spinbox_->setValue(tiling_size_);
    }
    else
    {
        window_->set_tiling_size();
    }

    if (coloring_type_==T_IMAGE)
    {
        window_->triangulation_coloring_ = T_SQUARES;
        window_->choose_coloring_2();
    }

    return;
}


void Configuration::restore_curve_menu_1()
{
    window_->input_menu_->empty_curve_parameters_menu();
    window_->curve_choice_flag_ = curve_choice_flag_1_;
    window_->input_menu_->choose_curve_->setCurrentIndex(curve_choice_flag_1_);
    window_->input_menu_->create_curve_parameters_menu();

    if(curve_choice_flag_1_ == POLYGON || curve_choice_flag_1_ == STAR  || curve_choice_flag_1_ ==EPICYCLOID || curve_choice_flag_1_ ==KOCH)
    {
        window_->input_menu_->set_nb_points_without_signal(nb_sides_1_);
    }

    if(curve_choice_flag_1_ == CUSTOM && (window_->draw_curve_flag_ == DRAW_CURVE_INVALID
                                          || window_->draw_curve_flag_ == DRAW_CURVE_READY
                                          || window_->draw_curve_flag_ ==DRAW_CURVE_DONE))
    {
        window_->input_menu_->smooth_curve_->setChecked(smooth_curve_1_);
        window_->input_menu_->smooth_curve_->setEnabled(true);
        window_->input_menu_->smooth_curve_label_->setEnabled(true);
    }

    return;
}

void Configuration::restore_curve_menu_2()
{
    window_->input_menu_->empty_curve_parameters_menu_2();
    window_->curve_choice_flag_2_ = curve_choice_flag_2_;
    window_->input_menu_->choose_curve_2_->setCurrentIndex(curve_choice_flag_2_);
    window_->input_menu_->create_curve_parameters_menu_2();
    if(curve_choice_flag_2_ == POLYGON || curve_choice_flag_2_ == STAR
            || curve_choice_flag_2_ ==EPICYCLOID || curve_choice_flag_2_ ==KOCH)
    {
        window_->input_menu_->set_nb_points_2_without_signal(nb_sides_2_);
    }

    if(curve_choice_flag_2_ == CUSTOM && (window_->draw_curve_flag_2_ == DRAW_CURVE_INVALID
                                          || window_->draw_curve_flag_2_ == DRAW_CURVE_READY
                                          || window_->draw_curve_flag_2_ ==DRAW_CURVE_DONE))
    {
        window_->input_menu_->smooth_curve_2_->setChecked(smooth_curve_2_);
        window_->input_menu_->smooth_curve_2_->setEnabled(true);
        window_->input_menu_->smooth_curve_label_2_->setEnabled(true);
    }

    return;
}


void Configuration::restore_left_curve_canvas()
{
    if(draw_curve_flag_1_ == DRAW_CURVE_INVALID
            || draw_curve_flag_1_ == DRAW_CURVE_READY
            || draw_curve_flag_1_ ==DRAW_CURVE_DONE)
    {
        window_->left_canvas_->canvas_delegate_->set_smooth(smooth_curve_1_);

        window_->maths_contents_.curve_1_.create_curve(nodes_1_, tangent_factors_1_,
                                                       window_->curve_choice_flag_,smooth_curve_1_, nb_sides_1_, 0);
    }
    window_->left_canvas_->canvas_delegate_->set_nb_sides(nb_sides_1_);
    return;
}

void Configuration::restore_right_curve_canvas()
{
    if(draw_curve_flag_2_ == DRAW_CURVE_INVALID
            || window_->draw_curve_flag_2_ == DRAW_CURVE_READY
            || window_->draw_curve_flag_2_ ==DRAW_CURVE_DONE)
    {
        window_->right_canvas_->canvas_delegate_->set_smooth(smooth_curve_2_);

        window_->maths_contents_.curve_2_.create_curve(nodes_2_, tangent_factors_2_,
                                                       window_->curve_choice_flag_2_,smooth_curve_2_, nb_sides_2_,0);
    }
    window_->right_canvas_->canvas_delegate_->set_nb_sides(nb_sides_2_);

    return;
}

void Configuration::restore_custom_graph_menu()
{

    //std::cout << "Entering in Configuration::restore_custom_graph_menu" << std::endl;

    if(draw_graph_flag_ == DRAW_GRAPH_READY
            || draw_graph_flag_ ==DRAW_GRAPH_DONE)
    {
        window_->input_menu_->set_draw_neither_vertices_nor_edges();
    }
    else
    {
        if(input_planar_graph_.nb_vertices()>2)
        {
            window_->input_menu_->complete_->setEnabled(true);
        }
        else
        {
            window_->input_menu_->complete_->setEnabled(false);
        }
        if (draw_graph_flag_ == DRAW_GRAPH_VERTICES)
        {
            if(input_planar_graph_.nb_vertices()>1)
            {
                window_->input_menu_->set_draw_vertices_with_edges_allowed();
            }
            else
            {
                window_->input_menu_->set_draw_vertices_with_edges_forbidden();
            }
        }
        else if (draw_graph_flag_ == DRAW_GRAPH_EDGES)
        {
            window_->input_menu_->set_draw_edges();
        }

    }
    return;
}

void Configuration::restore_sample_graph_menu()
{
    window_->input_menu_->create_draw_graph_submenu();
    window_->input_menu_->empty_graph_parameters_menu();
    window_->input_menu_->create_graph_parameters_menu();

    switch(graph_choice_flag_)
    {
    case GRID:
        window_->input_menu_->set_graph_parameter_1_without_signal(graph_parameter_1_);
        window_->set_graph_parameter(graph_parameter_1_);
        break;

    case WEB:
        window_->input_menu_->set_graph_parameter_1_without_signal(graph_parameter_1_);
        window_->set_graph_parameter(graph_parameter_1_);
        window_->input_menu_->set_graph_parameter_2_without_signal(graph_parameter_2_);
        window_->set_graph_parameter(graph_parameter_2_);
        break;

    default:
        std::cout << "ERROR in Configuration::restore_sample_graph_menu: never supposed to reach here" << std::endl;
        throw(QString("ERROR in Configuration::restore_sample_graph_menu: never supposed to reach here"));
    }

    return;
}



void Configuration::write_in_file(std::ofstream &save_file)
{
    save_file << "Circle_Packings_save_file" << std::endl << std::endl; //Espace rajouté
    save_file <<  "mode_flag_: "<< mode_flag_ << std::endl;

    switch(mode_flag_)
    {
    case START:
        break;

    case DRAW_GRAPH :
        save_file <<  "draw_graph_flag_: "<<  draw_graph_flag_ << " graph_choice_flag_: " << graph_choice_flag_ << std::endl;
        if(graph_choice_flag_ == CUSTOM_GRAPH)
        {
            save_custom_graph(save_file);
        }
        else
        {
            save_sample_graph(save_file);
        }
        break;

    case DRAW_CURVE :
        save_file << "tiling_size_: "<< tiling_size_ << std::endl << "coloring_type_: " << coloring_type_ << std::endl;
        save_curve_1(save_file);
        break;

    case DRAW_2_CURVES :
        save_file << "tiling_size_: "<< tiling_size_ << std::endl << "coloring_type_: " << coloring_type_ << std::endl;
        save_file <<  "draw_2_curves_flag_: " << draw_2_curves_flag_ <<std::endl;
        save_file << "inverse_: " << inverse_ << std::endl;
        save_curve_1(save_file);
        save_curve_2(save_file);
        break;
    }
    save_file << "key: " << compute_key();
    return;
}

int Configuration::compute_key()
{
    int key = 0;
    key +=mode_flag_;
    switch(mode_flag_)
    {
    case START:
        break;
    case DRAW_GRAPH :
        key += draw_graph_flag_ + graph_choice_flag_;
        if(graph_choice_flag_ == CUSTOM_GRAPH){
            key += input_planar_graph_.nb_vertices();
        }
        else
        {
            key += graph_parameter_1_;
        }
        break;
    case DRAW_CURVE :
        key += tiling_size_ + coloring_type_;
        key += draw_curve_flag_1_+curve_choice_flag_1_ + nodes_1_.size();
        break;
    case DRAW_2_CURVES :
        key += tiling_size_ + coloring_type_;
        key += draw_2_curves_flag_;
        key += draw_curve_flag_1_ + curve_choice_flag_1_  + nodes_1_.size();
        key += draw_curve_flag_2_ + curve_choice_flag_2_ + nodes_2_.size();
        break;
    }
    return key;
}

void Configuration::save_sample_graph(std::ofstream &save_file)
{
    switch(graph_choice_flag_)
    {
    case GRID:
        save_file << "graph_parameter_1_: " << graph_parameter_1_ <<std::endl;
        break;

    case WEB:
        save_file << "graph_parameter_1_: " << graph_parameter_1_ <<std::endl;
        save_file << "graph_parameter_2_: " << graph_parameter_2_ <<std::endl;
        break;

    default:
        std::cout << "ERROR in Configuration::save_sample_graph: never supposed to reach here" << std::endl;
        throw(QString("ERROR in Configuration::save_sample_graph: never supposed to reach here"));
    }

    return;
}

void Configuration::save_custom_graph(std::ofstream &save_file)
{

    unsigned int nb_vertices = input_planar_graph_.nb_vertices();
    save_file << "nb_vertices: " << nb_vertices << std::endl;

    std::vector<vertex_label> neighbors;
    Point point;
    unsigned int i,j;
    save_file << "Graph_data: " << std::endl;
    for(i = 0 ; i < nb_vertices;i++)
    {
        save_file << input_planar_graph_.get_label_by_index(i) << " ";
        save_file << input_planar_graph_.get_color_by_index(i).red() << " ";
        save_file << input_planar_graph_.get_color_by_index(i).green() << " ";
        save_file << input_planar_graph_.get_color_by_index(i).blue() << " ";
        point = input_planar_graph_.get_content_by_index(i);
        save_file << point.get_affix().real() << " "<< point.get_affix().imag();
        neighbors =  input_planar_graph_.get_neighbors_by_index(i);
        save_file << " " << neighbors.size() ;
        for(j = 0; j< neighbors.size();j++)
        {
            save_file << " " << neighbors[j] ;
        }
        save_file << std::endl;
    }
}

void Configuration::save_curve_1(std::ofstream &save_file)
{
    save_file <<  "draw_curve_flag_: " <<  draw_curve_flag_1_ << std::endl;
    save_file << "curve_choice_flag_: " << curve_choice_flag_1_ << " ";
    save_file << "smooth_curve_1_: "<< smooth_curve_1_ << " ";
    save_file << "nb_sides_1_: " <<nb_sides_1_ << std::endl;
    save_file << "nodes_1_.size(): " << nodes_1_.size() << std::endl;
    save_file << "nodes_1_: " << std::endl;

    unsigned int i;
    for(i = 0 ; i < nodes_1_.size();i++)
    {
        save_file << real(nodes_1_[i].get_affix()) << " " << imag(nodes_1_[i].get_affix()) << std::endl;
    }

    if (curve_choice_flag_1_ == CUSTOM && draw_curve_flag_1_ != DRAW_CURVE_NODES && smooth_curve_1_)
    {
        save_file << "tangent_factors_1_: " << std::endl;
        if (nodes_1_.size() == tangent_factors_1_.size())
        {
            for(i = 0 ; i < nodes_1_.size();i++)
            {
                save_file << real(tangent_factors_1_[i]) << " " << imag(tangent_factors_1_[i]) << std::endl;
            }
        }
        else
        {
            std::cout << "ERROR in Configuration::save_curve_1: size problem: " << std::endl;
            std::cout << "nodes size = " << nodes_1_.size() << ", tangent factors size = " << tangent_factors_1_.size() << std::endl;
            throw(QString("ERROR in Configuration::save_curve_1: size problem: "));
        }
    }
    return;
}

void Configuration::save_curve_2(std::ofstream &save_file)
{
    save_file <<  "draw_curve_flag_2_: " <<  draw_curve_flag_2_ << std::endl;
    save_file << "curve_choice_flag_2_: " << curve_choice_flag_2_ << " ";
    save_file << "smooth_curve_2_: "<< smooth_curve_2_ << " ";
    save_file << "nb_sides_2_: " <<nb_sides_2_ << std::endl;
    save_file << "nodes_2_.size(): " << nodes_2_.size() << std::endl;

    save_file << "nodes_2_: " << std::endl;
    unsigned int i;
    for(i = 0 ; i < nodes_2_.size();i++)
    {
        save_file << real(nodes_2_[i].get_affix()) << " "<< imag(nodes_2_[i].get_affix()) << std::endl;
    }

    if (curve_choice_flag_2_ == CUSTOM && draw_curve_flag_2_ != DRAW_CURVE_NODES && smooth_curve_2_)
    {
        save_file << "tangent_factors_2_: " << std::endl;
        if (nodes_2_.size() == tangent_factors_2_.size())
        {
            for(i = 0 ; i < nodes_2_.size();i++)
            {
                save_file << real(tangent_factors_2_[i]) << " " << imag(tangent_factors_2_[i]) << std::endl;
            }
        }
        else
        {
            std::cout << "ERROR in Configuration::save_curve_1: size problem" << std::endl;
            throw(QString("ERROR in Configuration::save_curve_1: size problem"));
        }
    }
    return;
}



int Configuration::read_from_file(std::ifstream &load_file)
{
    int output_key;
    std::string start_key, trash;
    load_file >> start_key;
    if(start_key.compare("Circle_Packings_save_file"))
    {
        return 2;
    }
    load_file >> trash >> mode_flag_;
    switch(mode_flag_)
    {
    case START:
        break;

    case DRAW_GRAPH :
        load_file >>  trash >> draw_graph_flag_ >>  trash >> graph_choice_flag_;
        if(graph_choice_flag_ == CUSTOM_GRAPH)
        {
            load_custom_graph(load_file);
        }

        else
        {
            load_sample_graph(load_file);
        }
        break;

    case DRAW_CURVE :
        load_file >> trash >> tiling_size_ >> trash >> coloring_type_;
        load_curve_1(load_file);
        break;

    case DRAW_2_CURVES :
        load_file >> trash >> tiling_size_ >> trash >> coloring_type_;
        load_file >> trash >> draw_2_curves_flag_;
        load_file >> trash >> inverse_;
        load_curve_1(load_file);
        load_curve_2(load_file);
        break;

    default :
        std::cout << "ERROR in Configuration::read_from_file: flag problem" << std::endl;
        throw(QString("ERROR in Configuration::read_from_file: flag problem"));
    }

    load_file >> trash >> output_key;
    if(output_key == compute_key() && load_file.eof())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void Configuration::load_custom_graph(std::ifstream &load_file)
{
    std::string trash;
    unsigned int nb_vertices;
    load_file >> trash >> nb_vertices;

    std::vector<vertex_label> neighbors;
    neighbors.clear();
    unsigned int i,j;
    vertex_label label, neighbor;
    int red,green,blue;
    unsigned int nb_neighbors;
    double re,im;
    input_planar_graph_.reset_graph();
    load_file >> trash;
    for(i = 0 ; i < nb_vertices;i++)
    {
        load_file >> label;
        load_file >> red >> green >> blue;
        load_file >> re >> im;
        load_file >> nb_neighbors;
        neighbors.clear();
        for(j = 0; j<nb_neighbors;j++)
        {
            load_file >> neighbor;
            neighbors.push_back(neighbor);
        }
        input_planar_graph_.add_isolated_vertex(label,Point(complex_number(re,im)),QColor(red,green,blue));
        input_planar_graph_.reset_neighbors(i,neighbors);
    }
    return;
}

void Configuration::load_sample_graph(std::ifstream &load_file)
{
    std::string trash;

    switch(graph_choice_flag_)
    {
    case GRID:
        load_file >> trash >> graph_parameter_1_;
        break;

    case WEB:
        load_file >> trash >> graph_parameter_1_;
        load_file >> trash >> graph_parameter_2_;
        break;

    default:
        std::cout << "ERROR in Configuration::load_sample_graph: never supposed to reach here" << std::endl;
        throw(QString("ERROR in Configuration::load_sample_graph: never supposed to reach here"));
    }

    return;
}

void Configuration::load_curve_1(std::ifstream &load_file)
{
    std::string trash;
    unsigned int nb_nodes;
    unsigned int i;
    double re, im;

    load_file >> trash >>  draw_curve_flag_1_;
    load_file >> trash >> curve_choice_flag_1_;
    load_file >> trash >> smooth_curve_1_;
    load_file >> trash >> nb_sides_1_;
    load_file >> trash>> nb_nodes;
    load_file >> trash;
    nodes_1_.clear();
    nodes_1_.reserve(nb_nodes);
    for(i = 0 ; i < nb_nodes;i++)
    {
        load_file >> re >> im;
        nodes_1_.push_back(complex_number(re,im));
    }


    if (curve_choice_flag_1_ == CUSTOM && draw_curve_flag_1_ != DRAW_CURVE_NODES && smooth_curve_1_)
    {
        load_file >> trash;
        tangent_factors_1_.clear();
        tangent_factors_1_.reserve(nb_nodes);
        for(i = 0 ; i < nb_nodes;i++)
        {
            load_file >> re >> im;
            tangent_factors_1_.push_back(complex_number(re,im));
        }
    }
    return;
}

void Configuration::load_curve_2(std::ifstream &load_file)
{
    std::string trash;
    unsigned int nb_nodes_2;
    unsigned int i;
    double re, im;

    load_file >> trash >>  draw_curve_flag_2_;
    load_file >> trash >> curve_choice_flag_2_;
    load_file >> trash >> smooth_curve_2_;
    load_file >> trash >> nb_sides_2_;
    load_file >> trash>> nb_nodes_2;
    load_file >> trash;
    nodes_2_.clear();
    nodes_2_.reserve(nb_nodes_2);
    for(i = 0 ; i < nb_nodes_2; i++)
    {
        load_file >> re >> im;
        nodes_2_.push_back(complex_number(re,im));
    }


    if (curve_choice_flag_2_ == CUSTOM && draw_curve_flag_2_ != DRAW_CURVE_NODES && smooth_curve_2_)
    {
        load_file >> trash;
        tangent_factors_2_.clear();
        tangent_factors_2_.reserve(nb_nodes_2);
        for(i = 0 ; i < nb_nodes_2; i++)
        {
            load_file >> re >> im;
            tangent_factors_2_.push_back(complex_number(re,im));
        }
    }
    return;
}
