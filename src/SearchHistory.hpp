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

    bool operator==(SearchHistoryObject const& other) const {
        return floor(time / 86400.0) == floor(other.time / 86400.0) && 
            type == other.type &&
            query == other.query &&
            difficulties == other.difficulties &&
            lengths == other.lengths &&
            uncompleted == other.uncompleted &&
            completed == other.completed &&
            featured == other.featured &&
            original == other.original &&
            twoPlayer == other.twoPlayer &&
            coins == other.coins &&
            epic == other.epic &&
            legendary == other.legendary &&
            mythic == other.mythic &&
            song == other.song &&
            (!song || (customSong == other.customSong && songID == other.songID)) &&
            demonFilter == other.demonFilter &&
            noStar == other.noStar &&
            star == other.star;
    }
};

class SearchHistory {
public:
    static void add(GJSearchObject*, time_t, int);
    static void clear();
    static std::vector<SearchHistoryObject> get();
    static void remove(int);
};

#define PROPERTY_OR_DEFAULT(obj, prop, isFunc, asFunc, def) (obj.contains(prop) && obj[prop].isFunc() ? obj[prop].asFunc() : def)

template<>
struct matjson::Serialize<std::vector<SearchHistoryObject>> {
    static std::vector<SearchHistoryObject> from_json(matjson::Value const& value) {
        std::vector<SearchHistoryObject> vec;

        for (auto const& elem : value.as_array()) {
            std::vector<int> difficulties;
            if (elem.contains("difficulties") && elem["difficulties"].is_array()) {
                for (auto const& e : elem["difficulties"].as_array()) {
                    difficulties.push_back(e.as_int());
                }
            }

            std::vector<int> lengths;
            if (elem.contains("lengths") && elem["lengths"].is_array()) {
                for (auto const& e : elem["lengths"].as_array()) {
                    lengths.push_back(e.as_int());
                }
            }

            SearchHistoryObject obj = {
                .time = (int64_t)PROPERTY_OR_DEFAULT(elem, "time", is_number, as_double, 0),
                .type = PROPERTY_OR_DEFAULT(elem, "type", is_number, as_int, 0),
                .query = PROPERTY_OR_DEFAULT(elem, "query", is_string, as_string, ""),
                .difficulties = difficulties,
                .lengths = lengths,
                .uncompleted = PROPERTY_OR_DEFAULT(elem, "uncompleted", is_bool, as_bool, false),
                .completed = PROPERTY_OR_DEFAULT(elem, "completed", is_bool, as_bool, false),
                .featured = PROPERTY_OR_DEFAULT(elem, "featured", is_bool, as_bool, false),
                .original = PROPERTY_OR_DEFAULT(elem, "original", is_bool, as_bool, false),
                .twoPlayer = PROPERTY_OR_DEFAULT(elem, "two-player", is_bool, as_bool, false),
                .coins = PROPERTY_OR_DEFAULT(elem, "coins", is_bool, as_bool, false),
                .epic = PROPERTY_OR_DEFAULT(elem, "epic", is_bool, as_bool, false),
                .legendary = PROPERTY_OR_DEFAULT(elem, "legendary", is_bool, as_bool, false),
                .mythic = PROPERTY_OR_DEFAULT(elem, "mythic", is_bool, as_bool, false),
                .song = PROPERTY_OR_DEFAULT(elem, "song", is_bool, as_bool, false),
                .customSong = PROPERTY_OR_DEFAULT(elem, "custom-song", is_bool, as_bool, false),
                .songID = PROPERTY_OR_DEFAULT(elem, "song-id", is_number, as_int, 0),
                .demonFilter = PROPERTY_OR_DEFAULT(elem, "demon-filter", is_number, as_int, 0),
                .noStar = PROPERTY_OR_DEFAULT(elem, "no-star", is_bool, as_bool, false),
                .star = PROPERTY_OR_DEFAULT(elem, "star", is_bool, as_bool, false)
            };

            if (!std::any_of(vec.begin(), vec.end(), [&obj](SearchHistoryObject const& o) {
                return obj == o;
            })) vec.push_back(obj);
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

            matjson::Object historyObject;
            historyObject["time"] = obj.time;
            historyObject["type"] = obj.type;
            if (!obj.query.empty()) historyObject["query"] = obj.query;
            if (!difficulties.empty()) historyObject["difficulties"] = difficulties;
            if (!lengths.empty()) historyObject["lengths"] = lengths;
            if (obj.uncompleted) historyObject["uncompleted"] = obj.uncompleted;
            if (obj.completed) historyObject["completed"] = obj.completed;
            if (obj.featured) historyObject["featured"] = obj.featured;
            if (obj.original) historyObject["original"] = obj.original;
            if (obj.twoPlayer) historyObject["two-player"] = obj.twoPlayer;
            if (obj.coins) historyObject["coins"] = obj.coins;
            if (obj.epic) historyObject["epic"] = obj.epic;
            if (obj.legendary) historyObject["legendary"] = obj.legendary;
            if (obj.mythic) historyObject["mythic"] = obj.mythic;
            if (obj.song) {
                historyObject["song"] = obj.song;
                if (obj.customSong) historyObject["custom-song"] = obj.customSong;
                historyObject["song-id"] = obj.songID;
            }
            if (obj.demonFilter != 0) historyObject["demon-filter"] = obj.demonFilter;
            if (obj.noStar) historyObject["no-star"] = obj.noStar;
            if (obj.star) historyObject["star"] = obj.star;

            arr.push_back(historyObject);
        }

        return arr;
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_array();
    }
};
