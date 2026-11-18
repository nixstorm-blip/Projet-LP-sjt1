
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

class Media;

class Liste {
private:
    int id;
    std::string nom;
    std::string description;

    std::vector<Media*> medias;

public:

    Liste(int id, const std::string& nom, const std::string& description)
        : id(id), nom(nom), description(description) 

        {
             if (nom.empty()) {
        throw std::invalid_argument(
            "Le nom de la liste ne peut pas etre vide"
        );
    }
        }



    int getId() const {
        return id;
    }

    const std::string& getNom() const {
        return nom;
    }

    const std::string& getDescription() const {
        return description;
    }

    const std::vector<Media*>& getMedias() const {
        return medias;
    }

    void setNom(const std::string& nouveauNom) {
         if (nouveauNom.empty()) {
        throw std::invalid_argument(
            "Le nom de la liste ne peut pas etre vide"
        );
    }

    nom = nouveauNom;
}

    void setDescription(const std::string& nouvelleDescription) {
        description = nouvelleDescription;
    }

    bool contientMedia(Media* media) const {
        return std::find(medias.begin(), medias.end(), media)
            != medias.end();
    }


  bool ajouterMedia(Media* media) {
    if (media == nullptr || contientMedia(media)) {
        return false;
    }

    medias.push_back(media);
    return true;
} 
 

   bool retirerMedia(Media* media) {
    auto it = std::find(medias.begin(), medias.end(), media);

    if (it == medias.end()) {
        return false;
    }

    medias.erase(it);
    return true;
}

   std::size_t getNombreMedias() const {
    return medias.size();
}

    std::string toString() const {
        return "Liste [id=" + std::to_string(id)
            + ", nom=" + nom
            + ", description=" + description
            + ", nbMedias=" + std::to_string(medias.size())
            + "]";
    }

};
