
#pragma once

#include <climits>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Models/Users.cpp"

// Structure utilisee pour la sauvegarde JSON
struct CompteSauvegarde {
    int id;
    std::string login;
    std::string passwordHash;
};

class Authentification {

private:
    std::vector<std::unique_ptr<Users>> comptes;
    Users* utilisateurActuel = nullptr;
    int prochainId = 1;

    Users* trouverParLogin(const std::string& login) const {
        for (const auto& utilisateur : comptes) {
            if (utilisateur->getLogin() == login) {
                return utilisateur.get();
            }
        }

        return nullptr;
    }

public:

    // INSCRIPTION
    bool inscrire(
        const std::string& login,
        const std::string& mdp
    ) {
        if (login.find_first_not_of(" \t\r\n")
            == std::string::npos) {
            throw std::invalid_argument(
                "Le login ne peut pas etre vide."
            );
        }

        if (mdp.empty()) {
            throw std::invalid_argument(
                "Le mot de passe ne peut pas etre vide."
            );
        }

        if (trouverParLogin(login) != nullptr) {
            return false;
        }

        if (prochainId == INT_MAX) {
            throw std::runtime_error(
                "Limite des identifiants atteinte."
            );
        }

        comptes.push_back(
            std::make_unique<Users>(
                prochainId, login, mdp
            )
        );

        ++prochainId;
        return true;
    }

    // CONNEXION
    bool connecter(
        const std::string& login,
        const std::string& mdp
    ) {
        utilisateurActuel = nullptr;

        Users* utilisateur = trouverParLogin(login);

        if (utilisateur == nullptr) {
            return false;
        }

        if (!utilisateur->verifierMdp(mdp)) {
            return false;
        }

        utilisateurActuel = utilisateur;
        return true;
    }

    // DECONNEXION
    void deconnecter() {
        utilisateurActuel = nullptr;
    }

    bool estConnecte() const {
        return utilisateurActuel != nullptr;
    }

    Users* getUtilisateurConnecte() const {
        return utilisateurActuel;
    }

    std::size_t nombreComptes() const {
        return comptes.size();
    }

    // SAUVEGARDE DES COMPTES
    std::vector<CompteSauvegarde> exporterComptes() const {

        std::vector<CompteSauvegarde> resultat;

        for (const auto& utilisateur : comptes) {
            resultat.push_back({
                utilisateur->getId(),
                utilisateur->getLogin(),
                utilisateur->getPasswordHash()
            });
        }

        return resultat;
    }

    // CHARGEMENT DES COMPTES
    void importerComptes(
        const std::vector<CompteSauvegarde>& sauvegarde
    ) {
        std::vector<std::unique_ptr<Users>> nouveauxComptes;
        std::set<int> ids;
        std::set<std::string> logins;

        int maximum = 0;

        for (const auto& compte : sauvegarde) {

            if (compte.id <= 0 ||
                compte.id == INT_MAX ||
                compte.login.find_first_not_of(" \t\r\n")
                    == std::string::npos ||
                !ids.insert(compte.id).second ||
                !logins.insert(compte.login).second) {

                throw std::runtime_error(
                    "Compte JSON invalide ou duplique."
                );
            }

            nouveauxComptes.push_back(
                Users::depuisEmpreinte(
                    compte.id,
                    compte.login,
                    compte.passwordHash
                )
            );

            if (compte.id > maximum) {
                maximum = compte.id;
            }
        }

        deconnecter();
        comptes.swap(nouveauxComptes);
        prochainId = maximum + 1;
    }
};
