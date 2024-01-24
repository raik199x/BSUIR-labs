-- Вывести имена клиентов и количество бронирований,
-- сделанных каждым клиентом.
SELECT c.full_name,
	COUNT(r.identification_number) AS reservation_count
FROM client c
	LEFT JOIN reservation r ON c.full_name = r.client_full_name
GROUP BY c.full_name;