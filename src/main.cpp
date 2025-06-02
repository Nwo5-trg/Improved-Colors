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

// not the cleanest but like idrc

class $modify(ColorPopup, ColorSelectPopup) {
    struct Fields {
        ColorPalette* palette;
        CCMenuItemSpriteExtra* hexCalculateButton;
        CCMenuItemSpriteExtra* copyButton;
        CCMenuItemSpriteExtra* pasteButton;
    };
    
    static void onModify(auto& self) { // better color picker
        (void)self.setHookPriority("ColorSelectPopup::init", Priority::LastPost);
    }

	bool init(EffectGameObject* p0, CCArray* p1, ColorAction* p2) {
        if (!ColorSelectPopup::init(p0, p1, p2)) return false;

        if (mod->getSettingValue<bool>("enable-hex-calculator")) {
            auto hexCalculateButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Hex Calc"), this, menu_selector(ColorPopup::onHexCalculator));
            hexCalculateButton->setScale(0.6f);
            hexCalculateButton->m_baseScale = 0.6f;
            hexCalculateButton->setPosition(-155.0f, 110.0f);
            m_buttonMenu->addChild(hexCalculateButton);
            m_fields->hexCalculateButton = hexCalculateButton;
        }
        
        if (mod->getSettingValue<bool>("enable-palette")) {
            if (p0 || p1) {
                auto palette = ColorPalette::create(0, CCSize(240.0f, 60.0f));
                palette->setPosition(315.0f, 142.0f);
                palette->setScale(0.575f);
                this->addChild(palette);
                m_fields->palette = palette;
            } else {
                auto palette = ColorPalette::create(0, CCSize(240.0f, 60.0f));
                if (mod->getSettingValue<bool>("dont-use-bigger-palette")) {
                    palette->setPosition(315.0f, 142.0f);
                    palette->setScale(0.575f);
                } else {
                    palette->setPosition(290.0f, 15.0f);
                    palette->setScale(0.75f);
                }
                this->addChild(palette);
                m_fields->palette = palette;
            }
            
            if (Loader::get()->isModLoaded("anatom3000.bettercolorpicker")) removeBetterColorPickerToggle(m_buttonMenu); // safe prolly
        }

        if (mod->getSettingValue<bool>("enable-copy-paste-hex")) {
            auto copyButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_copyBtn_001.png"), this, menu_selector(ColorPopup::onCopyHex));
            if (mod->getSettingValue<bool>("swap-copy-paste-buttons")) copyButton->setPosition(-195.0f, 135.0f);
            else copyButton->setPosition(-113.0f, 135.0f);
            copyButton->setScale(0.3f);
            copyButton->m_baseScale = 0.3f;
            m_buttonMenu->addChild(copyButton);
            m_fields->copyButton = copyButton;

            auto pasteButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_pasteBtn_001.png"), this, menu_selector(ColorPopup::onPasteHex));
            if (mod->getSettingValue<bool>("swap-copy-paste-buttons")) pasteButton->setPosition(-113.0f, 135.0f);
            else pasteButton->setPosition(-195.0f, 135.0f);
            pasteButton->setScale(0.3f);
            pasteButton->m_baseScale = 0.3f;
            m_buttonMenu->addChild(pasteButton);
            m_fields->pasteButton = pasteButton;
        }

        if (m_hsvWidget->isVisible()) { // i love fucking with null checks int he worst way ever
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(false);
            if (auto palette = m_fields->palette) palette->setVisible(false);
            if (auto copyButton = m_fields->copyButton) copyButton->setVisible(false);
            if (auto pasteButton = m_fields->pasteButton) pasteButton->setVisible(false);
        }

        return true;
    }

    void onHexCalculator(CCObject* sender) {
        HexCalculatorPopup::create()->show();
    }

    void onToggleHSVMode(CCObject* sender) {
        ColorSelectPopup::onToggleHSVMode(sender);
        if (!m_hsvWidget->isVisible()) {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(true);
            if (auto palette = m_fields->palette) palette->setVisible(true);
            if (auto copyButton = m_fields->copyButton) copyButton->setVisible(true);
            if (auto pasteButton = m_fields->pasteButton) pasteButton->setVisible(true);
        } else {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(false);
            if (auto palette = m_fields->palette) palette->setVisible(false);
            if (auto copyButton = m_fields->copyButton) copyButton->setVisible(false);
            if (auto pasteButton = m_fields->pasteButton) pasteButton->setVisible(false);
        }
    }

    void onCopyHex(CCObject* sender) {
        auto string = m_hexInput->getString();
        if (!string.empty()) clipboard::write(string);
    }

    void onPasteHex(CCObject* sender) {
        auto string = clipboard::read();
        if (string.empty()) return;
        if (!validateHex(string)) return; // i dont trust robtop with inputting an invalid hex
        if (string.front() == '#') string.erase(0, 1);
        m_hexInput->setString(string);
    }
};

class $modify(PulsePopup, SetupPulsePopup) {
    struct Fields {
        ColorPalette* palette;
        CCMenuItemSpriteExtra* hexCalculateButton;
        CCMenuItemSpriteExtra* copyButton;
        CCMenuItemSpriteExtra* pasteButton;
    };

    static void onModify(auto& self) {
        (void)self.setHookPriority("SetupPulsePopup::init", Priority::LastPost);
    }

	bool init(EffectGameObject* p0, CCArray* p1) {
        if (!SetupPulsePopup::init(p0, p1)) return false;

        if (mod->getSettingValue<bool>("enable-hex-calculator")) {
            auto hexCalculateButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Hex Calc"), this, menu_selector(PulsePopup::onHexCalculator));
            hexCalculateButton->setScale(0.6f);
            hexCalculateButton->m_baseScale = 0.6f;
            hexCalculateButton->setPosition(116.0f, 215.0f);
            m_buttonMenu->addChild(hexCalculateButton);
            m_fields->hexCalculateButton = hexCalculateButton;
        }

        if (mod->getSettingValue<bool>("enable-palette")) {
            auto palette = ColorPalette::create(1, CCSize(240.0f, 60.0f));
            palette->setPosition(309.0f, 177.0f);
            palette->setScale(0.575f);
            this->addChild(palette);
            m_fields->palette = palette;
            if (Loader::get()->isModLoaded("anatom3000.bettercolorpicker")) removeBetterColorPickerToggle(m_buttonMenu);
        }
        
        // fuck refactoring thats for bitches
        if (mod->getSettingValue<bool>("enable-copy-paste-hex")) {
            auto copyButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_copyBtn_001.png"), this, menu_selector(PulsePopup::onCopyHex));
            if (mod->getSettingValue<bool>("swap-copy-paste-buttons")) copyButton->setPosition(-130.0f, 122.0f);
            else copyButton->setPosition(-111.0f, 122.0f);
            copyButton->setScale(0.3f);
            copyButton->m_baseScale = 0.3f;
            m_buttonMenu->addChild(copyButton);
            m_fields->copyButton = copyButton;

            auto pasteButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_pasteBtn_001.png"), this, menu_selector(PulsePopup::onPasteHex));
            if (mod->getSettingValue<bool>("swap-copy-paste-buttons")) pasteButton->setPosition(-111.0f, 122.0f);
            else pasteButton->setPosition(-130.0f, 122.0f);
            pasteButton->setScale(0.3f);
            pasteButton->m_baseScale = 0.3f;
            m_buttonMenu->addChild(pasteButton);
            m_fields->pasteButton = pasteButton;
        }

        if (m_hsvWidget->isVisible()) {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(false);
            if (auto palette = m_fields->palette) palette->setVisible(false);
            if (auto copyButton = m_fields->copyButton) copyButton->setVisible(false);
            if (auto pasteButton = m_fields->pasteButton) pasteButton->setVisible(false);
        }

        return true;
    }

    void onHexCalculator(CCObject* sender) {
        HexCalculatorPopup::create()->show();
    }

    void onSelectPulseMode(CCObject* sender) {
        SetupPulsePopup::onSelectPulseMode(sender);
        if (!m_hsvWidget->isVisible()) {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(true);
            if (auto palette = m_fields->palette) palette->setVisible(true);
            if (auto copyButton = m_fields->copyButton) copyButton->setVisible(true);
            if (auto pasteButton = m_fields->pasteButton) pasteButton->setVisible(true);
        } else {
            if (auto hexCalculateButton = m_fields->hexCalculateButton) hexCalculateButton->setVisible(false);
            if (auto palette = m_fields->palette) palette->setVisible(false);
            if (auto copyButton = m_fields->copyButton) copyButton->setVisible(false);
            if (auto pasteButton = m_fields->pasteButton) pasteButton->setVisible(false);
        }
    }

    void onCopyHex(CCObject* sender) {
        auto string = m_hexInput->getString();
        if (!string.empty()) clipboard::write(string);
    }

    void onPasteHex(CCObject* sender) {
        auto string = clipboard::read();
        if (string.empty()) return;
        if (!validateHex(string)) return;
        if (string.front() == '#') string.erase(0, 1);
        m_hexInput->setString(string);
    }
};