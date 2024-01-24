-- Вывести имена сотрудников и минимальную зарплату для каждой должности.
SELECT e.full_name,
	MIN(e.salary) AS min_salary
FROM employee e
GROUP BY e.full_name;