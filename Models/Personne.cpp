#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <vector>

class Media;

class Personne {
private:
    int id;
    std::string nom;
    std::string prenom;
    
    std::vector<Media*> medias;
 

public:
    Personne(int id, std::string nom, std::string prenom)
    : id(id), nom(nom), prenom(prenom)
{
    if(this->nom.empty()){
        throw std::invalid_argument(
            "le nom ne peut pas etre vide"
        );
    }
}
    int getId()const{
    return id;
  }

    const std::string& getNom() const {
    return nom;
}

    const std::string& getPrenom() const {
    return prenom;
}

const std::vector<Media*>& getMedias() const {
    return medias;
}

void setNom(const std::string& nouveauNom) {
    if (nouveauNom.empty()) {
        throw std::invalid_argument(
            "Le nom ne peut pas etre vide"
        );
    }
    nom = nouveauNom;
}

void setPrenom(const std::string& nouveauPrenom) {
    prenom = nouveauPrenom;
}

bool contientMedia(Media* media) const{
    return std::find(medias.begin(), medias.end(), media)
           != medias.end();
}

void ajouterMedia(Media* media){
    if (media != nullptr && !contientMedia(media)) {
        medias.push_back(media);
    }

}


void retirerMedia(Media* media) {
    medias.erase(
        std::remove(medias.begin(), medias.end(), media),
        medias.end()
    );
}

};


