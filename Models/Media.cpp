#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Tag.cpp"
#include "Format.cpp"
#include "Genre.cpp"
#include "Personne.cpp"

using namespace std;
class Mediatheque;

class Media{

private : int id;
private : string titre,annee,commentaire;
private : int note;
private : bool possede;

vector<Mediatheque*> mediatheques;
vector<Tag*> tags;
Format* format = nullptr;
Genre* genre = nullptr;
vector<Personne*> createurs;


public : Media(int id, string titre, string annee, int note, string commentaire, bool possede){

        this->id = id;
        this->titre = titre;
        this->annee = annee;
        this->note = note;
        this->commentaire = commentaire;
        this->possede = possede;

}

public : Media(const Media&) = delete;
public : Media& operator=(const Media&) = delete;

public : virtual ~Media(){
    for (Tag* tag : this->tags){
        delete tag;
    }
    if (this->format != nullptr){
        this->format->removeMedia(this);
    }
    if (this->genre != nullptr){
        this->genre->removeMedia(this);
    }
    for (Personne* personne : this->createurs){
        personne->removeOeuvre(this);
    }
}

public : int getId(){
    return this->id;
}

public : void setId(int id){
    this->id = id;
}

public : string getTitre(){
    return this->titre;
}

public : void setTitre(string titre){
    this->titre = titre;
}

public : string getAnnee(){
    return this->annee;
}

public : void setAnnee(string annee){
    this->annee = annee;
}

public : int getNote(){
    return this->note;
}

public : void setNote(int note){
    this->note = note;
}

public : string getCommentaire(){
    return this->commentaire;
}

public : void setCommentaire(string commentaire){
    this->commentaire = commentaire;
}

public : bool getPossede(){
    return this->possede;
}

public : void setPossede(bool possede){
    this->possede = possede;
}

public : vector<Mediatheque*> getMediatheques(){
    return this->mediatheques;
}

public : void ajouterMediatheque(Mediatheque* mediatheque){
    if (find(this->mediatheques.begin(), this->mediatheques.end(), mediatheque) == this->mediatheques.end()){
        this->mediatheques.push_back(mediatheque);
    }
}

public : void retirerMediatheque(Mediatheque* mediatheque){
    this->mediatheques.erase(remove(this->mediatheques.begin(), this->mediatheques.end(), mediatheque), this->mediatheques.end());
}

public : vector<Tag*> getTags(){
    return this->tags;
}

public : void ajouterTag(Tag* tag){
    if (tag == nullptr || find(this->tags.begin(), this->tags.end(), tag) != this->tags.end()){
        return;
    }
    if (tag->getMedia() != nullptr && tag->getMedia() != this){
        tag->getMedia()->detacherTag(tag);
    }
    tag->setMedia(this);
    this->tags.push_back(tag);
}

public : void supprimerTag(string nom){
    for (Tag* tag : this->tags){
        if (tag->getNom() == nom){
            this->tags.erase(remove(this->tags.begin(), this->tags.end(), tag), this->tags.end());
            delete tag;
            return;
        }
    }
}

public : void detacherTag(Tag* tag){
    this->tags.erase(remove(this->tags.begin(), this->tags.end(), tag), this->tags.end());
    tag->setMedia(nullptr);
}

public : Format* getFormat(){
    return this->format;
}

public : void setFormat(Format* format){
    if (this->format == format){
        return;
    }
    if (this->format != nullptr){
        this->format->removeMedia(this);
    }
    this->format = format;
    if (format != nullptr){
        format->addMedia(this);
    }
}

public : Genre* getGenre(){
    return this->genre;
}

public : void setGenre(Genre* genre){
    if (this->genre == genre){
        return;
    }
    if (this->genre != nullptr){
        this->genre->removeMedia(this);
    }
    this->genre = genre;
    if (genre != nullptr){
        genre->addMedia(this);
    }
}

public : vector<Personne*> getCreateurs(){
    return this->createurs;
}

public : void ajouterCreateur(Personne* personne){
    if (find(this->createurs.begin(), this->createurs.end(), personne) == this->createurs.end()){
        this->createurs.push_back(personne);
        personne->addOeuvre(this);
    }
}

public : void retirerCreateur(Personne* personne){
    this->createurs.erase(remove(this->createurs.begin(), this->createurs.end(), personne), this->createurs.end());
    personne->removeOeuvre(this);
}

public : virtual string toString(){
    return "Media [id=" + to_string(this->id)
        + ", titre=" + this->titre
        + ", annee=" + this->annee
        + ", note=" + to_string(this->note)
        + ", commentaire=" + this->commentaire
        + ", possede=" + (this->possede ? "oui" : "non")
        + ", format=" + (this->format != nullptr ? this->format->getNomFormat() : "aucun")
        + ", genre=" + (this->genre != nullptr ? this->genre->getLibelle() : "aucun")
        + ", nbCreateurs=" + to_string(this->createurs.size())
        + ", nbTags=" + to_string(this->tags.size())
        + ", nbMediatheques=" + to_string(this->mediatheques.size())
        + "]";
}

};
