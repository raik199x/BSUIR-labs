-- Вывести средний возраст актеров и минимальный рейтинг фильма, в котором они участвовали.
SELECT AVG(a.age) AS average_age,
	MIN(f.stars) AS min_film_rating
FROM actors a
	JOIN film_actor fa ON a.full_name = fa.actor_name
	JOIN film f ON fa.specific_film_name = f.film_name;