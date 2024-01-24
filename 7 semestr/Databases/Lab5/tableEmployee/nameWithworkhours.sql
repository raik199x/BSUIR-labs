-- Вывести имена сотрудников и общее количество часов работы, упорядоченные по убыванию общего количества часов.
SELECT e.full_name,
	SUM(
		EXTRACT(
			HOUR
			FROM (e.end_time - e.start_time)
		)
	) AS total_hours
FROM employee e
GROUP BY e.full_name
ORDER BY total_hours DESC;