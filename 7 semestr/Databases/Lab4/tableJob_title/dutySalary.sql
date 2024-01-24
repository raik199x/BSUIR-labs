-- Вывести названия должностей и зарплаты для каждой должности.
SELECT j.title,
	e.salary
FROM job_title j
	JOIN employee e ON j.title = e.job_title;