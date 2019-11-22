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
-------------------------------------------------------------------------№3-------------------------------------------------------------------------
SELECT MONTHS_BETWEEN((SELECT MAX(hire_date) FROM employees), (SELECT MIN(hire_date) FROM employees)) FROM DUAL
-------------------------------------------------------------------------№4-------------------------------------------------------------------------
SELECT DISTINCT last_name, MONTHS_BETWEEN((SELECT MAX(hire_date) FROM employees WHERE employees.last_name = emp.last_name), (SELECT MIN(hire_date) FROM employees WHERE employees.last_name = emp.last_name)) AS months
FROM employees emp
WHERE last_name IN (SELECT last_name FROM employees GROUP BY last_name HAVING COUNT(*) > 1)
ORDER BY months
-------------------------------------------------------------------------№5-------------------------------------------------------------------------
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
