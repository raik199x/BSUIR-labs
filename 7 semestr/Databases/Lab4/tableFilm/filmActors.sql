-- Вывести названия фильмов, имена актеров, и их роли в фильмах.
SELECT f.film_name,
	fa.actor_name,
	fa.actor_role
FROM film f
	JOIN film_actor fa ON f.film_name = fa.specific_film_name;