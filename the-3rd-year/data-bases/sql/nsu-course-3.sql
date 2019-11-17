-------------------------------------------------------------------------№1-------------------------------------------------------------------------
SELECT cntry_id, country_name, COUNT(cntry_id) AS cnt
FROM (SELECT cntry_id, (SELECT country_name
                        FROM countries
                        WHERE country_id = cntry_id) AS country_name
      FROM (SELECT (SELECT country_id
                    FROM locations
                    WHERE location_id = id) AS cntry_id
            FROM (SELECT location_id AS id
                  FROM departments)))
GROUP BY cntry_id, country_name
ORDER BY cntry_id, country_name, cnt;
-------------------------------------------------------------------------№2-------------------------------------------------------------------------
SELECT COUNT(country_name)
FROM (SELECT (SELECT country_name
              FROM countries
              WHERE country_id = cntry_id) AS country_name
      FROM (SELECT (SELECT country_id
                    FROM locations
                    WHERE location_id = loc_id) AS cntry_id
            FROM (SELECT (SELECT location_id
                          FROM departments
                          WHERE department_id = dep_id ) AS loc_id
                  FROM (SELECT department_id AS dep_id
                        FROM employees
                        WHERE employee_id IN (SELECT manager_id AS id
                                              FROM employees
                                              GROUP BY manager_id
                                              HAVING manager_id IS NOT NULL)))))
WHERE country_name = 'United Kingdom'
GROUP BY country_name;
-------------------------------------------------------------------------№3-------------------------------------------------------------------------
SELECT COUNT(reg_name)
FROM (SELECT (SELECT region_name
              FROM regions
              WHERE region_id = reg_id) as reg_name
      FROM (SELECT (SELECT region_id
                    FROM countries
                    WHERE country_id = cntry_id) AS reg_id
            FROM (SELECT (SELECT country_id
                          FROM locations
                          WHERE location_id = loc_id) as cntry_id
                  FROM (SELECT (SELECT location_id
                                FROM departments
                                WHERE department_id = dep_id) AS loc_id
                        FROM (SELECT department_id AS dep_id
                              FROM employees)))))
WHERE reg_name = 'Europe' GROUP BY reg_name;
-------------------------------------------------------------------------№4-------------------------------------------------------------------------
SELECT AVG(salary)
FROM employees
WHERE employee_id IN (SELECT employee_id
                      FROM (SELECT employee_id, (SELECT region_name
                                                 FROM regions
                                                 WHERE region_id = reg_id) as reg_name
                            FROM (SELECT employee_id, (SELECT region_id
                                                       FROM countries
                                                       WHERE country_id = cntry_id) AS reg_id
                                  FROM (SELECT employee_id, (SELECT country_id
                                                             FROM locations
                                                             WHERE location_id = loc_id) as cntry_id
                                        FROM (SELECT employee_id, (SELECT location_id
                                                                   FROM departments
                                                                   WHERE department_id = dep_id) AS loc_id
                                              FROM (SELECT employee_id, department_id AS dep_id
                                                    FROM employees)))))
                      WHERE reg_name = 'Europe');
-------------------------------------------------------------------------№5-------------------------------------------------------------------------
SELECT SUM(salary)
FROM employees
WHERE employee_id IN (SELECT employee_id
                      FROM (SELECT employee_id, (SELECT country_name
                                                 FROM countries
                                                 WHERE country_id = cntry_id) AS cntry_name
                            FROM (SELECT employee_id, (SELECT country_id
                                                       FROM locations
                                                       WHERE location_id = loc_id) as cntry_id
                                  FROM (SELECT employee_id, (SELECT location_id
                                                             FROM departments
                                                             WHERE department_id = dep_id) AS loc_id
                                        FROM (SELECT employee_id, department_id AS dep_id
                                              FROM employees))))
                      WHERE cntry_name LIKE 'United Kingdom');
-------------------------------------------------------------------------№6-------------------------------------------------------------------------
SELECT *
FROM (SELECT dep_id, dep_name, COUNT(dep_id)
      FROM (SELECT dep_id, (SELECT department_name
                            FROM departments
                            WHERE department_id = dep_id) AS dep_name
            FROM (SELECT department_id as dep_id
                  FROM employees))
      GROUP BY dep_id, dep_name
      ORDER BY COUNT(dep_id) DESC)
WHERE rownum = 1;
-------------------------------------------------------------------------№7-------------------------------------------------------------------------
SELECT street_address FROM locations WHERE city LIKE '%Tokyo%'
-------------------------------------------------------------------------№8-------------------------------------------------------------------------
SELECT DISTINCT department_name, job_title FROM emp_details_view
-------------------------------------------------------------------------№9-------------------------------------------------------------------------
SELECT department_id, COUNT(department_id)
FROM (SELECT department_id
      FROM ((SELECT manager_id as m_id, COUNT(employee_id) AS emp_count
             FROM emp_details_view GROUP BY manager_id) manager_emp_count)
      INNER JOIN emp_details_view ON emp_details_view.employee_id = manager_emp_count.m_id)
GROUP BY department_id ORDER BY department_id
-------------------------------------------------------------------------№10------------------------------------------------------------------------
SELECT dep_id, dep_name, COUNT(dep_id)
      FROM (SELECT dep_id, (SELECT department_name
                            FROM departments
                            WHERE department_id = dep_id) AS dep_name
            FROM (SELECT department_id as dep_id
                  FROM employees))
WHERE dep_id IS NOT NULL
GROUP BY dep_id, dep_name
ORDER BY dep_id, dep_name, COUNT(dep_id)
-------------------------------------------------------------------------№11------------------------------------------------------------------------
SELECT country_id, country_name, COUNT(department_id)
FROM departments
INNER JOIN locations ON departments.location_id = locations.location_id
INNER JOIN countries ON locations.country_id = countries.country_id
GROUP BY country_name, country_id
ORDER BY country_id, country_name
-------------------------------------------------------------------------№12------------------------------------------------------------------------
SELECT DISTINCT job_title
FROM emp_details_view
WHERE country_name LIKE 'United States of America'
ORDER BY job_title
-------------------------------------------------------------------------№13------------------------------------------------------------------------
SELECT job_id, COUNT(job_id)
FROM emp_details_view
GROUP BY job_id
ORDER BY job_id
-------------------------------------------------------------------------№14------------------------------------------------------------------------
SELECT employee_id
FROM (SELECT employee_id, COUNT(employee_id) AS counter
      FROM job_history
      GROUP BY employee_id)
WHERE counter >= 2 ORDER BY employee_id
-------------------------------------------------------------------------№15------------------------------------------------------------------------
SELECT country_id, SUM(salary)
FROM emp_details_view
GROUP BY country_id
ORDER BY country_id
-------------------------------------------------------------------------№16------------------------------------------------------------------------
SELECT DISTINCT job_title
FROM emp_details_view
WHERE region_name LIKE 'Americas'
ORDER BY job_title
