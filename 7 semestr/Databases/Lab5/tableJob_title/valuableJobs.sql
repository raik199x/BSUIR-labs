-- Вывести названия должностей и количество сотрудников, у которых зарплата выше средней зарплаты по всем должностям.
SELECT jt.title,
	COUNT(e.full_name) AS high_salary_employee_count
FROM job_title jt
	JOIN employee e ON jt.title = e.job_title
WHERE e.salary > (
		SELECT AVG(salary)
		FROM employee
	)
GROUP BY jt.title;