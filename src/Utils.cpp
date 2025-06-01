#include <Geode/Geode.hpp>
#include "Utils.hpp"

using namespace geode::prelude;

auto mod = Mod::get();

bool validateHex(const std::string& hex) {
    if (hex.length() != 6) return false;
    for (auto c : hex) if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) return false;
    return true;
}

int clampInt(int num) {
    if (num < 0) return 0;
    if (num > 255) return 255;
    return num;
}