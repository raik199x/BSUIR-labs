-- Вывести имена актеров и количество фильмов,
-- в которых они участвовали, упорядоченные по убыванию количества фильмов.
SELECT a.full_name,
	COUNT(fa.specific_film_name) AS film_count
FROM actors a
	LEFT JOIN film_actor fa ON a.full_name = fa.actor_name
GROUP BY a.full_name
ORDER BY film_count DESC;