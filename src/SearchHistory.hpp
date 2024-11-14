#pragma once

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

template<>
struct matjson::Serialize<std::vector<SearchHistoryObject>> {
    static geode::Result<std::vector<SearchHistoryObject>> fromJson(matjson::Value const& value) {
        if (!value.isArray()) return geode::Err("Expected array");

        std::vector<SearchHistoryObject> vec;

        for (auto const& elem : value.asArray().unwrap()) {
            std::vector<int> difficulties;
            if (elem.contains("difficulties") && elem["difficulties"].isArray()) {
                for (auto const& e : elem["difficulties"].asArray().unwrap()) {
                    difficulties.push_back(e.asInt().unwrapOr(0));
                }
            }

            std::vector<int> lengths;
            if (elem.contains("lengths") && elem["lengths"].isArray()) {
                for (auto const& e : elem["lengths"].asArray().unwrap()) {
                    lengths.push_back(e.asInt().unwrapOr(0));
                }
            }

            SearchHistoryObject obj = {
                .time = (int64_t)elem["time"].asInt().unwrapOr(0),
                .type = (int)elem["type"].asInt().unwrapOr(0),
                .query = elem["query"].asString().unwrapOr(""),
                .difficulties = difficulties,
                .lengths = lengths,
                .uncompleted = elem["uncompleted"].asBool().unwrapOr(false),
                .completed = elem["completed"].asBool().unwrapOr(false),
                .featured = elem["featured"].asBool().unwrapOr(false),
                .original = elem["original"].asBool().unwrapOr(false),
                .twoPlayer = elem["two-player"].asBool().unwrapOr(false),
                .coins = elem["coins"].asBool().unwrapOr(false),
                .epic = elem["epic"].asBool().unwrapOr(false),
                .legendary = elem["legendary"].asBool().unwrapOr(false),
                .mythic = elem["mythic"].asBool().unwrapOr(false),
                .song = elem["song"].asBool().unwrapOr(false),
                .customSong = elem["custom-song"].asBool().unwrapOr(false),
                .songID = (int)elem["song-id"].asInt().unwrapOr(0),
                .demonFilter = (int)elem["demon-filter"].asInt().unwrapOr(0),
                .noStar = elem["no-star"].asBool().unwrapOr(false),
                .star = elem["star"].asBool().unwrapOr(false)
            };

            if (!std::any_of(vec.begin(), vec.end(), [&obj](SearchHistoryObject const& o) {
                return obj == o;
            })) vec.push_back(obj);
        }

        return geode::Ok(vec);
    }

    static matjson::Value toJson(std::vector<SearchHistoryObject> const& vec) {
        std::vector<matjson::Value> arr;

        for (auto const& obj : vec) {
            std::vector<matjson::Value> difficulties;
            for (int const& e : obj.difficulties) {
                difficulties.push_back(e);
            }

            std::vector<matjson::Value> lengths;
            for (int const& e : obj.lengths) {
                lengths.push_back(e);
            }

            matjson::Value historyObject;
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
};
