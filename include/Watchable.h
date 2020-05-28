#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    const std::vector<std::string> getTags () const;
    int getLength() const;
    const long getId() const;
    virtual Watchable* clone()=0;
    virtual const std::string &getName() const=0;

private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    const std::string &getName() const;
    Watchable* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    Watchable* clone();
    const std::string &getName() const;
    int getSeason() const;
    int getEpisode() const;
    long getNextEpisodeId() const;
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
