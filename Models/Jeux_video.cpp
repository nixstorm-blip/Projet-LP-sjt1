#pragma once
#include "Media.cpp"

using namespace std;

class JeuVideo : public Media {
private:
	string editeur;
	string plateforme;

public:
	JeuVideo(int id, string titre, string annee, int note,
			 string commentaire, bool possede,
			 string editeur, string plateforme)
		: Media(id, titre, annee, note, commentaire, possede),
		  editeur(editeur), plateforme(plateforme) {}

	string getEditeur() const {
		return editeur;
	}

	void setEditeur(string editeur) {
		this->editeur = editeur;
	}

	string getPlateforme() const {
		return plateforme;
	}

	void setPlateforme(string plateforme) {
		this->plateforme = plateforme;
	}

	string afficher() override {
		return "JeuVideo [id=" + to_string(getId())
			+ ", titre=" + getTitre()
			+ ", annee=" + getAnnee()
			+ ", note=" + to_string(getNote())
			+ ", commentaire=" + getCommentaire()
			+ ", possede=" + (getPossede() ? "oui" : "non")
			+ ", editeur=" + editeur
			+ ", plateforme=" + plateforme
			+ "]";
	}
};
