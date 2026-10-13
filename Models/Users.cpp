#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Mediatheque.cpp"

using namespace std;

class Users{

private : int id;
private : string login,mdp;

Mediatheque* mediatheque;

// Liste de tous les utilisateurs
inline static vector<Users*> users;


public : Users(int id, string login, string mdp){


        this->id = id;
        this->login = login;
        this->mdp = mdp;
        this->mediatheque = nullptr;


}

public : string getLogin(){
    return this->login;
}

public : void setLogin(string login){
    this->login = login;
}

public : string getMdp(){
    return this->mdp;
}

public : void setMdp(string mdp){
    this->mdp = mdp;
}

public : int getId(){
    return this->id;
}

public : void setId(int id){
    this->id = id;
}

public : Mediatheque* getMediatheque(){
    return this->mediatheque;
}

public : void setMediatheque(Mediatheque* mediatheque){
    if (this->mediatheque == mediatheque){
        return;
    }
    if (this->mediatheque != nullptr){
        this->mediatheque->setUser(nullptr);
    }
    if (mediatheque != nullptr && mediatheque->getUser() != nullptr){
        mediatheque->getUser()->mediatheque = nullptr;
    }
    this->mediatheque = mediatheque;
    if (mediatheque != nullptr){
        mediatheque->setUser(this);
    }
}

public : bool verifierMdp(string mdp){
    return this->mdp == mdp;
}

public : string afficher(){
    return toString();
}

public : string toString(){
    return "Users [id=" + to_string(this->id)
        + ", login=" + this->login
        + ", mediatheque=" + (this->mediatheque != nullptr ? this->mediatheque->getNom() : "aucune")
        + "]";
}

public : void creer(){
    users.push_back(this);
}

public : static Users* lire(int id){
    for (Users* u : users){
        if (u->getId() == id){
            return u;
        }
    }
    return nullptr;
}

public : static vector<Users*> lireTous(){
    return users;
}

public : void modifier(string login, string mdp){
    this->login = login;
    this->mdp = mdp;
}

public : void supprimer(){
    for (int i = 0; i < users.size(); i++){
        if (users[i] == this){
            users.erase(users.begin() + i);
            return;
        }
    }
}
};