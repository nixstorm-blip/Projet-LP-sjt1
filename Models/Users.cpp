
#pragma once

#include <string>
#include <stdexcept>
#include <sodium.h>
#include "Mediatheque.cpp"

using namespace std;

class Users {

private:
    int id;
    string login;
    string passwordHash;
    Mediatheque* mediatheque;

    // Hacher un mot de passe avec Argon2id
    static string hacherMdp(const string& motDePasse) {

        if (sodium_init() < 0) {
            throw runtime_error("Erreur initialisation libsodium");
        }

        char hash[crypto_pwhash_STRBYTES];

        if (crypto_pwhash_str(
            hash,
            motDePasse.c_str(),
            motDePasse.size(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE
        ) != 0) {
            throw runtime_error("Erreur de hachage");
        }

        string resultat(hash);
        sodium_memzero(hash, sizeof(hash));

        return resultat;
    }

public:

    // Constructeur : hache automatiquement le mot de passe
    Users(int id, string login, string mdp) {
        this->id = id;
        this->login = login;
        this->passwordHash = hacherMdp(mdp);
        this->mediatheque = nullptr;
    }

    // Eviter de copier un utilisateur
    Users(const Users&) = delete;
    Users& operator=(const Users&) = delete;

    // GETTERS ET SETTERS

    int getId() const {
        return this->id;
    }

    void setId(int id) {
        this->id = id;
    }

    string getLogin() const {
        return this->login;
    }

    void setLogin(const string& login) {
        this->login = login;
    }

    // Renvoie uniquement l'empreinte, jamais le mot de passe
    string getPasswordHash() const {
        return this->passwordHash;
    }

    // Changement du mot de passe
    void setMdp(const string& nouveauMdp) {
        this->passwordHash = hacherMdp(nouveauMdp);
    }

    // Verification du mot de passe avec Argon2id
    bool verifierMdp(const string& mdp) const {

        return crypto_pwhash_str_verify(
            this->passwordHash.c_str(),
            mdp.c_str(),
            mdp.size()
        ) == 0;
    }

    // ASSOCIATION AVEC MEDIATHEQUE

    Mediatheque* getMediatheque() const {
        return this->mediatheque;
    }

    void setMediatheque(Mediatheque* nouvelleMediatheque) {

        if (this->mediatheque == nouvelleMediatheque) {
            return;
        }

        // Detacher l'ancienne mediatheque
        if (this->mediatheque != nullptr) {
            this->mediatheque->setUser(nullptr);
        }

        // Detacher l'ancien utilisateur de la nouvelle mediatheque
        if (nouvelleMediatheque != nullptr &&
            nouvelleMediatheque->getUser() != nullptr) {

            nouvelleMediatheque->getUser()->mediatheque = nullptr;
        }

        this->mediatheque = nouvelleMediatheque;

        if (nouvelleMediatheque != nullptr) {
            nouvelleMediatheque->setUser(this);
        }
    }

    // MODIFICATION DU COMPTE

    void modifier(const string& nouveauLogin,
                  const string& nouveauMdp) {

        this->login = nouveauLogin;
        setMdp(nouveauMdp);
    }

    // AFFICHAGE SANS MOT DE PASSE

    string afficher() const {

        return "Users [id=" + to_string(this->id)
            + ", login=" + this->login
            + ", mediatheque="
            + (this->mediatheque != nullptr
                ? this->mediatheque->getNom()
                : "aucune")
            + "]";
    }
};
