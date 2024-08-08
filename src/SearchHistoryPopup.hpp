#include "SearchHistoryNode.hpp"

typedef MiniFunction<void(SearchHistoryObject const&)> SearchHistoryCallback;

class SearchHistoryPopup : public geode::Popup<SearchHistoryCallback> {
protected:
    ScrollLayer* m_scrollLayer;
    SearchHistoryCallback m_searchCallback;
    TextInput* m_searchInput;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    CCLabelBMFont* m_countLabel;
    int m_page;

    bool setup(SearchHistoryCallback) override;
public:
    static SearchHistoryPopup* create(SearchHistoryCallback);

    void page(int);
};
