-- Вывести названия фильмов и общее количество забронированных мест на этих фильмах.
SELECT f.film_name,
	SUM(r.column_number * r.sit_number) AS total_reserved_seats
FROM film f
	JOIN reservation r ON f.film_name = r.playing_film_name
GROUP BY f.film_name;