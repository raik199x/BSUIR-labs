-- Вывести названия всех должностей и общую зарплату для каждой должности,
-- упорядоченные по убыванию общей зарплаты.
SELECT jt.title,
	SUM(e.salary) AS total_salary
FROM job_title jt
	JOIN employee e ON jt.title = e.job_title
GROUP BY jt.title
ORDER BY total_salary DESC;