#include "Media.cpp"

class Livre : public Media {
private:
	std::string edition;
	std::string isbn;

public:
	Livre(int id, std::string titre, std::string annee, std::string note,
		  std::string commentaire, bool possede,
		  std::string edition, std::string isbn)
		: Media(id, titre, annee, note, commentaire, possede),
		  edition(edition), isbn(isbn) {}

	~Livre() override = default;

	std::string getEdition() const {
		return edition;
	}

	void setEdition(std::string edition) {
		this->edition = edition;
	}

	std::string getIsbn() const {
		return isbn;
	}

	void setIsbn(std::string isbn) {
		this->isbn = isbn;
	}

	std::string toString() override {
		return "Livre [id=" + std::to_string(getId())
			+ ", titre=" + getTitre()
			+ ", annee=" + getAnnee()
			+ ", note=" + getNote()
			+ ", commentaire=" + getCommentaire()
			+ ", possede=" + (getPossede() ? "oui" : "non")
			+ ", edition=" + edition
			+ ", isbn=" + isbn
			+ "]";
	}
};
