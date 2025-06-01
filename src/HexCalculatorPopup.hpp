#pragma once

class HexCalculatorPopup : public geode::Popup<> {
private:
    cocos2d::CCSprite* m_colorDisplay1;
    cocos2d::CCSprite* m_colorDisplay2;
    cocos2d::CCSprite* m_colorOutputDisplay;
    geode::TextInput* m_colorInput1;
    geode::TextInput* m_colorInput2;
    geode::TextInput* m_colorOutput;
    CCMenuItemToggler* m_operatorToggler;
    
    void calculate(bool mode);
    void reverseCalculate(bool mode);
    void updateColorDisplays();

    void onOperatorToggle(cocos2d::CCObject* sender);
    void onPaste(cocos2d::CCObject* sender);
    void onCopy(cocos2d::CCObject* sender);
    void onClear(cocos2d::CCObject* sender);

public:
    static HexCalculatorPopup* create();
    bool setup() override;
    
};
