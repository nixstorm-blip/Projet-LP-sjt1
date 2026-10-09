// ============================================
// PROJET MEDIATHEQUE - Fichier unique
// ============================================

#include <iostream>
#include <string>
#include <sqlite3.h>

// ============================================
// CLASSE MEDIA (classe mère)
// ============================================
class Media {
protected:
    int id;
    std::string titre;
    int annee;
    float note;
    std::string commentaire;
    bool posseder;

public:
    // Constructeur par défaut
    Media() : id(0), titre(""), annee(0), note(0), commentaire(""), posseder(false) {}

    // Constructeur avec paramètres
    Media(int id, const std::string& titre, int annee)
        : id(id), titre(titre), annee(annee), note(0), commentaire(""), posseder(false) {}

    // Destructeur virtuel (car on a de l'héritage)
    virtual ~Media() {}

    // Getters
    int getId() const { return id; }
    std::string getTitre() const { return titre; }
    int getAnnee() const { return annee; }
    float getNote() const { return note; }
    std::string getCommentaire() const { return commentaire; }
    bool getPosseder() const { return posseder; }

    // Setters
    void setId(int i) { id = i; }
    void setTitre(const std::string& t) { titre = t; }
    void setAnnee(int a) { annee = a; }
    void setNote(float n) { note = n; }
    void setCommentaire(const std::string& c) { commentaire = c; }
    void setPosseder(bool p) { posseder = p; }

    // Méthode d'affichage (virtuelle pour être redéfinie)
    virtual void afficher() const {
        std::cout << "ID : " << id << std::endl;
        std::cout << "Titre : " << titre << std::endl;
        std::cout << "Annee : " << annee << std::endl;
        std::cout << "Note : " << note << "/5" << std::endl;
        std::cout << "Commentaire : " << commentaire << std::endl;
        std::cout << "Possede : " << (posseder ? "Oui" : "Non") << std::endl;
    }
};

// ============================================
// CLASSE FILMS (hérite de Media)
// ============================================
class Films : public Media {
private:
    int duree;
    std::string isan;

public:
    // Constructeur par défaut
    Films() : Media(), duree(0), isan("") {}

    // Constructeur avec paramètres
    Films(int id, const std::string& titre, int annee, int duree, const std::string& isan)
        : Media(id, titre, annee), duree(duree), isan(isan) {}

    // Getters
    int getDuree() const { return duree; }
    std::string getIsan() const { return isan; }

    // Setters
    void setDuree(int d) { duree = d; }
    void setIsan(const std::string& i) { isan = i; }

    // Redéfinition de afficher()
    void afficher() const override {
        std::cout << "=== FILM ===" << std::endl;
        Media::afficher();  // Appel de l'affichage de Media
        std::cout << "Duree : " << duree << " minutes" << std::endl;
        std::cout << "ISAN : " << isan << std::endl;
    }
};

// ============================================
// CLASSE DATABASE (connexion SQLite)
// ============================================
class Database {
private:
    sqlite3 *db;

public:
    Database() : db(nullptr) {}

    ~Database() {
        if (db != nullptr) {
            sqlite3_close(db);
        }
    }

    bool connect(const std::string& filename) {
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erreur : " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        std::cout << "Connexion SQLite OK : " << filename << std::endl;
        return true;
    }

    bool executeQuery(const std::string& query) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Erreur SQL : " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }
};

// ============================================
// FONCTION MAIN
// ============================================
int main() {
    std::cout << "=== MEDIATHEQUE ===" << std::endl;

    // 1. Connexion à la base SQLite
    Database db;
    if (!db.connect("data/mediatheque.db")) {
        std::cerr << "Impossible de se connecter." << std::endl;
        return 1;
    }

    std::cout << "\nConnexion OK !" << std::endl;

    // 2. Test de la classe Films
    std::cout << "\n--- Test Film ---" << std::endl;
    Films film(1, "Inception", 2010, 148, "ISAN123456");
    film.setNote(4.5);
    film.afficher();

    // 3. Test d'un deuxième film
    std::cout << "\n--- Test Film 2 ---" << std::endl;
    Films film2(2, "Interstellar", 2014, 169, "ISAN789012");
    film2.setNote(5);
    film2.setCommentaire("Chef d'oeuvre !");
    film2.afficher();

    return 0;
}
