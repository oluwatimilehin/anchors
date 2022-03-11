#include <anchor.h>
#include <engine.h>

#include <iostream>

using namespace anchors;

int main(int, char**) {
    // Test with an int
    anchors::Engine engine;
    {
        AnchorPtr<int> anchorA(Anchor<int>::create(2));
        AnchorPtr<int> anchorB(Anchor<int>::create(3));

        auto dualInputUpdater = [](int a, int b) {
            return a + b;
        };

        AnchorPtr<int> anchorC(Anchor<int>::map2(anchorA, anchorB, dualInputUpdater));

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

        AnchorPtr<int> anchorD(Anchor<int>::map(anchorC, singleInputUpdater));

        engine.observe(anchorD);
        std::cout << "Anchor D value: " << engine.get(anchorD) << "\n";  // should be 39
    }

    // Test with a string
    {
        AnchorPtr<std::string> username(Anchor<std::string>::create("John"));

        auto updater = [](std::string& text) {
            return "Hello, " + text;
        };

        AnchorPtr<std::string> greeting(Anchor<std::string>::map(username, updater));

        engine.observe(greeting);

        std::cout << "Greeting: " << engine.get(greeting) << "\n";

        engine.set(username, std::string("Samuel"));

        std::cout << "Greeting after changing the value: " << engine.get(greeting) << "\n";
    }
}