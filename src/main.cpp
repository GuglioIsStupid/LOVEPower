#define SOL_ALL_SAFETIES_ON 1

#include "love/love.hpp"

int main(int argc, char** argv) {
    try {
        return love::initialize(argc, argv);
    } catch (const std::exception &e) {
        love::logError(std::string("Fatal exception: ") + e.what());
    } catch (...) {
        love::logError("Fatal unknown exception");
    }
    return -1;
}
