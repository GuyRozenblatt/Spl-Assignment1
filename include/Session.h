#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    const std::vector<BaseAction *> &getActionsLog() const;
    const std::string &getUserWord1() const;
    const std::string &getUserWord2() const;
    const std::string &getUserWord3() const;
    void setUserWord2(const std::string &userWord2);
    const std::unordered_map<std::string, User *> &getUserMap() const;
    const std::vector<Watchable *> &getContent() const;
    User* const getActiveUser();
    void RemoveFromMap (std::string nameToRemove);
    void setActiveUser(User *activeUser);
    void addToActionLog (BaseAction* toInsert);
    void InsertToMap (std::string nameToInsert , User* UserToInsert);
    Session(const Session &other);
    Session(Session &&other);
    Session&operator=(Session &other);
    Session&operator=(Session &&other);
    void clear ();


private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string UserWord1;
    std::string UserWord2;
    std::string UserWord3;
};
#endif