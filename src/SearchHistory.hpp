#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct SearchHistoryObject {
    int64_t time;
    int type;
    std::string query;
    std::vector<int> difficulties;
    std::vector<int> lengths;
    bool uncompleted;
    bool completed;
    bool featured;
    bool original;
    bool twoPlayer;
    bool coins;
    bool epic;
    bool legendary;
    bool mythic;
    bool song;
    bool customSong;
    int songID;
    int demonFilter;
    bool noStar;
    bool star;
};

class SearchHistory {
public:
    static void add(GJSearchObject*, time_t, int);
    static void clear();
    static std::vector<SearchHistoryObject> get();
    static void remove(int);
};

template<>
struct matjson::Serialize<std::vector<SearchHistoryObject>> {
    static std::vector<SearchHistoryObject> from_json(matjson::Value const& value) {
        std::vector<SearchHistoryObject> vec;

        for (auto const& elem : value.as_array()) {
            std::vector<int> difficulties;
            for (auto const& e : elem["difficulties"].as_array()) {
                difficulties.push_back(e.as_int());
            }

            std::vector<int> lengths;
            for (auto const& e : elem["lengths"].as_array()) {
                lengths.push_back(e.as_int());
            }

            vec.push_back({
                .time = (int64_t)elem["time"].as_double(),
                .type = elem["type"].as_int(),
                .query = elem["query"].as_string(),
                .difficulties = difficulties,
                .lengths = lengths,
                .uncompleted = elem["uncompleted"].as_bool(),
                .completed = elem["completed"].as_bool(),
                .featured = elem["featured"].as_bool(),
                .original = elem["original"].as_bool(),
                .twoPlayer = elem["two-player"].as_bool(),
                .coins = elem["coins"].as_bool(),
                .epic = elem["epic"].as_bool(),
                .legendary = elem["legendary"].as_bool(),
                .mythic = elem["mythic"].as_bool(),
                .song = elem["song"].as_bool(),
                .customSong = elem["custom-song"].as_bool(),
                .songID = elem["song-id"].as_int(),
                .demonFilter = elem["demon-filter"].as_int(),
                .noStar = elem["no-star"].as_bool(),
                .star = elem["star"].as_bool()
            });
        }

        return vec;
    }

    static matjson::Value to_json(std::vector<SearchHistoryObject> const& vec) {
        matjson::Array arr;

        for (auto const& obj : vec) {
            matjson::Array difficulties;
            for (int const& e : obj.difficulties) {
                difficulties.push_back(e);
            }

            matjson::Array lengths;
            for (int const& e : obj.lengths) {
                lengths.push_back(e);
            }

            arr.push_back(matjson::Object {
                { "time", obj.time },
                { "type", obj.type },
                { "query", obj.query },
                { "difficulties", difficulties },
                { "lengths", lengths },
                { "uncompleted", obj.uncompleted },
                { "completed", obj.completed },
                { "featured", obj.featured },
                { "original", obj.original },
                { "two-player", obj.twoPlayer },
                { "coins", obj.coins },
                { "epic", obj.epic },
                { "legendary", obj.legendary },
                { "mythic", obj.mythic },
                { "song", obj.song },
                { "custom-song", obj.customSong },
                { "song-id", obj.songID },
                { "demon-filter", obj.demonFilter },
                { "no-star", obj.noStar },
                { "star", obj.star }
            });
        }

        return arr;
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_array();
    }
};
