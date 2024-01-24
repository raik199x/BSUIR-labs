-- Вывести названия залов и средний возраст актеров, снимавшихся в фильмах,
-- показываемых в каждом зале.
SELECT ch.hall_name,
	AVG(a.age) AS average_actor_age
FROM cinema_hall ch
	JOIN reservation r ON ch.hall_name = r.cinema_hall_name
	JOIN film_actor fa ON r.playing_film_name = fa.specific_film_name
	JOIN actors a ON fa.actor_name = a.full_name
GROUP BY ch.hall_name;