CREATE OR REPLACE procedure show_info(from_date_show IN DATE,
                                      to_date_show IN DATE,
                                      first_time_show IN INT,
                                      id_show IN INT,
                                      status IN INT,
                                      from_century_show IN INT,
                                      to_century_show IN INT,
                                      id_conductor IN INT,
                                      id_production_designer IN INT,
                                      id_director IN INT,
                                      id_genre IN INT,
                                      id_age_category IN INT,
                                      id_author IN INT,
                                      id_country IN INT,
                                      show_cur OUT SYS_REFCURSOR)
    is
begin
    open show_cur for
        select "id_show",
               "name_show"                                                               as "Название",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_director")                                     as "Режиссер-постановщик",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_conductor")                                    as "Диpижеp-постановщик",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_production_designer")                          as "Художник-постановщик",
               ("name_author" || ' ' || "surname_author" || ' ' || "middle_name_author") as "Автор",
               "name_genre"                                                              as "Жанр",
               "century_show"                                                            as "Век спектакля",
               "premier_date_show"                                                       as "Дата премьеры"
        from (("Show" inner join "Author" using ("id_author"))
                 inner join "Genre" using ("id_genre"))
        where "id_show" in (select "id_show"
                            from (("Repertoire" inner join "Show" using ("id_show"))
                                     inner join "Author" using ("id_author"))
                            where ("performance_date_repertoire" <= NVL(to_date_show, "performance_date_repertoire")
                                and "performance_date_repertoire" >= NVL(from_date_show, "performance_date_repertoire")
                                and ((status = 0) or "performance_date_repertoire" <= (select CURRENT_DATE from dual)))
                              and "id_show" = NVL(id_show, "id_show")
                              and "century_show" <= NVL(to_century_show, "century_show")
                              and "century_show" >= NVL(from_century_show, "century_show")
                              and "id_conductor" = NVL(id_conductor, "id_conductor")
                              and "id_production_designer" = NVL(id_production_designer, "id_production_designer")
                              and "id_director" = NVL(id_director, "id_director")
                              and "id_genre" = NVL(id_genre, "id_genre")
                              and "id_age_category" = NVL(id_age_category, "id_age_category")
                              and "id_author" = NVL(id_author, "id_author")
                              and "id_country" = NVL(id_country, "id_country")
                            group by "id_show"
                            having first_time_show = 0
                                or (first_time_show != 0 and count(*) = 1));
end;
/

CREATE OR REPLACE procedure author_info(name_author IN VARCHAR2,
                                        surname_author IN VARCHAR2,
                                        middle_name_author IN VARCHAR2,
                                        from_century_life IN INT,
                                        to_century_life IN INT,
                                        id_country IN INT,
                                        author_cur OUT SYS_REFCURSOR)
    is
begin
    open author_cur for
        select "id_author",
               "name_author"         as "Имя",
               "surname_author"      as "Фаммлия",
               "middle_name_author"  as "Отчество",
               "life_century_author" as "Век жизни",
               "name_country"        as "Страна"
        from ("Author"
                 inner join "Country" using ("id_country"))
        where ("name_author" like '%' || name_author || '%' or name_author is null)
          and ("surname_author" like '%' || surname_author || '%' or surname_author is null)
          and ("middle_name_author" like '%' || middle_name_author || '%' or middle_name_author is null)
          and "life_century_author" >= NVL(from_century_life, "life_century_author")
          and "life_century_author" <= NVL(to_century_life, "life_century_author")
          and "id_country" = NVL(id_country, "id_country");
end;
/

CREATE OR REPLACE procedure author_insert(name IN "Author"."name_author"%TYPE,
                                          surname IN "Author"."surname_author"%TYPE,
                                          middle_name IN "Author"."middle_name_author"%TYPE,
                                          century IN "Author"."life_century_author"%TYPE,
                                          country IN "Author"."id_country"%TYPE)
    is
begin
    INSERT INTO "Author"("id_author", "name_author", "surname_author", "middle_name_author", "life_century_author",
                         "id_country")
    VALUES (0, name, surname, middle_name, century, country);

    COMMIT;
end;
/

CREATE OR REPLACE procedure author_update(name IN "Author"."name_author"%TYPE,
                                          surname IN "Author"."surname_author"%TYPE,
                                          middle_name IN "Author"."middle_name_author"%TYPE,
                                          century IN "Author"."life_century_author"%TYPE,
                                          country IN "Author"."id_country"%TYPE,
                                          id_author IN "Author"."id_author"%TYPE)
    is
begin
    UPDATE "Author"
    SET "name_author"         = name,
        "surname_author"      = surname,
        "middle_name_author"  = middle_name,
        "life_century_author" = century,
        "id_country"          = country
    WHERE "id_author" = id_author;

    COMMIT;
end;
/

CREATE OR REPLACE procedure author_delete(
    id_author IN "Author"."id_author"%TYPE)
    is
begin
    DELETE FROM "Author" WHERE "id_author" = id_author;

    COMMIT;
end;
/

CREATE OR REPLACE procedure author_shows(from_date_show IN "Repertoire"."performance_date_repertoire"%TYPE,
                                         to_date_show IN "Repertoire"."performance_date_repertoire"%TYPE,
                                         id_genre IN "Genre"."id_genre"%TYPE,
                                         id_author IN "Author"."id_author"%TYPE,
                                         show_cur OUT SYS_REFCURSOR)
    is
begin
    open show_cur for
        select "id_show",
               "name_show"                                                               as "Название",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_director")                                     as "Режиссер-постановщик",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_conductor")                                    as "Диpижеp-постановщик",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_production_designer")                          as "Художник-постановщик",
               ("name_author" || ' ' || "surname_author" || ' ' || "middle_name_author") as "автор",
               "name_genre"                                                              as "Жанр",
               "century_show"                                                            as "Век спектакля",
               "premier_date_show"                                                       as "Дата премьеры"
        from (("Show" inner join "Author" using ("id_author"))
                 inner join "Genre" using ("id_genre"))
        where "id_show" in (select "id_show"
                            from (("Repertoire" inner join "Show" using ("id_show"))
                                     inner join "Author" using ("id_author"))
                            where ("performance_date_repertoire" <= NVL(TO_DATE(to_date_show, 'yyyy/mm/dd'),
                                                                        "performance_date_repertoire")
                                and "performance_date_repertoire" >= NVL(TO_DATE(from_date_show, 'yyyy/mm/dd'),
                                                                         "performance_date_repertoire")
                                and "id_genre" = NVL(id_genre, "id_genre")
                                and "id_author" = NVL(id_author, "id_author"))
                            group by "id_show");

end;
/

CREATE OR REPLACE procedure actor_role_show_info(id_show IN INT, list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as "Актер",
               "name_role"                                                                     as "Роль",
               "is_understudy_direction"                                                       as "Дублер?"
        from ("Direction" inner join "Employee" on "id_actor" = "id_employee")
                 inner join "Role" using ("id_role")
        where "id_show" = id_show;
end;
/

CREATE OR REPLACE procedure musician_show_info(id_show IN INT, list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as "Музыкант",
               "name_instrument"                                                               as "Инструмент"
        from (("Musician-Show" inner join "Employee" on "id_musician" = "id_employee")
            inner join "Musician-Instrument" using ("id_musician"))
                 inner join "Musical_instruments" using ("id_instrument")
        where "id_show" = id_show;
end;
/

CREATE OR REPLACE procedure get_genders_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_gender", "name_gender"
        from "Gender";
end;
/

CREATE OR REPLACE procedure get_education_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_education", "name_education"
        from "Education";
end;
/

CREATE OR REPLACE procedure get_job_types_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_job_type", "name_job_type"
        from "Job_types";
end;
/

CREATE OR REPLACE procedure employee_info(name IN "Employee"."name_employee"%TYPE,
                                          surname IN "Employee"."surname_employee"%TYPE,
                                          middle_name IN "Employee"."middle_name_employee"%TYPE,
                                          id_gender IN "Employee"."id_gender"%TYPE,
                                          birthday_from IN "Employee"."birthday_employee"%TYPE,
                                          birthday_to IN "Employee"."birthday_employee"%TYPE,
                                          age_from IN INT,
                                          age_to IN INT,
                                          experience_from IN INT,
                                          experience_to IN INT,
                                          children_amount_from IN "Employee"."children_amount_employee"%TYPE,
                                          children_amount_to IN "Employee"."children_amount_employee"%TYPE,
                                          salary_from IN "Employee"."salary_employee"%TYPE,
                                          salary_to IN "Employee"."salary_employee"%TYPE,
                                          id_education IN "Employee"."id_education"%TYPE,
                                          id_job_type IN "Employee"."id_job_type"%TYPE,
                                          employee_cur OUT SYS_REFCURSOR)
    is
begin
    open employee_cur for
        select "id_employee",
               "name_employee"            as "Имя",
               "surname_employee"         as "Фамилия",
               "middle_name_employee"     as "Отчество",
               "name_gender"              as "Гендер",
               "birthday_employee"        as "Дата рождения",
               "hire_date_employee"       as "Дата найма",
               "children_amount_employee" as "Кол-во детей",
               "salary_employee"          as "Зарплата(руб.)",
               "name_education"           as "Образование",
               "name_job_type"            as "Должность"
        from ((("Employee" inner join "Education" using ("id_education"))
            inner join "Gender" using ("id_gender"))
                 inner join "Job_types" using ("id_job_type"))
        where ("name_employee" like '%' || name || '%' or name is null)
          and ("surname_employee" like '%' || surname || '%' or surname is null)
          and ("middle_name_employee" like '%' || middle_name || '%' or middle_name is null)
          and "id_gender" = NVL(id_gender, "id_gender")
          and "birthday_employee" >= NVL(birthday_from, "birthday_employee")
          and "birthday_employee" <= NVL(birthday_to, "birthday_employee")
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "hire_date_employee")) >= NVL(experience_from,
                                                                                TRUNC(((SELECT SYSDATE FROM DUAL) - "hire_date_employee")))
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "hire_date_employee")) <= NVL(experience_to,
                                                                                TRUNC(((SELECT SYSDATE FROM DUAL) - "hire_date_employee")))
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")) >= NVL(age_from,
                                                                               TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")))
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")) <= NVL(age_to,
                                                                               TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")))
          and "children_amount_employee" >= NVL(children_amount_from, "children_amount_employee")
          and "children_amount_employee" <= NVL(children_amount_to, "children_amount_employee")
          and "salary_employee" >= NVL(salary_from, "salary_employee")
          and "salary_employee" <= NVL(salary_to, "salary_employee")
          and "id_education" = NVL(id_education, "id_education")
          and (id_job_type is null or is_sub_job_type(id_job_type, "id_job_type") = 1);
end;
/

CREATE OR REPLACE function is_sub_job_type(id_parent_job_type IN "Job_types"."id_parent_job_type"%TYPE,
                                           id_job_type IN "Job_types"."id_job_type"%TYPE)
    return int
    is
    cursor job_cur is
        select "id_job_type"
        from "Job_types"
        where "id_parent_job_type" = id_parent_job_type;
    result int;
begin
    if id_parent_job_type = id_job_type then
        return 1;
    end if;

    for job_rec in job_cur
        loop
            if job_rec."id_job_type" = id_job_type then
                return 1;
            else
                result := is_sub_job_type(job_rec."id_job_type", id_job_type);
                if result = 1 then
                    return 1;
                end if;
            end if;
        end loop;

    return 0;
end;
/

CREATE OR REPLACE procedure employee_insert(name IN "Employee"."name_employee"%TYPE,
                                            surname IN "Employee"."surname_employee"%TYPE,
                                            middle_name IN "Employee"."middle_name_employee"%TYPE,
                                            id_gender IN "Employee"."id_gender"%TYPE,
                                            birthday IN "Employee"."birthday_employee"%TYPE,
                                            hire_date IN "Employee"."hire_date_employee"%TYPE,
                                            children_amount IN "Employee"."children_amount_employee"%TYPE,
                                            salary IN "Employee"."salary_employee"%TYPE,
                                            id_education IN "Employee"."id_education"%TYPE,
                                            id_job_type IN "Employee"."id_job_type"%TYPE)
    is
begin
    INSERT INTO "Employee"
    VALUES (0, name, surname, middle_name, id_gender, birthday, hire_date, children_amount, salary,
            id_education, id_job_type);

    COMMIT;
end;
/

CREATE OR REPLACE procedure employee_update(name IN "Employee"."name_employee"%TYPE,
                                            surname IN "Employee"."surname_employee"%TYPE,
                                            middle_name IN "Employee"."middle_name_employee"%TYPE,
                                            id_gender IN "Employee"."id_gender"%TYPE,
                                            birthday IN "Employee"."birthday_employee"%TYPE,
                                            hire_date IN "Employee"."hire_date_employee"%TYPE,
                                            children_amount IN "Employee"."children_amount_employee"%TYPE,
                                            salary IN "Employee"."salary_employee"%TYPE,
                                            id_education IN "Employee"."id_education"%TYPE,
                                            id_job_type IN "Employee"."id_job_type"%TYPE,
                                            id_employee IN "Employee"."id_employee"%TYPE)
    is
begin
    UPDATE "Employee"
    SET "name_employee"            = name,
        "surname_employee"         = surname,
        "middle_name_employee"     = middle_name,
        "id_gender"                = id_gender,
        "birthday_employee"        = birthday,
        "hire_date_employee"       = hire_date,
        "children_amount_employee" = children_amount,
        "salary_employee"          = salary,
        "id_education"             = id_education,
        "id_job_type"              = id_job_type
    WHERE "id_employee" = id_employee;

    COMMIT;
end;
/

CREATE OR REPLACE procedure employee_delete(
    id_employee IN "Employee"."id_employee"%TYPE)
    is
begin
    DELETE FROM "Employee" WHERE "id_employee" = id_employee;

    COMMIT;
end;
/

CREATE OR REPLACE procedure get_shows_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_show", "name_show"
        from "Show";
end;
/

CREATE OR REPLACE procedure get_genres_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_genre", "name_genre"
        from "Genre";
end;
/

CREATE OR REPLACE procedure get_age_categories_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_age_category", "name_age_category"
        from "Age_category";
end;
/

CREATE OR REPLACE procedure get_authors_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_author", "name_author"
        from "Author";
end;
/

CREATE OR REPLACE procedure get_countries_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_country", "name_country"
        from "Country";
end;
/

CREATE OR REPLACE procedure get_employee_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_employee", ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as name
        from ("Employee"
                 inner join "Job_types" using ("id_job_type"))
        where "name_job_type" like 'pежиссеp-постановщик';
end;
/

CREATE OR REPLACE procedure get_conductors_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_employee", ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as name
        from ("Employee"
                 inner join "Job_types" using ("id_job_type"))
        where "name_job_type" like 'диpижеp-постановщик';
end;
/

CREATE OR REPLACE procedure get_designers_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_employee", ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as name
        from ("Employee"
                 inner join "Job_types" using ("id_job_type"))
        where "name_job_type" like 'художник-постановщик';
end;
/

create or replace procedure get_directors_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_employee", ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as name
        from ("Employee"
                 inner join "Job_types" using ("id_job_type"))
        where "name_job_type" like 'pежиссеp-постановщик';
end;
/

CREATE OR REPLACE procedure get_actors_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_employee", ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as name
        from ("Employee"
                 inner join "Job_types" using ("id_job_type"))
        where "name_job_type" like 'актер';
end;
/

CREATE OR REPLACE procedure get_rank_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_rank", "name_rank"
        from "Rank";
end;
/

CREATE OR REPLACE procedure get_competition_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_competition", "name_competition"
        from "Competition";
end;
/

CREATE OR REPLACE procedure actor_rank_info(id_actor IN "Employee"."id_employee"%TYPE,
                                            id_gender IN "Employee"."id_gender"%TYPE,
                                            age_from IN INT,
                                            age_to IN INT,
                                            date_from IN "Actor-Rank"."obtaining_date_actor_rank"%TYPE,
                                            date_to IN "Actor-Rank"."obtaining_date_actor_rank"%TYPE,
                                            id_rank IN "Rank"."id_rank"%TYPE,
                                            id_competition IN "Competition"."id_competition"%TYPE,
                                            actor_rank_cur OUT SYS_REFCURSOR)
    is
begin
    open actor_rank_cur for
        select "name_rank"                                                                     as "Звание",
               "name_competition"                                                              as "Конкурс",
               "obtaining_date_actor_rank"                                                     as "Дата получения",
               ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as "Актер"
        from ("Actor-Rank"
            inner join "Rank" using ("id_rank"))
                 inner join "Competition" using ("id_competition")
                 inner join "Employee" on "id_actor" = "id_employee"
        where "id_actor" = NVL(id_actor, "id_actor")
          and "id_gender" = NVL(id_gender, "id_gender")
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")) >= NVL(age_from,
                                                                               TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")))
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")) <= NVL(age_to,
                                                                               TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")))
          and "obtaining_date_actor_rank" >= NVL(date_from, "obtaining_date_actor_rank")
          and "obtaining_date_actor_rank" <= NVL(date_to, "obtaining_date_actor_rank")
          and "id_rank" = NVL(id_rank, "id_rank")
          and "id_competition" = NVL(id_competition, "id_competition");

end;
/

CREATE OR REPLACE procedure actor_rank_list(id_actor IN "Actor-Rank"."id_actor"%TYPE,
                                            list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_rank",
               "name_rank"                 as "Звание",
               "name_competition"          as "Конкурс",
               "obtaining_date_actor_rank" as "Дата получения"
        from ("Actor-Rank" inner join "Rank" using ("id_rank"))
                 inner join "Competition" using ("id_competition")
        where "id_actor" = id_actor;
end;
/

CREATE OR REPLACE procedure actor_rank_insert(id_actor IN "Actor-Rank"."id_actor"%TYPE,
                                              id_rank IN "Actor-Rank"."id_rank"%TYPE,
                                              obtaining_date IN "Actor-Rank"."obtaining_date_actor_rank"%TYPE,
                                              id_competition IN "Actor-Rank"."id_competition"%TYPE)
    is
begin
    INSERT INTO "Actor-Rank" VALUES (id_actor, id_rank, obtaining_date, id_competition);

    COMMIT;
end;
/

CREATE OR REPLACE procedure actor_rank_delete(id_actor IN "Actor-Rank"."id_actor"%TYPE,
                                              id_rank IN "Actor-Rank"."id_rank"%TYPE)
    is
begin
    DELETE
    FROM "Actor-Rank"
    WHERE "id_actor" = id_actor
      and "id_rank" = id_rank;

    COMMIT;
end;
/

CREATE OR REPLACE procedure actor_characteristic_insert(id_actor IN "Actor-Characteristic"."id_actor"%TYPE,
                                                        id_characteristic IN "Actor-Characteristic"."id_characteristic"%TYPE,
                                                        value IN "Actor-Characteristic"."value_actor_characteristic"%TYPE)
    is
begin
    INSERT INTO "Actor-Characteristic" VALUES (id_actor, id_characteristic, value);

    COMMIT;
end;
/

CREATE OR REPLACE procedure actor_characteristic_delete(id_actor IN "Actor-Characteristic"."id_actor"%TYPE,
                                                        id_characteristic IN "Actor-Characteristic"."id_characteristic"%TYPE)
    is
begin
    DELETE
    FROM "Actor-Characteristic"
    WHERE "id_actor" = id_actor
      and "id_characteristic" = id_characteristic;

    COMMIT;
end;
/

CREATE OR REPLACE procedure actor_characteristic_list(id_actor IN "Actor-Characteristic"."id_actor"%TYPE,
                                                      list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_characteristic",
               "type_characteristic"        as "Характеристика",
               "value_actor_characteristic" as "Значение"
        from "Actor-Characteristic"
                 inner join "Characteristic" using ("id_characteristic")
        where "id_actor" = id_actor;
end;
/

CREATE OR REPLACE procedure actor_roles_info(id_actor IN "Employee"."id_employee"%TYPE,
                                             id_gender IN "Employee"."id_gender"%TYPE,
                                             age_from IN INT,
                                             age_to IN INT,
                                             date_from IN "Repertoire"."performance_date_repertoire"%TYPE,
                                             date_to IN "Repertoire"."performance_date_repertoire"%TYPE,
                                             id_genre IN "Genre"."id_genre"%TYPE,
                                             id_age_category IN "Age_category"."id_age_category"%TYPE,
                                             id_director IN "Employee"."id_employee"%TYPE,
                                             actor_roles_cur OUT SYS_REFCURSOR)
    is
begin
    open actor_roles_cur for
        select ("name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee") as "Актер",
               "name_role"                                                                     as "Роль",
               "name_show"                                                                     as "Спектакль",
               "name_genre"                                                                    as "Жанр",
               "name_age_category"                                                             as "Возрастная категория",
               (select "name_employee" || ' ' || "surname_employee" || ' ' || "middle_name_employee"
                from "Employee"
                where "id_employee" = "id_director")                                           as "Режиссер-постановщик"
        from (((("Direction"
            inner join "Employee" on "id_actor" = "id_employee")
            inner join "Role" using ("id_role"))
            inner join "Show" using ("id_show"))
            inner join "Genre" using ("id_genre"))
                 inner join "Age_category" using ("id_age_category")
        where "id_actor" = NVL(id_actor, "id_actor")
          and "id_gender" = NVL(id_gender, "id_gender")
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")) >= NVL(age_from,
                                                                               TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")))
          and TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")) <= NVL(age_to,
                                                                               TRUNC(((SELECT SYSDATE FROM DUAL) - "birthday_employee")))
          and "id_genre" = NVL(id_genre, "id_genre")
          and "id_age_category" = NVL(id_age_category, "id_age_category")
          and "id_director" = NVL(id_director, "id_director")
          and "id_show" in (select "id_show"
                            from "Repertoire"
                            where "performance_date_repertoire" >= NVL(date_from, "performance_date_repertoire")
                              and "performance_date_repertoire" <= NVL(date_to, "performance_date_repertoire"));

end;
/

CREATE OR REPLACE procedure get_characteristics_list(
    list OUT SYS_REFCURSOR)
    is
begin
    open list for
        select "id_characteristic", "type_characteristic"
        from "Characteristic";
end;
/