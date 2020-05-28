#include "../include/Session.h"
#include <fstream>
#include<sstream>
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Action.h"
//
// Created by shaiami@wincs.cs.bgu.ac.il on 22/11/2019.
//

Session::Session(const std::string &configFilePath) : content(), actionsLog(), userMap(), activeUser() {
    using json = nlohmann::json;
    std::ifstream i(configFilePath);
    json j;
    i>>j;

    json movies=j["movies"];
    int MovieId=0;
    for (auto& input : movies.items()) {
        json CurrMovie=input.value();
        content.push_back(new Movie(MovieId, CurrMovie["name"], CurrMovie["length"],CurrMovie["tags"]));
        MovieId++;
    }

    json series = j["tv_series"];
    int Id=content.size();

    for (auto& CurrentSerie : series.items()) {
        int NumOfSeason = 1;
        json serie = CurrentSerie.value();
        std::vector<int> Seasons = serie["seasons"];
        for (int NumOfEpisodes : Seasons) {
            for (int NumberOfEpisode = 1; NumberOfEpisode <= NumOfEpisodes; NumberOfEpisode++) {
                Episode *NewEpisode = new Episode(Id, serie["name"], serie["episode_length"], NumOfSeason,
                                                  NumberOfEpisode, serie["tags"]);
                content.push_back(NewEpisode);
                Id++;
            }
            NumOfSeason++;
        }
    }
    LengthRecommenderUser* defaultUser = new LengthRecommenderUser ("default");
    userMap.insert({"default", defaultUser});
    activeUser=defaultUser;
}

Session::~Session() { // destructor.
for (size_t i=0; i<content.size();i++) {
    delete content.at(i);
    content.at(i)= nullptr;
}
content.clear();
for (size_t i=0; i<actionsLog.size();i++) {
    delete actionsLog.at(i);
    actionsLog.at(i)= nullptr;
}

actionsLog.clear();

for (auto& element : userMap)
    delete(element.second);

userMap.clear();
activeUser=nullptr;
}

const std::vector<BaseAction*> &Session::getActionsLog() const {return actionsLog;}

void Session::start() {
    std::cout<<"SPLFLIX is now on!"<<std::endl;
    int NumOfWord=0;
    std::string UserCommand;
    std::string CurrWord;
    std::getline(std::cin, UserCommand);
    std::stringstream UserCommandIterator (UserCommand);
    while (UserCommandIterator>>CurrWord&&NumOfWord<3) {
        if (NumOfWord == 0)
            UserWord1 = CurrWord;
        if (NumOfWord == 1)
            UserWord2 = CurrWord;
        if (NumOfWord == 2)
            UserWord3 = CurrWord;
        NumOfWord++;
    }
    while (UserWord1!="exit") {
        if (UserWord1 == "createuser") {
            CreateUser *CUaction = new CreateUser();
            CUaction->act(*this);
            actionsLog.push_back(CUaction);
        }
        else if (UserWord1 == "changeuser") {
            ChangeActiveUser *CGUaction = new ChangeActiveUser();
            CGUaction->act(*this);
            actionsLog.push_back(CGUaction);
        }
        else if (UserWord1 == "deleteuser") {
            DeleteUser *DUaction = new DeleteUser();
            DUaction->act(*this);
            actionsLog.push_back(DUaction);
        }
        else if (UserWord1 == "dupuser") {
            DuplicateUser *Daction = new DuplicateUser();
            Daction->act(*this);
            actionsLog.push_back(Daction);
        }
        else if (UserWord1 == "content") {
            PrintContentList *cont = new PrintContentList();
            cont->act(*this);
            actionsLog.push_back(cont);
        }
        else if (UserWord1 == "watchhist") {
            PrintWatchHistory *watch = new PrintWatchHistory();
            watch->act(*this);
            actionsLog.push_back(watch);
        }
        else if (UserWord1=="watch") {
            Watch* WatchCmd = new Watch (std::stoi(UserWord2)-1);
            WatchCmd->act(*this);
        }
        else if (UserWord1 == "log") {
            PrintActionsLog *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        }
        else  std::cout << "illegal command. Please insert a new one" << endl;
        int NumOfWord = 0;
        std::string UserCommand;
        std::string CurrWord;
        std::getline(std::cin, UserCommand);
        std::stringstream UserCommandIterator(UserCommand);
        while (UserCommandIterator >> CurrWord && NumOfWord < 3) {
            if (NumOfWord == 0)
                UserWord1 = CurrWord;
            if (NumOfWord == 1)
                UserWord2 = CurrWord;
            if (NumOfWord == 2)
                UserWord3 = CurrWord;
            NumOfWord++;
        }
    }
    Exit* ExitCmd = new Exit();
    ExitCmd->act(*this);
    actionsLog.push_back(ExitCmd);
}

const std::string &Session::getUserWord1() const {
    return UserWord1;
}

const std::string &Session::getUserWord2() const {
    return UserWord2;
}

const std::string &Session::getUserWord3() const {
    return UserWord3;
}

const std::unordered_map<std::string, User *> &Session::getUserMap() const {
    return userMap;
}

void Session::setActiveUser(User *activeUser) {
    this->activeUser = activeUser;
}

const std::vector<Watchable *> &Session::getContent() const {
    return content;
}

User* const Session::getActiveUser() {
    return activeUser;
}

void Session::setUserWord2(const std::string &userWord2) {
    UserWord2 = userWord2;
}

void Session::addToActionLog(BaseAction *toInsert) {
    actionsLog.push_back(toInsert);
}

void Session::InsertToMap(std::string nameToInsert,User* UserToInsert) {
    userMap.insert({nameToInsert, UserToInsert});
}

void Session::RemoveFromMap(std::string nameToRemove) {
    userMap.erase(nameToRemove);
}
//copy constructor
Session::Session(const Session &other) {
    for (size_t i = 0; i < other.content.size(); i++) { // copy the content
        Watchable *ToInsert = (other.content.at(i)->clone());
        this->content.push_back(ToInsert);
    }
    for (size_t i = 0; i < other.actionsLog.size(); i++) { //copy actionsLog
        BaseAction *ToInsert = other.actionsLog.at(i)->clone();
        this->actionsLog.push_back(ToInsert);
    }

    string ToInsertName;
    User* ToInsertUser;
    for(auto it=other.userMap.begin(); it!=other.userMap.end(); it++){ //copy the userMap
        ToInsertName=it->first;
        ToInsertUser=it->second->clone(ToInsertName);
        ToInsertUser->UpdatePointers(*this);
        userMap.insert({ToInsertName,ToInsertUser});
    }
    string OtherName=other.activeUser->getName();
    for(auto it=userMap.begin(); it!=userMap.end(); it++) { // set the active user.
        if (it->first == OtherName)
            activeUser = it->second;
    }
}
// move constructor
Session::Session(Session &&other) : content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), activeUser(other.activeUser) { // steal the pointers from another session and update his pointers to be null.
    for (size_t i = 0; i < other.content.size(); i++)
        other.content.at(i) = nullptr;
    for (size_t i = 0; i < other.actionsLog.size(); i++)
        other.actionsLog.at(i) = nullptr;
    for (auto it = other.userMap.begin(); it != userMap.end(); it++)
        it->second = nullptr;
}
//copy assignment operator
Session &Session::operator=(Session &other){
    if(this!=&other){
        clear();
        for (size_t i = 0; i < other.content.size(); i++) {
            Watchable *ToInsert = (other.content.at(i)->clone());
            this->content.push_back(ToInsert);
        }
        for (size_t i = 0; i < other.actionsLog.size(); i++) {
            BaseAction *ToInsert = (other.actionsLog.at(i)->clone());
            this->actionsLog.push_back(ToInsert);
        }
        string ToInsertName;
        User* ToInsertUser;

        for(auto it=other.userMap.begin(); it!=other.userMap.end(); it++){
            ToInsertName=it->first;
            ToInsertUser=it->second->clone(ToInsertName);
            ToInsertUser->UpdatePointers(*this);
            userMap.insert({ToInsertName,ToInsertUser});
        }
        string OtherName=other.activeUser->getName();
        for(auto it=userMap.begin(); it!=userMap.end(); it++) {
            if (it->first == OtherName)
                activeUser = it->second;
        }
    }
    return *this;
}
// move assignment
Session &Session::operator=(Session &&other) {
    if (this != &other) {
        // clean my vectors and map and then steal others pointers and clear his vectors and map.

        for (size_t i = 0; i < content.size(); i++) {
            delete content.at(i);
            content.at(i) = nullptr;
        }
        content.clear();
        content=other.content;
        other.content.clear();

        for (size_t i=0; i<actionsLog.size();i++) {
            delete actionsLog.at(i);
            actionsLog.at(i)= nullptr;
        }
        actionsLog.clear();
        actionsLog=other.actionsLog;
        other.actionsLog.clear();

        for (auto& element : userMap)
            delete(element.second);

        userMap.clear();
        userMap=other.userMap;
        other.userMap.clear();

        activeUser=other.activeUser;
        other.activeUser= nullptr;

    }
    return *this;
}

void Session::clear() {
    delete this;
}




