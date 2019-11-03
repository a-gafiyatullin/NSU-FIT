INSERT INTO university_group (group_num) VALUES (17201);
INSERT INTO university_group (group_num) VALUES (17202);
INSERT INTO university_group (group_num) VALUES (17203);
INSERT INTO university_group (group_num) VALUES (17204);
INSERT INTO university_group (group_num) VALUES (17205);
INSERT INTO university_group (group_num) VALUES (17206);
INSERT INTO university_group (group_num) VALUES (17207);
INSERT INTO university_group (group_num) VALUES (17209);
INSERT INTO university_group (group_num) VALUES (17210);

DELETE FROM university_group WHERE group_num = 17812;

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (2, 'Курбатов', 'Филипп', 'Касьянович', 17201);

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (3, 'Кузуба', 'Инна', 'Всеволодовна', 17201);
    
UPDATE university_group SET headman = 2 WHERE group_num = 17201;
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (4, 'Сёмин', 'Иван', 'Михеевич', 17202);
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (5, 'Масмех', 'Адам', 'Богданович', 17202);
    
UPDATE university_group SET headman = 4 WHERE group_num = 17202;

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (6, 'Большова', 'Христина', 'Феликсовна', 17203);
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (7, 'Злобин', 'Поликарп', 'Ипатович', 17203);
    
UPDATE university_group SET headman = 6 WHERE group_num = 17203;

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (8, 'Сизый', 'Варфоломей', 'Егорович', 17204);
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (9, 'Абрашина', 'Александра', 'Антониновна', 17204);
    
UPDATE university_group SET headman = 8 WHERE group_num = 17204;
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (10, 'Другаков', 'Чеслав', 'Эмилевич', 17205);

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (11, 'Русанова', 'Оксана', 'Владиленовна', 17205);
    
UPDATE university_group SET headman = 10 WHERE group_num = 17205;

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (12, 'Головченко', 'Кира', 'Ильевна', 17206);

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (13, 'Петракова', 'Агафья', 'Яновна', 17206);

UPDATE university_group SET headman = 12 WHERE group_num = 17206;

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (14, 'Панюшкина', 'Татьяна', 'Касьянович', 17207);
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (15, 'Мясникова', 'Софья', 'Федоровна', 17207);
    
UPDATE university_group SET headman = 14 WHERE group_num = 17207;
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (16, 'Стрельникова', 'Вероника', 'Игнатиевна', 17208);

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (17, 'Ратников', 'Петр', 'Глебович', 17209);
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (18, 'Якшибаева', 'Римма', 'Несторовна', 17209);
    
UPDATE university_group SET headman = 17 WHERE group_num = 17209;
    
INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (19, 'Беломестныха', 'Ульяна', 'Емельяновна', 17210);

INSERT INTO student (record, first_name, second_name, middle_name, group_num) 
    VALUES (20, 'Парамонов', 'Фадей', 'Яковович', 17210);
    
UPDATE university_group SET headman = 19 WHERE group_num = 17210;