-- Вывести имена сотрудников и количество залов, в которых они работают.
SELECT e.full_name,
	COUNT(ch.hall_name) AS hall_count
FROM employee e
	LEFT JOIN cinema_hall ch ON e.full_name = ch.assigned_employee
GROUP BY e.full_name;