#define _USE_MATH_DEFINES 
#include "svg.h"

#include <cmath> 

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // Делегируем вывод тега своим подклассам
        RenderObject(context);

        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\" "sv;
        out << "/>"sv;
    }


    // ---------- Polyline ------------------
    Polyline& Polyline::AddPoint(Point point) {
        points_.push_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        context.out << "<polyline points=\"";
        bool first = true;
        for (const auto& point : points_) {
            if (!first) {
                context.out << " ";
            }
            else {
                first = false;
            }
            context.out << point.x << "," << point.y;
        }
        context.out << "\" />";
    }


    // ---------- Text ------------------
    Text& Text::SetPosition(Point pos) {
        pos_ = pos;
        return *this;
    }

        // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

        // Задаёт размеры шрифта (атрибут font-size)
    Text& Text::SetFontSize(uint32_t size) {
        size_ = size;
        return *this;
    }

        // Задаёт название шрифта (атрибут font-family)
    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = font_family;
        return *this;
    }

        // Задаёт толщину шрифта (атрибут font-weight)
    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = font_weight;
        return *this;
    }

        // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& Text::SetData(std::string data) {
        data_ = data;
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        context.out << "<text "
            << "x=\"" << pos_.x << "\" " << "y=\"" << pos_.y << "\" "
            << "dx=\"" << offset_.x << "\" " << "dy=\"" << offset_.y << "\" "
            << "font-size=\"" << size_ << "\" ";

        if (!font_family_.empty()) {
            context.out << "font-family=\"" << font_family_ << "\" ";
        }
        
        if (!font_weight_.empty()) {
            context.out << "font-weight=\"" << font_weight_ << "\"";
        }

        context.out << ">";
        for (const char& c : data_) {
            switch (c) {
            case '\"':
                context.out << "&quot;";
                break;
            case '<':
                context.out << "&lt;";
                break;
            case '>':
                context.out << "&gt;";
                break;
            case '\'':
                context.out << "&apos;";
                break;
            case '&':
                context.out << "&amp;";
                break;
            default:
                context.out << c;
            }
        }
        context.out << "</text>";
    }

    // ---------- Document ------------------

    // Добавляет в svg-документ объект-наследник svg::Object
    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    // Выводит в ostream svg-представление документа
    void Document::Render(std::ostream& out) const {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << std::endl;
        RenderContext context(out, 2, 2);
        for (const auto& obj : objects_) {
            obj->Render(context);
        }
        out << "</svg>";
    }
        

}  // namespace svg


namespace {
    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) {
        using namespace svg;
        Polyline polyline;
        for (int i = 0; i <= num_rays; ++i) {
            double angle = 2 * M_PI * (i % num_rays) / num_rays;
            polyline.AddPoint({ center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle) });
            if (i == num_rays) {
                break;
            }
            angle += M_PI / num_rays;
            polyline.AddPoint({ center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle) });
        }
        return polyline;
    }
}

namespace shapes {

Triangle::Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
    : p1_(p1)
    , p2_(p2)
    , p3_(p3)
{}

void Triangle::Draw(svg::ObjectContainer& container) const {
    container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
}


Star::Star(svg::Point center, double outer_rad, double inner_rad, int num_rays)
    : center_(center)
    , outer_rad_(outer_rad)
    , inner_rad_(inner_rad)
    , num_rays_(num_rays)
{}

void Star::Draw(svg::ObjectContainer& container) const {
    container.Add(CreateStar(center_, outer_rad_, inner_rad_, num_rays_));
}

Snowman::Snowman(svg::Point head_center, double head_radius)
    : head_center_(head_center)
    , head_radius_(head_radius)
{}

void Snowman::Draw(svg::ObjectContainer& container) const {
    container.Add(svg::Circle().SetCenter({ head_center_.x, head_center_.y + 5.0 * head_radius_ }).SetRadius(2.0 * head_radius_));
    container.Add(svg::Circle().SetCenter({ head_center_.x, head_center_.y + 2.0 * head_radius_ }).SetRadius(1.5 * head_radius_));
    container.Add(svg::Circle().SetCenter(head_center_).SetRadius(head_radius_));
}

} // namespace shapes 

