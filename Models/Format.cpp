#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class Media;

class Format{

private : int id;
private : string nomFormat;

vector<Media*> medias;


public : Format(int id, string nomFormat){

        this->id = id;
        this->nomFormat = nomFormat;

}

public : int getId(){
    return this->id;
}

public : void setId(int id){
    this->id = id;
}

public : string getNomFormat(){
    return this->nomFormat;
}

public : void setNomFormat(string nomFormat){
    this->nomFormat = nomFormat;
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

public : string afficher(){
    return "Format [id=" + to_string(this->id)
        + ", nomFormat=" + this->nomFormat
        + ", nbMedias=" + to_string(this->medias.size())
        + "]";
}

};
