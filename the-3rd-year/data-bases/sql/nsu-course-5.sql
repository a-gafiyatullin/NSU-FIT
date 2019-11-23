-------------------------------------------------------------------------№1-------------------------------------------------------------------------
SELECT month
FROM (SELECT EXTRACT(month FROM hire_date) AS month
      FROM employees)
GROUP BY month
HAVING COUNT(*) = (SELECT MAX(cnt)
                   FROM (SELECT month, COUNT(*) AS cnt
                         FROM (SELECT EXTRACT(month FROM hire_date) AS month
                               FROM employees) GROUP BY month))
ORDER BY month
-------------------------------------------------------------------------№2-------------------------------------------------------------------------
SELECT (SELECT department_name
        FROM departments
        WHERE department_id = new_dep_id) AS name
FROM (SELECT employee_id, new_dep_id, old_min_salary, job_id
      FROM (SELECT employee_id, department_id AS new_dep_id, min_salary AS old_min_salary
            FROM job_history
            INNER JOIN jobs USING(job_id))
      INNER JOIN employees USING(employee_id))
INNER JOIN jobs USING(job_id)
WHERE max_salary - old_min_salary = (SELECT MAX(max_salary - old_min_salary)
                                     FROM (SELECT employee_id, new_dep_id, old_min_salary, job_id
                                           FROM (SELECT employee_id, department_id AS new_dep_id, min_salary AS old_min_salary
                                                 FROM job_history INNER JOIN jobs USING(job_id))
                                           INNER JOIN employees USING(employee_id))
                                     INNER JOIN jobs USING(job_id))
-------------------------------------------------------------------------№3-------------------------------------------------------------------------
SELECT MONTHS_BETWEEN((SELECT MAX(hire_date) FROM employees), (SELECT MIN(hire_date) FROM employees)) FROM DUAL
-------------------------------------------------------------------------№4-------------------------------------------------------------------------
SELECT DISTINCT last_name, MONTHS_BETWEEN((SELECT MAX(hire_date) FROM employees WHERE employees.last_name = emp.last_name), (SELECT MIN(hire_date) FROM employees WHERE employees.last_name = emp.last_name)) AS months
FROM employees emp
WHERE last_name IN (SELECT last_name FROM employees GROUP BY last_name HAVING COUNT(*) > 1)
ORDER BY months
-------------------------------------------------------------------------№5-------------------------------------------------------------------------
SELECT employee_id
FROM emp_details_view
WHERE country_id IN (SELECT country_id
                     FROM (SELECT country_id, SUM(salary) as sum_salary
                           FROM emp_details_view
                           GROUP BY country_id
                           ORDER BY sum_salary DESC)
                     WHERE rownum <= 2)
ORDER BY employee_id
-------------------------------------------------------------------------№6-------------------------------------------------------------------------
SELECT job_id, SUM(counter)
FROM (SELECT job_id, COUNT(*) AS counter
      FROM employees
      INNER JOIN jobs USING(job_id)
      WHERE salary = min_salary
      GROUP BY job_id
      UNION ALL
      SELECT job_id, 0
      FROM jobs)
GROUP BY job_id ORDER BY job_id
-------------------------------------------------------------------------№7-------------------------------------------------------------------------
SELECT job_id, AVG(salary)
FROM employees
WHERE job_id IN (SELECT job_id
                 FROM jobs
                 WHERE max_salary - min_salary = (SELECT MAX(max_salary - min_salary)
                                                  FROM jobs))
GROUP BY job_id
ORDER BY job_id
-------------------------------------------------------------------------№8-------------------------------------------------------------------------
SELECT job_id, AVG(salary)
FROM employees
WHERE job_id IN (SELECT job_id
                 FROM jobs
                 WHERE max_salary - min_salary = (SELECT MIN(max_salary - min_salary)
                                                  FROM jobs))
GROUP BY job_id
ORDER BY job_id
-------------------------------------------------------------------------№9-------------------------------------------------------------------------
SELECT job_id, SUM(salary)
FROM employees
WHERE job_id IN (SELECT job_id
                 FROM jobs
                 WHERE max_salary - min_salary = (SELECT MIN(max_salary - min_salary)
                                                  FROM jobs))
GROUP BY job_id
ORDER BY job_id
-------------------------------------------------------------------------№10-------------------------------------------------------------------------
SELECT * FROM (SELECT salary_group, COUNT(job_id)
               FROM (SELECT DISTINCT salary_group, job_id
                     FROM (SELECT (CASE WHEN salary BETWEEN 0 AND 5000 THEN '0-5000'
                                        WHEN salary BETWEEN 5001 AND 10000 THEN '5001-10000'
                                        WHEN salary BETWEEN 10001 AND 15000 THEN '10001-15000'
                                        WHEN salary BETWEEN 15001 AND 10000000 THEN '15001-10000000'
                                   END) AS salary_group, employee_id, salary, job_id
                           FROM employees))
               GROUP BY salary_group)
INNER JOIN (SELECT salary_group, COUNT(*) AS count, SUM(salary), AVG(salary)
            FROM (SELECT (CASE WHEN salary BETWEEN 0 AND 5000 THEN '0-5000'
                               WHEN salary BETWEEN 5001 AND 10000 THEN '5001-10000'
                               WHEN salary BETWEEN 10001 AND 15000 THEN '10001-15000'
                               WHEN salary BETWEEN 15001 AND 10000000 THEN '15001-10000000'
                          END) AS salary_group, salary
                  FROM employees)
           GROUP BY salary_group) USING(salary_group)
ORDER BY salary_group
-------------------------------------------------------------------------№11-------------------------------------------------------------------------
SELECT last_name, employee_id, salary
FROM employees
INNER JOIN (SELECT department_id, AVG(salary) AS avg_salary
            FROM employees
            GROUP BY department_id HAVING department_id IS NOT NULL) USING(department_id)
WHERE last_name IN (SELECT last_name
                    FROM employees
                    GROUP BY last_name HAVING COUNT(*) >= 2) AND salary > avg_salary
ORDER BY 1, 2, 3
-------------------------------------------------------------------------№12-------------------------------------------------------------------------
SELECT last_name, employee_id, salary
FROM employees
INNER JOIN (SELECT last_name, MAX(salary) AS max_salary
            FROM employees
            GROUP BY last_name) USING(last_name)
WHERE last_name IN (SELECT last_name
                    FROM employees
                    GROUP BY last_name HAVING COUNT(*) >= 2) AND salary = max_salary
ORDER BY 1, 2, 3
-------------------------------------------------------------------------№13-------------------------------------------------------------------------
SELECT employee_id
FROM employees
WHERE TRUNC(((SELECT SYSDATE FROM DUAL) - hire_date)) = (SELECT MAX(TRUNC(((SELECT SYSDATE FROM DUAL) - hire_date)))
                                                         FROM employees
                                                         WHERE employee_id IN (SELECT manager_id
                                                                               FROM (SELECT manager_id, COUNT(*) AS count
                                                                                     FROM employees
                                                                                     GROUP BY manager_id
                                                                                     ORDER BY count DESC)
                                                                               WHERE rownum <= 3))
AND employee_id IN (SELECT manager_id
                    FROM (SELECT manager_id, COUNT(*) AS count
                          FROM employees
                          GROUP BY manager_id
                          ORDER BY count DESC)
                    WHERE rownum <= 3)
ORDER BY employee_id
-------------------------------------------------------------------------№14-------------------------------------------------------------------------
SELECT manager_id
FROM employees
WHERE manager_id IN (SELECT *
                     FROM (SELECT employee_id
                           FROM employees
                           ORDER BY salary DESC)
                     WHERE rownum <= 3)
GROUP BY manager_id HAVING COUNT(*) = (SELECT MAX(count)
                                       FROM (SELECT COUNT(*) AS count
                                             FROM employees
                                             WHERE manager_id IN (SELECT *
                                                                  FROM (SELECT employee_id
                                                                        FROM employees
                                                                        ORDER BY salary DESC)
                                                                  WHERE rownum <= 3)
                                             GROUP BY manager_id))
ORDER BY manager_id
-------------------------------------------------------------------------№15-------------------------------------------------------------------------
SELECT emp_last_name, emp_first_name, emp_hire_date
FROM (SELECT last_name AS emp_last_name, first_name AS emp_first_name, hire_date AS emp_hire_date, salary AS emp_salary, manager_id
      FROM employees) El
INNER JOIN employees Er ON El.manager_id = Er.employee_id
WHERE ABS(emp_salary - salary) < 5000 AND emp_hire_date = (SELECT MIN(emp_hire_date)
                                                           FROM (SELECT hire_date AS emp_hire_date, salary AS emp_salary, manager_id
                                                                 FROM employees) El
                                                           INNER JOIN employees Er ON El.manager_id = Er.employee_id
                                                           WHERE ABS(emp_salary - salary) < 5000)
ORDER BY emp_hire_date
-------------------------------------------------------------------------№16-------------------------------------------------------------------------
SELECT department_id, employee_id
FROM (SELECT department_id, ROW_NUMBER() OVER(ORDER BY department_id) AS r_num
      FROM (SELECT department_id, SUM(salary) AS sum_salary
            FROM employees
            GROUP BY department_id HAVING department_id IS NOT NULL
            ORDER BY sum_salary) WHERE rownum <= 5) D,
     (SELECT employee_id, ROW_NUMBER() OVER(ORDER BY employee_id) AS r_num
      FROM (SELECT employee_id
            FROM employees
            ORDER BY salary DESC)
      WHERE rownum <= 5) E
WHERE D.r_num = E.r_num
ORDER BY department_id
