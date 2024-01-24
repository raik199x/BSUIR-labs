-- Вывести все залы кинотеатра и количество сотрудников,
-- работающих в каждом зале.
SELECT ch.hall_name,
	COUNT(e.full_name) AS employee_count
FROM cinema_hall ch
	LEFT JOIN employee e ON ch.assigned_employee = e.full_name
GROUP BY ch.hall_name;