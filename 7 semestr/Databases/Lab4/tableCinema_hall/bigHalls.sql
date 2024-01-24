-- Вывести названия залов и общее количество мест в каждом зале, где общее количество мест больше 50.
SELECT hall_name,
	total_number_of_seats
FROM cinema_hall
WHERE total_number_of_seats > 120;