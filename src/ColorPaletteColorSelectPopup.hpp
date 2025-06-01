#pragma once

class ColorPalette;

class ColorPaletteColorSelectPopup : public geode::Popup<> {
private:
    cocos2d::CCSprite* m_sprite;
    ColorPalette* m_palette;
    
    geode::TextInput* m_input;

    void onPaste(cocos2d::CCObject* sender);
    
public:
    static ColorPaletteColorSelectPopup* create(cocos2d::CCSprite* sprite, ColorPalette* palette);
    bool setup() override;

};
