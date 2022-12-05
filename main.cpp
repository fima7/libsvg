#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>
#include <vector>

using namespace std::literals;
using namespace svg;

/*
Пример использования библиотеки. Он будет компилироваться и работать, когда вы реализуете
все классы библиотеки.
*/
namespace {

/*
Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays) {
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
    return polyline;
}
*/

// Выводит приветствие, круг и звезду
/*
void DrawPicture() {
    Document doc;
    doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
    doc.Add(Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetFontWeight("bold"s)
                .SetData("Hello C++"s));
    doc.Add(CreateStar({20, 50}, 10, 5, 5));
    doc.Render(std::cout);
}
*/


/*

Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays) {
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
*/
/*
void DrawGreeting() {
    Document doc;
    doc.Add(Circle().SetCenter({ 20, 20 }).SetRadius(10));
    doc.Add(Text()
        .SetFontFamily("Verdana"s)
        .SetPosition({ 35, 20 })
        .SetOffset({ 0, 6 })
        .SetFontSize(12)
        .SetFontWeight("bold"s)
        .SetData("Hello C++"s));
    doc.Add(CreateStar({ 20, 50 }, 10, 5, 5));
    doc.Render(std::cout);
}
*/





}  // namespace

/*


  <polyline points="20,40 22.9389,45.9549 29.5106,46.9098 24.7553,51.5451 25.8779,58.0902 20,55 14.1221,58.0902 15.2447,51.5451 10.4894,46.9098 17.0611,45.9549 20,40 " />
  <polyline points="20,40 22.9389,45.9549 29.5106,46.9098 24.7553,51.5451 25.8779,58.0902 20,55 14.1221,58.0902 15.2447,51.5451 10.4894,46.9098 17.0611,45.9549 20,40" />

*/


template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

static void DrawNicePicture() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;

    picture.emplace_back(make_unique<Triangle>(Point{ 100, 20 }, Point{ 120, 50 }, Point{ 80, 40 }));
    // 5-лучевая звезда с центром {50, 20}, длиной лучей 10 и внутренним радиусом 4
    picture.emplace_back(make_unique<Star>(Point{ 50.0, 20.0 }, 10.0, 4.0, 5));
    // Снеговик с "головой" радиусом 10, имеющей центр в точке {30, 20}
    picture.emplace_back(make_unique<Snowman>(Point{ 30, 20 }, 10.0));

    svg::Document doc;
    // Так как документ реализует интерфейс ObjectContainer,
    // его можно передать в DrawPicture в качестве цели для рисования
    DrawPicture(picture, doc);

    // Выводим полученный документ в stdout
    doc.Render(cout);
}

static void DrawWithColor() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Triangle>(Point{ 100, 20 }, Point{ 120, 50 }, Point{ 80, 40 }));
    picture.emplace_back(make_unique<Star>(Point{ 50.0, 20.0 }, 10.0, 4.0, 5));
    picture.emplace_back(make_unique<Snowman>(Point{ 30, 20 }, 10.0));

    svg::Document doc;
    DrawPicture(picture, doc);

    const Text base_text =  //
        Text()
        .SetFontFamily("Verdana"s)
        .SetFontSize(12)
        .SetPosition({ 10, 100 })
        .SetData("Happy New Year!"s);
    doc.Add(Text{ base_text }
        .SetStrokeColor("yellow"s)
        .SetFillColor("yellow"s)
        .SetStrokeLineJoin(StrokeLineJoin::ROUND)
        .SetStrokeLineCap(StrokeLineCap::ROUND)
        .SetStrokeWidth(3));
    doc.Add(Text{ base_text }.SetFillColor("red"s));

    doc.Render(cout);
}

void TestColors() {
    using namespace svg;
    using namespace std;

    Color none_color;
    cout << none_color << endl; // none

    Color purple{ "purple"s };
    cout << purple << endl; // purple

    Color rgb = Rgb{ 100, 200, 255 };
    cout << rgb << endl; // rgb(100,200,255)

    Color rgba = Rgba{ 100, 200, 255, 0.5 };
    cout << rgba << endl; // rgba(100,200,255,0.5)

    Circle c;
    c.SetRadius(3.5).SetCenter({ 1.0, 2.0 });
    c.SetFillColor(rgba);
    c.SetStrokeColor(purple);

    Document doc;
    doc.Add(std::move(c));
    doc.Render(cout);
}

// Выполняет линейную интерполяцию значения от from до to в зависимости от параметра t
uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

// Выполняет линейную интерполяцию Rgb цвета от from до to в зависимости от параметра t
svg::Rgb Lerp(svg::Rgb from, svg::Rgb to, double t) {
    return { Lerp(from.red, to.red, t), Lerp(from.green, to.green, t), Lerp(from.blue, to.blue, t) };
}

void DrawLerp() {
    using namespace svg;
    using namespace std;

    Rgb start_color{ 0, 255, 30 };
    Rgb end_color{ 20, 20, 150 };

    const int num_circles = 10;
    Document doc;
    for (int i = 0; i < num_circles; ++i) {
        const double t = double(i) / (num_circles - 1);

        const Rgb fill_color = Lerp(start_color, end_color, t);

        doc.Add(Circle()
            .SetFillColor(fill_color)
            .SetStrokeColor("black"s)
            .SetCenter({ i * 20.0 + 40, 40.0 })
            .SetRadius(15));
    }
    doc.Render(cout);
}

static void TestRgba() {
    {
        svg::Rgb rgb{ 255, 0, 100 };
        assert(rgb.red == 255);
        assert(rgb.green == 0);
        assert(rgb.blue == 100);

        // Задаёт цвет по умолчанию: red=0, green=0, blue=0
        svg::Rgb color;
        assert(color.red == 0 && color.green == 0 && color.blue == 0);
    }
    {
        // Задаёт цвет в виде четырёх компонент: red, green, blue, opacity
        svg::Rgba rgba{ 100, 20, 50, 0.3 };
        assert(rgba.red == 100);
        assert(rgba.green == 20);
        assert(rgba.blue == 50);
        assert(rgba.opacity == 0.3);

        // Чёрный непрозрачный цвет: red=0, green=0, blue=0, alpha=1.0
        svg::Rgba color;
        assert(color.red == 0 && color.green == 0 && color.blue == 0 && color.opacity == 1.0);
    }
}

int main() {
    /*
       Это пример для иллюстрации работы класса Circle, данного в заготовке решения.
       После того как вы реализуете реализуете класс Document, аналогичного результата
       можно будет достичь так:

       Document doc;
       doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
       doc.Render(std::cout);
    */
    // Rgb c(1u, 2u, 3u);

    // TestRgba();
    // DrawLerp();
    TestColors();
    // DrawWithColor();
    // DrawNicePicture();
    // DrawGreeting();
    // std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    // std::cout << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;

    // DrawPicture();
    /*
    Circle c;
    c.SetCenter({ 20, 20 }).SetRadius(10);
    RenderContext ctx(std::cout, 2, 2);
    c.Render(ctx);
    */

    // std::cout << "</svg>"sv;
}

