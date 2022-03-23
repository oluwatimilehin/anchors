#include "anchorutil.h"
#include "engine.h"

#include <iostream>

using namespace anchors;

int main(int, char**) {
    // Test with an int
    anchors::Engine engine;
    {
        AnchorPtr<int> anchorA(AnchorUtil::create(2));
        AnchorPtr<int> anchorB(AnchorUtil::create(3));

        auto dualInputUpdater = [](int a, int b) {
            return a + b;
        };

        AnchorPtr<int> anchorC(AnchorUtil::map2(anchorA, anchorB, dualInputUpdater));

        engine.observe(anchorC);

        std::cout << "Anchor c value: " << engine.get(anchorC) << "\n";
        std::cout << "Anchor a value: " << engine.get(anchorA) << "\n";
        std::cout << "Anchor b value: " << engine.get(anchorB) << "\n";

        engine.set(anchorA, 10);

        std::cout << "Anchor c value after setting anchor A to 10: " << engine.get(anchorC) << "\n";  // Should be 13
        std::cout << "Anchor a value: " << engine.get(anchorA) << "\n";

        auto singleInputUpdater = [](int a) {
            return a * 3;
        };

        AnchorPtr<int> anchorD(AnchorUtil::map(anchorC, singleInputUpdater));

        engine.observe(anchorD);
        std::cout << "Anchor D value: " << engine.get(anchorD) << "\n";  // should be 39
    }

    // Test with a string
    {
        std::cout << "\n";
        std::cout << "Test set 2\n";
        AnchorPtr<std::string> username(AnchorUtil::create(std::string("John")));

        auto updater = [](std::string& text) {
            return "Hello, " + text;
        };

        AnchorPtr<std::string> greeting(AnchorUtil::map(username, updater));

        engine.observe(greeting);

        std::cout << "Greeting: " << engine.get(greeting) << "\n";

        engine.set(username, std::string("Samuel"));

        std::cout << "Greeting after changing the value: " << engine.get(greeting) << "\n";
    }

    // More with numbers
    {
        std::cout << "\n";
        std::cout << "Test set 3\n";
        AnchorPtr<int> anchorW(AnchorUtil::create(10));
        AnchorPtr<int> anchorX(AnchorUtil::create(4));

        auto dualInputUpdater = [](int a, int b) {
            std::cout << "a=" << a << "; b=" << b << "\n";  // should print 10 and 4 only once
            return a + b;
        };

        AnchorPtr<int> anchorY(AnchorUtil::map2(anchorW, anchorX, dualInputUpdater));
        AnchorPtr<int> anchorZ(AnchorUtil::create(5));  // TODO:: It'll be great if I could just create an Anchor without the template. Maybe just AnchorUtil::create();

        AnchorPtr<int> resultAnchor(AnchorUtil::map2(anchorY, anchorZ, dualInputUpdater));

        engine.observe(resultAnchor);

        std::cout << "ResultAnchor: " << engine.get(resultAnchor) << "\n";  // Should be 19

        engine.set(anchorZ, 9);
        std::cout << "ResultAnchor: " << engine.get(resultAnchor) << "\n";  // Should be 23. What's important here is it should not recompute W + X
    }
}