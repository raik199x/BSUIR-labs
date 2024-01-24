-- Вывести имена и должности сотрудников,
-- упорядоченные по алфавиту и затем по должности.
SELECT full_name,
	job_title
FROM employee
ORDER BY full_name,
	job_title;