-- Вывести названия фильмов и список жанров в одной строке для каждого фильма.
SELECT film_name,
	STRING_AGG(genre, ', ') AS genres
FROM (
		SELECT f.film_name,
			UNNEST(f.genres) AS genre
		FROM film f
	) AS film_genres
GROUP BY film_name;