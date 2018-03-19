#include <vector>
using std::vector;
#include <string>
using std::string;
#include <utility>
#include "gtest/gtest.h"
#include "flag/boostflagparser.cpp"

using namespace beta;

class BoostFlagParserTest : public testing::Test {
public:
    /** Parses flags which appear after the program name */
    void parseFlags(BoostFlagParser &parser, vector<string> args);
    /** Joins the arguments into a single string */
    string asString(vector<string> args);
};

void BoostFlagParserTest::parseFlags(BoostFlagParser &parser, vector<string> args) {
    args.insert(args.begin(), "beta");
    std::vector<char*> argv;
    for (const auto &arg : args) argv.push_back((char*)arg.data());
    argv.push_back(nullptr);
    
    parser.parse(args.size(), argv.data());
}

std::string BoostFlagParserTest::asString(vector<string> args) {
    std::string str = "< ";
    for (auto arg : args) str += "'" + arg + "' ";
    return str + ">";
}

TEST_F(BoostFlagParserTest, wantsHelp) {
    // [<argument list, was help flag given>...]
    std::vector<std::pair<vector<string>, bool>> testCases;
    #define ADD_CASE(WANTS_HELP, ...) testCases.emplace_back(vector<string>{__VA_ARGS__}, WANTS_HELP)
    ADD_CASE(true, "--help");
    ADD_CASE(false, "--hulp");
    ADD_CASE(false, "--provider", "mockprovider");

    for (auto testCase : testCases) {
        BoostFlagParser parser;
        parseFlags(parser, testCase.first);
        ASSERT_EQ(testCase.second, parser.wantsHelp()) << "{"
            << asString(testCase.first) << ", "
            << (testCase.second ? "true" : "false") << "}";
    }
}

TEST_F(BoostFlagParserTest, provider) {
    // [<argument list, expected provider name>...]
    std::pair<vector<string>, string> testCases[] = {
        std::make_pair(vector<string>{}, ""),
        std::make_pair(vector<string>{"--provider", "mock"}, "mock")
    };

    for (auto testCase : testCases) {
        BoostFlagParser parser;
        parseFlags(parser, testCase.first);
        ASSERT_EQ(testCase.second, parser.provider()) << "{"
            << asString(testCase.first) << ", "
            << testCase.second << "}";
    }
}