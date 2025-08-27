#include "TestRunner.h"

void TestRunner::addTest(const std::string& name, std::function<void()> test) {
    tests.push_back({name, test});
}

void TestRunner::runTests() {
    std::cout << "=== Running Tests ===" << std::endl;
    for (const auto& testCase : tests) {
        std::cout << "Running test: " << testCase.name << std::endl;
        try {
            testCase.test();
            std::cout << "Test passed: " << testCase.name << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Test failed: " << testCase.name << " with exception: " << e.what() << std::endl;
        }
    }
    std::cout << "=====================" << std::endl;
}
