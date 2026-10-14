#ifndef MEDIATHEQUE_TAG_CPP
#define MEDIATHEQUE_TAG_CPP

#include <string>
#include <utility>

class Tag {
private:
	int id;
	std::string nom;
	std::string description;

public:
	Tag(int id, std::string nom, std::string description)
		: id(id), nom(std::move(nom)), description(std::move(description)) {}
	int getId() const { return id; }
	void setId(int valeur) { id = valeur; }
	std::string getNom() const { return nom; }
	void setNom(std::string valeur) { nom = std::move(valeur); }
	std::string getDescription() const { return description; }
	void setDescription(std::string valeur) { description = std::move(valeur); }
	std::string toString() const {
		return "Tag [id=" + std::to_string(id) + ", nom=" + nom
			+ ", description=" + description + "]";
	}
};

#endif
