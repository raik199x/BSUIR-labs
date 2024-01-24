-- Вывести названия всех должностей
-- и количество сотрудников на каждой должности.
SELECT j.title,
	COUNT(e.full_name) AS employee_count
FROM job_title j
	JOIN employee e ON j.title = e.job_title
GROUP BY j.title;