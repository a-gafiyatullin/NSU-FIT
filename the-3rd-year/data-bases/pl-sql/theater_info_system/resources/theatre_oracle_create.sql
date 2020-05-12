CREATE TABLE "Rank" (
	"id_rank" INT PRIMARY KEY,
	"name_rank" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "RANK_ID_RANK_SEQ";

CREATE trigger "BI_RANK_ID_RANK"
    before insert on "Rank"
    for each row
begin
    select "RANK_ID_RANK_SEQ".nextval into :NEW."id_rank" from dual;
end;
/

CREATE TABLE "Employee" (
	"id_employee" INT PRIMARY KEY,
	"name_employee" VARCHAR2(255) NOT NULL,
	"surname_employee" VARCHAR2(255),
	"middle_name_employee" VARCHAR2(255),
	"id_gender" INT NOT NULL,
	"birthday_employee" DATE NOT NULL,
	"hire_date_employee" DATE NOT NULL,
	"children_amount_employee" INT DEFAULT 0 CHECK("children_amount_employee" >= 0),
	"salary_employee" NUMERIC(*, 2) NOT NULL CHECK("salary_employee" > 0),
	"id_education" INT NOT NULL,
	"id_job_type" INT NOT NULL);

CREATE sequence "EMPLOYEE_ID_EMPLOYEE_SEQ";
/

CREATE TABLE "Musician-Show" (
    "id_musician" INT NOT NULL,
    "id_show" INT NOT NULL,
    constraint MUSICIAN_SHOW_PK PRIMARY KEY ("id_musician","id_show"));
/

CREATE TABLE "Gender" (
	"id_gender" INT PRIMARY KEY,
	"name_gender" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "GENDER_ID_GENDER_SEQ";

CREATE trigger "BI_GENDER_ID_GENDER"
    before insert
    on "Gender"
    for each row
begin
    select "GENDER_ID_GENDER_SEQ".nextval into :NEW."id_gender" from dual;
end;
/

CREATE TABLE "Actor-Characteristic"
(
    "id_actor"                   INT NOT NULL,
    "id_characteristic"          INT NOT NULL,
    "value_actor_characteristic" NUMERIC(*, 2),
    constraint ACTOR_CHARACTERISTIC_PK PRIMARY KEY ("id_actor", "id_characteristic")
);
/

CREATE TABLE "Competition"
(
    "id_competition"   INT PRIMARY KEY,
    "name_competition" VARCHAR2(255) UNIQUE NOT NULL
);

CREATE sequence "COMPETITION_ID_COMPETITON_SEQ";

CREATE trigger "BI_COMPETITION_ID_COMPETITON"
    before insert
    on "Competition"
    for each row
begin
    select "COMPETITION_ID_COMPETITON_SEQ".nextval into :NEW."id_competition" from dual;
end;
/

CREATE TABLE "Actor-Rank" (
	"id_actor" INT NOT NULL,
	"id_rank" INT NOT NULL,
	"obtaining_date_actor_rank" DATE NOT NULL,
	"id_competition" INT,
	constraint ACTOR_RANK_PK PRIMARY KEY ("id_actor","id_rank"));
/

CREATE TABLE "Characteristic" (
	"id_characteristic" INT PRIMARY KEY,
	"type_characteristic" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "CHARACTERISTIC_ID_CHARACTERISTIC_SEQ";

CREATE trigger "BI_CHARACTERISTIC_ID_CHARACTERISTIC"
    before insert on "Characteristic"
    for each row
begin
    select "CHARACTERISTIC_ID_CHARACTERISTIC_SEQ".nextval into :NEW."id_characteristic" from dual;
end;
/

CREATE TABLE "Education" (
	"id_education" INT PRIMARY KEY,
	"name_education" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "EDUCATION_ID_EDUCATION_SEQ";

CREATE trigger "BI_EDUCATION_ID_EDUCATION"
    before insert on "Education"
    for each row
begin
    select "EDUCATION_ID_EDUCATION_SEQ".nextval into :NEW."id_education" from dual;
end;
/

CREATE TABLE "Show" (
	"id_show" INT PRIMARY KEY,
	"name_show" VARCHAR2(255) UNIQUE NOT NULL,
	"id_director" INT NOT NULL,
	"id_production_designer" INT NOT NULL,
	"id_conductor" INT NOT NULL,
	"id_author" INT NOT NULL,
	"id_genre" INT NOT NULL,
	"id_age_category" INT NOT NULL,
	"century_show" INT NOT NULL,
	"premier_date_show" DATE NOT NULL);

CREATE sequence "SHOW_ID_SHOW_SEQ";
/

CREATE TABLE "Author" (
	"id_author" INT PRIMARY KEY,
	"name_author" VARCHAR2(255) NOT NULL,
	"surname_author" VARCHAR2(255),
	"middle_name_author" VARCHAR2(255),
	"life_century_author" INT NOT NULL,
	"id_country" INT NOT NULL);

CREATE sequence "AUTHOR_ID_AUTHOR_SEQ";

CREATE trigger "BI_AUTHOR_ID_AUTHOR"
    before insert on "Author"
    for each row
begin
    select "AUTHOR_ID_AUTHOR_SEQ".nextval into :NEW."id_author" from dual;
end;
/

CREATE TABLE "Country" (
	"id_country" INT PRIMARY KEY,
	"name_country" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "COUNTRY_ID_COUNTRY_SEQ";

CREATE trigger "BI_COUNTRY_ID_COUNTRY"
    before insert on "Country"
    for each row
begin
    select "COUNTRY_ID_COUNTRY_SEQ".nextval into :NEW."id_country" from dual;
end;
/

CREATE TABLE "Genre" (
	"id_genre" INT PRIMARY KEY,
	"name_genre" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "GENRE_ID_GENRE_SEQ";

CREATE trigger "BI_GENRE_ID_GENRE"
    before insert on "Genre"
    for each row
begin
    select "GENRE_ID_GENRE_SEQ".nextval into :NEW."id_genre" from dual;
end;
/

CREATE TABLE "Age_category" (
	"id_age_category" INT PRIMARY KEY,
	"name_age_category" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "AGE_CATEGORY_ID_AGE_CATEGORY_SEQ";

CREATE trigger "BI_AGE_CATEGORY_ID_AGE_CATEGORY"
    before insert on "Age_category"
    for each row
begin
    select "AGE_CATEGORY_ID_AGE_CATEGORY_SEQ".nextval into :NEW."id_age_category" from dual;
end;
/

CREATE TABLE "Ticket" (
	"id_ticket" INT PRIMARY KEY,
	"id_performance" INT NOT NULL,
	"seat_number_ticket" INT NOT NULL CHECK("seat_number_ticket" >= 0),
	"cost_ticket" NUMERIC NOT NULL CHECK("cost_ticket" >= 0),
    "is_sold" INT NOT NULL,
    "date_sale" DATE);

CREATE sequence "TICKET_ID_TICKET_SEQ";
/

CREATE TABLE "Repertoire" (
	"id_performance" INT PRIMARY KEY,
	"id_show" INT NOT NULL,
	"performance_date_repertoire" DATE NOT NULL);

CREATE sequence "REPERTOIRE_ID_PERFORMANCE_SEQ";
/

CREATE TABLE "Direction" (
	"id_actor" INT NOT NULL,
	"id_role" INT NOT NULL,
	"is_understudy_direction" INT NOT NULL,
	constraint DIRECTION_PK PRIMARY KEY ("id_actor","id_role"));
/

CREATE TABLE "Tour" (
	"id_employee" INT NOT NULL,
	"id_show" INT NOT NULL,
	"from_date_tour" DATE NOT NULL,
	"to_date_tour" DATE NOT NULL,
	"is_visiting_tour" INT NOT NULL);
/

CREATE TABLE "Job_types" (
	"id_job_type" INT PRIMARY KEY,
    "id_parent_job_type" INT, 
	"name_job_type" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "JOB_TYPES_ID_JOB_TYPE_SEQ";

CREATE trigger "BI_JOB_TYPES_ID_JOB_TYPE"
    before insert on "Job_types"
    for each row
begin
    select "JOB_TYPES_ID_JOB_TYPE_SEQ".nextval into :NEW."id_job_type" from dual;
end;
/

CREATE TABLE "Role-Characteristic" (
	"id_characteristic" INT NOT NULL,
	"id_role" INT NOT NULL,
	"value_role_characteristic" DECIMAL,
	constraint ROLE_CHARACTERISTIC_PK PRIMARY KEY ("id_characteristic","id_role"));

CREATE TABLE "Role" (
	"id_role" INT PRIMARY KEY,
	"id_show" INT NOT NULL,
	"name_role" VARCHAR2(255) NOT NULL,
	"is_main_role" INT NOT NULL);

CREATE sequence "ROLE_ID_ROLE_SEQ";

CREATE trigger "BI_ROLE_ID_ROLE"
    before insert on "Role"
    for each row
begin
    select "ROLE_ID_ROLE_SEQ".nextval into :NEW."id_role" from dual;
end;
/

CREATE TABLE "Subscription" (
	"id_subscription" INT PRIMARY KEY,
	"id_genre" INT,
	"id_author" INT);

CREATE sequence "SUBSCRIPTION_ID_SUBSCRIPTION_SEQ";
/

CREATE TABLE "Ticket-Subscription" (
	"id_ticket" INT NOT NULL,
	"id_subscription" INT NOT NULL,
	constraint TICKET_SUBSCRIPTION_PK PRIMARY KEY ("id_ticket","id_subscription"));
/

CREATE TABLE "Musical_instruments" (
    "id_instrument" INT PRIMARY KEY,
    "name_instrument" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "MUSICAL_INSTRUMENTS_ID_INSTRUMENT_SEQ";

CREATE trigger "BI_MUSICAL_INSTRUMENTS_ID_INSTRUMENT"
    before insert on "Musical_instruments"
    for each row
begin
    select "MUSICAL_INSTRUMENTS_ID_INSTRUMENT_SEQ".nextval into :NEW."id_instrument" from dual;
end;
/

CREATE TABLE "Musician-Instrument" (
    "id_musician" INT NOT NULL,
    "id_instrument" INT NOT NULL,
    constraint MUSICIAN_INSTRUMENT_PK PRIMARY KEY ("id_musician","id_instrument"))
/

ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk0" FOREIGN KEY ("id_gender") REFERENCES "Gender"("id_gender");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk1" FOREIGN KEY ("id_education") REFERENCES "Education"("id_education");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk2" FOREIGN KEY ("id_job_type") REFERENCES "Job_types"("id_job_type");

ALTER TABLE "Musician-Show" ADD CONSTRAINT "Musician-Show_fk0" FOREIGN KEY ("id_musician") REFERENCES "Employee"("id_employee");
ALTER TABLE "Musician-Show" ADD CONSTRAINT "Musician-Show_fk1" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show")
    ON DELETE CASCADE;

ALTER TABLE "Job_types" ADD CONSTRAINT "Job_types_fk0" FOREIGN KEY ("id_parent_job_type") REFERENCES "Job_types"("id_job_type")
    ON DELETE CASCADE;

ALTER TABLE "Actor-Characteristic" ADD CONSTRAINT "Actor-Characteristic_fk0" FOREIGN KEY ("id_actor")
    REFERENCES "Employee"("id_employee") ON DELETE CASCADE;
ALTER TABLE "Actor-Characteristic" ADD CONSTRAINT "Actor-Characteristic_fk1" FOREIGN KEY ("id_characteristic")
    REFERENCES "Characteristic"("id_characteristic");

ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk0" FOREIGN KEY ("id_actor") REFERENCES "Employee"("id_employee")
    ON DELETE CASCADE;
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk1" FOREIGN KEY ("id_rank") REFERENCES "Rank"("id_rank");
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk2" FOREIGN KEY ("id_competition") REFERENCES "Competition"("id_competition");

ALTER TABLE "Show" ADD CONSTRAINT "Show_fk0" FOREIGN KEY ("id_director") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk1" FOREIGN KEY ("id_production_designer") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk2" FOREIGN KEY ("id_conductor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk3" FOREIGN KEY ("id_author") REFERENCES "Author"("id_author");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk4" FOREIGN KEY ("id_genre") REFERENCES "Genre"("id_genre");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk5" FOREIGN KEY ("id_age_category") REFERENCES "Age_category"("id_age_category");

ALTER TABLE "Author" ADD CONSTRAINT "Author_fk0" FOREIGN KEY ("id_country") REFERENCES "Country"("id_country");

ALTER TABLE "Ticket" ADD CONSTRAINT "Ticket_fk0" FOREIGN KEY ("id_performance") REFERENCES "Repertoire"("id_performance");

ALTER TABLE "Repertoire" ADD CONSTRAINT "Repertoire_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");

ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk0" FOREIGN KEY ("id_actor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk1" FOREIGN KEY ("id_role") REFERENCES "Role"("id_role");

ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk0" FOREIGN KEY ("id_employee") REFERENCES "Employee"("id_employee");
ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk1" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");

ALTER TABLE "Role-Characteristic" ADD CONSTRAINT "Role-Characteristic_fk0" FOREIGN KEY ("id_characteristic")
    REFERENCES "Characteristic"("id_characteristic");
ALTER TABLE "Role-Characteristic" ADD CONSTRAINT "Role-Characteristic_fk1" FOREIGN KEY ("id_role")
    REFERENCES "Role"("id_role") ON DELETE CASCADE;

ALTER TABLE "Role" ADD CONSTRAINT "Role_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show") ON DELETE CASCADE;

ALTER TABLE "Subscription" ADD CONSTRAINT "Subscription_fk0" FOREIGN KEY ("id_genre") REFERENCES "Genre"("id_genre")
    ON DELETE CASCADE;
ALTER TABLE "Subscription" ADD CONSTRAINT "Subscription_fk1" FOREIGN KEY ("id_author") REFERENCES "Author"("id_author")
    ON DELETE CASCADE;

ALTER TABLE "Ticket-Subscription" ADD CONSTRAINT "Ticket-Subscription_fk0" FOREIGN KEY ("id_ticket")
    REFERENCES "Ticket"("id_ticket") ON DELETE CASCADE;
ALTER TABLE "Ticket-Subscription" ADD CONSTRAINT "Ticket-Subscription_fk1" FOREIGN KEY ("id_subscription")
    REFERENCES "Subscription"("id_subscription") ON DELETE CASCADE;
    
ALTER TABLE "Musician-Instrument" ADD CONSTRAINT "Musician-Instrument_fk0" FOREIGN KEY ("id_musician")
    REFERENCES "Employee"("id_employee") ON DELETE CASCADE;
ALTER TABLE "Musician-Instrument" ADD CONSTRAINT "Musician-Instrument_fk1" FOREIGN KEY ("id_instrument")
    REFERENCES "Musical_instruments"("id_instrument");
/

COMMIT;