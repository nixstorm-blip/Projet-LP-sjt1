#include <iostream>
#include <string>

using namespace std;
class Mediatheque;

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
    this->mediatheque = mediatheque;
}

public : string toString(){
    return "Users [id=" + to_string(this->id)
        + ", login=" + this->login
        + ", mediatheque=" + (this->mediatheque != nullptr ? "oui" : "aucune")
        + "]";
}








};