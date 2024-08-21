#include "SearchHistoryPopup.hpp"

SearchHistoryPopup* SearchHistoryPopup::create(SearchHistoryCallback callback) {
    auto ret = new SearchHistoryPopup();
    if (ret->initAnchored(440.0f, 290.0f, callback, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SearchHistoryPopup::setup(SearchHistoryCallback callback) {
    setTitle("Search History", "bigFont.fnt", 0.53f);

    m_searchCallback = callback;

    auto background = CCScale9Sprite::create("square02_001.png", { 0, 0, 80, 80 });
    background->setContentSize({ 400.0f, 195.0f });
    background->setPosition(220.0f, 117.5f);
    background->setOpacity(127);
    m_mainLayer->addChild(background);

    m_scrollLayer = ScrollLayer::create({ 400.0f, 195.0f });
    m_scrollLayer->setPosition(20.0f, 20.0f);
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(195.0f)
            ->setGap(2.5f)
    );
    m_mainLayer->addChild(m_scrollLayer);

    m_prevButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", 1.0f, [this](auto) {
        page(m_page - 1);
    });
    m_prevButton->setPosition(-34.5f, 145.0f);
    m_buttonMenu->addChild(m_prevButton);

    auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextButtonSprite->setFlipX(true);
    m_nextButton = CCMenuItemExt::createSpriteExtra(nextButtonSprite, [this](auto) {
        page(m_page + 1);
    });
    m_nextButton->setPosition(474.5f, 145.0f);
    m_buttonMenu->addChild(m_nextButton);

    auto clearButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_deleteBtn_001.png", 0.6f, [this](auto) {
        createQuickPopup("Clear History", "Are you sure you want to clear your search history?", "No", "Yes", [this](auto, bool btn2) {
            if (btn2) {
                SearchHistory::clear();
                page(0);
            }
        });
    });
    clearButton->setPosition(420.0f, 270.0f);
    m_buttonMenu->addChild(clearButton);

    m_countLabel = CCLabelBMFont::create("", "goldFont.fnt");
    m_countLabel->setAnchorPoint({ 1.0f, 0.0f });
    m_countLabel->setScale(0.5f);
    m_countLabel->setPosition(435.0f, 7.0f);
    m_mainLayer->addChild(m_countLabel);

    m_searchInput = TextInput::create(400.0f, "Search History...");
    m_searchInput->setCommonFilter(CommonFilter::Any);
    m_searchInput->setTextAlign(TextInputAlign::Left);
    m_searchInput->setPosition(220.0f, 235.0f);
    m_searchInput->setCallback([this](auto) { page(0); });
    m_mainLayer->addChild(m_searchInput);

    page(0);

    return true;
}

void SearchHistoryPopup::page(int p) {
    m_scrollLayer->m_contentLayer->removeAllChildren();

    std::vector<SearchHistoryObject> history;
    auto query = string::toLower(m_searchInput->getString());
    for (auto const& object : SearchHistory::get()) {
        if (string::toLower(object.query).find(query) != std::string::npos) history.push_back(object);
    }

    auto count = history.size();
    m_prevButton->setVisible(p > 0);
    m_nextButton->setVisible(p < (count > 0 ? (count - 1) / 5 : 0));

    auto h12 = Mod::get()->getSettingValue<bool>("12-hour-time");
    auto white = Mod::get()->getSettingValue<bool>("white-time");
    for (int i = p * 5; i < (p + 1) * 5 && i < count; i++) {
        m_scrollLayer->m_contentLayer->addChild(SearchHistoryNode::create(history[i], i, [this](SearchHistoryObject const& object) {
            m_searchCallback(object);
            onClose(nullptr);
        }, [this](int index) {
            SearchHistory::remove(index);
            page(m_page);
        }, h12, white));
    }

    m_scrollLayer->m_contentLayer->updateLayout();
    m_scrollLayer->scrollToTop();

    m_countLabel->setString(fmt::format("{} to {} of {}", count > 0 ? p * 5 + 1 : 0, std::min((p + 1) * 5, (int)count), count).c_str());

    m_page = p;
}
