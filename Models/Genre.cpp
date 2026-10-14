#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class Media;

class Genre{

private : int id;
private : string libelle;

vector<Media*> medias;


public : Genre(int id, string libelle){

        this->id = id;
        this->libelle = libelle;

}

public : int getId(){
    return this->id;
}

public : void setId(int id){
    this->id = id;
}

public : string getLibelle(){
    return this->libelle;
}

public : void setLibelle(string libelle){
    this->libelle = libelle;
}

public : vector<Media*> getMedias(){
    return this->medias;
}

public : void addMedia(Media* media){
    if (find(this->medias.begin(), this->medias.end(), media) == this->medias.end()){
        this->medias.push_back(media);
    }
}

public : void removeMedia(Media* media){
    this->medias.erase(remove(this->medias.begin(), this->medias.end(), media), this->medias.end());
}

public : string toString(){
    return "Genre [id=" + to_string(this->id)
        + ", libelle=" + this->libelle
        + ", nbMedias=" + to_string(this->medias.size())
        + "]";
}

};
