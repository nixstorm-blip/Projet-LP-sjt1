#include <iostream>
#include <string>
#include <vector>
#include "Models/Users.cpp"
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
#include "Models/Parseur.cpp"

using namespace std;

int main(){

    Format* dvd = new Format(1, "DVD");
    Genre* sf = new Genre(1, "SF");
    Personne* nolan = new Personne(1, "Nolan", "Christopher");

    Films* film = new Films(1, "Inception", "2010", 9, "Excellent film", true, 148, "ISAN-1");
    film->setFormat(dvd);
    film->setGenre(sf);
    film->ajouterCreateur(nolan);
    film->ajouterTag(new Tag(1, "rêve", "film sur les rêves"));

    Mediatheque* mediatheque = new Mediatheque(1, "Ma médiathèque", "");
    mediatheque->ajouterMedia(film);
    mediatheque->ajouterMedia(new Serie(2, "Dark", "2017", 8, "A revoir", false, 3, 26));
    mediatheque->ajouterMedia(new Livre(3, "Dune", "1965", 10, "", true, "Pocket", "978-2266320481"));
    mediatheque->ajouterMedia(new Album(4, "Discovery", "2001", 7, "", true));
    mediatheque->ajouterMedia(new JeuVideo(5, "Zelda", "2017", 10, "Tres bon jeu", true, "Nintendo", "Switch"));

    Parseur::sauvegarderMedias(mediatheque->getMedias(), "medias.json");
    cout << "Sauvegarde terminee" << endl;

    return 0;
}
