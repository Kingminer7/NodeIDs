// //#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include "LiteUtils.hpp"

using namespace geode::prelude;
using namespace geode::node_ids;

template<class... Args>
static void reorderButtons(Args... args) {
    int ooa = 0;
    for (auto& arg : { args... }) {
        if (arg) {
            arg->setOrderOfArrival(ooa);
            ooa += 1;
        }
    }
}

$register_ids(CreatorLayer) {
    setIDSafe<CCSprite>(this, 0, "background");
    setIDSafe<CCSprite>(this, 1, "bottom-left-corner");
    setIDSafe<CCSprite>(this, 2, "top-left-corner");
    auto winSize = CCDirector::get()->getWinSize();

    if (auto menu = this->getChildByType<CCMenu>(0)) {
        menu->setID("creator-buttons-menu");

        // move vault button to its own menu
        if (auto lockBtn = setIDSafe(menu, -2, "vault-button")) {
            auto menu = detachAndCreateMenu(
                this,
                "top-right-menu",
                ColumnLayout::create()
                    ->setAxisReverse(true)
                    ->setAxisAlignment(AxisAlignment::End),
                lockBtn
            );
            menu->setPositionY(
                menu->getPositionY() - 150.f / 2 + 
                    lockBtn->getScaledContentSize().height / 2
            );
            menu->setContentSize({ 60.f, 150.f });
            menu->updateLayout();
        }

        // move treasure room button to its own menu
        if (auto roomBtn = setIDSafe(menu, -1, "treasure-room-button")) {
            auto menu = detachAndCreateMenu(
                this,
                "bottom-right-menu",
                ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start),
                roomBtn
            );
            menu->setPositionY(
                menu->getPositionY() + 125.f / 2 - 
                    roomBtn->getScaledContentSize().height / 2
            );
            menu->setContentSize({ 60.f, 125.f });
            menu->updateLayout();
        }

        auto name11 = geode::lite::isLite() ? "bonus-button" : "lists-button";

        // row order is inverted because of layout
        reorderButtons(
            setIDSafe(menu, 10, "featured-button"),
            setIDSafe(menu, 11, name11),
            setIDSafe(menu, 12, "paths-button"),
            setIDSafe(menu, 13, "map-packs-button"),
            setIDSafe(menu, 14, "search-button"),

            setIDSafe(menu, 5, "map-button"),
            setIDSafe(menu, 6, "daily-button"),
            setIDSafe(menu, 7, "weekly-button"),
            setIDSafe(menu, 8, "event-button"),
            setIDSafe(menu, 9, "gauntlets-button"),

            setIDSafe(menu, 0, "create-button"),
            setIDSafe(menu, 1, "saved-button"),
            setIDSafe(menu, 2, "scores-button"),
            setIDSafe(menu, 3, "quests-button"),
            setIDSafe(menu, 4, "versus-button")
        );
        /*if (winSize.width / winSize.height <= 5.1f / 3.f) {
            menu->setContentSize({ winSize.width - 80.f, 310.f });
        }
        else {
            menu->setContentSize({ winSize.width - 120.f, 310.f });
        }*/
        menu->setContentSize({ 450.f, 310.f });
        menu->setLayout(
            RowLayout::create()
                ->setGap(6.f)
                ->setCrossAxisReverse(true)
                ->setGrowCrossAxis(true)
                ->setCrossAxisOverflow(false)
        );
    }

    if (auto menu = this->getChildByType<CCMenu>(1)) {
        menu->setID("exit-menu");
        auto exitBtn = setIDSafe(menu, 0, "exit-button");
        menu->setPositionY(
            menu->getPositionY() - 125.f / 2 + 
                getSizeSafe(exitBtn).height / 2
        );
        menu->setContentSize({ 60.f, 125.f });
        menu->setLayout(
            ColumnLayout::create()
                ->setAxisAlignment(AxisAlignment::End)
        );
    }

    // add a menu to the bottom left corner that is empty but prolly a place mods 
    // want to add stuff to
    auto bottomLeftMenu = CCMenu::create();
    bottomLeftMenu->setPosition(3, 3);
    bottomLeftMenu->setID("bottom-left-menu");
    bottomLeftMenu->setAnchorPoint({0, 0});
    bottomLeftMenu->setZOrder(1);
    bottomLeftMenu->setContentSize({ 40.f, winSize.height/2 });
    bottomLeftMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChild(bottomLeftMenu);
}

struct CreatorLayerIDs : Modify<CreatorLayerIDs, CreatorLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("CreatorLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set CreatorLayer::init hook priority, node IDs may not work properly");
        }
    }
    bool init() {
        if (!CreatorLayer::init()) return false;
        NodeIDs::get()->provide(this);
        return true;
    }
};
