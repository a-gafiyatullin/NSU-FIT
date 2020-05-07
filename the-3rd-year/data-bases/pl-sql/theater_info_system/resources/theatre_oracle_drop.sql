ALTER TABLE "Employee" DROP CONSTRAINT "Employee_fk0";
ALTER TABLE "Employee" DROP CONSTRAINT "Employee_fk1";
ALTER TABLE "Employee" DROP CONSTRAINT "Employee_fk2";

ALTER TABLE "Musician-Show" DROP CONSTRAINT "Musician-Show_fk0";
ALTER TABLE "Musician-Show" DROP CONSTRAINT "Musician-Show_fk1";

ALTER TABLE "Musician-Instrument" DROP CONSTRAINT "Musician-Instrument_fk0";
ALTER TABLE "Musician-Instrument" DROP CONSTRAINT "Musician-Instrument_fk1";

ALTER TABLE "Actor-Characteristic" DROP CONSTRAINT "Actor-Characteristic_fk0";
ALTER TABLE "Actor-Characteristic" DROP CONSTRAINT "Actor-Characteristic_fk1";

ALTER TABLE "Actor-Rank" DROP CONSTRAINT "Actor-Rank_fk0";
ALTER TABLE "Actor-Rank" DROP CONSTRAINT "Actor-Rank_fk1";
ALTER TABLE "Actor-Rank" DROP CONSTRAINT "Actor-Rank_fk2";

ALTER TABLE "Show" DROP CONSTRAINT "Show_fk0";
ALTER TABLE "Show" DROP CONSTRAINT "Show_fk1";
ALTER TABLE "Show" DROP CONSTRAINT "Show_fk2";
ALTER TABLE "Show" DROP CONSTRAINT "Show_fk3";
ALTER TABLE "Show" DROP CONSTRAINT "Show_fk4";
ALTER TABLE "Show" DROP CONSTRAINT "Show_fk5";

ALTER TABLE "Author" DROP CONSTRAINT "Author_fk0";

ALTER TABLE "Ticket" DROP CONSTRAINT "Ticket_fk0";

ALTER TABLE "Repertoire" DROP CONSTRAINT "Repertoire_fk0";

ALTER TABLE "Direction" DROP CONSTRAINT "Direction_fk0";
ALTER TABLE "Direction" DROP CONSTRAINT "Direction_fk1";

ALTER TABLE "Tour" DROP CONSTRAINT "Tour_fk0";
ALTER TABLE "Tour" DROP CONSTRAINT "Tour_fk1";

ALTER TABLE "Role-Characteristic" DROP CONSTRAINT "Role-Characteristic_fk0";
ALTER TABLE "Role-Characteristic" DROP CONSTRAINT "Role-Characteristic_fk1";

ALTER TABLE "Role" DROP CONSTRAINT "Role_fk0";

ALTER TABLE "Subscription" DROP CONSTRAINT "Subscription_fk0";
ALTER TABLE "Subscription" DROP CONSTRAINT "Subscription_fk1";

ALTER TABLE "Ticket-Subscription" DROP CONSTRAINT "Ticket-Subscription_fk0";
ALTER TABLE "Ticket-Subscription" DROP CONSTRAINT "Ticket-Subscription_fk1";

DROP TABLE "Rank";
DROP TABLE "Musical_instruments";
DROP TABLE "Musician-Instrument";
DROP TABLE "Employee";
DROP TABLE "Musician-Show";
DROP TABLE "Gender";
DROP TABLE "Actor-Characteristic";
DROP TABLE "Competition";
DROP TABLE "Actor-Rank";
DROP TABLE "Characteristic";
DROP TABLE "Education";
DROP TABLE "Show";
DROP TABLE "Author";
DROP TABLE "Country";
DROP TABLE "Genre";
DROP TABLE "Age_category";
DROP TABLE "Ticket";
DROP TABLE "Repertoire";
DROP TABLE "Direction";
DROP TABLE "Tour";
DROP TABLE "Job_types";
DROP TABLE "Role-Characteristic";
DROP TABLE "Role";
DROP TABLE "Ticket-Subscription";
DROP TABLE "Subscription";

DROP sequence "RANK_ID_RANK_SEQ";
DROP sequence "EMPLOYEE_ID_EMPLOYEE_SEQ";
DROP sequence "GENDER_ID_GENDER_SEQ";
DROP sequence "COMPETITION_ID_COMPETITON_SEQ";
DROP sequence "CHARACTERISTIC_ID_CHARACTERISTIC_SEQ";
DROP sequence "EDUCATION_ID_EDUCATION_SEQ";
DROP sequence "SHOW_ID_SHOW_SEQ";
DROP sequence "AUTHOR_ID_AUTHOR_SEQ";
DROP sequence "COUNTRY_ID_COUNTRY_SEQ";
DROP sequence "GENRE_ID_GENRE_SEQ";
DROP sequence "AGE_CATEGORY_ID_AGE_CATEGORY_SEQ";
DROP sequence "TICKET_ID_TICKET_SEQ";
DROP sequence "REPERTOIRE_ID_PERFORMANCE_SEQ";
DROP sequence "JOB_TYPES_ID_JOB_TYPE_SEQ";
DROP sequence "ROLE_ID_ROLE_SEQ";
DROP sequence "SUBSCRIPTION_ID_SUBSCRIPTION_SEQ";
DROP sequence "MUSICAL_INSTRUMENTS_ID_INSTRUMENT_SEQ";

COMMIT;