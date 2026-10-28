
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>

#include "Models/Users.cpp"

using namespace std;

class Authentification {

private:
    // Liste des comptes : responsable de leur memoire
    vector<unique_ptr<Users>> comptes;

    // Utilisateur actuellement connecte
    Users* utilisateurActuel = nullptr;

    // Prochain identifiant disponible
    int prochainId = 1;

    // Rechercher un compte par son login
    Users* trouverParLogin(const string& login) const {

        for (const auto& utilisateur : comptes) {

            if (utilisateur->getLogin() == login) {
                return utilisateur.get();
            }
        }

        return nullptr;
    }

public:

    // INSCRIPTION
    bool inscrire(const string& login, const string& mdp) {

        // Login obligatoire
        if (login.find_first_not_of(" \t\r\n") == string::npos) {
            throw invalid_argument("Le login est obligatoire");
        }

       if (mdp.empty()) {
         throw invalid_argument(
        "Le mot de passe ne peut pas etre vide"
     );
}

        // Interdire deux comptes avec le meme login
        if (trouverParLogin(login) != nullptr) {
            return false;
        }

        // Users hache automatiquement le mot de passe
        auto utilisateur = make_unique<Users>(
            prochainId,
            login,
            mdp
        );

        // Ajouter le compte dans le tableau
        comptes.push_back(move(utilisateur));

        prochainId++;

        return true;
    }

    // CONNEXION
    bool connecter(const string& login, const string& mdp) {

        // Fermer la session precedente
        utilisateurActuel = nullptr;

        // Rechercher l'utilisateur
        Users* utilisateur = trouverParLogin(login);

        if (utilisateur == nullptr) {
            return false;
        }

        // Verifier le mot de passe hache
        if (!utilisateur->verifierMdp(mdp)) {
            return false;
        }

        // Connexion reussie
        utilisateurActuel = utilisateur;

        return true;
    }

    // DECONNEXION
    void deconnecter() {
        utilisateurActuel = nullptr;
    }

    // VERIFIER SI UNE SESSION EST ACTIVE
    bool estConnecte() const {
        return utilisateurActuel != nullptr;
    }

    // UTILISATEUR CONNECTE
    Users* getUtilisateurConnecte() const {
        return utilisateurActuel;
    }

    // NOMBRE DE COMPTES
    size_t nombreComptes() const {
        return comptes.size();
    }
};
