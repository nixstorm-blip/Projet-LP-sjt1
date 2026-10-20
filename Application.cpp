// Application console additive. Les classes métier restent inchangées;
// les conversions propres aux classes sont centralisées dans Models/Parseur.cpp.
#include "Models/Mediatheque.cpp"
#include "Models/Media.cpp"
#include "Models/Films.cpp"
#include "Models/Serie.cpp"
#include "Models/Livres.cpp"
#include "Models/Album.cpp"
#include "Models/Jeux_video.cpp"
#include "Models/Format.cpp"
#include "Models/Genre.cpp"
#include "Models/Tag.cpp"
#include "Models/Personne.cpp"
#include "Models/Liste.cpp"
#include "Models/Franchise.cpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

// Parseur JSON autonome pour éviter d'imposer une bibliothèque tierce.
class Json {
public:
    using Array = std::vector<Json>;
    using Object = std::map<std::string, Json>;
private:
    using Data = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;
    Data data;

    static void appendUtf8(std::string& out, std::uint32_t code) {
        if (code <= 0x7f) out.push_back(static_cast<char>(code));
        else if (code <= 0x7ff) {
            out.push_back(static_cast<char>(0xc0 | (code >> 6)));
            out.push_back(static_cast<char>(0x80 | (code & 0x3f)));
        } else if (code <= 0xffff) {
            out.push_back(static_cast<char>(0xe0 | (code >> 12)));
            out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3f)));
            out.push_back(static_cast<char>(0x80 | (code & 0x3f)));
        } else {
            out.push_back(static_cast<char>(0xf0 | (code >> 18)));
            out.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3f)));
            out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3f)));
            out.push_back(static_cast<char>(0x80 | (code & 0x3f)));
        }
    }

    class Parser {
        const std::string& text;
        std::size_t pos = 0;

        void whitespace() {
            while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) ++pos;
        }
        char take() {
            if (pos >= text.size()) throw std::runtime_error("JSON incomplet.");
            return text[pos++];
        }
        void expect(char wanted) {
            whitespace();
            if (take() != wanted) throw std::runtime_error("JSON mal formé : caractère inattendu.");
        }
        std::uint32_t hex4() {
            std::uint32_t value = 0;
            for (int i = 0; i < 4; ++i) {
                const char c = take();
                value <<= 4;
                if (c >= '0' && c <= '9') value += static_cast<std::uint32_t>(c - '0');
                else if (c >= 'a' && c <= 'f') value += static_cast<std::uint32_t>(c - 'a' + 10);
                else if (c >= 'A' && c <= 'F') value += static_cast<std::uint32_t>(c - 'A' + 10);
                else throw std::runtime_error("Échappement Unicode invalide.");
            }
            return value;
        }
        std::string string() {
            expect('"');
            std::string out;
            while (true) {
                const unsigned char c = static_cast<unsigned char>(take());
                if (c == '"') return out;
                if (c < 0x20) throw std::runtime_error("Caractère de contrôle non échappé dans une chaîne JSON.");
                if (c != '\\') { out.push_back(static_cast<char>(c)); continue; }
                switch (take()) {
                case '"': out.push_back('"'); break;
                case '\\': out.push_back('\\'); break;
                case '/': out.push_back('/'); break;
                case 'b': out.push_back('\b'); break;
                case 'f': out.push_back('\f'); break;
                case 'n': out.push_back('\n'); break;
                case 'r': out.push_back('\r'); break;
                case 't': out.push_back('\t'); break;
                case 'u': {
                    std::uint32_t code = hex4();
                    if (code >= 0xd800 && code <= 0xdbff) {
                        if (take() != '\\' || take() != 'u') throw std::runtime_error("Paire Unicode invalide.");
                        const std::uint32_t low = hex4();
                        if (low < 0xdc00 || low > 0xdfff) throw std::runtime_error("Paire Unicode invalide.");
                        code = 0x10000 + ((code - 0xd800) << 10) + (low - 0xdc00);
                    } else if (code >= 0xdc00 && code <= 0xdfff) {
                        throw std::runtime_error("Demi-surrogate Unicode sans paire.");
                    }
                    Json::appendUtf8(out, code);
                    break;
                }
                default: throw std::runtime_error("Échappement JSON invalide.");
                }
            }
        }
        Json number() {
            const std::size_t start = pos;
            if (text[pos] == '-') ++pos;
            if (pos >= text.size()) throw std::runtime_error("Nombre JSON invalide.");
            if (text[pos] == '0') {
                ++pos;
                if (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) throw std::runtime_error("Zéro initial interdit dans un nombre JSON.");
            } else {
                if (text[pos] < '1' || text[pos] > '9') throw std::runtime_error("Nombre JSON invalide.");
                while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) ++pos;
            }
            if (pos < text.size() && text[pos] == '.') {
                ++pos;
                const std::size_t digitStart = pos;
                while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) ++pos;
                if (digitStart == pos) throw std::runtime_error("Fraction JSON invalide.");
            }
            if (pos < text.size() && (text[pos] == 'e' || text[pos] == 'E')) {
                ++pos;
                if (pos < text.size() && (text[pos] == '+' || text[pos] == '-')) ++pos;
                const std::size_t digitStart = pos;
                while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) ++pos;
                if (digitStart == pos) throw std::runtime_error("Exposant JSON invalide.");
            }
            char* end = nullptr;
            const std::string token = text.substr(start, pos - start);
            const double parsed = std::strtod(token.c_str(), &end);
            if (!end || *end != '\0' || !std::isfinite(parsed)) throw std::runtime_error("Nombre JSON hors limites.");
            return Json(parsed);
        }
        Json value() {
            whitespace();
            if (pos >= text.size()) throw std::runtime_error("JSON incomplet.");
            if (text[pos] == '"') return Json(string());
            if (text[pos] == '{') {
                ++pos;
                Object object;
                whitespace();
                if (pos < text.size() && text[pos] == '}') { ++pos; return Json(std::move(object)); }
                while (true) {
                    whitespace();
                    if (pos >= text.size() || text[pos] != '"') throw std::runtime_error("Clé d'objet JSON attendue.");
                    std::string key = string();
                    expect(':');
                    if (!object.emplace(key, value()).second) throw std::runtime_error("Clé JSON dupliquée : " + key);
                    whitespace();
                    const char separator = take();
                    if (separator == '}') return Json(std::move(object));
                    if (separator != ',') throw std::runtime_error("Objet JSON mal formé.");
                }
            }
            if (text[pos] == '[') {
                ++pos;
                Array array;
                whitespace();
                if (pos < text.size() && text[pos] == ']') { ++pos; return Json(std::move(array)); }
                while (true) {
                    array.push_back(value());
                    whitespace();
                    const char separator = take();
                    if (separator == ']') return Json(std::move(array));
                    if (separator != ',') throw std::runtime_error("Tableau JSON mal formé.");
                }
            }
            if (text.compare(pos, 4, "true") == 0) { pos += 4; return Json(true); }
            if (text.compare(pos, 5, "false") == 0) { pos += 5; return Json(false); }
            if (text.compare(pos, 4, "null") == 0) { pos += 4; return Json(nullptr); }
            if (text[pos] == '-' || std::isdigit(static_cast<unsigned char>(text[pos]))) return number();
            throw std::runtime_error("Valeur JSON invalide.");
        }
    public:
        explicit Parser(const std::string& source) : text(source) {}
        Json parse() {
            Json result = value();
            whitespace();
            if (pos != text.size()) throw std::runtime_error("Contenu inattendu après la valeur JSON.");
            return result;
        }
    };

    static std::string quote(const std::string& text) {
        std::ostringstream out;
        out << '"';
        for (unsigned char c : text) {
            switch (c) {
            case '"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b"; break;
            case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default:
                if (c < 0x20) out << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c) << std::dec;
                else out << static_cast<char>(c);
            }
        }
        out << '"';
        return out.str();
    }
    void write(std::ostream& out, int indent, int depth) const {
        if (std::holds_alternative<std::nullptr_t>(data)) out << "null";
        else if (const auto* b = std::get_if<bool>(&data)) out << (*b ? "true" : "false");
        else if (const auto* n = std::get_if<double>(&data)) out << std::setprecision(15) << *n;
        else if (const auto* s = std::get_if<std::string>(&data)) out << quote(*s);
        else if (const auto* a = std::get_if<Array>(&data)) {
            out << '[';
            for (std::size_t i = 0; i < a->size(); ++i) {
                if (i) out << ',';
                if (indent > 0) out << '\n' << std::string((depth + 1) * indent, ' ');
                (*a)[i].write(out, indent, depth + 1);
            }
            if (!a->empty() && indent > 0) out << '\n' << std::string(depth * indent, ' ');
            out << ']';
        } else {
            const auto& o = std::get<Object>(data);
            out << '{';
            std::size_t index = 0;
            for (const auto& entry : o) {
                if (index++) out << ',';
                if (indent > 0) out << '\n' << std::string((depth + 1) * indent, ' ');
                out << quote(entry.first) << (indent > 0 ? ": " : ":");
                entry.second.write(out, indent, depth + 1);
            }
            if (!o.empty() && indent > 0) out << '\n' << std::string(depth * indent, ' ');
            out << '}';
        }
    }
public:
    Json() : data(Object{}) {}
    Json(std::nullptr_t) : data(nullptr) {}
    Json(bool v) : data(v) {}
    Json(int v) : data(static_cast<double>(v)) {}
    Json(double v) : data(v) {}
    Json(const char* v) : data(std::string(v)) {}
    Json(const std::string& v) : data(v) {}
    Json(std::string&& v) : data(std::move(v)) {}
    Json(Array v) : data(std::move(v)) {}
    Json(Object v) : data(std::move(v)) {}
    Json(std::initializer_list<std::pair<std::string, Json>> fields) : data(Object{}) {
        auto& object = std::get<Object>(data);
        for (const auto& field : fields) object[field.first] = field.second;
    }
    static Json array() { return Json(Array{}); }
    bool isObject() const { return std::holds_alternative<Object>(data); }
    bool isArray() const { return std::holds_alternative<Array>(data); }
    Json& operator[](const std::string& key) {
        if (!isObject()) data = Object{};
        return std::get<Object>(data)[key];
    }
    const Json& at(const std::string& key) const {
        const auto* object = std::get_if<Object>(&data);
        if (!object) throw std::runtime_error("La valeur JSON attendue n'est pas un objet.");
        const auto found = object->find(key);
        if (found == object->end()) throw std::runtime_error("Champ JSON absent : " + key);
        return found->second;
    }
    void push(Json item) { std::get<Array>(data).push_back(std::move(item)); }
    Array::const_iterator begin() const { return std::get<Array>(data).begin(); }
    Array::const_iterator end() const { return std::get<Array>(data).end(); }
    bool isNull() const { return std::holds_alternative<std::nullptr_t>(data); }
    template<class T> T get() const {
        if constexpr (std::is_same_v<T, Json>) return *this;
        else if constexpr (std::is_same_v<T, std::string>) return std::get<std::string>(data);
        else if constexpr (std::is_same_v<T, bool>) return std::get<bool>(data);
        else if constexpr (std::is_same_v<T, int>) {
            const double number = std::get<double>(data);
            if (std::trunc(number) != number || number < std::numeric_limits<int>::min() || number > std::numeric_limits<int>::max()) throw std::runtime_error("Entier JSON invalide ou hors limites.");
            return static_cast<int>(number);
        } else throw std::runtime_error("Type JSON demandé non pris en charge.");
    }
    template<class T> T value(const std::string& key, T fallback) const {
        const auto* object = std::get_if<Object>(&data);
        if (!object) return fallback;
        const auto found = object->find(key);
        return found == object->end() ? fallback : found->second.template get<T>();
    }
    std::string value(const std::string& key, const char* fallback) const { return value<std::string>(key, std::string(fallback)); }
    std::string dump(int indent = 2) const { std::ostringstream out; write(out, indent, 0); return out.str(); }
    static Json parse(const std::string& input) { return Parser(input).parse(); }
    friend std::istream& operator>>(std::istream& in, Json& json) {
        const std::string text((std::istreambuf_iterator<char>(in)), {});
        json = parse(text);
        return in;
    }
};

#include "Models/Parseur.cpp"

class Application {
private:
    // Les objets de référence restent vivants jusqu'à la destruction des bibliothèques.
    std::vector<std::unique_ptr<Personne>> personnes;
    std::vector<std::unique_ptr<Format>> formats;
    std::vector<std::unique_ptr<Genre>> genres;
    std::vector<std::unique_ptr<Liste>> listes;
    std::vector<std::unique_ptr<Franchise>> franchises;
    // Dépôt privé propriétaire des médias; les médiathèques ordinaires sont seulement des liens.
    // Déclaré avant bibliotheques pour être détruit après elles (destruction C++ en ordre inverse).
    std::unique_ptr<Mediatheque> depotMedias;
    std::vector<std::unique_ptr<Mediatheque>> bibliotheques;
    std::map<int, std::string> nationalites;
    std::map<int, std::string> artistesAlbums;
    std::map<std::string, std::string> rolesCreateurs;
    const std::string cheminJson = "mediatheque.json";
    bool chargementInvalide = false;

    static std::string lireTexte(const std::string& invite) {
        std::cout << invite;
        std::string texte;
        if (!std::getline(std::cin, texte)) throw std::runtime_error("Fin de saisie.");
        return texte;
    }
    static int lireEntier(const std::string& invite, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) {
        while (true) {
            const std::string texte = lireTexte(invite);
            try {
                std::size_t position = 0;
                const long long nombre = std::stoll(texte, &position);
                if (position == texte.size() && nombre >= min && nombre <= max) return static_cast<int>(nombre);
            } catch (const std::exception&) {}
            std::cout << "Saisis un entier entre " << min << " et " << max << ".\n";
        }
    }
    static bool lireOuiNon(const std::string& invite) {
        while (true) {
            std::string valeur = lireTexte(invite + " (o/n) : ");
            std::transform(valeur.begin(), valeur.end(), valeur.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            if (valeur == "o" || valeur == "oui" || valeur == "1") return true;
            if (valeur == "n" || valeur == "non" || valeur == "0") return false;
            std::cout << "Réponds par o ou n.\n";
        }
    }
    static std::string cleRole(int mediaId, int personneId) { return std::to_string(mediaId) + ":" + std::to_string(personneId); }
    template<class T> static int prochainId(const std::vector<std::unique_ptr<T>>& objects) {
        int maximum = 0;
        for (const auto& object : objects) maximum = std::max(maximum, object->getId());
        return maximum + 1;
    }
    std::vector<Media*> tousLesMedias() const {
        return depotMedias ? depotMedias->getMedias() : std::vector<Media*>{};
    }
    Media* trouverMedia(int id) const {
        for (Media* media : tousLesMedias()) if (media->getId() == id) return media;
        return nullptr;
    }
    Personne* trouverPersonne(int id) const { for (const auto& p : personnes) if (p->getId() == id) return p.get(); return nullptr; }
    Liste* trouverListe(int id) const { for (const auto& p : listes) if (p->getId() == id) return p.get(); return nullptr; }
    Franchise* trouverFranchise(int id) const { for (const auto& p : franchises) if (p->getId() == id) return p.get(); return nullptr; }
    Genre* trouverGenre(int id) const { for (const auto& p : genres) if (p->getId() == id) return p.get(); return nullptr; }
    Format* trouverFormat(int id) const { for (const auto& p : formats) if (p->getId() == id) return p.get(); return nullptr; }
    Mediatheque* trouverBibliotheque(int id) const { for (const auto& p : bibliotheques) if (p->getId() == id) return p.get(); return nullptr; }

    void assurerBibliothequePrincipale() {
        if (bibliotheques.empty()) bibliotheques.push_back(Parseur::chargerMediatheque(prochainId(bibliotheques), "Ma médiathèque", "Collection principale"));
    }

    int prochainIdMedia() const { int max = 0; for (Media* m : tousLesMedias()) max = std::max(max, m->getId()); return max + 1; }
    static std::string typeMedia(Media* media) {
        if (dynamic_cast<Films*>(media)) return "Film";
        if (dynamic_cast<Serie*>(media)) return "Serie";
        if (dynamic_cast<Livre*>(media)) return "Livre";
        if (dynamic_cast<JeuVideo*>(media)) return "JeuVideo";
        if (dynamic_cast<Album*>(media)) return "Album";
        return "Media";
    }
    std::string affichageEtendu(Media* media) const {
        std::string resultat = media->afficher();
        if (dynamic_cast<Album*>(media)) {
            auto it = artistesAlbums.find(media->getId());
            if (it != artistesAlbums.end()) resultat += ", artiste=" + it->second;
        }
        resultat += "\n  Créateurs : ";
        if (media->getCreateurs().empty()) resultat += "aucun";
        for (Personne* personne : media->getCreateurs()) {
            if (personne) {
                resultat += personne->getPrenom() + " " + personne->getNom();
                const auto role = rolesCreateurs.find(cleRole(media->getId(), personne->getId()));
                if (role != rolesCreateurs.end() && !role->second.empty()) resultat += " (" + role->second + ")";
                resultat += "; ";
            }
        }
        resultat += "\n  Tags : ";
        if (media->getTags().empty()) resultat += "aucun";
        for (Tag* tag : media->getTags()) if (tag) resultat += tag->getNom() + "; ";
        resultat += "\n  Listes : ";
        bool trouve = false;
        for (const auto& liste : listes) for (Media* m : liste->getMedias()) if (m == media) { resultat += liste->getNom() + "; "; trouve = true; }
        if (!trouve) resultat += "aucune";
        resultat += "\n  Franchises : ";
        trouve = false;
        for (const auto& franchise : franchises) for (Media* m : franchise->getMedias()) if (m == media) { resultat += franchise->getNom() + "; "; trouve = true; }
        if (!trouve) resultat += "aucune";
        return resultat;
    }

    void afficherMedias(const std::vector<Media*>& medias) const {
        if (medias.empty()) { std::cout << "Aucun média correspondant.\n"; return; }
        for (Media* media : medias) std::cout << affichageEtendu(media) << "\n\n";
    }
    void afficherTous() const { afficherMedias(tousLesMedias()); }
    void ajouterMedia() {
        if (!depotMedias) depotMedias = std::make_unique<Mediatheque>(-1, "Dépôt interne", "Propriétaire des médias de l'application");
        assurerBibliothequePrincipale();
        std::cout << "1 Livre | 2 Film | 3 Série | 4 Jeu vidéo | 5 Album\n";
        const int type = lireEntier("Type : ", 1, 5);
        const int id = prochainIdMedia();
        const std::string titre = lireTexte("Titre : ");
        if (titre.empty()) { std::cout << "Le titre ne peut pas être vide.\n"; return; }
        const std::string annee = lireTexte("Année : ");
        const int note = lireEntier("Note (0-10) : ", 0, 10);
        const std::string commentaire = lireTexte("Commentaire : ");
        const bool possede = lireOuiNon("Possédé");
        Media* media = nullptr;
        if (type == 1) {
            const std::string edition = lireTexte("Édition : ");
            const std::string isbn = lireTexte("ISBN : ");
            media = new Livre(id, titre, annee, note, commentaire, possede, edition, isbn);
        } else if (type == 2) {
            const int duree = lireEntier("Durée en minutes : ", 0);
            const std::string isan = lireTexte("ISAN : ");
            media = new Films(id, titre, annee, note, commentaire, possede, duree, isan);
        } else if (type == 3) {
            const int saisons = lireEntier("Nombre de saisons : ", 0);
            const int episodes = lireEntier("Nombre d'épisodes : ", 0);
            media = new Serie(id, titre, annee, note, commentaire, possede, saisons, episodes);
        } else if (type == 4) {
            const std::string editeur = lireTexte("Éditeur : ");
            const std::string plateforme = lireTexte("Plateforme : ");
            media = new JeuVideo(id, titre, annee, note, commentaire, possede, editeur, plateforme);
        } else {
            media = new Album(id, titre, annee, note, commentaire, possede);
            artistesAlbums[id] = lireTexte("Artiste (métadonnée complémentaire) : ");
        }
        depotMedias->ajouterMedia(media);
        bibliotheques.front()->ajouterMedia(media);
        std::cout << "Média créé avec l'identifiant " << id << ".\n";
    }
    void supprimerMedia() {
        const int id = lireEntier("Identifiant du média à supprimer : ", 1);
        Media* media = trouverMedia(id);
        if (!media) { std::cout << "Média introuvable.\n"; return; }
        for (auto& liste : listes) liste->retirerMedia(media);
        for (auto& franchise : franchises) franchise->retirerMedia(media);
        for (Personne* createur : media->getCreateurs()) media->retirerCreateur(createur);
        artistesAlbums.erase(id);
        for (auto it = rolesCreateurs.begin(); it != rolesCreateurs.end();) {
            if (it->first.rfind(std::to_string(id) + ":", 0) == 0) it = rolesCreateurs.erase(it); else ++it;
        }
        for (Mediatheque* lien : media->getMediatheques())
            if (lien != depotMedias.get()) lien->supprimerMedia(id);
        if (depotMedias) depotMedias->supprimerMedia(id);
        std::cout << "Média et ses associations supprimés.\n";
    }
    void modifierMedia() {
        Media* media = trouverMedia(lireEntier("Identifiant du média à modifier : ", 1));
        if (!media) { std::cout << "Média introuvable.\n"; return; }
        while (true) {
            std::cout << "1 Titre  2 Année  3 Note  4 Commentaire  5 Possédé  6 Attribut spécialisé  0 Terminer\n";
            const int choix = lireEntier("Modification : ", 0, 6);
            if (choix == 0) return;
            if (choix == 1) { std::string v = lireTexte("Nouveau titre : "); if (!v.empty()) media->setTitre(v); }
            else if (choix == 2) media->setAnnee(lireTexte("Nouvelle année : "));
            else if (choix == 3) media->setNote(lireEntier("Nouvelle note (0-10) : ", 0, 10));
            else if (choix == 4) media->setCommentaire(lireTexte("Nouveau commentaire : "));
            else if (choix == 5) media->setPossede(lireOuiNon("Possédé"));
            else if (choix == 6) {
                if (auto* livre = dynamic_cast<Livre*>(media)) { livre->setEdition(lireTexte("Édition : ")); livre->setIsbn(lireTexte("ISBN : ")); }
                else if (auto* film = dynamic_cast<Films*>(media)) { film->setDuree(lireEntier("Durée : ", 0)); film->setIsan(lireTexte("ISAN : ")); }
                else if (auto* serie = dynamic_cast<Serie*>(media)) { serie->setNombreSaisons(lireEntier("Saisons : ", 0)); serie->setNombreEpisodes(lireEntier("Épisodes : ", 0)); }
                else if (auto* jeu = dynamic_cast<JeuVideo*>(media)) { jeu->setEditeur(lireTexte("Éditeur : ")); jeu->setPlateforme(lireTexte("Plateforme : ")); }
                else if (dynamic_cast<Album*>(media)) artistesAlbums[media->getId()] = lireTexte("Artiste : ");
            }
        }
    }
    void creerPersonne() {
        const int id = prochainId(personnes);
        const std::string nom = lireTexte("Nom : ");
        if (nom.empty()) { std::cout << "Le nom est obligatoire.\n"; return; }
        const std::string prenom = lireTexte("Prénom : ");
        const std::string nationalite = lireTexte("Nationalité (extension de l'application) : ");
        for (const auto& personne : personnes) {
            if (personne->getNom() == nom && personne->getPrenom() == prenom) {
                std::cout << "Cette personne existe déjà.\n";
                return;
            }
        }
        personnes.push_back(std::make_unique<Personne>(id, nom, prenom));
        nationalites[id] = nationalite;
        std::cout << "Personne créée, id " << id << ".\n";
    }
    void creerListe() {
        const int id = prochainId(listes);
        const std::string nom = lireTexte("Nom de la liste : ");
        if (nom.empty()) { std::cout << "Le nom est obligatoire.\n"; return; }
        for (const auto& liste : listes) {
            if (liste->getNom() == nom) { std::cout << "Cette liste existe déjà.\n"; return; }
        }
        listes.push_back(std::make_unique<Liste>(id, nom, lireTexte("Description : ")));
        std::cout << "Liste créée, id " << id << ".\n";
    }
    void creerFranchise() {
        const int id = prochainId(franchises);
        const std::string nom = lireTexte("Nom de la franchise : ");
        if (nom.empty()) { std::cout << "Le nom est obligatoire.\n"; return; }
        for (const auto& franchise : franchises) {
            if (franchise->getNom() == nom) { std::cout << "Cette franchise existe déjà.\n"; return; }
        }
        franchises.push_back(std::make_unique<Franchise>(id, nom, lireTexte("Description : ")));
        std::cout << "Franchise créée, id " << id << ".\n";
    }
    void creerGenre() {
        const int id = prochainId(genres);
        const std::string nom = lireTexte("Genre : ");
        if (nom.empty()) { std::cout << "Le genre est obligatoire.\n"; return; }
        for (const auto& genre : genres) {
            if (genre->getLibelle() == nom) { std::cout << "Ce genre existe déjà.\n"; return; }
        }
        genres.push_back(std::make_unique<Genre>(id, nom));
        std::cout << "Genre créé, id " << id << ".\n";
    }
    void creerFormat() {
        const int id = prochainId(formats);
        const std::string nom = lireTexte("Format : ");
        if (nom.empty()) { std::cout << "Le format est obligatoire.\n"; return; }
        for (const auto& format : formats) {
            if (format->getNomFormat() == nom) { std::cout << "Ce format existe déjà.\n"; return; }
        }
        formats.push_back(std::make_unique<Format>(id, nom));
        std::cout << "Format créé, id " << id << ".\n";
    }
    void creerBibliotheque() {
        const int id = prochainId(bibliotheques);
        const std::string nom = lireTexte("Nom de la médiathèque : ");
        if (nom.empty()) { std::cout << "Le nom est obligatoire.\n"; return; }
        const std::string description = lireTexte("Description : ");
        for (const auto& bibliotheque : bibliotheques) {
            if (bibliotheque->getNom() == nom) { std::cout << "Cette médiathèque existe déjà.\n"; return; }
        }
        bibliotheques.push_back(std::make_unique<Mediatheque>(id, nom, description));
        std::cout << "Médiathèque créée, id " << id << ".\n";
    }
    void associer() {
        std::cout << "1 Médiathèque  2 Personne/créateur  3 Liste  4 Franchise  5 Genre  6 Format  7 Tag\n";
        const int type = lireEntier("Relation : ", 1, 7);
        Media* media = trouverMedia(lireEntier("ID du média : ", 1));
        if (!media) { std::cout << "Média introuvable.\n"; return; }
        if (type == 1) {
            const int id = lireEntier("ID de la médiathèque : ", 1); Mediatheque* b = trouverBibliotheque(id);
            if (!b) std::cout << "Médiathèque introuvable.\n"; else { b->ajouterMedia(media); std::cout << "Association ajoutée.\n"; }
        } else if (type == 2) {
            const int id = lireEntier("ID de la personne : ", 1); Personne* p = trouverPersonne(id);
            if (!p) std::cout << "Personne introuvable.\n";
            else { media->ajouterCreateur(p); rolesCreateurs[cleRole(media->getId(), p->getId())] = lireTexte("Rôle (auteur, réalisateur...) : "); }
        } else if (type == 3) {
            const int id = lireEntier("ID de la liste : ", 1); Liste* p = trouverListe(id);
            if (!p) std::cout << "Liste introuvable.\n"; else if (!p->ajouterMedia(media)) std::cout << "Média déjà dans cette liste.\n"; else std::cout << "Association ajoutée.\n";
        } else if (type == 4) {
            const int id = lireEntier("ID de la franchise : ", 1); Franchise* p = trouverFranchise(id);
            if (!p) std::cout << "Franchise introuvable.\n"; else if (!p->ajouterMedia(media)) std::cout << "Média déjà dans cette franchise.\n"; else std::cout << "Association ajoutée.\n";
        } else if (type == 5) {
            const int id = lireEntier("ID du genre : ", 1); Genre* p = trouverGenre(id);
            if (!p) std::cout << "Genre introuvable.\n"; else { media->setGenre(p); std::cout << "Genre affecté.\n"; }
        } else if (type == 6) {
            const int id = lireEntier("ID du format : ", 1); Format* p = trouverFormat(id);
            if (!p) std::cout << "Format introuvable.\n"; else { media->setFormat(p); std::cout << "Format affecté.\n"; }
        } else {
            std::string nom = lireTexte("Nom du tag : "); std::string desc = lireTexte("Description du tag : ");
            int id = 1; for (Media* m : tousLesMedias()) for (Tag* t : m->getTags()) if (t) id = std::max(id, t->getId() + 1);
            media->ajouterTag(new Tag(id, nom, desc));
            std::cout << "Tag ajouté.\n";
        }
    }
    void afficherCollections() const {
        std::cout << "\n-- Listes --\n"; for (const auto& x : listes) std::cout << x->toString() << "\n";
        std::cout << "-- Franchises --\n"; for (const auto& x : franchises) std::cout << x->toString() << "\n";
        std::cout << "-- Genres --\n"; for (const auto& x : genres) std::cout << x->afficher() << "\n";
        std::cout << "-- Formats --\n"; for (const auto& x : formats) std::cout << x->afficher() << "\n";
        std::cout << "-- Médiathèques --\n"; for (const auto& x : bibliotheques) std::cout << x->afficher() << "\n";
        std::cout << "-- Personnes --\n";
        for (const auto& x : personnes) { std::cout << x->getId() << " : " << x->getPrenom() << " " << x->getNom(); auto it = nationalites.find(x->getId()); if (it != nationalites.end()) std::cout << " (" << it->second << ")"; std::cout << "\n"; }
    }

    Json versJson() const {
        Json root;
        root["version"] = 1;
        root["personnes"] = Json::array(); root["formats"] = Json::array(); root["genres"] = Json::array();
        root["listes"] = Json::array(); root["franchises"] = Json::array(); root["bibliotheques"] = Json::array(); root["medias"] = Json::array();
        for (const auto& p : personnes) root["personnes"].push(Json::parse(Parseur::sauvegarderPersonne(p.get(), nationalites.count(p->getId()) ? nationalites.at(p->getId()) : "")));
        for (const auto& f : formats) root["formats"].push(Json::parse(Parseur::sauvegarderFormat(f.get())));
        for (const auto& g : genres) root["genres"].push(Json::parse(Parseur::sauvegarderGenre(g.get())));
        for (const auto& l : listes) root["listes"].push(Json::parse(Parseur::sauvegarderListe(l.get())));
        for (const auto& f : franchises) root["franchises"].push(Json::parse(Parseur::sauvegarderFranchise(f.get())));
        for (const auto& b : bibliotheques) root["bibliotheques"].push(Json::parse(Parseur::sauvegarderMediatheque(b.get())));
        for (Media* m : tousLesMedias()) {
            const std::string artiste = artistesAlbums.count(m->getId()) ? artistesAlbums.at(m->getId()) : "";
            Json item = Json::parse(Parseur::sauvegarderMedia(m, artiste));
            item["createurs"] = Json::array(); item["tags"] = Json::array(); item["listes"] = Json::array(); item["franchises"] = Json::array(); item["bibliotheques"] = Json::array();
            if (m->getFormat()) item["formatId"] = m->getFormat()->getId(); else item["formatId"] = nullptr;
            if (m->getGenre()) item["genreId"] = m->getGenre()->getId(); else item["genreId"] = nullptr;
            for (Personne* p : m->getCreateurs()) item["createurs"].push({{"id",p->getId()},{"role",rolesCreateurs.count(cleRole(m->getId(),p->getId())) ? rolesCreateurs.at(cleRole(m->getId(),p->getId())) : ""}});
            for (Tag* t : m->getTags()) if (t) item["tags"].push(Json::parse(Parseur::sauvegarderTag(t)));
            for (const auto& l : listes) for (Media* linked : l->getMedias()) if (linked == m) item["listes"].push(l->getId());
            for (const auto& f : franchises) for (Media* linked : f->getMedias()) if (linked == m) item["franchises"].push(f->getId());
            for (Mediatheque* b : m->getMediatheques())
                if (b && b != depotMedias.get()) item["bibliotheques"].push(b->getId());
            root["medias"].push(std::move(item));
        }
        return root;
    }
    void sauvegarder() const {
        if (chargementInvalide) {
            std::cerr << "Sauvegarde refusée : un chargement JSON a échoué. Répare ou déplace le fichier avant de le remplacer.\n";
            return;
        }
        try {
            const std::string temporaire = cheminJson + ".tmp";
            { std::ofstream file(temporaire, std::ios::binary | std::ios::trunc); if (!file) throw std::runtime_error("Impossible d'ouvrir le fichier temporaire."); file << versJson().dump(2) << '\n'; file.flush(); if (!file) throw std::runtime_error("Échec d'écriture du JSON."); }
            if (std::rename(temporaire.c_str(), cheminJson.c_str()) != 0) { std::remove(temporaire.c_str()); throw std::runtime_error("Impossible de remplacer " + cheminJson); }
            std::cout << "Sauvegarde réussie dans " << cheminJson << ".\n";
        } catch (const std::exception& error) { std::cerr << "Erreur de sauvegarde : " << error.what() << '\n'; }
    }
    void charger() {
        std::ifstream file(cheminJson, std::ios::binary);
        if (!file) { chargementInvalide = false; std::cout << "Aucun fichier " << cheminJson << " : démarrage avec une collection vide.\n"; return; }
        try {
            Json root; file >> root;
            if (!root.isObject() || root.at("version").get<int>() != 1) throw std::runtime_error("Format ou version JSON non pris en charge.");
            const std::vector<std::string> required = {"personnes", "formats", "genres", "listes", "franchises", "bibliotheques", "medias"};
            for (const std::string& key : required) {
                if (!root.at(key).isArray()) throw std::runtime_error("Le champ '" + key + "' doit être un tableau JSON.");
            }
            // Construire un nouvel état. À sa destruction, les médiathèques se
            // détachent avant le dépôt qui possède les médias; les autres objets
            // restent vivants pendant la destruction de ces médias.
            std::vector<std::unique_ptr<Personne>> newPeople;
            std::vector<std::unique_ptr<Format>> newFormats;
            std::vector<std::unique_ptr<Genre>> newGenres;
            std::vector<std::unique_ptr<Liste>> newLists;
            std::vector<std::unique_ptr<Franchise>> newFranchises;
            auto newDepot = Parseur::chargerMediatheque(-1, "Dépôt interne", "Propriétaire des médias");
            std::vector<std::unique_ptr<Mediatheque>> newLibraries;
            std::map<int,std::string> newNationalities, newArtists;
            std::map<std::string,std::string> newRoles;
            std::set<int> peopleIds, formatIds, genreIds, listIds, franchiseIds, libraryIds, mediaIds;
            auto verifyId = [](int id, std::set<int>& ids, const char* type) {
                if (id <= 0 || !ids.insert(id).second)
                    throw std::runtime_error(std::string("Identifiant invalide ou dupliqué pour ") + type + ".");
            };
            std::set<std::string> peopleNames, formatNames, genreNames, listNames, franchiseNames, libraryNames;
            for (const Json& x : root.at("personnes")) { int id=x.at("id").get<int>(); verifyId(id,peopleIds,"personne"); std::string nom=x.value("nom", ""), prenom=x.value("prenom", ""); if (nom.empty() || !peopleNames.insert(nom + "\x1f" + prenom).second) throw std::runtime_error("Nom de personne vide ou dupliqué."); newPeople.push_back(Parseur::chargerPersonne(id,nom,prenom)); newNationalities[id]=x.value("nationalite",""); }
            for (const Json& x : root.at("formats")) { int id=x.at("id").get<int>(); verifyId(id,formatIds,"format"); std::string nom=x.value("nom", ""); if (nom.empty() || !formatNames.insert(nom).second) throw std::runtime_error("Format vide ou dupliqué."); newFormats.push_back(Parseur::chargerFormat(id,nom)); }
            for (const Json& x : root.at("genres")) { int id=x.at("id").get<int>(); verifyId(id,genreIds,"genre"); std::string nom=x.value("nom", ""); if (nom.empty() || !genreNames.insert(nom).second) throw std::runtime_error("Genre vide ou dupliqué."); newGenres.push_back(Parseur::chargerGenre(id,nom)); }
            for (const Json& x : root.at("listes")) { int id=x.at("id").get<int>(); verifyId(id,listIds,"liste"); std::string nom=x.value("nom", ""); if (nom.empty() || !listNames.insert(nom).second) throw std::runtime_error("Liste vide ou dupliquée."); newLists.push_back(Parseur::chargerListe(id,nom,x.value("description",""))); }
            for (const Json& x : root.at("franchises")) { int id=x.at("id").get<int>(); verifyId(id,franchiseIds,"franchise"); std::string nom=x.value("nom", ""); if (nom.empty() || !franchiseNames.insert(nom).second) throw std::runtime_error("Franchise vide ou dupliquée."); newFranchises.push_back(Parseur::chargerFranchise(id,nom,x.value("description",""))); }
            for (const Json& x : root.at("bibliotheques")) { int id=x.at("id").get<int>(); verifyId(id,libraryIds,"médiathèque"); std::string nom=x.value("nom", ""); if (nom.empty() || !libraryNames.insert(nom).second) throw std::runtime_error("Médiathèque vide ou dupliquée."); newLibraries.push_back(Parseur::chargerMediatheque(id,nom,x.value("description",""))); }
            auto findIn = [](auto& objects, int id) -> auto* { for (auto& obj : objects) if (obj->getId()==id) return obj.get(); return static_cast<typename std::remove_reference_t<decltype(objects)>::value_type::element_type*>(nullptr); };

            for (const Json& x : root.at("medias")) {
                const int id=x.at("id").get<int>(); verifyId(id,mediaIds,"média");
                const std::string type=x.at("type").get<std::string>();
                const std::string title=x.value("titre",""), year=x.value("annee",""), comment=x.value("commentaire","");
                if (title.empty()) throw std::runtime_error("Un titre de média ne peut pas être vide.");
                const int rating=x.value("note",0);
                if (rating < 0 || rating > 10) throw std::runtime_error("Note invalide (attendu 0 à 10).");
                auto media = Parseur::chargerMedia(type,id,title,year,rating,comment,x.value("possede",false),
                    x.value("edition",""),x.value("isbn",""),x.value("duree",0),x.value("isan",""),
                    x.value("saisons",0),x.value("episodes",0),x.value("editeur",""),x.value("plateforme",""),x.value("artiste",""));
                newDepot->ajouterMedia(media.get());
                media.release(); // Le dépôt garde l'unique propriété du média.
                if (type=="Album") newArtists[id]=x.value("artiste","");
            }

            for (const Json& x : root.at("medias")) {
                const int mediaId=x.at("id").get<int>();
                Media* media = nullptr;
                for (Media* candidate : newDepot->getMedias())
                    if (candidate && candidate->getId() == mediaId) { media = candidate; break; }
                if (!media) throw std::runtime_error("Média absent de son dépôt interne.");
                auto checkUnique = [](std::set<int>& ids, int id, const char* association) {
                    if (!ids.insert(id).second) throw std::runtime_error(std::string("Association dupliquée : ") + association + ".");
                };
                std::set<int> creatorLinks, tagLinks, listLinks, franchiseLinks, libraryLinks;
                for (const Json& cid : x.at("createurs")) {
                    const int id=cid.at("id").get<int>(); checkUnique(creatorLinks,id,"créateur");
                    Personne* p=findIn(newPeople,id); if (!p) throw std::runtime_error("Créateur référencé introuvable.");
                    media->ajouterCreateur(p); newRoles[cleRole(mediaId,id)]=cid.value("role","");
                }
                for (const Json& t : x.at("tags")) {
                    const int id=t.at("id").get<int>(); checkUnique(tagLinks,id,"tag");
                    auto tag=Parseur::chargerTag(id,t.value("nom",""),t.value("description",""));
                    media->ajouterTag(tag.get()); tag.release();
                }
                const Json& fid=x.at("formatId");
                if (!fid.isNull()) { Format* f=findIn(newFormats,fid.get<int>()); if (!f) throw std::runtime_error("Format référencé introuvable."); media->setFormat(f); }
                const Json& gid=x.at("genreId");
                if (!gid.isNull()) { Genre* g=findIn(newGenres,gid.get<int>()); if (!g) throw std::runtime_error("Genre référencé introuvable."); media->setGenre(g); }
                for (const Json& lid : x.at("listes")) { int id=lid.get<int>(); checkUnique(listLinks,id,"liste"); Liste* l=findIn(newLists,id); if (!l) throw std::runtime_error("Liste référencée introuvable."); l->ajouterMedia(media); }
                for (const Json& fid : x.at("franchises")) { int id=fid.get<int>(); checkUnique(franchiseLinks,id,"franchise"); Franchise* f=findIn(newFranchises,id); if (!f) throw std::runtime_error("Franchise référencée introuvable."); f->ajouterMedia(media); }
                for (const Json& bid : x.at("bibliotheques")) { int id=bid.get<int>(); checkUnique(libraryLinks,id,"médiathèque"); Mediatheque* b=findIn(newLibraries,id); if (!b) throw std::runtime_error("Médiathèque référencée introuvable."); b->ajouterMedia(media); }
            }

            // Commit : supprimer les anciens liens, puis détruire l'ancien dépôt
            // pendant que ses anciens objets référencés existent encore.
            for (Media* oldMedia : tousLesMedias()) {
                for (auto& liste : listes) liste->retirerMedia(oldMedia);
                for (auto& franchise : franchises) franchise->retirerMedia(oldMedia);
            }
            bibliotheques.clear();
            depotMedias.reset();
            personnes.clear(); formats.clear(); genres.clear(); listes.clear(); franchises.clear();
            personnes=std::move(newPeople); formats=std::move(newFormats); genres=std::move(newGenres);
            listes=std::move(newLists); franchises=std::move(newFranchises);
            depotMedias=std::move(newDepot); bibliotheques=std::move(newLibraries);
            nationalites=std::move(newNationalities); artistesAlbums=std::move(newArtists); rolesCreateurs=std::move(newRoles);
            chargementInvalide=false;
            std::cout << "Chargement JSON réussi.\n";
        } catch (const std::exception& error) {
            chargementInvalide=true;
            std::cerr << "Chargement annulé, données en mémoire conservées : " << error.what() << '\n';
        }
    }
    void filtrer() const;
public:
    Application() : depotMedias(Parseur::chargerMediatheque(-1, "Dépôt interne", "Propriétaire des médias")) {
        assurerBibliothequePrincipale();
    }
    void executer() {
        charger();
        while (true) {
            std::cout << "\n=== MÉDIATHÈQUE PERSONNELLE ===\n"
                      << "1 Afficher médias  2 Ajouter média  3 Modifier média  4 Supprimer média\n"
                      << "5 Filtrer/rechercher  6 Créer personne  7 Créer liste  8 Créer franchise\n"
                      << "9 Créer genre  10 Créer format  11 Créer médiathèque  12 Associer\n"
                      << "13 Afficher collections  14 Sauvegarder  15 Recharger  0 Quitter\n";
            std::string choix;
            try { choix = lireTexte("Choix : "); }
            catch (const std::exception&) { sauvegarder(); return; }
            if (choix == "0") { sauvegarder(); return; }
            try {
                if (choix == "1") afficherTous();
                else if (choix == "2") ajouterMedia();
                else if (choix == "3") modifierMedia();
                else if (choix == "4") supprimerMedia();
                else if (choix == "5") filtrer();
                else if (choix == "6") creerPersonne();
                else if (choix == "7") creerListe();
                else if (choix == "8") creerFranchise();
                else if (choix == "9") creerGenre();
                else if (choix == "10") creerFormat();
                else if (choix == "11") creerBibliotheque();
                else if (choix == "12") associer();
                else if (choix == "13") afficherCollections();
                else if (choix == "14") sauvegarder();
                else if (choix == "15") charger();
                else std::cout << "Choix inconnu.\n";
            } catch (const std::exception& error) { std::cout << "Action impossible : " << error.what() << '\n'; }
        }
    }
};

void Application::filtrer() const {
    std::cout << "1 Type  2 Genre  3 Liste (id)  4 Franchise (id)  5 Possession  6 Titre\n";
    const int filtre = lireEntier("Filtre : ", 1, 6);
    const std::string valeur = lireTexte("Valeur : ");
    std::vector<Media*> resultat;
    if (filtre == 1) {
        for (Media* m : tousLesMedias()) { std::string type=typeMedia(m), query=valeur; std::transform(type.begin(),type.end(),type.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));}); std::transform(query.begin(),query.end(),query.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));}); if(type.find(query)!=std::string::npos) resultat.push_back(m); }
    } else if (filtre == 2) {
        for (Media* m : tousLesMedias()) if (m->getGenre() && m->getGenre()->getLibelle()==valeur) resultat.push_back(m);
    } else if (filtre == 3 || filtre == 4) {
        int id=0; try { std::size_t pos=0; id=std::stoi(valeur,&pos); if(pos!=valeur.size()) throw std::invalid_argument("id"); } catch (...) { std::cout << "Identifiant invalide.\n"; return; }
        if (filtre==3) { if (Liste* x=trouverListe(id)) resultat=x->getMedias(); }
        else { if (Franchise* x=trouverFranchise(id)) resultat=x->getMedias(); }
    } else if (filtre == 5) {
        const bool possede=(valeur=="o"||valeur=="O"||valeur=="oui"||valeur=="1");
        for (Media* m : tousLesMedias()) if (m->getPossede()==possede) resultat.push_back(m);
    } else {
        std::string query=valeur; std::transform(query.begin(),query.end(),query.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});
        for (Media* m : tousLesMedias()) { std::string title=m->getTitre(); std::transform(title.begin(),title.end(),title.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));}); if(title.find(query)!=std::string::npos) resultat.push_back(m); }
    }
    afficherMedias(resultat);
}

