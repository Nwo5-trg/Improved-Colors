#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectPopup.hpp>
#include <Geode/modify/SetupPulsePopup.hpp>
#include "HexCalculatorPopup.hpp"
#include "ColorPalette.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

$execute {
    mod->setSavedValue<int>("current-palette", 1);
}

class $modify(ColorPopup, ColorSelectPopup) {
	bool init(EffectGameObject* p0, CCArray* p1, ColorAction* p2) {
        if (!ColorSelectPopup::init(p0, p1, p2)) return false;
        auto hexCalculateButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Hex Calc"), this, menu_selector(ColorPopup::onHexCalculator));
        hexCalculateButton->setScale(0.6f);
        hexCalculateButton->m_baseScale = 0.6f;
        m_buttonMenu->addChild(hexCalculateButton);
        if (p0 || p1) {
            hexCalculateButton->setPosition(-155.0f, 90.0f);
            auto palette = ColorPalette::create(0, CCSize(240.0f, 60.0f));
            palette->setPosition(315.0f, 142.0f);
            palette->setScale(0.575f);
            this->addChild(palette);
        } else {
            hexCalculateButton->setPosition(-77.5f, 25.0f);
            auto palette = ColorPalette::create(0, CCSize(240.0f, 60.0f));
            if (mod->getSettingValue<bool>("dont-use-bigger-palette")) {
                palette->setPosition(315.0f, 142.0f);
                palette->setScale(0.575f);
            }
            palette->setPosition(290.0f, 15.0f);
            palette->setScale(0.75f);
            this->addChild(palette);
        }

        return true;
    }

    void onHexCalculator(CCObject* sender) {
        HexCalculatorPopup::create()->show();
    }
};

class $modify(PulsePopup, SetupPulsePopup) {
    struct Fields {
        ColorPalette* palette;
        CCMenuItemSpriteExtra* hexCalculateButton;
    };

	bool init(EffectGameObject* p0, CCArray* p1) {
        if (!SetupPulsePopup::init(p0, p1)) return false;
        auto hexCalculateButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Hex Calc"), this, menu_selector(PulsePopup::onHexCalculator));
        hexCalculateButton->setScale(0.6f);
        hexCalculateButton->m_baseScale = 0.6f;
        hexCalculateButton->setPosition(116.0f, 215.0f);
        m_buttonMenu->addChild(hexCalculateButton);
        m_fields->hexCalculateButton = hexCalculateButton;
        auto palette = ColorPalette::create(1, CCSize(240.0f, 60.0f));
        palette->setPosition(309.0f, 177.0f);
        palette->setScale(0.575f);
        this->addChild(palette);
        m_fields->palette = palette;
        return true;
    }

    void onHexCalculator(CCObject* sender) {
        HexCalculatorPopup::create()->show();
    }

    void onSelectPulseMode(CCObject* sender) {
        SetupPulsePopup::onSelectPulseMode(sender);
        if (m_pulseMode == 1) {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(true);
            if (auto palette = m_fields->palette) palette->setVisible(true);
        } else {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(false);
            if (auto palette = m_fields->palette) palette->setVisible(false);
        }
    }
};