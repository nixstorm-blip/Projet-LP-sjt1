#pragma once
#include <stdexcept>
#include "Media.cpp"

using namespace std;

class Serie : public Media {
private:
    int nombreSaisons;
    int nombreEpisodes;

public:
    Serie(int id, string titre, string annee, int note,
          string commentaire, bool possede, int nombreSaisons, int nombreEpisodes)
        : Media(id, titre, annee, note, commentaire, possede),
          nombreSaisons(nombreSaisons), nombreEpisodes(nombreEpisodes) {}

    int getNombreSaisons() const {
        return nombreSaisons;
    }

    void setNombreSaisons(int nombreSaisons) {
        if (nombreSaisons < 0) {
            throw invalid_argument("Le nombre de saisons ne peut pas etre negatif");
        }
        this->nombreSaisons = nombreSaisons;
    }

    int getNombreEpisodes() const {
        return nombreEpisodes;
    }

    void setNombreEpisodes(int nombreEpisodes) {
        if (nombreEpisodes < 0) {
            throw invalid_argument("Le nombre d'episodes ne peut pas etre negatif");
        }
        this->nombreEpisodes = nombreEpisodes;
    }

    string afficher() override {
        return "Serie [id=" + to_string(getId())
            + ", titre=" + getTitre()
            + ", annee=" + getAnnee()
            + ", note=" + to_string(getNote())
            + ", commentaire=" + getCommentaire()
            + ", possede=" + (getPossede() ? "oui" : "non")
            + ", nombreSaisons=" + to_string(nombreSaisons)
            + ", nombreEpisodes=" + to_string(nombreEpisodes)
            + "]";
    }
};
