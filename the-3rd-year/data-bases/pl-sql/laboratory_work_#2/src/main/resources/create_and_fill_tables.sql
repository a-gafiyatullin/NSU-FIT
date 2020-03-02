DROP TABLE bands_authors;
DROP TABLE disks_songs;
DROP TABLE authors_songs;
DROP TABLE song;
DROP TABLE disk;
DROP TABLE band;
DROP TABLE author;
DROP TABLE occupation;

CREATE TABLE occupation (
    occupation_id INT PRIMARY KEY CHECK(occupation_id > 0),
    occupation_name VARCHAR2(255) NOT NULL
);

CREATE TABLE author (
    author_id INT PRIMARY KEY CHECK(author_id > 0),
    author_surname VARCHAR2(255),
    author_name VARCHAR2(255) NOT NULL,
    author_midname VARCHAR2(255),
    occupation_id INT NOT NULL,
    FOREIGN KEY (occupation_id) REFERENCES occupation(occupation_id)
);

CREATE TABLE band (
    band_id INT PRIMARY KEY CHECK(band_id > 0),
    band_name VARCHAR2(255) NOT NULL
);

CREATE TABLE disk (
    disk_id INT PRIMARY KEY CHECK(disk_id > 0),
    disk_name VARCHAR2(255) NOT NULL
);

CREATE TABLE song (
    song_id INT PRIMARY KEY CHECK(song_id > 0),
    song_name VARCHAR2(255) NOT NULL,
    composer_id INT NOT NULL,
    poet_id INT NOT NULL,
    FOREIGN KEY (composer_id) REFERENCES author(author_id),
    FOREIGN KEY (poet_id) REFERENCES author(author_id)
);

CREATE TABLE bands_authors (
    band_id INT NOT NULL,
    author_id INT NOT NULL,
    FOREIGN KEY (band_id) REFERENCES band(band_id),
    FOREIGN KEY (author_id) REFERENCES author(author_id),
    CONSTRAINT bands_authors_unique UNIQUE(band_id, author_id)
);

CREATE TABLE disks_songs (
    disk_id INT NOT NULL,
    song_id INT NOT NULL,
    FOREIGN KEY (disk_id) REFERENCES disk(disk_id),
    FOREIGN KEY (song_id) REFERENCES song(song_id),
    CONSTRAINT disks_songs_unique UNIQUE(disk_id, song_id)
);

CREATE TABLE authors_songs (
    song_id INT NOT NULL,
    author_id INT NOT NULL,
    FOREIGN KEY (author_id) REFERENCES author(author_id),
    FOREIGN KEY (song_id) REFERENCES song(song_id),
    CONSTRAINT authors_songs_unique UNIQUE(author_id, song_id)
);

INSERT INTO occupation VALUES(1, 'композитор');
INSERT INTO occupation VALUES(2, 'поэт');
INSERT INTO occupation VALUES(3, 'исполнитель');

INSERT INTO author VALUES(1, 'Мозжухин',    'Дмитрий',      NULL,   1);
INSERT INTO author VALUES(2, 'Псевдоним',   'Максим',       NULL,   3);
INSERT INTO author VALUES(3, 'Тимофеев',    'Александр',    NULL,   3);
INSERT INTO author VALUES(4, 'Герасименко', 'Илья',         NULL,   3);
INSERT INTO author VALUES(5, 'Дерюгина',    'Дарья',        NULL,   3);
INSERT INTO author VALUES(6, 'Андреева',    'Арина',        NULL,   3);
INSERT INTO author VALUES(7, 'Барамия',     'Илья',         NULL,   1);
INSERT INTO author VALUES(8, 'Иванов',      'Кирилл',       NULL,   1);
INSERT INTO author VALUES(9, 'Захаренко',   'Александра',   NULL,   2);
INSERT INTO author VALUES(10, 'Зайцев',     'Александр',    NULL,   1);

INSERT INTO band VALUES(1,  'Дайте танк (!)');
INSERT INTO band VALUES(2,  'Комсомольск');
INSERT INTO band VALUES(3,  'Самое большое простое число');

INSERT INTO disk VALUES(1,  'На вырост');
INSERT INTO disk VALUES(2,  'См. рис. 1');
INSERT INTO disk VALUES(3,  'Дорогие москвичи');
INSERT INTO disk VALUES(4,  'День психа');
INSERT INTO disk VALUES(5,  'Мы не спали - мы снились');

INSERT INTO song VALUES(1,  'Спам',                                 1,  2);
INSERT INTO song VALUES(2,  'Мы',                                   3,  2);
INSERT INTO song VALUES(3,  'Утро',                                 1,  3);
INSERT INTO song VALUES(4,  'Всё исчезло',                          5,  6);
INSERT INTO song VALUES(5,  'Где мы сейчас?',                       6,  6);
INSERT INTO song VALUES(6,  'Чёрный квадрат',                       5,  5);
INSERT INTO song VALUES(7,  'Мы ищём человека',                     5,  6);
INSERT INTO song VALUES(8,  'Пустая голова',                        5,  7);
INSERT INTO song VALUES(9,  'Молодость простит',                    9,  10);
INSERT INTO song VALUES(10, 'Друг',                                 8,  9);
INSERT INTO song VALUES(11, 'Это всё, что останется после меня',    2,  7);

INSERT INTO bands_authors VALUES(1, 1);
INSERT INTO bands_authors VALUES(1, 2);
INSERT INTO bands_authors VALUES(1, 3);
INSERT INTO bands_authors VALUES(2, 5);
INSERT INTO bands_authors VALUES(2, 6);
INSERT INTO bands_authors VALUES(3, 7);
INSERT INTO bands_authors VALUES(3, 8);
INSERT INTO bands_authors VALUES(3, 9);
INSERT INTO bands_authors VALUES(3, 10);

INSERT INTO disks_songs VALUES(1,   1);
INSERT INTO disks_songs VALUES(1,   2);
INSERT INTO disks_songs VALUES(1,   8);
INSERT INTO disks_songs VALUES(2,   3);
INSERT INTO disks_songs VALUES(2,   4);
INSERT INTO disks_songs VALUES(3,   4);
INSERT INTO disks_songs VALUES(3,   5);
INSERT INTO disks_songs VALUES(4,   7);
INSERT INTO disks_songs VALUES(4,   8);
INSERT INTO disks_songs VALUES(4,   10);
INSERT INTO disks_songs VALUES(4,   11);
INSERT INTO disks_songs VALUES(5,   9);
INSERT INTO disks_songs VALUES(5,   10);

INSERT INTO authors_songs VALUES(1,     1);
INSERT INTO authors_songs VALUES(1,     2);
INSERT INTO authors_songs VALUES(1,     3);
INSERT INTO authors_songs VALUES(1,     4);
INSERT INTO authors_songs VALUES(2,     1);
INSERT INTO authors_songs VALUES(2,     2);
INSERT INTO authors_songs VALUES(2,     3);
INSERT INTO authors_songs VALUES(2,     4);
INSERT INTO authors_songs VALUES(3,     1);
INSERT INTO authors_songs VALUES(3,     2);
INSERT INTO authors_songs VALUES(3,     3);
INSERT INTO authors_songs VALUES(3,     4);
INSERT INTO authors_songs VALUES(4,     5);
INSERT INTO authors_songs VALUES(4,     6);
INSERT INTO authors_songs VALUES(5,     5);
INSERT INTO authors_songs VALUES(5,     6);
INSERT INTO authors_songs VALUES(6,     5);
INSERT INTO authors_songs VALUES(6,     6);
INSERT INTO authors_songs VALUES(7,     5);
INSERT INTO authors_songs VALUES(7,     6);
INSERT INTO authors_songs VALUES(8,     5);
INSERT INTO authors_songs VALUES(8,     6);
INSERT INTO authors_songs VALUES(9,     9);
INSERT INTO authors_songs VALUES(9,     10);
INSERT INTO authors_songs VALUES(10,    8);
INSERT INTO authors_songs VALUES(10,    9);
