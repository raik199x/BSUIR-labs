-- Вывести названия фильмов и их жанры, где в жанрах есть "триллер".
SELECT film_name,
	genres
FROM film
WHERE 'Мистика' = ANY (genres);