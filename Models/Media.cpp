#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class ElementMediatheque;

class Media{

private : int id;
private : string titre,annee,note,commentaire;
private : bool possede;

vector<ElementMediatheque*> elements;


public : Media(int id, string titre, string annee, string note, string commentaire, bool possede){

        this->id = id;
        this->titre = titre;
        this->annee = annee;
        this->note = note;
        this->commentaire = commentaire;
        this->possede = possede;

}

public : virtual ~Media(){}

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

public : virtual string toString(){
    return "Media [id=" + to_string(this->id)
        + ", titre=" + this->titre
        + ", annee=" + this->annee
        + ", note=" + this->note
        + ", commentaire=" + this->commentaire
        + ", possede=" + (this->possede ? "oui" : "non")
        + ", nbMediatheques=" + to_string(this->elements.size())
        + "]";
}

};
