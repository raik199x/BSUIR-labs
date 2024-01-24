-- Вывести имена и должности сотрудников,
-- работающих в залах с номером больше 2.
SELECT e.full_name,
	e.job_title
FROM employee e
	JOIN cinema_hall ch ON e.full_name = ch.assigned_employee
WHERE ch.hall_number > 2;