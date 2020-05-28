#ifndef USER_H_
#define USER_H_
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

using namespace std; //TODO: IMPLEMENT IN ALL CLASSES!

class User{
public:
    User(const string& name);
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    string getName() const;
    vector<Watchable*> get_history() const;
    const vector<Watchable *> &getHistory() const;
    virtual User* clone(string)=0;
    void clear();
    void InsertToHistory(Watchable* ToInsert);
    void UpdateCommonTags (const vector<string>& tags);
    void UpdatePointers(Session& sess);

protected:
    vector<Watchable*> history;
    vector<pair<string, int>> CommonTags;
private:
    string name;
};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const string& name);
    virtual Watchable* getRecommendation(Session& s);
    User* clone(string newUserName);
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const string& name);
    virtual Watchable* getRecommendation(Session& s);
    User* clone(string newUserName);
private:
    int Reco;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const string& name);
    virtual Watchable* getRecommendation(Session& s);
    User* clone(string newUserName);
private:
};

#endif