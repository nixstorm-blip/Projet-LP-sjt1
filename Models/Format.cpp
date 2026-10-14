#ifndef MEDIATHEQUE_FORMAT_CPP
#define MEDIATHEQUE_FORMAT_CPP

#include <string>
#include <utility>

class Format {
private:
	int id;
	std::string nomFormat;

public:
	Format(int id, std::string nomFormat) : id(id), nomFormat(std::move(nomFormat)) {}
	int getId() const { return id; }
	void setId(int valeur) { id = valeur; }
	std::string getNomFormat() const { return nomFormat; }
	void setNomFormat(std::string valeur) { nomFormat = std::move(valeur); }
	std::string toString() const {
		return "Format [id=" + std::to_string(id) + ", nomFormat=" + nomFormat + "]";
	}
};

#endif
