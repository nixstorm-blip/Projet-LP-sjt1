#ifndef MEDIATHEQUE_SERIE_CPP
#define MEDIATHEQUE_SERIE_CPP

#include "Media.cpp"

#include <stdexcept>
#include <string>

class Serie : public Media {
private:
    int nombreSaisons;
    int nombreEpisodes;

public:
    Serie(int id, std::string titre, std::string annee, std::string note,
          std::string commentaire, bool possede, int nombreSaisons,
          int nombreEpisodes)
        : Media(id, std::move(titre), std::move(annee), std::move(note),
                std::move(commentaire), possede),
          nombreSaisons(nombreSaisons), nombreEpisodes(nombreEpisodes) {
        if (nombreSaisons < 0 || nombreEpisodes < 0) {
            throw std::invalid_argument("Les nombres de saisons et d'épisodes ne peuvent pas être négatifs.");
        }
    }

    int getNombreSaisons() const { return nombreSaisons; }
    void setNombreSaisons(int nombre) {
        if (nombre < 0) throw std::invalid_argument("Le nombre de saisons ne peut pas être négatif.");
        nombreSaisons = nombre;
    }

    int getNombreEpisodes() const { return nombreEpisodes; }
    void setNombreEpisodes(int nombre) {
        if (nombre < 0) throw std::invalid_argument("Le nombre d'épisodes ne peut pas être négatif.");
        nombreEpisodes = nombre;
    }
    // Alias temporaire pour préserver la compatibilité avec l'ancien nom.
    void setnombreEpisodes(int nombre) { setNombreEpisodes(nombre); }

    std::string toString() override {
        return "Serie [id=" + std::to_string(getId())
            + ", titre=" + getTitre()
            + ", annee=" + getAnnee()
            + ", note=" + getNote()
            + ", commentaire=" + getCommentaire()
            + ", possede=" + (getPossede() ? "oui" : "non")
            + ", nombreSaisons=" + std::to_string(nombreSaisons)
            + ", nombreEpisodes=" + std::to_string(nombreEpisodes)
            + "]";
    }
};

#endif

