-- Вывести названия должностей и среднюю зарплату для каждой должности.
SELECT title, AVG(salary) AS average_salary
FROM job_title
JOIN employee ON job_title.title = employee.job_title
GROUP BY title;