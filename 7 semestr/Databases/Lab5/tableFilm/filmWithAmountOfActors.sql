-- Вывести названия фильмов и количество актеров, 
-- участвовавших в каждом фильме.
SELECT f.film_name,
	COUNT(fa.actor_name) AS actor_count
FROM film f
	LEFT JOIN film_actor fa ON f.film_name = fa.specific_film_name
GROUP BY f.film_name;