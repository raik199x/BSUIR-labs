-- Вывести имена сотрудников и среднюю зарплату для каждого сотрудника.
SELECT e.full_name,
	AVG(e.salary) AS average_salary
FROM employee e
GROUP BY e.full_name;