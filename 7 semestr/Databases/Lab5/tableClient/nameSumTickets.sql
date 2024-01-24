-- Вывести имена клиентов и общую сумму билетов, которые они приобрели.
SELECT c.full_name,
	SUM(f.stars) AS total_ticket_cost
FROM client c
	JOIN reservation r ON c.full_name = r.client_full_name
	JOIN film f ON r.playing_film_name = f.film_name
GROUP BY c.full_name;