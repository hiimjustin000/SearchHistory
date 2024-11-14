#include "SearchHistory.hpp"

typedef std::function<void(SearchHistoryObject const&)> SearchCallback;
typedef std::function<void(int)> RemoveCallback;

class SearchHistoryNode : public cocos2d::CCNode {
protected:
    SearchHistoryObject m_object;
    SearchCallback m_searchCallback;
    RemoveCallback m_removeCallback;
    int m_index;
    int m_count;
public:
    static SearchHistoryNode* create(SearchHistoryObject const&, int, int, SearchCallback const&, RemoveCallback const&, bool, bool, bool);

    bool init(SearchHistoryObject const&, int, int, SearchCallback const&, RemoveCallback const&, bool, bool, bool);
    void draw() override;
};
