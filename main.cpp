#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

/*
ѕример использовани€ библиотеки. ќн будет компилироватьс€ и работать, когда вы реализуете
все классы библиотеки.
*/
namespace {

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

// ¬ыводит приветствие, круг и звезду
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





}  // namespace

/*


  <polyline points="20,40 22.9389,45.9549 29.5106,46.9098 24.7553,51.5451 25.8779,58.0902 20,55 14.1221,58.0902 15.2447,51.5451 10.4894,46.9098 17.0611,45.9549 20,40 " />
  <polyline points="20,40 22.9389,45.9549 29.5106,46.9098 24.7553,51.5451 25.8779,58.0902 20,55 14.1221,58.0902 15.2447,51.5451 10.4894,46.9098 17.0611,45.9549 20,40" />

*/

int main() {
    /*
       Ёто пример дл€ иллюстрации работы класса Circle, данного в заготовке решени€.
       ѕосле того как вы реализуете реализуете класс Document, аналогичного результата
       можно будет достичь так:

       Document doc;
       doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
       doc.Render(std::cout);
    */
    DrawGreeting();
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

