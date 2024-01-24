-- Вывести имена актеров, снимавшихся в фильмах жанра "Комедия".
SELECT DISTINCT a.full_name
FROM actors a
	JOIN film_actor fa ON a.full_name = fa.actor_name
	JOIN film f ON fa.specific_film_name = f.film_name
WHERE 'Комедия' = ANY (f.genres);