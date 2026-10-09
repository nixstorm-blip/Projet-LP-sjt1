CREATE TABLE IF NOT EXISTS Users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    mdp TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Mediatheque (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    description TEXT,
    id_user INTEGER
);

CREATE TABLE IF NOT EXISTS Genre (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    libelle TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Format (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nomFormat TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Tag (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    description TEXT
);

CREATE TABLE IF NOT EXISTS Liste (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    description TEXT
);

CREATE TABLE IF NOT EXISTS Franchise (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Personne (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    prenom TEXT,
    nationalite TEXT
);

CREATE TABLE IF NOT EXISTS Media (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    titre TEXT NOT NULL,
    annee INTEGER,
    note REAL DEFAULT 0,
    commentaire TEXT,
    posseder INTEGER DEFAULT 0,
    id_genre INTEGER,
    id_tag INTEGER,
    id_format INTEGER,
    id_franchise INTEGER
);

CREATE TABLE IF NOT EXISTS Mediatheque_Media (
    id_mediatheque INTEGER,
    id_media INTEGER,
    PRIMARY KEY (id_mediatheque, id_media)
);

CREATE TABLE IF NOT EXISTS Media_Liste (
    id_media INTEGER,
    id_liste INTEGER,
    PRIMARY KEY (id_media, id_liste)
);

CREATE TABLE IF NOT EXISTS Media_Personne (
    id_media INTEGER,
    id_personne INTEGER,
    role TEXT,
    PRIMARY KEY (id_media, id_personne, role)
);

CREATE TABLE IF NOT EXISTS Livre (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    Edition TEXT,
    ISBN TEXT,
    id_media INTEGER
);

CREATE TABLE IF NOT EXISTS Film (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    duree INTEGER,
    ISAN TEXT,
    id_media INTEGER
);

CREATE TABLE IF NOT EXISTS Serie (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT,
    nb_saison INTEGER,
    nb_episode INTEGER,
    id_media INTEGER
);

CREATE TABLE IF NOT EXISTS Jeu_video (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    editeur TEXT,
    plateforme TEXT,
    id_media INTEGER
);

CREATE TABLE IF NOT EXISTS Album (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    artiste TEXT,
    id_media INTEGER
);
