#include "Media.cpp"

class Album : public Media {
private:
	std::string artiste;

public:
	Album(int id, std::string titre, std::string annee, std::string note,
		  std::string commentaire, bool possede, std::string artiste)
		: Media(id, titre, annee, note, commentaire, possede),
		  artiste(artiste) {}

	~Album() override = default;

	std::string getArtiste() const {
		return artiste;
	}

	void setArtiste(std::string artiste) {
		this->artiste = artiste;
	}

	std::string afficher() {
		return "Album [id=" + std::to_string(getId())
			+ ", titre=" + getTitre()
			+ ", annee=" + getAnnee()
			+ ", note=" + getNote()
			+ ", commentaire=" + getCommentaire()
			+ ", possede=" + (getPossede() ? "oui" : "non")
			+ ", artiste=" + artiste
			+ "]";
	}

	std::string toString() override {
		return afficher();
	}
};
