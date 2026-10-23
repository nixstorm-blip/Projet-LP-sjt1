#pragma once
#include "Media.cpp"

using namespace std;

class Album : public Media {
public:
	Album(int id, string titre, string annee, int note,
		  string commentaire, bool possede)
		: Media(id, titre, annee, note, commentaire, possede) {}

	~Album() override = default;

	string afficher() override {
		return "Album [id=" + to_string(getId())
			+ ", titre=" + getTitre()
			+ ", annee=" + getAnnee()
			+ ", note=" + to_string(getNote())
			+ ", commentaire=" + getCommentaire()
			+ ", possede=" + (getPossede() ? "oui" : "non")
			+ "]";
	}
};
