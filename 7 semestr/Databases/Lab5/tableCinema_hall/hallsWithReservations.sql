-- Вывести названия залов и количество бронирований, сделанных в каждом зале.
SELECT ch.hall_name,
	COUNT(r.identification_number) AS reservation_count
FROM cinema_hall ch
	LEFT JOIN reservation r ON ch.hall_name = r.cinema_hall_name
GROUP BY ch.hall_name;