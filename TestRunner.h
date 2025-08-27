#pragma once
#include <vector>
#include <string>
#include <functional>
#include <iostream>

class TestRunner {
public:
    struct TestCase {
        std::string name;
        std::function<void()> test;
    };

    void addTest(const std::string& name, std::function<void()> test);
    void runTests();

private:
    std::vector<TestCase> tests;
};
