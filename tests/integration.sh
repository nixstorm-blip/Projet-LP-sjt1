#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
mkdir -p "$tmp"

# Le test doit partir d'un état connu, indépendamment des données personnelles
# présentes dans le fichier JSON de l'utilisateur.
cat > "$tmp/mediatheque.json" <<'JSON'
{
  "version": 1,
  "personnes": [],
  "formats": [],
  "genres": [],
  "listes": [],
  "franchises": [],
  "bibliotheques": [],
  "medias": []
}
JSON

# Crée les objets, un film, ses liens multiples, sauvegarde puis quitte.
printf '%s\n' \
  6 "Nolan" "Christopher" "Britannique" \
  7 "Favoris" "Mes films préférés" \
  8 "Inception" "Univers du film" \
  9 "Science-fiction" \
  10 "Blu-ray" \
  11 "Collection principale" "Bibliothèque principale" \
  11 "Collection secondaire" "Films du salon" \
  2 2 "Inception" "2010" 9 "Excellent, avec un rêve" o 148 "ISAN-123" n \
  12 1 1 1 \
  12 1 1 2 \
  12 2 1 1 "Réalisateur" \
  12 3 1 1 \
  12 4 1 1 \
  12 5 1 1 \
  12 6 1 1 \
  12 7 1 "rêve" "Mot-clé avec accent" \
  2 1 "Fondation" "1951" 8 "Classique" o "Pocket" "978000000001" n \
  2 3 "Dark" "2017" 8 "À revoir" n 3 26 n \
  2 4 "Zelda" "2017" 10 "Très bon jeu" o "Nintendo" "Switch" n \
  2 5 "Discovery" "2001" 9 "Album préféré" o "Daft Punk" n \
  14 0 | (cd "$tmp" && "$root/mediatheque") > "$tmp/create.log"

python3 -m json.tool "$tmp/mediatheque.json" >/dev/null
python3 - "$tmp/mediatheque.json" <<'PY'
import json, sys
with open(sys.argv[1], encoding="utf-8") as stream:
    data = json.load(stream)
assert {item["type"] for item in data["medias"]} == {"Film", "Livre", "Serie", "JeuVideo", "Album"}
film = next(item for item in data["medias"] if item["type"] == "Film")
assert film["isan"] == "ISAN-123" and film["createurs"][0]["role"] == "Réalisateur"
assert film["tags"][0]["nom"] == "rêve"
assert film["listes"] == [1] and film["franchises"] == [1]
assert film["bibliotheques"] == [1, 2]
assert all(-1 not in media["bibliotheques"] for media in data["medias"])
assert next(item for item in data["medias"] if item["type"] == "Album")["artiste"] == "Daft Punk"
PY

# Redémarre: filtre par liste et vérifie la restauration des objets, liens et accents.
printf '%s\n' 5 3 1 1 0 | (cd "$tmp" && "$root/mediatheque") > "$tmp/reload.log"
grep -q 'Chargement JSON réussi' "$tmp/reload.log"
grep -q 'Type : Film' "$tmp/reload.log"
grep -q 'Christopher Nolan (Réalisateur)' "$tmp/reload.log"
grep -q 'Titre : Inception' "$tmp/reload.log"
python3 - "$tmp/mediatheque.json" <<'PY'
import json, sys
with open(sys.argv[1], encoding="utf-8") as stream:
  data = json.load(stream)
film = next(item for item in data["medias"] if item["type"] == "Film")
assert film["bibliotheques"] == [1, 2]
assert film["tags"] == [{"id": 1, "nom": "rêve", "description": "Mot-clé avec accent"}]
PY

# Supprime ensuite le média et vérifie que la sauvegarde ne conserve pas le lien.
printf '%s\n' 4 1 0 | (cd "$tmp" && "$root/mediatheque") > "$tmp/delete.log"
python3 -m json.tool "$tmp/mediatheque.json" >/dev/null
grep -q 'Média et ses associations supprimés' "$tmp/delete.log"
python3 - "$tmp/mediatheque.json" <<'PY'
import json, sys
with open(sys.argv[1], encoding="utf-8") as stream:
  data = json.load(stream)
assert len(data["medias"]) == 4
assert all(media["id"] != 1 for media in data["medias"])
PY

# Un fichier incomplet doit être refusé et ne jamais être écrasé à la fermeture.
printf '{"version":1}\n' > "$tmp/mediatheque.json"
cp "$tmp/mediatheque.json" "$tmp/invalid-before.json"
printf '%s\n' 0 | (cd "$tmp" && "$root/mediatheque") > "$tmp/invalid.log" 2>&1
cmp "$tmp/invalid-before.json" "$tmp/mediatheque.json"
grep -q 'Sauvegarde refusée' "$tmp/invalid.log"

# Une liste avec deux identifiants identiques ne doit pas être acceptée.
printf '{"version":1,"personnes":[],"formats":[],"genres":[],"listes":[{"id":1,"nom":"Doublon","description":""},{"id":1,"nom":"Autre","description":""}],"franchises":[],"bibliotheques":[],"medias":[]}\n' > "$tmp/mediatheque.json"
cp "$tmp/mediatheque.json" "$tmp/duplicate-before.json"
printf '%s\n' 0 | (cd "$tmp" && "$root/mediatheque") > "$tmp/duplicate.log" 2>&1
cmp "$tmp/duplicate-before.json" "$tmp/mediatheque.json"
grep -q 'Identifiant invalide ou dupliqué pour liste' "$tmp/duplicate.log"

echo "Test réussi : cinq types, associations exactes, JSON aller-retour, filtre, suppression, schéma strict et protection des fichiers invalides."