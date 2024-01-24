-- Вывести названия фильмов, у которых рейтинг выше 3.5 и жанр "драма".
SELECT film_name
FROM film
WHERE stars > 3.5
	AND 'Драмма' = ANY (genres);