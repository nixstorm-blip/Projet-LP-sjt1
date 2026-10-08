#include <iostream>
#include <string>

using namespace std;
class Mediatheque;
class Media;

class ElementMediatheque{

private : int id;
private : string dateAjout;
Mediatheque* mediatheque;
Media* media;


public : ElementMediatheque(int id, string dateAjout){


        this->id = id;
        this->dateAjout = dateAjout;
        this->mediatheque = nullptr;
        this->media = nullptr;


}

public : int getId(){
    return this->id;
}

public : void setId(int id){
    this->id = id;
}

public : string getDateAjout(){
    return this->dateAjout;
}

public : void setDateAjout(string dateAjout){
    this->dateAjout = dateAjout;
}

public : Mediatheque* getMediatheque(){
    return this->mediatheque;
}

public : void setMediatheque(Mediatheque* mediatheque){
    this->mediatheque = mediatheque;
}

public : Media* getMedia(){
    return this->media;
}

public : void setMedia(Media* media){
    this->media = media;
}

public : string toString(){
    return "ElementMediatheque [id=" + to_string(this->id)
        + ", dateAjout=" + this->dateAjout
        + ", mediatheque=" + (this->mediatheque != nullptr ? "oui" : "aucune")
        + ", media=" + (this->media != nullptr ? "oui" : "aucun")
        + "]";
}







};
