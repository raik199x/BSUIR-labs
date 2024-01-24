-- Вывести названия залов и количество бронирований в каждом зале,
-- где количество бронирований больше 0.
SELECT ch.hall_name,
	COUNT(r.identification_number) AS reservation_count
FROM cinema_hall ch
	LEFT JOIN reservation r ON ch.hall_name = r.cinema_hall_name
GROUP BY ch.hall_name
HAVING COUNT(r.identification_number) > 0;