-- Вывести названия должностей и обязанности,
-- где обязанности содержат слово "управление".
SELECT title,
	duties
FROM job_title
WHERE duties ILIKE '%управление%';