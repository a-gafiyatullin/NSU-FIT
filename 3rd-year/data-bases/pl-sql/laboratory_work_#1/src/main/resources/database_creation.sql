DROP TABLE st_teac;
DROP TABLE student;
DROP TABLE teacher;

CREATE TABLE teacher(
    id number(3) PRIMARY KEY,
    name varchar2(30) NOT NULL,
    last_name varchar2(50) NOT NULL
    -- unique(name,last_name)
);

CREATE TABLE student(
    id number(3) PRIMARY KEY,
    name varchar2(30) NOT NULL,
    last_name varchar2(50) NOT NULL
    -- unique(name,last_name)
);

CREATE TABLE st_teac(
    id_stud number(3) NOT NULL,
    id_teac number(3) NOT NULL,
    PRIMARY KEY (id_stud, id_teac),
    FOREIGN KEY (id_stud) REFERENCES student,
    FOREIGN KEY (id_teac) REFERENCES teacher
);

INSERT INTO student values(1,   'Ivanov',      'Petr');
INSERT INTO student values(2,   'Petrov',      'Evgeniy');
INSERT INTO student values(3,   'Kuznetsov',   'Arkadiy');
INSERT INTO student values(4,   'Pyatachok',   'Swin');
INSERT INTO student values(5,   'Sharapov',    'Vladimir');

-- v teachers
INSERT INTO teacher values(1,   'Zheglov',      'Gleb');
INSERT INTO teacher values(2,   'Pukh',         'Vinnie');

-- v st_teac
INSERT INTO st_teac values(1, 2);
INSERT INTO st_teac values(2, 2);
INSERT INTO st_teac values(4, 2);
INSERT INTO st_teac values(3, 1);
INSERT INTO st_teac values(5, 1);
INSERT INTO st_teac values(2, 1);

COMMIT;