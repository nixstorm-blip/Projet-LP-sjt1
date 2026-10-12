#pragma once
#include <iostream>
#include <string>
#include "Mediatheque.cpp"

using namespace std;

class Users{

private : int id;
private : string login,mdp;

Mediatheque* mediatheque;


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








};