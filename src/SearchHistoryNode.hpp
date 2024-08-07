#include "SearchHistory.hpp"

typedef MiniFunction<void(SearchHistoryObject const&)> SearchCallback;
typedef MiniFunction<void(int)> RemoveCallback;

class SearchHistoryNode : public CCNode {
protected:
    SearchHistoryObject m_object;
    SearchCallback m_searchCallback;
    RemoveCallback m_removeCallback;
    int m_index;

    bool init(SearchHistoryObject const&, int, SearchCallback, RemoveCallback, bool, bool);
public:
    static SearchHistoryNode* create(SearchHistoryObject const&, int, SearchCallback, RemoveCallback, bool, bool);
};
