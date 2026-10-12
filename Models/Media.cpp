#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Tag.cpp"
#include "Format.cpp"

using namespace std;
class ElementMediatheque;

class Media{

private : int id;
private : string titre,annee,note,commentaire;
private : bool possede;

vector<ElementMediatheque*> elements;
vector<Tag*> tags;
Format* format = nullptr;


public : Media(int id, string titre, string annee, string note, string commentaire, bool possede){

        this->id = id;
        this->titre = titre;
        this->annee = annee;
        this->note = note;
        this->commentaire = commentaire;
        this->possede = possede;

}

// Copie interdite : deux copies détruiraient les mêmes tags
public : Media(const Media&) = delete;
public : Media& operator=(const Media&) = delete;

// Un tag n'appartient qu'à un seul media : le media les détruit avec lui
public : virtual ~Media(){
    for (Tag* tag : this->tags){
        delete tag;
    }
    if (this->format != nullptr){
        this->format->removeMedia(this);
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

public : string getNote(){
    return this->note;
}

public : void setNote(string note){
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

public : vector<ElementMediatheque*> getElements(){
    return this->elements;
}

public : void setElements(vector<ElementMediatheque*> elements){
    this->elements = elements;
}

public : void addElement(ElementMediatheque* element){
    if (find(this->elements.begin(), this->elements.end(), element) == this->elements.end()){
        this->elements.push_back(element);
    }
}

public : void removeElement(ElementMediatheque* element){
    this->elements.erase(remove(this->elements.begin(), this->elements.end(), element), this->elements.end());
}

public : vector<Tag*> getTags(){
    return this->tags;
}

// Le media prend possession du tag
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

// Retire le tag sans le détruire (utilisé quand il change de media)
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

public : virtual string toString(){
    return "Media [id=" + to_string(this->id)
        + ", titre=" + this->titre
        + ", annee=" + this->annee
        + ", note=" + this->note
        + ", commentaire=" + this->commentaire
        + ", possede=" + (this->possede ? "oui" : "non")
        + ", format=" + (this->format != nullptr ? this->format->getNomFormat() : "aucun")
        + ", nbTags=" + to_string(this->tags.size())
        + ", nbMediatheques=" + to_string(this->elements.size())
        + "]";
}

};
