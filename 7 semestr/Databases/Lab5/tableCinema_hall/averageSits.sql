-- Вывести названия залов и среднее количество мест в каждом зале.
SELECT hall_name,
	AVG(total_number_of_seats) AS average_seats
FROM cinema_hall
GROUP BY hall_name;