#include "../include/User.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <cmath>
#include <algorithm>
#include <iostream>
//
// Created by shaiami@wincs.cs.bgu.ac.il on 22/11/2019.
//


////////////////////////////////////////////////USER////////////////////////////////////////////////////////////////////


User::User(const std::string &name) : history(), CommonTags(), name(name) {}

std::string User::getName() const {return name;}

std::vector<Watchable*> User::get_history() const {return history;}

const std::vector<Watchable *> &User::getHistory() const {
    return history;
}

void User::UpdateCommonTags(const std::vector<std::string>& tags) { // update a common tags vector for genre recommendation
    std::string currentTag;
    for (size_t i=0 ; i<tags.size() ; i++) {
        currentTag=tags.at(i);
        auto it = std::find_if(CommonTags.begin(), CommonTags.end(), [&currentTag](const std::pair<std::string, int>& element) {return element.first==currentTag;});
        if (it!=CommonTags.end()) {
            int indexOfCurrentTag = distance(CommonTags.begin(), it);
            CommonTags[indexOfCurrentTag].second++;
        }
        else CommonTags.push_back(make_pair(currentTag , 1));
    }
    sort(CommonTags.begin(), CommonTags.end(), [](const pair<string, int>&x, const pair<string, int>&y) {
        if (x.second != y.second)
            return x.second < y.second;
        return x.first > y.first;
    });
}

void User::clear() {
    history.clear();
}

void User::InsertToHistory(Watchable *ToInsert) {
    for (size_t i=0; i<history.size(); i++) {
        if (history.at(i)==ToInsert)
            return;
    }
    history.push_back((ToInsert));
}

void User::UpdatePointers(Session &sess) { // update a users history vector of pointers to the new content vector.
    for (size_t i=0; i<history.size(); i++) {
        for (size_t j=0; j<sess.getContent().size(); j++) {
            if (history.at(i)->getId()==sess.getContent().at(j)->getId()) {
                history.at(i)=sess.getContent().at(j);
            }
        }
    }
}

User::~User() {}


////////////////////////////////////////////////LENGTHRECOMMENDERUSER///////////////////////////////////////////////////


LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name)  {}

User* LengthRecommenderUser::clone(std::string newUserName) {
    LengthRecommenderUser* toDup = new LengthRecommenderUser(newUserName);
    for (size_t i=0; i<this->get_history().size(); i++) {
        Watchable *toInsert = this->get_history().at(i);
        toDup->history.push_back(toInsert);
    }
    return toDup;
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    int SumOfHistory=0;
    double AvgHistoryLength;
    for (size_t i=0 ; i<history.size(); i++) {
        SumOfHistory=SumOfHistory+history.at(i)->getLength();
    }
    AvgHistoryLength=SumOfHistory/history.size();
    double MinDis=INFINITY;
    int MinDisIndex=0;
    if(history.size()==s.getContent().size())
        return nullptr;
    else {
        for (size_t i = 0; i < s.getContent().size(); i++) {
            bool found = true;
            if (MinDis > fabs(AvgHistoryLength - s.getContent().at(i)->getLength())) {
                for (size_t j=0; (j<history.size()) & (found); j++) {
                    if (s.getContent().at(i)==history.at(j)) {
                        found=false;
                    }
                }
                if (found) {
                    MinDis = fabs(AvgHistoryLength - s.getContent().at(i)->getLength());
                    MinDisIndex = i;
                }
            }
        }
    }
    Watchable* toRecommend = s.getContent().at(MinDisIndex);
    return toRecommend;
}


////////////////////////////////////////////////GENRECOMMENDERUSER//////////////////////////////////////////////////////


GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {}

User* GenreRecommenderUser::clone(std::string newUserName) {
    GenreRecommenderUser* toDup = new GenreRecommenderUser(newUserName);
    for (size_t i=0; i<this->get_history().size(); i++) {
        Watchable *toInsert = this->get_history().at(i);
        toDup->history.push_back(toInsert);
    }
    return toDup;
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    bool found =false;
    for (size_t i=CommonTags.size()-1; i>=0 ; i--) {
        found=false;
        for (size_t j=0; j<s.getContent().size() ; j++) {
            found=false;
            for (size_t k=0; (k<s.getContent().at(j)->getTags().size()) & (!found); k++) {
                if (CommonTags.at(i).first==s.getContent().at(j)->getTags().at(k)) {
                    for (auto * elem : history) {
                        if (elem->getName()==s.getContent().at(j)->getName())
                            found=true;
                    }
                    if (!found)
                        return s.getContent().at(j);
                }
            }
        }
    }
    return nullptr;
}

///////////////////////////////////////////////////RERUNRECOMMENDERUSER/////////////////////////////////////////////////

User* RerunRecommenderUser::clone(std::string newUserName) {
    RerunRecommenderUser* toDup = new RerunRecommenderUser(newUserName);
    for (size_t i=0; i<this->get_history().size(); i++) {
        Watchable *toInsert = this->get_history().at(i);
        toDup->history.push_back(toInsert);
    }
    return toDup;
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    if(get_history().size()==0){
        return nullptr;
    }
    Watchable* toRecomend = history.at(Reco%history.size());
    Reco++;
    return toRecomend;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {Reco=0;}




