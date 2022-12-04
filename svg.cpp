#include "svg.h"

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // ���������� ����� ���� ����� ����������
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

        // ����� �������� ������������ ������� ����� (�������� dx, dy)
    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

        // ����� ������� ������ (������� font-size)
    Text& Text::SetFontSize(uint32_t size) {
        size_ = size;
        return *this;
    }

        // ����� �������� ������ (������� font-family)
    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = font_family;
        return *this;
    }

        // ����� ������� ������ (������� font-weight)
    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = font_weight;
        return *this;
    }

        // ����� ��������� ���������� ������� (������������ ������ ���� text)
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

    // ��������� � svg-�������� ������-��������� svg::Object
    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    // ������� � ostream svg-������������� ���������
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

