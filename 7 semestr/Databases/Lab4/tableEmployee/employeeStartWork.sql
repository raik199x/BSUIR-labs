-- Вывести имена и зарплаты сотрудников, у которых рабочее время начинается после 10:00 утра.
SELECT full_name,
	salary
FROM employee
WHERE start_time > '10:00:00';