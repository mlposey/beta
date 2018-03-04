#include "gtest/gtest.h"
#include "provider/providerloader.cpp"
#include "provider/provideradapter.cpp"

using namespace beta;

class ProviderLoaderTest : public testing::Test {
public:
    /** Builds a .so from a mock source file */
    void buildMockLib(std::string filename);
};

void ProviderLoaderTest::buildMockLib(std::string filename) {
    // This assumes the tests are being run from inside the build directory
    // CMake will copy the mock files in to the build directory when calling "cmake ."
    std::string cmd = "g++ -std=c++14 -I ./inc -fpic -c mock/" + filename + " -o mockprovider.o";
    system(cmd.c_str());
    system("g++ -shared -o mockprovider.so mockprovider.o");
}

TEST_F(ProviderLoaderTest, load_validLib) {
    buildMockLib("mockprovider_valid.cpp");
    ProviderLoader loader(".");
    std::shared_ptr<PathProvider> provider = loader.load("mockprovider.so")->provider();

    ASSERT_EQ("MockProvider", provider->name());
    ASSERT_EQ("Mock", provider->author());
    ASSERT_FLOAT_EQ(0.1, provider->version());
    ASSERT_FALSE(provider->findPath(0, 0, 0, 0).empty());
}

TEST_F(ProviderLoaderTest, load_missingLib) {
    ProviderLoader loader(".");
    ASSERT_ANY_THROW(loader.load("wefoiwefoij23243wfwefpaief.so"));
}

TEST_F(ProviderLoaderTest, load_missingName) {
    buildMockLib("mockprovider_missingName.cpp");
    ProviderLoader loader(".");
    ASSERT_ANY_THROW(loader.load("mockprovider.so"));    
}

TEST_F(ProviderLoaderTest, load_missingAuthor) {
    buildMockLib("mockprovider_missingAuthor.cpp");
    ProviderLoader loader(".");
    ASSERT_ANY_THROW(loader.load("mockprovider.so"));    
}