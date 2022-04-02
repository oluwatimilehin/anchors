#include <iostream>

#include "anchorutil.h"
#include "engine.h"

using namespace anchors;

int main(int, char**) {
    // Test with an int
    Engine engine;
    {
        AnchorPtr<int> anchorA(Anchors::create(2));
        AnchorPtr<int> anchorB(Anchors::create(3));

        auto sum = [](int a, int b) { return a + b; };

        AnchorPtr<int> anchorC(Anchors::map2(anchorA, anchorB, sum));

        engine.observe(anchorC);

        std::cout << "Anchor c value: " << engine.get(anchorC) << "\n";
        std::cout << "Anchor a value: " << engine.get(anchorA) << "\n";
        std::cout << "Anchor b value: " << engine.get(anchorB) << "\n";

        engine.set(anchorA, 10);

        std::cout << "Anchor c value after setting anchor A to 10: "
                  << engine.get(anchorC) << "\n";  // Should be 13
        std::cout << "Anchor a value: " << engine.get(anchorA) << "\n";

        auto triple = [](int a) { return a * 3; };

        AnchorPtr<int> anchorD(Anchors::map(anchorC, triple));

        engine.observe(anchorD);
        std::cout << "Anchor D value: " << engine.get(anchorD)
                  << "\n";  // should be 39
    }

    // Test with a string
    {
        std::cout << "\n";
        std::cout << "Test set 2\n";
        AnchorPtr<std::string> username(Anchors::create(std::string("John")));

        auto updater = [](std::string& text) { return "Hello, " + text; };

        AnchorPtr<std::string> greeting(Anchors::map(username, updater));

        engine.observe(greeting);

        std::cout << "Greeting: " << engine.get(greeting) << "\n";

        engine.set(username, std::string("Samuel"));

        std::cout << "Greeting after changing the value: "
                  << engine.get(greeting) << "\n";
    }

    // More with numbers
    {
        std::cout << "\n";
        std::cout << "Test set 3\n";
        AnchorPtr<int> anchorW(Anchors::create(10));
        AnchorPtr<int> anchorX(Anchors::create(4));

        auto dualInputUpdater = [](int a, int b) {
            std::cout << "a=" << a << "; b=" << b
                      << "\n";  // should print 10 and 4 only once
            return a + b;
        };

        AnchorPtr<int> anchorY(
            Anchors::map2(anchorW, anchorX, dualInputUpdater));
        AnchorPtr<int> anchorZ(Anchors::create(5));

        AnchorPtr<int> resultAnchor(
            Anchors::map2(anchorY, anchorZ, dualInputUpdater));

        engine.observe(resultAnchor);

        std::cout << "ResultAnchor: " << engine.get(resultAnchor)
                  << "\n";  // Should be 19

        engine.set(anchorZ, 9);
        std::cout << "ResultAnchor: " << engine.get(resultAnchor)
                  << "\n";  // Should be 23. What's important here is it should
                            // not recompute W + X
    }
}