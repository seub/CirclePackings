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

#include "canvas_delegate_riemann.hpp"
#include "triangulation.hpp"
#include "triangulation_search_tree.hpp"
#include "riemann_positions.hpp"
#include "triangle.hpp"
#include "window.hpp"

#include <QString>
#include <QComboBox>
#include <QApplication>
#include <QPainter>
Canvas_Delegate_Riemann_Mapping::Canvas_Delegate_Riemann_Mapping(const unsigned int &size_in_pixels,
                                                                 const draw_2_curves_mode_type *const draw_2_curves_flag,
                                                                 const draw_curve_mode_type *const draw_curve_flag,
                                                                 const curve_choice_type *const curve_flag, Curve *const curve,
                                                                 Graph<Circle> *const tiling,
                                                                 std::vector<Point> *const nodes,
                                                                 std::vector<complex_number> *const tangent_factors,
                                                                 Graph<Circle> *const circle_packing,
                                                                 Triangulation *const input_triangulation,
                                                                 Triangulation *const output_triangulation,
                                                                 Triangulation *const other_canvas_input_triangulation,
                                                                 Triangulation_Search_Tree * const search_tree,
                                                                 Triangulation_Search_Tree * const output_search_tree,
                                                                 Riemann_Positions *const riemann_positions,
                                                                 Graph<Circle> *const circle_cp,
                                                                 Graph<Circle> *const circle_cp_save,
                                                                 Window *window, QImage *image,
                                                                 double *image_scale, int *image_origin_x_in_pixels,
                                                                 int *image_origin_y_in_pixels,
                                                                 double *other_canvas_image_scale,
                                                                 int *other_canvas_image_origin_x_in_pixels,
                                                                 int *other_canvas_image_origin_y_in_pixels,
                                                                 bool reset_contents) :
    draw_2_curves_flag_(draw_2_curves_flag), draw_curve_flag_(draw_curve_flag),
    curve_choice_flag_(curve_flag),
    curve_(curve), tiling_(tiling), nodes_(nodes), tangent_factors_(tangent_factors),
    circle_packing_(circle_packing), input_triangulation_(input_triangulation),
    output_triangulation_(output_triangulation), other_canvas_input_triangulation_(other_canvas_input_triangulation),
    search_tree_(search_tree), output_search_tree_(output_search_tree),
    riemann_positions_(riemann_positions), circle_cp_(circle_cp), circle_cp_save_(circle_cp_save)
{
    size_in_pixels_ = size_in_pixels;
    window_ = window;
    message_ = &(window_->message_received_);
    common_reset();
    smooth_ = false;
    nodes_radius_in_pixels_ = 6;

    image_ = image;
    image_scale_ = image_scale;
    image_origin_x_in_pixels_ = image_origin_x_in_pixels;
    image_origin_y_in_pixels_ = image_origin_y_in_pixels;
    other_canvas_image_scale_ = other_canvas_image_scale;
    other_canvas_image_origin_x_in_pixels_ = other_canvas_image_origin_x_in_pixels;
    other_canvas_image_origin_y_in_pixels_ = other_canvas_image_origin_y_in_pixels;

    image_mode_ = false;
    reset_canvas_delegate(reset_contents);
}

void Canvas_Delegate_Riemann_Mapping::reset_canvas_delegate(bool reset_contents)
{
    common_reset();

    if (reset_contents)
    {
        curve_->reset_curve();
        nodes_->clear();
        nodes_->reserve(20);
        circle_packing_->reset_graph();
        input_triangulation_->reset_triangulation();
        output_triangulation_->reset_triangulation();
        circle_cp_save_->reset_graph();
        circle_cp_->reset_graph();
        shower_ = !left_canvas_;
        tangent_factors_->clear();
    }

    mobius_ = Mobius(complex_number(0.0, 0.0));
    is_node_point_selected_ = false;
    is_node_point_under_mouse_ = false;
    node_point_created_ = false;
    smooth_ = false;

    is_tangent_control_point_selected_ = false;
    is_tangent_control_point_under_mouse_ = false;

    return;
}

void Canvas_Delegate_Riemann_Mapping::reset_canvas_delegate_for_computing()
{
    mobius_ = Mobius(complex_number(0.0, 0.0));
    reset_selected_stuff();
    return;
}

void Canvas_Delegate_Riemann_Mapping::reset_selected_stuff()
{
    is_node_point_under_mouse_ = false;
    is_node_point_selected_ = false;
    node_point_created_ = false;
    is_tangent_control_point_under_mouse_ = false;
    is_tangent_control_point_selected_ = false;

    common_reset_selected_stuff();
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_left(bool left)
{
    left_canvas_ = left;
    shower_ = !left;
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_image_mode(bool b)
{
    if (b)
    {
        set_image_parameters();
    }
    image_mode_ = b;
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_image_parameters()
{
    //std::cout << "Entering Canvas_Delegate_Riemann_Mapping::set_image_parameters" << std::endl;
    int image_width = image_->width(), image_height = image_->height();

    double widths_ratio = image_width*1.0/3.0;
    double heights_ratio = image_height*1.0/3.0;

    *image_scale_ = (widths_ratio > heights_ratio) ? widths_ratio : heights_ratio;//heights_ratio : widths_ratio;
    *image_origin_x_in_pixels_ = int_round(image_width/2.0);
    *image_origin_y_in_pixels_ = int_round(image_height/2.0);

    image_width_ = image_->width();
    image_height_ = image_->height();
    image_data_  = (QRgb*) image_->bits();
    return;
}

bool Canvas_Delegate_Riemann_Mapping::get_color_from_image(int x, int y, QRgb &color)
{
    if(x < 0 || x >= image_width_ || y <0 || y>= image_height_)
    {
        return false;
    }
    color = *(image_data_ + y*image_width_ + x);
    return true;
}

void Canvas_Delegate_Riemann_Mapping::draw_triangle_from_image(const Triangle &T)
{
    //std::cout << "Entering Canvas_Delegate_Riemann_Mapping::draw_triangle_from_image" << std::endl;7

    bool flat_base_bool = false;
    bool flat_top_bool = false;
    integer_coordinates xy_A, xy_B, xy_C, xy_D;
    Point A, B, C;

    T.get_ABC_by_decreasing_y(A, B, C);
    xy_A = complex_to_pixels_coordinates(A.get_affix());
    xy_B = complex_to_pixels_coordinates(B.get_affix());
    xy_C = complex_to_pixels_coordinates(C.get_affix());

    if(xy_B.second == xy_A.second)
    {
        flat_base_bool = true;
        xy_D = xy_A;
    }
    else if(xy_B.second == xy_C.second)
    {
        flat_top_bool = true;
        xy_D = xy_C;
    }
    else
    {
        double inv_pente_AC = (xy_C.first-xy_A.first)*1.0/(xy_C.second-xy_A.second);
        int x_D = int_round((xy_B.second-xy_A.second)*inv_pente_AC + xy_A.first);
        xy_D = integer_coordinates(x_D,xy_B.second);

    }
    if(xy_D.first < xy_B.first)
    {
        std::swap(xy_B,xy_D);
    }

    int i, j;
    double i1, i2;
    double inv_pente_AB = (xy_B.first-xy_A.first)*1.0/(xy_B.second-xy_A.second);
    double inv_pente_AD = (xy_D.first-xy_A.first)*1.0/(xy_D.second-xy_A.second);
    double inv_pente_CB = (xy_B.first-xy_C.first)*1.0/(xy_B.second-xy_C.second);
    double inv_pente_CD = (xy_D.first-xy_C.first)*1.0/(xy_D.second-xy_C.second);
    double ops1 = xy_A.first-xy_A.second*inv_pente_AB;
    double ops2 = xy_A.first-xy_A.second*inv_pente_AD;
    double ops3 = xy_C.first-xy_C.second*inv_pente_CB;
    double ops4 = xy_C.first-xy_C.second*inv_pente_CD;
    QRgb color;
    integer_coordinates ij;
    if(!flat_base_bool)
    {
        for(j = std::max(0,xy_A.second); j<=std::min(size_in_pixels_, xy_B.second);j++)
        {
            i1 = std::max(j*inv_pente_AB + ops1,0.0);
            i2 = std::min(j*inv_pente_AD + ops2,size_in_pixels_*1.0);
            for(i = int_round(i1);i<= int_round(i2);i++)
            {
                ij = affine_transform_.kick(integer_coordinates(i,j));
                if(ij.first<image_->width() && ij.first >0 && ij.second < image_->height() && ij.second >0)
                {
                    //painter_->setPen(QColor((image_->pixel(ij.first, ij.second))));
                    if(get_color_from_image(ij.first, ij.second, color))
                    {
                        draw_pixel(i,j,color);
                    }
                    //painter_->drawPoint(i,j);
                }
            }
        }
    }
    if(!flat_top_bool)
    {
        for(j = std::min(size_in_pixels_, xy_C.second); j>=std::max(0,xy_B.second);j--)
        {
            i1 = std::max(j*inv_pente_CB + ops3,0.0);
            i2 = std::min(j*inv_pente_CD + ops4,size_in_pixels_*1.0);
            for(i = int_round(i1);i<= int_round(i2);i++)
            {
                ij = affine_transform_.kick(integer_coordinates(i,j));
                if(ij.first<image_->size().width() && ij.first >0 && ij.second < image_->size().height() && ij.second >0)
                {
                    //painter_->setPen(QColor((image_->pixel(ij.first, ij.second))));
                    //painter_->drawPoint(i,j);
                    if(get_color_from_image(ij.first, ij.second, color))
                    {
                        draw_pixel(i,j,color);
                    }
                }
            }
        }
    }

    return;
}

void Canvas_Delegate_Riemann_Mapping::draw_triangulation_from_image()
{
    //std::cout << "Entering Canvas_Delegate_Riemann_Mapping::draw_triangulation_from_image" << std::endl;

    affine_transform_.set_for_match_triangles_in_pixels(&scale_, &origin_x_in_pixels_, &origin_y_in_pixels_,
                                                        image_scale_, image_origin_x_in_pixels_, image_origin_y_in_pixels_);
    affine_transform_.match_coordinates_in_pixels();
    unsigned int i, N = input_triangulation_->nb_triangles();
    Triangle T;
    for (i=0; i<N; i++)
    {
        if (input_triangulation_->get_triangle(i, T))
        {
            draw_triangle_from_image(T);
        }
    }
    return;
}

void Canvas_Delegate_Riemann_Mapping::draw_shower_triangulation_from_image()
{
    affine_transform_.set_for_match_triangles_in_pixels(&scale_, &origin_x_in_pixels_, &origin_y_in_pixels_,
                                                        other_canvas_image_scale_, other_canvas_image_origin_x_in_pixels_,
                                                        other_canvas_image_origin_y_in_pixels_);

    Triangle T_input, T_output;
    unsigned int i, N = output_triangulation_->nb_triangles();
    for (i=0; i<N; i++)
    {
        other_canvas_input_triangulation_->get_triangle(i, T_input);
        output_triangulation_->get_triangle(i, T_output);
        affine_transform_.match_triangles_in_pixels(T_output, T_input);
        draw_triangle_from_image(T_output);
    }

    return;
}

void Canvas_Delegate_Riemann_Mapping::draw_background_image()
{
    QRect source(0, 0, image_width_, image_height_);
    complex_number left_top_corner, right_bottom_corner;
    int top, left, width, height;
    width = int_round(image_width_*scale_/(*image_scale_));
    height = int_round(image_height_*scale_/(*image_scale_));

    if (width > height)
    {
        left_top_corner = complex_number(-1.5, image_height_*0.5/(*image_scale_));
        right_bottom_corner = complex_number(1.5, -image_height_*0.5/(*image_scale_));
    }
    else
    {
        left_top_corner = complex_number(- image_width_*0.5/(*image_scale_), 1.5);
        right_bottom_corner = complex_number(image_width_*0.5/(*image_scale_), - 1.5);
    }
    integer_coordinates left_top_corner_in_pixels = complex_to_pixels_coordinates(left_top_corner);
    integer_coordinates right_bottom_corner_in_pixels = complex_to_pixels_coordinates(right_bottom_corner);

    //affine_transform_.set_for_match_triangles_in_pixels(&scale_, &origin_x_in_pixels_, &origin_y_in_pixels_,
    //            image_scale_, image_origin_x_in_pixels_, image_origin_y_in_pixels_);
    //affine_transform_.match_coordinates_in_pixels();
    //integer_coordinates left_top = affine_transform_.kick(left_top_corner_in_pixels);



    left = left_top_corner_in_pixels.first;
    top = left_top_corner_in_pixels.second;
    width = right_bottom_corner_in_pixels.first - left;
    height = right_bottom_corner_in_pixels.second - top;
    QRect target(left, top, width, height);
    /*std::cout << "width = " << width << ", "
                 << "height = " << height << ", "
                    << "top = " << top << ", "
                       << "left = " << left << ", "
                          << "ox = " << *image_origin_x_in_pixels_ << ", "
                          << "oy = " << *image_origin_y_in_pixels_ << std::endl;*/
    painter_->setOpacity(0.5);
    painter_->drawImage(target, *image_, source);
    painter_->setOpacity(1.0);
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_node_point_under_mouse(Point const &point)
{
    unsigned int i;
    for (i=0; i<nodes_->size(); i++)
    {
        if (abs((*nodes_)[i].get_affix() - point.get_affix()) < nodes_radius_in_pixels_/scale_)
        {
            is_node_point_under_mouse_ = true;
            node_point_under_mouse_ = i;
            return;
        }
    }
    is_node_point_under_mouse_ = false;
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_vertex_under_mouse(const Point &point)
{
    double alpha, beta, gamma;
    if (*draw_2_curves_flag_==DRAW_2_CURVES_DONE && shower_ )
    {
        if(click_on_ && QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            is_vertex_under_mouse_ = output_triangulation_->triangle_containing(index_vertex_under_mouse_,alpha, beta, gamma,
                                                                                point);
        }
        else
        {
            is_vertex_under_mouse_ = output_search_tree_->get_containing_triangle_index(alpha, beta, gamma,
                                                                                        index_vertex_under_mouse_, point);
        }
    }
    else
    {
        is_vertex_under_mouse_ = input_triangulation_->triangle_containing(index_vertex_under_mouse_,alpha, beta, gamma,
                                                                           point);
    }
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_tangent_control_point_under_mouse(Point const &point)
{
    complex_number T;
    unsigned int i, N = nodes_->size();
    if (tangent_factors_->size() != nodes_->size())
    {
        std::cout << "ERROR in Canvas_Delegate_Riemann_Mapping::set_tangent_control_point_under_mouse: bad size" << std::endl;
        throw(QString("ERROR in Canvas_Delegate_Riemann_Mapping::set_tangent_control_point_under_mouse: bad size"));
    }
    else if (!beziers_mode())
    {
        std::cout << "ERROR in Canvas_Delegate_Riemann_Mapping::set_tangent_control_point_under_mouse: never supposed to reach here"
                  << std::endl;
        throw(QString("ERROR in Canvas_Delegate_Riemann_Mapping::set_tangent_control_point_under_mouse: never supposed to reach here"));
    }

    for (i=0; i<N; i++)
    {
        T = (*nodes_)[i].get_affix() + (*tangent_factors_)[i] *
                ((*nodes_)[(i+1)%N].get_affix() - (*nodes_)[(N+i-1)%N].get_affix());



        if (abs(T - point.get_affix()) < 0.8*nodes_radius_in_pixels_/scale_)
        {
            is_tangent_control_point_under_mouse_ = true;
            tangent_control_point_under_mouse_ = i;
            return;
        }
    }
    is_tangent_control_point_under_mouse_ = false;
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_smooth(bool smooth)
{
    smooth_ = smooth;
    if (!smooth)
    {
        is_tangent_control_point_under_mouse_ = false;
        is_tangent_control_point_selected_ = false;
    }
    return;
}

void Canvas_Delegate_Riemann_Mapping::set_shower(bool shower)
{
    shower_ = shower;
    return;
}

bool Canvas_Delegate_Riemann_Mapping::beziers_mode()
{
    return *draw_2_curves_flag_ == DRAWING_CURVES && *draw_curve_flag_ != DRAW_CURVE_NODES
            && *draw_curve_flag_ != DRAW_CURVE_DONE
            && smooth_ && *curve_choice_flag_ == CUSTOM;
}

void Canvas_Delegate_Riemann_Mapping::set_nb_sides(const unsigned int &nb_sides)
{
    nb_sides_ = nb_sides;
    return;
}

void Canvas_Delegate_Riemann_Mapping::apply_mobius()
{
    //clock_t t0 = clock();

    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        complex_number z_0 = point_clicked_inverse_in_unit_disk_.get_affix();
        complex_number z_1 = point_mouse_over_inverse_in_unit_disk_.get_affix();
        if (abs(z_0)!=0 && abs(z_1)!=0)
        {
            *circle_cp_ = *circle_cp_save_;
            complex_number u_0 = z_0/abs(z_0);
            complex_number u_1 = z_1/abs(z_1);
            complex_number u = u_1*conj(u_0);
            mobius_temp_ = Mobius(u, 0.0, 0.0, 1.0)*mobius_;
            circle_cp_->apply_mobius(mobius_temp_);

        }
    }
    else
    {
        *circle_cp_ = *circle_cp_save_;

        if (norm(point_mouse_over_inverse_in_unit_disk_.get_affix()) < 1 &&
                norm(point_clicked_inverse_in_unit_disk_.get_affix()) < 1)
        {
            mobius_temp_ = Mobius(-point_mouse_over_inverse_in_unit_disk_.get_affix())*
                    Mobius(point_clicked_inverse_in_unit_disk_.get_affix())*mobius_;
            circle_cp_->apply_mobius(mobius_temp_);
        }

    }

    riemann_positions_->compute_output_circle_packing();
    riemann_positions_->compute_final_triangulation(false);

    //clock_t t1 = clock();
    //std::cout << "Mobius applied in " << (t1 - t0)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    return;
}


void Canvas_Delegate_Riemann_Mapping::paint_event()
{
    /*if (left_canvas_)
    {
        std::cout << "Entering left Canvas_Delegate_Riemann_Mapping::paint_event" << std::endl;
    }
    else
    {
        std::cout << "Entering right Canvas_Delegate_Riemann_Mapping::paint_event" << std::endl;
    }*/

    if (image_mode_ && *draw_2_curves_flag_ == DRAWING_CURVES && !shower_)
    {
        draw_background_image();
    }


    if (tiling_->nb_vertices()>0 && *draw_2_curves_flag_ == DRAWING_CURVES)
    {
        draw_tiling(*tiling_);
    }

    if ((*draw_2_curves_flag_ == DRAWING_CURVES || !shower_)
            && input_triangulation_->nb_triangles() > 0)
    {
        if (input_triangulation_->nb_triangles() == input_triangulation_->nb_colors())
        {
            if (shower_)
            {
                draw_empty_triangulation(*input_triangulation_);
            }
            else
            {

                if (image_mode_)
                {
                    draw_triangulation_from_image();
                }
                else
                {
                    draw_triangulation(*input_triangulation_);
                }
            }
        }
        else
        {
            std::cout << "ERROR in Canvas_Delegate_Riemann_Mapping::paint_event: tried to draw unfit triangulation" << std::endl;
        }
    }
    else if(shower_)
    {
        if (image_mode_)
        {
            draw_shower_triangulation_from_image();
        }
        else
        {
            draw_triangulation(*output_triangulation_);
        }
    }

    if (is_vertex_highlighted_ && (!is_node_point_under_mouse_) && (!is_node_point_selected_) &&
            (*draw_2_curves_flag_==DRAW_2_CURVES_DONE || !shower_) && (!is_tangent_control_point_under_mouse_))
    {
        //std::cout << "about to hightlight triangle" << std::endl;
        if (*draw_2_curves_flag_ == DRAW_2_CURVES_DONE && shower_)
        {
            if (index_vertex_highlighted_ < output_triangulation_->nb_triangles())
            {
                Triangle T;
                if (output_triangulation_->get_triangle(index_vertex_highlighted_, T))
                {
                    mark_triangle(T, QColor("red"));
                }
            }
            else
            {
                std::cout << "ERROR in Canvas_Delegate_Riemann_Mapping::paint_event: tried to draw a triangle with bad index" << std::endl;
            }
        }
        else
        {
            if (index_vertex_highlighted_ < input_triangulation_->nb_triangles())
            {
                Triangle T;
                if (input_triangulation_->get_triangle(index_vertex_highlighted_, T))
                {
                    mark_triangle(T, QColor("red"));
                }
            }
            else
            {
                std::cout << "ERROR in Canvas_Delegate_Riemann_Mapping::paint_event: tried to draw a triangle with bad index" << std::endl;
            }
        }
        //std::cout << "triangle highlighted" << std::endl;
    }

    draw_nodes(*nodes_, nodes_radius_in_pixels_/scale_);

    if (*draw_curve_flag_==DRAW_CURVE_NODES && nodes_->size()>0)
    {
        mark_circle(Circle((*nodes_)[0], nodes_radius_in_pixels_/scale_), QColor("red"));
        draw_incomplete_curve(*nodes_);
    }
    else
    {
        draw_curve(*curve_);
    }

    if (beziers_mode())
    {
        draw_tangents(*nodes_, *tangent_factors_, 0.8*nodes_radius_in_pixels_/scale_);
    }

    if (is_node_point_under_mouse_)
    {
        if(node_point_under_mouse_<nodes_->size())
        {
            mark_circle(Circle((*nodes_)[node_point_under_mouse_], nodes_radius_in_pixels_/scale_), QColor("green"));
        }
        else
        {
            std::cout << "ERROR in Canvas_Delegate_Riemann_Mapping::paint_event: node index problem" << std::endl;
            throw(QString("ERROR in Canvas_Delegate_Riemann_Mapping::paint_event: node index problem"));
        }
    }

    if (is_tangent_control_point_under_mouse_ && !is_node_point_under_mouse_ && beziers_mode())
    {
        if(tangent_control_point_under_mouse_<nodes_->size() && tangent_factors_->size() == nodes_->size())
        {
            unsigned int i = tangent_control_point_under_mouse_, N = nodes_->size();
            Point T((*nodes_)[i].get_affix() +
                    (*tangent_factors_)[i] * ((*nodes_)[(i+1)%N].get_affix() - (*nodes_)[(N+i-1)%N].get_affix()));
            mark_circle(Circle(T, 0.8*nodes_radius_in_pixels_/scale_), QColor("green"));
        }
        else
        {
            std::cout << "ERROR in Canvas_Delegate_Draw_Curve::paint_event: tangent index problem" << std::endl;
            throw(QString("ERROR in Canvas_Delegate_Draw_Curve::paint_event: tangent index problem"));
        }
    }

    /*if (left_canvas_)
    {
        std::cout << "Leaving left Canvas_Delegate_Riemann_Mapping::paint_event" << std::endl;
    }
    else
    {
        std::cout << "Leaving right Canvas_Delegate_Riemann_Mapping::paint_event" << std::endl;
    }*/
    return;
}

void Canvas_Delegate_Riemann_Mapping::mouse_press_event()
{
    set_node_point_under_mouse(point_clicked_);
    if (beziers_mode())
    {
        set_tangent_control_point_under_mouse(point_clicked_);
    }

    if (is_node_point_under_mouse_)
    {
        is_node_point_selected_=true;
        node_point_selected_ = node_point_under_mouse_;
        if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
        {
            if (left_canvas_)
            {
                window_->signal_received(CURVE_READY_OFF);
            }
            else
            {
                window_->signal_received(CURVE_READY_OFF_2);
            }
        }
    }

    else if (beziers_mode() && is_tangent_control_point_under_mouse_)
    {
        is_tangent_control_point_selected_=true;
        tangent_control_point_selected_ = tangent_control_point_under_mouse_;
        if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
        {
            if (left_canvas_)
            {
                window_->signal_received(CURVE_READY_OFF);
            }
            else
            {
                window_->signal_received(CURVE_READY_OFF_2);
            }
        }
    }

    else if (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE)
    {
        set_vertex_under_mouse(point_clicked_);
        set_vertex_highlighted(is_vertex_under_mouse_, index_vertex_under_mouse_);

        if (*draw_2_curves_flag_==DRAW_2_CURVES_DONE)
        {
            if (left_canvas_)
            {
                window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
            }
            else
            {
                window_->signal_received(VERTEX_HIGHLIGHTED_RIGHT);
            };
        }
    }

    if (*draw_curve_flag_==DRAW_CURVE_NODES)
    {
        if (is_node_point_selected_)
        {
            if (node_point_selected_==0 && nodes_->size()>2)
            {
                curve_->create_custom(*nodes_, *tangent_factors_, false);
                if (left_canvas_)
                {
                    window_->signal_received(CURVE_CREATED);
                }
                else
                {
                    window_->signal_received(CURVE_CREATED_2);
                }
            }
        }
        else
        {
            nodes_->push_back(point_clicked_);
            node_point_created_ = true;
            QString qstring = QString("Node added!");
            *message_ = qstring;
            window_->signal_received(MESSAGE);
        }
    }

    if (shower_ && *draw_2_curves_flag_ == DRAW_2_CURVES_DONE && curve_->is_inside(point_clicked_))
    {
        Point P = riemann_positions_->compute_inverse_image_in_unit_disk(point_clicked_, search_tree_);
        if (norm(P.get_affix()) < 1)
        {
            point_clicked_inverse_in_unit_disk_ = P;
        }
    }
    return;
}

void Canvas_Delegate_Riemann_Mapping::mouse_release_event()
{

    set_node_point_under_mouse(point_released_);

    if (beziers_mode())
    {
        set_tangent_control_point_under_mouse(point_released_);
    }


    if ((is_node_point_selected_ || is_tangent_control_point_selected_)
            && (*draw_curve_flag_==DRAW_CURVE_INVALID || *draw_curve_flag_==DRAW_CURVE_READY
                || *draw_curve_flag_==DRAW_CURVE_DONE))
    {
        curve_->create_curve(*nodes_, *tangent_factors_, *curve_choice_flag_, smooth_, nb_sides_, node_point_selected_);
        if (left_canvas_)
        {
            window_->signal_received(CURVE_READY_ON);
        }
        else
        {
            window_->signal_received(CURVE_READY_ON_2);
        }
    }

    if (shower_ && *draw_2_curves_flag_==DRAW_2_CURVES_DONE)// && curve_->is_inside(point_under_mouse_))
    {
        mobius_ = mobius_temp_;

        *message_ = QString("Recomputing triangulation search tree...");
        window_->signal_received(MESSAGE);

        output_search_tree_->reset_search_tree(output_triangulation_);
        window_->signal_received(MESSAGE, -1);
    }

    is_node_point_selected_ = false;
    is_tangent_control_point_selected_ = false;
    return;
}

void Canvas_Delegate_Riemann_Mapping::ctrl_pressed()
{
    if (curve_->is_inside(point_under_mouse_))
    {
        if (click_on_ && shower_)
        {
            point_clicked_inverse_in_unit_disk_ = point_mouse_over_inverse_in_unit_disk_;
            mobius_ = mobius_temp_;
        }
    }
    return;
}

void Canvas_Delegate_Riemann_Mapping::ctrl_released()
{
    if (curve_->is_inside(point_under_mouse_))
    {
        if (click_on_ && shower_)
        {
            point_clicked_inverse_in_unit_disk_ = point_mouse_over_inverse_in_unit_disk_;

            mobius_ = mobius_temp_;
        }

    }
    return;
}

void Canvas_Delegate_Riemann_Mapping::mouse_move_event()
{
    if (!click_on_)
    {
        set_node_point_under_mouse(point_under_mouse_);
    }

    if (beziers_mode() && !click_on_)
    {
        set_tangent_control_point_under_mouse(point_under_mouse_);
    }

    if ((!is_node_point_selected_) && (!is_tangent_control_point_selected_) &&
            (*draw_curve_flag_==DRAW_CURVE_INVALID
             || *draw_curve_flag_==DRAW_CURVE_READY || *draw_curve_flag_==DRAW_CURVE_DONE))
    {
        if ((*draw_2_curves_flag_==DRAW_2_CURVES_DONE || !shower_) &&
                (!click_on_ || QApplication::keyboardModifiers() == Qt::ControlModifier))
        {
            set_vertex_under_mouse(point_under_mouse_);
            set_vertex_highlighted(is_vertex_under_mouse_, index_vertex_under_mouse_);
        }

        if (*draw_2_curves_flag_==DRAW_2_CURVES_DONE && (!click_on_ || QApplication::keyboardModifiers() == Qt::ControlModifier))
        {
            if (left_canvas_)
            {
                window_->signal_received(VERTEX_HIGHLIGHTED_LEFT);
            }
            else
            {
                window_->signal_received(VERTEX_HIGHLIGHTED_RIGHT);
            }
        }
    }

    if (click_on_ && is_node_point_selected_)
    {
        (*nodes_)[node_point_selected_] = point_under_mouse_;
        curve_->create_curve(*nodes_, *tangent_factors_, *curve_choice_flag_, smooth_, nb_sides_, node_point_selected_);
    }
    else if (click_on_ && is_tangent_control_point_selected_
             && beziers_mode())
    {
        unsigned int i = tangent_control_point_selected_, N = nodes_->size();
        complex_number actual_tangent = point_under_mouse_.get_affix() - (*nodes_)[i].get_affix();
        complex_number default_tangent = (*nodes_)[(i+1)%N].get_affix() - (*nodes_)[(N+i-1)%N].get_affix();
        if (actual_tangent != complex_number(0.0, 0.0))
        {
            (*tangent_factors_)[i] = (1.0/default_tangent)*actual_tangent;
        }
        curve_->create_curve(*nodes_, *tangent_factors_, *curve_choice_flag_, smooth_, nb_sides_, node_point_selected_);
    }


    if (click_on_ && shower_ && *draw_2_curves_flag_==DRAW_2_CURVES_DONE)
    {
        if (curve_->is_inside(point_under_mouse_))
        {
            point_mouse_over_inverse_in_unit_disk_ = riemann_positions_->compute_inverse_image_in_unit_disk(
                        point_under_mouse_, search_tree_);
            if (norm(point_mouse_over_inverse_in_unit_disk_.get_affix())<1)
            {
                apply_mobius();
            }
        }
    }

    QString message;
    if (is_node_point_under_mouse_)
    {
        if (node_point_under_mouse_ == 0 && *draw_curve_flag_ == DRAW_CURVE_NODES && nodes_->size()>2)
        {
            message = QString("Click on this node to close the curve");

            *message_ = message;
            window_->signal_received(MESSAGE);
        }
        else if (!(*draw_curve_flag_ == DRAW_CURVE_NODES && node_point_under_mouse_+1 == nodes_->size()
                   && node_point_created_))
        {
            message = QString("Click and drag to move this node");

            *message_ = message;
            window_->signal_received(MESSAGE);
        }
    }
    else
    {
        node_point_created_ = false;
    }

    if (!is_node_point_under_mouse_ && is_tangent_control_point_under_mouse_ && beziers_mode())
    {
        message = QString("Click and drag to move this control point");

        *message_ = message;
        window_->signal_received(MESSAGE);
    }

    if (is_vertex_highlighted_ && !is_node_point_under_mouse_
            && !(is_tangent_control_point_under_mouse_ && beziers_mode()))
    {
        message = QString("Triangle n%1 %2")
                .arg(QChar(0x00B0))
                .arg(index_vertex_highlighted_ + 1);
        *message_ = message;
        window_->signal_received(MESSAGE);
    }

    if (!is_vertex_under_mouse_ && !is_node_point_under_mouse_ &&
            !(is_tangent_control_point_under_mouse_ && beziers_mode()))
    {
        window_->signal_received(MESSAGE, -1);
    }

    return;
}

