-- Вывести названия залов и номера залов, в которых работают сотрудники с зарплатой выше 1300.
SELECT hall_name,
	hall_number
FROM cinema_hall ch
	JOIN employee e ON ch.assigned_employee = e.full_name
WHERE e.salary > 1300;