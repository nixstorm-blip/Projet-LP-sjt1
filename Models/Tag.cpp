#pragma once
#include <iostream>
#include <string>

using namespace std;
class Media;

class Tag{

private : int id;
private : string nom,description;
Media* media;


public : Tag(int id, string nom, string description){

        this->id = id;
        this->nom = nom;
        this->description = description;
        this->media = nullptr;

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

public : string getDesc(){
    return this->description;
}

public : void setDesc(string description){
    this->description = description;
}

// Le lien est géré par Media::ajouterTag / Media::supprimerTag
public : Media* getMedia(){
    return this->media;
}

public : void setMedia(Media* media){
    this->media = media;
}

public : string toString(){
    return "Tag [id=" + to_string(this->id)
        + ", nom=" + this->nom
        + ", description=" + this->description
        + ", media=" + (this->media != nullptr ? "oui" : "aucun")
        + "]";
}

};
