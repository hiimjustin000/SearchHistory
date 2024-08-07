#include "SearchHistory.hpp"

void SearchHistory::add(GJSearchObject* search, time_t time, int type) {
    auto history = Mod::get()->getSavedValue<std::vector<SearchHistoryObject>>("search-history");

    auto difficultyStrings = search->m_difficulty != "-" ? string::split(search->m_difficulty, ",") : std::vector<std::string>();
    auto difficulties = std::vector<int>();
    for (auto const& str : difficultyStrings) {
        difficulties.push_back(std::stoi(str));
    }

    auto lengthStrings = search->m_length != "-" ? string::split(search->m_length, ",") : std::vector<std::string>();
    auto lengths = std::vector<int>();
    for (auto const& str : lengthStrings) {
        lengths.push_back(std::stoi(str));
    }

    history.insert(history.begin(), {
        .time = time,
        .type = type,
        .query = search->m_searchQuery,
        .difficulties = difficulties,
        .lengths = lengths,
        .uncompleted = search->m_uncompletedFilter,
        .completed = search->m_completedFilter,
        .featured = search->m_featuredFilter,
        .original = search->m_originalFilter,
        .twoPlayer = search->m_twoPlayerFilter,
        .coins = search->m_coinsFilter,
        .epic = search->m_epicFilter,
        .legendary = search->m_legendaryFilter,
        .mythic = search->m_mythicFilter,
        .song = search->m_songFilter,
        .customSong = search->m_customSongFilter,
        .songID = search->m_songID,
        .demonFilter = (int)search->m_demonFilter,
        .noStar = search->m_noStarFilter,
        .star = search->m_starFilter
    });

    Mod::get()->setSavedValue("search-history", history);
}

void SearchHistory::clear() {
    Mod::get()->setSavedValue("search-history", std::vector<SearchHistoryObject>());
}

std::vector<SearchHistoryObject> SearchHistory::get() {
    return Mod::get()->getSavedValue<std::vector<SearchHistoryObject>>("search-history");
}

void SearchHistory::remove(int index) {
    auto history = Mod::get()->getSavedValue<std::vector<SearchHistoryObject>>("search-history");
    history.erase(history.begin() + index);
    Mod::get()->setSavedValue("search-history", history);
}