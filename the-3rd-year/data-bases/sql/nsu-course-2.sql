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
    
CREATE TABLE position
    (id INT PRIMARY KEY,
     position_name VARCHAR(20));
     
ALTER TABLE teacher ADD CONSTRAINT fk_degree FOREIGN KEY (degree) REFERENCES degree(id);

ALTER TABLE teacher ADD CONSTRAINT fk_position FOREIGN KEY (position) REFERENCES position(id);

CREATE TABLE university_course
    (id INT PRIMARY KEY,
     name VARCHAR2(20) NOT NULL,
     lectures_hours INT NOT NULL CHECK(lectures_hours <=2 AND lectures_hours > 0),
     seminar_hours INT NOT NULL CHECK(seminar_hours <=2 AND seminar_hours > 0));
    
CREATE TABLE places
    (id INT PRIMARY KEY,
     building_adress VARCHAR2(20) NOT NULL,
     classroom_num INT NOT NULL);
    
CREATE TABLE lessons
    (group_num INT NOT NULL,
     course INT NOT NULL,
     place INT NOT NULL,
     FOREIGN KEY (group_num) REFERENCES university_group(group_num),
     FOREIGN KEY (course) REFERENCES university_course(id),
     FOREIGN KEY (place) REFERENCES places(id));
     
CREATE TABLE study_type
    (id INT PRIMARY KEY,
     name VARCHAR2(20));
    
CREATE TABLE military_duty
    (id INT PRIMARY KEY,
     type VARCHAR2(20));

ALTER TABLE student ADD study_type INT;
     
ALTER TABLE student ADD military_duty INT;

ALTER TABLE student ADD CONSTRAINT fk_study_type FOREIGN KEY (study_type) REFERENCES study_type(id);

ALTER TABLE student ADD CONSTRAINT fk_military_duty FOREIGN KEY (military_duty) REFERENCES military_duty(id);

ALTER TABLE student ADD live_in_dormitory INT CHECK(live_in_dormitory = 0 OR live_in_dormitory = 1);

ALTER TABLE student ADD home_address VARCHAR2(100);

ALTER TABLE student ADD dent FLOAT;

COMMIT;