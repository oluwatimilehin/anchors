#include <anchor.h>
#include <engine.h>

#include <iostream>
#include <memory>

int main(int, char**) {
    std::shared_ptr<anchors::Anchor<int>> anchorA(std::make_shared<anchors::Anchor<int>>(2));
    std::shared_ptr<anchors::Anchor<int>> anchorB(std::make_shared<anchors::Anchor<int>>(3));

    auto doubleInputUpdater = [](int a, int b) {
        return a + b;
    };

    std::shared_ptr<anchors::Anchor<int>> anchorC(std::make_shared<anchors::Anchor<int>>(anchors::Anchor<int>::map2(anchorA, anchorB, doubleInputUpdater)));
    anchors::Engine                       engine;

    engine.observe(anchorC);

    std::cout << "Anchor c value: " << engine.get(anchorC) << "\n";
    std::cout << "Anchor a value: " << engine.get(anchorA) << "\n";
    std::cout << "Anchor b value: " << engine.get(anchorB) << "\n";

    engine.set(anchorA, 10);

    // std::cout << "Anchor c value: " << engine.get(anchorC) << "\n";  // Should be 13
    // std::cout << "Anchor a value: " << engine.get(anchorA) << "\n";

    auto singleInputUpdater = [](int a) {
        return a * 3;
    };

    std::shared_ptr<anchors::Anchor<int>>
        anchorD(std::make_shared<anchors::Anchor<int>>(anchors::Anchor<int>::map(anchorC, singleInputUpdater)));

    engine.observe(anchorD);

    std::cout << "Anchor D value: " << engine.get(anchorD) << "\n";  // should be 39
}