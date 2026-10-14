#ifndef MEDIATHEQUE_JEUX_VIDEO_CPP
#define MEDIATHEQUE_JEUX_VIDEO_CPP

#include "Media.cpp"

#include <stdexcept>
#include <string>
#include <utility>

class Jeux_video : public Media {
private:
	std::string editeur;
	std::string plateforme;

public:
	Jeux_video(int id, std::string titre, std::string annee, std::string note,
			   std::string commentaire, bool possede,
			   std::string editeur, std::string plateforme)
		: Media(id, std::move(titre), std::move(annee), std::move(note),
				std::move(commentaire), possede),
		  editeur(std::move(editeur)), plateforme(std::move(plateforme)) {}

	std::string getEditeur() const { return editeur; }
	void setEditeur(std::string valeur) { editeur = std::move(valeur); }
	std::string getPlateforme() const { return plateforme; }
	void setPlateforme(std::string valeur) { plateforme = std::move(valeur); }

	std::string toString() override {
		return "Jeux_video [id=" + std::to_string(getId())
			+ ", titre=" + getTitre()
			+ ", annee=" + getAnnee()
			+ ", note=" + getNote()
			+ ", commentaire=" + getCommentaire()
			+ ", possede=" + (getPossede() ? "oui" : "non")
			+ ", editeur=" + editeur
			+ ", plateforme=" + plateforme
			+ "]";
	}
};

#endif
