-------------------------------------------------------------------------№1-------------------------------------------------------------------------
SELECT employee_id
FROM (SELECT employee_id, TRUNC(((SELECT SYSDATE FROM DUAL) - hire_date)) AS exp
      FROM (SELECT manager_id AS m_id
            FROM emp_details_view
            GROUP BY manager_id) managers
      INNER JOIN employees ON managers.m_id = employees.employee_id
      ORDER BY exp)
WHERE exp = (SELECT MIN(TRUNC(((SELECT SYSDATE FROM DUAL) - hire_date))) AS exp
             FROM (SELECT manager_id AS m_id
                   FROM emp_details_view
                   GROUP BY manager_id) managers
             INNER JOIN employees ON managers.m_id = employees.employee_id)
ORDER BY employee_id
-------------------------------------------------------------------------№2-------------------------------------------------------------------------
SELECT employee_id, last_name, salary, department_id
FROM emp_details_view
INNER JOIN (SELECT department_id AS d_id, AVG(salary) AS avg_salary
            FROM emp_details_view
            GROUP BY department_id) dep_avg_sal ON emp_details_view.department_id = dep_avg_sal.d_id
WHERE salary > avg_salary
ORDER BY department_id, salary, last_name
-------------------------------------------------------------------------№3-------------------------------------------------------------------------
SELECT employee_id, last_name, salary, department_id
FROM emp_details_view
INNER JOIN (SELECT department_id AS d_id, MIN(salary) AS min_salary
            FROM emp_details_view
            GROUP BY department_id) dep_min_sal ON emp_details_view.department_id = dep_min_sal.d_id
WHERE salary = min_salary
ORDER BY department_id, salary, last_name
-------------------------------------------------------------------------№4-------------------------------------------------------------------------
SELECT last_name, first_name
FROM emp_details_view
WHERE region_name LIKE 'Americas' AND salary > (SELECT salary
                                                FROM (SELECT salary
                                                      FROM (SELECT manager_id as m_id, COUNT(manager_id) AS cnt_m_id
                                                            FROM emp_details_view
                                                            GROUP BY manager_id
                                                            ORDER BY cnt_m_id DESC) m_info
                                                      INNER JOIN emp_details_view ON m_info.m_id = emp_details_view.employee_id
                                                      WHERE region_name LIKE 'Europe')
                                                WHERE rownum = 1)
ORDER BY last_name, first_name
-------------------------------------------------------------------------№5-------------------------------------------------------------------------
SELECT department_id, SUM(sum_salary) AS sum_salary, SUM(frac) AS frac
FROM (SELECT department_id, SUM(salary) AS sum_salary, CEIL((SUM(salary) * 100) / (SELECT SUM(salary) FROM employees)) AS frac
      FROM employees
      GROUP BY department_id
      UNION SELECT department_id, 0, 0
            FROM departments)
GROUP BY department_id
ORDER BY frac
-------------------------------------------------------------------------№6-------------------------------------------------------------------------
SELECT department_id, SUM(avg_salary) AS avg_salary, SUM(frac) AS frac
FROM (SELECT department_id, AVG(salary) AS avg_salary, CEIL((AVG(salary) * 100) / (SELECT AVG(salary) FROM employees)) AS frac
      FROM employees
      GROUP BY department_id
      UNION SELECT department_id, 0, 0
            FROM departments)
GROUP BY department_id
ORDER BY department_id, avg_salary, frac
-------------------------------------------------------------------------№7-------------------------------------------------------------------------
SELECT first_name, COUNT(first_name) AS cnt
FROM employees
GROUP BY first_name
HAVING COUNT(first_name) > 1
ORDER BY cnt
-------------------------------------------------------------------------№8-------------------------------------------------------------------------
SELECT dep_id, COUNT(*) - COUNT(third_lvl_manager) AS first_lvl, COUNT(third_lvl_manager) - COUNT(second_lvl_manager) AS second_lvl, COUNT(second_lvl_manager) - COUNT(first_lvl_manager) AS third_lvl
FROM (SELECT dep_id, fourth_lvl_manager, third_lvl_manager, second_lvl_manager, manager_id AS first_lvl_manager
      FROM (SELECT dep_id, fourth_lvl_manager, third_lvl_manager, manager_id AS second_lvl_manager
            FROM (SELECT department_id AS dep_id, employee_id AS fourth_lvl_manager, manager_id AS third_lvl_manager
                  FROM emp_details_view WHERE region_name LIKE 'Americas')
            LEFT JOIN employees ON third_lvl_manager = employee_id)
      LEFT JOIN employees ON second_lvl_manager=employee_id)
GROUP BY dep_id
ORDER BY dep_id
-------------------------------------------------------------------------№9-------------------------------------------------------------------------
SELECT department_name
FROM emp_details_view
GROUP BY department_name
HAVING MIN(salary) > (SELECT AVG(SALARY)
                      FROM emp_details_view
                      WHERE region_name = 'Americas')
ORDER BY department_name
-------------------------------------------------------------------------№10-------------------------------------------------------------------------
Что если таких отделов много? Считаю, что один.
SELECT SUM(salary)
FROM emp_details_view
WHERE department_id = (SELECT department_id
                       FROM (SELECT department_id, COUNT(*) AS cnt
                             FROM emp_details_view
                             GROUP BY department_id
                             ORDER BY cnt)
                       WHERE rownum = 1)
-------------------------------------------------------------------------№11-------------------------------------------------------------------------
SELECT last_name, COUNT(*)
FROM employees
GROUP BY last_name
HAVING COUNT(*) > 1
ORDER BY last_name
-------------------------------------------------------------------------№12-------------------------------------------------------------------------
SELECT country_id, employee_id
FROM (SELECT country_id, MAX(salary) AS max_salary
      FROM emp_details_view
      WHERE country_id IN (SELECT country_id AS count
                           FROM departments D
                           INNER JOIN locations L ON D.location_id = L.location_id
                           GROUP BY country_id HAVING COUNT(department_id) = (SELECT MAX(count)
                                                                              FROM (SELECT country_id, COUNT(department_id) AS count
                                                                                    FROM departments D
                                                                                    INNER JOIN locations L ON D.location_id = L.location_id
                                                                                    GROUP BY country_id)))
      GROUP BY country_id)
INNER JOIN emp_details_view USING (country_id)
WHERE salary = max_salary
ORDER BY country_id
-------------------------------------------------------------------------№13-------------------------------------------------------------------------
А если таких работников в одной стране несколько? Решение пытается учесть этот нюанс.
SELECT manager_id
FROM emp_details_view
WHERE country_id IN (SELECT country_id
                     FROM departments
                     INNER JOIN locations USING(location_id)
                     GROUP BY country_id
                     HAVING COUNT(*) = (SELECT MAX(cnt)
                                        FROM (SELECT COUNT(*) AS cnt
                                              FROM departments
                                              INNER JOIN locations USING(location_id)
                                              GROUP BY country_id)))
GROUP BY country_name, manager_id
HAVING COUNT(*) = (SELECT MAX(mng_counter)
                   FROM (SELECT COUNT(*) AS mng_counter
                         FROM emp_details_view
                         WHERE country_id IN (SELECT country_id
                                              FROM departments
                                              INNER JOIN locations USING(location_id)
                                              GROUP BY country_id
                                              HAVING COUNT(*) = (SELECT MAX(cnt)
                                                                 FROM (SELECT COUNT(*) AS cnt
                                                                       FROM departments
                                                                       INNER JOIN locations USING(location_id)
                                                                       GROUP BY country_id)))
                         GROUP BY country_name, manager_id))
-------------------------------------------------------------------------№14-------------------------------------------------------------------------
А если таких работников, у которого в подчинении больше всего человек, в одной стране несколько? Решение пытается учесть этот нюанс.
SELECT manager_id
FROM emp_details_view
WHERE country_id IN (SELECT country_id
                     FROM (SELECT employee_id, TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) AS time, country_id
                           FROM (employees
                           INNER JOIN departments USING(department_id))
                           INNER JOIN locations USING(location_id)
                           WHERE TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) = (SELECT MIN(TRUNC((SELECT SYSDATE FROM DUAL) - hire_date))
                                                                                  FROM (employees
                                                                                  INNER JOIN departments USING(department_id))
                                                                                  INNER JOIN locations USING(location_id)))
                     GROUP BY country_id)
GROUP BY manager_id
HAVING COUNT(*) = (SELECT MAX(cnt)
                   FROM (SELECT COUNT(*) AS cnt
                         FROM emp_details_view
                         WHERE country_id IN (SELECT country_id
                                              FROM (SELECT employee_id, TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) AS time, country_id
                                                    FROM (employees
                                                    INNER JOIN departments USING(department_id))
                                                    INNER JOIN locations USING(location_id)
                                                    WHERE TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) = (SELECT MIN(TRUNC((SELECT SYSDATE FROM DUAL) - hire_date))
                                                                                                           FROM (employees
                                                                                                           INNER JOIN departments USING(department_id))
                                                                                                           INNER JOIN locations USING(location_id)))
                                              GROUP BY country_id)
                         GROUP BY manager_id))
ORDER BY manager_id
-------------------------------------------------------------------------№15-------------------------------------------------------------------------
А если таких работников, у которого в подчинении человек с наибольшей зарплатой, в одной стране несколько? Решение пытается учесть этот нюанс.
SELECT manager_id
FROM emp_details_view
WHERE country_id IN (SELECT country_id
                     FROM (SELECT employee_id, TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) AS time, country_id
                           FROM (employees
                           INNER JOIN departments USING(department_id))
                           INNER JOIN locations USING(location_id)
                           WHERE TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) = (SELECT MAX(TRUNC((SELECT SYSDATE FROM DUAL) - hire_date))
                                                                                  FROM (employees
                                                                                  INNER JOIN departments USING(department_id))
                                                                                  INNER JOIN locations USING(location_id)))
                     GROUP BY country_id)
GROUP BY manager_id
HAVING MAX(salary) = (SELECT MAX(salary)
                         FROM emp_details_view
                         WHERE country_id IN (SELECT country_id
                                              FROM (SELECT employee_id, TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) AS time, country_id
                                                    FROM (employees
                                                    INNER JOIN departments USING(department_id))
                                                    INNER JOIN locations USING(location_id)
                                                    WHERE TRUNC((SELECT SYSDATE FROM DUAL) - hire_date) = (SELECT MAX(TRUNC((SELECT SYSDATE FROM DUAL) - hire_date))
                                                                                                           FROM (employees
                                                                                                           INNER JOIN departments USING(department_id))
                                                                                                           INNER JOIN locations USING(location_id)))
                                              GROUP BY country_id) AND manager_id IS NOT NULL)
ORDER BY manager_id
-------------------------------------------------------------------------№16-------------------------------------------------------------------------
А что если таких годов несколько? Решение пытается учесть этот нюанс.
SELECT EXTRACT(year FROM hire_date), COUNT(*)
FROM employees
GROUP BY EXTRACT(year FROM hire_date)
HAVING COUNT(*) = (SELECT MAX(cnt)
                   FROM (SELECT COUNT(*) AS cnt
                         FROM employees
                         GROUP BY EXTRACT(year FROM hire_date)))
