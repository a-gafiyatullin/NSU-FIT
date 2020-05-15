INSERT INTO "User_Role"
VALUES (0, 'headmaster'); /* должно выполниться */
INSERT INTO "User_Role"
VALUES (0, 'admin'); /* должно выполниться */
INSERT INTO "User_Role"
VALUES (0, 'cashier'); /* должно выполниться */
INSERT INTO "Users"
VALUES ('xp10rd', '1234', 1); /* должно выполниться */
INSERT INTO "Users"
VALUES ('admin', '1234', 2); /* должно выполниться */
INSERT INTO "Users"
VALUES ('slave', '1234', 3); /* должно выполниться */

INSERT INTO "Job_types"
VALUES (0, NULL, 'актер', 1); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, NULL, 'постановщик', 1); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, 2, 'pежиссеp-постановщик', 1); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, 2, 'художник-постановщик', 1); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, 2, 'диpижеp-постановщик', 1); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, NULL, 'музыкант', 1); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, NULL, 'уборщик', 0); /* должно выполниться */
INSERT INTO "Job_types"
VALUES (0, NULL, 'NoName', 0); /* должно выполниться */
DELETE
FROM "Job_types"
WHERE "name_job_type" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Education"
VALUES (0, 'среднее'); /* должно выполниться */
INSERT INTO "Education"
VALUES (0, 'среднее специальное'); /* должно выполниться */
INSERT INTO "Education"
VALUES (0, 'студент'); /* должно выполниться */
INSERT INTO "Education"
VALUES (0, 'высшее'); /* должно выполниться */
INSERT INTO "Education"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Education"
WHERE "name_education" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Gender"
VALUES (0, 'мужской'); /* должно выполниться */
INSERT INTO "Gender"
VALUES (0, 'женский'); /* должно выполниться */
INSERT INTO "Gender"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Gender"
WHERE "name_gender" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Rank"
VALUES (0, 'заслуженный артист'); /* должно выполниться */
INSERT INTO "Rank"
VALUES (0, 'народный артист'); /* должно выполниться */
INSERT INTO "Rank"
VALUES (0, 'лауреат'); /* должно выполниться */
INSERT INTO "Rank"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Rank"
WHERE "name_rank" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Competition"
VALUES (0, 'Голос'); /* должно выполниться */
INSERT INTO "Competition"
VALUES (0, 'Таланты России'); /* должно выполниться */
INSERT INTO "Competition"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Competition"
WHERE "name_competition" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Age_category"
VALUES (0, 'дети'); /* должно выполниться */
INSERT INTO "Age_category"
VALUES (0, 'взрослые'); /* должно выполниться */
INSERT INTO "Age_category"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Age_category"
WHERE "name_age_category" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Genre"
VALUES (0, 'музыкальная комедия'); /* должно выполниться */
INSERT INTO "Genre"
VALUES (0, 'тpагедия'); /* должно выполниться */
INSERT INTO "Genre"
VALUES (0, 'опеpетта'); /* должно выполниться */
INSERT INTO "Genre"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Genre"
WHERE "name_genre" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Country"
VALUES (0, 'Священная Римская империя'); /* должно выполниться */
INSERT INTO "Country"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Country"
WHERE "name_country" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Author"
VALUES (0, 'Людвиг', 'ван Бетховен', '', 18, 1); /* должно выполниться */
INSERT INTO "Author"
VALUES (0, 'Людвиг ван', 'Бетховен', '', 18, 1); /* должно выполниться */
INSERT INTO "Author"
VALUES (0, 'NoName', '', '', 18, 1); /* должно выполниться */
DELETE
FROM "Author"
WHERE "name_author" LIKE 'NoName'; /* должно выполниться */

DELETE
FROM "Country"
WHERE "name_country" LIKE 'Священная Римская империя'; /* не должно выполниться */

INSERT INTO "Musical_instruments"
VALUES (0, 'гитара'); /* должно выполниться */
INSERT INTO "Musical_instruments"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE
FROM "Musical_instruments"
WHERE "name_instrument" LIKE 'NoName'; /* должно выполниться */

INSERT INTO "Characteristic"
VALUES (0, 'худой'); /* должно выполниться */
INSERT INTO "Characteristic"
VALUES (0, 'высокий'); /* должно выполниться */
INSERT INTO "Characteristic"
VALUES (0, 'NoName'); /* должно выполниться */
DELETE FROM "Characteristic" WHERE "type_characteristic" LIKE 'NoName';                                 /* должно выполниться */

/* тест Employee 1------------------------ -----------------------------------------------------------------------------------*/
INSERT INTO "Employee" VALUES(0, 'Иван', '', '', 1, TO_DATE('2019/01/01', 'yyyy/mm/dd'), TO_DATE('2018/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 1);                                                                                 /* не должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Иван', '', '', 1, TO_DATE('2021/01/01', 'yyyy/mm/dd'), TO_DATE('2022/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 1);                                                                                 /* не должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Иван', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2018/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 1);                                                                                    /* должно выполниться */
UPDATE "Employee" SET "hire_date_employee" = TO_DATE('1998/01/01', 'yyyy/mm/dd')
    WHERE "id_employee" = 1;                                                                         /* не должно выполниться */
UPDATE "Employee" SET "birthday_employee" = TO_DATE('2020/01/01', 'yyyy/mm/dd')
    WHERE "id_employee" = 1;                                                                         /* не должно выполниться */
UPDATE "Employee" SET "hire_date_employee" = TO_DATE('2020/01/01', 'yyyy/mm/dd'),
    "birthday_employee" = TO_DATE('1998/01/01', 'yyyy/mm/dd') WHERE "id_employee" = 1;                  /* должно выполниться */
/*----------------------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Employee" VALUES(0, 'Оксана', '', '', 2, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 3);                                                                                    /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Алексей', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 4);                                                                                    /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Рафаэль', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 5);                                                                                    /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Марсель', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 1);                                                                                    /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Лариса', '', '', 2, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 1);                                                                                    /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'Ксения', '', '', 2, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
                              0, 30000, 4, 1); /* должно выполниться */
INSERT INTO "Employee"
VALUES (0, 'Алексей', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        0, 30000, 4, 1); /* должно выполниться */
INSERT INTO "Employee"
VALUES (0, 'NoName', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        0, 30000, 4, 1); /* должно выполниться */
INSERT INTO "Employee"
VALUES (0, 'Владимир', '', '', 2, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        0, 30000, 1, 6); /* должно выполниться */
INSERT INTO "Employee"
VALUES (0, 'Максим', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        0, 30000, 4, 6); /* должно выполниться */
INSERT INTO "Employee"
VALUES (0, 'Юрий', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        0, 30000, 1, 7); /* должно выполниться */
INSERT INTO "Employee"
VALUES (0, 'Гузель', '', '', 2, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        0, 30000, 1, 3);
/* должно выполниться */
/* тест простейших таблиц ----------------------------------------------------------------------------------------------------*/
DELETE
FROM "Job_types"
WHERE "name_job_type" LIKE 'актер'; /* не должно выполниться */

DELETE
FROM "Education"
WHERE "name_education" LIKE 'высшее'; /* не должно выполниться */

DELETE
FROM "Gender"
WHERE "name_gender" LIKE 'мужской';
/* не должно выполниться */
/* тест Actor-Rank -----------------------------------------------------------------------------------------------------------*/
INSERT INTO "Actor-Rank"
VALUES (1, 1, TO_DATE('1997/01/01', 'yyyy/mm/dd'), 2); /* не должно выполниться */
INSERT INTO "Actor-Rank"
VALUES (1, 1, TO_DATE('2020/01/01', 'yyyy/mm/dd'), 2); /* должно выполниться */
UPDATE "Actor-Rank"
SET "obtaining_date_actor_rank" = TO_DATE('1997/01/01', 'yyyy/mm/dd')
WHERE "id_actor" = 1;
/* не должно выполниться */ /* не должно выполниться */
UPDATE "Actor-Rank"
SET "obtaining_date_actor_rank" = TO_DATE('2019/01/01', 'yyyy/mm/dd')
WHERE "id_actor" = 1;
/* должно выполниться */
/* тест простейших таблиц ----------------------------------------------------------------------------------------------------*/
DELETE
FROM "Rank"
WHERE "name_rank" LIKE 'заслуженный артист'; /* не должно выполниться */

DELETE
FROM "Competition"
WHERE "name_competition" LIKE 'Таланты России';
/* не должно выполниться */
/* тест Show 1 ---------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Show"
VALUES (0, 'Творения Прометея', 2, 3, 4, 1, 1, 2, 17,
        TO_DATE('2022/01/01', 'yyyy/mm/dd')); /* не должно выполниться */
INSERT INTO "Show"
VALUES (0, 'Творения Прометея', 2, 3, 4, 1, 1, 2, 19,
        TO_DATE('2020/01/01', 'yyyy/mm/dd')); /* не должно выполниться */
INSERT INTO "Show"
VALUES (0, 'Творения Прометея', 2, 3, 4, 1, 1, 2, 19,
        TO_DATE('2022/01/01', 'yyyy/mm/dd')); /* должно выполниться */
UPDATE "Show"
SET "century_show" = 17
WHERE "id_show" = 1; /* не должно выполниться */
UPDATE "Show"
SET "premier_date_show" = TO_DATE('2020/01/01', 'yyyy/mm/dd')
WHERE "id_show" = 1; /* не должно выполниться */
UPDATE "Show"
SET "century_show"      = 18,
    "premier_date_show" = TO_DATE('2025/01/01', 'yyyy/mm/dd')
WHERE "id_show" = 1;
/* должно выполниться */
/* тест простейших таблиц ----------------------------------------------------------------------------------------------------*/
DELETE
FROM "Genre"
WHERE "name_genre" LIKE 'музыкальная комедия'; /* не должно выполниться */

DELETE
FROM "Age_category"
WHERE "name_age_category" LIKE 'взрослые'; /* не должно выполниться */

DELETE
FROM "Author"
WHERE "name_author" LIKE 'Людвиг'; /* не должно выполниться */
UPDATE "Author"
SET "life_century_author" = 21
WHERE "id_author" = 1; /* не должно выполниться */
UPDATE "Author"
SET "life_century_author" = 17
WHERE "id_author" = 1;
/* должно выполниться */
/*----------------------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Role"
VALUES (0, 1, 'Прометей', 1); /* должно выполниться */
INSERT INTO "Role"
VALUES (0, 1, 'Амфион', 0); /* должно выполниться */
INSERT INTO "Role"
VALUES (0, 1, 'None', 0); /* должно выполниться */
DELETE
FROM "Role"
WHERE "name_role" LIKE 'None'; /* должно выполниться */

INSERT INTO "Role-Characteristic"
VALUES (1, 1, 0); /* должно выполниться */
INSERT INTO "Role-Characteristic"
VALUES (2, 2, 0); /* должно выполниться */

INSERT INTO "Musician-Instrument"
VALUES (11, 1); /* должно выполниться */
INSERT INTO "Musician-Show" VALUES(11, 1);                                                              /* должно выполниться */
INSERT INTO "Musician-Show" VALUES(10, 1);                                                              /* должно выполниться */
DELETE FROM "Musician-Show" WHERE "id_musician" = 10;                                                   /* должно выполниться */

INSERT INTO "Actor-Characteristic" VALUES(1, 1, 0);                                                     /* должно выполниться */
/* тест Direction 1-----------------------------------------------------------------------------------------------------------*/
INSERT INTO "Direction" VALUES(8, 1, 1);                                                                /* должно выполниться */
INSERT INTO "Direction" VALUES(8, 2, 0);                                                             /* не должно выполниться */
INSERT INTO "Direction" VALUES(7, 2, 1);                                                                /* должно выполниться */
INSERT INTO "Direction" VALUES(6, 2, 0);                                                             /* не должно выполниться */
INSERT INTO "Direction" VALUES(1, 1, 1);                                                             /* не должно выполниться */
INSERT INTO "Direction" VALUES(1, 1, 0);                                                                /* должно выполниться */
INSERT INTO "Direction" VALUES(5, 1, 1);                                                             /* не должно выполниться */
INSERT INTO "Direction" VALUES(5, 1, 0);                                                             /* не должно выполниться */
/* тест простейших таблиц ----------------------------------------------------------------------------------------------------*/
DELETE FROM "Role" WHERE "name_role" LIKE 'Прометей';                                                /* не должно выполниться */

INSERT INTO "Role-Characteristic" VALUES(2, 1, 0);                                                   /* не должно выполниться */
UPDATE "Role-Characteristic" SET "id_characteristic" = 2 WHERE "id_role" = 1;                        /* не должно выполниться */
DELETE FROM "Role-Characteristic" WHERE "id_role" = 1;                                               /* не должно выполниться */

DELETE FROM "Characteristic" WHERE "type_characteristic" LIKE 'худой';                               /* не должно выполниться */
DELETE FROM "Musical_instruments" WHERE "name_instrument" LIKE 'гитара';                             /* не должно выполниться */
/*тест Employee 2-------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Employee" VALUES(0, 'NoName2', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2018/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 1);                                                                                    /* должно выполниться */
DELETE FROM "Employee" WHERE "name_employee" LIKE 'NoName2';                                            /* должно выполниться */
DELETE FROM "Employee" WHERE "id_employee" = 1;                                                      /* не должно выполниться */
DELETE FROM "Employee" WHERE "id_employee" = 2;                                                      /* не должно выполниться */
DELETE FROM "Employee" WHERE "id_employee" = 3;                                                      /* не должно выполниться */
DELETE FROM "Employee" WHERE "id_employee" = 4;                                                      /* не должно выполниться */
DELETE FROM "Employee" WHERE "id_employee" = 11;                                                     /* не должно выполниться */
INSERT INTO "Employee" VALUES(0, 'NoName2', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2018/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 3);                                                                                    /* должно выполниться */
DELETE FROM "Employee" WHERE "name_employee" LIKE 'NoName2';                                            /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'NoName2', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2018/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 4);                                                                                    /* должно выполниться */
DELETE FROM "Employee" WHERE "name_employee" LIKE 'NoName2';                                            /* должно выполниться */
INSERT INTO "Employee" VALUES(0, 'NoName2', '', '', 1, TO_DATE('1999/01/01', 'yyyy/mm/dd'), TO_DATE('2018/01/01', 'yyyy/mm/dd'),
    0, 30000, 4, 5);                                                                                    /* должно выполниться */
DELETE FROM "Employee" WHERE "name_employee" LIKE 'NoName2';                                            /* должно выполниться */
/*----------------------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Show" VALUES(0, 'Леонора', 2, 3, 4, 2, 2, 2, 19, TO_DATE('2022/01/01', 'yyyy/mm/dd'));     /* должно выполниться */
INSERT INTO "Role" VALUES(0, 2, 'Леонора', 1);                                                          /* должно выполниться */ 
INSERT INTO "Role" VALUES(0, 2, 'Пицарро', 0);                                                          /* должно выполниться */
INSERT INTO "Musician-Show" VALUES(11, 2);                                                              /* должно выполниться */  
/* тест Repertoire 1----------------------------------------------------------------------------------------------------------*/
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2023/06/01', 'yyyy/mm/dd'));                          /* не должно выполниться */
INSERT INTO "Direction" VALUES(8, 4, 1);                                                                /* должно выполниться */ 
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2023/06/01', 'yyyy/mm/dd'));                          /* не должно выполниться */
INSERT INTO "Direction" VALUES(6, 4, 0);                                                                /* должно выполниться */
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2023/06/01', 'yyyy/mm/dd'));                          /* не должно выполниться */
INSERT INTO "Direction" VALUES(7, 5, 1);                                                                /* должно выполниться */ 
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2018/01/01', 'yyyy/mm/dd'));                          /* не должно выполниться */
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2022/01/03', 'yyyy/mm/dd'));                             /* должно выполниться */
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2022/01/03', 'yyyy/mm/dd'));                          /* не должно выполниться */
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2023/01/01', 'yyyy/mm/dd'));                             /* должно выполниться */
INSERT INTO "Repertoire" VALUES(0, 2, TO_DATE('2024/01/01', 'yyyy/mm/dd'));                             /* должно выполниться */
DELETE FROM "Repertoire" WHERE "id_performance" = 3;                                                    /* должно выполниться */
/* тест Ticket ---------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Ticket" VALUES(0, 1, 10, 100, 0, NULL);                                                    /* должно выполниться */
INSERT INTO "Ticket" VALUES(0, 1, 10, 100, 0, NULL);                                                 /* не должно выполниться */
INSERT INTO "Ticket" VALUES(0, 1, 11, 100, 0, NULL);                                                    /* должно выполниться */
/* тест Repertoire 2----------------------------------------------------------------------------------------------------------*/
DELETE FROM "Repertoire" WHERE "id_performance" = 1;                                                 /* не должно выполниться */
/* тест Show 2----------------------------------------------------------------------------------------------------------------*/
UPDATE "Show" SET "premier_date_show" = TO_DATE('2030/01/01', 'yyyy/mm/dd') WHERE "id_show" = 2;     /* не должно выполниться */
UPDATE "Show" SET "premier_date_show" = TO_DATE('2005/01/01', 'yyyy/mm/dd') WHERE "id_show" = 2;     /* не должно выполниться */
DELETE FROM "Show" WHERE "id_show" = 2;                                                              /* не должно выполниться */
/*----------------------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Show" VALUES(0, 'NoName', 2, 3, 4, 1, 1, 2, 19, TO_DATE('2022/01/01', 'yyyy/mm/dd'));      /* должно выполниться */
INSERT INTO "Role" VALUES(0, 3, 'NoName', 1);                                                           /* должно выполниться */
INSERT INTO "Direction" VALUES(8, 6, 1);                                                                /* должно выполниться */
INSERT INTO "Direction" VALUES(9, 6, 0);                                                                /* должно выполниться */
/* тест Direction 2-----------------------------------------------------------------------------------------------------------*/
DELETE FROM "Direction" WHERE "id_actor" = 9 and "id_role" = 6;                                         /* должно выполниться */
INSERT INTO "Direction" VALUES(9, 6, 0);                                                                /* должно выполниться */
INSERT INTO "Repertoire" VALUES(0, 3, TO_DATE('2024/01/01', 'yyyy/mm/dd'));                             /* должно выполниться */
UPDATE "Show" SET "century_show" = 21 WHERE "id_show" = 3;                                           /* не должно выполниться */
INSERT INTO "Role" VALUES(0, 3, 'NoName2', 1);                                                          /* должно выполниться */
INSERT INTO "Direction" VALUES(1, 7, 1);                                                             /* не должно выполниться */
DELETE FROM "Direction" WHERE "id_actor" = 9 and "id_role" = 6;                                      /* не должно выполниться */
/* тест Tour -----------------------------------------------------------------------------------------------------------------*/
INSERT INTO "Tour"
VALUES (12, 1, TO_DATE('2021/01/01', 'yyyy/mm/dd'),
        TO_DATE('2022/01/01', 'yyyy/mm/dd'), 1); /* не должно выполниться */
INSERT INTO "Tour"
VALUES (1, 2, TO_DATE('2021/01/01', 'yyyy/mm/dd'),
        TO_DATE('2022/01/01', 'yyyy/mm/dd'), 1); /* не должно выполниться */
INSERT INTO "Tour"
VALUES (1, 1, TO_DATE('2023/01/01', 'yyyy/mm/dd'),
        TO_DATE('2021/01/01', 'yyyy/mm/dd'), 1); /* не должно выполниться */
INSERT INTO "Tour"
VALUES (13, 1, TO_DATE('2021/01/01', 'yyyy/mm/dd'),
        TO_DATE('2022/01/01', 'yyyy/mm/dd'), 1); /* не должно выполниться */
INSERT INTO "Tour"
VALUES (1, 1, TO_DATE('2020/01/01', 'yyyy/mm/dd'),
        TO_DATE('2021/01/01', 'yyyy/mm/dd'), 1); /* не должно выполниться */
INSERT INTO "Tour"
VALUES (1, 1, TO_DATE('2022/01/01', 'yyyy/mm/dd'),
        TO_DATE('2023/01/01', 'yyyy/mm/dd'), 1); /* должно выполниться */
INSERT INTO "Tour"
VALUES (2, 1, TO_DATE('2022/01/01', 'yyyy/mm/dd'),
        TO_DATE('2023/01/01', 'yyyy/mm/dd'), 1); /* должно выполниться */
INSERT INTO "Tour"
VALUES (3, 1, TO_DATE('2022/01/01', 'yyyy/mm/dd'),
        TO_DATE('2023/01/01', 'yyyy/mm/dd'), 1); /* должно выполниться */
INSERT INTO "Tour"
VALUES (4, 1, TO_DATE('2022/01/01', 'yyyy/mm/dd'),
        TO_DATE('2023/01/01', 'yyyy/mm/dd'), 1); /* должно выполниться */
INSERT INTO "Tour"
VALUES (2, 2, TO_DATE('2022/03/01', 'yyyy/mm/dd'),
        TO_DATE('2024/01/01', 'yyyy/mm/dd'), 1); /* не должно выполниться */
INSERT INTO "Tour"
VALUES (2, 2, TO_DATE('2023/03/01', 'yyyy/mm/dd'),
        TO_DATE('2024/01/01', 'yyyy/mm/dd'), 1);
/* должно выполниться */
/* тест Subscription ---------------------------------------------------------------------------------------------------------*/
INSERT INTO "Subscription"
VALUES (0, 1, 1); /* не должно выполниться */
INSERT INTO "Subscription"
VALUES (0, 1, NULL); /* должно выполниться */
INSERT INTO "Subscription"
VALUES (0, NULL, 1); /* должно выполниться */
UPDATE "Subscription"
SET "id_genre" = 1
WHERE "id_subscription" = 2; /* не должно выполниться */
UPDATE "Subscription"
SET "id_author" = 1
WHERE "id_subscription" = 1; /* не должно выполниться */
UPDATE "Subscription"
SET "id_genre" = 2
WHERE "id_subscription" = 1; /* должно выполниться */
UPDATE "Subscription"
SET "id_author" = 2
WHERE "id_subscription" = 2; /* должно выполниться */
INSERT INTO "Subscription"
VALUES (0, NULL, 1);
/* должно выполниться */
/* тест Ticket-Subscription --------------------------------------------------------------------------------------------------*/
INSERT INTO "Ticket-Subscription"
VALUES (1, 1); /* должно выполниться */
INSERT INTO "Ticket-Subscription"
VALUES (1, 2); /* не должно выполниться */
INSERT INTO "Ticket-Subscription"
VALUES (2, 3);
/* не должно выполниться */
/* тест Musician-Show --------------------------------------------------------------------------------------------------------*/
INSERT INTO "Musician-Show"
VALUES (10, 2); /* не должно выполниться */
DELETE
FROM "Musician-Show"
WHERE "id_musician" = 11;
/* не должно выполниться */
/*----------------------------------------------------------------------------------------------------------------------------*/
COMMIT;