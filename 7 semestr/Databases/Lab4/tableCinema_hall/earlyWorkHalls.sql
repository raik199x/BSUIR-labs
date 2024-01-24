-- Вывести названия залов и имена сотрудников, у которых начальное время работы раньше 9:00.
SELECT ch.hall_name,
	e.full_name
FROM cinema_hall ch
	JOIN employee e ON ch.assigned_employee = e.full_name
WHERE e.start_time < '9:00:00';