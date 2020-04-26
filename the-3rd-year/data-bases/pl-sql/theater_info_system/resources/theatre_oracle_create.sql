CREATE TABLE "Rank" (
	"id_rank" INT PRIMARY KEY,
	"rank_name" VARCHAR2(255) UNIQUE NOT NULL);

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
	"name" VARCHAR2(255) NOT NULL,
	"surname" VARCHAR2(255),
	"middle_name" VARCHAR2(255),
	"id_gender" INT NOT NULL,
	"birthday" DATE NOT NULL,
	"hire_date" DATE NOT NULL,
	"children_amount" INT DEFAULT 0 CHECK("children_amount" >= 0),
	"salary" NUMERIC(*, 2) NOT NULL CHECK("salary" > 0),
	"id_education" INT NOT NULL,
	"id_job_type" INT NOT NULL);

CREATE sequence "EMPLOYEE_ID_EMPLOYEE_SEQ";

CREATE trigger "BI_EMPLOYEE_ID_EMPLOYEE"
  before insert on "Employee"
  for each row
begin
  select "EMPLOYEE_ID_EMPLOYEE_SEQ".nextval into :NEW."id_employee" from dual;
end;

/
CREATE TABLE "Gender" (
	"id_gender" INT PRIMARY KEY,
	"gender_name" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "GENDER_ID_GENDER_SEQ";

CREATE trigger "BI_GENDER_ID_GENDER"
  before insert on "Gender"
  for each row
begin
  select "GENDER_ID_GENDER_SEQ".nextval into :NEW."id_gender" from dual;
end;

/
CREATE TABLE "Employee-Characteristic" (
	"id_employee" INT PRIMARY KEY,
	"id_characteristic" INT NOT NULL,
	"value" NUMERIC(*, 2));

/
CREATE TABLE "Competition" (
	"id_competiton" INT PRIMARY KEY,
	"competiton_name" VARCHAR2(255) UNIQUE NOT NULL);

CREATE sequence "COMPETITION_ID_COMPETITON_SEQ";

CREATE trigger "BI_COMPETITION_ID_COMPETITON"
  before insert on "Competition"
  for each row
begin
  select "COMPETITION_ID_COMPETITON_SEQ".nextval into :NEW."id_competiton" from dual;
end;

/
CREATE TABLE "Actor-Rank" (
	"id_actor" INT NOT NULL,
	"id_rank" INT NOT NULL,
	"obtaining_date" DATE NOT NULL,
	"id_competition" INT NOT NULL,
	constraint ACTOR_RANK_PK PRIMARY KEY ("id_actor","id_rank"));

/
CREATE TABLE "Characteristic" (
	"id_characteristic" INT PRIMARY KEY,
	"characteristic_type" VARCHAR2(255) UNIQUE NOT NULL);

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
	"level_name" VARCHAR2(255) UNIQUE NOT NULL);

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
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	"id_director" INT NOT NULL,
	"id_production_designer" INT NOT NULL,
	"id_conductor" INT NOT NULL,
	"id_author" INT NOT NULL,
	"id_genre" INT NOT NULL,
	"id_age_category" INT NOT NULL,
	"century" INT NOT NULL,
	"premier_date" DATE NOT NULL);

CREATE sequence "SHOW_ID_SHOW_SEQ";

CREATE trigger "BI_SHOW_ID_SHOW"
  before insert on "Show"
  for each row
begin
  select "SHOW_ID_SHOW_SEQ".nextval into :NEW."id_show" from dual;
end;

/
CREATE TABLE "Author" (
	"id_author" INT PRIMARY KEY,
	"name" VARCHAR2(255) NOT NULL,
	"surname" VARCHAR2(255),
	"middle_name" VARCHAR2(255),
	"life_century" INT NOT NULL,
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
	"country_name" VARCHAR2(255) UNIQUE NOT NULL);

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
	"genre_name" VARCHAR2(255) UNIQUE NOT NULL);

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
	"age_category_name" VARCHAR2(255) UNIQUE NOT NULL);

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
	"seat_number" INT NOT NULL CHECK("seat_number" >= 0),
	"cost" NUMERIC NOT NULL CHECK("cost" >= 0));

CREATE sequence "TICKET_ID_TICKET_SEQ";

CREATE trigger "BI_TICKET_ID_TICKET"
  before insert on "Ticket"
  for each row
begin
  select "TICKET_ID_TICKET_SEQ".nextval into :NEW."id_ticket" from dual;
end;

/
CREATE TABLE "Sale" (
	"id_ticket" INT PRIMARY KEY,
	"sale_date" DATE NOT NULL);

/
CREATE TABLE "Repertoire" (
	"id_performance" INT PRIMARY KEY,
	"id_show" INT NOT NULL,
	"performance_date" DATE NOT NULL);

CREATE sequence "REPERTOIRE_ID_PERFORMANCE_SEQ";

CREATE trigger "BI_REPERTOIRE_ID_PERFORMANCE"
  before insert on "Repertoire"
  for each row
begin
  select "REPERTOIRE_ID_PERFORMANCE_SEQ".nextval into :NEW."id_performance" from dual;
end;

/
CREATE TABLE "Direction" (
	"id_actor" INT NOT NULL,
	"id_role" INT NOT NULL,
	"is_understudy" INT NOT NULL,
	constraint DIRECTION_PK PRIMARY KEY ("id_actor","id_role"));

/
CREATE TABLE "Tour" (
	"id_employee" INT NOT NULL,
	"id_show" INT NOT NULL,
	"from_date" DATE NOT NULL,
	"to_date" DATE NOT NULL,
	"is_visiting_tour" INT NOT NULL);

/
CREATE TABLE "Job_types" (
	"id_job_type" INT PRIMARY KEY,
	"job_name" VARCHAR2(255) UNIQUE NOT NULL);

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
	"value" DECIMAL,
	constraint ROLE_CHARACTERISTIC_PK PRIMARY KEY ("id_characteristic","id_role"));

CREATE TABLE "Role" (
	"id_role" INT PRIMARY KEY,
	"id_show" INT NOT NULL,
	"role_name" VARCHAR2(255) NOT NULL,
	"is_main" INT NOT NULL);

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

CREATE trigger "BI_SUBSCRIPTION_ID_SUBSCRIPTION"
  before insert on "Subscription"
  for each row
begin
  select "SUBSCRIPTION_ID_SUBSCRIPTION_SEQ".nextval into :NEW."id_subscription" from dual;
end;

/
CREATE TABLE "Ticket-Subscription" (
	"id_ticket" INT NOT NULL,
	"id_subscription" INT NOT NULL,
	constraint TICKET_SUBSCRIPTION_PK PRIMARY KEY ("id_ticket","id_subscription"));

/
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk0" FOREIGN KEY ("id_gender") REFERENCES "Gender"("id_gender");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk1" FOREIGN KEY ("id_education") REFERENCES "Education"("id_education");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk2" FOREIGN KEY ("id_job_type") REFERENCES "Job_types"("id_job_type");

ALTER TABLE "Employee-Characteristic" ADD CONSTRAINT "Employee-Characteristic_fk0" FOREIGN KEY ("id_employee") REFERENCES "Employee"("id_employee");
ALTER TABLE "Employee-Characteristic" ADD CONSTRAINT "Employee-Characteristic_fk1" FOREIGN KEY ("id_characteristic") REFERENCES "Characteristic"("id_characteristic");

ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk0" FOREIGN KEY ("id_actor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk1" FOREIGN KEY ("id_rank") REFERENCES "Rank"("id_rank");
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk2" FOREIGN KEY ("id_competition") REFERENCES "Competition"("id_competiton");

ALTER TABLE "Show" ADD CONSTRAINT "Show_fk0" FOREIGN KEY ("id_director") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk1" FOREIGN KEY ("id_production_designer") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk2" FOREIGN KEY ("id_conductor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk3" FOREIGN KEY ("id_author") REFERENCES "Author"("id_author");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk4" FOREIGN KEY ("id_genre") REFERENCES "Genre"("id_genre");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk5" FOREIGN KEY ("id_age_category") REFERENCES "Age_category"("id_age_category");

ALTER TABLE "Author" ADD CONSTRAINT "Author_fk0" FOREIGN KEY ("id_country") REFERENCES "Country"("id_country");

ALTER TABLE "Ticket" ADD CONSTRAINT "Ticket_fk0" FOREIGN KEY ("id_performance") REFERENCES "Repertoire"("id_performance");

ALTER TABLE "Sale" ADD CONSTRAINT "Sale_fk0" FOREIGN KEY ("id_ticket") REFERENCES "Ticket"("id_ticket");

ALTER TABLE "Repertoire" ADD CONSTRAINT "Repertoire_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");

ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk0" FOREIGN KEY ("id_actor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk1" FOREIGN KEY ("id_role") REFERENCES "Role"("id_role");

ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk0" FOREIGN KEY ("id_employee") REFERENCES "Employee"("id_employee");
ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk1" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");

ALTER TABLE "Role-Characteristic" ADD CONSTRAINT "Role-Characteristic_fk0" FOREIGN KEY ("id_characteristic") REFERENCES "Characteristic"("id_characteristic");
ALTER TABLE "Role-Characteristic" ADD CONSTRAINT "Role-Characteristic_fk1" FOREIGN KEY ("id_role") REFERENCES "Role"("id_role");

ALTER TABLE "Role" ADD CONSTRAINT "Role_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");

ALTER TABLE "Subscription" ADD CONSTRAINT "Subscription_fk0" FOREIGN KEY ("id_genre") REFERENCES "Genre"("id_genre");
ALTER TABLE "Subscription" ADD CONSTRAINT "Subscription_fk1" FOREIGN KEY ("id_author") REFERENCES "Author"("id_author");

ALTER TABLE "Ticket-Subscription" ADD CONSTRAINT "Ticket-Subscription_fk0" FOREIGN KEY ("id_ticket") REFERENCES "Ticket"("id_ticket");
ALTER TABLE "Ticket-Subscription" ADD CONSTRAINT "Ticket-Subscription_fk1" FOREIGN KEY ("id_subscription") REFERENCES "Subscription"("id_subscription");

/
CREATE trigger "ACTOR-RANK-INSERTION"
  before insert on "Actor-Rank"
  for each row
declare
  id_actor_job_type INT;
  employee_job_type INT;
begin
  select "id_job_type" into employee_job_type
  from "Employee"
  where "id_employee" = :NEW."id_actor";

  select "id_job_type" into id_actor_job_type
  from "Job_types"
  where "job_name" like 'Actor';
  
  if employee_job_type != id_actor_job_type then
    raise_application_error(-20000, 'Попытка добавления не актера! Отмена.');
  end if;
end;
/

COMMIT;