#include <Geode/Geode.hpp>
#include <Geode/modify/GJAccountManager.hpp>
#include <Geode/modify/GManager.hpp>

using namespace geode::prelude;

static bool s_insideBackup = false;
static bool s_togglerChecked = true;

/**
* Functionality
*/
class $modify(GJAccountManager) {
    bool backupAccount(gd::string url) {
        s_insideBackup = true;
        auto res = GJAccountManager::backupAccount(url);
        s_insideBackup = false;

        return res;
    }
};

class $modify(GManager) {
    gd::string getCompressedSaveString() {
        if (!s_togglerChecked && s_insideBackup && typeinfo_cast<LocalLevelManager*>(this)) return "";
        return GManager::getCompressedSaveString();
    }
};

/**
* UI
*/
#include <Geode/modify/AccountLayer.hpp>

class $modify(AccountLayer) {
    /*void onBackup(CCObject* pSender) {
        AccountLayer::onBackup(pSender);
        FLAlertLayer* alert = nullptr;
        for(size_t i = 0; i < CCScene::get()->getChildrenCount(); i++) {
            alert = CCScene::get()->getChildByType<FLAlertLayer*>(i);
            if(alert) break;
        }

        if(!alert) return;

        auto saveWithoutBtn = CCMenuItemExt::createSpriteExtra(
            ButtonSprite::create("Save Without Levels"), 
            [this, alert](CCMenuItemSpriteExtra* item) {
                this->FLAlert_Clicked(alert, false);
            }
        );
        alert->m_buttonMenu->addChild(saveWithoutBtn);
        alert->m_buttonMenu->updateLayout();
    }*/

    void customSetup() {
        AccountLayer::customSetup();
        auto winSize = CCDirector::get()->getWinSize();

        auto includeLevelsCheck = CCMenuItemExt::createTogglerWithStandardSprites(
            0.8f, [](auto toggler) {
                s_togglerChecked = !toggler->isToggled();
            }
        );
        includeLevelsCheck->toggle(s_togglerChecked = true);
        includeLevelsCheck->setPosition(winSize.width / 2.f + 110.f, - winSize.height / 2.f + 53.f);
        m_buttonMenu->addChild(includeLevelsCheck);

        auto label = CCLabelBMFont::create("Include\nLevels", "bigFont.fnt");
        label->setScale(0.325f);
        label->setPosition(includeLevelsCheck->getPositionX(), includeLevelsCheck->getPositionY() + 27.f);
        label->setAlignment(cocos2d::kCCTextAlignmentCenter);
        m_buttonMenu->addChild(label);
    }
};