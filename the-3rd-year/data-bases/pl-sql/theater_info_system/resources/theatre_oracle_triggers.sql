CREATE OR REPLACE trigger "ACTOR-RANK-INSERT-UPDATE"
    before insert or update on "Actor-Rank"
    for each row
declare
    birthday DATE;
begin
    select "birthday_employee" into birthday
    from "Employee"
    where "id_employee" = :NEW."id_actor";
    
    if :NEW."obtaining_date_actor_rank" < birthday then
        raise_application_error(-20000, 'Получение звания не может произойти раньше рождения!');
    end if;
end;
/

CREATE OR REPLACE trigger "DIRECTION-INSERT"
    before insert on "Direction"
    for each row
declare
    actors_count INT;
    role_row "Role"%ROWTYPE;
    actor_roles INT;
    main_actors_count INT;
    today_date DATE;
    performance_cnt INT;
begin
    select CURRENT_DATE into today_date from dual;
    
    select * into role_row
    from "Role"
    where "id_role" = :NEW."id_role";
    
    select count(*) into performance_cnt
    from "Repertoire"
    where "id_show" = role_row."id_show"
        and "performance_date_repertoire" > today_date;
        
    if performance_cnt != 0 then
        raise_application_error(-20001, 'Нельзя утверждать новые роли, пока не пройдут показы спектакля!');
    end if;
    
    select count(*) into actors_count
    from "Direction"
    where "id_role" = :NEW."id_role";
  
    if role_row."is_main_role" = 0 and actors_count = 1 then
        raise_application_error(-20002, 'Попытка назначения второго актера на не главную роль!');
    elsif role_row."is_main_role" = 1 and actors_count = 2 then
        raise_application_error(-20003, 'Попытка назначения третьего актера на главную роль!');
    elsif role_row."is_main_role" = 1 and actors_count = 1 then
        select count(*) into main_actors_count
        from "Direction"
        where "id_role" = :NEW."id_role" and "is_understudy_direction" = :NEW."is_understudy_direction";
        if main_actors_count = 1 then
            raise_application_error(-20004, 'Попытка назначения второго основного актера или дублера на главную роль!');
        end if;
    end if;
    
    select count(*) into actor_roles
    from ("Direction" inner join "Role" using("id_role"))
    where "id_actor" = :NEW."id_actor" and "id_show" = role_row."id_show";
    
    if actor_roles != 0 then
        raise_application_error(-20005, 'Попытка назначения актера на две роли в одном и том же спектакле!');
    end if;
end;
/

CREATE OR REPLACE trigger "DIRECTION-DELETE"
    before delete on "Direction"
    for each row
declare
    id_show INT;
    performance_cnt INT;
    today_date DATE;
begin
    select CURRENT_DATE into today_date from dual;

    select "id_show" into id_show
    from "Role"
    where "id_role" = :OLD."id_role";
  
    select count(*) into performance_cnt
    from "Repertoire"
    where "id_show" = id_show
        and "performance_date_repertoire" > today_date;
    
    if performance_cnt != 0 then
        raise_application_error(-20006, 'Нельзя удалять утвержденные роли, пока не пройдут показы спектакля!');
    end if;
end;
/

CREATE OR REPLACE trigger "TOUR-INSERT"
    before insert on "Tour"
    for each row
declare
    today_date        DATE;
    employee_job_type VARCHAR2(255);
    counter_director  INT;
    counter_musician  INT;
    counter_actor     INT;
    cursor tour_cur
        is
        select *
        from "Tour"
        where "id_employee" = :NEW."id_employee";
begin
    select "is_art_job_type"
    into employee_job_type
    from ("Employee"
             inner join "Job_types" using ("id_job_type"))
    where "id_employee" = :NEW."id_employee";

    if employee_job_type = 0 then
        raise_application_error(-20007, 'Человек с данной профессией не может уезжать на гастроли!');
    end if;

    select CURRENT_DATE into today_date from dual;

    if :NEW."to_date_tour" < :NEW."from_date_tour" or :NEW."from_date_tour" < today_date then
        raise_application_error(-20008, 'Дата начала гастролей позже даты конца или раньше сегодняшнего дня!');
    end if;
    
    select count(*) into counter_director
    from "Show"
    where ("id_director" = :NEW."id_employee"
            or "id_production_designer" = :NEW."id_employee"
            or "id_conductor" = :NEW."id_employee")
            and "id_show" = :NEW."id_show";
    
    select count(*) into counter_musician
    from "Musician-Show"
    where "id_musician" = :NEW."id_employee"
            and "id_show" = :NEW."id_show";
    
    select count(*) into counter_actor
    from ("Direction" inner join "Role" using("id_role"))
    where "id_actor" = :NEW."id_employee"
            and "id_show" = :NEW."id_show";
        
    if counter_actor + counter_director + counter_musician = 0 then
        raise_application_error(-20009, 'Этот человек не участвует в этом спектакле!');
    end if;    
    
    for tour_rec in tour_cur
    loop
        if tour_rec."from_date_tour" <= :NEW."from_date_tour" and tour_rec."to_date_tour" >= :NEW."from_date_tour"
            or tour_rec."from_date_tour" <= :NEW."to_date_tour" and tour_rec."to_date_tour" >= :NEW."to_date_tour" then
                raise_application_error(-20010, 'У этого сотрудника есть пересекающиеся гастроли!');
        end if;
    end loop;
end;
/

CREATE OR REPLACE trigger "SHOW-INSERT-UPDATE"
    before insert or update on "Show"
    for each row
declare
    author_century INT;
    today_date DATE;
    performance_cnt INT;
    cursor repertoire_cur
    is
        select *
        from "Repertoire"
        where "id_show" = :NEW."id_show";
begin
    select CURRENT_DATE into today_date from dual;

    if updating then    
        select count(*) into performance_cnt
        from "Repertoire"
        where "id_show" = :NEW."id_show"
            and "performance_date_repertoire" > today_date;
            
        if performance_cnt != 0 then
            raise_application_error(-20011, 'Нельзя модифицировать информацию, пока не пройдут показы спектакля!');
        end if;
    end if;

    select "life_century_author" into author_century
    from "Author"
    where "id_author" = :NEW."id_author";
  
    if :NEW."century_show" < author_century then
        raise_application_error(-20012, 'Век спектакля раньше века жизни автора!');
    end if;
  
    if inserting then
        if :NEW."premier_date_show" < today_date then
            raise_application_error(-20013, 'Премьера спектакля не может быть раньше сегодняшней даты!');
        end if;
    else
        if :NEW."premier_date_show" < :OLD."premier_date_show" then
            raise_application_error(-20014, 'Премьера спектакля не может быть раньше предыдущей даты!');
        end if;
    end if;
    
    for repertoire_rec in repertoire_cur
    loop
        if repertoire_rec."performance_date_repertoire" < :NEW."premier_date_show" then
            raise_application_error(-20015, 'Премьера спектакля не может быть позже даты выступления!');
        end if;
    end loop;
    
    if inserting then
        select "SHOW_ID_SHOW_SEQ".nextval into :NEW."id_show" from dual;
    end if;
end;
/

CREATE OR REPLACE trigger "SHOW-DELETE"
    before delete on "Show"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Repertoire"
    where "id_show" = :OLD."id_show";
    
    if cnt != 0 then
        raise_application_error(-20016, 'Этот спектакль уже в репертуаре!');
    end if;
end;
/

CREATE OR REPLACE trigger "MUSICIAN-SHOW-DELETE"
    before delete on "Musician-Show"
    for each row
declare
    cnt INT;
    today_date DATE;
begin
    select CURRENT_DATE into today_date from dual;

    select count(*) into cnt
    from "Repertoire"
    where "id_show" = :OLD."id_show"
        and "performance_date_repertoire" > today_date;
    
    if cnt != 0 then
        raise_application_error(-20017, 'Этот спектакль уже в репертуаре!');
    end if;
end;
/

CREATE OR REPLACE trigger "MUSICIAN-SHOW-INSERT"
    before insert on "Musician-Show"
    for each row
declare
    cnt INT;
    today_date DATE;
begin
    select CURRENT_DATE into today_date from dual;

    select count(*) into cnt
    from "Repertoire"
    where "id_show" = :NEW."id_show"
        and "performance_date_repertoire" > today_date;
    
    if cnt != 0 then
        raise_application_error(-20018, 'Этот спектакль уже в репертуаре!');
    end if;
end;
/

CREATE OR REPLACE trigger "REPERTOIRE-INSERT"
    before insert on "Repertoire"
    for each row
declare
    premier_date DATE;
    actors_count INT;
    cursor show_roles is
        select "id_role", "is_main_role"
        from "Role"
        where "id_show" = :NEW."id_show";
    performance_amount INT;
                              
begin
    select "premier_date_show" into premier_date
    from "Show"
    where "id_show" = :NEW."id_show";
  
    if :NEW."performance_date_repertoire" < premier_date then
        raise_application_error(-20019, 'Дата показа не может быть раньше даты премьеры!');
    end if;
  
    for role_record in show_roles
    loop
        select count(*) into actors_count
        from "Direction"
        where "id_role" = role_record."id_role";
    
        if role_record."is_main_role" = 0 and actors_count < 1 or role_record."is_main_role" = 1 and actors_count < 2 then
            raise_application_error(-20020, 'Актерский состав для данного спектакля сформирован не полностью!');
        end if;
    end loop;
    
    select count(*) into performance_amount
    from "Repertoire"
    where "performance_date_repertoire" = :NEW."performance_date_repertoire";
    
    if performance_amount != 0 then
        raise_application_error(-20021, 'На это время уже назначен спектакль!');
    end if;
    
    select "REPERTOIRE_ID_PERFORMANCE_SEQ".nextval into :NEW."id_performance" from dual;
end;
/

CREATE OR REPLACE trigger "REPERTOIRE-DELETE"
    before delete on "Repertoire"
    for each row
declare
    tickets_amount INT;                      
begin
    select count(*) into tickets_amount
    from "Ticket"
    where "id_performance" = :OLD."id_performance";
    
    if tickets_amount != 0 then
        raise_application_error(-20022, 'На это выступление уже выпущены билеты!');
    end if;
end;
/

CREATE OR REPLACE trigger "TICKET-INSERT"
    before insert on "Ticket"
    for each row
declare
    tickets_count INT;
begin
    select count(*) into tickets_count
    from "Ticket" 
    where "id_performance" = :NEW."id_performance" and "seat_number_ticket" = :NEW."seat_number_ticket";
    if tickets_count = 1 then
        raise_application_error(-20023, 'Билет на это место для этого показа уже существует!');
    end if;
    
    select "TICKET_ID_TICKET_SEQ".nextval into :NEW."id_ticket" from dual;
end;
/

CREATE OR REPLACE trigger "TICKET-SUBSCRIPTION-INSERT"
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
        raise_application_error(-20024, 'Этот билет уже добавлен в абонемент!');
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
        raise_application_error(-20025, 'Этот билет имеет неподходящую под абонемент тематику!');
    end if;
end;
/

CREATE OR REPLACE trigger "SUBSCRIPTION-INSERT-UPDATE"
    before insert or update on "Subscription"
    for each row
begin
    if :NEW."id_genre" is not null and :NEW."id_author" is not null
        or :NEW."id_genre" is null and :NEW."id_author" is null then
        raise_application_error(-20026, 'Абонемент может иметь либо конкретного автора, либо конкретный жанр!');
    end if;
    
    if inserting then
        select "SUBSCR_ID_SUBSCR_SEQ".nextval into :NEW."id_subscription" from dual;
    end if;
end;
/

CREATE OR REPLACE trigger "EMPLOYEE-INSERT-UPDATE"
    before insert or update on "Employee"
    for each row
declare
    today_date DATE;
begin
    if updating or inserting then
        if :NEW."hire_date_employee" < :NEW."birthday_employee" then
            raise_application_error(-20027, 'Дата рождения сотрудника позже даты найма!');
        end if;
        
        select CURRENT_DATE into today_date from dual;
        
        if :NEW."birthday_employee" > today_date then
            raise_application_error(-20028, 'Дата рождения сотрудника позже сегоднящнего дня!');
        end if;
    end if;
    
    if inserting then
        select "EMPLOYEE_ID_EMPLOYEE_SEQ".nextval into :NEW."id_employee" from dual;
    end if;
end;
/

CREATE OR REPLACE trigger "EMPLOYEE-DELETE"
    before delete on "Employee"
    for each row
declare
    counter_director INT;
    counter_musician INT;
    counter_actor INT;
begin
    select count(*) into counter_director
    from "Show"
    where ("id_director" = :OLD."id_employee"
        or "id_production_designer" = :OLD."id_employee"
        or "id_conductor" = :OLD."id_employee");
            
    select count(*) into counter_musician
    from "Musician-Show"
    where "id_musician" = :OLD."id_employee";
    
    select count(*) into counter_actor
    from "Direction"
    where "id_actor" = :OLD."id_employee";
        
    if counter_actor + counter_director + counter_musician != 0 then
        raise_application_error(-20029, 'Этот человек занят в спектаклях!');
    end if;
end;
/

CREATE OR REPLACE trigger "CHARACTERISTIC-DELETE"
    before delete on "Characteristic"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Actor-Characteristic"
    where "id_characteristic" = :OLD."id_characteristic";
    
    if cnt != 0 then
        raise_application_error(-20030, 'Данная запись используется!');
    end if;
    
    select count(*) into cnt
    from "Role-Characteristic"
    where "id_characteristic" = :OLD."id_characteristic";
    
    if cnt != 0 then
        raise_application_error(-20031, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "GENDER-DELETE"
    before delete on "Gender"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Employee"
    where "id_gender" = :OLD."id_gender";
    
    if cnt != 0 then
        raise_application_error(-20032, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "MUSICAL_INSTRUMENTS-DELETE"
    before delete on "Musical_instruments"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Musician-Instrument"
    where "id_instrument" = :OLD."id_instrument";
    
    if cnt != 0 then
        raise_application_error(-20033, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "EDUCATION-DELETE"
    before delete on "Education"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Employee"
    where "id_education" = :OLD."id_education";
    
    if cnt != 0 then
        raise_application_error(-20034, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "JOB_TYPES-DELETE"
    before delete on "Job_types"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Employee"
    where "id_job_type" = :OLD."id_job_type";
    
    if cnt != 0 then
        raise_application_error(-20035, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "AGE_CATEGORY-DELETE"
    before delete on "Age_category"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Show"
    where "id_age_category" = :OLD."id_age_category";
    
    if cnt != 0 then
        raise_application_error(-20036, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "GENRE-DELETE"
    before delete on "Genre"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Show"
    where "id_genre" = :OLD."id_genre";
    
    if cnt != 0 then
        raise_application_error(-20037, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "COUNTRY-DELETE"
    before delete on "Country"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Author"
    where "id_country" = :OLD."id_country";
    
    if cnt != 0 then
        raise_application_error(-20038, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "RANK-DELETE"
    before delete on "Rank"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Actor-Rank"
    where "id_rank" = :OLD."id_rank";
    
    if cnt != 0 then
        raise_application_error(-20039, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "COMPETITION-DELETE"
    before delete on "Competition"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Actor-Rank"
    where "id_competition" = :OLD."id_competition";
    
    if cnt != 0 then
        raise_application_error(-20040, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "ROLE-DELETE"
    before delete on "Role"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Direction"
    where "id_role" = :OLD."id_role";
    
    if cnt != 0 then
        raise_application_error(-20041, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "ROLE-CHARACTER-INS-UPD-DEL"
    before insert or update or delete
    on "Role-Characteristic"
    for each row
declare
    cnt     INT;
    id_role INT;
begin
    if inserting or updating then
        id_role := :NEW."id_role";
    else
        id_role := :OLD."id_role";
    end if;
    select count(*)
    into cnt
    from "Direction"
    where "id_role" = id_role;

    if cnt != 0 then
        raise_application_error(-20042, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "AUTHOR-DELETE"
    before delete on "Author"
    for each row
declare
    cnt INT;
begin
    select count(*) into cnt
    from "Show"
    where "id_author" = :OLD."id_author";
    
    if cnt != 0 then
        raise_application_error(-20043, 'Данная запись используется!');
    end if;
end;
/

CREATE OR REPLACE trigger "AUTHOR-UPDATE"
    before update on "Author"
    for each row
declare
    cursor show_cur
    is
        select "century_show"
        from "Show"
        where "id_author" = :NEW."id_author";
begin
    for show_rec in show_cur
    loop
        if show_rec."century_show" < :NEW."life_century_author" then
            raise_application_error(-20044, 'Век жизни автора не может быть позже времени постановки его спектаклей!');
        end if;
    end loop;
end;
/

COMMIT;