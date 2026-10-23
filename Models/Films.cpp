#pragma once
#include "Media.cpp"

using namespace std;

class Films : public Media {
private:
    int duree;
    string isan;

public:
    Films(int id, string titre, string annee, int note,
          string commentaire, bool possede, int duree, string isan)
        : Media(id, titre, annee, note, commentaire, possede),
          duree(duree), isan(isan) {}

    ~Films() override = default;

    int getDuree() const {
        return duree;
    }

    void setDuree(int duree) {
        this->duree = duree;
    }

    string getIsan() const {
        return isan;
    }

    void setIsan(string isan) {
        this->isan = isan;
    }

    string afficher() override {
        return "Films [id=" + to_string(getId())
            + ", titre=" + getTitre()
            + ", annee=" + getAnnee()
            + ", note=" + to_string(getNote())
            + ", commentaire=" + getCommentaire()
            + ", possede=" + (getPossede() ? "oui" : "non")
            + ", duree=" + to_string(duree)
            + ", isan=" + isan
            + "]";
    }
};