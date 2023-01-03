/* select all record from the table */
SELECT * FROM student;

/* select all record from the table, but show only first_name and group_num fields */
SELECT first_name, group_num FROM student;

/* select students from the group #17208 */
SELECT record, first_name, group_num FROM student WHERE group_num = 17208;

SELECT * FROM student WHERE group_num > 17205 AND record < 15;

SELECT record, second_name FROM student;

UPDATE student SET second_name = 'Иван' WHERE record = 10;

UPDATE student SET second_name = 'Иван' WHERE record = 7;

UPDATE student SET second_name = 'Иван' WHERE record = 5;

/* show all second_name from the student table without duplicates */
SELECT DISTINCT second_name FROM student;

/* select all records from the student table and sort them by first_name in the alphabetic order */
SELECT * FROM student ORDER BY first_name;

/* select all records from the student table and sort them by first_name in the reverse alphabetic order */
SELECT * FROM student ORDER BY first_name DESC;

SELECT * FROM student WHERE record >= 5 AND record <= 10;

/* equivalent for the previous query */
SELECT * FROM student WHERE record BETWEEN 5 AND 10;

/* select records with value 1, 3, 5 in the primary key */
SELECT * FROM student WHERE record IN(1, 3, 5);

/* select records that are started from the 'Г' character */
SELECT * FROM student WHERE first_name LIKE 'Г%';

COMMIT;
