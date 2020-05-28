#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

//
// Created by shaiami@wincs.cs.bgu.ac.il on 22/11/2019.
//

////////////////////////////////////////////////WATCHABLE///////////////////////////////////////////////////////////////

Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length),  tags(tags){}

const std::vector<std::string> Watchable::getTags() const {return tags;}

Watchable::~Watchable() {}

int Watchable::getLength() const {return length;}

const long Watchable::getId() const {return id;}


//////////////////////////////////////////////MOVIE/////////////////////////////////////////////////////////////////////


Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags) : Watchable(id, length, tags), name(name) {}

std::string Movie::toString() const {
    std::string toReturn = name + " " + std::to_string(getLength()) + " " + "[";
    for (size_t i=0; i<getTags().size();i++) {
        toReturn = toReturn + getTags().at(i);
        toReturn =toReturn + ", ";
    }
    toReturn=toReturn.substr(0, toReturn.size()-2);
    toReturn=toReturn+"]";
    return toReturn;
}

Watchable* Movie::getNextWatchable(Session& s) const {
    return s.getActiveUser()->getRecommendation(s);
}

Watchable* Movie::clone() {
    Movie* toDup = new Movie(this->getId(), this->getName(), this->getLength(), this->getTags());
    return toDup;
}

const std::string &Movie::getName() const {return name;}


//////////////////////////////////////////////EPISODE///////////////////////////////////////////////////////////////////

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode, const std::vector<std::string> &tags) : Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode) {
    nextEpisodeId=id+1;
}

std::string Episode::toString() const {
    std::string toReturn = seriesName + " ";
    if ((season<=9) & (episode<=9)) {
        toReturn = toReturn + "S0" +std::to_string(season) + "E0" + std::to_string(episode) + " " + std::to_string(getLength()) + " " + "[";
    }
    else if ((season<=9) & (episode > 9)) {
        toReturn = toReturn + "S0" +std::to_string(season) + "E" + std::to_string(episode) + " " + std::to_string(getLength()) + " " + "[";
    }
    else if ((season>9) & (episode <=9)) {
        toReturn = toReturn + "S" +std::to_string(season) + "E0" + std::to_string(episode) + " " + std::to_string(getLength()) + " " + "[";
    }
    else toReturn = toReturn + "S" +std::to_string(season) + "E" + std::to_string(episode) + " " + std::to_string(getLength()) + " " + "[";

    for (size_t i=0; i<getTags().size();i++) {
       toReturn = toReturn + getTags().at(i);
        toReturn =toReturn + ", ";
    }
    toReturn=toReturn.substr(0, (toReturn.length())-2);
    toReturn=toReturn+"]";
    return toReturn;
}

Watchable *Episode::getNextWatchable(Session& s) const {
    if (this->seriesName==s.getContent().at(nextEpisodeId)->getName()) {
        return s.getContent().at(nextEpisodeId);
    }
    return s.getActiveUser()->getRecommendation(s);
}

Watchable* Episode::clone() {
    Episode* toDup = new Episode(this->getId(), this->getName(),this->getLength(), this->getSeason(), this->getEpisode() ,this->getTags());
    return toDup;
}

const std::string &Episode::getName() const {
    return seriesName;
}

int Episode::getSeason() const {
    return season;
}

int Episode::getEpisode() const {
    return episode;
}

long Episode::getNextEpisodeId() const {
    return nextEpisodeId;
}

