-- Вывести имена актеров, участвовавших в фильмах с жанром "Приключение" и рейтингом выше 4.0.
SELECT DISTINCT a.full_name
FROM actors a
	JOIN film_actor fa ON a.full_name = fa.actor_name
	JOIN film f ON fa.specific_film_name = f.film_name
WHERE 'Приключение' = ANY (f.genres)
	AND f.stars > 4.0;