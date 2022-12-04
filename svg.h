#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <list>

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
     * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
     * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
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
     * Абстрактный базовый класс Object служит для унифицированного хранения
     * конкретных тегов SVG-документа
     * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
     */
    class Object {
    public:
        void Render(const RenderContext& context) const;

        virtual ~Object() = default;

    private:
        virtual void RenderObject(const RenderContext& context) const = 0;
    };

    /*
        Интерфейс для доступа к контейнеру SVG-объектов.
        Через этот интерфейс Drawable-объекты могут визуализировать себя, добавляя в контейнер SVG-примитивы.
        svg::Document — пока единственный класс библиотеки, реализующий интерфейс ObjectContainer
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
        Унифицирует работу с объектами, которые можно нарисовать, подключив SVG-библиотеку.
        Для этого в нём есть метод Draw, принимающий ссылку на интерфейс ObjectContainer.
    */
    struct Drawable {
        virtual void Draw(ObjectContainer& container) const = 0;
        virtual ~Drawable() = default;
    };


  

    /*
     * Класс Circle моделирует элемент <circle> для отображения круга
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
     */
    class Circle final : public Object {
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
     * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
     */
    class Polyline final : public Object {
    public:
        // Добавляет очередную вершину к ломаной линии
        Polyline& AddPoint(Point point);

        /*
         * Прочие методы и данные, необходимые для реализации элемента <polyline>
         */
    private:
        void RenderObject(const RenderContext& context) const override;

    private:
        std::list<Point> points_;
    };

    /*
     * Класс Text моделирует элемент <text> для отображения текста
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
     */
    class Text final : public Object {
    public:
        // Задаёт координаты опорной точки (атрибуты x и y)
        Text& SetPosition(Point pos);

        // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
        Text& SetOffset(Point offset);

        // Задаёт размеры шрифта (атрибут font-size)
        Text& SetFontSize(uint32_t size);

        // Задаёт название шрифта (атрибут font-family)
        Text& SetFontFamily(std::string font_family);

        // Задаёт толщину шрифта (атрибут font-weight)
        Text& SetFontWeight(std::string font_weight);

        // Задаёт текстовое содержимое объекта (отображается внутри тега text)
        Text& SetData(std::string data);

        // Прочие данные и методы, необходимые для реализации элемента <text>
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
         Метод Add добавляет в svg-документ любой объект-наследник svg::Object.
         Пример использования:
         Document doc;
         doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
        */

        // Добавляет в svg-документ объект-наследник svg::Object
        void AddPtr(std::unique_ptr<Object>&& obj) override;

        // Выводит в ostream svg-представление документа
        void Render(std::ostream& out) const;

        // Прочие методы и данные, необходимые для реализации класса Document
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

