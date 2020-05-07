SET SERVEROUTPUT ON;

CREATE OR REPLACE procedure show_info(
    from_date_show IN DATE,
    to_date_show IN DATE,
    first_time_show IN INT,
    name_show IN VARCHAR2,
    is_ongoing_show IN INT,
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
    show_rec "Show"%ROWTYPE;
begin
    open show_cur for
        select *
        from "Show"
        where "id_show" in (select "id_show"
                            from (("Repertoire" inner join "Show" using ("id_show"))
                                    inner join "Author" using ("id_author"))
                            where
                                (is_ongoing_show = 0
                                    and "performance_date_repertoire" <= NVL(to_date_show, "performance_date_repertoire")
                                    and "performance_date_repertoire" >= NVL(from_date_show, "performance_date_repertoire")
                                or (is_ongoing_show != 0 ) and "performance_date_repertoire" >= (select CURRENT_DATE from dual))
                                and "name_show" like NVL('%' || name_show || '%', "name_show")
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
                            having first_time_show = 0 or (first_time_show != 0 and count(*) = 1));

    loop
        fetch show_cur into show_rec;
        exit when show_cur%notfound;
        dbms_output.put_line(show_rec."id_show");
    end loop;
end;

/
EXEC show_info(NULL, NULL, 1, NULL, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);