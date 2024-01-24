-- Вывести имена актеров и средний рейтинг фильмов, в которых они снимались.
SELECT a.full_name,
	AVG(f.stars) AS average_rating
FROM actors a
	JOIN film_actor fa ON a.full_name = fa.actor_name
	JOIN film f ON fa.specific_film_name = f.film_name
GROUP BY a.full_name;