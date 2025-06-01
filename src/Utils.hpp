#pragma once

extern geode::Mod* mod;

bool validateHex(const std::string& hex);
int clampInt(int num);
void removeBetterColorPickerToggle(cocos2d::CCMenu* menu);