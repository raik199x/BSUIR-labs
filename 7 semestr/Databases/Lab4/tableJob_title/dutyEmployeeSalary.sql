-- Вывести названия должностей и имена сотрудников, у которых зарплата выше 2000.
SELECT j.title,
	e.full_name
FROM job_title j
	JOIN employee e ON j.title = e.job_title
WHERE e.salary > 2000;