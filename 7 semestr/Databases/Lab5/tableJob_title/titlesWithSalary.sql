-- Вывести все названия должностей и соответствующие им минимальные
-- и максимальные зарплаты.
SELECT title,
	MIN(salary) AS min_salary,
	MAX(salary) AS max_salary
FROM job_title
	JOIN employee ON job_title.title = employee.job_title
GROUP BY title;