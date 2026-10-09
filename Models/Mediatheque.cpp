#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class Users;
class ElementMediatheque;

class Mediatheque{

private : int id;
private : string nom,description;
Users* user;

vector<ElementMediatheque*> elements;




public: Mediatheque(int id, string nom, string description)
    {
        this->id = id;
        this->nom = nom;
        this->description = description;
        this->user = nullptr;


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

public : string toString(){
    return "Mediatheque [id=" + to_string(this->id)
        + ", nom=" + this->nom
        + ", description=" + this->description
        + ", user=" + (this->user != nullptr ? "oui" : "aucun")
        + ", nbMedias=" + to_string(this->elements.size())
        + "]";
}

    









};