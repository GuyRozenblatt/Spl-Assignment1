#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
##!!
//
// Created by shaiami@wincs.cs.bgu.ac.il on 22/11/2019.
//
BaseAction::BaseAction() {}

void BaseAction::complete() {
    status=COMPLETED;
}

void BaseAction::pending() {
    status=PENDING;
}

void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

BaseAction::~BaseAction() {}

int Watch::getId() const {
    return id;
}

Watch::Watch(int id) : BaseAction(), id(id) {}

void CreateUser::act(Session &sess) {
    this->pending();
    std::unordered_map<std::string, User *>::const_iterator iter = sess.getUserMap().find(sess.getUserWord2()); //check if user created already exists.
    if (iter!=sess.getUserMap().end()){
        this->error("user already exists");
        std::cout << this->toString()<<std::endl;
    }
    else { // here we know that that the username does not exist.
        if (sess.getUserWord3() == "len") {
            LengthRecommenderUser *toInsert = new LengthRecommenderUser(sess.getUserWord2());
            sess.InsertToMap(sess.getUserWord2(), toInsert);
            complete();
        }
        else if (sess.getUserWord3() == "rer") {
            RerunRecommenderUser *toInsert = new RerunRecommenderUser(sess.getUserWord2());
            sess.InsertToMap(sess.getUserWord2(), toInsert);
            complete();
        }
        else if (sess.getUserWord3() == "gen") {
            GenreRecommenderUser *toInsert = new GenreRecommenderUser(sess.getUserWord2());
            sess.InsertToMap(sess.getUserWord2(), toInsert);
            complete();
        }
        else {
            error ("invalid recommendation algorithm");
            std::cout << this->toString()<<std::endl;
        }
    }
}

std::string CreateUser::toString() const {
    std::string toReturn = "CreateUser";
    if (getStatus() == ERROR)
        toReturn = toReturn + " " + "ERROR: " + getErrorMsg();
    else if (getStatus() == COMPLETED)
        toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

CreateUser *CreateUser::clone() {
    return new CreateUser(*this);
}

void ChangeActiveUser::act(Session &sess) {
    this->pending();
    std::unordered_map<std::string, User *>::const_iterator iter = sess.getUserMap().find(sess.getUserWord2());
    if (iter!=sess.getUserMap().end()) {
        sess.setActiveUser(iter->second);
        complete();
    }
    else {
        this->error("User does not exist");
        std::cout << this->toString() << std::endl;
    }
}

std::string ChangeActiveUser::toString() const {
    std::string toReturn = "ChangeActiveUser";
    if (getStatus() == ERROR)
        toReturn = toReturn + " " + "ERROR: " + getErrorMsg();
    else if (getStatus() == COMPLETED)
        toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

ChangeActiveUser *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

void DeleteUser::act(Session &sess) {
    this->pending();
    std::unordered_map<std::string, User *>::const_iterator iter = sess.getUserMap().find(sess.getUserWord2());
    if (iter!=sess.getUserMap().end()) {
        delete iter->second;
        sess.RemoveFromMap(iter->first);
        complete();
    }
    else {
        this->error("User does not exist");
        std::cout << this->toString() << std::endl;
    }
}

std::string DeleteUser::toString() const {
    std::string toReturn = "DeleteUser";
    if (getStatus() == ERROR)
        toReturn = toReturn + " " + "ERROR: " + getErrorMsg();
    else if (getStatus() == COMPLETED)
        toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

DeleteUser *DeleteUser::clone() {
    return new DeleteUser(*this);
}

void DuplicateUser::act(Session &sess) {
    this->pending();
    std::unordered_map<std::string, User *>::const_iterator iter1 = sess.getUserMap().find(sess.getUserWord2());
    std::unordered_map<std::string, User *>::const_iterator iter2 = sess.getUserMap().find(sess.getUserWord3());
    if (iter1==sess.getUserMap().end()) {
        this->error("Original user does not exist");
        std::cout << this->toString() << std::endl;
    }
    else if (iter2!=sess.getUserMap().end()) {
        this->error("new user name is already taken");
        std::cout << this->toString() << std::endl;
    }
    else { // here we know that original user is valid and the new user does not exist.
        User* toDup=iter1->second;
        std::string newName= sess.getUserWord3();
        toDup=toDup->clone(newName);
        complete();
        sess.InsertToMap(toDup->getName(), toDup);
    }
}

std::string DuplicateUser::toString() const {
    std::string toReturn = "DuplicateUser";
    if (getStatus() == ERROR)
        toReturn = toReturn + " " + "ERROR: " + getErrorMsg();
    else if (getStatus() == COMPLETED)
        toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

DuplicateUser *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}

void PrintContentList::act(Session &sess) {
    this->pending();
    if (sess.getContent().size()==0) {
        std::cout << "No content available" << std::endl;
        complete();
    }
    else {
        int NumberOfContent=1;
        for (size_t i=0; i<sess.getContent().size(); i++) {
            std::cout << std::to_string(NumberOfContent) + ". ";
            std::cout << sess.getContent().at(i)->toString() << std::endl;
            NumberOfContent++;
        }
    }
    complete();
}

std::string PrintContentList::toString() const {
    std::string toReturn = "PrintContentList";
    toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

PrintContentList *PrintContentList::clone() {
    return new PrintContentList(*this);
}

void PrintWatchHistory::act(Session &sess) {
    this->pending();
    if (sess.getActiveUser()->getHistory().size()==0) {
        std::cout << "Your history list is empty" << std::endl;
        complete();
    }
    else {
        int NumberOfContent=1;
        for (size_t i=0; i<sess.getActiveUser()->getHistory().size(); i++) {
            std::cout << std::to_string(NumberOfContent) + ". ";
            std::cout << sess.getActiveUser()->getHistory().at(i)->toString() << std::endl;
            NumberOfContent++;
        }
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    std::string toReturn = "PrintWatchHistory";
    toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

PrintWatchHistory *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

void Watch::act(Session &sess) {
    std::string AnsFromUser="n";
    this->pending();
    size_t InputId = this->getId();
    if ((InputId<0) | (InputId > sess.getContent().size())) {
        error("Invalid id");
        std::cout << this->toString()<<std::endl;
    }
    else { // here we know that the id of the content is valid.
        std::cout << "Wathcing " << (sess.getContent().at(InputId))->toString() <<std::endl;
        this->complete();
        sess.getActiveUser()->InsertToHistory(sess.getContent().at(InputId)); //insert the item watched to history list.
        sess.getActiveUser()->UpdateCommonTags(sess.getContent().at(InputId)->getTags()); //update CommonTags for genre recommendation
        Watchable* toRec = sess.getContent().at(InputId)->getNextWatchable(sess);
        if (toRec != nullptr) { // here we know that there is a recommendation available.
            std::cout << "We recommend watching " << toRec->toString() << ", continue watching? [y/n]"<< std::endl;
            getline(std::cin, AnsFromUser);
            if (AnsFromUser=="y") {
                BaseAction *RecommendedContent = new Watch(toRec->getId());
                sess.setUserWord2(std::to_string(toRec->getId())); // we assign UserWord2 with the recommended contnet id.
                this->complete();
                sess.addToActionLog(this);
                RecommendedContent->act(sess);
            }
            else sess.addToActionLog(this);
        }
        else {
            std::cout<< "We dont have a recommendation for you" <<std::endl;
            sess.addToActionLog(this);
        }
    }
}

std::string Watch::toString() const {
    std::string toReturn = "Watch";
    if (getStatus() == ERROR)
        toReturn = toReturn + " " + "ERROR: " + getErrorMsg();
    else if (getStatus() == COMPLETED)
        toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

Watch *Watch::clone() {
    return new Watch(*this);
}


void PrintActionsLog::act(Session &sess) {
    this->pending();
    if (sess.getActionsLog().size()==0) {
        std::cout << "This is your first action" << std::endl;
    }
    else {
        for (int i=sess.getActionsLog().size()-1; i>=0; i--)
            std::cout << sess.getActionsLog().at(i)->toString() << std::endl;
    }
    this->complete();
}

std::string PrintActionsLog::toString() const {
    std::string toReturn = "PrintActionsLog";
    toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

PrintActionsLog *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

void Exit::act(Session &sess) {
    this->complete();
}

std::string Exit::toString() const {
    std::string toReturn = "Exit";
    toReturn = toReturn + " " + "COMPLETED";
    return toReturn;
}

Exit *Exit::clone() {
    return new Exit(*this);
}


