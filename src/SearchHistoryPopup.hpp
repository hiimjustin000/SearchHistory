#include "SearchHistory.hpp"

typedef std::function<void(SearchHistoryObject const&)> SearchHistoryCallback;

class SearchHistoryPopup : public geode::Popup<SearchHistoryCallback const&> {
protected:
    geode::ScrollLayer* m_scrollLayer;
    SearchHistoryCallback m_searchCallback;
    geode::TextInput* m_searchInput;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    cocos2d::CCLabelBMFont* m_countLabel;
    int m_page;

    bool setup(SearchHistoryCallback const&) override;
public:
    static SearchHistoryPopup* create(SearchHistoryCallback const&);

    void page(int);
};
