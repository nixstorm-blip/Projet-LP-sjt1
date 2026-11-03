
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <sodium.h>
#include "Mediatheque.cpp"

class Users {

private:
    int id;
    std::string login;
    std::string passwordHash;
    Mediatheque* mediatheque = nullptr;

    struct EmpreinteExistante {};
     
    
    Users(
        int id,
        const std::string& login,
        const std::string& empreinte,
        EmpreinteExistante
    )
        : id(id),
          login(login),
          passwordHash(empreinte),
          mediatheque(nullptr) {}


    static std::string hacherMdp(const std::string& mdp) {

        if (sodium_init() < 0) {
            throw std::runtime_error(
                "Initialisation de libsodium impossible."
            );
        }

        char hash[crypto_pwhash_STRBYTES];

        if (crypto_pwhash_str(
                hash,
                mdp.c_str(),
                mdp.size(),
                crypto_pwhash_OPSLIMIT_INTERACTIVE,
                crypto_pwhash_MEMLIMIT_INTERACTIVE
            ) != 0) {

            throw std::runtime_error(
                "Hachage du mot de passe impossible."
            );
        }

        std::string resultat(hash);

        sodium_memzero(hash, sizeof(hash));

        return resultat;
    }

   
public:

    // Inscription : le mot de passe est hache
    Users(
        int id,
        const std::string& login,
        const std::string& mdp
    )
        : id(id),
          login(login),
          passwordHash(hacherMdp(mdp)),
          mediatheque(nullptr) {}

    Users(const Users&) = delete;
    Users& operator=(const Users&) = delete;

    // Charger une empreinte deja presente en JSON
    static std::unique_ptr<Users> depuisEmpreinte(
        int id,
        const std::string& login,
        const std::string& empreinte
    ) {

        if (id <= 0 ||
            login.empty() ||
            empreinte.rfind("$argon2id$", 0) != 0 ||
            empreinte.size() >= crypto_pwhash_STRBYTES) {

            throw std::invalid_argument(
                "Compte ou empreinte invalide."
            );
        }

        return std::unique_ptr<Users>(
            new Users(
                id,
                login,
                empreinte,
                EmpreinteExistante{}
            )
        );
    }

    int getId() const {
        return id;
    }

    void setId(int valeur) {
        id = valeur;
    }

    std::string getLogin() const {
        return login;
    }

    void setLogin(const std::string& valeur) {
        login = valeur;
    }

    std::string getPasswordHash() const {
        return passwordHash;
    }

    void setMdp(const std::string& mdp) {
        passwordHash = hacherMdp(mdp);
    }

    bool verifierMdp(const std::string& mdp) const {
        return crypto_pwhash_str_verify(
            passwordHash.c_str(),
            mdp.c_str(),
            mdp.size()
        ) == 0;
    }

    Mediatheque* getMediatheque() const {
        return mediatheque;
    }

    void setMediatheque(Mediatheque* nouvelleMediatheque) {

        if (mediatheque == nouvelleMediatheque) {
            return;
        }

        if (mediatheque != nullptr) {
            mediatheque->setUser(nullptr);
        }

        if (nouvelleMediatheque != nullptr &&
            nouvelleMediatheque->getUser() != nullptr) {

            nouvelleMediatheque->getUser()->mediatheque = nullptr;
        }

        mediatheque = nouvelleMediatheque;

        if (nouvelleMediatheque != nullptr) {
            nouvelleMediatheque->setUser(this);
        }
    }

    void modifier(
        const std::string& nouveauLogin,
        const std::string& nouveauMdp
    ) {
        login = nouveauLogin;
        setMdp(nouveauMdp);
    }

    std::string afficher() const {

        return "Users [id=" + std::to_string(id)
            + ", login=" + login
            + ", mediatheque="
            + (mediatheque != nullptr
                ? mediatheque->getNom()
                : "aucune")
            + "]";
    }
};
