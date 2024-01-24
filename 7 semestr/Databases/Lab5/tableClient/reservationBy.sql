-- Вывести имена клиентов и общее количество бронирований 
-- в залах с номером меньше 5.
SELECT c.full_name,
	COUNT(r.identification_number) AS reservation_count
FROM client c
	JOIN reservation r ON c.full_name = r.client_full_name
	JOIN cinema_hall ch ON r.cinema_hall_name = ch.hall_name
WHERE ch.hall_number < 2
GROUP BY c.full_name;