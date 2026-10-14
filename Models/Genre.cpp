#ifndef MEDIATHEQUE_GENRE_CPP
#define MEDIATHEQUE_GENRE_CPP

#include <string>
#include <utility>

class Genre {
private:
	int id;
	std::string libelle;

public:
	Genre(int id, std::string libelle) : id(id), libelle(std::move(libelle)) {}
	int getId() const { return id; }
	void setId(int valeur) { id = valeur; }
	std::string getLibelle() const { return libelle; }
	void setLibelle(std::string valeur) { libelle = std::move(valeur); }
	std::string toString() const {
		return "Genre [id=" + std::to_string(id) + ", libelle=" + libelle + "]";
	}
};

#endif
