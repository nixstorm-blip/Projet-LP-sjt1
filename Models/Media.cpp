#ifndef MEDIATHEQUE_MEDIA_CPP
#define MEDIATHEQUE_MEDIA_CPP

#include <string>
#include <utility>
#include <vector>

class Mediatheque;
class Personne;
class Liste;
class Franchise;

class Media {
private:
    int id;
    std::string titre;
    std::string annee;
    std::string note;
    std::string commentaire;
    bool possede;

    // Pointeurs non propriétaires : les objets sont possédés par le programme.
    std::vector<Mediatheque*> mediatheques;
    std::vector<Personne*> personnes;
    std::vector<Liste*> listes;
    std::vector<Franchise*> franchises;

    friend class Mediatheque;
    friend class Personne;
    friend class Liste;
    friend class Franchise;

public:
    Media(int id, std::string titre, std::string annee, std::string note,
          std::string commentaire, bool possede)
        : id(id), titre(std::move(titre)), annee(std::move(annee)),
          note(std::move(note)), commentaire(std::move(commentaire)),
          possede(possede) {}

    virtual ~Media() = default;
    Media(const Media&) = delete;
    Media& operator=(const Media&) = delete;

    int getId() const { return id; }
    void setId(int valeur) { id = valeur; }
    std::string getTitre() const { return titre; }
    void setTitre(std::string valeur) { titre = std::move(valeur); }
    std::string getAnnee() const { return annee; }
    void setAnnee(std::string valeur) { annee = std::move(valeur); }
    std::string getNote() const { return note; }
    void setNote(std::string valeur) { note = std::move(valeur); }
    std::string getCommentaire() const { return commentaire; }
    void setCommentaire(std::string valeur) { commentaire = std::move(valeur); }
    bool getPossede() const { return possede; }
    void setPossede(bool valeur) { possede = valeur; }

    const std::vector<Mediatheque*>& getMediatheques() const { return mediatheques; }
    const std::vector<Personne*>& getPersonnes() const { return personnes; }
    const std::vector<Liste*>& getListes() const { return listes; }

    virtual std::string toString() {
        return "Media [id=" + std::to_string(id)
            + ", titre=" + titre
            + ", annee=" + annee
            + ", note=" + note
            + ", commentaire=" + commentaire
            + ", possede=" + (possede ? "oui" : "non")
            + ", nbMediatheques=" + std::to_string(mediatheques.size())
            + ", nbPersonnes=" + std::to_string(personnes.size())
            + ", nbListes=" + std::to_string(listes.size())
            + ", nbFranchises=" + std::to_string(franchises.size())
            + "]";
    }
};

#endif
    