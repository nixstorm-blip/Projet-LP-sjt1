#pragma once
#include "Media.cpp"

using namespace std;

class Livre : public Media {
private:
	string edition;
	string isbn;

public:
	Livre(int id, string titre, string annee, int note,
		  string commentaire, bool possede,
		  string edition, string isbn)
		: Media(id, titre, annee, note, commentaire, possede),
		  edition(edition), isbn(isbn) {}

	~Livre() override = default;

	string getEdition() const {
		return edition;
	}

	void setEdition(string edition) {
		this->edition = edition;
	}

	string getIsbn() const {
		return isbn;
	}

	void setIsbn(string isbn) {
		this->isbn = isbn;
	}

	string afficher() override {
		return afficherAttributsCommuns("Livre")
			+ "\nÉdition      : " + edition
			+ "\nISBN         : " + isbn;
	}
};
