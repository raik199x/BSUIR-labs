-- Вывести названия фильмов и рейтинг, где рейтинг выше 6.5
-- и фильм не является комедией.
SELECT film_name,
	stars
FROM film
WHERE stars > 4.2
	AND 'Драмма' != ALL (genres);