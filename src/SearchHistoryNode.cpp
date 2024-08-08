#include "SearchHistoryNode.hpp"

SearchHistoryNode* SearchHistoryNode::create(SearchHistoryObject const& object, int index, SearchCallback search, RemoveCallback remove, bool h12, bool white) {
    auto ret = new SearchHistoryNode();
    if (ret->init(object, index, search, remove, h12, white)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SearchHistoryNode::init(SearchHistoryObject const& object, int index, SearchCallback search, RemoveCallback remove, bool h12, bool white) {
    if (!CCNode::init()) return false;

    setContentSize({ 400.0f, 100.0f });

    m_object = object;
    m_searchCallback = search;
    m_removeCallback = remove;
    m_index = index;

    auto background = CCScale9Sprite::create("GJ_square01.png", { 0, 0, 80, 80 });
    background->setContentSize({ 400.0f, 100.0f });
    background->setPosition(200.0f, 50.0f);
    addChild(background);

    auto queryLabel = CCLabelBMFont::create(object.query.empty() ? "(No Query)" : object.query.c_str(), "bigFont.fnt");
    queryLabel->setColor(object.query.empty() ? ccColor3B { 127, 127, 127 } : ccColor3B { 255, 255, 255 });
    queryLabel->setScale(0.75f);
    queryLabel->setAnchorPoint({ 0.0f, 0.5f });
    queryLabel->setPositionX(100.0f);
    queryLabel->limitLabelWidth(225.0f, 0.75f, 0.1f);
    addChild(queryLabel);

    auto type = object.type;
    switch (type) {
        case 0: {
            queryLabel->setPositionY(80.0f);
            auto levelSprite = CCSprite::createWithSpriteFrameName("GJ_viewLevelsBtn_001.png");
            levelSprite->setScale(1.75f);
            levelSprite->setPosition({ 50.0f, 50.0f });
            addChild(levelSprite);
            break;
        }
        case 1: {
            queryLabel->setPositionY(65.0f);
            auto listSprite = CCSprite::createWithSpriteFrameName("GJ_viewListsBtn_001.png");
            listSprite->setScale(1.75f);
            listSprite->setPosition({ 50.0f, 50.0f });
            addChild(listSprite);
            break;
        }
        case 2: {
            queryLabel->setPositionY(50.0f);
            auto userSprite = CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
            userSprite->setScale(1.45f);
            userSprite->setPosition({ 50.0f, 50.0f });
            addChild(userSprite);
            break;
        }
    }

    auto buttonMenu = CCMenu::create();
    buttonMenu->setContentSize({ 400.0f, 100.0f });
    buttonMenu->setPosition(0.0f, 0.0f);
    addChild(buttonMenu);

    auto removeButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_deleteBtn_001.png", 0.5f, [this](auto) {
        createQuickPopup("Remove Search", "Are you sure you want to remove this search history entry?", "No", "Yes", [this](auto, bool btn2) {
            if (btn2) m_removeCallback(m_index);
        });
    });
    removeButton->setPosition(queryLabel->getScaledContentSize().width + 120.0f, queryLabel->getPositionY());
    buttonMenu->addChild(removeButton);

    auto searchButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_undoBtn_001.png", 0.6f, [this](auto) {
        createQuickPopup("Search", "Are you sure you want to search with this query?", "No", "Yes", [this](auto, bool btn2) {
            if (btn2) m_searchCallback(m_object);
        });
    });
    searchButton->setPosition(queryLabel->getScaledContentSize().width + 150.0f, queryLabel->getPositionY());
    buttonMenu->addChild(searchButton);

    if (type < 2) {
        if (type < 1) {
            auto filtersNode = CCNode::create();
            filtersNode->setAnchorPoint({ 0.0f, 0.5f });
            filtersNode->setContentSize({ 300.0f, 15.0f });
            filtersNode->setPosition(100.0f, 55.0f);
            filtersNode->setLayout(RowLayout::create()->setGap(3.0f)->setAxisAlignment(AxisAlignment::Start)->setAutoScale(false));
            addChild(filtersNode);

            if (object.uncompleted) filtersNode->addChild(CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"));
            if (object.completed) filtersNode->addChild(CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"));
            if (object.featured) filtersNode->addChild(CCSprite::createWithSpriteFrameName("SH_featuredCoin_001.png"_spr));
            if (object.original) filtersNode->addChild(CCSprite::createWithSpriteFrameName("SH_originalIcon_001.png"_spr));
            if (object.twoPlayer) filtersNode->addChild(CCSprite::createWithSpriteFrameName("SH_twoPlayerBtn_001.png"_spr));
            if (object.coins) filtersNode->addChild(CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png"));
            if (object.epic) filtersNode->addChild(CCSprite::createWithSpriteFrameName("SH_epicCoin_001.png"_spr));
            if (object.mythic) filtersNode->addChild(CCSprite::createWithSpriteFrameName("SH_epicCoin2_001.png"_spr));
            if (object.legendary) filtersNode->addChild(CCSprite::createWithSpriteFrameName("SH_epicCoin3_001.png"_spr));
            if (object.noStar) filtersNode->addChild(CCSprite::createWithSpriteFrameName("GJ_starsIcon_gray_001.png"));

            for (auto child : CCArrayExt<CCNode*>(filtersNode->getChildren())) {
                child->setScale(0.7f);
            }

            if (filtersNode->getChildrenCount() == 0) {
                filtersNode->removeFromParent();
                auto noFiltersLabel = CCLabelBMFont::create("(No Filters)", "bigFont.fnt");
                noFiltersLabel->setAnchorPoint({ 0.0f, 0.5f });
                noFiltersLabel->setColor({ 127, 127, 127 });
                noFiltersLabel->setScale(0.5f);
                noFiltersLabel->setPosition(100.0f, 55.0f);
                addChild(noFiltersLabel);
            }
            else filtersNode->updateLayout();
        }

        auto difficultiesNode = CCNode::create();
        difficultiesNode->setAnchorPoint({ 0.0f, 0.5f });
        difficultiesNode->setContentSize({ 300.0f, 15.0f });
        difficultiesNode->setPosition(100.0f, 35.0f + (type >= 1 ? 5.0f : 0.0f));
        difficultiesNode->setLayout(RowLayout::create()->setGap(3.0f)->setAxisAlignment(AxisAlignment::Start)->setAutoScale(false));
        addChild(difficultiesNode);

        for (auto difficulty : object.difficulties) {
            auto difficultyFrame = std::string();
            switch (difficulty) {
                case -1: difficultyFrame = "diffIcon_00_btn_001.png"; break;
                case -2: difficultyFrame = "diffIcon_06_btn_001.png"; break;
                case -3: difficultyFrame = "diffIcon_auto_btn_001.png"; break;
                default: difficultyFrame = fmt::format("diffIcon_{:02d}_btn_001.png", difficulty);
            }

            auto difficultySprite = CCSprite::createWithSpriteFrameName(difficultyFrame.c_str());
            difficultySprite->setScale(0.5f);
            difficultiesNode->addChild(difficultySprite);
        }

        if (std::find(object.difficulties.begin(), object.difficulties.end(), -2) != object.difficulties.end() && object.demonFilter > 0) {
            auto demonFilterSprite = CCSprite::createWithSpriteFrameName(fmt::format("diffIcon_{:02d}_btn_001.png", object.demonFilter).c_str());
            demonFilterSprite->setScale(0.5f);
            difficultiesNode->addChild(demonFilterSprite);
        }

        if (type < 1) for (auto time : object.lengths) {
            auto length = std::string();
            switch (time) {
                case 0: length = "Tiny"; break;
                case 1: length = "Short"; break;
                case 2: length = "Medium"; break;
                case 3: length = "Long"; break;
                case 4: length = "XL"; break;
                case 5: length = "Plat."; break;
            }

            auto lengthLabel = CCLabelBMFont::create(length.c_str(), "bigFont.fnt");
            lengthLabel->setScale(0.3f);
            difficultiesNode->addChild(lengthLabel);
        }

        if (object.star) {
            auto starSprite = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
            starSprite->setScale(0.7f);
            difficultiesNode->addChild(starSprite);
        }

        if (difficultiesNode->getChildrenCount() == 0) {
            difficultiesNode->removeFromParent();
            auto noDifficultiesLabel = CCLabelBMFont::create("(No Difficulties/Lengths)", "bigFont.fnt");
            noDifficultiesLabel->setAnchorPoint({ 0.0f, 0.5f });
            noDifficultiesLabel->setColor({ 127, 127, 127 });
            noDifficultiesLabel->setScale(0.5f);
            noDifficultiesLabel->setPosition(100.0f, 35.0f + (type >= 1 ? 5.0f : 0.0f));
            addChild(noDifficultiesLabel);
        }
        else difficultiesNode->updateLayout();

        if (type < 1 && object.song) {
            auto songSprite = CCSprite::createWithSpriteFrameName("GJ_musicIcon_001.png");
            songSprite->setScale(0.65f);
            songSprite->setPosition({ 100.0f + (songSprite->getScaledContentSize().width / 2), 15.0f });
            addChild(songSprite);

            auto songLabel = CCLabelBMFont::create(
                (object.customSong ? std::to_string(object.songID) : std::string(LevelTools::getAudioTitle(object.songID - 1).c_str())).c_str(), "bigFont.fnt");
            songLabel->setScale(0.4f);
            songLabel->setAnchorPoint({ 0.0f, 0.5f });
            songLabel->setPosition(120.0f, 15.0f);
            songLabel->limitLabelWidth(280.0f, 0.4f, 0.1f);
            addChild(songLabel);
        } else if (type < 1) {
            auto noSongLabel = CCLabelBMFont::create("(No Song)", "bigFont.fnt");
            noSongLabel->setAnchorPoint({ 0.0f, 0.5f });
            noSongLabel->setColor({ 127, 127, 127 });
            noSongLabel->setScale(0.5f);
            noSongLabel->setPosition(100.0f, 15.0f);
            addChild(noSongLabel);
        }
    }

    std::stringstream ss;
    auto time = (time_t)object.time;
    ss << std::put_time(std::localtime(&time), h12 ? "%Y-%m-%d %I:%M:%S %p" : "%Y-%m-%d %H:%M:%S");

    auto timeLabel = CCLabelBMFont::create(ss.str().c_str(), "chatFont.fnt");
    timeLabel->setColor(white ? ccColor3B { 255, 255, 255 } : ccColor3B { 51, 51, 51 });
    timeLabel->setOpacity(white ? 200 : 152);
    timeLabel->setScale(0.5f);
    timeLabel->setAnchorPoint({ 1.0f, 0.0f });
    timeLabel->setPosition(394.0f, 6.0f);
    addChild(timeLabel);

    return true;
}
