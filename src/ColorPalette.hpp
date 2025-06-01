#pragma once

class ColorPalette : public cocos2d::CCLayer {
    private:
        int m_type;
        bool m_editPaletteMode = false;
        cocos2d::CCSize m_size;

        cocos2d::CCArray* m_paletteSet1;
        cocos2d::CCArray* m_paletteSet2;
        cocos2d::CCArray* m_paletteSet3;
        cocos2d::CCArray* m_paletteSet4;
        cocos2d::CCArray* m_paletteSet5;
        cocos2d::CCArray* m_paletteSet6;
        cocos2d::CCArray* m_paletteSet7;
        cocos2d::CCArray* m_paletteSet8;
        cocos2d::CCNode* m_buttonContainer;

        void onEditPaletteToggle(cocos2d::CCObject* sender);
        void onNextPalette(cocos2d::CCObject* sender);
        void onPreviousPalette(cocos2d::CCObject* sender);
        void onColorButton(cocos2d::CCObject* sender);
        
        void goToPalette(int palette);

    public:
        static ColorPalette* create(int type, const cocos2d::CCSize& size);
        bool init(int type, const cocos2d::CCSize& size);
        void update(bool updateSprites);

};