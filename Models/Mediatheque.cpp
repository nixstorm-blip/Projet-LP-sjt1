#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Media.cpp"

using namespace std;
class Users;

class Mediatheque{

private : int id;
private : string nom,description;
Users* user;

vector<Media*> medias;




public: Mediatheque(int id, string nom, string description)
    {
        this->id = id;
        this->nom = nom;
        this->description = description;
        this->user = nullptr;


    }

public : Mediatheque(const Mediatheque&) = delete;
public : Mediatheque& operator=(const Mediatheque&) = delete;

// Détruit les media qui ne sont plus dans aucune autre médiathèque
public : ~Mediatheque(){
    for (Media* media : this->medias){
        media->retirerMediatheque(this);
        if (media->getMediatheques().empty()){
            delete media;
        }
    }
}

public : int getId(){
    return this->id;
}

public : void setId(int id){
    this->id = id;
}

public : string getNom(){
    return this->nom;
}

public : void setNom(string nom){
    this->nom = nom;
}

public : string getDescription(){
    return this->description;
}

public : void setDescription(string description){
    this->description = description;
}

public : Users* getUser(){
    return this->user;
}

public : void setUser(Users* user){
    this->user = user;
}

public : vector<Media*> getMedias(){
    return this->medias;
}

public : void ajouterMedia(Media* media){
    if (find(this->medias.begin(), this->medias.end(), media) == this->medias.end()){
        this->medias.push_back(media);
        media->ajouterMediatheque(this);
    }
}

public : void supprimerMedia(int id){
    for (Media* media : this->medias){
        if (media->getId() == id){
            this->medias.erase(remove(this->medias.begin(), this->medias.end(), media), this->medias.end());
            media->retirerMediatheque(this);
            if (media->getMediatheques().empty()){
                delete media;
            }
            return;
        }
    }
}

public : string afficher(){
    return "Mediatheque [id=" + to_string(this->id)
        + ", nom=" + this->nom
        + ", description=" + this->description
        + ", user=" + (this->user != nullptr ? "oui" : "aucun")
        + ", nbMedias=" + to_string(this->medias.size())
        + "]";
}

    









};