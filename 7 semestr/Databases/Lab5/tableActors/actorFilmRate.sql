-- Вывести имена актеров и их роли в фильмах с рейтингом выше 4.2.
SELECT actor_name,
	actor_role
FROM film_actor
WHERE specific_film_name IN (
		SELECT film_name
		FROM film
		WHERE stars > 4.2
	);