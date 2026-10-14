#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class Media;

class Personne{

private : int id;
private : string nom,prenom,nationalite;

vector<Media*> oeuvres;


public : Personne(int id, string nom, string prenom, string nationalite){

        this->id = id;
        this->nom = nom;
        this->prenom = prenom;
        this->nationalite = nationalite;

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

public : string getPrenom(){
    return this->prenom;
}

public : void setPrenom(string prenom){
    this->prenom = prenom;
}

public : string getNationalite(){
    return this->nationalite;
}

public : void setNationalite(string nationalite){
    this->nationalite = nationalite;
}

public : vector<Media*> getOeuvres(){
    return this->oeuvres;
}

public : void addOeuvre(Media* media){
    if (find(this->oeuvres.begin(), this->oeuvres.end(), media) == this->oeuvres.end()){
        this->oeuvres.push_back(media);
    }
}

public : void removeOeuvre(Media* media){
    this->oeuvres.erase(remove(this->oeuvres.begin(), this->oeuvres.end(), media), this->oeuvres.end());
}

public : string toString(){
    return "Personne [id=" + to_string(this->id)
        + ", nom=" + this->nom
        + ", prenom=" + this->prenom
        + ", nationalite=" + this->nationalite
        + ", nbOeuvres=" + to_string(this->oeuvres.size())
        + "]";
}

};
