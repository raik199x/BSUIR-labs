-- Вывести имена сотрудников, у которых зарплата выше средней зарплаты.
SELECT full_name
FROM employee
WHERE salary > (
		SELECT AVG(salary)
		FROM employee
	);