#include "Media.cpp"

class Films : public Media {
private:
    int duree;
    std::string isan;

public:
    Films(int id, std::string titre, std::string annee, std::string note,
          std::string commentaire, bool possede, int duree, std::string isan)
        : Media(id, titre, annee, note, commentaire, possede),
          duree(duree), isan(isan) {}

    ~Films() override = default;

    int getDuree() const {
        return duree;
    }

    void setDuree(int duree) {
        this->duree = duree;
    }

    std::string getIsan() const {
        return isan;
    }

    void setIsan(std::string isan) {
        this->isan = isan;
    }

    std::string afficher() {
        return "Films [id=" + std::to_string(getId())
            + ", titre=" + getTitre()
            + ", annee=" + getAnnee()
            + ", note=" + getNote()
            + ", commentaire=" + getCommentaire()
            + ", possede=" + (getPossede() ? "oui" : "non")
            + ", duree=" + std::to_string(duree)
            + ", isan=" + isan
            + "]";
    }

    std::string toString() override {
        return afficher();
    }
};