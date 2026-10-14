#ifndef MEDIATHEQUE_LISTE_CPP
#define MEDIATHEQUE_LISTE_CPP

#include "Media.cpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

class Liste {
private:
	int id;
	std::string nom;
	std::string description;
	std::vector<Media*> medias;

public:
	Liste(int id, std::string nom, std::string description)
		: id(id), nom(std::move(nom)), description(std::move(description)) {}

	int getId() const { return id; }
	void setId(int valeur) { id = valeur; }
	std::string getNom() const { return nom; }
	void setNom(std::string valeur) { nom = std::move(valeur); }
	std::string getDescription() const { return description; }
	void setDescription(std::string valeur) { description = std::move(valeur); }
	const std::vector<Media*>& getMedias() const { return medias; }

	void ajouterMedia(Media* media) {
		if (media == nullptr || std::find(medias.begin(), medias.end(), media) != medias.end()) return;
		medias.push_back(media);
		if (std::find(media->listes.begin(), media->listes.end(), this) == media->listes.end()) {
			media->listes.push_back(this);
		}
	}

	void retirerMedia(Media* media) {
		if (media == nullptr) return;
		medias.erase(std::remove(medias.begin(), medias.end(), media), medias.end());
		media->listes.erase(std::remove(media->listes.begin(), media->listes.end(), this), media->listes.end());
	}

	std::string toString() const {
		return "Liste [id=" + std::to_string(id)
			+ ", nom=" + nom
			+ ", description=" + description
			+ ", nbMedias=" + std::to_string(medias.size())
			+ "]";
	}
};

#endif
