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

#ifndef CANVAS_DELEGATE_SHOW_RM_HPP
#define CANVAS_DELEGATE_SHOW_RM_HPP

#include "basic_types.hpp"
#include "canvas_delegate.hpp"
#include "graph.hpp"
#include "mobius.hpp"
#include "affine_transform.hpp"


class QComboBox;
class QImage;

class Triangulation;
class Triangulation_Search_Tree;
class Riemann_Positions;
class Triangle;
class Window;

class Canvas_Delegate_Riemann_Mapping : public Canvas_Delegate
{
public:
    Canvas_Delegate_Riemann_Mapping(const unsigned int &size_in_pixels,
                                    const draw_2_curves_mode_type *const draw_2_curves_flag,
                                    const draw_curve_mode_type *const draw_curve_flag,
                                    const curve_choice_type *const curve_flag,
                                    Curve *const curve, Graph<Circle> *const tiling,
                                    std::vector<Point> *const nodes,
                                    std::vector<complex_number> *const tangent_factors,
                                    Graph<Circle> *const circle_packing,
                                    Triangulation *const input_triangulation, Triangulation *const output_triangulation,
                                    Triangulation *const other_canvas_input_triangulation,
                                    Triangulation_Search_Tree * const search_tree,
                                    Triangulation_Search_Tree * const output_search_tree,
                                    Riemann_Positions * const riemann_positions, Graph<Circle> * const circle_cp,
                                    Graph<Circle> * const circle_cp_save,
                                    Window *window,
                                    QImage *image,
                                    double *image_scale,
                                    int *image_origin_x_in_pixels,
                                    int *image_origin_y_in_pixels,
                                    double *other_canvas_image_scale,
                                    int *other_canvas_image_origin_x_in_pixels,
                                    int *other_canvas_image_origin_y_in_pixels,
                                    bool reset_contents = true);
    void reset_canvas_delegate(bool reset_contents = true); // Does not reset tiling
    void reset_canvas_delegate_for_computing();
    void reset_selected_stuff();
    void set_left(bool left);

    void paint_event();
    void mouse_press_event();
    void mouse_release_event();
    void mouse_move_event();
    void ctrl_released();
    void ctrl_pressed();

    void set_nb_sides(const unsigned int &nb_sides);
    void set_smooth(bool smooth);
    void set_shower(bool shower);

    bool beziers_mode();


private:
    void draw_triangle_from_image(const Triangle &T);
    void draw_triangulation_from_image();
    void draw_shower_triangulation_from_image();
    void set_node_point_under_mouse(const Point &point);
    void set_tangent_control_point_under_mouse(Point const &point);
    void set_vertex_under_mouse(const Point &point);
    void apply_mobius();

    bool shower_;
    const draw_2_curves_mode_type * const draw_2_curves_flag_;
    const draw_graph_mode_type * const draw_curve_flag_;
    const curve_choice_type * const curve_choice_flag_;
    bool smooth_;
    int nb_sides_;

    Curve * const curve_;
    Graph<Circle> * const tiling_;

    std::vector<Point> * const nodes_;
    int nodes_radius_in_pixels_;
    std::vector<complex_number> * const tangent_factors_;

    bool is_node_point_under_mouse_;
    unsigned int node_point_under_mouse_;
    bool is_node_point_selected_;
    unsigned int node_point_selected_;
    bool node_point_created_;

    bool is_tangent_control_point_under_mouse_;
    unsigned int tangent_control_point_under_mouse_;
    bool is_tangent_control_point_selected_;
    unsigned int tangent_control_point_selected_;

    Graph<Circle> * const circle_packing_;
    Triangulation * const input_triangulation_;
    Triangulation * const output_triangulation_;
    Triangulation * const other_canvas_input_triangulation_;
    Triangulation_Search_Tree * const search_tree_;
    Triangulation_Search_Tree * const output_search_tree_;

    Mobius mobius_;
    Mobius mobius_temp_;
    Riemann_Positions * const riemann_positions_;
    Graph<Circle> * const circle_cp_;
    Graph<Circle> * const circle_cp_save_;
    Point point_mouse_over_inverse_in_unit_disk_;
    Point point_clicked_inverse_in_unit_disk_;
    Point point_released_inverse_in_unit_disk_;

    // Draw image stuff
    QImage *image_;
    int image_width_;
    int image_height_;
    QRgb *image_data_;
    bool get_color_from_image(int x, int y, QRgb &color);
    double *image_scale_;
    int *image_origin_x_in_pixels_;
    int *image_origin_y_in_pixels_;
    double *other_canvas_image_scale_;
    int *other_canvas_image_origin_x_in_pixels_;
    int *other_canvas_image_origin_y_in_pixels_;
    bool image_mode_;
    void set_image_mode(bool b);
    void set_image_parameters();
    void draw_background_image();
    Affine_Transform affine_transform_;
};

#endif // CANVAS_DELEGATE_SHOW_RM_HPP
