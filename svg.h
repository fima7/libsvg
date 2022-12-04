#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <optional>

namespace svg {

    struct Point {
        Point() = default;
        Point(double x, double y)
            : x(x)
            , y(y) {
        }
        double x = 0;
        double y = 0;
    };

    /*
     * ��������������� ���������, �������� �������� ��� ������ SVG-��������� � ���������.
     * ������ ������ �� ����� ������, ������� �������� � ��� ������� ��� ������ ��������
     */
    struct RenderContext {
        RenderContext(std::ostream& out)
            : out(out) {
        }

        RenderContext(std::ostream& out, int indent_step, int indent = 0)
            : out(out)
            , indent_step(indent_step)
            , indent(indent) {
        }

        RenderContext Indented() const {
            return { out, indent_step, indent + indent_step };
        }

        void RenderIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }

        std::ostream& out;
        int indent_step = 0;
        int indent = 0;
    };

    /*
     * ����������� ������� ����� Object ������ ��� ���������������� ��������
     * ���������� ����� SVG-���������
     * ��������� ������� "��������� �����" ��� ������ ����������� ����
     */
    class Object {
    public:
        void Render(const RenderContext& context) const;

        virtual ~Object() = default;

    private:
        virtual void RenderObject(const RenderContext& context) const = 0;
    };

    /*
        ��������� ��� ������� � ���������� SVG-��������.
        ����� ���� ��������� Drawable-������� ����� ��������������� ����, �������� � ��������� SVG-���������.
        svg::Document � ���� ������������ ����� ����������, ����������� ��������� ObjectContainer
    */
    class ObjectContainer {
    public:
        template <typename ObjectType>
        void Add(ObjectType obj) {
            AddPtr(std::make_unique<ObjectType>(std::move(obj)));
        }
        virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;
    protected:
        ~ObjectContainer() = default;
    };

    /*
        ����������� ������ � ���������, ������� ����� ����������, ��������� SVG-����������.
        ��� ����� � �� ���� ����� Draw, ����������� ������ �� ��������� ObjectContainer.
    */
    struct Drawable {
        virtual void Draw(ObjectContainer& container) const = 0;
        virtual ~Drawable() = default;
    };


    using Color = std::string;

    // ������� � ������������ ����� ��������� �� �������������� inline,
    // �� ������� ���, ��� ��� ����� ����� �� ��� ������� ����������,
    // ������� ���������� ���� ���������.
    // � ��������� ������ ������ ������� ���������� ����� ������������ ���� ����� ���� ���������
    inline const Color NoneColor{ "none" };

    enum class StrokeLineCap {
        BUTT,
        ROUND,
        SQUARE,
    };

    std::ostream& operator<<(std::ostream& os, StrokeLineCap linecap);

    enum class StrokeLineJoin {
        ARCS,
        BEVEL,
        MITER,
        MITER_CLIP,
        ROUND,
    };

    std::ostream& operator<<(std::ostream& os, StrokeLineJoin linejoin);

    template <typename Owner>
    class PathProps {
    public:
        Owner& SetFillColor(Color color) {
            fill_color_ = color;
            return static_cast<Owner&>(*this);
        }
        Owner& SetStrokeColor(Color color) {
            stroke_color_ = color;
            return static_cast<Owner&>(*this);
        }
        Owner& SetStrokeWidth(double width) {
            stroke_width_ = width;
            return static_cast<Owner&>(*this);
        }
        Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
            stroke_linecap_ = line_cap;
            return static_cast<Owner&>(*this);
        }
        Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
            stroke_linejoin_ = line_join;
            return static_cast<Owner&>(*this);
        }
    protected:
        ~PathProps() = default;

        void RenderAttrs(std::ostream& os) const {
            using namespace std::literals;
            if (fill_color_) {
                os << " fill=\""sv << *fill_color_ << "\""sv;
            }
            if (stroke_color_) {
                os << " stroke=\""sv << *stroke_color_ << "\""sv;
            }
            if (stroke_width_) {
                os << " stroke-width=\""sv << *stroke_width_ << "\""sv;
            }
            if (stroke_linecap_) {
                os << " stroke-linecap=\""sv << *stroke_linecap_ << "\""sv;
            }
            if (stroke_linejoin_) {
                os << " stroke-linejoin=\""sv << *stroke_linejoin_ << "\""sv;
            }

        }
    private:
        std::optional<Color> fill_color_;
        std::optional<Color> stroke_color_;
        std::optional<double> stroke_width_;
        std::optional<StrokeLineCap> stroke_linecap_;
        std::optional<StrokeLineJoin> stroke_linejoin_;
    };

    /*
     * ����� Circle ���������� ������� <circle> ��� ����������� �����
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
     */
    class Circle final : public Object, public PathProps<Circle> {
    public:
        Circle& SetCenter(Point center);
        Circle& SetRadius(double radius);

    private:
        void RenderObject(const RenderContext& context) const override;

    private:
        Point center_;
        double radius_ = 1.0;
    };

    /*
     * ����� Polyline ���������� ������� <polyline> ��� ����������� ������� �����
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
     */
    class Polyline final : public Object, public PathProps<Polyline> {
    public:
        // ��������� ��������� ������� � ������� �����
        Polyline& AddPoint(Point point);

        /*
         * ������ ������ � ������, ����������� ��� ���������� �������� <polyline>
         */
    private:
        void RenderObject(const RenderContext& context) const override;

    private:
        std::list<Point> points_;
    };

    /*
     * ����� Text ���������� ������� <text> ��� ����������� ������
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
     */
    class Text final : public Object, public PathProps<Text> {
    public:
        // ����� ���������� ������� ����� (�������� x � y)
        Text& SetPosition(Point pos);

        // ����� �������� ������������ ������� ����� (�������� dx, dy)
        Text& SetOffset(Point offset);

        // ����� ������� ������ (������� font-size)
        Text& SetFontSize(uint32_t size);

        // ����� �������� ������ (������� font-family)
        Text& SetFontFamily(std::string font_family);

        // ����� ������� ������ (������� font-weight)
        Text& SetFontWeight(std::string font_weight);

        // ����� ��������� ���������� ������� (������������ ������ ���� text)
        Text& SetData(std::string data);

        // ������ ������ � ������, ����������� ��� ���������� �������� <text>
    private:
        void RenderObject(const RenderContext& context) const override;

    private:
        Point pos_;
        Point offset_;
        uint32_t size_ = 1;
        std::string font_family_;
        std::string font_weight_;
        std::string data_;

    };

    class Document : public ObjectContainer {
    public:
        /*
         ����� Add ��������� � svg-�������� ����� ������-��������� svg::Object.
         ������ �������������:
         Document doc;
         doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
        */

        // ��������� � svg-�������� ������-��������� svg::Object
        void AddPtr(std::unique_ptr<Object>&& obj) override;

        // ������� � ostream svg-������������� ���������
        void Render(std::ostream& out) const;

        // ������ ������ � ������, ����������� ��� ���������� ������ Document
    private:
        std::list<std::unique_ptr<Object>> objects_;
    };

}  // namespace svg

namespace shapes {
    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3);
        void Draw(svg::ObjectContainer& container) const override;
    private:
        svg::Point p1_, p2_, p3_;
    };

    class Star : public svg::Drawable { 
    public:
        Star(svg::Point center, double outer_rad, double inner_rad, int num_rays);
        void Draw(svg::ObjectContainer& container) const override;
    private:
        svg::Point center_;
        double outer_rad_ = 0.0;
        double inner_rad_ = 0.0;
        int num_rays_ = 0;
    };

    class Snowman : public svg::Drawable {
    public:
        Snowman(svg::Point head_center, double head_radius);
        void Draw(svg::ObjectContainer& container) const override;
    private:
        svg::Point head_center_;
        double head_radius_ = 0.0;

    };

} // namespace shapes 

