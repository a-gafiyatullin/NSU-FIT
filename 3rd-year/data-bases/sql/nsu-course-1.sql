CREATE TABLE university_group
    (group_num INT PRIMARY KEY,
    average_mark NUMBER(2, 1));
    
CREATE TABLE student
    (record INT PRIMARY KEY,
    first_name VARCHAR2(40) NOT NULL,
    second_name VARCHAR2(40),
    middle_name VARCHAR2(40),
    average_mark NUMBER(2, 1),
    group_num INT NOT NULL,
    foreign key (group_num) references university_group(group_num));

ALTER TABLE university_group ADD headman INT;

ALTER TABLE university_group ADD CONSTRAINT fk_headman FOREIGN KEY (headman) REFERENCES student(record);

INSERT INTO university_group(group_num) VALUES(17208);
INSERT INTO university_group(group_num) VALUES(17812);

INSERT INTO student(record, first_name, second_name, middle_name, average_mark, group_num)
    VALUES(1, 'Гафиятуллин', 'Альберт', 'Рамилевич', 5, 17208);
    
UPDATE student SET average_mark = 4.8 WHERE record = 1;

UPDATE university_group SET headman = 1 WHERE group_num = 17208;