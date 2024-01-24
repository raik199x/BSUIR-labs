-- Вывести имена сотрудников и общее количество бронирований в фильмах,
-- где они работали.
SELECT e.full_name,
	COUNT(r.identification_number) AS reservation_count
FROM employee e
	JOIN cinema_hall ch ON e.full_name = ch.assigned_employee
	JOIN reservation r ON ch.hall_name = r.cinema_hall_name
GROUP BY e.full_name;