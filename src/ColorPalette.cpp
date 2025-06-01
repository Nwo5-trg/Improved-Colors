#include <Geode/Geode.hpp>
#include "ColorPaletteColorSelectPopup.hpp"
#include "ColorPalette.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

bool ColorPalette::init(int type, const CCSize& size) {
    if (!CCLayer::init()) return false;
    m_size = size;
    m_type = type;
    std::array<CCArray*, 8> paletteSets = {
        m_paletteSet1, m_paletteSet2, m_paletteSet3, 
        m_paletteSet4, m_paletteSet5, m_paletteSet6, 
        m_paletteSet7, m_paletteSet8
    };

    this->setContentSize(size);
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    auto background = CCScale9Sprite::create("square02_001.png");
    background->setContentSize(size);
    background->setPosition(size / 2);
    background->setOpacity(100);
    this->addChild(background);

    auto buttonContainer = CCMenu::create();
    buttonContainer->setPosition(0, 0);
    buttonContainer->setTouchPriority(-999); // fuck me whyyyyyyy
    this->addChild(buttonContainer);
    m_buttonContainer = buttonContainer;

    std::array<CCPoint, 9> positions {
        ccp((m_size.width / 2) - 60, (m_size.height / 2) + 15), ccp((m_size.width / 2) - 30, (m_size.height / 2) + 15),
        ccp(m_size.width / 2, (m_size.height / 2) + 15), ccp((m_size.width / 2) + 30, (m_size.height / 2) + 15),
        ccp((m_size.width / 2) + 60, (m_size.height / 2) + 15), ccp((m_size.width / 2) - 60, (m_size.height / 2) - 15),
        ccp((m_size.width / 2) - 30, (m_size.height / 2) - 15), ccp(m_size.width / 2, (m_size.height / 2) - 15),
        ccp((m_size.width / 2) + 30, (m_size.height / 2) - 15)
    };

    for (int i = 0; i < 8; i++) {
        auto array = CCArray::create();
        paletteSets[i] = array;
        for (int j = 0; j < 9; j++) {
            auto button = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png"), this, menu_selector(ColorPalette::onColorButton));
            button->setScale(0.75f);
            button->m_baseScale = 0.75f;
            button->setTag(i + 1); // so i can just loop thru everything with this tag for grabbing or setting colors
            button->setPosition(positions[j]);
            auto paletteIDLabel = CCLabelBMFont::create(std::to_string((i * 9) + (j + 1)).c_str(), "chatFont.fnt");
            paletteIDLabel->setColor(ccGRAY);
            paletteIDLabel->setPosition(button->getChildByType<CCSprite>(0)->getContentSize() / 2);
            button->addChild(paletteIDLabel);
            auto editModeIndicator = CCLabelBMFont::create("", "bigFont.fnt");
            editModeIndicator->setPosition(button->getChildByType<CCSprite>(0)->getContentSize() / 2);
            button->addChild(editModeIndicator);
            array->addObject(button);
            buttonContainer->addChild(button);
        }
    }

    auto editToggle = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"), this, menu_selector(ColorPalette::onEditPaletteToggle));
    editToggle->setScale(0.5f);
    editToggle->m_baseScale = 0.5f;
    editToggle->setPosition((m_size.width / 2) + 60, (m_size.height / 2) - 15);
    buttonContainer->addChild(editToggle);

    auto nextPalette =  CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(ColorPalette::onNextPalette));
    nextPalette->setScale(0.75f);
    nextPalette->m_baseScale = 0.75f;
    nextPalette->setRotationY(180.0f);
    nextPalette->setPosition((size.width / 2) + 100, size.height / 2);
    buttonContainer->addChild(nextPalette);

    auto previousPalette =  CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), this, menu_selector(ColorPalette::onPreviousPalette));
    previousPalette->setScale(0.75f);
    previousPalette->m_baseScale = 0.75f;
    previousPalette->setPosition((size.width / 2) - 100, size.height / 2);
    buttonContainer->addChild(previousPalette);
    
    update(true);
    goToPalette(mod->getSavedValue<int>("current-palette"));

    return true;
}

ColorPalette* ColorPalette::create(int type, const CCSize& size) {
    auto ret = new ColorPalette();
    if (ret->init(type, size)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ColorPalette::update(bool updateSprites) { // trust it works (somehow)
    std::vector<ccColor3B> vector;
    if (updateSprites) {
        auto paletteColors = mod->getSavedValue<std::vector<ccColor3B>>("palette-colors");
        if (paletteColors.empty()) {
            for (int i = 0; i < 72; i++) vector.emplace_back(ccc3(255, 255, 255));
            mod->setSavedValue<std::vector<ccColor3B>>("palette-colors", vector);
            paletteColors = vector;
            return;
        }
        int i = 0;
        for (auto node : CCArrayExt<CCNode>(m_buttonContainer->getChildren())) {
            if (node->getTag() >= 1) {
                node->getChildByType<CCSprite>(0)->setColor(paletteColors[i]);
                i++;
            }
        }
        return;
    } else {
        for (auto node : CCArrayExt<CCNode>(m_buttonContainer->getChildren())) if (node->getTag() >= 1) vector.emplace_back(node->getChildByType<CCSprite>(0)->getColor());
        mod->setSavedValue<std::vector<ccColor3B>>("palette-colors", vector);
    }
}

void ColorPalette::goToPalette(int palette) {
    for (auto node : CCArrayExt<CCNode>(m_buttonContainer->getChildren())) {
        if (node->getTag() >= 1) node->setVisible(false);
        if (node->getTag() == palette) node->setVisible(true);
    }
}

void ColorPalette::onEditPaletteToggle(CCObject* sender) {
    m_editPaletteMode = !m_editPaletteMode;
    if (m_editPaletteMode) {
        for (auto node : CCArrayExt<CCNode>(m_buttonContainer->getChildren())) if (node->getTag() >= 1) node->getChildByType<CCLabelBMFont>(1)->setString(" .");
    } else for (auto node : CCArrayExt<CCNode>(m_buttonContainer->getChildren())) if (node->getTag() >= 1) node->getChildByType<CCLabelBMFont>(1)->setString("");
};

void ColorPalette::onColorButton(CCObject* sender) {
    if (m_editPaletteMode) ColorPaletteColorSelectPopup::create(static_cast<CCNode*>(sender)->getChildByType<CCSprite>(0), this)->show();
    else { // trust this wont implode
        if (m_type == 0) {
            static_cast<ColorSelectPopup*>(this->getParent())->m_hexInput->setString(cc3bToHexString(static_cast<CCNode*>(sender)->getChildByType<CCSprite>(0)->getColor()));
        } else {
            static_cast<SetupPulsePopup*>(this->getParent())->m_hexInput->setString(cc3bToHexString(static_cast<CCNode*>(sender)->getChildByType<CCSprite>(0)->getColor()));
        }
    }
}

void ColorPalette::onNextPalette(CCObject* sender) {
    auto currentPalette = mod->getSavedValue<int>("current-palette");
    if (currentPalette == 8) currentPalette = 1;
    else currentPalette++;
    mod->setSavedValue<int>("current-palette", currentPalette);
    goToPalette(currentPalette);
};

void ColorPalette::onPreviousPalette(CCObject* sender) {
    auto currentPalette = mod->getSavedValue<int>("current-palette");
    if (currentPalette == 1) currentPalette = 8;
    else currentPalette--;
    mod->setSavedValue<int>("current-palette", currentPalette);
    goToPalette(currentPalette);
};