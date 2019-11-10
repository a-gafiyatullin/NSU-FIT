CREATE TABLE teacher
    (id INT PRIMARY KEY,
     first_name VARCHAR2(40) NOT NULL,
     second_name VARCHAR2(40),
     middle_name VARCHAR2(40),
     degree INT,
     position INT NOT NULL,
     home_address VARCHAR2(100) NOT NULL,
     SSN INT CHECK(SSN > 100000 AND SSN < 999999));
    
CREATE TABLE degree
    (id INT PRIMARY KEY,
     degree_name VARCHAR(10));
     
INSERT INTO degree(id, degree_name) VALUES (1, 'MSc');
INSERT INTO degree(id, degree_name) VALUES (2, 'PhD');
INSERT INTO degree(id, degree_name) VALUES (3, 'BSC');
    
CREATE TABLE position
    (id INT PRIMARY KEY,
     position_name VARCHAR(20));
     
INSERT INTO position(id, position_name) VALUES (1, 'lector');
INSERT INTO position(id, position_name) VALUES (2, 'seminarian');
INSERT INTO position(id, position_name) VALUES (3, 'assistant');
     
ALTER TABLE teacher ADD CONSTRAINT fk_degree FOREIGN KEY (degree) REFERENCES degree(id);

ALTER TABLE teacher ADD CONSTRAINT fk_position FOREIGN KEY (position) REFERENCES position(id);

ALTER TABLE teacher MODIFY SSN INT UNIQUE CHECK(SSN > 100000 AND SSN < 999999);  

INSERT INTO teacher(id, first_name, second_name, middle_name, degree, position, home_address, SSN) 
    VALUES (1, 'Пряхин', 'Андриян', 'Ираклиевич', 2, 1, 'Академгородок', 100001);

INSERT INTO teacher(id, first_name, second_name, middle_name, degree, position, home_address, SSN) 
    VALUES (2, 'Катин', 'Тарас', 'Самуилович', 1, 2, 'Нижняя Ельцовка', 100002);

INSERT INTO teacher(id, first_name, second_name, middle_name, degree, position, home_address, SSN) 
    VALUES (3, 'Кушнарёв', 'Георгий', 'Леонович', 3, 3, 'Новосибирск', 100003);
    
INSERT INTO teacher(id, first_name, second_name, middle_name, degree, position, home_address, SSN) 
    VALUES (4, 'Маркелова', 'Нона', 'Данилевна', 2, 1, 'Академгородок, Пирогова 18, 125', 100004);

INSERT INTO teacher(id, first_name, second_name, middle_name, degree, position, home_address, SSN) 
    VALUES (5, 'Ткач', 'Дементий', 'Евстафиевич', 1, 2, 'Нижняя Ельцовка, дом 2', 100005);

CREATE TABLE university_course
    (id INT PRIMARY KEY,
     name VARCHAR2(20) NOT NULL,
     lectures_hours INT NOT NULL CHECK(lectures_hours <=2 AND lectures_hours > 0),
     seminar_hours INT NOT NULL CHECK(seminar_hours <=2 AND seminar_hours > 0));
     
ALTER TABLE university_course MODIFY lectures_hours NUMBER(2, 1) CHECK(lectures_hours <=2 AND lectures_hours > 0);

ALTER TABLE university_course MODIFY seminar_hours NUMBER(2, 1) CHECK(seminar_hours <=2 AND seminar_hours > 0);
    
INSERT INTO university_course (id, name, lectures_hours, seminar_hours) VALUES (1, 'Data Bases', 1.5, 1.5);

INSERT INTO university_course (id, name, lectures_hours, seminar_hours) VALUES (2, 'Algebra', 1.5, 1.5);

INSERT INTO university_course (id, name, lectures_hours, seminar_hours) VALUES (3, 'C++ programming', 1.5, 1.5);

INSERT INTO university_course (id, name, lectures_hours, seminar_hours) VALUES (4, 'C programming', 2, 2);

INSERT INTO university_course (id, name, lectures_hours, seminar_hours) VALUES (5, 'Java programming', 2, 2);

CREATE TABLE places
    (id INT PRIMARY KEY,
     building_adress VARCHAR2(20) NOT NULL,
     classroom_num INT NOT NULL);
    
INSERT INTO places (id, building_adress, classroom_num) VALUES (1, 'Main building', 222);

INSERT INTO places (id, building_adress, classroom_num) VALUES (2, 'New building', 333);

INSERT INTO places (id, building_adress, classroom_num) VALUES (3, 'Sports complex', 1);

CREATE TABLE lessons
    (group_num INT NOT NULL,
     course INT NOT NULL,
     place INT NOT NULL,
     FOREIGN KEY (group_num) REFERENCES university_group(group_num),
     FOREIGN KEY (course) REFERENCES university_course(id),
     FOREIGN KEY (place) REFERENCES places(id));
     
INSERT INTO lessons(group_num, course, place) VALUES (17201, 1, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17202, 1, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17203, 1, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17204, 1, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17205, 1, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17206, 1, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17207, 1, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17208, 1, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17209, 1, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17210, 1, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17201, 3, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17202, 3, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17203, 3, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17204, 3, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17205, 3, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17206, 3, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17207, 3, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17208, 3, 2);

INSERT INTO lessons(group_num, course, place) VALUES (17209, 3, 1);

INSERT INTO lessons(group_num, course, place) VALUES (17210, 3, 2);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17201, 2, 1, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17202, 2, 2, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17203, 2, 1, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17204, 2, 2, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17205, 2, 1, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17206, 2, 2, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17207, 2, 1, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17208, 2, 2, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17209, 2, 1, 1);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17210, 2, 2, 1);

UPDATE lessons SET lesson_type = 2 WHERE course = 2;

ALTER TABLE lessons ADD teacher INT;

ALTER TABLE lessons ADD CONSTRAINT fk_teacher FOREIGN KEY (teacher) REFERENCES teacher(id);

UPDATE lessons SET teacher = 1 WHERE course = 1 OR course = 3;

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17201, 4, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17202, 4, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17203, 4, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17204, 4, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17205, 4, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17206, 4, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17207, 4, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17208, 4, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17209, 4, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17210, 4, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17201, 5, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17202, 5, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17203, 5, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17204, 5, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17205, 5, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17206, 5, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17207, 5, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17208, 5, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17209, 5, 1, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17210, 5, 2, 4);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17201, 5, 1, 2);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17202, 5, 2, 2);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17201, 4, 1, 5);

INSERT INTO lessons(group_num, course, place, teacher) VALUES (17204, 3, 2, 3);

CREATE TABLE lesson_type
    (id INT PRIMARY KEY,
     type_name VARCHAR2(20));
         
ALTER TABLE lessons ADD lesson_type INT;

ALTER TABLE lessons ADD CONSTRAINT fk_lesson_type FOREIGN KEY (lesson_type) REFERENCES lesson_type(id);

INSERT INTO lesson_type(id, type_name) VALUES (1, 'seminar');

INSERT INTO lesson_type(id, type_name) VALUES (2, 'lecture');

UPDATE lessons SET lesson_type = 2 WHERE course = 1 OR course = 3;

UPDATE lessons SET lesson_type = 2 WHERE teacher = 4;

UPDATE lessons SET lesson_type = 1 WHERE teacher = 2 OR teacher = 5;

CREATE TABLE study_type
    (id INT PRIMARY KEY,
     name VARCHAR2(20));
     
INSERT INTO study_type(id, name) VALUES (1, 'Бюджет');
 
INSERT INTO study_type(id, name) VALUES (2, 'Коммерция'); 
   
CREATE TABLE military_duty
    (id INT PRIMARY KEY,
     type VARCHAR2(20));
     
INSERT INTO military_duty(id, type) VALUES (1, 'Освободжен');

INSERT INTO military_duty(id, type) VALUES (2, 'Воен. каф.');

INSERT INTO military_duty(id, type) VALUES (3, 'Отслужил');

ALTER TABLE student ADD study_type INT;
     
ALTER TABLE student ADD military_duty INT;

ALTER TABLE student ADD CONSTRAINT fk_study_type FOREIGN KEY (study_type) REFERENCES study_type(id);

ALTER TABLE student ADD CONSTRAINT fk_military_duty FOREIGN KEY (military_duty) REFERENCES military_duty(id);

ALTER TABLE student ADD live_in_dormitory INT CHECK(live_in_dormitory = 0 OR live_in_dormitory = 1);

ALTER TABLE student ADD home_address VARCHAR2(100);

ALTER TABLE student ADD dent FLOAT;

UPDATE student SET live_in_dormitory = 0, study_type = 1, military_duty = 2 WHERE record <= 10;

UPDATE student SET live_in_dormitory = 1, study_type = 2, military_duty = 1 WHERE record > 10 AND record <= 15;

UPDATE student SET live_in_dormitory = 0, study_type = 2, military_duty = 3 WHERE record > 15;

CREATE TABLE exam_status
    (id INT PRIMARY KEY,
     status_name VARCHAR2(20));

INSERT INTO exam_status(id, status_name) VALUES (1, 'Сдано');

INSERT INTO exam_status(id, status_name) VALUES (2, 'Перенесено');

INSERT INTO exam_status(id, status_name) VALUES (3, 'Пересдача');

CREATE TABLE exams
    (student INT NOT NULL,
     course INT NOT NULL,
     teacher INT NOT NULL,
     status INT NOT NULL,
     grade INT NOT NULL CHECK(grade >= 2 AND grade <= 5),
     retake INT CHECK(retake >= 0),
     FOREIGN KEY (student) REFERENCES student(record),
     FOREIGN KEY (course) REFERENCES university_course(id),
     FOREIGN KEY (teacher) REFERENCES teacher(id),
     FOREIGN KEY (status) REFERENCES exam_status(id),
     PRIMARY KEY (student, course));

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (1, 1, 1, 1, 5, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (2, 1, 1, 1, 4, 1);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (3, 1, 1, 1, 5, 2);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (4, 2, 3, 2, 2, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (4, 1, 4, 3, 2, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (4, 3, 5, 3, 2, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (5, 1, 3, 2, 2, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (5, 2, 4, 3, 2, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (5, 3, 5, 3, 2, 0);

INSERT INTO exams (student, course, teacher, status, grade, retake) VALUES (6, 3, 5, 3, 2, 0);
---------------------------------------------------------№1---------------------------------------------------------
SELECT first_name, second_name, middle_name 
FROM teacher 
WHERE home_address LIKE '%Нижняя Ельцовка%' OR home_address LIKE '%Академгородок%';    
---------------------------------------------------------№2---------------------------------------------------------
SELECT (SELECT first_name || ' ' || second_name || ' ' || middle_name 
        FROM teacher 
        WHERE id = teacher), SUM(hours) AS hours
FROM (SELECT teacher, (SELECT lectures_hours 
                       FROM university_course 
                       WHERE id = course) AS hours 
      FROM (lessons) 
      WHERE lesson_type = 2)
GROUP BY teacher HAVING SUM(hours) > 45;
---------------------------------------------------------№3---------------------------------------------------------
SELECT * 
FROM (SELECT group_num, SUM(hours) AS hours
      FROM (SELECT group_num, (SELECT seminar_hours 
                               FROM university_course 
                               WHERE id = course) AS hours 
            FROM (lessons) WHERE lesson_type = 1)
     GROUP BY group_num) 
ORDER BY hours DESC;
---------------------------------------------------------№4---------------------------------------------------------
SELECT * 
FROM (SELECT (SELECT (record || ' ' || first_name || ' ' || second_name) 
              FROM student 
              WHERE record = headman AND military_duty = 2 AND live_in_dormitory = 0) AS info
      FROM university_group)
WHERE info IS NOT NULL;
---------------------------------------------------------№5---------------------------------------------------------      
SELECT (SELECT first_name || ' ' || second_name || ' ' || middle_name 
        FROM student 
        WHERE record = student) AS student, 
       (SELECT name 
        FROM university_course 
        WHERE id = course) AS course,
       (SELECT first_name || ' ' || second_name || ' ' || middle_name 
        FROM teacher 
        WHERE id = teacher) AS teacher
FROM (SELECT student, teacher, course
      FROM exams 
      WHERE student IN (SELECT student 
                        FROM (SELECT student, course, teacher 
                              FROM exams 
                              WHERE status = 2 OR status = 3)
                        GROUP BY student HAVING COUNT(course) = 3));
                        
COMMIT;