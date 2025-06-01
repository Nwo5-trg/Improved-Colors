#include <Geode/Geode.hpp>
#include "HexCalculatorPopup.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

HexCalculatorPopup* HexCalculatorPopup::create() { 
    auto ret = new HexCalculatorPopup();
    if (ret->initAnchored(320.0f, 280.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool HexCalculatorPopup::setup() {
    this->setTitle("Hex Calculator");

    m_buttonMenu->setTouchPriority(-1000);

    auto operatorToggler = CCMenuItemToggler::create(CCSprite::create("minusoperatorbutton.png"_spr), 
    CCSprite::create("plusoperatorbutton.png"_spr), this, menu_selector(HexCalculatorPopup::onOperatorToggle));
    operatorToggler->toggle(true);
    operatorToggler->setPosition(m_size.width / 2, (m_size.height / 2) + 15);
    m_buttonMenu->addChild(operatorToggler);
    m_operatorToggler = operatorToggler;

    for (int i = 0; i < 2; i++) {
        auto colorDisplay = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png"); // usually dont get settings like this but fuck it its way easier in this case
        colorDisplay->setPosition(ccp((m_size.width / 2) + (i == 0 ? -60 : 60), (m_size.height / 2) + 60));
        m_mainLayer->addChild(colorDisplay);
        (i == 0 ? m_colorDisplay1 : m_colorDisplay2) = colorDisplay;

        auto colorInput = TextInput::create(90.0f, "");
        colorInput->setCommonFilter(CommonFilter::Hex);
        colorInput->setCallback([this, colorDisplay, colorInput] (const std::string& input) {
            if (input.length() > 6) {
                colorInput->setString(input.substr(0, 6)); // no clue why but max char count aint working sooooo
                return;
            }
            if (validateHex(input)) {
                calculate(m_operatorToggler->m_toggled);
                updateColorDisplays();
            }
            else colorDisplay->setColor(ccWHITE);
        });
        colorInput->setScale(0.75f);
        colorInput->setPosition((m_size.width / 2) + (i == 0 ? -60 : 60), (m_size.height / 2) + 15);
        m_mainLayer->addChild(colorInput);
        (i == 0 ? m_colorInput1 : m_colorInput2) = colorInput; 

        auto pasteButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_pasteBtn_001.png"), this, menu_selector(HexCalculatorPopup::onPaste));
        if (Mod::get()->getSettingValue<bool>("paste-button-under-inputs")) pasteButton->setPosition(ccp((m_size.width / 2) + (i == 0 ? -60 : 60), (m_size.height / 2) - 15));
        else pasteButton->setPosition((m_size.width / 2) + (i == 0 ? -115 : 115), (m_size.height / 2) + 15);
        pasteButton->setScale(0.5f);
        pasteButton->m_baseScale = 0.5f;
        pasteButton->setID(std::to_string(i + 1));
        m_buttonMenu->addChild(pasteButton);
    }

    auto colorOutputDisplay = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png"); // usually dont get settings like this but fuck it its way easier in this case
    colorOutputDisplay->setPosition(ccp(m_size.width / 2, (m_size.height / 2) - 30));
    m_mainLayer->addChild(colorOutputDisplay);
    m_colorOutputDisplay = colorOutputDisplay;

    auto colorOutput = TextInput::create(90.0f, "");
    colorOutput->setCommonFilter(CommonFilter::Hex);
    colorOutput->setCallback([this, colorOutput] (const std::string& input) {
        if (input.length() > 6) {
            colorOutput->setString(input.substr(0, 6));
            return;
        }
        if (validateHex(input)) {
            reverseCalculate(m_operatorToggler->m_toggled);
            updateColorDisplays();
        }
        else m_colorOutputDisplay->setColor(ccWHITE);
    });
    colorOutput->setScale(0.75f);
    colorOutput->setPosition(m_size.width / 2, (m_size.height / 2) - 75);
    m_mainLayer->addChild(colorOutput);
    m_colorOutput = colorOutput;

    auto copyButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_copyBtn_001.png"), this, menu_selector(HexCalculatorPopup::onCopy));
    copyButton->setPosition((m_size.width / 2) + 55, (m_size.height / 2) - 75);
    copyButton->setScale(0.5f);
    copyButton->m_baseScale = 0.5f;
    m_buttonMenu->addChild(copyButton);

    auto clearButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), this, menu_selector(HexCalculatorPopup::onClear));
    clearButton->setPosition((m_size.width / 2) - 55, (m_size.height / 2) - 75);
    clearButton->setScale(0.5f);
    clearButton->m_baseScale = 0.5f;
    m_buttonMenu->addChild(clearButton);

    return true;
}

void  HexCalculatorPopup::updateColorDisplays() { // little hacky thing so i can be lazy
    auto hex1 = m_colorInput1->getString();
    auto hex2 = m_colorInput2->getString();
    auto hex3 = m_colorOutput->getString();
    if (validateHex(hex1)) m_colorDisplay1->setColor(*cc3bFromHexString(hex1).ok());
    if (validateHex(hex2)) m_colorDisplay2->setColor(*cc3bFromHexString(hex2).ok());
    if (validateHex(hex3)) m_colorOutputDisplay->setColor(*cc3bFromHexString(hex3).ok());
}

void HexCalculatorPopup::calculate(bool mode) {
    auto hex1 = m_colorInput1->getString();
    auto hex2 = m_colorInput2->getString();
    if (!validateHex(hex1) || !validateHex(hex2)) return;
    auto col1 = *cc3bFromHexString(hex1).ok();
    auto col2 = *cc3bFromHexString(hex2).ok();
    auto outputCol = mode
    ? ccc3((col1.r + col2.r) / 2, (col1.g + col2.g) / 2, (col1.b + col2.b) / 2)
    : ccc3(clampInt(col1.r - col2.r), clampInt(col1.g - col2.g), clampInt(col1.b - col2.b));
    m_colorOutput->setString(cc3bToHexString(outputCol));
    updateColorDisplays();
}

void HexCalculatorPopup::reverseCalculate(bool mode) {
    auto hex1 = m_colorOutput->getString();
    auto hex2 = m_colorInput1->getString();
    if (!validateHex(hex1) || !validateHex(hex2)) return;
    auto col1 = *cc3bFromHexString(hex1).ok();
    auto col2 = *cc3bFromHexString(hex2).ok();
    auto outputCol = mode 
    ? ccc3(clampInt((col1.r * 2) - col2.r), clampInt((col1.g * 2) - col2.g), clampInt((col1.b * 2) - col2.b))
    : ccc3(clampInt(col1.r - col2.r), clampInt(col1.g - col2.g), clampInt(col1.b - col2.b));
    m_colorInput2->setString(cc3bToHexString(outputCol));
    updateColorDisplays();
}



void HexCalculatorPopup::onOperatorToggle(CCObject* sender) {
    calculate(!m_operatorToggler->m_toggled);
    updateColorDisplays();
}

void HexCalculatorPopup::onPaste(CCObject* sender) {
    auto string = clipboard::read();
    if (string.empty()) return;
    if (string.front() == '#') string.erase(0, 1);
    if (static_cast<CCNode*>(sender)->getID() == "1") m_colorInput1->setString(string, true);
    else m_colorInput2->setString(string, true);
}

void HexCalculatorPopup::onCopy(CCObject* sender) {
    auto string = m_colorOutput->getString();
    if (!string.empty()) clipboard::write(string);
}

void HexCalculatorPopup::onClear(CCObject* sender) {
    for (auto input : {m_colorInput1, m_colorInput2, m_colorOutput}) input->setString("", true);
}

void HexCalculatorPopup::onClose(CCObject* sender) { // textinput crash fix
    m_colorInput1->getInputNode()->onClickTrackNode(false);
    m_colorInput2->getInputNode()->onClickTrackNode(false);
    m_colorOutput->getInputNode()->onClickTrackNode(false);
    Popup::onClose(sender);
}