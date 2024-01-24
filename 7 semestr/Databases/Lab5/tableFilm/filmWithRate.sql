-- Вывести названия фильмов и средний рейтинг по каждому фильму.
SELECT film_name,
	AVG(stars) AS average_rating
FROM film
GROUP BY film_name;