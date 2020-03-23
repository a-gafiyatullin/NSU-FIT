CREATE TABLE "Rank" (
	"id_rank" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint RANK_PK PRIMARY KEY ("id_rank"));

CREATE sequence "RANK_ID_RANK_SEQ";

CREATE trigger "BI_RANK_ID_RANK"
  before insert on "Rank"
  for each row
begin
  select "RANK_ID_RANK_SEQ".nextval into :NEW."id_rank" from dual;
end;

/
CREATE TABLE "Employee" (
	"id_employee" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) NOT NULL,
	"surname" VARCHAR2(255),
	"middle_name" VARCHAR2(255),
	"id_gender" INT,
	"birthday" DATE NOT NULL,
	"hire_date" DATE NOT NULL,
	"children_amount" INT NOT NULL DEFAULT "0",
	"salary" NUMERIC NOT NULL,
	"id_education" INT,
	constraint EMPLOYEE_PK PRIMARY KEY ("id_employee"));

CREATE sequence "EMPLOYEE_ID_EMPLOYEE_SEQ";

CREATE trigger "BI_EMPLOYEE_ID_EMPLOYEE"
  before insert on "Employee"
  for each row
begin
  select "EMPLOYEE_ID_EMPLOYEE_SEQ".nextval into :NEW."id_employee" from dual;
end;

/
CREATE TABLE "Gender" (
	"id_gender" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint GENDER_PK PRIMARY KEY ("id_gender"));

CREATE sequence "GENDER_ID_GENDER_SEQ";

CREATE trigger "BI_GENDER_ID_GENDER"
  before insert on "Gender"
  for each row
begin
  select "GENDER_ID_GENDER_SEQ".nextval into :NEW."id_gender" from dual;
end;

/
CREATE TABLE "Actor" (
	"id_actor" INT NOT NULL,
	"id_voice" INT NOT NULL,
	"height" NUMERIC NOT NULL,
	constraint ACTOR_PK PRIMARY KEY ("id_actor"));


/
CREATE TABLE "Competition" (
	"id_competiton" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint COMPETITION_PK PRIMARY KEY ("id_competiton"));

CREATE sequence "COMPETITION_ID_COMPETITON_SEQ";

CREATE trigger "BI_COMPETITION_ID_COMPETITON"
  before insert on "Competition"
  for each row
begin
  select "COMPETITION_ID_COMPETITON_SEQ".nextval into :NEW."id_competiton" from dual;
end;

/
CREATE TABLE "Director_type" (
	"id_director_type" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint DIRECTOR_TYPE_PK PRIMARY KEY ("id_director_type"));

CREATE sequence "DIRECTOR_TYPE_ID_DIRECTOR_TYPE_SEQ";

CREATE trigger "BI_DIRECTOR_TYPE_ID_DIRECTOR_TYPE"
  before insert on "Director_type"
  for each row
begin
  select "DIRECTOR_TYPE_ID_DIRECTOR_TYPE_SEQ".nextval into :NEW."id_director_type" from dual;
end;

/
CREATE TABLE "Staff" (
	"id_staff" INT NOT NULL,
	"id_staff_type" INT NOT NULL,
	constraint STAFF_PK PRIMARY KEY ("id_staff"));


/
CREATE TABLE "Actor-Rank" (
	"id_actor" INT NOT NULL,
	"id_rank" INT NOT NULL,
	"obtaining_date" DATE NOT NULL,
	"id_competition" INT,
	constraint ACTOR-RANK_PK PRIMARY KEY ("id_actor","id_rank"));


/
CREATE TABLE "Director" (
	"id_director" INT NOT NULL,
	"id_director_type" INT NOT NULL,
	constraint DIRECTOR_PK PRIMARY KEY ("id_director"));


/
CREATE TABLE "Voice" (
	"id_voice" INT UNIQUE NOT NULL,
	"voice_type" VARCHAR2(255) UNIQUE NOT NULL,
	constraint VOICE_PK PRIMARY KEY ("id_voice"));

CREATE sequence "VOICE_ID_VOICE_SEQ";

CREATE trigger "BI_VOICE_ID_VOICE"
  before insert on "Voice"
  for each row
begin
  select "VOICE_ID_VOICE_SEQ".nextval into :NEW."id_voice" from dual;
end;

/
CREATE TABLE "Education" (
	"id_education" INT UNIQUE NOT NULL,
	"level_name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint EDUCATION_PK PRIMARY KEY ("id_education"));

CREATE sequence "EDUCATION_ID_EDUCATION_SEQ";

CREATE trigger "BI_EDUCATION_ID_EDUCATION"
  before insert on "Education"
  for each row
begin
  select "EDUCATION_ID_EDUCATION_SEQ".nextval into :NEW."id_education" from dual;
end;

/
CREATE TABLE "Musician" (
	"id_musician" INT NOT NULL,
	"id_musician_type" INT NOT NULL,
	constraint MUSICIAN_PK PRIMARY KEY ("id_musician"));


/
CREATE TABLE "Musician_type" (
	"id_musician_type" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint MUSICIAN_TYPE_PK PRIMARY KEY ("id_musician_type"));

CREATE sequence "MUSICIAN_TYPE_ID_MUSICIAN_TYPE_SEQ";

CREATE trigger "BI_MUSICIAN_TYPE_ID_MUSICIAN_TYPE"
  before insert on "Musician_type"
  for each row
begin
  select "MUSICIAN_TYPE_ID_MUSICIAN_TYPE_SEQ".nextval into :NEW."id_musician_type" from dual;
end;

/
CREATE TABLE "Staff_type" (
	"id_staff_type" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint STAFF_TYPE_PK PRIMARY KEY ("id_staff_type"));

CREATE sequence "STAFF_TYPE_ID_STAFF_TYPE_SEQ";

CREATE trigger "BI_STAFF_TYPE_ID_STAFF_TYPE"
  before insert on "Staff_type"
  for each row
begin
  select "STAFF_TYPE_ID_STAFF_TYPE_SEQ".nextval into :NEW."id_staff_type" from dual;
end;

/
CREATE TABLE "Show" (
	"id_show" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	"id_director" INT NOT NULL,
	"id_production_designer" INT NOT NULL,
	"id_conductor" INT NOT NULL,
	"id_author" INT,
	"id_genre" INT NOT NULL,
	"id_age_category" INT NOT NULL,
	"century" INT,
	"premier_date" DATE NOT NULL,
	constraint SHOW_PK PRIMARY KEY ("id_show"));

CREATE sequence "SHOW_ID_SHOW_SEQ";

CREATE trigger "BI_SHOW_ID_SHOW"
  before insert on "Show"
  for each row
begin
  select "SHOW_ID_SHOW_SEQ".nextval into :NEW."id_show" from dual;
end;

/
CREATE TABLE "Author" (
	"id_author" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) NOT NULL,
	"surname" VARCHAR2(255),
	"middle_name" VARCHAR2(255),
	"life_century" INT,
	"id_country" INT,
	constraint AUTHOR_PK PRIMARY KEY ("id_author"));

CREATE sequence "AUTHOR_ID_AUTHOR_SEQ";

CREATE trigger "BI_AUTHOR_ID_AUTHOR"
  before insert on "Author"
  for each row
begin
  select "AUTHOR_ID_AUTHOR_SEQ".nextval into :NEW."id_author" from dual;
end;

/
CREATE TABLE "Country" (
	"id_country" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint COUNTRY_PK PRIMARY KEY ("id_country"));

CREATE sequence "COUNTRY_ID_COUNTRY_SEQ";

CREATE trigger "BI_COUNTRY_ID_COUNTRY"
  before insert on "Country"
  for each row
begin
  select "COUNTRY_ID_COUNTRY_SEQ".nextval into :NEW."id_country" from dual;
end;

/
CREATE TABLE "Genre" (
	"id_genre" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint GENRE_PK PRIMARY KEY ("id_genre"));

CREATE sequence "GENRE_ID_GENRE_SEQ";

CREATE trigger "BI_GENRE_ID_GENRE"
  before insert on "Genre"
  for each row
begin
  select "GENRE_ID_GENRE_SEQ".nextval into :NEW."id_genre" from dual;
end;

/
CREATE TABLE "Age_category" (
	"id_age_category" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint AGE_CATEGORY_PK PRIMARY KEY ("id_age_category"));

CREATE sequence "AGE_CATEGORY_ID_AGE_CATEGORY_SEQ";

CREATE trigger "BI_AGE_CATEGORY_ID_AGE_CATEGORY"
  before insert on "Age_category"
  for each row
begin
  select "AGE_CATEGORY_ID_AGE_CATEGORY_SEQ".nextval into :NEW."id_age_category" from dual;
end;

/
CREATE TABLE "Ticket" (
	"id_ticket" INT UNIQUE NOT NULL,
	"id_performance" INT NOT NULL,
	"seat_number" INT NOT NULL,
	"cost" DATE NOT NULL,
	constraint TICKET_PK PRIMARY KEY ("id_ticket"));

CREATE sequence "TICKET_ID_TICKET_SEQ";

CREATE trigger "BI_TICKET_ID_TICKET"
  before insert on "Ticket"
  for each row
begin
  select "TICKET_ID_TICKET_SEQ".nextval into :NEW."id_ticket" from dual;
end;

/
CREATE TABLE "Sale" (
	"id_ticket" INT NOT NULL,
	"sale_date" DATE NOT NULL,
	constraint SALE_PK PRIMARY KEY ("id_ticket"));


/
CREATE TABLE "Repertoire" (
	"id_performance" INT UNIQUE NOT NULL,
	"id_show" INT NOT NULL,
	"performance_date" DATE NOT NULL,
	constraint REPERTOIRE_PK PRIMARY KEY ("id_performance"));

CREATE sequence "REPERTOIRE_ID_PERFORMANCE_SEQ";

CREATE trigger "BI_REPERTOIRE_ID_PERFORMANCE"
  before insert on "Repertoire"
  for each row
begin
  select "REPERTOIRE_ID_PERFORMANCE_SEQ".nextval into :NEW."id_performance" from dual;
end;

/
CREATE TABLE "Direction" (
	"id_show" INT NOT NULL,
	"id_actor" INT NOT NULL,
	"role" VARCHAR2(255) NOT NULL,
	"id_actor_type" INT NOT NULL,
	constraint DIRECTION_PK PRIMARY KEY ("id_show","id_actor"));


/
CREATE TABLE "Actor_type" (
	"id_actor_type" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint ACTOR_TYPE_PK PRIMARY KEY ("id_actor_type"));

CREATE sequence "ACTOR_TYPE_ID_ACTOR_TYPE_SEQ";

CREATE trigger "BI_ACTOR_TYPE_ID_ACTOR_TYPE"
  before insert on "Actor_type"
  for each row
begin
  select "ACTOR_TYPE_ID_ACTOR_TYPE_SEQ".nextval into :NEW."id_actor_type" from dual;
end;

/
CREATE TABLE "Tour" (
	"id_employee" BFILE NOT NULL,
	"id_show" INT NOT NULL,
	"from_date" DATE NOT NULL,
	"to_date" DATE NOT NULL,
	"id_tour_type" INT NOT NULL);


/
CREATE TABLE "Tour_type" (
	"id_tour_type" INT UNIQUE NOT NULL,
	"name" VARCHAR2(255) UNIQUE NOT NULL,
	constraint TOUR_TYPE_PK PRIMARY KEY ("id_tour_type"));

CREATE sequence "TOUR_TYPE_ID_TOUR_TYPE_SEQ";

CREATE trigger "BI_TOUR_TYPE_ID_TOUR_TYPE"
  before insert on "Tour_type"
  for each row
begin
  select "TOUR_TYPE_ID_TOUR_TYPE_SEQ".nextval into :NEW."id_tour_type" from dual;
end;

/

ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk0" FOREIGN KEY ("id_gender") REFERENCES "Gender"("id_gender");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk1" FOREIGN KEY ("id_education") REFERENCES "Education"("id_education");


ALTER TABLE "Actor" ADD CONSTRAINT "Actor_fk0" FOREIGN KEY ("id_actor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Actor" ADD CONSTRAINT "Actor_fk1" FOREIGN KEY ("id_voice") REFERENCES "Voice"("id_voice");



ALTER TABLE "Staff" ADD CONSTRAINT "Staff_fk0" FOREIGN KEY ("id_staff") REFERENCES "Employee"("id_employee");
ALTER TABLE "Staff" ADD CONSTRAINT "Staff_fk1" FOREIGN KEY ("id_staff_type") REFERENCES "Staff_type"("id_staff_type");

ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk0" FOREIGN KEY ("id_actor") REFERENCES "Actor"("id_actor");
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk1" FOREIGN KEY ("id_rank") REFERENCES "Rank"("id_rank");
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk2" FOREIGN KEY ("id_competition") REFERENCES "Competition"("id_competiton");

ALTER TABLE "Director" ADD CONSTRAINT "Director_fk0" FOREIGN KEY ("id_director") REFERENCES "Employee"("id_employee");
ALTER TABLE "Director" ADD CONSTRAINT "Director_fk1" FOREIGN KEY ("id_director_type") REFERENCES "Director_type"("id_director_type");



ALTER TABLE "Musician" ADD CONSTRAINT "Musician_fk0" FOREIGN KEY ("id_musician") REFERENCES "Employee"("id_employee");
ALTER TABLE "Musician" ADD CONSTRAINT "Musician_fk1" FOREIGN KEY ("id_musician_type") REFERENCES "Musician_type"("id_musician_type");



ALTER TABLE "Show" ADD CONSTRAINT "Show_fk0" FOREIGN KEY ("id_director") REFERENCES "Director"("id_director");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk1" FOREIGN KEY ("id_production_designer") REFERENCES "Director"("id_director");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk2" FOREIGN KEY ("id_conductor") REFERENCES "Director"("id_director");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk3" FOREIGN KEY ("id_author") REFERENCES "Author"("id_author");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk4" FOREIGN KEY ("id_genre") REFERENCES "Genre"("id_genre");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk5" FOREIGN KEY ("id_age_category") REFERENCES "Age_category"("id_age_category");

ALTER TABLE "Author" ADD CONSTRAINT "Author_fk0" FOREIGN KEY ("id_country") REFERENCES "Country"("id_country");




ALTER TABLE "Ticket" ADD CONSTRAINT "Ticket_fk0" FOREIGN KEY ("id_performance") REFERENCES "Repertoire"("id_performance");

ALTER TABLE "Sale" ADD CONSTRAINT "Sale_fk0" FOREIGN KEY ("id_ticket") REFERENCES "Ticket"("id_ticket");

ALTER TABLE "Repertoire" ADD CONSTRAINT "Repertoire_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");

ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");
ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk1" FOREIGN KEY ("id_actor") REFERENCES "Actor"("id_actor");
ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk2" FOREIGN KEY ("id_actor_type") REFERENCES "Actor_type"("id_actor_type");


ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk0" FOREIGN KEY ("id_employee") REFERENCES "Employee"("id_employee");
ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk1" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show");
ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk2" FOREIGN KEY ("id_tour_type") REFERENCES "Tour_type"("id_tour_type");


