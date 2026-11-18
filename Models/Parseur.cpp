#pragma once

#include "Media.cpp"
#include "Films.cpp"
#include "Serie.cpp"
#include "Livres.cpp"
#include "Album.cpp"
#include "Jeux_video.cpp"
#include "Personne.cpp"
#include "Tag.cpp"
#include "Format.cpp"
#include "Genre.cpp"
#include "Liste.cpp"
#include "Franchise.cpp"
#include "Mediatheque.cpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Parseur central : chaque méthode convertit une classe métier vers/depuis
// ses champs JSON. Application gère le fichier complet et reconnecte les IDs.
class Parseur {
private:
    static std::string citer(const std::string& texte) {
        std::ostringstream json;
        json << '"';
        for (unsigned char c : texte) {
            switch (c) {
            case '"': json << "\\\""; break;
            case '\\': json << "\\\\"; break;
            case '\b': json << "\\b"; break;
            case '\f': json << "\\f"; break;
            case '\n': json << "\\n"; break;
            case '\r': json << "\\r"; break;
            case '\t': json << "\\t"; break;
            default:
                if (c < 0x20) {
                    const char* hex = "0123456789abcdef";
                    json << "\\u00" << hex[(c >> 4) & 0x0f] << hex[c & 0x0f];
                } else json << static_cast<char>(c);
            }
        }
        json << '"';
        return json.str();
    }

    static std::string mediasArray(const std::vector<Media*>& medias) {
        std::string json = "[";
        bool first = true;
        for (Media* media : medias) {
            if (!media) continue;
            if (!first) json += ',';
            first = false;
            json += sauvegarderMedia(media);
        }
        return json + "]";
    }

public:
    static std::string typeMedia(Media* media) {
        if (dynamic_cast<Films*>(media)) return "Film";
        if (dynamic_cast<Serie*>(media)) return "Serie";
        if (dynamic_cast<Livre*>(media)) return "Livre";
        if (dynamic_cast<JeuVideo*>(media)) return "JeuVideo";
        if (dynamic_cast<Album*>(media)) return "Album";
        return "Media";
    }

    static std::string sauvegarderMediaInterne(Media* media, const std::string& artiste = "") {
        if (!media) throw std::invalid_argument("Impossible de sauvegarder un média nul.");
        std::ostringstream json;
        json << "{\"type\":" << citer(typeMedia(media))
             << ",\"id\":" << media->getId()
             << ",\"titre\":" << citer(media->getTitre())
             << ",\"annee\":" << citer(media->getAnnee())
             << ",\"note\":" << media->getNote()
             << ",\"commentaire\":" << citer(media->getCommentaire())
             << ",\"possede\":" << (media->getPossede() ? "true" : "false");
        if (const auto* film = dynamic_cast<Films*>(media))
            json << ",\"duree\":" << film->getDuree() << ",\"isan\":" << citer(film->getIsan());
        else if (const auto* serie = dynamic_cast<Serie*>(media))
            json << ",\"saisons\":" << serie->getNombreSaisons() << ",\"episodes\":" << serie->getNombreEpisodes();
        else if (const auto* livre = dynamic_cast<Livre*>(media))
            json << ",\"edition\":" << citer(livre->getEdition()) << ",\"isbn\":" << citer(livre->getIsbn());
        else if (const auto* jeu = dynamic_cast<JeuVideo*>(media))
            json << ",\"editeur\":" << citer(jeu->getEditeur()) << ",\"plateforme\":" << citer(jeu->getPlateforme());
        else if (dynamic_cast<Album*>(media))
            json << ",\"artiste\":" << citer(artiste);

        json << ",\"formatId\":";
        if (media->getFormat()) json << media->getFormat()->getId(); else json << "null";
        json << ",\"genreId\":";
        if (media->getGenre()) json << media->getGenre()->getId(); else json << "null";
        json << ",\"createurs\":[";
        bool first = true;
        for (Personne* personne : media->getCreateurs()) {
            if (!personne) continue;
            if (!first) json << ',';
            first = false;
            json << "{\"id\":" << personne->getId()
                 << ",\"nom\":" << citer(personne->getNom())
                 << ",\"prenom\":" << citer(personne->getPrenom()) << '}';
        }
        json << "],\"tags\":[";
        first = true;
        for (Tag* tag : media->getTags()) {
            if (!tag) continue;
            if (!first) json << ',';
            first = false;
            json << "{\"id\":" << tag->getId()
                 << ",\"nom\":" << citer(tag->getNom())
                 << ",\"description\":" << citer(tag->getDesc()) << '}';
        }
        json << "]}";
        return json.str();
    }

    static std::string sauvegarderFilm(Films* media) { return sauvegarderMediaInterne(media); }
    static std::string sauvegarderSerie(Serie* media) { return sauvegarderMediaInterne(media); }
    static std::string sauvegarderLivre(Livre* media) { return sauvegarderMediaInterne(media); }
    static std::string sauvegarderJeuVideo(JeuVideo* media) { return sauvegarderMediaInterne(media); }
    static std::string sauvegarderAlbum(Album* media, const std::string& artiste = "") { return sauvegarderMediaInterne(media, artiste); }
    static std::string sauvegarderMedia(Media* media, const std::string& artiste = "") {
        if (auto* film = dynamic_cast<Films*>(media)) return sauvegarderFilm(film);
        if (auto* serie = dynamic_cast<Serie*>(media)) return sauvegarderSerie(serie);
        if (auto* livre = dynamic_cast<Livre*>(media)) return sauvegarderLivre(livre);
        if (auto* jeu = dynamic_cast<JeuVideo*>(media)) return sauvegarderJeuVideo(jeu);
        if (auto* album = dynamic_cast<Album*>(media)) return sauvegarderAlbum(album, artiste);
        return sauvegarderMediaInterne(media, artiste);
    }

    static std::string sauvegarderPersonne(Personne* personne, const std::string& nationalite = "") {
        if (!personne) throw std::invalid_argument("Personne nulle.");
        return "{\"id\":" + std::to_string(personne->getId())
            + ",\"nom\":" + citer(personne->getNom())
            + ",\"prenom\":" + citer(personne->getPrenom())
            + ",\"nationalite\":" + citer(nationalite) + "}";
    }
    static std::string sauvegarderTag(Tag* tag) {
        if (!tag) throw std::invalid_argument("Tag nul.");
        return "{\"id\":" + std::to_string(tag->getId())
            + ",\"nom\":" + citer(tag->getNom())
            + ",\"description\":" + citer(tag->getDesc()) + "}";
    }
    static std::string sauvegarderFormat(Format* format) {
        if (!format) throw std::invalid_argument("Format nul.");
        return "{\"id\":" + std::to_string(format->getId()) + ",\"nom\":" + citer(format->getNomFormat()) + "}";
    }
    static std::string sauvegarderGenre(Genre* genre) {
        if (!genre) throw std::invalid_argument("Genre nul.");
        return "{\"id\":" + std::to_string(genre->getId()) + ",\"nom\":" + citer(genre->getLibelle()) + "}";
    }
    static std::string sauvegarderListe(const Liste* liste) {
        if (!liste) throw std::invalid_argument("Liste nulle.");
        return "{\"id\":" + std::to_string(liste->getId()) + ",\"nom\":" + citer(liste->getNom()) + ",\"description\":" + citer(liste->getDescription()) + "}";
    }
    static std::string sauvegarderFranchise(const Franchise* franchise) {
        if (!franchise) throw std::invalid_argument("Franchise nulle.");
        return "{\"id\":" + std::to_string(franchise->getId()) + ",\"nom\":" + citer(franchise->getNom()) + ",\"description\":" + citer(franchise->getDescription()) + "}";
    }
    static std::string sauvegarderMediatheque(Mediatheque* bibliotheque) {
        if (!bibliotheque) throw std::invalid_argument("Médiathèque nulle.");
        return "{\"id\":" + std::to_string(bibliotheque->getId()) + ",\"nom\":" + citer(bibliotheque->getNom()) + ",\"description\":" + citer(bibliotheque->getDescription()) + "}";
    }

    // Factories de lecture : un constructeur spécialisé par classe concrète.
    static std::unique_ptr<Films> chargerFilm(int id, const std::string& titre, const std::string& annee, int note, const std::string& commentaire, bool possede, int duree, const std::string& isan) {
        return std::make_unique<Films>(id, titre, annee, note, commentaire, possede, duree, isan);
    }
    static std::unique_ptr<Serie> chargerSerie(int id, const std::string& titre, const std::string& annee, int note, const std::string& commentaire, bool possede, int saisons, int episodes) {
        return std::make_unique<Serie>(id, titre, annee, note, commentaire, possede, saisons, episodes);
    }
    static std::unique_ptr<Livre> chargerLivre(int id, const std::string& titre, const std::string& annee, int note, const std::string& commentaire, bool possede, const std::string& edition, const std::string& isbn) {
        return std::make_unique<Livre>(id, titre, annee, note, commentaire, possede, edition, isbn);
    }
    static std::unique_ptr<JeuVideo> chargerJeuVideo(int id, const std::string& titre, const std::string& annee, int note, const std::string& commentaire, bool possede, const std::string& editeur, const std::string& plateforme) {
        return std::make_unique<JeuVideo>(id, titre, annee, note, commentaire, possede, editeur, plateforme);
    }
    static std::unique_ptr<Album> chargerAlbum(int id, const std::string& titre, const std::string& annee, int note, const std::string& commentaire, bool possede) {
        return std::make_unique<Album>(id, titre, annee, note, commentaire, possede);
    }

    static std::unique_ptr<Media> chargerMedia(const std::string& type, int id,
        const std::string& titre, const std::string& annee, int note,
        const std::string& commentaire, bool possede, const std::string& edition = "",
        const std::string& isbn = "", int duree = 0, const std::string& isan = "",
        int saisons = 0, int episodes = 0, const std::string& editeur = "",
        const std::string& plateforme = "", const std::string& artiste = "") {
        (void)artiste; // L'artiste n'existe pas encore dans la classe Album partagée.
        if (type == "Livre") return chargerLivre(id, titre, annee, note, commentaire, possede, edition, isbn);
        if (type == "Film" || type == "Films") return chargerFilm(id, titre, annee, note, commentaire, possede, duree, isan);
        if (type == "Serie") return chargerSerie(id, titre, annee, note, commentaire, possede, saisons, episodes);
        if (type == "JeuVideo" || type == "Jeux_video") return chargerJeuVideo(id, titre, annee, note, commentaire, possede, editeur, plateforme);
        if (type == "Album") return chargerAlbum(id, titre, annee, note, commentaire, possede);
        throw std::runtime_error("Type de média non reconnu : " + type);
    }
    static std::unique_ptr<Personne> chargerPersonne(int id, const std::string& nom, const std::string& prenom) {
        return std::make_unique<Personne>(id, nom, prenom);
    }
    static std::unique_ptr<Format> chargerFormat(int id, const std::string& nom) { return std::make_unique<Format>(id, nom); }
    static std::unique_ptr<Genre> chargerGenre(int id, const std::string& nom) { return std::make_unique<Genre>(id, nom); }
    static std::unique_ptr<Liste> chargerListe(int id, const std::string& nom, const std::string& description) { return std::make_unique<Liste>(id, nom, description); }
    static std::unique_ptr<Franchise> chargerFranchise(int id, const std::string& nom, const std::string& description) { return std::make_unique<Franchise>(id, nom, description); }
    static std::unique_ptr<Mediatheque> chargerMediatheque(int id, const std::string& nom, const std::string& description) { return std::make_unique<Mediatheque>(id, nom, description); }
    static std::unique_ptr<Tag> chargerTag(int id, const std::string& nom, const std::string& description) { return std::make_unique<Tag>(id, nom, description); }

    // API historique utilisée par main.cpp; on la conserve pour la démo.
    static void sauvegarderMedias(std::vector<Media*> medias, std::string chemin) {
        std::ofstream fichier(chemin, std::ios::binary | std::ios::trunc);
        if (!fichier) throw std::runtime_error("Impossible d'ouvrir le fichier " + chemin);
        fichier << "{\n  \"_total\": " << medias.size() << ",\n  \"medias\": [";
        for (std::size_t i = 0; i < medias.size(); ++i) {
            fichier << (i == 0 ? "\n    " : ",\n    ") << sauvegarderMedia(medias[i]);
        }
        if (!medias.empty()) fichier << '\n';
        fichier << "  ]\n}\n";
        if (!fichier) throw std::runtime_error("Erreur lors de l'écriture du fichier " + chemin);
    }
};
