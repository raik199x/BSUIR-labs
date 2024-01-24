-- Вывести названия фильмов и количество уникальных жанров в каждом фильме.
SELECT film_name,
	COUNT(DISTINCT genre) AS unique_genre_count
FROM (
		SELECT f.film_name,
			UNNEST(f.genres) AS genre
		FROM film f
	) AS film_genres
GROUP BY film_name;