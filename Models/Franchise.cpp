#ifndef MEDIATHEQUE_FRANCHISE_CPP
#define MEDIATHEQUE_FRANCHISE_CPP

#include "Media.cpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

class Franchise {
private:
	int id;
	std::string nom;
	std::vector<Media*> medias;

public:
	Franchise(int id, std::string nom) : id(id), nom(std::move(nom)) {}

	int getId() const { return id; }
	void setId(int valeur) { id = valeur; }
	std::string getNom() const { return nom; }
	void setNom(std::string valeur) { nom = std::move(valeur); }
	const std::vector<Media*>& getMedias() const { return medias; }

	void ajouterMedia(Media* media) {
		if (media == nullptr || std::find(medias.begin(), medias.end(), media) != medias.end()) return;
		medias.push_back(media);
		if (std::find(media->franchises.begin(), media->franchises.end(), this) == media->franchises.end()) {
			media->franchises.push_back(this);
		}
	}

	void retirerMedia(Media* media) {
		if (media == nullptr) return;
		medias.erase(std::remove(medias.begin(), medias.end(), media), medias.end());
		media->franchises.erase(std::remove(media->franchises.begin(), media->franchises.end(), this), media->franchises.end());
	}

	std::string toString() const {
		return "Franchise [id=" + std::to_string(id)
			+ ", nom=" + nom
			+ ", nbMedias=" + std::to_string(medias.size())
			+ "]";
	}
};

#endif
