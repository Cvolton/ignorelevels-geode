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
                if (!Mod::get()->getSavedValue("shown-toggler-warning", false)) {
                    createQuickPopup(
                        "Warning",
                        "<cr>Warning:</c> When you <cl>Save Without Levels</c>, the <co>Load</c> feature will "
                        "be <cp>unable</c> to recover any of your levels. Make sure you keep a backup "
                        "of your important levels elsewhere, for example in the form of unlisted levels.\n\n"
                        "Are you sure you want to continue?",
                        "Yes", "No",
                        [toggler = Ref(toggler)](auto alert, bool btn2) {
                            if (btn2) {
                                s_togglerChecked = true;
                                toggler->toggle(true);
                            } else {
                                Mod::get()->setSavedValue("shown-toggler-warning", true);
                            }
                        }
                    );
                    return;
                }
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