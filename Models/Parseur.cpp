#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Media.cpp"
#include "Films.cpp"
#include "Serie.cpp"
#include "Livres.cpp"
#include "Album.cpp"
#include "Jeux_video.cpp"

using namespace std;

class Parseur {
public:
    static void sauvegarderMedias(vector<Media*> medias, string chemin) {

        ofstream fichier(chemin);
        if (!fichier.is_open()) {
            throw runtime_error("Impossible d'ouvrir le fichier " + chemin);
        }

        fichier << "{\n";
        fichier << "  \"_total\": " << medias.size() << ",\n";
        fichier << "  \"medias\": [\n";
        for (size_t i = 0; i < medias.size(); i++) {
            fichier << sauvegarderMedia(medias[i]);
            if (i < medias.size() - 1) {
                fichier << ",";
            }
            fichier << "\n";
        }
        fichier << "  ]\n}\n";
    }

    static string sauvegarderMedia(Media* media) {

        string json = "    {\n";
        json += "      \"type\": \"" + typeMedia(media) + "\"";
        json += ",\n      \"id\": " + to_string(media->getId());
        json += ",\n      \"titre\": \"" + media->getTitre() + "\"";
        json += ",\n      \"annee\": \"" + media->getAnnee() + "\"";
        json += ",\n      \"note\": " + to_string(media->getNote());
        json += ",\n      \"commentaire\": \"" + media->getCommentaire() + "\"";
        json += ",\n      \"possede\": " + string(media->getPossede() ? "true" : "false");

        if (Films* films = dynamic_cast<Films*>(media)) {
            json += ",\n      \"duree\": " + to_string(films->getDuree());
            json += ",\n      \"isan\": \"" + films->getIsan() + "\"";
        } else if (Serie* serie = dynamic_cast<Serie*>(media)) {
            json += ",\n      \"nombreSaisons\": " + to_string(serie->getNombreSaisons());
            json += ",\n      \"nombreEpisodes\": " + to_string(serie->getNombreEpisodes());
        } else if (Livre* livre = dynamic_cast<Livre*>(media)) {
            json += ",\n      \"edition\": \"" + livre->getEdition() + "\"";
            json += ",\n      \"isbn\": \"" + livre->getIsbn() + "\"";
        } else if (JeuVideo* jeu = dynamic_cast<JeuVideo*>(media)) {
            json += ",\n      \"editeur\": \"" + jeu->getEditeur() + "\"";
            json += ",\n      \"plateforme\": \"" + jeu->getPlateforme() + "\"";
        }

        json += ",\n      \"format\": " + (media->getFormat() != nullptr ? "\"" + media->getFormat()->getNomFormat() + "\"" : string("null"));
        json += ",\n      \"genre\": " + (media->getGenre() != nullptr ? "\"" + media->getGenre()->getLibelle() + "\"" : string("null"));

        json += ",\n      \"createurs\": [";
        vector<Personne*> createurs = media->getCreateurs();
        for (size_t i = 0; i < createurs.size(); i++) {
            if (i > 0) {
                json += ", ";
            }
            json += "{\"nom\": \"" + createurs[i]->getNom() + "\""
                + ", \"prenom\": \"" + createurs[i]->getPrenom() + "\"}";
        }
        json += "]";

        json += ",\n      \"tags\": [";
        vector<Tag*> tags = media->getTags();
        for (size_t i = 0; i < tags.size(); i++) {
            if (i > 0) {
                json += ", ";
            }
            json += "{\"id\": " + to_string(tags[i]->getId())
                + ", \"nom\": \"" + tags[i]->getNom() + "\""
                + ", \"description\": \"" + tags[i]->getDesc() + "\"}";
        }
        json += "]";

        json += "\n    }";
        return json;
    }

    static string typeMedia(Media* media) {
        if (dynamic_cast<Films*>(media) != nullptr) return "Films";
        if (dynamic_cast<Serie*>(media) != nullptr) return "Serie";
        if (dynamic_cast<Livre*>(media) != nullptr) return "Livre";
        if (dynamic_cast<JeuVideo*>(media) != nullptr) return "JeuVideo";
        if (dynamic_cast<Album*>(media) != nullptr) return "Album";
        return "Media";
    }

};
