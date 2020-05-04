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
	"id_job_type" INT NOT NULL,
    "is_active_employee" INT NOT NULL);

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
    before insert on "Gender"
    for each row
begin
    select "GENDER_ID_GENDER_SEQ".nextval into :NEW."id_gender" from dual;
end;
/

CREATE TABLE "Employee-Characteristic" (
	"id_employee" INT PRIMARY KEY,
	"id_characteristic" INT NOT NULL,
	"value_employee_characteristic" NUMERIC(*, 2));
/

CREATE TABLE "Competition" (
	"id_competiton" INT PRIMARY KEY,
	"name_competiton" VARCHAR2(255) UNIQUE NOT NULL);

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
	"cost_ticket" NUMERIC NOT NULL CHECK("cost_ticket" >= 0));

CREATE sequence "TICKET_ID_TICKET_SEQ";
/

CREATE TABLE "Sale" (
	"id_ticket" INT PRIMARY KEY,
	"date_sale" DATE NOT NULL);
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

ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk0" FOREIGN KEY ("id_gender") REFERENCES "Gender"("id_gender");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk1" FOREIGN KEY ("id_education") REFERENCES "Education"("id_education");
ALTER TABLE "Employee" ADD CONSTRAINT "Employee_fk2" FOREIGN KEY ("id_job_type") REFERENCES "Job_types"("id_job_type");

ALTER TABLE "Musician-Show" ADD CONSTRAINT "Musician-Show_fk0" FOREIGN KEY ("id_musician") REFERENCES "Employee"("id_employee")
    ON DELETE CASCADE;
ALTER TABLE "Musician-Show" ADD CONSTRAINT "Musician-Show_fk1" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show")
    ON DELETE CASCADE;

ALTER TABLE "Employee-Characteristic" ADD CONSTRAINT "Employee-Characteristic_fk0" FOREIGN KEY ("id_employee")
    REFERENCES "Employee"("id_employee") ON DELETE CASCADE;
ALTER TABLE "Employee-Characteristic" ADD CONSTRAINT "Employee-Characteristic_fk1" FOREIGN KEY ("id_characteristic")
    REFERENCES "Characteristic"("id_characteristic") ON DELETE CASCADE;

ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk0" FOREIGN KEY ("id_actor") REFERENCES "Employee"("id_employee")
    ON DELETE CASCADE;
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk1" FOREIGN KEY ("id_rank") REFERENCES "Rank"("id_rank")
    ON DELETE CASCADE;
ALTER TABLE "Actor-Rank" ADD CONSTRAINT "Actor-Rank_fk2" FOREIGN KEY ("id_competition")
    REFERENCES "Competition"("id_competiton");

ALTER TABLE "Show" ADD CONSTRAINT "Show_fk0" FOREIGN KEY ("id_director") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk1" FOREIGN KEY ("id_production_designer") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk2" FOREIGN KEY ("id_conductor") REFERENCES "Employee"("id_employee");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk3" FOREIGN KEY ("id_author") REFERENCES "Author"("id_author");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk4" FOREIGN KEY ("id_genre") REFERENCES "Genre"("id_genre");
ALTER TABLE "Show" ADD CONSTRAINT "Show_fk5" FOREIGN KEY ("id_age_category") REFERENCES "Age_category"("id_age_category");

ALTER TABLE "Author" ADD CONSTRAINT "Author_fk0" FOREIGN KEY ("id_country") REFERENCES "Country"("id_country");

ALTER TABLE "Ticket" ADD CONSTRAINT "Ticket_fk0" FOREIGN KEY ("id_performance")
    REFERENCES "Repertoire"("id_performance") ON DELETE CASCADE;

ALTER TABLE "Sale" ADD CONSTRAINT "Sale_fk0" FOREIGN KEY ("id_ticket") REFERENCES "Ticket"("id_ticket") ON DELETE CASCADE;

ALTER TABLE "Repertoire" ADD CONSTRAINT "Repertoire_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show") ON DELETE CASCADE;

ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk0" FOREIGN KEY ("id_actor")
    REFERENCES "Employee"("id_employee") ON DELETE CASCADE;
ALTER TABLE "Direction" ADD CONSTRAINT "Direction_fk1" FOREIGN KEY ("id_role") REFERENCES "Role"("id_role") ON DELETE CASCADE;

ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk0" FOREIGN KEY ("id_employee") REFERENCES "Employee"("id_employee") ON DELETE CASCADE;
ALTER TABLE "Tour" ADD CONSTRAINT "Tour_fk1" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show") ON DELETE CASCADE;

ALTER TABLE "Role-Characteristic" ADD CONSTRAINT "Role-Characteristic_fk0" FOREIGN KEY ("id_characteristic")
    REFERENCES "Characteristic"("id_characteristic") ON DELETE CASCADE;
ALTER TABLE "Role-Characteristic" ADD CONSTRAINT "Role-Characteristic_fk1" FOREIGN KEY ("id_role")
    REFERENCES "Role"("id_role") ON DELETE CASCADE;

ALTER TABLE "Role" ADD CONSTRAINT "Role_fk0" FOREIGN KEY ("id_show") REFERENCES "Show"("id_show") ON DELETE CASCADE;

ALTER TABLE "Subscription" ADD CONSTRAINT "Subscription_fk0" FOREIGN KEY ("id_genre")
    REFERENCES "Genre"("id_genre") ON DELETE CASCADE;
ALTER TABLE "Subscription" ADD CONSTRAINT "Subscription_fk1" FOREIGN KEY ("id_author")
    REFERENCES "Author"("id_author") ON DELETE CASCADE;

ALTER TABLE "Ticket-Subscription" ADD CONSTRAINT "Ticket-Subscription_fk0" FOREIGN KEY ("id_ticket")
    REFERENCES "Ticket"("id_ticket") ON DELETE CASCADE;
ALTER TABLE "Ticket-Subscription" ADD CONSTRAINT "Ticket-Subscription_fk1" FOREIGN KEY ("id_subscription")
    REFERENCES "Subscription"("id_subscription") ON DELETE CASCADE;
/

CREATE OR REPLACE trigger "ACTOR-RANK-INSERTION"
    before insert on "Actor-Rank"
    for each row
declare
    actor_job_type VARCHAR2(255);
    birthday DATE;
    is_active INT;
begin
    select "name_job_type", "birthday_employee", "is_active_employee" into actor_job_type, birthday, is_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_actor";

    if actor_job_type != 'актер' then
        raise_application_error(-20000, 'Попытка добавления не актера!');
    end if;
    
    if :NEW."obtaining_date_actor_rank" < birthday then
        raise_application_error(-20001, 'Получение звания не может произойти раньше рождения!');
    end if;
    
    if is_active = 0 then
        raise_application_error(-20002, 'Попытка добавления уволенного актера!');
    end if;
end;
/

CREATE OR REPLACE trigger "DIRECTION-INSERTION"
    before insert on "Direction"
    for each row
declare
    actor_job_type VARCHAR2(255);
    actors_count INT;
    role_row "Role"%ROWTYPE;
    actor_roles INT;
    is_active INT;
    main_actors_count INT;
begin
    select "name_job_type", "is_active_employee" into actor_job_type, is_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_actor";

    if actor_job_type != 'актер' then
        raise_application_error(-20000, 'Попытка назначения не актера!');
    end if;
    
    if is_active = 0 then
        raise_application_error(-20002, 'Попытка назначения уволенного актера!');
    end if;
  
    select count(*) into actors_count
    from "Direction"
    where "id_role" = :NEW."id_role";
  
    select * into role_row
    from "Role"
    where "id_role" = :NEW."id_role";
  
    if role_row."is_main_role" = 0 and actors_count = 1 then
        raise_application_error(-20003, 'Попытка назначения второго актера на не главную роль!');
    elsif role_row."is_main_role" = 1 and actors_count = 2 then
        raise_application_error(-20004, 'Попытка назначения третьего актера на главную роль!');
    elsif role_row."is_main_role" = 1 and :NEW."is_understudy_direction" = 0 and actors_count = 1 then
    begin
        select count(*) into main_actors_count
        from "Direction"
        where "id_role" = :NEW."id_role" and "is_understudy_direction" = 0;
        if main_actors_count = 1 then
            raise_application_error(-20005, 'Попытка назначения второго основного актера на главную роль!');
        end if;
    end;
    end if;
    
    select count(*) into actor_roles
    from ("Direction" inner join "Role" using("id_role"))
    where "id_actor" = :NEW."id_actor" and "id_show" = role_row."id_show";
    
    if actor_roles != 0 then
        raise_application_error(-20006, 'Попытка назначения актера на две роли в одном и том же спектакле!');
    end if;
end;
/

CREATE OR REPLACE trigger "TOUR-INSERTION"
    before insert on "Tour"
    for each row
declare
    employee_job_type VARCHAR2(255);
    is_active INT;
begin
    select "name_job_type", "is_active_employee" into employee_job_type, is_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_employee";
    
    if is_active = 0 then
        raise_application_error(-20002, 'Попытка добавления бывшего сотрудника!');
    end if;

    if employee_job_type != 'актер'
        and employee_job_type != 'музыкант'
        and employee_job_type != 'pежиссеp-постановщик'
        and employee_job_type != 'художник-постановщик'
        and employee_job_type != 'диpижеp-постановщик' then
        raise_application_error(-20007, 'Человек с данной профессией не может уезжать на гастроли!');
    end if;
  
    if :NEW."to_date_tour" < :NEW."from_date_tour" then
        raise_application_error(-20008, 'Дата начала гастролей позже даты конца!');
    end if;
end;
/

CREATE OR REPLACE trigger "SHOW-INSERTION"
    before insert on "Show"
    for each row
declare
    author_century INT;
    today_date DATE;
    conductor_job_type VARCHAR(255);
    production_designer_job_type VARCHAR(255);
    director_job_type VARCHAR(255);
    conductor_active INT;
    production_designer_active INT;
    director_active INT;
begin
    select "life_century_author" into author_century
    from "Author"
    where "id_author" = :NEW."id_author";
  
    if :NEW."century_show" < author_century then
        raise_application_error(-20009, 'Век спектакля раньше века жизни автора!');
    end if;
  
    select CURRENT_DATE into today_date from dual;
  
    if :NEW."premier_date_show" < today_date then
        raise_application_error(-20010, 'Премьера спектакля не может быть раньше сегодняшней даты!');
    end if;
  
    select "name_job_type", "is_active_employee" into director_job_type, director_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_director";
    
    if director_job_type != 'pежиссеp-постановщик' then
        raise_application_error(-20011, 'Попытка назначения на роль pежиссеpа-постановщика человека с неподхоядящей профессией!');
    end if;
    
    if director_active = 0 then
        raise_application_error(-20002, 'Попытка назначения уволенного pежиссеpа-постановщика!');
    end if;
  
    select "name_job_type", "is_active_employee" into production_designer_job_type, production_designer_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_production_designer";
    
    if production_designer_job_type != 'художник-постановщик' then
        raise_application_error(-20012, 'Попытка назначения на роль художника-постановщика человека с неподхоядящей профессией!');
    end if;
    
    if production_designer_active = 0 then
        raise_application_error(-20002, 'Попытка назначения уволенного художника-постановщика!');
    end if;
  
    select "name_job_type", "is_active_employee" into conductor_job_type, conductor_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_conductor";
  
    if conductor_job_type != 'диpижеp-постановщик' then
        raise_application_error(-20013, 'Попытка назначения на роль диpижеpа-постановщика человека с неподхоядящей профессией!');
    end if;
    
    if conductor_active = 0 then
        raise_application_error(-20002, 'Попытка назначения уволенного диpижеpа-постановщика!');
    end if;
    
    select "SHOW_ID_SHOW_SEQ".nextval into :NEW."id_show" from dual;
end;
/

CREATE OR REPLACE trigger "REPERTOIRE-INSERTION"
    before insert on "Repertoire"
    for each row
declare
    premier_date DATE;
    actors_count INT;
    cursor show_roles is
        select "id_role", "is_main_role"
        from "Role"
        where "id_show" = :NEW."id_show";
begin
    select "premier_date_show" into premier_date
    from "Show"
    where "id_show" = :NEW."id_show";
  
    if :NEW."performance_date_repertoire" < premier_date then
        raise_application_error(-20014, 'Дата показа не может быть раньше даты премьеры!');
    end if;
  
    for role_record in show_roles
    loop
        select count(*) into actors_count
        from "Direction"
        where "id_role" = role_record."id_role";
    
        if role_record."is_main_role" = 0 and actors_count < 1 or role_record."is_main_role" = 1 and actors_count < 2 then
            raise_application_error(-20015, 'Актерский состав для данного спектакля сформирован не полностью!');
        end if;
    end loop;
    
    select "REPERTOIRE_ID_PERFORMANCE_SEQ".nextval into :NEW."id_performance" from dual;
end;
/

CREATE OR REPLACE trigger "SALE-INSERTION"
    before insert on "Sale"
    for each row
begin
    select CURRENT_DATE into :NEW."sale_date" from dual;
end;
/

CREATE OR REPLACE trigger "TICKET-INSERTION"
    before insert on "Ticket"
    for each row
declare
    tickets_count INT;
begin
    select count(*) into tickets_count
    from "Ticket" 
    where "id_performance" = :NEW."id_performance" and "seat_number_ticket" = :NEW."seat_number_ticket";
    if tickets_count = 1 then
        raise_application_error(-20016, 'Билет на это место для этого спектакля уже существует!');
    end if;
    
    select "TICKET_ID_TICKET_SEQ".nextval into :NEW."id_ticket" from dual;
end;
/

CREATE OR REPLACE trigger "TICKET-SUBSCRIPTION-INSERTION"
    before insert on "Ticket-Subscription"
    for each row
declare 
    tickets_count INT;
    id_show INT;
    id_show_author INT;
    id_show_genre INT;
    id_subscription_author INT;
    id_subscription_genre INT;
begin
    select count(*) into tickets_count
    from "Ticket-Subscription"
    where "id_ticket" = :NEW."id_ticket";
    
    if tickets_count = 1 then
        raise_application_error(-20017, 'Этот билет уже добавлен в абонемент!');
    end if;
    
    select "id_show" into id_show
    from ("Ticket" inner join "Repertoire" using("id_performance"))
    where "id_ticket" = :NEW."id_ticket";
    
    select "id_author", "id_genre" into id_show_author, id_show_genre
    from "Show"
    where "id_show" = id_show;
    
    select "id_author", "id_genre" into id_subscription_author, id_subscription_genre
    from "Subscription"
    where "id_subscription" = :NEW."id_subscription";
    
    if id_subscription_author is null and id_subscription_genre != id_show_genre
    or id_subscription_genre is null and id_subscription_author != id_show_author then
        raise_application_error(-20018, 'Этот билет имеет неподходящую под абонемент тематику!');
    end if;
end;
/

CREATE OR REPLACE trigger "SUBSCRIPTION-INSERTION"
    before insert on "Subscription"
    for each row
begin
    if :NEW."id_genre" is not null and :NEW."id_author" is not null then
        raise_application_error(-20019, 'Абонемент может иметь либо конкретного автора, либо конкретный жанр!');
    end if;
    
    select "SUBSCRIPTION_ID_SUBSCRIPTION_SEQ".nextval into :NEW."id_subscription" from dual;
end;
/

CREATE OR REPLACE trigger "EMPLOYEE-INSERTION"
    before insert on "Employee"
    for each row
begin
    if :NEW."hire_date_employee" < :NEW."birthday_employee" then
        raise_application_error(-20020, 'Дата рождения сотрудника позже даты найма!');
    end if;
    
    select "EMPLOYEE_ID_EMPLOYEE_SEQ".nextval into :NEW."id_employee" from dual;
end;
/

CREATE OR REPLACE trigger "MUSICIAN-SHOW-INSERTION"
    before insert on "Musician-Show"
    for each row
declare
    musician_job_type VARCHAR2(255);
    is_active INT;
begin
    select "name_job_type", "is_active_employee" into musician_job_type, is_active
    from ("Employee" inner join "Job_types" using("id_job_type"))
    where "id_employee" = :NEW."id_musician";

    if musician_job_type != 'музыкант' then
        raise_application_error(-20021, 'Попытка добавления не музыканта!');
    end if;
    
    if is_active = 0 then
        raise_application_error(-20002, 'Попытка добавления уволенного музыканта!');
    end if;
end;
/

COMMIT;