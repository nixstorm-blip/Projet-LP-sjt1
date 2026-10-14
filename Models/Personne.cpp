#ifndef MEDIATHEQUE_PERSONNE_CPP
#define MEDIATHEQUE_PERSONNE_CPP

#include "Media.cpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

class Personne {
private:
	int id;
	std::string nom;
	std::string prenom;
	std::string nationalite;
	std::vector<Media*> medias;

public:
	Personne(int id, std::string nom, std::string prenom, std::string nationalite)
		: id(id), nom(std::move(nom)), prenom(std::move(prenom)), nationalite(std::move(nationalite)) {}

	int getId() const { return id; }
	void setId(int valeur) { id = valeur; }
	std::string getNom() const { return nom; }
	void setNom(std::string valeur) { nom = std::move(valeur); }
	std::string getPrenom() const { return prenom; }
	void setPrenom(std::string valeur) { prenom = std::move(valeur); }
	std::string getNationalite() const { return nationalite; }
	void setNationalite(std::string valeur) { nationalite = std::move(valeur); }
	const std::vector<Media*>& getMedias() const { return medias; }

	void ajouterMedia(Media* media) {
		if (media == nullptr || std::find(medias.begin(), medias.end(), media) != medias.end()) return;
		medias.push_back(media);
		if (std::find(media->personnes.begin(), media->personnes.end(), this) == media->personnes.end()) {
			media->personnes.push_back(this);
		}
	}

	void retirerMedia(Media* media) {
		if (media == nullptr) return;
		medias.erase(std::remove(medias.begin(), medias.end(), media), medias.end());
		media->personnes.erase(std::remove(media->personnes.begin(), media->personnes.end(), this), media->personnes.end());
	}

	std::string toString() const {
		return "Personne [id=" + std::to_string(id)
			+ ", nom=" + nom
			+ ", prenom=" + prenom
			+ ", nationalite=" + nationalite
			+ ", nbMedias=" + std::to_string(medias.size())
			+ "]";
	}
};

#endif
