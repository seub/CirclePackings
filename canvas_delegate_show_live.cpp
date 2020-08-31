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

#include "canvas_delegate_show_live.hpp"
#include "graph.hpp"

#include <QPainter>

Canvas_Delegate_Show_Live::Canvas_Delegate_Show_Live(const int &size_in_pixels, Graph<Circle> *const circle_packing) :
    circle_packing_(circle_packing)
{
    size_in_pixels_ = size_in_pixels;
    common_reset();
}

void Canvas_Delegate_Show_Live::paint_event()
{
    painter_->setOpacity(0.9);
    draw_circle_packing(*circle_packing_);
    painter_->setOpacity(1.0);
    return;
}
