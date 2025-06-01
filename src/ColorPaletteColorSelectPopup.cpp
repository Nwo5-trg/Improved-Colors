#include <Geode/Geode.hpp>
#include "ColorPalette.hpp"
#include "ColorPaletteColorSelectPopup.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

ColorPaletteColorSelectPopup* ColorPaletteColorSelectPopup::create(CCSprite* sprite, ColorPalette* palette) { 
    auto ret = new ColorPaletteColorSelectPopup();
    ret->m_sprite = sprite;
    ret->m_palette = palette;
    if (ret->initAnchored(150.0f, 75.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ColorPaletteColorSelectPopup::setup() {
    m_mainLayer->getChildByType<CCScale9Sprite>(0)->setOpacity(0);
    
    auto colorInput = TextInput::create(90.0f, "FFFFFF");
    if (m_sprite->getColor() != ccWHITE) colorInput->setString(cc3bToHexString(m_sprite->getColor()));
    colorInput->setCommonFilter(CommonFilter::Hex);
    colorInput->setCallback([this, colorInput] (const std::string& input) {
        if (input.length() > 6) {
            colorInput->setString(input.substr(0, 6));
            return;
        }
        if (validateHex(input)) {
            m_sprite->setColor(*cc3bFromHexString(input).ok());
            m_palette->update(false);
        }
        if (input.empty()) {
            m_sprite->setColor(ccWHITE);
            m_palette->update(false);
        }
    });
    colorInput->setScale(1.5f);
    colorInput->setPosition(m_size / 2);
    m_mainLayer->addChild(colorInput);
    m_input = colorInput;

    auto pasteButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_pasteBtn_001.png"), this, menu_selector(ColorPaletteColorSelectPopup::onPaste));
    pasteButton->setScale(0.75f);
    pasteButton->m_baseScale = 0.75f;
    pasteButton->setPosition((m_size.width / 2) + 100, m_size.height / 2);
    m_buttonMenu->addChild(pasteButton);

    m_closeBtn->setPosition((m_size.width / 2) - 100, m_size.height / 2);

    return true;
}

void ColorPaletteColorSelectPopup::onPaste(CCObject* sender) {
    auto string = clipboard::read();
    if (string.empty()) return;
    if (string.front() == '#') string.erase(0, 1);
    m_input->setString(string, true);
}